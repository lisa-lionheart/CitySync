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

        uint16_t major = reader.read<uint16_t>();
        uint16_t minor = reader.read<uint16_t>();

        assert(major == 1);
        assert(minor == 13 || minor == 9);


        city->tileX = reader.read<uint32_t>();
        city->tileY = reader.read<uint32_t>();

        city->sizeX = reader.read<uint32_t>();
        city->sizeY = reader.read<uint32_t>();



        city->residential = reader.read<uint32_t>();
        city->commercial = reader.read<uint32_t>();
        city->industrial = reader.read<uint32_t>();

        //We havent yet decode, this assuming there is a guid?
        if(minor == 9)
        {
            city->cityName = "New City";
            city->guid = 0;

            return city;
        }

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
