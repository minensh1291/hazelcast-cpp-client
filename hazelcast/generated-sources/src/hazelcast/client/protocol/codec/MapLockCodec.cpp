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

#include "hazelcast/util/Util.h"
#include "hazelcast/util/ILogger.h"

#include "hazelcast/client/protocol/codec/MapLockCodec.h"
#include "hazelcast/client/serialization/pimpl/Data.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                const MapMessageType MapLockCodec::REQUEST_TYPE = HZ_MAP_LOCK;
                const bool MapLockCodec::RETRYABLE = true;
                const ResponseMessageConst MapLockCodec::RESPONSE_TYPE = (ResponseMessageConst) 100;

                std::auto_ptr<ClientMessage> MapLockCodec::encodeRequest(
                        const std::string &name,
                        const serialization::pimpl::Data &key,
                        int64_t threadId,
                        int64_t ttl,
                        int64_t referenceId) {
                    int32_t requiredDataSize = calculateDataSize(name, key, threadId, ttl, referenceId);
                    std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                    clientMessage->setMessageType((uint16_t) MapLockCodec::REQUEST_TYPE);
                    clientMessage->setRetryable(RETRYABLE);
                    clientMessage->set(name);
                    clientMessage->set(key);
                    clientMessage->set(threadId);
                    clientMessage->set(ttl);
                    clientMessage->set(referenceId);
                    clientMessage->updateFrameLength();
                    return clientMessage;
                }

                int32_t MapLockCodec::calculateDataSize(
                        const std::string &name,
                        const serialization::pimpl::Data &key,
                        int64_t threadId,
                        int64_t ttl,
                        int64_t referenceId) {
                    int32_t dataSize = ClientMessage::HEADER_SIZE;
                    dataSize += ClientMessage::calculateDataSize(name);
                    dataSize += ClientMessage::calculateDataSize(key);
                    dataSize += ClientMessage::calculateDataSize(threadId);
                    dataSize += ClientMessage::calculateDataSize(ttl);
                    dataSize += ClientMessage::calculateDataSize(referenceId);
                    return dataSize;
                }


            }
        }
    }
}

