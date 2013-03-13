#include "sc4savegame.h"
#include "regionviewfile.h"
#include "binaryreader.hpp"

RegionViewFile* RegionViewFile::loadFrom(Sc4SaveGame &sc4)
{
    const void* data = 0;
    size_t size = 0;

    if(sc4.getFile(Sc4SaveGame::REGION_DATA, Sc4SaveGame::REGION,  0, data,size))
    {
        RegionViewFile* city = new RegionViewFile;
        BinaryReader reader((char*)data);

        assert(reader.read<uint16_t>() == 1);
        assert(reader.read<uint16_t>() == 13);

        city->tileX = reader.read<uint32_t>();
        city->tileY = reader.read<uint32_t>();

        city->sizeX = reader.read<uint32_t>();
        city->sizeY = reader.read<uint32_t>();

        city->residential = reader.read<uint32_t>();
        city->commercial = reader.read<uint32_t>();
        city->industrial = reader.read<uint32_t>();


        reader.read<float>();

        city->mayorRating = reader.read<uint8_t>();
        city->starCount = reader.read<uint8_t>();

        reader.readBytes(1);

        city->guid = reader.read<uint32_t>();

        reader.readBytes(21);

        city->cityName = reader.readString();
        reader.readString();
        city->mayorName = reader.readString();

        return city;
    }

    return 0;
}
