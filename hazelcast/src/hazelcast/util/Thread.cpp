//
// Created by sancar koyunlu on 5/28/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/util/Thread.h"


namespace hazelcast {
    namespace util {
        Thread::Thread(void *(*runnable)(void *))
        : attributes(NULL)
        , runnable(runnable)
        , parameters(NULL) {
        };

        Thread::Thread(void *(*runnable)(void *), void *parameters)
        : attributes(NULL)
        , runnable(runnable)
        , parameters(parameters) {
        };

        Thread::Thread(void *(*runnable)(void *), pthread_attr_t const *attributes, void *parameters)
        : attributes(attributes)
        , runnable(runnable)
        , parameters(parameters) {
        };

        void Thread::start() {
            pthread_create(&thread, attributes, runnable, parameters);
        };

        bool Thread::join(useconds_t timeout) {
            useconds_t sleepInterval = 1000 * 100; //100ms
            while (timeout > 0) {
                usleep(sleepInterval);
                if (ESRCH == pthread_kill(thread, 0)) {
                    //Thread not found, means it is finished
                    return true;
                }
                timeout -= sleepInterval;
            }
            pthread_cancel(thread);
            return false;
        };

        void Thread::join() {
            pthread_join(thread, NULL);
        };


        void Thread::detach() {
            pthread_detach(thread);
        };

        Thread::Thread(Thread const & param) {

        };

        Thread & Thread::operator = (Thread const & param) {
            return (*this);
        };


    }
}