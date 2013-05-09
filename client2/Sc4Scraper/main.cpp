#include <iostream>
#include <fstream>
#include <string>

#include <sc4savegame.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

using namespace std;

int main(int arc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("thumbnail", po::value<string>(), "Extract thumbnail")
        ("savegame", po::value<string>(), "Save game to open")
    ;

    po::positional_options_description p;
    p.add("savegame", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(arc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);


    if(vm.count("savegame") == 0)
    {
        cerr << "Must specify savegame" << endl;
        return 1;
    }


    Sc4SaveGame file( vm["savegame"].as<string>());
    if(vm.count("thumbnail") == 1)
    {
        const void* thumbData = 0;
        size_t thumbSize = 0;
        if(file.getFile(Sc4SaveGame::PNG, 0, 0, thumbData,thumbSize))
        {
            ofstream out;

            string fname = vm["thumbnail"].as<string>() + ".png";

            out.open(fname.c_str(),ios_base::binary);

            out.write((const char*)thumbData,thumbSize);
        }
    }

    RegionViewFile* region = RegionViewFile::loadFrom(file);
    if(region)
    {
        cout << "{" << endl;

        cout << "\"name\":\"" << region->cityName << "\"," << endl;
        cout << "\"mayorName\":\"" << region->mayorName << "\"," << endl;

        cout << "\"tileX\":" << region->tileX << "," << endl;
        cout << "\"tileY\":" << region->tileY << "," << endl;

        cout << "\"sizeX\":" << region->sizeX << "," << endl;
        cout << "\"sizeY\":" << region->sizeY << "," << endl;
        cout << "\"guid\":\"" << hex << region->guid << "\"" << endl;

        cout << "}";

        delete region;
    }
    else
    {
        cerr << "Could not get region view file" << endl;
    }

    return 0;
}

