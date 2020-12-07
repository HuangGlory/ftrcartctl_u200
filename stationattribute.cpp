#include "stationattribute.h"

StationAttribute::StationAttribute()
{
    this->name = "";
    this->FDist = 0;
    this->BDist = 0;
    this->FSpeed= 0;
    this->BSpeed= 0;
}
StationAttribute::StationAttribute(QString name)
{
    this->name = name;
    this->FDist = 0;
    this->BDist = 0;
    this->FSpeed= 0;
    this->BSpeed= 0;
}
StationAttribute::StationAttribute(QString name,quint32 FDist,qint16 FSpeed,quint32 BDist,qint16 BSpeed)
{
    this->name = name;
    this->FDist = FDist;
    this->BDist = BDist;
    this->FSpeed= FSpeed;
    this->BSpeed= BSpeed;
}

quint32 StationAttribute::GetFDist()
{
    return this->FDist;
}
quint32 StationAttribute::GetBDist()
{
    return this->BDist;
}
qint16  StationAttribute::GetFSpeed()
{
    return this->FSpeed;
}
qint16  StationAttribute::GetBSpeed()
{
    return this->BSpeed;
}

QString StationAttribute::GetName()
{
    return this->name;
}

void StationAttribute::SetFDist(quint32 dist)
{
    this->FDist = dist;
}
void StationAttribute::SetBDist(quint32 dist)
{
    this->BDist = dist;
}
void StationAttribute::SetFSpeed(qint16 speed)
{
    this->FSpeed = speed;
}
void StationAttribute::SetBSpeed(qint16 speed)
{
    this->BSpeed = speed;
}

void StationAttribute::SetName(QString name)
{
    this->name = name;
}
