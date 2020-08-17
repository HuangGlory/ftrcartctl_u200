#ifndef RC_THREAD_H
#define RC_THREAD_H
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "motorctrlinfo.h"
#include "PlatformParameter.h"

class RC_Thread:public QThread
{
    Q_OBJECT
public:
    RC_Thread();
    ~RC_Thread();

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

#endif // RC_THREAD_H
