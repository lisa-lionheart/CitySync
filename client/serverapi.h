#ifndef SERVERAPI_H
#define SERVERAPI_H


#include <QString>
class Region;



class ServerApi
{
public:
    ServerApi();


    Region* fetchRegion(QString name);

};

#endif // SERVERAPI_H
