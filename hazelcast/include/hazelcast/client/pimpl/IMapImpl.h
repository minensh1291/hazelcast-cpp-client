#ifndef HAZELCAST_IMAP
#define HAZELCAST_IMAP

#include "hazelcast/client/exception/IException.h"
#include "hazelcast/client/serialization/pimpl/Data.h"
#include "hazelcast/client/spi/ClientContext.h"
#include "hazelcast/client/spi/InvocationService.h"
#include "hazelcast/client/spi/ServerListenerService.h"
#include "hazelcast/client/impl/MapKeySet.h"
#include "hazelcast/client/impl/MapEntrySet.h"
#include "hazelcast/client/impl/MapValueCollection.h"
#include "hazelcast/client/map/GetRequest.h"
#include "hazelcast/client/map/PutRequest.h"
#include "hazelcast/client/map/RemoveRequest.h"
#include "hazelcast/client/map/ContainsKeyRequest.h"
#include "hazelcast/client/map/ContainsValueRequest.h"
#include "hazelcast/client/map/FlushRequest.h"
#include "hazelcast/client/map/RemoveIfSameRequest.h"
#include "hazelcast/client/map/DeleteRequest.h"
#include "hazelcast/client/map/TryRemoveRequest.h"
#include "hazelcast/client/map/TryPutRequest.h"
#include "hazelcast/client/map/PutTransientRequest.h"
#include "hazelcast/client/map/ReplaceIfSameRequest.h"
#include "hazelcast/client/map/ReplaceRequest.h"
#include "hazelcast/client/map/SetRequest.h"
#include "hazelcast/client/map/LockRequest.h"
#include "hazelcast/client/map/IsLockedRequest.h"
#include "hazelcast/client/map/UnlockRequest.h"
#include "hazelcast/client/map/GetEntryViewRequest.h"
#include "hazelcast/client/map/EvictRequest.h"
#include "hazelcast/client/map/EvictAllRequest.h"
#include "hazelcast/client/map/KeySetRequest.h"
#include "hazelcast/client/map/GetAllRequest.h"
#include "hazelcast/client/map/EntrySetRequest.h"
#include "hazelcast/client/map/ValuesRequest.h"
#include "hazelcast/client/map/AddIndexRequest.h"
#include "hazelcast/client/map/SizeRequest.h"
#include "hazelcast/client/map/ClearRequest.h"
#include "hazelcast/client/map/PutAllRequest.h"
#include "hazelcast/client/map/QueryRequest.h"
#include "hazelcast/client/map/AddEntryListenerRequest.h"
#include "hazelcast/client/map/ExecuteOnKeyRequest.h"
#include "hazelcast/client/map/ExecuteOnAllKeysRequest.h"
#include "hazelcast/client/map/AddInterceptorRequest.h"
#include "hazelcast/client/map/RemoveInterceptorRequest.h"
#include "hazelcast/client/map/PutIfAbsentRequest.h"
#include "hazelcast/client/map/RemoveEntryListenerRequest.h"
#include "hazelcast/client/map/MapIsEmptyRequest.h"
#include "hazelcast/client/map/DataEntryView.h"
#include "hazelcast/client/impl/EntryListener.h"
#include "hazelcast/client/impl/EntryEventHandler.h"
#include "hazelcast/client/impl/BaseEventHandler.h"
#include "hazelcast/client/impl/PortableEntryEvent.h"
#include "hazelcast/client/impl/QueryResultSet.h"
#include "hazelcast/client/serialization/pimpl/SerializationService.h"
#include "hazelcast/client/DistributedObject.h"
#include "hazelcast/client/EntryView.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>
#include <climits>

namespace hazelcast {
    namespace client {

        class IMapImpl : public DistributedObject {
            friend class HazelcastClient;

        public:

            bool containsKey(const serialization::pimpl::Data& key) {
                int partitionId = getPartitionId(key);
                map::ContainsKeyRequest *request = new map::ContainsKeyRequest(getName(), key);
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            bool containsValue(const serialization::pimpl::Data& value) {
                map::ContainsValueRequest *request = new map::ContainsValueRequest(getName(), value);
                boost::shared_ptr<bool> success = invoke<bool>(request);
                return *success;
            };

            boost::shared_ptr<serialization::pimpl::Data> get(const serialization::pimpl::Data& key) {
                int partitionId = getPartitionId(keyData);
                map::GetRequest *request = new map::GetRequest(getName(), keyData);
                return invoke<serialization::pimpl::Data>(request, partitionId);
            };

            boost::shared_ptr<serialization::pimpl::Data> put(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::PutRequest *request = new map::PutRequest(getName(), keyData, valueData, util::getThreadId(), 0);
                return invoke<serialization::pimpl::Data>(request, partitionId);
            };

            boost::shared_ptr<serialization::pimpl::Data> remove(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::RemoveRequest *request = new map::RemoveRequest(getName(), keyData, util::getThreadId());
                return invoke<serialization::pimpl::Data>(request, partitionId);
            };

            bool remove(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::RemoveIfSameRequest *request = new map::RemoveIfSameRequest(getName(), keyData, valueData, util::getThreadId());
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            void deleteEntry(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::DeleteRequest *request = new map::DeleteRequest(getName(), keyData, util::getThreadId());
                invoke<serialization::pimpl::Void>(request, partitionId);
            };

            void flush() {
                map::FlushRequest *request = new map::FlushRequest(getName());
                invoke<serialization::pimpl::Void>(request);
            };

            bool tryRemove(const serialization::pimpl::Data& key, long timeoutInMillis) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::TryRemoveRequest *request = new map::TryRemoveRequest(getName(), keyData, util::getThreadId(), timeoutInMillis);
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            bool tryPut(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long timeoutInMillis) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::TryPutRequest *request = new map::TryPutRequest(getName(), keyData, valueData, util::getThreadId(), timeoutInMillis);
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            boost::shared_ptr<serialization::pimpl::Data> put(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttlInMillis) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::PutRequest *request = new map::PutRequest(getName(), keyData, valueData, util::getThreadId(), ttlInMillis);
                return invoke<serialization::pimpl::Data>(request, partitionId);
            };

            void putTransient(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttlInMillis) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::PutTransientRequest *request = new map::PutTransientRequest(getName(), keyData, valueData, util::getThreadId(), ttlInMillis);
                invoke<serialization::pimpl::Void>(request, partitionId);
            };

            boost::shared_ptr<serialization::pimpl::Data> putIfAbsent(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                return putIfAbsent(key, value, -1);
            }

            /**
            * Puts an entry into this map with a given ttl (time to live) value
            * if the specified key is not already associated with a value.
            * Entry will expire and get evicted after the ttl.
            *
            * @param key            key of the entry
            * @param value          value of the entry
            * @param ttlInMillis    maximum time in milliseconds for this entry to stay in the map
            * @return the previous value of the entry, if there is no mapping for key
            * then returns NULL in shared_ptr.
            */
            boost::shared_ptr<serialization::pimpl::Data> putIfAbsent(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttlInMillis) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::PutIfAbsentRequest *request = new map::PutIfAbsentRequest(getName(), keyData, valueData, util::getThreadId(), ttlInMillis);
                return invoke<serialization::pimpl::Data>(request, partitionId);
            }

            /**
            * Replaces the entry for a key only if currently mapped to a given value.
            * @param key key with which the specified value is associated
            * @param oldValue value expected to be associated with the specified key
            * @param newValue
            * @return <tt>true</tt> if the value was replaced
            */
            bool replace(const serialization::pimpl::Data& key, const V& oldValue, const V& newValue) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(oldValue);
                serialization::pimpl::Data newValueData = toData(newValue);
                map::ReplaceIfSameRequest *request = new map::ReplaceIfSameRequest(getName(), keyData, valueData, newValueData, util::getThreadId());
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            /**
            * Replaces the entry for a key only if currently mapped to some value.
            * @param key key with which the specified value is associated
            * @param value
            * @return the previous value of the entry, if there is no mapping for key
            * then returns NULL in shared_ptr.
            */
            boost::shared_ptr<serialization::pimpl::Data> replace(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::ReplaceRequest *request = new map::ReplaceRequest(getName(), keyData, valueData, util::getThreadId());
                return invoke<serialization::pimpl::Data>(request, partitionId);
            };

            /**
            * Puts an entry into this map.
            * Similar to put operation except that set
            * doesn't return the old value which is more efficient.
            * @param key key with which the specified value is associated
            * @param value
            * @param ttl maximum time in milliseconds for this entry to stay in the map
            0 means infinite.
            */
            void set(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long ttl) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                serialization::pimpl::Data valueData = toData(value);
                map::SetRequest *request = new map::SetRequest(getName(), keyData, valueData, util::getThreadId(), ttl);
                invoke<serialization::pimpl::Void>(request, partitionId);
            };

            /**
            * Acquires the lock for the specified key.
            * <p>If the lock is not available then
            * the current thread becomes disabled for thread scheduling
            * purposes and lies dormant until the lock has been acquired.
            *
            * Scope of the lock is this map only.
            * Acquired lock is only for the key in this map.
            *
            * Locks are re-entrant so if the key is locked N times then
            * it should be unlocked N times before another thread can acquire it.
            *
            * @param key key to lock.
            */
            void lock(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::LockRequest *request = new map::LockRequest(getName(), keyData, util::getThreadId());
                invoke<serialization::pimpl::Void>(request, partitionId);
            };

            /**
            * Acquires the lock for the specified key for the specified lease time.
            * <p>After lease time, lock will be released..
            *
            * <p>If the lock is not available then
            * the current thread becomes disabled for thread scheduling
            * purposes and lies dormant until the lock has been acquired.
            *
            * Scope of the lock is this map only.
            * Acquired lock is only for the key in this map.
            *
            * Locks are re-entrant so if the key is locked N times then
            * it should be unlocked N times before another thread can acquire it.
            *
            *
            * @param key key to lock.
            * @param leaseTime time in milliseconds to wait before releasing the lock.
            */
            void lock(const serialization::pimpl::Data& key, long leaseTime) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::LockRequest *request = new map::LockRequest(getName(), keyData, util::getThreadId(), leaseTime, -1);
                invoke<serialization::pimpl::Void>(request, partitionId);
            };

            /**
            * Checks the lock for the specified key.
            * <p>If the lock is acquired then returns true, else false.
            *
            *
            * @param key key to lock to be checked.
            * @return <tt>true</tt> if lock is acquired, <tt>false</tt> otherwise.
            */
            bool isLocked(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::IsLockedRequest *request = new map::IsLockedRequest(getName(), keyData);
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            /**
            * Tries to acquire the lock for the specified key.
            * <p>If the lock is not available then the current thread
            * doesn't wait and returns false immediately.
            *
            *
            * @param key key to lock.
            * @return <tt>true</tt> if lock is acquired, <tt>false</tt> otherwise.
            */
            bool tryLock(const serialization::pimpl::Data& key) {
                return tryLock(key, 0);
            };

            /**
            * Tries to acquire the lock for the specified key.
            * <p>If the lock is not available then
            * the current thread becomes disabled for thread scheduling
            * purposes and lies dormant until one of two things happens:
            * <ul>
            * <li>The lock is acquired by the current thread; or
            * <li>The specified waiting time elapses
            * </ul>
            *
            *
            * @param key      key to lock in this map
            * @param timeInMillis     maximum time in milliseconds to wait for the lock
            * @return <tt>true</tt> if the lock was acquired and <tt>false</tt>
            *         if the waiting time elapsed before the lock was acquired.
            */
            bool tryLock(const serialization::pimpl::Data& key, long timeInMillis) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::LockRequest *request = new map::LockRequest(getName(), keyData, util::getThreadId(), LONG_MAX, timeInMillis);
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            /**
            * Releases the lock for the specified key. It never blocks and
            * returns immediately.
            *
            * <p>If the current thread is the holder of this lock then the hold
            * count is decremented.  If the hold count is now zero then the lock
            * is released.  If the current thread is not the holder of this
            * lock then IllegalMonitorStateException is thrown.
            *
            *
            * @param key key to lock.
            * @throws IllegalMonitorStateException if the current thread does not hold this lock MTODO
            */
            void unlock(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::UnlockRequest *request = new map::UnlockRequest(getName(), keyData, util::getThreadId(), false);
                invoke<serialization::pimpl::Void>(request, partitionId);
            };

            /**
            * Releases the lock for the specified key regardless of the lock owner.
            * It always successfully unlocks the key, never blocks
            * and returns immediately.
            *
            *
            * @param key key to lock.
            */
            void forceUnlock(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::UnlockRequest *request = new map::UnlockRequest(getName(), keyData, util::getThreadId(), true);
                invoke<serialization::pimpl::Void>(request, partitionId);
            };

            /**
            * Adds an interceptor for this map. Added interceptor will intercept operations
            * and execute user defined methods and will cancel operations if user defined method throw exception.
            *
            *
            * Interceptor should extend either Portable or IdentifiedSerializable.
            * Notice that map interceptor runs on the nodes. Because of that same class should be implemented in java side
            * with same classId and factoryId.
            * @param interceptor map interceptor
            * @return id of registered interceptor
            */
            template<typename MapInterceptor>
            std::string addInterceptor(MapInterceptor& interceptor) {
                map::AddInterceptorRequest<MapInterceptor> *request = new map::AddInterceptorRequest<MapInterceptor>(getName(), interceptor);
                boost::shared_ptr<std::string> response = invoke<std::string>(request);
                return *response;
            }

            /**
            * Removes the given interceptor for this map. So it will not intercept operations anymore.
            *
            *
            * @param id registration id of map interceptor
            */
            void removeInterceptor(const std::string& id) {
                map::RemoveInterceptorRequest *request = new map::RemoveInterceptorRequest(getName(), id);
                invoke<serialization::pimpl::Void>(request);
            }

            /**
            * Adds an entry listener for this map.
            *
            * Warning 1: If listener should do a time consuming operation, off-load the operation to another thread.
            * otherwise it will slow down the system.
            *
            * Warning 2: Do not make a call to hazelcast. It can cause deadlock.
            *
            * @param listener     entry listener
            * @param includeValue <tt>true</tt> if <tt>EntryEvent</tt> should
            *                     contain the value.
            *
            * @return registrationId of added listener that can be used to remove the entry listener.
            */
            std::string addEntryListener(EntryListener<K, V>& listener, bool includeValue) {
                map::AddEntryListenerRequest *request = new map::AddEntryListenerRequest(getName(), includeValue);
                impl::EntryEventHandler<K, V> *entryEventHandler = new impl::EntryEventHandler<K, V>(getName(), getContext().getClusterService(), getContext().getSerializationService(), listener, includeValue);
                return listen(request, entryEventHandler);
            };

            /**
            * Removes the specified entry listener
            * Returns silently if there is no such listener added before.
            *
            *
            * @param registrationId id of registered listener
            *
            * @return true if registration is removed, false otherwise
            */
            bool removeEntryListener(const std::string& registrationId) {
                map::RemoveEntryListenerRequest *request = new map::RemoveEntryListenerRequest(getName(), registrationId);
                return stopListening(request, registrationId);
            };


            /**
            * Adds the specified entry listener for the specified key.
            *
            * Warning 1: If listener should do a time consuming operation, off-load the operation to another thread.
            * otherwise it will slow down the system.
            *
            * Warning 2: Do not make a call to hazelcast. It can cause deadlock.
            *
            * @param listener     entry listener
            * @param key          key to listen
            * @param includeValue <tt>true</tt> if <tt>EntryEvent</tt> should
            *                     contain the value.
            */
            std::string addEntryListener(EntryListener<K, V>& listener, const serialization::pimpl::Data& key, bool includeValue) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::AddEntryListenerRequest *request = new map::AddEntryListenerRequest(getName(), includeValue, keyData);
                impl::EntryEventHandler<K, V> *entryEventHandler = new impl::EntryEventHandler<K, V>(getName(), getContext().getClusterService(), getContext().getSerializationService(), listener, includeValue);
                return listen(request, partitionId, entryEventHandler);
            };

            /**
            * Returns the <tt>EntryView</tt> for the specified key.
            *
            *
            * @param key key of the entry
            * @return <tt>EntryView</tt> of the specified key
            * @see EntryView
            */
            EntryView<K, V> getEntryView(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::GetEntryViewRequest *request = new map::GetEntryViewRequest(getName(), keyData);
                boost::shared_ptr<map::DataEntryView> dataEntryView = invoke<map::DataEntryView>(request, partitionId);
                boost::shared_ptr<serialization::pimpl::Data> v = toObject<V>(dataEntryView->value);
                EntryView<K, V> view(key, *v, *dataEntryView);
                return view;
            };

            /**
            * Evicts the specified key from this map. If
            * a <tt>MapStore</tt> defined for this map, then the entry is not
            * deleted from the underlying <tt>MapStore</tt>, evict only removes
            * the entry from the memory.
            *
            *
            * @param key key to evict
            * @return <tt>true</tt> if the key is evicted, <tt>false</tt> otherwise.
            */
            bool evict(const serialization::pimpl::Data& key) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::EvictRequest *request = new map::EvictRequest(getName(), keyData, util::getThreadId());
                boost::shared_ptr<bool> success = invoke<bool>(request, partitionId);
                return *success;
            };

            /**
            * Evicts all keys from this map except locked ones.
            * <p/>
            * If a <tt>MapStore</tt> is defined for this map, deleteAll is <strong>not</strong> called by this method.
            * If you do want to deleteAll to be called use the #clear() method.
            * <p/>
            * The EVICT_ALL event is fired for any registered listeners.
            * See EntryListener#mapEvicted(MapEvent)}.
            *
            * @see #clear()
            */
            void evictAll() {
                map::EvictAllRequest *request = new map::EvictAllRequest(getName());
                invoke<int>(request);
            };


            /**
            * Returns a vector clone of the keys contained in this map.
            * The vector is <b>NOT</b> backed by the map,
            * so changes to the map are <b>NOT</b> reflected in the vector, and vice-versa.
            *
            * @return a vector clone of the keys contained in this map
            */
            std::vector<K> keySet() {
                map::KeySetRequest *request = new map::KeySetRequest(getName());
                boost::shared_ptr<map::MapKeySet> dataKeySet = invoke<map::MapKeySet>(request);
                std::vector<serialization::pimpl::Data> const& dataResult = dataKeySet->getKeySet();
                int size = dataResult.size();
                std::vector<K> keySet(size);
                for (int i = 0; i < size; ++i) {
                    boost::shared_ptr<K> k = toObject<K>(dataResult[i]);
                    keySet[i] = *k;
                }
                return keySet;
            };

            /**
            * Returns the entries for the given keys.
            *
            * @param keys keys to get
            * @return map of entries
            */
            std::map<K, V> getAll(const std::set<K>& keys) {
                std::vector<serialization::pimpl::Data> keySet(keys.size());
                int i = 0;
                for (typename std::set<K>::iterator it = keys.begin(); it != keys.end(); ++it) {
                    keySet[i++] = toData(*it);
                }
                map::GetAllRequest *request = new map::GetAllRequest(getName(), keySet);
                boost::shared_ptr<map::MapEntrySet> mapEntrySet = invoke<map::MapEntrySet>(request);
                std::map<K, V> result;
                const std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >& entrySet = mapEntrySet->getEntrySet();
                for (std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >::const_iterator it = entrySet.begin(); it != entrySet.end(); ++it) {
                    boost::shared_ptr<K> key = toObject<K>(it->first);
                    boost::shared_ptr<serialization::pimpl::Data> value = toObject<V>(it->second);
                    result[*key] = *value;
                }
                return result;
            };

            /**
            * Returns a vector clone of the values contained in this map.
            * The vector is <b>NOT</b> backed by the map,
            * so changes to the map are <b>NOT</b> reflected in the collection, and vice-versa.
            *
            * @return a vector clone of the values contained in this map
            */
            std::vector<V> values() {
                map::ValuesRequest *request = new map::ValuesRequest(getName());
                boost::shared_ptr<map::MapValueCollection> valueCollection = invoke<map::MapValueCollection>(request);
                const std::vector<serialization::pimpl::Data>& getValues = valueCollection->getValues();
                int size = getValues.size();
                std::vector<V> values(size);
                for (int i = 0; i < size; i++) {
                    boost::shared_ptr<serialization::pimpl::Data> value = toObject<V>(getValues[i]);
                    values[i] = *value;
                }
                return values;
            };

            /**
            * Returns a std::vector< std::pair<K, V> > clone of the mappings contained in this map.
            * The vector is <b>NOT</b> backed by the map,
            * so changes to the map are <b>NOT</b> reflected in the set, and vice-versa.
            *
            * @return a vector clone of the keys mappings in this map
            */
            std::vector<std::pair<K, V> > entrySet() {
                map::EntrySetRequest *request = new map::EntrySetRequest(getName());
                boost::shared_ptr<map::MapEntrySet> result = invoke<map::MapEntrySet>(request);
                const std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >& returnedEntries = result->getEntrySet();
                int size = returnedEntries.size();
                std::vector<std::pair<K, V> > entrySet(size);
                for (int i = 0; i < size; ++i) {
                    boost::shared_ptr<K> key = toObject<K>(returnedEntries[i].first);
                    boost::shared_ptr<serialization::pimpl::Data> value = toObject<V>(returnedEntries[i].second);
                    entrySet[i] = std::make_pair(*key, *value);
                }
                return entrySet;
            };


            /**
            * Queries the map based on the specified sql predicate and
            * returns the keys of matching entries.
            *
            * Specified predicate runs on all members in parallel.
            *
            *
            * @param sql string query criteria
            * @return result key set of the query
            */
            std::vector<K> keySet(const std::string& sql) {
                std::string iterationType = "KEY";
                map::QueryRequest *request = new map::QueryRequest(getName(), iterationType, sql);
                boost::shared_ptr<impl::QueryResultSet> queryDataResultStream = invoke<impl::QueryResultSet>(request);
                const std::vector<impl::QueryResultEntry>& dataResult = queryDataResultStream->getResultData();
                int size = dataResult.size();
                std::vector<K> keySet(size);
                for (int i = 0; i < size; ++i) {
                    boost::shared_ptr<K> key = toObject<K>(dataResult[i].key);
                    keySet[i] = *key;
                }
                return keySet;
            };

            /**
            * Queries the map based on the specified sql predicate and
            * returns the matching entries.
            *
            * Specified predicate runs on all members in parallel.
            *
            *
            * @param sql string query criteria
            * @return result entry vector of the query
            */
            std::vector<std::pair<K, V> > entrySet(const std::string& sql) {
                std::string iterationType = "ENTRY";
                map::QueryRequest *request = new map::QueryRequest(getName(), iterationType, sql);
                boost::shared_ptr<impl::QueryResultSet> queryDataResultStream = invoke<impl::QueryResultSet>(request);
                const std::vector<impl::QueryResultEntry>& dataResult = queryDataResultStream->getResultData();
                int size = dataResult.size();
                std::vector<std::pair<K, V> > keySet(size);
                for (int i = 0; i < size; ++i) {
                    boost::shared_ptr<K> key = toObject<K>(dataResult[i].key);
                    boost::shared_ptr<serialization::pimpl::Data> value = toObject<V>(dataResult[i].value);
                    keySet[i] = std::make_pair(*key, *value);
                }
                return keySet;
            };

            /**
            * Queries the map based on the specified predicate and
            * returns the values of matching entries.
            *
            * Specified predicate runs on all members in parallel.
            *
            *
            * @param sql predicate query string
            * @return result value vector of the query
            */
            std::vector<V> values(const std::string& sql) {
                std::string iterationType = "VALUE";
                map::QueryRequest *request = new map::QueryRequest(getName(), iterationType, sql);
                boost::shared_ptr<impl::QueryResultSet> queryDataResultStream = invoke<impl::QueryResultSet>(request);
                const std::vector<impl::QueryResultEntry>& dataResult = queryDataResultStream->getResultData();
                int size = dataResult.size();
                std::vector<V> keySet(size);
                for (int i = 0; i < size; ++i) {
                    boost::shared_ptr<serialization::pimpl::Data> value = toObject<V>(dataResult[i].value);
                    keySet[i] = *value;
                }
                return keySet;
            };

            /**
            * Adds an index to this map for the specified entries so
            * that queries can run faster.
            *
            * Let's say your map values are Employee objects.
            *
            *   class Employee : public serialization::Portable {
            *       //...
            *       private:
            *          bool active;
            *          int age;
            *          std::string name;
            *
            *   }
            *
            *
            * If you are querying your values mostly based on age and active then
            * you should consider indexing these fields.
            *
            *   IMap<std::string, Employee > imap = hazelcastInstance.getMap<std::string, Employee >("employees");
            *   imap.addIndex("age", true);        // ordered, since we have ranged queries for this field
            *   imap.addIndex("active", false);    // not ordered, because boolean field cannot have range
            *
            *
            * In the server side, Index  should either have a getter method or be public.
            * You should also make sure to add the indexes before adding
            * entries to this map.
            *
            * @param attribute attribute of value
            * @param ordered   <tt>true</tt> if index should be ordered,
            *                  <tt>false</tt> otherwise.
            */
            void addIndex(const std::string& attribute, bool ordered) {
                map::AddIndexRequest *request = new map::AddIndexRequest(getName(), attribute, ordered);
                invoke<serialization::pimpl::Void>(request);
            };

            /**
            * Applies the user defined EntryProcessor to the entry mapped by the key.
            * Returns the the ResultType which is result of the process() method of EntryProcessor.
            *
            * EntryProcessor should extend either Portable or IdentifiedSerializable.
            * Notice that map EntryProcessor runs on the nodes. Because of that, same class should be implemented in java side
            * with same classId and factoryId.
            *
            * @tparam EntryProcessor type of entry processor class
            * @tparam ResultType that entry processor will return
            * @param entryProcessor that will be applied
            * @param key of entry that entryProcessor will be applied on
            * @return result of entry process.
            */
            template<typename ResultType, typename EntryProcessor>
            ResultType executeOnKey(const serialization::pimpl::Data& key, EntryProcessor& entryProcessor) {
                serialization::pimpl::Data keyData = toData(key);
                int partitionId = getPartitionId(keyData);
                map::ExecuteOnKeyRequest<EntryProcessor> *request = new map::ExecuteOnKeyRequest<EntryProcessor>(getName(), entryProcessor, keyData);
                return invoke<ResultType>(request, partitionId);
            }

            /**
            * Applies the user defined EntryProcessor to the all entries in the map.
            * Returns the results mapped by each key in the map.
            *
            *
            * EntryProcessor should extend either Portable or IdentifiedSerializable.
            * Notice that map EntryProcessor runs on the nodes. Because of that, same class should be implemented in java side
            * with same classId and factoryId.
            *
            * @tparam ResultType that entry processor will return
            * @tparam EntryProcessor type of entry processor class
            * @param entryProcessor that will be applied
            */
            template<typename ResultType, typename EntryProcessor>
            std::map<K, ResultType> executeOnEntries(EntryProcessor& entryProcessor) {
                map::ExecuteOnAllKeysRequest<EntryProcessor> *request = new map::ExecuteOnAllKeysRequest<EntryProcessor>(getName(), entryProcessor);
                boost::shared_ptr<map::MapEntrySet> mapEntrySet = invoke<map::MapEntrySet>(request);
                std::map<K, ResultType> result;
                const std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >& entrySet = mapEntrySet->getEntrySet();
                for (std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >::const_iterator it = entrySet.begin(); it != entrySet.end(); ++it) {
                    K key = toObject<K>(it->first);
                    ResultType resultType = toObject<ResultType>(it->second);
                    result[key] = resultType;
                }
                return result;
            }

            /**
            * Puts an entry into this map.
            * Similar to put operation except that set
            * doesn't return the old value which is more efficient.
            * @param key
            * @param value
            */
            void set(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                set(key, value, -1);
            };

            /**
            * Returns the number of key-value mappings in this map.  If the
            * map contains more than <tt>Integer.MAX_VALUE</tt> elements, returns
            * <tt>Integer.MAX_VALUE</tt>.
            *
            * @return the number of key-value mappings in this map
            */
            int size() {
                map::SizeRequest *request = new map::SizeRequest(getName());
                int s = *(invoke<int>(request));
                return s;
            };

            /**
            * Returns <tt>true</tt> if this map contains no key-value mappings.
            *
            * @return <tt>true</tt> if this map contains no key-value mappings
            */
            bool isEmpty() {
                map::MapIsEmptyRequest *request = new map::MapIsEmptyRequest(getName());

                return *(invoke<bool>(request));;
            };


            /**
            * Copies all of the mappings from the specified map to this map
            * (optional operation).  The effect of this call is equivalent to that
            * of calling put(k, v) on this map once
            * for each mapping from key <tt>k</tt> to value <tt>v</tt> in the
            * specified map.  The behavior of this operation is undefined if the
            * specified map is modified while the operation is in progress.
            *
            * @param m mappings to be stored in this map
            */
            void putAll(const std::map<serialization::pimpl::Data, serialization::pimpl::Data,>& m) {
                map::MapEntrySet entrySet;
                std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >& entryDataSet = entrySet.getEntrySet();
                entryDataSet.resize(m.size());
                int i = 0;
                for (typename std::map<K, V>::const_iterator it = m.begin(); it != m.end(); ++it) {
                    entryDataSet[i++] = std::make_pair(toData(it->first), toData(it->second));
                }
                map::PutAllRequest *request = new map::PutAllRequest(getName(), entrySet);
                invoke<serialization::pimpl::Void>(request);
            };

            /**
            * Removes all of the mappings from this map (optional operation).
            * The map will be empty after this call returns.
            */
            void clear() {
                map::ClearRequest *request = new map::ClearRequest(getName());
                invoke<serialization::pimpl::Void>(request);
            }

        private:
            IMapImpl(const std::string& instanceName, spi::ClientContext *context)
            : DistributedObject("hz:impl:mapService", instanceName, context) {

            }

            void onDestroy() {
            }


        };
    }
}

#endif /* HAZELCAST_IMAP */
