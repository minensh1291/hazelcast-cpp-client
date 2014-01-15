//
// Created by sancar koyunlu on 9/12/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/collection/CollectionAddAllRequest.h"
#include "hazelcast/client/serialization/Data.h"
#include "hazelcast/client/collection/CollectionPortableHook.h"
#include "hazelcast/client/serialization/PortableWriter.h"

namespace hazelcast {
    namespace client {
        namespace collection {
            CollectionAddAllRequest::CollectionAddAllRequest(const std::string &name, const std::string &serviceName, const std::vector<serialization::Data> &valueList)
            : CollectionRequest(name, serviceName)
            , valueList(valueList) {

            }

            int CollectionAddAllRequest::getClassId() const {
                return CollectionPortableHook::COLLECTION_ADD_ALL;
            }

            void CollectionAddAllRequest::write(serialization::PortableWriter &writer) const {
                CollectionRequest::write(writer);
                serialization::ObjectDataOutput &output = writer.getRawDataOutput();
                output.writeInt(valueList.size());
                for (int i = 0; i < valueList.size(); i++) {
                    valueList[i].writeData(output);
                }
            }
        }
    }
}