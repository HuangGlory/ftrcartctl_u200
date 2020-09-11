#include "readvtppipe_thread.h"

ReadVTPPipe_Thread::ReadVTPPipe_Thread()
{
    //run in main thread
}

ReadVTPPipe_Thread::ReadVTPPipe_Thread(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
    qDebug()<<pipeFileName;
}

void ReadVTPPipe_Thread::run()
{
    qDebug()<<"Read VTP Pipe:"<<QThread::currentThreadId();
    this->isRunable = true;
    this->pipeFile  = new QFile(this->pipeFileName);
    if(pipeFile->exists() && (!pipeFile->isOpen()))
    {
        if(pipeFile->open(QIODevice::ReadOnly))
        {
            qDebug()<<pipeFile->fileName()<<"Open File successful";
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
            QString VTPInfoStr = this->pipeFile->readLine();

            VTPInfoStr = VTPInfoStr.replace("\n","");

            QStringList RxInfoList = VTPInfoStr.split(",", QString::SkipEmptyParts);
            //qDebug()<<RxInfoList;
        #if(PLATFORM == PLATFORM_U250)
            if(11 == RxInfoList.size())//7points,SpeedUp/Down,StationName,ToStationDist,10*10Mark
            {
                for(quint8 i = 0;i<7;i++)
                {
                    this->VTPInfo.PointOnTape[i] = RxInfoList.at(i).toInt();
                }
                this->VTPInfo.SpeedCtl      = (SpeedCtl_e)(RxInfoList.at(7).toInt());
                this->VTPInfo.StationName   = (RxInfoList.at(8).toInt());
                this->VTPInfo.ToStationDist = (RxInfoList.at(9).toUInt());
                this->VTPInfo.GotMarkFlag   = (bool)(RxInfoList.at(10).toInt());
                //qDebug()<<this->VTPInfo.SpeedCtl;
                this->UpdateInfo();
                //emit UpdateInfoSignal(VTPInfo);
            }
        #elif(PLATFORM_R3 == PLATFORM)
            if(10 == RxInfoList.size())
            {
                for(quint8 i = 0;i<7;i++)
                {
                    this->VTPInfo.PointOnTape[i] = RxInfoList.at(i).toInt();
                }
                this->VTPInfo.SpeedCtl = (SpeedCtl_e)(RxInfoList.at(7).toInt());
                this->VTPInfo.StationName = (RxInfoList.at(8).toInt());
                this->VTPInfo.ToStationDist = (RxInfoList.at(9).toUInt());
                //qDebug()<<this->VTPInfo.SpeedCtl;
                this->UpdateInfo();
                //emit UpdateInfoSignal(VTPInfo);
            }
        #endif
        }
    }

    qDebug()<<"Stop ReadVTPPipe!";
    this->pipeFile->close();
    delete this->pipeFile;
}

void ReadVTPPipe_Thread::stop()
{
    this->isRunable = false;
}

void ReadVTPPipe_Thread::UpdateInfo()
{
    emit UpdateInfoSignal(this->VTPInfo);
}

void ReadVTPPipe_Thread::SetPipeFileName(QString pipeFileName)
{
    this->pipeFileName = pipeFileName;
}

ReadVTPPipe_Thread::~ReadVTPPipe_Thread()
{
    qDebug()<<"End Process";
}
