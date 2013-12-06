//
// Created by sancar koyunlu on 9/18/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_ClientTxnListTest
#define HAZELCAST_ClientTxnListTest


#include "iTest/iTest.h"
#include "hazelcast/client/ClientConfig.h"
#include "HazelcastInstance.h"

namespace hazelcast {
    namespace client {

        class HazelcastClient;

        namespace test {

            class HazelcastInstanceFactory;

            class HAZELCAST_API ClientTxnListTest : public iTest::iTestFixture<ClientTxnListTest> {

            public:

                ClientTxnListTest(HazelcastInstanceFactory&);

                ~ClientTxnListTest();

                void addTests();

                void beforeClass();

                void afterClass();

                void beforeTest();

                void afterTest();

                void testAddRemove();


            private:
                HazelcastInstanceFactory& hazelcastInstanceFactory;
                HazelcastInstance instance;
                ClientConfig clientConfig;
                std::auto_ptr<HazelcastClient> client;
            };

        }
    }
}

#endif //HAZELCAST_ClientTxnListTest
