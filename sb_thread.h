#ifndef SB_THREAD_H
#define SB_THREAD_H
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "motorctrlinfo.h"
#include "PlatformParameter.h"

class SB_Thread:public QThread
{
    Q_OBJECT
public:
    SB_Thread();
    ~SB_Thread();

private:
    virtual void run() override;
    void    UpdateVelocity(void);

public:
    MotorCtrlInfo MotorCtrl;
    CartState_e CartState;

signals:
    void UpdateVelocitySignal(MotorCtrlInfo);

public slots:
    void CartStateSyncSlot(CartState_e CartState);

public:
    void stop(void);
    bool isRunable;
};

#endif // SB_THREAD_H
