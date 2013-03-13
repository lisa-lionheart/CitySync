#pragma once

#pragma pack(0)


class Sc4SaveGame;

struct RegionViewFile
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

    static RegionViewFile* loadFrom(Sc4SaveGame& sc4);
};
