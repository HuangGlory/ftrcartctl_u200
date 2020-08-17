#include "rc_thread.h"

RC_Thread::RC_Thread()
{
    //run in main thread
}

void RC_Thread::run()
{
    //run in sub thread
    qDebug()<<"RC Thread:"<<QThread::currentThreadId();
    this->isRunable = true;
    this->MotorCtrl.ProcessName = tr("RC_Thread:");

    while(this->isRunable)
    {
        msleep(1000);
        this->MotorCtrl.MotorVelocity.linearV  = 0;
        this->MotorCtrl.MotorVelocity.angularV = 0;
        this->UpdateVelocity();
    }
}

RC_Thread::~RC_Thread()
{
    this->quit();
    this->wait();
}

void    RC_Thread::stop()
{
    this->isRunable = false;
}

void    RC_Thread::UpdateVelocity(void)
{
    emit UpdateVelocitySignal(this->MotorCtrl);
}

void RC_Thread::CartStateSyncSlot(CartState_e CartState)
{
    this->CartState = CartState;
    qDebug()<<this->CartState;
}
