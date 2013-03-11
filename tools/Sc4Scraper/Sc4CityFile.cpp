#include "Sc4CityFile.h"

#include <iostream>

using namespace std;
using namespace boost;
using namespace iostreams;


unsigned char *uncompress_data( unsigned char *inbuf,int *buflen );

Sc4CityFile::Sc4CityFile(const std::string &path)
{
   // int size = sizeof(Header);
   // assert(size == 96);

    int size=sizeof(IndexEntry);
    assert(size == 20);

    m_File.open(path);


    m_Header = (Header*)m_File.data();
    m_Index = (IndexEntry*)(m_File.data() + m_Header->indexFirstOffset);
    m_IndexSize = m_Header->indexEntryCount;

    //This is a sim city 4 file right?
    assert(m_Header->majorVer == 1);
    assert(m_Header->minorVer == 0);
    assert(m_Header->indexMajorVer == 7);
    assert(m_Header->indexMinorVer == 0);

    cout << "Opened city file " << m_IndexSize << " files" <<endl;


    for(int i=0; i < m_IndexSize; i ++)
    {
        IndexEntry& entry = m_Index[i];

        //cout << "Entry type=" << entry.typeId << " location=" << entry.location << " size: " << entry.size << endl;

        if(entry.typeId == PNG)
        {
            cout << "Found PNG instance=" << entry.instanceId << " location=" << entry.location <<  endl;


        }
    }
}


bool Sc4CityFile::getFile(FileType type, uint32_t instanceId, const void *&data, size_t &size)
{
    for(int i=0; i < m_IndexSize; i ++)
    {
        const IndexEntry& entry = m_Index[i];

        if(entry.typeId == type && entry.instanceId == instanceId)
        {
            cout << "Got file T:"<< hex << entry.typeId << " G:" << entry.groupId << " I:" << entry.instanceId << dec <<endl;

            data = (void*)(m_File.data() + entry.location);
            size = entry.size;
            return true;
        }
    }
    return false;
}

const Sc4CityFile::RegionData* Sc4CityFile::getRegionData()
{
    const void* data = 0;
    size_t size = 0;

    if(getFile(REGION_DATA, 0, data,size))
    {
        const RegionData* region = (RegionData*)uncompress_data( const_cast<unsigned char*>((unsigned char*)data), (int*)&size);


        //cout << "City name " << region->cityName()  << endl;
        //cout << "Mayro Name " << region->formerName() << endl;

        assert(region->versionMajor == 1);
        assert(region->versionMinor == 13);

        return region;
    }

    return 0;
}

