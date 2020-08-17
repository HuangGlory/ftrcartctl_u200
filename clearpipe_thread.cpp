#include "clearpipe_thread.h"

ClearPipe_Thread::ClearPipe_Thread()
{
    //run in main thread
}

ClearPipe_Thread::ClearPipe_Thread(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
    qDebug()<<pipeFileName;
}

void ClearPipe_Thread::run()
{
    qDebug()<<"Clear VTK Pipe:"<<QThread::currentThreadId()<<this->pipeFileName;
    this->isRunable = true;
    this->pipeFile  = new QFile(this->pipeFileName);
    if(pipeFile->exists() && (!pipeFile->isOpen()))
    {
        if(pipeFile->open(QIODevice::ReadOnly))
        {
            qDebug()<<pipeFile->fileName()<<"Open File successful for Clear:";
        }
        else
        {
            qDebug()<<"Open File faile";
        }
    }

    while(this->isRunable)
    {
        //if(this->pipeFile->canReadLine())
        //if(this->pipeFile->size() != 0)
        {
            QString RxInfo = this->pipeFile->readLine();
            //qDebug()<<RxInfo;
        }
    }

    qDebug()<<"Stop Clear Pipe!";
    this->pipeFile->close();
    delete this->pipeFile;
}

void ClearPipe_Thread::stop()
{
    this->isRunable = false;
}

void ClearPipe_Thread::SetPipeFileName(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
}

ClearPipe_Thread::~ClearPipe_Thread()
{
    qDebug()<<"End Process";
}
