#ifndef SC4CITYFILE_H
#define SC4CITYFILE_H


#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include <string>

#pragma pack(0)

class Sc4CityFile
{
public:

    struct Header
    {
        char identifier[4];
        uint32_t majorVer;
        uint32_t minorVer;

        uint32_t unknown[3];

        uint32_t dateCreated;
        uint32_t dateModified;

        uint32_t indexMajorVer;
        uint32_t indexEntryCount;

        uint32_t indexFirstOffset;
        uint32_t indexSize;

        uint32_t holeEntryCount;
        uint32_t holeOffset;
        uint32_t holeSize;

        uint32_t indexMinorVer;

        char reserved[24];
    };

    struct IndexEntry
    {
        uint32_t typeId;
        uint32_t groupId;
        uint32_t instanceId;
        uint32_t location;
        uint32_t size;

    };

    struct RegionData
    {
        uint16_t versionMajor;
        uint16_t versionMinor;

        int32_t tileX;
        int32_t tileY;
        uint16_t sizeX;
        uint16_t sizeY;

        uint32_t residential;
        uint32_t commercial;
        uint32_t industrial;

        float unknown;

        uint8_t mayorRating;
        uint8_t starCount;
        uint8_t tutorial;

        uint32_t guid;

        uint32_t unknown2[5];

        uint8_t mode;

        uint32_t nameLen;
        char data[];

        std::string cityName() const
        {
            return std::string(data + nameLen);
        }

        uint32_t formerNameLen() const
        {
            return *((uint32_t*)(data + nameLen));
        }

        std::string formerName() const
        {
            return std::string(data + nameLen + formerNameLen(), formerNameLen());
        }

    };


    enum FileType
    {
        PNG = 0x8a2482b9,
        REGION_DATA = 0xCA027EDB
    };


    Sc4CityFile(const std::string& path);

    const RegionData* getRegionData();


    bool getFile(FileType type, uint32_t instanceId, const void*& data,  size_t& size);

private:
    Header* m_Header;
    IndexEntry* m_Index;
    uint32_t m_IndexSize;

    boost::iostreams::mapped_file m_File;
};

#endif // SC4CITYFILE_H
