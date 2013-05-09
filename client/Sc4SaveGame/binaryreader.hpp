#pragma once

class BinaryReader
{
   const uint8_t* readHead;

public:
    BinaryReader(const void* head){
        readHead = (uint8_t*)head;
    }

    const void* readBytes(size_t bytes)
    {
        const void* data = readHead;
        readHead += bytes;
        return data;
    }

    template<typename T>
    T read()
    {
        return *((T*)readBytes(sizeof(T)));
    }

    std::string readString()
    {
        uint32_t size = read<uint32_t>();
        return std::string((char*)readBytes(size),size);
    }
};
