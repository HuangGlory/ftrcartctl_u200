#include "readvtkpipe_thread.h"

ReadVTKPipe_Thread::ReadVTKPipe_Thread()
{
    //run in main thread
}

ReadVTKPipe_Thread::ReadVTKPipe_Thread(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
    qDebug()<<pipeFileName;
}

void ReadVTKPipe_Thread::run()
{
    qDebug()<<"Read VTK Pipe:"<<QThread::currentThreadId()<<this->pipeFileName;
    this->isRunable = true;
    this->VTKInfo.VTKDist = 0;
    this->VTKInfo.VTKAngle= 0;
    this->pipeFile  = new QFile(this->pipeFileName);
    if(pipeFile->exists() && (!pipeFile->isOpen()))
    {
        if(pipeFile->open(QIODevice::ReadOnly))
        {
            qDebug()<<pipeFile->fileName()<<"Open File successful:";
        }
        else
        {
            qDebug()<<"Open File faile";
        }
    }

    while(this->isRunable)
    {
        //if(this->pipeFile->canReadLine())
        //qDebug()<<"Size:"<<this->pipeFile->size()<<this->pipeFile->waitForReadyRead(200);
        //if(this->pipeFile->size() != 0)
        {
            QString RxInfo = this->pipeFile->readLine();
            //RxInfo = RxInfo.replace("\n","");

            QStringList RxInfoList = RxInfo.split(",", QString::SkipEmptyParts);
            //qDebug()<<RxInfo;

            if(3 == RxInfoList.size())//angle,dist2foot,distBaseHeight
            {
                bool convertAngleResult = false,convertDistResult = false,convertDistBaseHeighResult = false;
                int  convertAngleValue  = RxInfoList.at(0).toInt(&convertAngleResult);
                int  convertDistValue   = RxInfoList.at(1).toUInt(&convertDistResult);
                int  convertDistValueBaseHeigh  = RxInfoList.at(2).toUInt(&convertDistBaseHeighResult);

                if(convertAngleResult && convertDistResult && convertDistBaseHeighResult)
                {
                    this->VTKInfo.VTKAngle = convertAngleValue;
                    this->VTKInfo.VTKDist  = convertDistValue;
                    this->VTKInfo.VTKDistBaseHeight = convertDistValueBaseHeigh;
                    this->UpdateInfo();
                }
            }
        }
    }

    qDebug()<<"Stop ReadVTKPipe!";
    this->pipeFile->close();
    delete this->pipeFile;
}

void ReadVTKPipe_Thread::stop()
{
    this->isRunable = false;
}

void ReadVTKPipe_Thread::UpdateInfo()
{
    emit UpdateInfoSignal(this->VTKInfo);
}

void ReadVTKPipe_Thread::SetPipeFileName(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
}

ReadVTKPipe_Thread::~ReadVTKPipe_Thread()
{
    qDebug()<<"End Process";
}
