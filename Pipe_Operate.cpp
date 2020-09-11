#include "ftr_ctr3speedctl.h"

void FTR_CTR3SpeedCtl::OpenPipe(QFile *pipeFile,QIODevice::OpenMode flags)
{
    if(pipeFile->exists() && (!pipeFile->isOpen()))
    {
        if(pipeFile->open(flags))
        {
            qDebug()<<pipeFile->fileName()<<"Open File successful";
        }
        else
        {
            qDebug()<<"Open File faile";
        }
    }
}
//void FTR_CTR3SpeedCtl::ClosePipe(QFile *pipeFile)
//{
//    pipeFile->close();
//}

//void FTR_CTR3SpeedCtl::ClearPipe(QFile *pipeFile)
//{
//#if(0)
//    QString cmd = "sudo cat ";
//    cmd.append(pipeFile->fileName());
//    this->clearPipeProcess->start(cmd);
//    this->clearPipeProcess->waitForStarted(50);
//    //qDebug()<<cmd<<this->clearPipeProcess->readAllStandardError()<<this->clearPipeProcess->readAllStandardOutput();
//    QThread::sleep(1);
//    this->clearPipeProcess->kill();
//    this->clearPipeProcess->waitForFinished(50);
//    qDebug()<<cmd;
//#else
//    while(pipeFile->atEnd())
//    {
//        QString VTKInfo = pipeFile->readLine(0);
//        qDebug()<<VTKInfo;
//    }
//    qDebug()<<"Clear Pipe finished!";
//#endif
//}

void FTR_CTR3SpeedCtl::WriteMainPipeSlot(CartState_e cartState)
{
    //NONE = -1,STANDBY = 0,MOTORRELEASE = 1,VISIONFOLLOW = 2,VISIONTAPE = 3,CameraCali = 5,
    QString state ="0";
    if((cartState == STATE_SB) || (cartState == STATE_RC))
    {
        state = "0";
    }
    else if((cartState == STATE_MOTOR_RELEASE) || (cartState == STATE_CAMERA_RELEASE))
    {
        state = "1";
    }
    else if(cartState == STATE_VTK)
    {
        state = "2";
    }
    else if(cartState == STATE_VTP)
    {
        state = "3";
    }
    else if(cartState == STATE_CAMERACALI_AUTO)
    {
        state = "5";
    }
    else if(cartState == STATE_CAMERACALI_MANUAL)
    {
        state = "6";
    }
    this->MainInputPipeFile->write(state.toUtf8());
    this->MainInputPipeFile->flush();
    //qDebug()<<"WriteMainPipeSlot"<<cartState<<state.toUtf8();
}

void FTR_CTR3SpeedCtl::WriteOutPipeSlot(QString str)
{
    qDebug()<<str;
}

//void FTR_CTR3SpeedCtl::ReadVTPInfoFromPipeSlot()
//{
//    if(this->VTPOutputPipeFile->isReadable() && this->VTPOutputPipeFile->isOpen())
//    {
//        QString VTKInfo = this->VTKOutputPipeFile->readAll();
//        qDebug()<<VTKInfo;
//    }
//}
//void FTR_CTR3SpeedCtl::ReadVTKInfoFromPipeSlot()
//{
//    if(this->VTKOutputPipeFile->isReadable() && this->VTKOutputPipeFile->isOpen())
//    {
//        QString VTKInfo = this->VTKOutputPipeFile->readAll();
//        qDebug()<<VTKInfo;
//    }
//}

void FTR_CTR3SpeedCtl::UpdateVTKInfoSlot(VTKInfo_t VTKInfo)
{
    if(this->Wait4CameraReadyIndecateFlag) this->CartStateCtlProcess->SetCameraReadyFlagSlot(true);//camera ready
    this->Wait4CameraReadyIndecateFlag = false;
    this->VTKInfo.VTKDist = VTKInfo.VTKDist;
    this->VTKInfo.VTKAngle= VTKInfo.VTKAngle;
    this->VTK_RealTimeInfo();
    //qDebug()<<VTKInfo.VTKDist<<VTKInfo.VTKAngle;
}

void FTR_CTR3SpeedCtl::UpdateVTPInfoSlot(VTPInfo_t VTPInfo)
{
    if(this->ShowLogFlag.vtpRtLogFlag)
    {
    #if(1)
        QString str = QString("7P:(%1,%2,%3,%4,%5,%6,%7);VCtl:(%8);SI:(%9,%10):GM(%11)").arg(VTPInfo.PointOnTape[0]).arg(VTPInfo.PointOnTape[1]).arg(VTPInfo.PointOnTape[2]).arg(VTPInfo.PointOnTape[3])
                                .arg(VTPInfo.PointOnTape[4]).arg(VTPInfo.PointOnTape[5]).arg(VTPInfo.PointOnTape[6])
                                .arg(VTPInfo.SpeedCtl).arg(VTPInfo.StationName).arg(VTPInfo.ToStationDist).arg(VTPInfo.GotMarkFlag);
    #else
        QString str = QString("SI:(%1,%2)").arg(VTPInfo.StationName).arg(VTPInfo.ToStationDist);
    #endif
        qDebug()<<str;
    }
    //quint8 CalcArcIndex = 0,NumCnt = 0;

    if(this->Wait4CameraReadyIndecateFlag) this->CartStateCtlProcess->SetCameraReadyFlagSlot(true);//camera ready
    this->Wait4CameraReadyIndecateFlag = false;
    for(quint8 i = 0;i<7;i++)
    {
        this->VTPInfo.PointOnTape[i] = VTPInfo.PointOnTape[i];
    }

#if(PLATFORM == PLATFORM_U250)
    //speed up&down
    if((VTPInfo.SpeedCtl == SPEED_CTL_NULL) && (this->StartActionFlag || this->InArcTurningFlag))
    {
        this->VTPInfo.SpeedCtl = SPEED_CTL_NULL;
    }
    else if(((VTPInfo.SpeedCtl == SPEED_CTL_DOWN) && this->SpeedUpAndDownState) ||\
        ((VTPInfo.SpeedCtl == SPEED_CTL_UP) && !this->SpeedUpAndDownState))//current state is speed up
    {
        this->VTPInfo.SpeedCtl = VTPInfo.SpeedCtl;
    }

    #if(NUM_STATION_USED)
        /*if((VTPInfo.StationName == -1) && (this->StartActionFlag))
        {

        }
        else*/
        //if(VTPInfo.StationName != -1)
        //if(this->VTPInfo.ToStationDist != VTPInfo.ToStationDist)
        //don't catch the station mark in action or arc turning
        if(VTPInfo.StationName != this->VTPInfo.StationName)
        {
            if((!this->StartActionFlag) && (!this->InArcTurningFlag) && (!this->SpeedUpAndDownState))
            {
                this->VTPInfo.ToStationDist = VTPInfo.ToStationDist;
                if((this->SocketReadyFlag) && (this->VTPInfo.StationName != VTPInfo.StationName))
                {
                    QString StationNameStr = "Station:"+QString::number(VTPInfo.StationName) + "--Dist:"+QString::number(VTPInfo.ToStationDist);
                    this->tcpSocketSendMessageSlot(StationNameStr);
                    qDebug()<<StationNameStr;
                }
                else
                {
                    this->VTPInfo.StationName = VTPInfo.StationName;
                    //qDebug()<<"Station:"<<VTPInfo.StationName;
                }
            }
            else
            {
                qDebug()<<"GotStationNameInTurning:"<<VTPInfo.StationName;
            }
        }
    #elif(0)
        if((VTPInfo.StationName == 1) && (abs(this->RxInfo.ODO - this->ODOMark4VTPStationCalc) >= this->distBtStation))
        {
            this->ODOMark4VTPStationCalc = this->RxInfo.ODO;

            if(this->FaceDirFlag)//forward
            {
                this->StationName4VTP++;
            }
            else
            {
                this->StationName4VTP--;
            }

            if((!this->StartActionFlag) && (!this->InArcTurningFlag))
            {
                this->VTPInfo.ToStationDist = VTPInfo.ToStationDist;
                if((this->SocketReadyFlag) && (this->VTPInfo.StationName != VTPInfo.StationName))
                {
                    QString StationNameStr = "Station:"+QString::number(this->StationName4VTP) + "--Dist:"+QString::number(VTPInfo.ToStationDist);
                    this->tcpSocketSendMessageSlot(StationNameStr);
                    qDebug()<<StationNameStr;
                }
                else
                {
                    this->VTPInfo.StationName = this->StationName4VTP;
                    //qDebug()<<"Station:"<<VTPInfo.StationName;
                }
            }
            else
            {
                qDebug()<<"GotStationNameInTurning:"<<this->StationName4VTP;
            }
            qDebug()<<"StationN:"<<this->StationName4VTP<<this->FaceDirFlag;
        }
    #else
        //Got the mark in speed down and no in arc turning
        if(VTPInfo.GotMarkFlag && (!this->StartActionFlag) && (!this->InArcTurningFlag) && (!this->SpeedUpAndDownState) && (abs(this->RxInfo.ODO - this->ODOMark4VTPStationCalc) >= this->distBtStation))
        {
            this->ODOMark4VTPStationCalc = this->RxInfo.ODO;
            this->MarkCntRecord++;

            this->VTPInfo.ToStationDist = VTPInfo.ToStationDist;
            if(this->SocketReadyFlag)
            {
                QString StationNameStr = "Station:"+QString::number(VTPInfo.GotMarkFlag) + "--Dist:"+QString::number(VTPInfo.ToStationDist);
                this->tcpSocketSendMessageSlot(StationNameStr);
                qDebug()<<StationNameStr<<this->MarkCntRecord;
            }
            else
            {
                this->VTPInfo.StationName = VTPInfo.StationName;
                qDebug()<<"Station:"<<VTPInfo.StationName<<this->MarkCntRecord;
            }
        }
    #endif

#endif
    this->VTP_RealTimeInfo();
    //qDebug()<<"VTPRTInfo:";
}

void FTR_CTR3SpeedCtl::UpdatePipeInputSlot(QString str)
{
    qDebug()<<"InPipe:"<<str;
    if(str.contains("WheelCali:"))
    {
        str = str.replace("WheelCali:","");
        str = str.replace("\n","");
        QStringList RxMessageList = str.split(",", QString::SkipEmptyParts);

        if(RxMessageList.size() == 2)
        {
            bool convertResult = false;
            int converValue = RxMessageList.at(0).toInt(&convertResult);
            if(convertResult)
            {
                if(converValue)//need odo cali
                {
                    this->NeedIntoODOCaliFlag = true;
                    converValue = RxMessageList.at(1).toInt(&convertResult);
                    if(convertResult) this->WheelCaliDist = converValue;
                }
                else
                {
                    WriteWheelDiamToBaseJsonSlot();
                    this->NeedOutODOCaliFlag = true;
                    this->WheelCaliDist = 0;
                }
            }
        }
    }
    #if(PLATFORM == PLATFORM_R3)
    else if(str.contains("AccGyroCali:"))
    {
        //qDebug()<<"toCaliBias:";
        this->imuData->CalcBias();
    }
    #endif
}

#if(PLATFORM == PLATFORM_R3)
void FTR_CTR3SpeedCtl::UpdateIMUInfoSlot(Pose_t pose)
{
    //qDebug()<<"Pose:"<<pose.norm<<pose.pitch<<pose.roll<<pose.yaw;
    this->pose.norm = pose.norm;
    this->pose.pitch= pose.pitch;
    this->pose.roll = pose.roll;
    this->pose.yaw  = pose.yaw;
}
#endif

void FTR_CTR3SpeedCtl::fileChangedSlot(const QString & path)
{
    //qDebug()<<path;
    if(path.contains(this->VTPOutputPipeName))
    {
        //emit this->ReadVTPInfoSignal();
        QString cmd = "sudo cat ";
        cmd.append(path);
        //qDebug()<<cmd;
        QProcess clearPipeProcess;
        clearPipeProcess.start(cmd);
        if(!clearPipeProcess.waitForFinished(100))
        {
            clearPipeProcess.kill();
            clearPipeProcess.waitForFinished(100);
        }
    }
    else if(path.contains(this->VTKOutputPipeName))
    {
        //emit this->ReadVTKInfoSignal();
        QString cmd = "sudo cat ";
        cmd.append(path);
        //qDebug()<<cmd;
        QProcess clearPipeProcess;
        clearPipeProcess.start(cmd);
        if(!clearPipeProcess.waitForFinished(100))
        {
            clearPipeProcess.kill();
            clearPipeProcess.waitForFinished(100);
        }
    }
    else if(path.contains(this->SettingJsonFileName))
    {
        //cout()<<path.toStdString()<<endl;
        this->GetSettingParameterFromJson(this->SettingJsonFileName);
        this->VTP_InitParameter();
    }
}
