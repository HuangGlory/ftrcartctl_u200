#ifndef VTK_THREAD_H
#define VTK_THREAD_H
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "motorctrlinfo.h"
#include "PlatformParameter.h"

class VTK_Thread : public QThread
{
    Q_OBJECT
public:
    VTK_Thread();
    ~VTK_Thread();

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

#endif // VTK_THREAD_H
