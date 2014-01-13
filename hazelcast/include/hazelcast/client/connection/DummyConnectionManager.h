//
// Created by sancar koyunlu on 8/19/13.
// Copyright (c) 2013 hazelcast. All rights reserved.




#ifndef HAZELCAST_DummyConnectionManager
#define HAZELCAST_DummyConnectionManager

#include "hazelcast/client/connection/ConnectionManager.h"
#include "hazelcast/client/Address.h"


namespace hazelcast {
    namespace client {
        class Address;
        namespace connection {

            class HAZELCAST_API DummyConnectionManager : public ConnectionManager {
            public:
                DummyConnectionManager(spi::ClientContext& clientContext);

//                virtual boost::shared_ptr<Connection>firstConnection(const Address &address);

//                virtual boost::shared_ptr<Connection>getConnection(Address const &address);

            private:
                Address address;
                bool isAddressAvailable;
            };
        }
    }
}


#endif //HAZELCAST_DummyConnectionManager
