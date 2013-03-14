#include "region.h"
#include "city.h"
#include "sc4savegame.h"

#include <QDir>

Region::Region(QString path)
{

    QDir regionDirectory(path);

    QStringList filter;
    filter.append("*.sc4");
    QStringList cityFiles = regionDirectory.entryList(filter);

    for(int i=0; i < cityFiles.size(); i++)
    {
        City* city = new City(this);
        city->loadSave(path + cityFiles[i]);
        m_Cities.append(city);

    }
}
