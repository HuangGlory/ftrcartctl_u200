#ifndef MOTORCTRLINFO_H
#define MOTORCTRLINFO_H

#include <QObject>

typedef struct _MotorVelocity_t
{
    volatile qint16 linearV;
    volatile qint16 angularV;
}MotorVelocity_t;

class MotorCtrlInfo
{
public:
    MotorCtrlInfo();

public:
    MotorVelocity_t MotorVelocity;
    QString ProcessName;
};

#endif // MOTORCTRLINFO_H
