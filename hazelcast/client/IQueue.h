#ifndef HAZELCAST_IQUEUE
#define HAZELCAST_IQUEUE

#include "queue/OfferRequest.h"
#include "queue/PollRequest.h"
#include "queue/RemainingCapacityRequest.h"
#include "queue/RemoveRequest.h"
#include "queue/ContainsRequest.h"
#include "queue/DrainRequest.h"
#include "queue/PeekRequest.h"
#include "queue/SizeRequest.h"
#include "queue/CompareAndRemoveRequest.h"
#include "queue/AddAllRequest.h"
#include "queue/ClearRequest.h"
#include "queue/DestroyRequest.h"
#include "queue/ItearatorRequest.h"
#include "impl/PortableCollection.h"
#include <stdexcept>

namespace hazelcast {
    namespace client {


        template<typename E>
        class IQueue {
        public:
            IQueue(const std::string& name, spi::ClientContext& clientContext)
            : name(name)
            , context(clientContext)
            , key(clientContext.getSerializationService().toData(name)) {

            };

            bool add(const E& e) {
                if (offer(e)) {
                    return true;
                }
                throw HazelcastException/*TODO IllegalState Exception*/("Queue is full!");
            };

            bool offer(const E& e) {
                try {
                    return offer(e, 0);
                } catch (HazelcastException/*TODO Interrupted Exception*/ ex) {
                    return false;
                }
            };

            void put(const E& e) throw(HazelcastException/*TODO Interrupted Exception*/) {
                offer(e, -1);
            };

            bool offer(const E& e, long timeoutInMillis) throw(HazelcastException/*TODO Interrupted Exception*/) {
                serialization::Data data = context.getSerializationService().toData(e);
                queue::OfferRequest request(name, timeoutInMillis, data);
                return invoke<bool>(request);
            };

            E take() throw(HazelcastException/*TODO Interrupted Exception*/) {
                return poll(-1);
            };

            E poll(long timeoutInMillis) throw(HazelcastException/*TODO Interrupted Exception*/) {
                queue::PollRequest request(name, timeoutInMillis);
                return invoke<E>(request);
            };

            int remainingCapacity() {
                queue::RemainingCapacityRequest request(name);
                return invoke<int>(request);
            };

            bool remove(const E& o) {
                serialization::Data data = context.getSerializationService().toData(o);
                queue::RemoveRequest request(name, data);
                bool result = invoke(request);
                return result;
            };

            bool contains(const E& o) {
                std::vector<serialization::Data> list(1);
                list[0] = context.getSerializationService().toData(o);
                queue::ContainsRequest request(name, list);
                return invoke<bool>(request);
            };

            int drainTo(const std::vector<E>& objects) {
                return drainTo(objects, -1);
            };

            int drainTo(const std::vector<E>& c, int maxElements) {
                queue::DrainRequest request(name, maxElements);
                impl::PortableCollection result = invoke<impl::PortableCollection>(request);
                const std::vector<serialization::Data>& coll = result.getCollection();
                for (std::vector<serialization::Data>::const_iterator it = coll.begin(); it != coll.end(); ++it) {
                    E e = context.getSerializationService().template toObject<E>(*it);
                    c.add(e);
                }
                return coll.size();
            };

            E remove() {
                E res = poll();
                if (res == E()) {//TODO ?
                    throw HazelcastException/*TODO NoSuchElementException*/("Queue is empty!");
                }
                return res;
            };

            E poll() {
                try {
                    return poll(0);
                } catch (HazelcastException/*TODO InterruptedException*/ e) {
                    return E();
                }
            };

            E element() {
                E res = peek();
                if (res == E()) {
                    throw HazelcastException/*TODO NoSuchElementException*/("Queue is empty!");
                }
                return res;
            };

            E peek() {
                queue::PeekRequest request(name);
                return invoke<E>(request);
            };

            int size() {
                queue::SizeRequest request(name);
                return invoke<int>(request);;
            }

            bool isEmpty() {
                return size() == 0;
            };

//        Iterator<E> iterator() {TODO
//                IteratorRequest request = new IteratorRequest(name);
//                PortableCollection result = invoke(request);
//                Collection<Data> coll = result.getCollection();
//                return new QueueIterator<E>(coll.iterator(), context.getSerializationService(), false);
//            }
//
            std::vector<E> toArray() {
                queue::IteratorRequest request(name);
                impl::PortableCollection result = invoke<impl::PortableCollection>(request);
                const vector<serialization::Data> const & coll = result.getCollection();
                return getObjectList(coll);
            };

            bool containsAll(const std::vector<E>& c) {
                std::vector<serialization::Data> list = getDataList(c);
                queue::ContainsRequest request(name, list);
                return invoke<bool>(request);
            }

            bool addAll(const std::vector<E>& c) {
                queue::AddAllRequest request(name, getDataList(c));
                return invoke<bool>(request);
            }

            bool removeAll(const std::vector<E>&c) {
                queue::CompareAndRemoveRequest request(name, getDataList(c), false);
                return invoke<bool>(request);
            }

            bool retainAll(const std::vector<E>&c) {
                queue::CompareAndRemoveRequest request(name, getDataList(c), true);
                return invoke<bool>(request);
            }


            void clear() {
                queue::ClearRequest request(name);
                invoke<bool>(request);
            };

            void onDestroy() {
                queue::DestroyRequest request(name);
                invoke<bool>(request);
            };

            std::string getName() {
                return name;
            };

            template<typename Response, typename Request>
            Response invoke(const Request& request) {
                return context.getInvocationService().template invokeOnKeyOwner<Response>(request, key);
            };

            std::vector<serialization::Data> getDataList(const std::vector<E>& objects) {
                std::vector<serialization::Data> dataList(objects.size());
                for (int i = 0; i < objects.size(); i++) {
                    dataList[i] = context.getSerializationService().toData(objects[i]);
                }
                return dataList;
            };

            std::vector<E> getObjectList(const std::vector<serialization::Data>& dataList) {
                std::vector<E> objects(dataList.size());
                for (int i = 0; i < dataList.size(); i++) {
                    objects[i] = context.getSerializationService(). template toObject<E>(dataList[i]);
                }
                return objects;
            };

        private:
            std::string name;
            serialization::Data key;
            spi::ClientContext& context;

        };
    }
}

#endif /* HAZELCAST_IQUEUE */