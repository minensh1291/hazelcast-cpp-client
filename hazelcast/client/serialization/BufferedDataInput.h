//
//  BufferedDataInput.h
//  Server
//
//  Created by sancar koyunlu on 1/3/13.
//  Copyright (c) 2013 sancar koyunlu. All rights reserved.
//

#ifndef HAZELCAST_DATA_INPUT
#define HAZELCAST_DATA_INPUT

#include <vector>
#include <string>

namespace hazelcast {
    namespace client {
        namespace serialization {

            class SerializationService;

            class SerializationContext;

            typedef unsigned char byte;

            class BufferedDataInput {
            public:

                BufferedDataInput(const std::vector<byte>&);

                BufferedDataInput& operator [](std::string);

                ~BufferedDataInput();

                void readFully(std::vector<byte>&);

                void readFully(byte *bytes, int off, int len);

                int skipBytes(int i);

                bool readBoolean();

                byte readByte();

                short readShort();

                char readChar();

                int readInt();

                long readLong();

                float readFloat();

                double readDouble();

                std::string readUTF();

                int position();

                void position(int newPos);

            private:
                byte *ptr;
                byte *beg;

                static int const STRING_CHUNK_SIZE = 16 * 1024;

                std::string readShortUTF();

                BufferedDataInput& operator = (const BufferedDataInput&);

            };

            template<typename T>
            inline void operator >>(BufferedDataInput& dataInput, T& data) {
                //TODO some control stuff can be added here : not sure what is needed right now
                //........
                readPortable(dataInput, data);
            };
        }
    }
}
#endif /* HAZELCAST_DATA_INPUT */