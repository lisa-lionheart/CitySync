#include <iostream>
#include <fstream>
#include <string>

#include <Sc4CityFile.h>


using namespace std;

int main()
{

    string folder = "/Users/lisa/Downloads/Steamboat Springs/";
    string city = "City - Peach Trees";

    Sc4CityFile file(folder + city + ".sc4");


    const void* thumbData = 0;
    size_t thumbSize = 0;
    if(file.getFile(Sc4CityFile::PNG, 0, thumbData,thumbSize))
    {
        ofstream out;

        string fname = folder + city + ".png";

        out.open(fname.c_str(),ios_base::binary);

        out.write((const char*)thumbData,thumbSize);
    }

    const Sc4CityFile::RegionData* region = file.getRegionData();
    if(region)
    {
        string fname = folder + city + ".json";
        ofstream out(fname.c_str());

            out << "{" << endl;

            out << "tileX:" << region->tileX << "," << endl;
            out << "tileY:" << region->tileY << "," << endl;

            out << "sizeX:" << region->sizeX << "," << endl;
            out << "sizeY:" << region->sizeY << "," << endl;


            out << "guid:" << region->guid << "," << endl;

            out << "}";

    }


    cout << "Hello World!" << endl;
    return 0;
}

