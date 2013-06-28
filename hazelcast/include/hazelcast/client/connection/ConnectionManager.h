//
// Created by sancar koyunlu on 5/21/13.
// Copyright (c) 2013 sancar koyunlu. All rights reserved.
#ifndef HAZELCAST_CONNECTION_MANAGER
#define HAZELCAST_CONNECTION_MANAGER

#include "../../util/ConcurrentMap.h"
#include "HeartBeatChecker.h"
#include "ConnectionPool.h"

namespace hazelcast {
    namespace client {

        namespace serialization {
            class SerializationService;
        }

        namespace protocol {
            class Principal;

            class Credentials;
        }

        namespace spi {
            class ClusterService;
        }

        class MembershipListener;

        class ClientConfig;

        namespace connection {
            class Connection;

            class ConnectionManager {
            public:

                ConnectionManager(spi::ClusterService& clusterService, serialization::SerializationService&, ClientConfig&);

                ~ConnectionManager();

                Connection *newConnection(const Address& address);

                Connection *getRandomConnection();

                Connection *getConnection(const Address& address);

                void releaseConnection(Connection *connection);

                ConnectionPool *getConnectionPool(const Address& address);

                void removeConnectionPool(const Address& address);

                void authenticate(Connection& connection, bool reAuth, bool firstConnection);

                void shutdown();

            private:
                util::ConcurrentMap<Address, ConnectionPool > poolMap;
                spi::ClusterService& clusterService;
                serialization::SerializationService& serializationService;
                ClientConfig& clientConfig;
                protocol::Principal *principal;
                HeartBeatChecker heartBeatChecker;
                volatile bool live;

                void checkLive();

            };
        }
    }
}

#endif //HAZELCAST_CONNECTION_MANAGER
