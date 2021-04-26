#include "clearpipe_thread.h"

ClearPipe_Thread::ClearPipe_Thread()
{
    //run in main thread
}

ClearPipe_Thread::ClearPipe_Thread(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
    qDebug()<<"ClearPipe_Thread:"<<pipeFileName;
}

void ClearPipe_Thread::run()
{
    qDebug()<<"Clear Pipe:"<<QThread::currentThreadId()<<this->pipeFileName;
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
            qDebug()<<pipeFile->fileName()<<"Open File faile";
            this->stop();
        }
    }
    else
    {
        qDebug()<<"Clear Pipe:"<<this->pipeFileName<<pipeFile->exists()<<pipeFile->isOpen();
        this->stop();
    }

    while(this->isRunable)
    {
        //if(this->pipeFile->canReadLine())
        //if(this->pipeFile->size() != 0)
        if(this->pipeFile->isReadable())
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
