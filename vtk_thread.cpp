#include "vtk_thread.h"

VTK_Thread::VTK_Thread()
{
    //run in main thread
}

void VTK_Thread::run()
{
    //run in sub thread
    qDebug()<<"VTK Thread:"<<QThread::currentThreadId();
    this->isRunable = true;
    this->MotorCtrl.ProcessName = tr("VTK_Thread:");

    while(this->isRunable)
    {
        msleep(1000);
        this->MotorCtrl.MotorVelocity.linearV  = 0;
        this->MotorCtrl.MotorVelocity.angularV = 0;
        this->UpdateVelocity();
    }
}

VTK_Thread::~VTK_Thread()
{
    this->quit();
    this->wait();
}

void    VTK_Thread::stop()
{
    this->isRunable = false;
}

void    VTK_Thread::UpdateVelocity(void)
{
    emit UpdateVelocitySignal(this->MotorCtrl);
}

void VTK_Thread::CartStateSyncSlot(CartState_e CartState)
{
    this->CartState = CartState;
    qDebug()<<this->CartState;
}
