#ifndef STATIONATTRIBUTE_H
#define STATIONATTRIBUTE_H

#include <QObject>

class StationAttribute
{
public:
    StationAttribute();
    StationAttribute(QString name);
    StationAttribute(QString name,quint32 FDist,qint16 FSpeed,quint32 BDist,qint16 BSpeed);

public:
    quint32 GetFDist();
    quint32 GetBDist();
    qint16  GetFSpeed();
    qint16  GetBSpeed();
    QString GetName();

    void SetFDist(quint32 dist);
    void SetBDist(quint32 dist);
    void SetFSpeed(qint16 speed);
    void SetBSpeed(qint16 speed);
    void SetName(QString name);

public:
    QString name;
    quint32 FDist;
    quint32 BDist;
    qint16  FSpeed;
    qint16  BSpeed;
};

#endif // STATIONATTRIBUTE_H
