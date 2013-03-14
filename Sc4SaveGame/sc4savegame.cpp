#include "sc4savegame.h"
#include <iostream>

using namespace std;
using namespace boost;
using namespace iostreams;


unsigned char *uncompress_data( unsigned char *inbuf,int *buflen );

Sc4SaveGame::Sc4SaveGame(const std::string &path)
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

    //cout << "Opened city file " << m_IndexSize << " files" <<endl;

    for(int i=0; i < m_IndexSize; i ++)
    {
        IndexEntry& entry = m_Index[i];

        //cout << "Entry type=" << entry.typeId << " location=" << entry.location << " size: " << entry.size << endl;
        if(entry.typeId == PNG)
        {
           // cout << "Found PNG instance=" << entry.instanceId << " location=" << entry.location <<  endl;
        }

        if(entry.typeId == 0xE86B1EEF &&
           entry.groupId == 0xE86B1EEF &&
           entry.instanceId == 0x286B1F03)
        {
            //cout << "Found index of compressed files" << endl;

            m_DBDF = (DBDFEntry*)(m_File.data()+entry.location);
            m_DBDFSize = entry.size / 16;
        }
    }
}

Sc4SaveGame::~Sc4SaveGame()
{
    for(vector<void*>::iterator itr = m_AllocedData.begin(); itr != m_AllocedData.end(); itr++ )
    {
        free(*itr);
    }
}


bool Sc4SaveGame::getFile(FileType type, uint32_t groupId, uint32_t instanceId, const void *&data, size_t &size)
{
    DBDFEntry* compressed = NULL;
    for(int i=0; i < m_DBDFSize; i ++)
    {
        if(type == m_DBDF[i].typeId && instanceId == m_DBDF[i].instanceId)
        {
            compressed = &m_DBDF[i];
        }
    }

    for(int i=0; i < m_IndexSize; i ++)
    {
        const IndexEntry& entry = m_Index[i];

        if(entry.typeId == type && entry.instanceId == instanceId)
        {
            //cout << "Got file T:"<< hex << entry.typeId << " G:" << entry.groupId << " I:" << entry.instanceId << dec <<endl;

            data = (void*)(m_File.data() + entry.location);
            size = entry.size;

            if(compressed)
            {
                void* newData = uncompress_data( const_cast<unsigned char*>((unsigned char*)data)+4, (int*)&size);
                m_AllocedData.push_back(newData);
                assert(size == compressed->size);

                data = newData;
            }

            return true;
        }
    }
    return false;
}



