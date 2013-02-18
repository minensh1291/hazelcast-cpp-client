#ifndef HAZELCAST_MULTI_MAP
#define HAZELCAST_MULTI_MAP

#include "ClientService.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>

namespace hazelcast {
    namespace client {

        class ClientService;

        template<typename K, typename V>
        class MultiMap {
        public:

            MultiMap(std::string instanceName, ClientService& clientService) : instanceName(instanceName)
            , clientService(clientService) {

            };

            MultiMap(const MultiMap& rhs) : instanceName(rhs.instanceName)
            , clientService(rhs.clientService) {
            };

            ~MultiMap() {

            };

            std::string getName() const {
                return instanceName;
            };

            /**
             * Stores a key-value pair in the multimap.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param key   the key to be stored
             * @param value the value to be stored
             * @return true if size of the multimap is increased, false if the multimap
             *         already contains the key-value pair.
             */
            bool put(K key, V value);

            /**
             * Returns the collection of values associated with the key.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             * <p/>
             * <p><b>Warning-2:</b></p>
             * The collection is <b>NOT</b> backed by the map,
             * so changes to the map are <b>NOT</b> reflected in the collection, and vice-versa.
             *
             * @param key the key whose associated values are to be returned
             * @return the collection of the values associated with the key.
             */
            std::vector<V> get(K key);

            /**
             * Removes the given key value pair from the multimap.
             * <p/>
             * <p><b>Warning:</b></p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             *
             * @param key   the key of the entry to remove
             * @param value the value of the entry to remove
             * @return true if the size of the multimap changed after the remove operation, false otherwise.
             */
            bool remove(K key, V value);

            /**
             * Removes all the entries with the given key.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             * <p/>
             * <p><b>Warning-2:</b></p>
             * The collection is <b>NOT</b> backed by the map,
             * so changes to the map are <b>NOT</b> reflected in the collection, and vice-versa.
             *
             * @param key the key of the entries to remove
             * @return the collection of removed values associated with the given key. Returned collection
             *         might be modifiable but it has no effect on the multimap
             */
            std::vector<V> remove(K key);

            /**
             * Returns the set of keys in the multimap.
             * <p/>
             * <p><b>Warning:</b></p>
             * The set is <b>NOT</b> backed by the map,
             * so changes to the map are <b>NOT</b> reflected in the set, and vice-versa.
             *
             * @return the set of keys in the multimap. Returned set might be modifiable
             *         but it has no effect on the multimap
             */
            std::set<K> keySet();

            /**
             * Returns the collection of values in the multimap.
             * <p/>
             * <p><b>Warning:</b></p>
             * The collection is <b>NOT</b> backed by the map,
             * so changes to the map are <b>NOT</b> reflected in the collection, and vice-versa.
             *
             * @return the collection of values in the multimap. Returned collection might be modifiable
             *         but it has no effect on the multimap
             */
            std::vector<V> values();

            /**
             * Returns the set of key-value pairs in the multimap.
             * <p/>
             * <p><b>Warning:</b></p>
             * The set is <b>NOT</b> backed by the map,
             * so changes to the map are <b>NOT</b> reflected in the set, and vice-versa.
             *
             * @return the set of key-value pairs in the multimap. Returned set might be modifiable
             *         but it has no effect on the multimap
             */
            std::vector< std::pair<K, V> > entrySet();

            /**
             * Returns whether the multimap contains an entry with the key.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param key the key whose existence is checked.
             * @return true if the multimap contains an entry with the key, false otherwise.
             */
            bool containsKey(K key);

            /**
             * Returns whether the multimap contains an entry with the value.
             * <p/>
             *
             * @param value the value whose existence is checked.
             * @return true if the multimap contains an entry with the value, false otherwise.
             */
            bool containsValue(V value);

            /**
             * Returns whether the multimap contains the given key-value pair.
             * <p/>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             *
             * @param key   the key whose existence is checked.
             * @param value the value whose existence is checked.
             * @return true if the multimap contains the key-value pair, false otherwise.
             */
            bool containsEntry(K key, V value);

            /**
             * Returns the number of key-value pairs in the multimap.
             *
             * @return the number of key-value pairs in the multimap.
             */
            int size();

            /**
             * Clears the multimap. Removes all key-value pairs.
             */
            void clear();

            /**
             * Returns number of values matching to given key in the multimap.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param key the key whose values count are to be returned
             * @return number of values matching to given key in the multimap.
             */
            int valueCount(K key);

            /**
             * Adds a local entry listener for this multimap. Added listener will be only
             * listening for the events (add/remove/update) of the locally owned entries.
             * <p/>
             * Note that entries in distributed multimap are partitioned across
             * the cluster members; each member owns and manages the some portion of the
             * entries. Owned entries are called local entries. This
             * listener will be listening for the events of local entries. Let's say
             * your cluster has member1 and member2. On member2 you added a local listener and from
             * member1, you call <code>multimap.put(key2, value2)</code>.
             * If the key2 is owned by member2 then the local listener will be
             * notified for the add/update event. Also note that entries can migrate to
             * other nodes for load balancing and/or membership change.
             *
             * @param listener entry listener
             * @see #localKeySet()
             */
            //            void addLocalEntryListener(EntryListener<K, V> listener);

            /**
             * Adds an entry listener for this multimap. Listener will get notified
             * for all multimap add/remove/update/evict events.
             *
             * @param listener     entry listener
             * @param includeValue <tt>true</tt> if <tt>EntryEvent</tt> should
             *                     contain the value.
             */
            //            void addEntryListener(EntryListener<K, V> listener, bool includeValue);

            /**
             * Removes the specified entry listener
             * Returns silently if there is no such listener added before.
             *
             * @param listener entry listener
             */
            //            void removeEntryListener(EntryListener<K, V> listener);

            /**
             * Adds the specified entry listener for the specified key.
             * The listener will get notified for all
             * add/remove/update/evict events of the specified key only.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param listener     entry listener
             * @param key          the key to listen
             * @param includeValue <tt>true</tt> if <tt>EntryEvent</tt> should
             *                     contain the value.
             */
            //            void addEntryListener(EntryListener<K, V> listener, K key, bool includeValue);

            /**
             * Removes the specified entry listener for the specified key.
             * Returns silently if there is no such listener added before for
             * the key.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param listener
             * @param key
             */
            //            void removeEntryListener(EntryListener<K, V> listener, K key);

            /**
             * Acquires the lock for the specified key.
             * <p>If the lock is not available then
             * the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until the lock has been acquired.
             * <p/>
             * Scope of the lock is this multimap only.
             * Acquired lock is only for the key in this multimap.
             * <p/>
             * Locks are re-entrant so if the key is locked N times then
             * it should be unlocked N times before another thread can acquire it.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param key key to lock.
             */
            void lock(K key);

            /**
             * Tries to acquire the lock for the specified key.
             * <p>If the lock is not available then the current thread
             * doesn't wait and returns false immediately.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param key key to lock.
             * @return <tt>true</tt> if lock is acquired, <tt>false</tt> otherwise.
             */
            bool tryLock(K key);

            /**
             * Tries to acquire the lock for the specified key.
             * <p>If the lock is not available then
             * the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until one of two things happens:
             * <ul>
             * <li>The lock is acquired by the current thread; or
             * <li>The specified waiting time elapses
             * </ul>
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param time     the maximum time to wait for the lock
             * @param timeunit the time unit of the <tt>time</tt> argument.
             * @return <tt>true</tt> if the lock was acquired and <tt>false</tt>
             *         if the waiting time elapsed before the lock was acquired.
             */
            bool tryLock(K key, long timeoutInMillis);

            /**
             * Releases the lock for the specified key. It never blocks and
             * returns immediately.
             * <p/>
             * <p><b>Warning:</b></p>
             * <p>
             * This method uses <tt>hashCode</tt> and <tt>equals</tt> of binary form of
             * the <tt>key</tt>, not the actual implementations of <tt>hashCode</tt> and <tt>equals</tt>
             * defined in <tt>key</tt>'s class.
             * </p>
             *
             * @param key key to lock.
             */
            void unlock(K key);

            /**
             * Returns LocalMultiMapStats for this map.
             * LocalMultiMapStats is the statistics for the local portion of this
             * distributed multi map and contains information such as ownedEntryCount
             * backupEntryCount, lastUpdateTime, lockedEntryCount.
             * <p/>
             * Since this stats are only for the local portion of this multi map, if you
             * need the cluster-wide MultiMapStats then you need to get the LocalMapStats
             * from all members of the cluster and combine them.
             *
             * @return this multimap's local statistics.
             */
        private:
            std::string instanceName;
            ClientService& clientService;
        };


    }
}

#endif /* HAZELCAST_MULTI_MAP */