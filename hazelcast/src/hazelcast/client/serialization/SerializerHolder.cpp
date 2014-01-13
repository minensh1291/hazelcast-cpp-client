//
// Created by sancar koyunlu on 7/31/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/serialization/SerializerHolder.h"
#include "hazelcast/client/serialization/Serializer.h"

namespace hazelcast {
    namespace client {
        namespace serialization {

            SerializerHolder::SerializerHolder(SerializationContext& context)
            :portableSerializer(context) {

            }

            bool SerializerHolder::registerSerializer(boost::shared_ptr<SerializerBase> serializer) {
                boost::shared_ptr<SerializerBase> available = serializers.putIfAbsent(serializer->getTypeId(), serializer);
                return available.get() == NULL;
            };

            boost::shared_ptr<SerializerBase> SerializerHolder::serializerFor(int typeId) {
                return serializers.get(typeId);
            }

            PortableSerializer& SerializerHolder::getPortableSerializer() {
                return portableSerializer;
            }

            DataSerializer& SerializerHolder::getDataSerializer() {
                return dataSerializer;
            }


        }
    }
}