#include "readinputpipe_thread.h"

ReadInputPipe_Thread::ReadInputPipe_Thread()
{
    //run in main thread
}

ReadInputPipe_Thread::ReadInputPipe_Thread(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
    qDebug()<<"ReadInputPipe_Thread:"<<pipeFileName;
}

void ReadInputPipe_Thread::run()
{
    qDebug()<<"Read Input Pipe:"<<QThread::currentThreadId()<<this->pipeFileName;
    this->isRunable = true;
    this->pipeFile  = new QFile(this->pipeFileName);
    if(this->pipeFile->exists() && (!this->pipeFile->isOpen()))
    {
        if(this->pipeFile->open(QIODevice::ReadWrite))//no block
        {
            qDebug()<<this->pipeFile->fileName()<<"Open File successful:";
        }
        else
        {
            qDebug()<<this->pipeFile->fileName()<<"Open File faile";
            this->stop();
        }
    }
    else
    {
        qDebug()<<"Read input Pipe:"<<this->pipeFileName<<pipeFile->exists()<<pipeFile->isOpen();
        this->stop();
    }

    while(this->isRunable)
    {
        //if(this->pipeFile->canReadLine())
        if(this->pipeFile->isReadable())
        {
            QString RxInfo = this->pipeFile->readLine();
            if(!RxInfo.isEmpty())  emit UpdateInfoSignal(RxInfo);
        }
    }
    qDebug()<<"Stop ReadInputPipe!";
    this->pipeFile->close();
    delete this->pipeFile;
}

void ReadInputPipe_Thread::stop()
{
    this->isRunable = false;
}

void ReadInputPipe_Thread::SetPipeFileName(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
}

ReadInputPipe_Thread::~ReadInputPipe_Thread()
{
    qDebug()<<"End Process";
}
