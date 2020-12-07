#ifndef MAPINFO_H
#define MAPINFO_H

#include <QObject>
#include "stationattribute.h"
#include "PlatformParameter.h"
#include <QList>

//map
#define MAP_TEMP_FILE_NAME ("/tmp/map.json")
#define MAP_FILE_NAME      ("/home/pi/ftrCartCtl/map.json")
#define ROUTE_FILE_NAME    ("/home/pi/ftrCartCtl/route.json")

typedef struct _StationInfo_t
{
    ActionOnCrossType_e action = ACTION_PASS;//ACTION_PAUSE;
    volatile quint32 dist;
    volatile qint16 speed;
    volatile quint16 pauseTime;
    QString name;
    uint8_t CtlByte;
}StationInfo_t;

typedef enum _MapType_t
{
    Type_Unseal = 0,
    Type_Seal   = 1
}MapType_t;

class MapInfo// : public QObject
{
//    Q_OBJECT
public:
    explicit MapInfo();
    ~MapInfo();
    void loadMap(QString fileName);    

private:
    quint16 stationsNum;
    MapType_t mapType;
    QList<StationAttribute> mapList;
signals:

};

#endif // MAPINFO_H
