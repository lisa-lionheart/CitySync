#include "city.h"

#include <iostream>
#include <sstream>

#include "Region.h"
#include "server.h"

#include "sc4savegame.h"
#include "regionviewfile.h"

using namespace std;


City::City(Region *region, Json::Value& data) :
    QObject((QObject*)region)
{
    cout << "Parsing city: " << data.toStyledString() << endl;

    m_Name = QString::fromStdString( data["name"].asString() );
    m_Owner = QString::fromStdString( data["owner"].asString() );
    m_Guid = QString::fromStdString( data["guid"].asString() );

    m_Position = QPoint( data["x"].asInt(), data["y"].asInt());
    m_Size = data["size"].asInt();

}


bool City::isLoading()
{

}

void City::loadSave(const QString& path)
{
    Sc4SaveGame save(path.toStdString());

    RegionViewFile* m_RegionData = RegionViewFile::loadFrom(save);


    cout << "City " << m_RegionData->cityName << " guid " << hex << m_RegionData->guid << endl;


    const void* thumbData;
    size_t thumbSize;


    if(!save.getFile(Sc4SaveGame::PNG,0,0,thumbData,thumbSize))
    {
        cerr << "Unable to get thumbnail" << endl;
        return;
    }

    m_Thumbnail.loadFromData((uchar*)thumbData,thumbSize,"png");
    emit cityUpdated();
}

Region* City::region()
{
    return (Region*)parent();
}

QString City::thumbnailUrl()
{
   // return "qrc:///resources/thumbnail.png";


    stringstream s;
    if(m_Thumbnail.isNull())
    {
        s << region()->server()->url().toStdString() << "region/" <<  region()->name().toStdString() << "/" << m_Guid.toStdString() << "/" << "thumbnail.png";
    }
    else
    {
        s << "image://" << region()->name().toStdString() << "/" << m_Guid.toStdString();
    }

    return QString::fromStdString(s.str());
}

QPoint City::position()
{
    return m_Position;
}

QPoint City::centerPoint()
{
    return QPoint(m_Position.x()+ m_Size/2, m_Position.y()+ m_Size/2);
}

int City::size()
{
    return m_Size;
}


QPoint City::bottomRightPosition()
{
    return QPoint(m_Position.x()  +m_Size,m_Position.y() + m_Size);
}

QPoint City::bottomLeftPosition()
{
    return QPoint(m_Position.x() ,m_Position.y() + m_Size);
}

QImage& City::thumbnail()
{
    return m_Thumbnail;
}

QString City::name()
{
    return m_Name;
}

int City::screenWidth()
{
    return m_Size * 128;
}

QString City::guid()
{
    return m_Guid;
}
