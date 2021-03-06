/*
 * Copyright (c) 2008-2018, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <memory>
#include <boost/foreach.hpp>

#include "hazelcast/client/spi/impl/AbstractClientInvocationService.h"
#include "hazelcast/client/ClientConfig.h"
#include "hazelcast/client/protocol/ClientExceptionFactory.h"
#include "hazelcast/client/protocol/codec/ErrorCodec.h"
#include "hazelcast/client/spi/impl/ClientExecutionServiceImpl.h"

namespace hazelcast {
    namespace client {
        namespace spi {
            namespace impl {
                AbstractClientInvocationService::AbstractClientInvocationService(ClientContext &client)
                        : CLEAN_RESOURCES_MILLIS(client.getClientProperties().getCleanResourcesPeriodMillis()),
                          client(client), invocationLogger(client.getLogger()),
                          connectionManager(NULL),
                          partitionService(client.getPartitionService()),
                          clientListenerService(NULL),
                          invocationTimeoutMillis(
                                  client.getClientProperties().getInvocationTimeoutSeconds().getInteger() * 1000),
                          invocationRetryPauseMillis(
                                  client.getClientProperties().getInvocationRetryPauseMillis().getLong()),
                          responseThread(client.getName() + ".response-", invocationLogger, *this, client) {
                }

                bool AbstractClientInvocationService::start() {
                    connectionManager = &client.getConnectionManager();
                    clientListenerService = static_cast<listener::AbstractClientListenerService *>(&client.getClientListenerService());

                    responseThread.start();

                    int64_t cleanResourcesMillis = CLEAN_RESOURCES_MILLIS.getLong();
                    if (cleanResourcesMillis <= 0) {
                        cleanResourcesMillis = util::IOUtil::to_value<int64_t>(
                                CLEAN_RESOURCES_MILLIS.getDefaultValue());
                    }

                    client.getClientExecutionService().scheduleWithRepetition(boost::shared_ptr<util::Runnable>(
                            new CleanResourcesTask(invocations)), cleanResourcesMillis, cleanResourcesMillis);

                    return true;
                }

                void AbstractClientInvocationService::shutdown() {
                    isShutdown = true;

                    responseThread.interrupt();

                    typedef std::vector<std::pair<int64_t, boost::shared_ptr<ClientInvocation> > > InvocationEntriesVector;
                    InvocationEntriesVector allEntries = invocations.clear();
                    boost::shared_ptr<exception::HazelcastClientNotActiveException> notActiveException(
                            new exception::HazelcastClientNotActiveException("AbstractClientInvocationService::shutdown",
                                    "Client is shutting down"));
                    BOOST_FOREACH (InvocationEntriesVector::value_type & entry, allEntries) {
                                    entry.second->notifyException(notActiveException);
                                }
                }

                int64_t AbstractClientInvocationService::getInvocationTimeoutMillis() const {
                    return invocationTimeoutMillis;
                }

                int64_t AbstractClientInvocationService::getInvocationRetryPauseMillis() const {
                    return invocationRetryPauseMillis;
                }

                bool AbstractClientInvocationService::isRedoOperation() {
                    return client.getClientConfig().isRedoOperation();
                }

                void AbstractClientInvocationService::handleClientMessage(
                        const boost::shared_ptr<connection::Connection> &connection,
                        const boost::shared_ptr<protocol::ClientMessage> &clientMessage) {
                    responseThread.responseQueue.push(clientMessage);
                }

                boost::shared_ptr<ClientInvocation> AbstractClientInvocationService::deRegisterCallId(int64_t callId) {
                    return invocations.remove(callId);
                }

                void AbstractClientInvocationService::send(boost::shared_ptr<impl::ClientInvocation> invocation,
                                                           boost::shared_ptr<connection::Connection> connection) {
                    if (isShutdown) {
                        throw exception::HazelcastClientNotActiveException("AbstractClientInvocationService::send",
                                                                           "Client is shut down");
                    }
                    registerInvocation(invocation);

                    const boost::shared_ptr<protocol::ClientMessage> &clientMessage = invocation->getClientMessage();
                    if (!writeToConnection(*connection, clientMessage)) {
                        int64_t callId = clientMessage->getCorrelationId();
                        boost::shared_ptr<ClientInvocation> clientInvocation = deRegisterCallId(callId);
                        if (clientInvocation.get() != NULL) {
                            std::ostringstream out;
                            out << "Packet not sent to ";
                            if (connection->getRemoteEndpoint().get()) {
                                out << *connection->getRemoteEndpoint();
                            } else {
                                out << "null";
                            }
                            throw exception::IOException("AbstractClientInvocationService::send", out.str());
                        } else {
                            if (invocationLogger.isFinestEnabled()) {
                                invocationLogger.finest() << "Invocation not found to deregister for call ID "
                                                          << callId;
                            }
                            return;
                        }
                    }

                    invocation->setSendConnection(connection);
                }

                void AbstractClientInvocationService::registerInvocation(
                        const boost::shared_ptr<ClientInvocation> &clientInvocation) {
                    const boost::shared_ptr<protocol::ClientMessage> &clientMessage = clientInvocation->getClientMessage();
                    int64_t correlationId = clientMessage->getCorrelationId();
                    invocations.put(correlationId, clientInvocation);
                    const boost::shared_ptr<EventHandler<protocol::ClientMessage> > handler = clientInvocation->getEventHandler();
                    if (handler.get() != NULL) {
                        clientListenerService->addEventHandler(correlationId, handler);
                    }
                }

                bool AbstractClientInvocationService::writeToConnection(connection::Connection &connection,
                                                                        const boost::shared_ptr<protocol::ClientMessage> &clientMessage) {
                    clientMessage->addFlag(protocol::ClientMessage::BEGIN_AND_END_FLAGS);
                    return connection.write(clientMessage);
                }

                void AbstractClientInvocationService::CleanResourcesTask::run() {
                    std::vector<int64_t> invocationsToBeRemoved;
                    typedef std::vector<std::pair<int64_t, boost::shared_ptr<ClientInvocation> > > INVOCATION_ENTRIES;
                    BOOST_FOREACH(const INVOCATION_ENTRIES::value_type &entry, invocations.entrySet()) {
                                    int64_t key = entry.first;
                                    const boost::shared_ptr<ClientInvocation> &invocation = entry.second;
                                    boost::shared_ptr<connection::Connection> connection = invocation->getSendConnection();
                                    if (!connection.get()) {
                                        continue;
                                    }

                                    if (connection->isAlive()) {
                                        continue;
                                    }

                                    invocationsToBeRemoved.push_back(key);

                                    notifyException(*invocation, connection);
                                }

                    BOOST_FOREACH(int64_t invocationId, invocationsToBeRemoved) {
                                    invocations.remove(invocationId);
                                }
                }

                void AbstractClientInvocationService::CleanResourcesTask::notifyException(ClientInvocation &invocation,
                                                                                          boost::shared_ptr<connection::Connection> &connection) {
                    boost::shared_ptr<exception::IException> ex(
                            new exception::TargetDisconnectedException("CleanResourcesTask::notifyException",
                                                                       connection->getCloseReason()));
                    invocation.notifyException(ex);
                }

                AbstractClientInvocationService::CleanResourcesTask::CleanResourcesTask(
                        util::SynchronizedMap<int64_t, ClientInvocation> &invocations) : invocations(invocations) {}

                const std::string AbstractClientInvocationService::CleanResourcesTask::getName() const {
                    return "AbstractClientInvocationService::CleanResourcesTask";
                }

                AbstractClientInvocationService::~AbstractClientInvocationService() {
                }

                AbstractClientInvocationService::ResponseThread::ResponseThread(const std::string &name,
                                                                                util::ILogger &invocationLogger,
                                                                                AbstractClientInvocationService &invocationService,
                                                                                ClientContext &clientContext)
                        : responseQueue(100000), invocationLogger(invocationLogger),
                          invocationService(invocationService), client(clientContext),
                          worker(boost::shared_ptr<util::Runnable>(new util::RunnableDelegator(*this)), invocationLogger) {
                }

                void AbstractClientInvocationService::ResponseThread::run() {
                    try {
                        doRun();
                    } catch (exception::IException &t) {
                        invocationLogger.severe() << t;
                    }
                }

                void AbstractClientInvocationService::ResponseThread::doRun() {
                    while (!invocationService.isShutdown) {
                        boost::shared_ptr<protocol::ClientMessage> task;
                        try {
                            task = responseQueue.pop();
                        } catch (exception::InterruptedException &) {
                            continue;
                        }
                        process(task);
                    }
                }

                void AbstractClientInvocationService::ResponseThread::process(
                        const boost::shared_ptr<protocol::ClientMessage> &clientMessage) {
                    try {
                        handleClientMessage(clientMessage);
                    } catch (exception::IException &e) {
                        invocationLogger.severe() << "Failed to process task: " << clientMessage
                                                  << " on responseThread: "
                                                  << getName() << e;
                    }
                }

                void AbstractClientInvocationService::ResponseThread::handleClientMessage(
                        const boost::shared_ptr<protocol::ClientMessage> &clientMessage) {
                    int64_t correlationId = clientMessage->getCorrelationId();

                    boost::shared_ptr<ClientInvocation> future = invocationService.deRegisterCallId(correlationId);
                    if (future.get() == NULL) {
                        invocationLogger.warning() << "No call for callId: " << correlationId << ", response: "
                                                   << *clientMessage;
                        return;
                    }
                    if (protocol::codec::ErrorCodec::TYPE == clientMessage->getMessageType()) {
                        boost::shared_ptr<exception::IException> exception(client.getClientExceptionFactory().createException(
                                "AbstractClientInvocationService::ResponseThread::handleClientMessage", *clientMessage));
                        future->notifyException(exception);
                    } else {
                        future->notify(clientMessage);
                    }
                }

                void AbstractClientInvocationService::ResponseThread::interrupt() {
                    responseQueue.interrupt();
                }

                void AbstractClientInvocationService::ResponseThread::start() {
                    worker.start();
                }

                const std::string AbstractClientInvocationService::ResponseThread::getName() const {
                    return "AbstractClientInvocationService::ResponseThread";
                }

                AbstractClientInvocationService::ResponseThread::~ResponseThread() {
                }
            }
        }
    }
}
