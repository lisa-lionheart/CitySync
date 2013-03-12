#pragma once

#pragma pack(0)

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

struct RegionFile
{
    int32_t tileX;
    int32_t tileY;
    uint32_t sizeX;
    uint32_t sizeY;

    uint32_t residential;
    uint32_t commercial;
    uint32_t industrial;

    uint8_t mayorRating;
    uint8_t starCount;

    uint32_t guid;

    std::string cityName;
    std::string mayorName;
};
