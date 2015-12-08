/*
 * Copyright (c) 2008-2015, Hazelcast, Inc. All Rights Reserved.
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



#include "hazelcast/client/protocol/codec/MapClearNearCacheCodec.h"
#include "hazelcast/client/exception/UnexpectedMessageTypeException.h"
#include "hazelcast/client/Address.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                const MapMessageType MapClearNearCacheCodec::RequestParameters::TYPE = HZ_MAP_CLEARNEARCACHE;
                const bool MapClearNearCacheCodec::RequestParameters::RETRYABLE = false;
                const int32_t MapClearNearCacheCodec::ResponseParameters::TYPE = 100;
                std::auto_ptr<ClientMessage> MapClearNearCacheCodec::RequestParameters::encode(
                        const std::string &name, 
                        const Address &target) {
                    int32_t requiredDataSize = calculateDataSize(name, target);
                    std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                    clientMessage->setMessageType((uint16_t)MapClearNearCacheCodec::RequestParameters::TYPE);
                    clientMessage->setRetryable(RETRYABLE);
                    clientMessage->set(name);
                    clientMessage->set(target);
                    clientMessage->updateFrameLength();
                    return clientMessage;
                }

                int32_t MapClearNearCacheCodec::RequestParameters::calculateDataSize(
                        const std::string &name, 
                        const Address &target) {
                    int32_t dataSize = ClientMessage::HEADER_SIZE;
                    dataSize += ClientMessage::calculateDataSize(name);
                    dataSize += ClientMessage::calculateDataSize(target);
                    return dataSize;
                }

                MapClearNearCacheCodec::ResponseParameters::ResponseParameters(ClientMessage &clientMessage) {
                    if (TYPE != clientMessage.getMessageType()) {
                        throw exception::UnexpectedMessageTypeException("MapClearNearCacheCodec::ResponseParameters::decode", clientMessage.getMessageType(), TYPE);
                    }
                }

                MapClearNearCacheCodec::ResponseParameters MapClearNearCacheCodec::ResponseParameters::decode(ClientMessage &clientMessage) {
                    return MapClearNearCacheCodec::ResponseParameters(clientMessage);
                }

                MapClearNearCacheCodec::ResponseParameters::ResponseParameters(const MapClearNearCacheCodec::ResponseParameters &rhs) {
                }
                //************************ EVENTS END **************************************************************************//

            }
        }
    }
}

