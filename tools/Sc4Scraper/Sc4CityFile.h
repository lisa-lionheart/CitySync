#pragma once

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "RegionViewFile.h"

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

    struct DBDFEntry
    {
        uint32_t typeId;
        uint32_t groupId;
        uint32_t instanceId;
        uint32_t size;
    };



    enum FileType
    {
        PNG         = 0x8a2482b9,
        REGION_DATA = 0xCA027EDB
    };


    enum GroupId
    {
        THUMBNAIL = 0x4a2482b9,
        REGION = 0xca027ee1
    };

    Sc4CityFile(const std::string& path);

    const RegionFile* getRegionData();


    bool getFile(FileType type, uint32_t groupId, uint32_t instanceId, const void*& data,  size_t& size);

private:
    Header* m_Header;

    IndexEntry* m_Index;
    uint32_t m_IndexSize;

    DBDFEntry* m_DBDF;
    uint32_t m_DBDFSize;

    boost::iostreams::mapped_file m_File;
};
