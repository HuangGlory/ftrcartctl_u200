#include "sb_thread.h"

SB_Thread::SB_Thread()
{
    //run in main thread
}

void SB_Thread::run()
{
    //run in sub thread
    qDebug()<<"SB Thread:"<<QThread::currentThreadId();
    this->isRunable = true;
    this->MotorCtrl.ProcessName = tr("SB_Thread:");

    while(this->isRunable)
    {
        msleep(1000);
        this->MotorCtrl.MotorVelocity.linearV  = 0;
        this->MotorCtrl.MotorVelocity.angularV = 0;
        this->UpdateVelocity();
    }
}

SB_Thread::~SB_Thread()
{
    this->quit();
    this->wait();
}

void    SB_Thread::stop()
{
    this->isRunable = false;
}

void    SB_Thread::UpdateVelocity(void)
{
    emit UpdateVelocitySignal(this->MotorCtrl);
}

void SB_Thread::CartStateSyncSlot(CartState_e CartState)
{
    this->CartState = CartState;
    qDebug()<<this->CartState;
}
