//
// Created by sancar koyunlu on 9/4/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/map/ReplaceRequest.h"
#include "hazelcast/client/serialization/Data.h"
#include "hazelcast/client/map/PortableHook.h"
#include "hazelcast/client/serialization/PortableWriter.h"


namespace hazelcast {
    namespace client {
        namespace map {
            ReplaceRequest::ReplaceRequest(const std::string& name, serialization::Data& key, serialization::Data& value, int threadId)
            :name(name)
            , key(key)
            , value(value)
            , threadId(threadId) {

            };

            int ReplaceRequest::getFactoryId() const {
                return PortableHook::F_ID;
            }

            int ReplaceRequest::getClassId() const {
                return PortableHook::REPLACE;
            }

            void ReplaceRequest::writePortable(serialization::PortableWriter& writer) const {
                writer.writeUTF("n", name);
                writer.writeInt("t", threadId);
                writer.writeLong("ttl", -1);
                serialization::ObjectDataOutput& out = writer.getRawDataOutput();
                key.writeData(out);
                value.writeData(out);
            };


        }
    }
}
