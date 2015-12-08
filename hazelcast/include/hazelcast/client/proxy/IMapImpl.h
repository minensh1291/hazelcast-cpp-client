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
#ifndef HAZELCAST_IMAP_IMPL
#define HAZELCAST_IMAP_IMPL

#include "hazelcast/client/protocol/codec/MapExecuteOnKeyCodec.h"
#include "hazelcast/util/Util.h"
#include "hazelcast/client/protocol/codec/MapExecuteOnAllKeysCodec.h"
#include "hazelcast/client/proxy/ProxyImpl.h"
#include "hazelcast/client/map/DataEntryView.h"

namespace hazelcast {
    namespace client {
        namespace serialization {
            class IdentifiedDataSerializable;
        }
        namespace proxy {
            class HAZELCAST_API IMapImpl : public ProxyImpl {
            protected:
                IMapImpl(const std::string& instanceName, spi::ClientContext *context);

                bool containsKey(const serialization::pimpl::Data& key);

                bool containsValue(const serialization::pimpl::Data& value);

                std::auto_ptr<serialization::pimpl::Data> get(const serialization::pimpl::Data& key);

                std::auto_ptr<serialization::pimpl::Data> put(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value);

                std::auto_ptr<serialization::pimpl::Data> remove(const serialization::pimpl::Data& key);

                bool remove(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value);

                void deleteEntry(const serialization::pimpl::Data& key);

                void flush();

                bool tryRemove(const serialization::pimpl::Data& key, long timeoutInMillis);

                bool tryPut(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long timeoutInMillis);

                std::auto_ptr<serialization::pimpl::Data> put(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttlInMillis);

                void putTransient(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttlInMillis);

                std::auto_ptr<serialization::pimpl::Data> putIfAbsent(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttlInMillis);

                bool replace(const serialization::pimpl::Data& key, const serialization::pimpl::Data& oldValue, const serialization::pimpl::Data& newValue);

                std::auto_ptr<serialization::pimpl::Data> replace(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value);

                void set(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttl);

                void lock(const serialization::pimpl::Data& key);

                void lock(const serialization::pimpl::Data& key, long leaseTime);

                bool isLocked(const serialization::pimpl::Data& key);

                bool tryLock(const serialization::pimpl::Data& key, long timeInMillis);

                void unlock(const serialization::pimpl::Data& key);

                void forceUnlock(const serialization::pimpl::Data& key);

                std::string addInterceptor(serialization::Portable &interceptor);

                std::string addInterceptor(serialization::IdentifiedDataSerializable &interceptor);

                void removeInterceptor(const std::string& id);

                std::string addEntryListener(impl::BaseEventHandler *entryEventHandler, bool includeValue);

                bool removeEntryListener(const std::string& registrationId);

                std::string addEntryListener(impl::BaseEventHandler *entryEventHandler, const serialization::pimpl::Data& key, bool includeValue);

                std::auto_ptr<map::DataEntryView> getEntryView(const serialization::pimpl::Data& key);

                bool evict(const serialization::pimpl::Data& key);

                void evictAll();

                EntryVector getAll(const std::vector<serialization::pimpl::Data>& keys);

                std::vector<serialization::pimpl::Data> keySet();

                std::vector<serialization::pimpl::Data> keySet(const serialization::IdentifiedDataSerializable &predicate);

                EntryVector entrySet();

                EntryVector entrySet(const serialization::IdentifiedDataSerializable &predicate);

                std::vector<serialization::pimpl::Data> values();

                std::vector<serialization::pimpl::Data> values(const serialization::IdentifiedDataSerializable &predicate);

                void addIndex(const std::string& attribute, bool ordered);

                int size();

                bool isEmpty();

                void putAll(const EntryVector& entries);

                void clear();

                template<typename KEY, typename RESULT, typename ENTRYPROCESSOR>
                boost::shared_ptr<RESULT> executeOnKey(const KEY& key, ENTRYPROCESSOR &entryProcessor) {
                    serialization::pimpl::Data keyData = toData(key);
                    serialization::pimpl::Data processor = toData(entryProcessor); 
                    int partitionId = getPartitionId(keyData);

                    std::auto_ptr<protocol::ClientMessage> request = protocol::codec::MapExecuteOnKeyCodec::RequestParameters::encode(getName(), processor, keyData, util::getThreadId());

                    std::auto_ptr<serialization::pimpl::Data> response = invokeAndGetResult<std::auto_ptr<serialization::pimpl::Data>, protocol::codec::MapExecuteOnKeyCodec::ResponseParameters>(request, partitionId);

                    return toObject<RESULT>(response);
                }

                template<typename KEY, typename RESULT, typename ENTRYPROCESSOR>
                std::map<KEY, boost::shared_ptr<RESULT> > executeOnEntries(ENTRYPROCESSOR &entryProcessor) {
                    serialization::pimpl::Data processor = toData(entryProcessor);

                    std::auto_ptr<protocol::ClientMessage> request = protocol::codec::MapExecuteOnAllKeysCodec::RequestParameters::encode(getName(), processor);

                    std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> > response =
                            invokeAndGetResult<std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >,
                                    protocol::codec::MapExecuteOnAllKeysCodec::ResponseParameters>(request);

                    std::map<KEY, boost::shared_ptr<RESULT> > results;
                    for (std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >::const_iterator it = response.begin();
                            response.end() != it;  ++it) {
                        KEY key = *toObject<KEY>(it->first);
                        boost::shared_ptr<RESULT> value = toObject<RESULT>(it->second);
                        results[key] = value;
                    }

                    return results;
                }
            };
        }

    }
}

#endif /* HAZELCAST_IMAP_IMPL */

