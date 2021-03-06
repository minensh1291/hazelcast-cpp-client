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
//
// Created by ihsan demir on 9/9/15.
// Copyright (c) 2015 hazelcast. All rights reserved.


#include "hazelcast/client/impl/BaseEventHandler.h"
#include "hazelcast/client/protocol/ClientMessage.h"

namespace hazelcast {
    namespace client {
        namespace impl {
            BaseEventHandler::~BaseEventHandler(){
            }

            void BaseEventHandler::handle(const boost::shared_ptr<protocol::ClientMessage> &event) {
                std::auto_ptr<protocol::ClientMessage> e(new protocol::ClientMessage(*event));
                handle(e);
            }

            BaseEventHandler::BaseEventHandler() : logger(NULL) {}

            void BaseEventHandler::setLogger(util::ILogger *logger) {
                BaseEventHandler::logger = logger;
            }

            util::ILogger *BaseEventHandler::getLogger() const {
                return logger;
            }
        }
    }
}
