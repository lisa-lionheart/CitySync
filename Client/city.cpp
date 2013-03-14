#include "city.h"

#include <iostream>

#include "sc4savegame.h"
#include "regionviewfile.h"

using namespace std;


City::City(QObject *parent) :
    QObject(parent)
{

}


void City::loadSave(const QString& path)
{
    Sc4SaveGame save(path.toStdString());

    m_RegionData = RegionViewFile::loadFrom(save);


    cout << "City " << m_RegionData->cityName << " guid " << hex << m_RegionData->guid << endl;


    const void* thumbData;
    size_t thumbSize;


    if(!save.getFile(Sc4SaveGame::PNG,0,0,thumbData,thumbSize))
    {
        cerr << "Unable to get thumbnail" << endl;
        return;
    }

    QImage thumb;
    thumb.loadFromData((uchar*)thumbData,thumbSize,"png");
    m_Thumbnail = QPixmap::fromImage(thumb);
}

QPoint City::tilePosition()
{
    cout << "City Position " << m_RegionData->tileX << ", " << m_RegionData->tileY << endl;

    int bx = m_RegionData->tileX + m_RegionData->sizeX;
    int by = m_RegionData->tileY + m_RegionData->sizeY;

    int x = (bx * 90) - (by * 38);
    int y = (by * 45) + (bx * 19);

    return QPoint(x,y);
}

QPoint City::position()
{
    return QPoint(m_RegionData->tileX + m_RegionData->sizeX,m_RegionData->tileY + m_RegionData->sizeY);
}

QPixmap& City::thumbnail()
{
    return m_Thumbnail;
}

QString City::name()
{
    return QString::fromStdString(m_RegionData->cityName);
}
