//
// Created by sancar koyunlu on 9/5/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/lock/GetRemainingLeaseRequest.h"
#include "hazelcast/client/serialization/Data.h"
#include "hazelcast/client/lock/LockPortableHook.h"
#include "hazelcast/client/serialization/PortableWriter.h"

namespace hazelcast {
    namespace client {
        namespace lock {
            GetRemainingLeaseRequest::GetRemainingLeaseRequest(serialization::Data& key)
            :key(key) {
            };

            int GetRemainingLeaseRequest::getClassId() const {
                return LockPortableHook::GET_REMAINING_LEASE;
            };

            int GetRemainingLeaseRequest::getFactoryId() const {
                return LockPortableHook::FACTORY_ID;
            };

            void GetRemainingLeaseRequest::writePortable(serialization::PortableWriter& writer) const {
                serialization::ObjectDataOutput& out = writer.getRawDataOutput();
                key.writeData(out);
            };
        }
    }
}