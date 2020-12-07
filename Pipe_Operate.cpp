#include "ftr_ctr3speedctl.h"
#include <stdlib.h>
#include <QPoint>

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

void FTR_CTR3SpeedCtl::WriteInfoToVisionPipeSlot(QString info)//cycle per second
{
    if(this->StateInfoInputPipeFile->size() == 0)
    {
        this->StateInfoInputPipeFile->write(info.toUtf8());
        this->StateInfoInputPipeFile->flush();
        //QFileInfo *fileInfo = new QFileInfo(this->StateInfoInputPipeFile->fileName());
        //qDebug()<<"ToVisionInfo:"<<info.toUtf8();
    }
    else
    {
        qDebug()<<"StateInfoInputPipe not empty:";
    }
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
#if(0)
    QVector<QPoint> pointInfo;
    for(quint8 i = 0;i<6;i++)
    {
        if(TAPE_ANGLE_LOST_VALUE != VTPInfo.PointOnTape[i])
        {
            QPoint validPoint = QPoint(i,VTPInfo.PointOnTape[i]);
            pointInfo.append(validPoint);
        }
    }
    if(pointInfo.size() == 1)//only one point it's invalid point
    {
        VTPInfo.PointOnTape[pointInfo.at(0).x()] = TAPE_ANGLE_LOST_VALUE;
//        qDebug()<<"GotInvalid:"<<pointInfo.at(0);
    }
    else if(pointInfo.size() == 2)
    {
        if(abs(VTPInfo.PointOnTape[pointInfo.at(0).x()] - VTPInfo.PointOnTape[pointInfo.at(1).x()]) > 100)//full sceen/4
        {
            VTPInfo.PointOnTape[pointInfo.at(0).x()] = TAPE_ANGLE_LOST_VALUE;
            VTPInfo.PointOnTape[pointInfo.at(1).x()] = TAPE_ANGLE_LOST_VALUE;
            qDebug()<<"GotInvalidPoint:2";
        }
    }
#endif
    if(this->Wait4CameraReadyIndecateFlag) this->CartStateCtlProcess->SetCameraReadyFlagSlot(true);//camera ready
    this->Wait4CameraReadyIndecateFlag = false;
    for(quint8 i = 0;i<7;i++)
    {
        this->VTPInfo.PointOnTape[i] = VTPInfo.PointOnTape[i];
    }

#if(PLATFORM == PLATFORM_U250)
    #if(0)
    //speed up&down
    if((VTPInfo.SpeedCtl == SPEED_CTL_NULL) || (this->StartActionFlag || this->InArcTurningFlag))
    {
        this->VTPInfo.SpeedCtl = SPEED_CTL_NULL;
    }
    else if(((VTPInfo.SpeedCtl == SPEED_CTL_DOWN) && this->SpeedUpAndDownState) ||\
        ((VTPInfo.SpeedCtl == SPEED_CTL_UP) && !this->SpeedUpAndDownState))//current state is speed up
    {
        this->VTPInfo.SpeedCtl = VTPInfo.SpeedCtl;
    }
    //qDebug()<<"SCTL:"<<this->VTPInfo.SpeedCtl;
    #endif
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
//        qDebug()<<VTPInfo.GotMarkFlag<<this->StartActionFlag<<this->SpeedUpAndDownState<<abs(this->RxInfo.ODO - this->ODOMark4VTPStationCalc);
        //Got the mark in speed down and no in arc turning
        //if(VTPInfo.GotMarkFlag && (!this->StartActionFlag) && (!this->InArcTurningFlag) && (!this->SpeedUpAndDownState) && (abs(this->RxInfo.ODO - this->ODOMark4VTPStationCalc) >= this->distBtStation))
//        if(VTPInfo.GotMarkFlag && (!this->StartActionFlag) && (!this->SpeedUpAndDownState) && (abs(this->RxInfo.ODO - this->ODOMark4VTPStationCalc) >= this->distBtStation))
//        if(VTPInfo.GotMarkFlag && (!this->StartActionFlag) && (abs(this->RxInfo.ODO - this->ODOMark4VTPStationCalc) >= this->distBtStation))
        if(VTPInfo.GotMarkFlag && (!this->StartActionFlag) && ((this->RxInfo.ODO - this->ODOMark4VTPStationCalc) >= this->distBtStation))
        {            
            if(this->FaceDirFlag)//forward
            {
                this->StationName4VTP++;
            }
            else
            {
                this->StationName4VTP--;
            }
            this->MarkCntRecord++;
        #if(CREATE_MAP_USED)
            if(this->inBuildMapModeFlag)
            {
//                if(this->mapIsSealTypeFlag )
                this->CreateMapSlot(QString::number(this->StationName4VTP),(this->RxInfo.ODO - this->ODOMark4VTPStationCalc + VTPInfo.ToStationDist),this->CartModeEnterParameter.maxSpeed,this->FaceDirFlag,(bool)(1==this->MarkCntRecord));
                this->EmitBeepSound(BEEP_TYPE_SHORT_BEEP_1);
            }
        #endif

            this->remainDistToStation = VTPInfo.ToStationDist;
        #if(ROUNT_SIMULATOR)
            if(this->MarkCntRecord == (this->stationsInRoute - 1))
            {
                this->isTheLastStationFlag = true;
//                qDebug()<<"EndS:"<<this->MarkCntRecord<<this->stationsInRoute;
            }

            if(this->MarkCntRecord < this->stationsInRoute)
            {
                StationInfo_t currentStationInfo = this->GetActionBaseStationName(QString::number(this->MarkCntRecord),true);//forword
                this->VTPInfo.StationName   = currentStationInfo.name.toUInt();
                this->VTPInfo.PauseTime     = currentStationInfo.pauseTime;
                this->VTPInfo.MaxSpeed      = currentStationInfo.speed;
                this->VTPInfo.ToStationDist = currentStationInfo.dist;
                this->VTP_UpdateAction(currentStationInfo.action);
                this->VTPInfo.CtlByte       = currentStationInfo.CtlByte;
            }
            else
            {
                qDebug()<<"station num too more:";
            }

        #else
            if(this->SocketReadyFlag)// && !this->inBuildMapModeFlag)
            {
                this->stationInfoToSocket = "Station:"+QString::number(VTPInfo.GotMarkFlag) + "," +QString::number(VTPInfo.ToStationDist);
                //this->tcpSocketSendMessageSlot(this->stationInfoToSocket);
                this->retrySendStationInfoToSocketFlag = true;
                qDebug()<<this->stationInfoToSocket<<this->MarkCntRecord<<(this->RxInfo.ODO - this->ODOMark4VTPStationCalc + VTPInfo.ToStationDist);
            }
            else
            {
                this->VTPInfo.StationName = VTPInfo.StationName;
                qDebug()<<"StationNormal:"<<VTPInfo.StationName<<this->StationName4VTP<<this->FaceDirFlag<<this->MarkCntRecord<<VTPInfo.ToStationDist;
            }
        #endif

            this->ODOMark4VTPStationCalc = (this->RxInfo.ODO + VTPInfo.ToStationDist);
        }
    #endif

#endif
    this->VTP_RealTimeInfo();
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
                    converValue = RxMessageList.at(1).toInt(&convertResult);
                    if(converValue)
                    {
                        WriteWheelDiamToBaseJsonSlot();
                        qDebug()<<"Diam Cali Save:";
                    }
                    else
                    {
						qDebug()<<"Diam Cali Cancle:";
					}
					
					this->NeedOutODOCaliFlag = true;
					this->WheelCaliDist = 0;
                }
            }
        }
    }
    else if(str.contains("ModeChange:"))
    {
        str = str.replace("ModeChange:","");
        str = str.replace("\n","");
        this->streamlitKeepRunFlag = true;
        if(str.contains("SB",Qt::CaseInsensitive))
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
            this->streamlitKeepRunFlag = false;
        }
        else if(str.contains("VTK",Qt::CaseInsensitive))
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_VTK);
        }
        else if(str.contains("VTP",Qt::CaseInsensitive))
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_VTP);
        }
        else if(str.contains("MR",Qt::CaseInsensitive))
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_MOTOR_RELEASE);
        }
    #if(CREATE_MAP_USED)
        else if(str.contains("BuildMap"))
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_VTP);
            this->MarkCntRecord             = 0;
            this->StationName4VTP           = 0;
            this->FaceDirFlag               = true;//forward
            this->ODOMark4VTPStationCalc    = this->RxInfo.ODO;
            this->inBuildMapModeFlag        = true;

            //clear map json
            QFile *file = new QFile(MAP_TEMP_FILE_NAME);
            if(file->exists()) file->remove();
            QJsonObject obj;
            obj.insert("stations_num",1);
            obj.insert("type",0);//it's unseal type
            obj.insert("0->0","0,0");
            this->mapIsSealTypeFlag = false;//it's unseal type

            str = str.replace("BuildMap:","");
            str = str.replace("\n","");
            if(!str.isEmpty())
            {
                QStringList RxMessageList = str.split(",", QString::SkipEmptyParts);
                if(RxMessageList.size() == 2)
                {
                    bool convertResult = false;
                    int converValue = RxMessageList.at(0).toInt(&convertResult);
                    if(convertResult && converValue)
                    {
                        obj.insert("type",1);//it's seal type
                        this->mapIsSealTypeFlag = true;//it's seal type
                    }

                    this->totalStationsNum = RxMessageList.at(1).toInt(&convertResult);
                }
            }

            QJsonDocument doc;
            doc.setObject(obj);

            file->open(QIODevice::WriteOnly|QIODevice::Truncate);
            file->seek(0);
            file->write(doc.toJson());
            file->flush();
            file->close();

            delete file;
        }        
    #endif
    }
#if(CREATE_MAP_USED)
    else if(str.contains("MapSave:"))
    {
        QString cmd = "sudo cp -f " + MAP_TEMP_FILE_NAME + " " + MAP_FILE_NAME;

        QProcess *process = new QProcess();
        process->start(cmd);
        process->waitForStarted();
        process->waitForFinished();

        process->start("sync");
        process->waitForStarted();
        process->waitForFinished();

        this->inBuildMapModeFlag        = false;
    }
    else if(str.contains("VTPInit:"))//face dir,current station
    {
        str = str.replace("VTPInit:","");
        str = str.replace("\n","");
        QStringList RxMessageList = str.split(",", QString::SkipEmptyParts);

        if(RxMessageList.size() == 2)
        {
            this->FaceDirFlag = (bool)(RxMessageList.at(0).toUInt());
            this->StationName4VTP = RxMessageList.at(1).toUInt();
//            qDebug()<<this->FaceDirFlag<<this->StationName4VTP;
        }
    }
#endif
    else if(str.contains("ResetWIFI:"))
    {
        ResetWIFISlot();
    }
    else if(str.contains("WIFISetting:"))
    {
        str = str.replace("WIFISetting:","");
        str = str.replace("\n","");

        QStringList RxMessageList = str.split(",", QString::SkipEmptyParts);

        if(RxMessageList.size() == 2)
        {
            QString ssid = RxMessageList.at(0);
            QString pwd  = RxMessageList.at(1);

            QString wpaStr = WPA_SUPPLICANT_CONF_HEADER;
            QString appendSSIDStr = QString("network={\n    ssid=\"%1\"\n    psk=\"%2\"\n    key_mgmt=WPA-PSK\n    priority=10\n}\n").arg(ssid).arg(pwd);
            wpaStr.append(appendSSIDStr);

            QFile *file = new QFile("/home/pi/ftrCartCtl/wpa_supplicant.conf");
            if(file->exists()) file->remove();
            if(file->open(QIODevice::WriteOnly))
            {
                file->write(wpaStr.toLocal8Bit().data());
                file->flush();
                file->close();
            }
            delete file;

            QString cmd = "sudo cp -f /home/pi/ftrCartCtl/wpa_supplicant.conf " + WPA_SUPPLICANT_CONF_FILE_NAME;
            QProcess *process = new QProcess();
            process->start(cmd);
            process->waitForStarted();
            process->waitForFinished();

            process->start("sync");
            process->waitForStarted();
            process->waitForFinished();

            cmd = "wpa_cli -i wlan0 reconfig";
            process->start(cmd);
            process->waitForStarted();
            process->waitForFinished();
            delete process;

            this->EmitBeepSound(BEEP_TYPE_SHORT_BEEP_2);
        }
    }
#if(PLATFORM == PLATFORM_R3)
    else if(str.contains("AccGyroCali:"))
    {
        //qDebug()<<"toCaliBias:";
        this->imuData->CalcBias();
    }
#endif
#if(STREAMLIT_USED)
    else if(str.contains("StartStreamlit:"))
    {
        this->StartStreamlitUISlot();
    }
    else if(str.contains("StopStreamlit:"))
    {
        this->StopStreamlitUISlot();
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
    else if(path.contains(ROUTE_FILE_NAME))
    {
        this->loadRount(ROUTE_FILE_NAME);
    }
}
