#include "ftr_ctr3speedctl.h"
#include <QThread>

class FTR_CTR3SpeedCtlData : public QSharedData
{
public:

};

FTR_CTR3SpeedCtl::FTR_CTR3SpeedCtl(QObject *parent) : QObject(parent), data(new FTR_CTR3SpeedCtlData)
{
    qDebug()<<"R3 Process Main Thread ID:"<<QThread::currentThreadId();
    qRegisterMetaType<MotorCtrlInfo>("MotorCtrlInfo");
    qRegisterMetaType<RxInfo_t>("RxInfo_t");
    qRegisterMetaType<VTKInfo_t>("VTKInfo_t");
    qRegisterMetaType<VTPInfo_t>("VTPInfo_t");
#if(PLATFORM == PLATFORM_R3)
    qRegisterMetaType<Pose_t>("Pose_t");
#endif

    //this->test();    
    this->batCapacityInfo.voltage       = BAT_MAX_VOLTAGE;
    this->batCapacityInfo.lessThan10Flag = false;
    this->batCapacityInfo.lessThen20Flag = false;

    this->InSetOAGlobalFlag             = false;
    this->SocketReadyFlag               = false;
    this->EboxReadyFlag                 = false;
    this->Wait4CameraReadyIndecateFlag  = false;
    this->GetVersionOfEBoxFlag          = false;
    this->CartStateCtlProcess           = new CartStateSetting("/home/pi/vision/data/settings.json");//("/home/pi/openvino/ftr/data/settings.json");
    this->JsonFileName                  = JSON_FILE_NAME;
    this->JsonFile                      = new QFile;

    this->SettingJsonFileName   = SETTING_JSON_FILE_NAME;
    this->SettingJsonFile       = new QFile;

    this->SBProcess = new SB_Thread;
    this->RCProcess = new RC_Thread;
    this->VTKProcess= new VTK_Thread;
#if(PLATFORM == PLATFORM_R3)
    this->imuData       = new imu();
#endif

    this->fileWatcher   = new QFileSystemWatcher;

    this->Time2Loop                 = new QTimer;
    this->Time2SendData             = new QTimer;
    this->PauseToGoTimer            = new QTimer;
    this->RecoverTimer              = new QTimer;

#if(BLUETOOTH_SERIAL_USED)
    this->Time4RCTimeout            = new QTimer;
#endif
    this->CartState                 = STATE_SB;
    this->LeftSettingSpeed          = 0;
    this->RightSettingSpeed         = 0;

    this->CartState                 = STATE_SB;
    this->PreCartState              = STATE_POWERON;
    this->HSWant2State              = STATE_POWERON;
    this->TaskFlag.TaskFlagKeepSendModeChange = true;

    //this->CartInPauseState          = true;//it in pause when power on
    this->CartWantToPNGState          = false;//this->CartInPauseState;

    this->SetODOFactor();
    this->GetPipeName(this->JsonFileName);
    this->GetSettingParameterFromJson(this->SettingJsonFileName);

    this->MainInputPipeFile         = new QFile(this->MainInputPipeName);
    this->VTKOutputPipeFile         = new QFile(this->VTKOutputPipeName);
    this->VTPOutputPipeFile         = new QFile(this->VTPOutputPipeName);

    this->ReadVTKPipeProcess        = new ReadVTKPipe_Thread(this->VTKOutputPipeName);
    this->ClearVTKPipeProcess       = new ClearPipe_Thread(this->VTKOutputPipeName);

    this->ReadVTPPipeProcess        = new ReadVTPPipe_Thread(this->VTPOutputPipeName);
    this->ClearVTPPipeProcess       = new ClearPipe_Thread(this->VTPOutputPipeName);

    this->theMinR                   = 1000;
    this->ArcInfo.ArcLocation       = NULL_LOCALTION;

#if(BLUETOOTH_SERIAL_USED)
    this->BTCtlProcess              = new BluetoothSerialPort;
#endif

#if(1)
    //wait pipe ready
    qDebug()<<"Wait Pipe file...";
    while(!(this->MainInputPipeFile->exists() && this->VTKOutputPipeFile->exists() && this->VTPOutputPipeFile->exists()));
    qDebug()<<"Pipe file ready";
#endif

    this->tcpSocket                 = new QTcpSocket(this);
    this->tcpServer                 = new QTcpServer(this);
    this->tcpServer->listen(QHostAddress::Any,50006);//smart phone
    this->tcpServer->setMaxPendingConnections(2);

    this->OpenPipe(this->MainInputPipeFile,QIODevice::WriteOnly);
    //this->OpenPipe(this->VTKOutputPipeFile,QIODevice::ReadOnly);
    //this->OpenPipe(this->VTPOutputPipeFile,QIODevice::ReadOnly);


    //this->fileWatcher->addPath(this->MainInputPipeName);
    //this->fileWatcher->addPath(this->VTKOutputPipeName);
    //this->fileWatcher->addPath(this->VTPOutputPipeName);
    this->fileWatcher->addPath(this->SettingJsonFileName);

    this->WriteMainPipeSlot(STATE_SB);
    this->VTP_InitParameter();

    //save log file
    QString logFileNameStr       = LOG_PATH_NAME;
    logFileNameStr.append(QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss")));
    this->SaveLogFile           = new QFile(logFileNameStr);
    if(!this->SaveLogFile->isOpen())
    {
        if(this->SaveLogFile->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            cout<<"file open sucessful"<<endl;
        }
        else
        {
            cout<<"file open faile"<<endl;
        }
    }

    {
        QDir dir(LOG_PATH_NAME);
        dir.setFilter(QDir::Files);
        QStringList files = dir.entryList();
        QDateTime time;
        uint currentDateTime = QDateTime::currentDateTime().toTime_t();
        foreach(QString fileStr, files)
        {
            time = QDateTime::fromString(fileStr,"yyyyMMdd-HH-mm-ss");
            uint detTime = currentDateTime - time.toTime_t();//un:second
            if(detTime >= 432000)//5*24*60*60 aboue 5 days
            {
                QFile file(LOG_PATH_NAME + fileStr);
                file.remove();
                //qDebug()<<LOG_PATH_NAME + fileStr<<detTime;
            }
        }

    }


#if(1)//communication with E-BOX seriap port
    this->AppUART   = new QSerialPort;    
    this->UartName = TTL_UART_NAME;

    this->AppUART->setBaudRate(QSerialPort::Baud115200);
    this->AppUART->setParity(QSerialPort::NoParity);
    this->AppUART->setDataBits(QSerialPort::Data8);
    this->AppUART->setStopBits(QSerialPort::OneStop);
    this->AppUART->setFlowControl(QSerialPort::NoFlowControl);

    this->AppUART->setPortName(this->UartName);
    if(this->AppUART->open(QIODevice::ReadWrite))
    {
        connect(this->AppUART,SIGNAL(readyRead()),this,SLOT(ReadUARTSlot()));
        qDebug()<<this->AppUART->portName()<<"UartOpen OK";
    }
    else
    {
        qDebug()<<this->AppUART->portName()<<"Uart Open Fail";
    }
#endif

    connect(this,SIGNAL(ReadVTKInfoSignal()),this,SLOT(ReadVTKInfoFromPipeSlot()));
    connect(this,SIGNAL(ReadVTPInfoSignal()),this,SLOT(ReadVTPInfoFromPipeSlot()));
    connect(this->Time2Loop,SIGNAL(timeout()),this,SLOT(Time2LoopSlot()));
    connect(this->Time2SendData,SIGNAL(timeout()),this,SLOT(Timer2SendDataSlot()));
    connect(this->PauseToGoTimer,SIGNAL(timeout()),this,SLOT(PauseToGoTimerSlot()));
    connect(this->RecoverTimer,SIGNAL(timeout()),this,SLOT(RecoverOATimerSlot()));

    connect(this->ReadVTKPipeProcess,SIGNAL(UpdateInfoSignal(VTKInfo_t)),this,SLOT(UpdateVTKInfoSlot(VTKInfo_t)));
    connect(this->ReadVTPPipeProcess,SIGNAL(UpdateInfoSignal(VTPInfo_t)),this,SLOT(UpdateVTPInfoSlot(VTPInfo_t)));
#if(PLATFORM == PLATFORM_R3)
    connect(this->imuData,SIGNAL(UpdateInfoSignal(Pose_t)),this,SLOT(UpdateIMUInfoSlot(Pose_t)));
#endif

    connect(this->fileWatcher,SIGNAL(fileChanged(const QString &)),this,SLOT(fileChangedSlot(const QString &)));
    connect(this->CartStateCtlProcess,SIGNAL(BroadcastCartStateSignal(CartState_e)),this,SLOT(CartStateSyncSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(BroadcastCartStateSignal(CartState_e)),this,SLOT(WriteMainPipeSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(SettingOAToggleSignal(CartState_e)),this,SLOT(SettingOAToggleSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(PNGButtonToggleSignal()),this,SLOT(PNGButtonToggleSlot()));
#if(BLUETOOTH_SERIAL_USED)
    connect(this->BTCtlProcess,SIGNAL(BTRxRCCMDSignal(RCCMD_e)),this,SLOT(BTRCCmdSlot(RCCMD_e)));
    connect(this->Time4RCTimeout,SIGNAL(timeout()),this,SLOT(Time4RCTimeoutSlot()));
#endif

#if(PLATFORM == PLATFORM_R3)
    connect(this->SBProcess,SIGNAL(UpdateVelocitySignal(MotorCtrlInfo)),this,SLOT(UpdateVelocitySlot(MotorCtrlInfo)));
    connect(this->RCProcess,SIGNAL(UpdateVelocitySignal(MotorCtrlInfo)),this,SLOT(UpdateVelocitySlot(MotorCtrlInfo)));
    connect(this->VTKProcess,SIGNAL(UpdateVelocitySignal(MotorCtrlInfo)),this,SLOT(UpdateVelocitySlot(MotorCtrlInfo)));

    connect(this->CartStateCtlProcess,SIGNAL(BroadcastCartStateSignal(CartState_e)),this->SBProcess,SLOT(CartStateSyncSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(BroadcastCartStateSignal(CartState_e)),this->RCProcess,SLOT(CartStateSyncSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(BroadcastCartStateSignal(CartState_e)),this->VTKProcess,SLOT(CartStateSyncSlot(CartState_e)));
#else
    this->SBProcess->stop();
    this->RCProcess->stop();
    this->VTKProcess->stop();
#endif

    connect(this,SIGNAL(toCalcCapacitySignal(double)),this,SLOT(calcCapacitySlot(double)));
    connect(this->tcpServer,SIGNAL(newConnection()),this,SLOT(tcpServerConnectionSlot()));

//    this->Time2Loop->start(LOOP_PER);
//    this->Time2SendData->start(SEND_DATA_PER);

}

void FTR_CTR3SpeedCtl::PNGButtonToggleSlot()
{
    //this->TaskFlag.TaskFlagNeedToChangePNGStete = true;
    this->CartWantToPNGState = !this->CartInPauseState;
    ++this->cnt4IntoConfigModePNGToggle;

    if(this->SocketReadyFlag)
    {
        QString StationNameStr = "Pause&Go:"+QString::number(this->CartWantToPNGState);
        this->tcpSocketSendMessageSlot(StationNameStr);
    }
    //qDebug()<<"PnG:"<<this->CartWantToPNGState<< this->CartInPauseState;
}

void FTR_CTR3SpeedCtl::SettingOAToggleSlot(CartState_e cartState)
{
    if(cartState == STATE_VTK)
    {
        this->VTKInfo.CtlByte    &= 0x3F;
        qDebug("SetOA@VTK:0x%x\n",this->VTKInfo.CtlByte);
    }
    else if(cartState == STATE_VTP)
    {
        this->VTPInfo.CtlByte   &= 0x3F;
        qDebug("SetOA@VTP:0x%x\n",this->VTPInfo.CtlByte);
    }
    this->RecoverTimer->start(this->RecoverOATimeout*1000);
}

bool FTR_CTR3SpeedCtl::SettingOAGlobalBaseJsonSlot(void)
{
    this->SettingJsonFile->setFileName(this->SettingJsonFileName);
    if(!this->SettingJsonFile->isOpen())
    {
        if(this->SettingJsonFile->open(QIODevice::ReadOnly))
        {
            QByteArray data=this->SettingJsonFile->readAll();
            this->SettingJsonFile->close();

            //使用json文件对象加载字符串
            QJsonParseError json_error;
            QJsonDocument doc=QJsonDocument::fromJson(data,&json_error);

            if(json_error.error != QJsonParseError::NoError)
            {
                qDebug()<<"Setting JSON ERROR!";
                return false;
            }

            //判断是否对象
            if(doc.isObject())
            {
                //把json文档转换为json对象
                QJsonObject obj=doc.object();

                this->GlobaOAStateFlag = (this->GlobaOAStateFlag)?(false):(true);
        #if(1)
            //RC parameter setting
                if(obj.contains("tof_oa_rc")) obj.insert("tof_oa_rc",this->GlobaOAStateFlag);
                if(obj.contains("us_oa_rc")) obj.insert("us_oa_rc",this->GlobaOAStateFlag);
            //end RC parameter setting

            //VTP parameter setting
                if(obj.contains("tof_oa_vtp")) obj.insert("tof_oa_vtp",this->GlobaOAStateFlag);
                if(obj.contains("us_oa_vtp")) obj.insert("us_oa_vtp",this->GlobaOAStateFlag);
            //end VTP parameter setting

            //VTK parameter setting
                if(obj.contains("tof_oa_vtk")) obj.insert("tof_oa_vtk",this->GlobaOAStateFlag);
                if(obj.contains("us_oa_vtk")) obj.insert("us_oa_vtk",this->GlobaOAStateFlag);
            //end VTK parameter setting

            //march parameter setting
                if(obj.contains("tof_oa_march")) obj.insert("tof_oa_march",this->GlobaOAStateFlag);
                if(obj.contains("us_oa_march")) obj.insert("us_oa_march",this->GlobaOAStateFlag);
            //end march parameter setting

                doc.setObject(obj);

                this->SettingJsonFile->open(QIODevice::WriteOnly|QIODevice::Truncate);

                this->SettingJsonFile->seek(0);
                this->SettingJsonFile->write(doc.toJson());
                this->SettingJsonFile->flush();
                this->SettingJsonFile->close();
        #endif
                qDebug()<<"SOAG:"<<this->SettingJsonFileName<<this->GlobaOAStateFlag<<"Successful!";
            }
            else
            {
                qDebug()<<"JSON no Object";
                return false;
            }
        }
        else
        {
            qDebug()<<"Open Setting Json faile";
        }
    }
    return true;
}

void FTR_CTR3SpeedCtl::UpdateVelocitySlot(MotorCtrlInfo MotorCtl)
{
    qDebug()<<MotorCtl.ProcessName<<MotorCtl.MotorVelocity.linearV<<MotorCtl.MotorVelocity.angularV;

    switch(this->CartState)
    {
    case STATE_SB:
        {
        #if(0)
            MotorVelocity_t velocity = {0x00,0x00};
            int16_t speed_l = 0;
            int16_t speed_r = 0;

            this->BLDC_StateSB_VelocityTxPreprocess(MotorCtl.MotorVelocity.linearV,MotorCtl.MotorVelocity.angularV,&speed_l,&speed_r);
            this->LeftSettingSpeed = speed_l;
            this->RightSettingSpeed= speed_r;

            uint8_t PIDStop = 0;
            TxToBLDC_RTData(10,-10,0xff,160.1,160.5,this->CartState,PIDStop);
        #endif
        }
        break;
    case STATE_MOTOR_RELEASE:
        {

        }
        break;
    case STATE_RC:
        {

        }
        break;

    case STATE_VTK:
        {

        }
        break;
    case STATE_VTP:
        {

        }
        break;

    case STATE_MARCH:
        {

        }
        break;
    case STATE_VMARCH:
        {

        }
        break;
    default:
        break;
    }
}

void FTR_CTR3SpeedCtl::CartStateSyncSlot(CartState_e CartState)
{
    qDebug()<<"Sync Cart State"<<CartState;
    this->SetCartState(CartState);
    //this->TaskFlag.TaskFlagKeepSendModeChange = true;
    if(this->CartState == STATE_SB) this->CartWantToPNGState = false;
}

void FTR_CTR3SpeedCtl::Time2LoopSlot(void)
{
    //qDebug()<<"MainLoop";
    //cart state sync

    if(this->CartWantToPNGState != this->CartInPauseState)
    {
        this->SendPNGCMD((quint8)(!this->CartWantToPNGState));        
    }
    else
    {
        //this->CartStateCtlProcess->BSP_SetLed(PNG_LED,this->CartInPauseState);
        if(this->CartState == STATE_SB)
        {
            if(this->CartInPauseState)//in pause state
            {
                //qDebug()<<this->cnt4IntoConfigModePNGToggle;

                if((!this->IntoConfigureModeFlag) && (this->cnt4IntoConfigModePNGToggle >= 3))
                {
                    this->WriteMainPipeSlot(STATE_CAMERA_RELEASE);//to release the camera
                    if(this->SocketReadyFlag)
                    {
                        this->tcpSocketSendMessageSlot("EnterConfig:");
                    }
                    this->IntoConfigureModeFlag = true;
                    this->cnt4IntoConfigModePNGToggle = 0;
                    this->CartStateCtlProcess->SetInConfigModeFlagSlot(this->IntoConfigureModeFlag);

                    qDebug()<<"CamearRelease@SB4Config:";
                }
            }
            else if(this->IntoConfigureModeFlag)
            {
                this->WriteMainPipeSlot(STATE_SB);
                this->IntoConfigureModeFlag = false;
                this->CartStateCtlProcess->SetInConfigModeFlagSlot(this->IntoConfigureModeFlag);
                if(this->SocketReadyFlag)
                {
                    this->tcpSocketSendMessageSlot("ExitConfig:");
                }
                qDebug()<<"ExitConfigCPG:";
            }
        }
        else
        {
            this->cnt4IntoConfigModePNGToggle = 0;

            if(this->CartState == STATE_MOTOR_RELEASE)
            {
                if(this->CartInPauseState && (!this->InSetOAGlobalFlag))//in pause state
                {
                    //qDebug()<<"ChangeOAS:";
                    this->SettingOAGlobalBaseJsonSlot();
                    this->InSetOAGlobalFlag = true;
                }
            }
            if(this->IntoConfigureModeFlag)
            {
                this->WriteMainPipeSlot(STATE_SB);
                this->IntoConfigureModeFlag = false;
                this->CartStateCtlProcess->SetInConfigModeFlagSlot(this->IntoConfigureModeFlag);
                if(this->SocketReadyFlag)
                {
                    this->tcpSocketSendMessageSlot("ExitConfig:");
                }
                qDebug()<<"ExitConfigCMC:";
            }
        }
    }


    {
        switch(this->CartState)
        {        
        case STATE_SB://1
            {
            #if(PLATFORM == PLATFORM_R3)
                //this->SBProcess->start();
                //this->RCProcess->stop();
                //this->VTKProcess->stop();
            #endif
                //if(this->CartState != this->PreCartState)
                if(this->TaskFlag.TaskFlagKeepSendModeChange)
                {
                    //if(!this->TaskFlag.TaskFlagKeepSendModeChange)  this->PreCartState = this->CartState;//state sync successful
                    //if(this->TaskFlag.TaskFlagKeepSendModeChange)  this->SB_Enter();

                    this->PreCartState = this->CartState;//state sync successful
                    this->SB_Enter();


                    this->ReadVTKPipeProcess->stop();
                    this->ReadVTPPipeProcess->stop();

                    this->CartWantToPNGState = false;

                    this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,LOW);

                    this->InSetOAGlobalFlag = false;

                    //this->Time2SendData->stop();
                    this->cnt4IntoConfigModePNGToggle = 0;
                    qDebug()<<"PNGT:"<<this->cnt4IntoConfigModePNGToggle;
                }
            #if(PLATFORM == PLATFORM_R3)
                else
                {
                    this->SB_RealTimeInfo();
                }
            #endif

                Wait4CameraReadyIndecateFlag    = false;
                this->VTKInfo.VTKDist           = 0;
                this->VTKInfo.VTKAngle          = 0;

                this->VTPInfo.PointOnTape[0]    = TAPE_ANGLE_LOST_VALUE;
                this->VTPInfo.PointOnTape[1]    = TAPE_ANGLE_LOST_VALUE;
                this->VTPInfo.PointOnTape[2]    = TAPE_ANGLE_LOST_VALUE;
                this->VTPInfo.PointOnTape[3]    = TAPE_ANGLE_LOST_VALUE;
                this->VTPInfo.PointOnTape[4]    = TAPE_ANGLE_LOST_VALUE;
                this->VTPInfo.PointOnTape[5]    = TAPE_ANGLE_LOST_VALUE;
                this->VTPInfo.PointOnTape[6]    = TAPE_ANGLE_LOST_VALUE;                
                this->VTPInfo.StationName       = -1;
                this->VTPInfo.SpeedCtl          = SPEED_CTL_NULL;
                 this->SpeedUpAndDownState      = false;

                if(!this->ClearVTKPipeProcess->isRunning()) this->ClearVTKPipeProcess->start();
                if(!this->ClearVTPPipeProcess->isRunning()) this->ClearVTPPipeProcess->start();

                if(this->batCapacityInfo.lessThan10Flag)
                {
                    this->lowPowerToShutDownSystemSlot();
                }
                //this->batCapacityInfo.lessThen20Flag
            }
            break;
        case STATE_MOTOR_RELEASE://9
            {
                //if(this->CartState != this->PreCartState)
                if(this->TaskFlag.TaskFlagKeepSendModeChange)
                {
                    //if(!this->TaskFlag.TaskFlagKeepSendModeChange)  this->PreCartState = this->CartState;//state sync successful
                    //if(this->TaskFlag.TaskFlagKeepSendModeChange) this->SetMotorRelease();

                    this->PreCartState = this->CartState;//state sync successful
                    this->SetMotorRelease();

                    this->CartWantToPNGState = false;

                    this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,LOW);
                }

                if(this->batCapacityInfo.lessThan10Flag)
                {
                    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                }
            }
            break;
        case STATE_RC://2 smart phone ctl
            {
            #if(PLATFORM == PLATFORM_R3)
                //this->SBProcess->stop();
                //this->RCProcess->start();
                //this->VTKProcess->stop();

                this->RC_RealTimeInfo();
            #endif
                if(this->CartState != this->PreCartState)//special for RC state
                //if(this->TaskFlag.TaskFlagKeepSendModeChange)
                {
                    //if(!this->TaskFlag.TaskFlagKeepSendModeChange)  this->PreCartState = this->CartState;//state sync successful
                    //if(this->TaskFlag.TaskFlagKeepSendModeChange) this->RC_Enter();

                    this->PreCartState = this->CartState;//state sync successful
                    this->RC_Enter();

                    this->CartWantToPNGState = false;

                    this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,LOW);
                }

                if(this->batCapacityInfo.lessThan10Flag)
                {
                    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                }
            }
            break;

        case STATE_VTK://4
            {
            #if(PLATFORM == PLATFORM_R3)
                //this->SBProcess->stop();
                //this->RCProcess->stop();
                //this->VTKProcess->start();
            #endif

                //if(this->CartState != this->PreCartState)
                if(this->TaskFlag.TaskFlagKeepSendModeChange)
                {
                    //if(!this->TaskFlag.TaskFlagKeepSendModeChange)  this->PreCartState = this->CartState;//state sync successful
                    //if(this->TaskFlag.TaskFlagKeepSendModeChange) this->VTK_Enter();

                    this->PreCartState = this->CartState;//state sync successful
                    this->VTK_Enter();

                    this->CartWantToPNGState = false;

                    if(this->LampEnInVTKFlag) this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,HIGH);

                    this->Wait4CameraReadyIndecateFlag = true;
                    this->CartStateCtlProcess->SetCameraReadyFlagSlot(false);//camera no ready

                    //this->Time2SendData->start(SEND_DATA_PER);
                }
                if(this->ClearVTKPipeProcess->isRunning()) this->ClearVTKPipeProcess->stop();
                if(!this->ReadVTKPipeProcess->isRunning() && !this->ClearVTKPipeProcess->isRunning()) this->ReadVTKPipeProcess->start();//fix sometimes change mode to VTK too fast,the thread not created

                if(this->batCapacityInfo.lessThan10Flag)
                {
                    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                }
            }
            break;
        case STATE_VTP://6
            {
                //if(this->CartState != this->PreCartState)
                if(this->TaskFlag.TaskFlagKeepSendModeChange)
                {
                    //if(!this->TaskFlag.TaskFlagKeepSendModeChange)  this->PreCartState = this->CartState;//state sync successful
                    //if(this->TaskFlag.TaskFlagKeepSendModeChange) this->VTP_Enter();

                    this->VTP_InitParameter();
                    this->PreCartState = this->CartState;//state sync successful
                    this->VTP_Enter();


                    this->CartWantToPNGState = false;

                    if(this->LampEnInVTPFlag) this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,HIGH);

                    this->Wait4CameraReadyIndecateFlag = true;
                    this->CartStateCtlProcess->SetCameraReadyFlagSlot(false);//camera no ready
                }
                if(this->ClearVTPPipeProcess->isRunning()) this->ClearVTPPipeProcess->stop();
                if(!this->ReadVTPPipeProcess->isRunning() && !this->ClearVTPPipeProcess->isRunning()) this->ReadVTPPipeProcess->start();

                if(this->batCapacityInfo.lessThan10Flag)
                {
                    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                }

                if(this->Wait4CameraReadyIndecateFlag) this->VTP_RealTimeInfo();//added for BLDC timeout error
            }
            break;
        case STATE_VMARCH:
        //case STATE_MARCH:
            {
            #if(PLATFORM == PLATFORM_R3)
                //this->SBProcess->stop();
                //this->RCProcess->start();
                //this->VTKProcess->stop();

                this->RC_RealTimeInfo();
            #endif
                //if(this->CartState != this->PreCartState)
                if(this->TaskFlag.TaskFlagKeepSendModeChange)
                {
                    //if(!this->TaskFlag.TaskFlagKeepSendModeChange)  this->PreCartState = this->CartState;//state sync successful
                    //if(this->TaskFlag.TaskFlagKeepSendModeChange) this->RC_Enter();

                    this->PreCartState = this->CartState;//state sync successful
                    this->RC_Enter();

                    this->CartWantToPNGState = false;

                    this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,LOW);

                    this->Wait4CameraReadyIndecateFlag = true;
                    this->CartStateCtlProcess->SetCameraReadyFlagSlot(false);//camera no ready
                }
            }

            if(this->batCapacityInfo.lessThan10Flag)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
            }
            break;
        default:
            {
                this->SBProcess->stop();
                this->RCProcess->stop();
                this->VTKProcess->stop();
            }
            break;
        }
    }
}

void FTR_CTR3SpeedCtl::Timer2SendDataSlot(void)
{//about 2s per cycle
    static quint32 tickCntPerSecond;
    ++tickCntPerSecond;
    //qDebug()<<tickCntPerSecond;
    if(this->AppUART->isOpen())
    {
        if((tickCntPerSecond % 2) == 0)
        {
            if(this->Wait4CameraReadyIndecateFlag) this->SendCMD(CMD_BEEP_SOUND,BEEP_TYPE_SHORT_BEEP_1);
        }

        if((tickCntPerSecond % (quint8)(this->batCapacityInfo.ratio)) == 0)
        {
            if(this->batCapacityInfo.lessThen20Flag) this->SendCMD(CMD_BEEP_SOUND,BEEP_TYPE_L1_S1_L1_S1);
        }

        if(this->CartInPauseState)
        {
            this->CartStateCtlProcess->BSP_SetLed(PNG_LED,this->CartInPauseState);
        }
        else
        {
            if( ((this->CartState == STATE_VTK) && (0xC0 & this->VTKInfo.CtlByte)) ||\
                ((this->CartState == STATE_VTP) && (0xC0 & this->VTPInfo.CtlByte)))
            {
                this->CartStateCtlProcess->BSP_LedToggle(PNG_LED);
            }
            else
            {
                this->CartStateCtlProcess->BSP_SetLed(PNG_LED,0);
            }
        }
    }
}

void FTR_CTR3SpeedCtl::PauseToGoTimerSlot(void)
{
    emit this->CartStateCtlProcess->PNGButtonToggleSignal();
    this->PauseToGoTimer->stop();
}

void FTR_CTR3SpeedCtl::RecoverOATimerSlot(void)
{
    this->RecoverTimer->stop();
    this->VTKInfo.CtlByte = this->SettingParameterFromJson.VTKCtlByte.CtlByte;;
    this->VTPInfo.CtlByte = this->SettingParameterFromJson.VTPCtlByte.CtlByte;
    //cout<<"OARecover:"<<hex<<this->VTKInfo.CtlByte<<this->VTPInfo.CtlByte<<endl;
    //printf("OARecover:0x%x,0x%x\n",this->VTKInfo.CtlByte,this->VTPInfo.CtlByte);
    qDebug("OARecover:0x%x,0x%x\n",this->VTKInfo.CtlByte,this->VTPInfo.CtlByte);
}

void FTR_CTR3SpeedCtl::ReadUARTSlot(void)
{
    if(this->AppUART->canReadLine())
    {
        QString RxInfo = this->AppUART->readLine();
        //this->AppUART->readAll();//clear buffer
        //cout<<RxInfo.toStdString()<<endl;
        //Info:1,1,0,0,0,0,0,0,26,25,38726,0,0,1,0:EndInfo\n        
        if(RxInfo.startsWith("Info:") && RxInfo.endsWith(":EndInfo\n"))
        {
            RxInfo = RxInfo.replace("Info:","");
            RxInfo = RxInfo.replace(":EndInfo\n","");

            QStringList RxInfoList = RxInfo.split(",", QString::SkipEmptyParts);
            //qDebug()<<RxInfoList;

            if(RxInfoList.size() == RX_INFO_LEN)
            {
                if(this->SocketReadyFlag && this->itNeedSendInfoToPhoneFlag)
                {
                    //this->itNeedSendInfoToPhoneFlag = false;
                    this->tcpSocketSendMessageSlot(tr("Info:")+RxInfo+","+QString::number(quint8(this->batCapacityInfo.ratio+0.5))+tr("\n"));
                    //qDebug()<<this->batCapacityInfo.ratio;
                }

                bool convertResult = false;
                int  convertValue   = RxInfoList.at(RX_INFO_ORDER_CART_MODE).toInt(&convertResult);
                if(convertResult)   this->RxInfo.CartState              = CartState_e(convertValue);

                convertValue        = RxInfoList.at(RX_INFO_ORDER_MOTOR_STATE).toInt(&convertResult);
                if(convertResult)   this->RxInfo.MotorState             = MotorState_e(convertValue);

                convertValue        = RxInfoList.at(RX_INFO_ORDER_LEFT_REAL_SPEED).toInt(&convertResult);
                if(convertResult)   this->RxInfo.LeftRealSpeed          = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_RIGHT_REAL_SPEED).toInt(&convertResult);
                if(convertResult)   this->RxInfo.RightRealSpeed         = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_LEFT_TARGET_SPEED).toInt(&convertResult);
                if(convertResult)   this->RxInfo.LeftTargetSpeed        = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_RIGHT_TARGET_SPEED).toInt(&convertResult);
                if(convertResult)   this->RxInfo.RightTargetSpeed       = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_LINEAR_SPEED).toInt(&convertResult);
                if(convertResult)   this->RxInfo.MotorVelocity.linearV  = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_ANGULAR_SPEED).toInt(&convertResult);
                if(convertResult)   this->RxInfo.MotorVelocity.angularV = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_LEFT_TEMP).toInt(&convertResult);
                if(convertResult)   this->RxInfo.LeftTemp               = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_RIGHT_TEMP).toInt(&convertResult);
                if(convertResult)   this->RxInfo.RightTemp              = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_BAT_VOL).toInt(&convertResult);
                if(convertResult)    this->RxInfo.BatVoltage             = convertValue/1000.0;
                if((this->RxInfo.BatVoltage < BAT_MAX_VOLTAGE) && (this->RxInfo.BatVoltage > BAT_MIN_VOLTAGE))
                {
                    emit this->toCalcCapacitySignal(this->RxInfo.BatVoltage);
                }

                convertValue        = RxInfoList.at(RX_INFO_ORDER_VTK_DIST).toInt(&convertResult);
                if(convertResult)    this->RxInfo.VTKDist                = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_VTK_ANGLE).toInt(&convertResult);
                if(convertResult)    this->RxInfo.VTKAngle               = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_PNG).toInt(&convertResult);
                if(convertResult)    this->RxInfo.PauseNGoState          = bool(convertValue);//false:go,true:pause

                convertValue        = RxInfoList.at(RX_INFO_ORDER_MULTI_FUN).toInt(&convertResult);
                //if(convertResult)    this->RxInfo.VTKInIdleState         = bool(convertValue);
                if(convertResult)    this->RxInfo.MultiFunction          = convertValue;

            #if(PLATFORM == PLATFORM_U250)

                if(this->VTKInIdleFlag != (bool)(this->RxInfo.MultiFunction & TKInIdleStateBit))
                {
                    this->VTKInIdleFlag = (bool)(this->RxInfo.MultiFunction & TKInIdleStateBit);
                    this->CartStateCtlProcess->SetVTKInIdleFlag(this->VTKInIdleFlag);
                }

                this->SpeedUpAndDownState       = (bool)(this->RxInfo.MultiFunction & SpeedUpAndDownBit);
                this->StartActionFlag           = (bool)(this->RxInfo.MultiFunction & StartActionBit);
                this->InArcTurningFlag          = (bool)(this->RxInfo.MultiFunction & InArcTurningBit);

                bool InPauseStateFlag           = (bool)(this->RxInfo.MultiFunction & InPauseStateBit);


                if(InPauseStateFlag != this->InPauseStateFlag)//pause case action
                {
                    this->InPauseStateFlag = InPauseStateFlag;
                    if(this->InPauseStateFlag)
                    {
                        if(this->VTPInfo.PauseTime != 0 && !this->PauseToGoTimer->isActive())
                        {
                            this->PauseToGoTimer->start(this->VTPInfo.PauseTime*1000);
                        }
                        emit this->CartStateCtlProcess->PNGButtonToggleSignal();//into pause state
                    }
                    else
                    {
                        if(this->PauseToGoTimer->isActive()) this->PauseToGoTimer->stop();
                    }
                }
            #else //R3
                if(this->RxInfo.MultiFunction & ClearYawBit) this->imuData->ClearYaw();
            #endif

                convertValue        = RxInfoList.at(RX_INFO_ORDER_TOF_OA_MIN_DIST).toInt(&convertResult);
                if(convertResult)    this->RxInfo.TOFMinDist = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_US_OA_MIN_DIST).toInt(&convertResult);
                if(convertResult)    this->RxInfo.USMinDist = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_ODO).toInt(&convertResult);
                if(convertResult)    this->RxInfo.ODO = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_CURRENT_ACTION).toInt(&convertResult);
                if(convertResult)    this->RxInfo.currentAction = ActionOnCrossType_e(convertValue);

                convertValue        = RxInfoList.at(RX_INFO_ORDER_HS_WANT_2_STATE).toInt(&convertResult);
                if(convertResult)    this->RxInfo.HSWantToCartState = CartState_e(convertValue);

                convertValue        = RxInfoList.at(RX_INFO_ORDER_ROLL).toInt(&convertResult);
                if(convertResult)    this->RxInfo.roll = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_PITCH).toInt(&convertResult);
                if(convertResult)    this->RxInfo.pitch = convertValue;

                convertValue        = RxInfoList.at(RX_INFO_ORDER_YAW).toInt(&convertResult);
                if(convertResult)    this->RxInfo.yaw = convertValue;

                if(this->HSWant2State != this->RxInfo.HSWantToCartState)
                {
                    this->HSWant2State = this->RxInfo.HSWantToCartState;
                    if(this->RxInfo.HSWantToCartState != STATE_DUMP) this->CartStateCtlProcess->SetCartStateExternal(this->HSWant2State);
                }
                else
                {
                    //if(this->RxInfo.CartState == this->CartState) this->TaskFlag.TaskFlagKeepSendModeChange = false;
                    this->TaskFlag.TaskFlagKeepSendModeChange = !(bool)(this->RxInfo.CartState == this->CartState);
                }
                this->CartInPauseState = this->RxInfo.PauseNGoState;//current state is pause

                //qDebug()<<this->CartInPauseState<<this->RxInfo.PauseNGoState;
            #if(0)
                QString CurrentTimeStr = QString((QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss"))));
                QString saveLogStr = CurrentTimeStr.append(QString(":CS:(%1);MS:%2;RS:(%3,%4);TS:(%5,%6);Vel:(%7,%8);Temp:(%9,%10);Vol:%11;VTK:(%12,%13);PnGS:%14;OA:(%15,%16);ODO:(%17);Pose(%18,%19,%20);HSW2:(%21,%22)\n")
                        .arg(QString::number(this->RxInfo.CartState) + ":" + QString::number(this->CartState)).arg(this->RxInfo.MotorState).arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                        .arg(this->RxInfo.RightTargetSpeed).arg(this->RxInfo.MotorVelocity.linearV).arg(this->RxInfo.MotorVelocity.angularV).arg(this->RxInfo.LeftTemp)
                        .arg(this->RxInfo.RightTemp).arg(this->RxInfo.BatVoltage).arg(this->RxInfo.VTKDist).arg(this->RxInfo.VTKAngle).arg(this->RxInfo.PauseNGoState)
                        .arg(this->RxInfo.TOFMinDist).arg(this->RxInfo.USMinDist).arg(this->RxInfo.ODO).arg(this->RxInfo.roll).arg(this->RxInfo.pitch).arg(this->RxInfo.yaw).arg(this->HSWant2State).arg(this->RxInfo.HSWantToCartState));
            #else
                QString saveLogStr;
                saveLogStr.clear();

                QString CurrentTimeStr = QString((QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss"))));
                if(STATE_VTK == this->CartState)
                {
                    saveLogStr = CurrentTimeStr.append(QString(":RS:(%1,%2);TS:(%3,%4);Vel:(%5,%6);RVTK:(%7,%8);TVTK:(%9,%10)\n")
                            .arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                            .arg(this->RxInfo.RightTargetSpeed).arg(this->RxInfo.MotorVelocity.linearV).arg(this->RxInfo.MotorVelocity.angularV)
                            .arg(this->RxInfo.VTKDist).arg(this->RxInfo.VTKAngle).arg(this->VTKInfo.VTKDist).arg(this->VTKInfo.VTKAngle));
                }
                else if(STATE_VTP == this->CartState)
                {
                    saveLogStr = CurrentTimeStr.append(QString(":RS:(%1,%2);TS:(%3,%4);SA(%5);IAT(%6);SUDS(%7);\n")
                            .arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                            .arg(this->RxInfo.RightTargetSpeed).arg(this->StartActionFlag).arg(this->InArcTurningFlag)
                            .arg(this->SpeedUpAndDownState));
                }
                else
                {
                    saveLogStr = CurrentTimeStr.append(QString(":CS:(%1);MS:%2;RS:(%3,%4);TS:(%5,%6);Vel:(%7,%8);Temp:(%9,%10);Vol:%11;VTK:(%12,%13);PnGS:%14;OA:(%15,%16);ODO:(%17);Pose(%18,%19,%20);HSW2:(%21,%22)\n")
                            .arg(QString::number(this->RxInfo.CartState) + ":" + QString::number(this->CartState)).arg(this->RxInfo.MotorState).arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                            .arg(this->RxInfo.RightTargetSpeed).arg(this->RxInfo.MotorVelocity.linearV).arg(this->RxInfo.MotorVelocity.angularV).arg(this->RxInfo.LeftTemp)
                            .arg(this->RxInfo.RightTemp).arg(this->RxInfo.BatVoltage).arg(this->RxInfo.VTKDist).arg(this->RxInfo.VTKAngle).arg(this->RxInfo.PauseNGoState)
                            .arg(this->RxInfo.TOFMinDist).arg(this->RxInfo.USMinDist).arg(this->RxInfo.ODO).arg(this->RxInfo.roll).arg(this->RxInfo.pitch).arg(this->RxInfo.yaw).arg(this->HSWant2State).arg(this->RxInfo.HSWantToCartState));
                }
            #endif
                if(this->ShowLogFlag.ShowRtLogFlag)  qDebug()<<saveLogStr;

                if(this->ShowLogFlag.SaveLog2FileFlag && this->SaveLogFile->isOpen())
                {
                #if(0) //会引起mem leak
                    QTextStream stream(this->SaveLogFile);
                    stream <<saveLogStr << "\r\n";
                    this->SaveLogFile->flush();
                #else
                    this->SaveLogFile->write(saveLogStr.toUtf8());
                    this->SaveLogFile->flush();
                #endif
                }
                //this->BTCtlProcess->WriteUARTSlot(saveLogStr);
                //if(this-Time2Loop->isActive())
                if(!this->EboxReadyFlag)//for Usart1 program
                {
                    this->itNeedSendInfoToPhoneFlag = true;//out infomation to socket erery time
                    this->EboxReadyFlag = true;
                    this->Time2Loop->start(LOOP_PER);
                    this->Time2SendData->start(SEND_DATA_PER);

                    this->ReadVersionOfEBox();

                    qDebug()<<"EboxReady:";
                }
            }

        }
        else if(this->GetVersionOfEBoxFlag)
        {
            this->EboxVersionStr += RxInfo;
            if(RxInfo.startsWith("EndVersion:"))
            {
                //report version to a file
                {
                    QFile file;
                    file.setFileName("/home/pi/ftrCartCtl/version");
                    if(file.exists()) file.remove();
                    if(file.open(QIODevice::WriteOnly))
                    {
                    #if(1)
                        int major,minor;
                        wiringPiVersion(&major,&minor);
                        //qDebug("wiPI Ver:%d.%d",major,minor);
                    #endif

                        QString verStr = QString("wiPi Ver:%1.%2\n").arg(major).arg(minor);
                        verStr.append(VERSION);
                        verStr.append(this->EboxVersionStr);

                        file.write(verStr.toLocal8Bit().data());
                        file.flush();
                        file.close();
                        //qDebug()<<"write ver:"<<verStr;
                        this->EboxVersionStr.clear();
                    }
                }


                this->GetVersionOfEBoxFlag = false;
            }
        }
        else if(this->ShowLogFlag.ShowMixLogFlag)// if(RxInfo.contains("SD2VTape@SB:") || RxInfo.contains("SD:"))
        {
            qDebug()<<RxInfo;
            //cout<<RxInfo.toStdString()<<endl;
        }
    }
}

void FTR_CTR3SpeedCtl::SendPNGCMD(quint8 needState)
{
    this->SendCMD(CMD_USER_PAUSE_GO,needState);
}

void    FTR_CTR3SpeedCtl::SendCMD(UserCmd cmd,quint8 data1,quint8 data2,quint8 data3)
{
    //qDebug("CMD:%x",cmd);
    QByteArray  ByteArray;
#if(SERIAL_TYPE == SERIAL_TYPE_USB)
    ByteArray[0]=SOP_USB;
    ByteArray[1]=cmd;
    ByteArray[2]=0x00;
    ByteArray[3]=0x03;
    ByteArray[4]=data1;
    ByteArray[5]=data2;
    ByteArray[6]=data3;

    ByteArray[7]=this->App_XOR(ByteArray);
#elif(SERIAL_TYPE == SERIAL_TYPE_TTL)
    ByteArray[0]=SOP_TTL;
    ByteArray[1]=cmd;
    ByteArray[2]=0x09;

    ByteArray[3]=data;
    ByteArray[4]=0x00;
    ByteArray[5]=0x00;
    ByteArray[6]=0x00;
    ByteArray[7]=0x00;
    ByteArray[8]=0x00;
    ByteArray[9]=0x00;
    ByteArray[10]=0x00;
    ByteArray[11]=0x00;

    quint16_t CRC = this->App_CRC(ByteArray);
    ByteArray[12]=(CRC>>8)& 0xFF;
    ByteArray[13]=CRC & 0xFF;
#endif
    this->AppUART->write(ByteArray);
    this->AppUART->flush();

    if(this->ShowLogFlag.ShowMixLogFlag) qDebug()<<"TX:"<<ByteArray.toHex();

    //QThread::msleep(5);
}

FTR_CTR3SpeedCtl::FTR_CTR3SpeedCtl(const FTR_CTR3SpeedCtl &rhs) : data(rhs.data)
{

}

FTR_CTR3SpeedCtl &FTR_CTR3SpeedCtl::operator=(const FTR_CTR3SpeedCtl &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

void FTR_CTR3SpeedCtl::SetCartState(CartState_e CartState)
{
    this->CartState = CartState;
}
CartState_e FTR_CTR3SpeedCtl::GetCartState(void)
{
    return this->CartState;
}



uint8_t  FTR_CTR3SpeedCtl::App_XOR(QByteArray data)
{
    uint16_t len = data.size();
    uint8_t  Result = 0;
    for(uint16_t i=0;i<len;i++)
    {
        Result ^= data[i];
    }
    return Result;
}

uint8_t  FTR_CTR3SpeedCtl::App_CheckSum(QByteArray data)
{
    uint16_t len = data.size();
    uint8_t  Result = 0;
    for(uint16_t i=0;i<len;i++)
    {
        Result += data[i];
    }
    return Result;
}
uint16_t FTR_CTR3SpeedCtl::App_CRC(QByteArray data)
{
    uint16_t    len = data.size();
    uint16_t	crc= 0;
    uint16_t	num = 0;
    for (uint16_t stat=len ; stat>0; stat--)
    {
        crc= crc^(uint16_t)(data[num++]) << 8;
        for(uint8_t i=8; i!=0; i--)
        {
            if (crc &0x8000)
            {
                crc= crc << 1 ^ 0x1021;
            }
            else
            {
                crc= crc << 1;
            }
        }
    }
    return crc;
}

void FTR_CTR3SpeedCtl::SetODOFactor(double LeftODOFactor,double RightODOFactor)
{
    this->LeftODOFactor = LeftODOFactor;
    this->RightODOFactor= RightODOFactor;
    this->AvgODOFactor  = 0.5*(LeftODOFactor + RightODOFactor);
}


void FTR_CTR3SpeedCtl::EmitBeepSound(BeepType_t BeepType)//CMD = 0xEF
{
    this->SendCMD(CMD_BEEP_SOUND,BeepType);
    //qDebug()<<"BeepSound";
}

bool FTR_CTR3SpeedCtl::GetSettingParameterFromJson(QString jsonFileName)
{
    this->SettingJsonFile->setFileName(jsonFileName);
    if(!this->SettingJsonFile->isOpen())
    {
        if(this->SettingJsonFile->open(QIODevice::ReadOnly))
        {
            QByteArray data=this->SettingJsonFile->readAll();
            this->SettingJsonFile->close();

            //使用json文件对象加载字符串
            QJsonParseError json_error;
            QJsonDocument doc=QJsonDocument::fromJson(data,&json_error);

            if(json_error.error != QJsonParseError::NoError)
            {
                qDebug()<<"Setting JSON ERROR!";
                return false;
            }

            //判断是否对象
            if(doc.isObject())
            {
                //把json文档转换为json对象
                QJsonObject obj=doc.object();

                if(obj.contains("Recover_OA_Timeout"))
                {
                    this->RecoverOATimeout = obj.value("Recover_OA_Timeout").toInt();
                    qDebug()<<"Recover_OA_Timeout:"<<this->RecoverOATimeout;
                }

                if(obj.contains("Lamp_En_In_VTK"))
                {
                    this->LampEnInVTKFlag = obj.value("Lamp_En_In_VTK").toBool();
                    qDebug()<<"VTKLampState:"<<this->LampEnInVTKFlag;
                }

                if(obj.contains("Lamp_En_In_VTP"))
                {
                    this->LampEnInVTPFlag = obj.value("Lamp_En_In_VTP").toBool();
                    qDebug()<<"VTPLampState:"<<this->LampEnInVTPFlag;
                }

                if(obj.contains("Bat_Capacity"))
                {
                    this->batCapacityInfo.theMaxCapacity = obj.value("Bat_Capacity").toDouble();
                    qDebug()<<"Bat_Capacity"<<this->batCapacityInfo.theMaxCapacity;
                }

                if(obj.contains("vtp_log"))
                {
                    this->ShowLogFlag.vtpRtLogFlag = obj.value("vtp_log").toBool();
                    qDebug()<<"vtp_log"<<this->ShowLogFlag.vtpRtLogFlag;
                }

                if(obj.contains("tx_log"))
                {
                    this->ShowLogFlag.ShowTxLogFlag = obj.value("tx_log").toBool();
                    qDebug()<<"tx_log"<<this->ShowLogFlag.ShowTxLogFlag;
                }

                if(obj.contains("rt_log"))
                {
                    this->ShowLogFlag.ShowRtLogFlag = obj.value("rt_log").toBool();
                    qDebug()<<"rt_log"<<this->ShowLogFlag.ShowRtLogFlag;
                }

                if(obj.contains("mix_log"))
                {
                    this->ShowLogFlag.ShowMixLogFlag = obj.value("mix_log").toBool();
                    qDebug()<<"mix_log"<<this->ShowLogFlag.ShowMixLogFlag;
                }

                if(obj.contains("save_log"))
                {
                    this->ShowLogFlag.SaveLog2FileFlag = obj.value("save_log").toBool();
                    qDebug()<<"save_log"<<this->ShowLogFlag.SaveLog2FileFlag;
                }
            //RC parameter setting
                if(obj.contains("min_speed"))
                {
                    this->SettingParameterFromJson.minSpeed = obj.value("min_speed").toInt();
                    qDebug()<<"min_speed"<<this->SettingParameterFromJson.minSpeed;
                }

                if(obj.contains("max_speed"))
                {
                    this->SettingParameterFromJson.maxSpeed = obj.value("max_speed").toInt();
                    qDebug()<<"max_speed"<<this->SettingParameterFromJson.maxSpeed;
                }

                if(obj.contains("tof_oa_rc"))
                {
                    this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.TOFOAOn_Off = obj.value("tof_oa_rc").toBool();
                    qDebug()<<"tof_oa_rc"<<this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.TOFOAOn_Off;
                }
                if(obj.contains("us_oa_rc"))
                {
                    this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.USOAOn_Off = obj.value("us_oa_rc").toBool();
                    qDebug()<<"us_oa_rc"<<this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.USOAOn_Off;
                }                
            //end RC parameter setting

            //VTP parameter setting
                if(obj.contains("uturn_dir_clockwise"))
                {
                    this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.UturnDir = obj.value("uturn_dir_clockwise").toBool();
                    qDebug()<<"UturnDir"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.UturnDir;
                }

                if(obj.contains("default_action"))
                {
                    this->SettingParameterFromJson.action = (ActionOnCrossType_e)(obj.value("default_action").toInt());
                    qDebug()<<"default_action"<<this->SettingParameterFromJson.action;
                }

                if(obj.contains("uturn_oa_dis"))
                {
                    this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OASetOnStation = obj.value("uturn_oa_dis").toBool();
                    qDebug()<<"uturn_oa_dis"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OASetOnStation;
                }

                if(obj.contains("tof_oa_vtp"))
                {
                    this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.TOFOAOn_Off = obj.value("tof_oa_vtp").toBool();
                    qDebug()<<"tof_oa_vtp"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.TOFOAOn_Off;
                }
                if(obj.contains("us_oa_vtp"))
                {
                    this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.USOAOn_Off = obj.value("us_oa_vtp").toBool();
                    qDebug()<<"us_oa_vtp"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.USOAOn_Off;
                }
            //end VTP parameter setting

            //VTK parameter setting
                if(obj.contains("tof_oa_vtk"))
                {
                    this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.TOFOAOn_Off = obj.value("tof_oa_vtk").toBool();
                    qDebug()<<"tof_oa_vtk"<<this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.TOFOAOn_Off;
                }
                if(obj.contains("us_oa_vtk"))
                {
                    this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.USOAOn_Off = obj.value("us_oa_vtk").toBool();
                    qDebug()<<"us_oa_vtk"<<this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.USOAOn_Off;
                }
            //end VTK parameter setting

            //march parameter setting
                if(obj.contains("tof_oa_march"))
                {
                    this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.TOFOAOn_Off = obj.value("tof_oa_march").toBool();
                    qDebug()<<"tof_oa_march"<<this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.TOFOAOn_Off;
                }
                if(obj.contains("us_oa_march"))
                {
                    this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.USOAOn_Off = obj.value("us_oa_march").toBool();
                    qDebug()<<"us_oa_march"<<this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.USOAOn_Off;
                }
            //end march parameter setting

                this->GlobaOAStateFlag = (this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.TOFOAOn_Off || this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.USOAOn_Off ||\
                                          this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.UturnDir || this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.USOAOn_Off ||\
                                          this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.TOFOAOn_Off || this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.USOAOn_Off ||\
                                          this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.TOFOAOn_Off || this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.USOAOn_Off);
            }
            else
            {
                qDebug()<<"JSON no Object";
                return false;
            }
        }
        else
        {
            qDebug()<<"Open Setting Json faile";
        }
    }
    return true;
}
bool FTR_CTR3SpeedCtl::GetPipeName(QString jsonFile)
{
   // qDebug()<<jsonFile;
#if(1)
   //打开文件
   //qDebug()<<this->JsonFile->fileName();
   this->JsonFile->setFileName(jsonFile);
   if(!this->JsonFile->isOpen())
   {
       if(this->JsonFile->open(QIODevice::ReadOnly))
       {
           //qDebug()<<this->JsonFile->isReadable();
           //qDebug()<<this->JsonFile->isWritable();

           QByteArray data=this->JsonFile->readAll();
           this->JsonFile->close();

           //使用json文件对象加载字符串
           QJsonParseError json_error;
           QJsonDocument doc=QJsonDocument::fromJson(data,&json_error);

           if(json_error.error != QJsonParseError::NoError)
           {
               qDebug()<<"JSON ERROR!";
               this->MainInputPipeName = MAIN_INPUT_DEFAULT_PIPE_NAME;
               this->VTKOutputPipeName = VTK_OUTPUT_DEFAULT_PIPE_NAME;
               this->VTPOutputPipeName = VTP_OUTPUT_DEFAULT_PIPE_NAME;
               return false;
           }

           //判断是否对象
           if(doc.isObject())
           {
               //把json文档转换为json对象
               QJsonObject obj=doc.object();

               if(obj.contains("wheel_outer_diameter_left"))
               {
                   this->SettingParameterFromJson.LeftDiam = obj.value("wheel_outer_diameter_left").toDouble();
                   qDebug()<<"LWD"<<this->SettingParameterFromJson.LeftDiam;
               }

               if(obj.contains("wheel_outer_diameter_right"))
               {
                   this->SettingParameterFromJson.RightDiam = obj.value("wheel_outer_diameter_right").toDouble();
                    qDebug()<<"RWD"<<this->SettingParameterFromJson.RightDiam;
               }

               if(obj.contains("pipe"))
               {
                   QJsonObject subObj = obj.value("pipe").toObject();
                   if(subObj.contains("main"))
                   {
                       QJsonObject finalObj = subObj.value("main").toObject();
                       this->MainInputPipeName = finalObj.value("input").toString();
                       qDebug()<<this->MainInputPipeName;
                   }
                   else
                   {
                       this->MainInputPipeName = MAIN_INPUT_DEFAULT_PIPE_NAME;
                       return false;
                   }

                   if(subObj.contains("person"))
                   {
                       QJsonObject finalObj = subObj.value("person").toObject();
                       this->VTKOutputPipeName = finalObj.value("output").toString();
                       qDebug()<<this->VTKOutputPipeName;
                   }
                   else
                   {
                       this->VTKOutputPipeName = VTK_OUTPUT_DEFAULT_PIPE_NAME;
                       return false;
                   }

                   if(subObj.contains("tape"))
                   {
                       QJsonObject finalObj = subObj.value("tape").toObject();
                       this->VTPOutputPipeName = finalObj.value("output").toString();
                       qDebug()<<this->VTPOutputPipeName;
                   }
                   else
                   {
                       this->VTPOutputPipeName = VTP_OUTPUT_DEFAULT_PIPE_NAME;
                       return false;
                   }
               }               
           }
           else
           {
               qDebug()<<"JSON no Object";
               this->MainInputPipeName = MAIN_INPUT_DEFAULT_PIPE_NAME;
               this->VTKOutputPipeName = VTK_OUTPUT_DEFAULT_PIPE_NAME;
               this->VTPOutputPipeName = VTP_OUTPUT_DEFAULT_PIPE_NAME;
               return false;
           }
       }
       else
       {
           qDebug()<<"Open JsonFile Err";
           this->MainInputPipeName = MAIN_INPUT_DEFAULT_PIPE_NAME;
           this->VTKOutputPipeName = VTK_OUTPUT_DEFAULT_PIPE_NAME;
           this->VTPOutputPipeName = VTP_OUTPUT_DEFAULT_PIPE_NAME;
           return false;
       }
   }
#endif
   return true;
}

void FTR_CTR3SpeedCtl::ReadVersionOfEBox(void)
{
    this->GetVersionOfEBoxFlag = true;
    this->EboxVersionStr.clear();
    this->SendCMD(CMD_USER_GET_SW_VERSION);

}
#if(BLUETOOTH_SERIAL_USED)
void FTR_CTR3SpeedCtl::Time4RCTimeoutSlot(void)
{
    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
    this->Time4RCTimeout->stop();
}
void FTR_CTR3SpeedCtl::BTRCCmdSlot(RCCMD_e cmd)
{
    //qDebug()<<cmd;
    switch(cmd)
    {
    case VTK_ENTER:// 0x09
        {
            if(STATE_SB == this->CartState)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_VTK);
            }
        }
        break;

    case VTP_ENTER:// 0x0A
        {
            if(STATE_SB == this->CartState)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_VTP);
            }
        }
        break;
    case MARCH_ENTER://= 0x0B,
        {
            if(STATE_SB == this->CartState)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_MARCH);
            }
        }
        break;
    case SB_ENTER://= 0x0C,
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
        }
        break;
    case MR_ENTER://= 0x0D motor release
        {
            if(STATE_SB == this->CartState)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_MOTOR_RELEASE);
            }
        }
        break;

    case RC_ENTER:
        {
            if(STATE_SB == this->CartState)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_RC);
                this->Time4RCTimeout->start(RC_COMM_TIMEOUT);//about 1s
            }
        }
        break;

    case RC_EXIT:
    case RC_STOP:
        {
            this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,this->CartState,CMD_HANDSET_CTL_STOP);//0x8B
        }
        break;

    case RC_UP:
        {
            if(STATE_RC == this->CartState)
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,this->CartState,CMD_HANDSET_CTL_FORWARD);//0x83
                this->Time4RCTimeout->start(RC_COMM_TIMEOUT);//about 1s
            }
        }
        break;
    case RC_DOWN:
        {
            if(STATE_RC == this->CartState)
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,this->CartState,CMD_HANDSET_CTL_BACKWARD);//0x84
                this->Time4RCTimeout->start(RC_COMM_TIMEOUT);//about 1s
            }
        }
        break;

    case RC_LEFT:
        {
            if(STATE_RC == this->CartState)
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,this->CartState,CMD_HANDSET_CTL_LEFT);//0x81
                this->Time4RCTimeout->start(RC_COMM_TIMEOUT);//about 1s
            }
        }
        break;
    case RC_RIGHT:
        {
            if(STATE_RC == this->CartState)
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,this->CartState,CMD_HANDSET_CTL_RIGHT);//0x82
                this->Time4RCTimeout->start(RC_COMM_TIMEOUT);//about 1s
            }
        }
        break;

    case RC_NON:
        {
            if(STATE_RC == this->CartState)
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,this->CartState,0x00);//0x82
                this->Time4RCTimeout->start(RC_COMM_TIMEOUT);//about 1s
            }
        }
        break;
    default:
        break;
    }
}
#endif

void FTR_CTR3SpeedCtl::tcpServerConnectionSlot(void)
{
    this->SocketReadyFlag = true;
    this->tcpSocket = this->tcpServer->nextPendingConnection();

    QString ip = this->tcpSocket->peerAddress().toString();
    quint16 port = this->tcpSocket->peerPort();

    //qDebug()<<ip<<":"<<port<<"connect success";
    qDebug()<<"TCP Server Connected"<<ip<<":"<<port<<"connect success";

    connect(this->tcpSocket,SIGNAL(readyRead()),this,SLOT(tcpSocketReadSlot()));
    connect(this->tcpSocket,SIGNAL(disconnected()),this,SLOT(tcpSocketDisconnectSlot()));
}

void FTR_CTR3SpeedCtl::tcpSocketDisconnectSlot()
{
    qDebug()<<"Socket Disconnect";
    this->SocketReadyFlag = false;
    this->tcpSocket->deleteLater();
}

void FTR_CTR3SpeedCtl::tcpSocketReadSlot(void)
{
    //if(this->tcpSocket->canReadLine())
    {
        QTcpSocket *socket = ((QTcpSocket*)sender());
        //qDebug()<<socket->peerAddress().toString()<<socket->peerPort();

        QString RxMessage = socket->readAll();
        //qDebug()<<RxMessage;

        if(RxMessage.startsWith("ModeChange:",Qt::CaseInsensitive))
        {
            RxMessage = RxMessage.replace("ModeChange:","");
            RxMessage = RxMessage.replace("\n","");

            bool convertResult = false;
            int CartState = RxMessage.toUInt(&convertResult);
            if(convertResult) this->CartStateCtlProcess->SetCartStateExternal((CartState_e)(CartState));
        }
        else if(RxMessage.startsWith("Pause&Go:",Qt::CaseInsensitive))
        {
            if(this->CartState != STATE_SB)
            {
                emit this->CartStateCtlProcess->PNGButtonToggleSignal();
                qDebug()<<"Trig P&G from Phone:";
            }
        }
    #if(0)
        else if(RxMessage.startsWith("RC:",Qt::CaseInsensitive))
        {
            RxMessage = RxMessage.replace("RC:","");
            RxMessage = RxMessage.replace("\n","");
            //RxMessage ="6b9e0003000000f6";
            /******
             * RC:Enter"6bf600030090000e"
             * RC:Left: "6b93000302810078"
             * RC:Right:"6b9300030282007b"
             * RC:Up:   "6b9300030283007a"
             * RC:Down: "6b9300030284007d"
             * RC:STOP: "6b930003028B0072"
             * RC:NULL: "6b930003020000f9"
             * ********/
            if(RxMessage.contains("stop",Qt::CaseInsensitive))
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,0x02,0x8b);
            }
            else if(RxMessage.contains("left",Qt::CaseInsensitive))
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,0x02,0x81);
            }
            else if(RxMessage.contains("right",Qt::CaseInsensitive))
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,0x02,0x82);
            }
            else if(RxMessage.contains("up",Qt::CaseInsensitive))
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,0x02,0x83);
            }
            else if(RxMessage.contains("down",Qt::CaseInsensitive))
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK,0x02,0x84);
            }
            else if(RxMessage.contains("null",Qt::CaseInsensitive))
            {
                this->SendCMD(CMD_BLUETOOTH_STATE_SYNC_ACK);
            }
            else if(RxMessage.contains("enter",Qt::CaseInsensitive))//enter RC
            {
                this->SendCMD(CMD_BLUETOOTH_HEAD,0x00,0x90);
            }
            //qDebug()<<RxMessage;
        }
    #endif
        else if(RxMessage.startsWith("Beep:",Qt::CaseInsensitive))
        {
            RxMessage = RxMessage.replace("Beep:","");
            RxMessage = RxMessage.replace("\n","");

            bool convertResult = false;
            int beepType = RxMessage.toUInt(&convertResult);
            if(convertResult) this->SendCMD(CMD_BEEP_SOUND,(BeepType_t)(beepType));

        }
        /*else if(RxMessage.startsWith("StateInfo:",Qt::CaseInsensitive))//need rt info
        {
            this->itNeedSendInfoToPhoneFlag = true;
        }*/
        else if(RxMessage.startsWith("VTPSetAction:",Qt::CaseInsensitive))
        {
            qDebug()<<RxMessage;
            RxMessage = RxMessage.replace("VTPSetAction:","");
            RxMessage = RxMessage.replace("\n","");

            QStringList RxMessageList = RxMessage.split(",", QString::SkipEmptyParts);
            //qDebug()<<RxInfoList;

            if(RxMessageList.size() == 5)//ctybyte,action,stationName,MaxSpeed PauseTime
            {
                bool convertResult = false;

                int  convertValue   = RxMessageList.at(2).toInt(&convertResult);
                if(convertResult)   this->VTPInfo.StationName = (int8_t)(convertValue);

                if(this->VTPInfo.StationName != -1)
                {
                #if(!USED_DEFAULT_PARAMETER_ON_STATION)

                    convertValue   = RxMessageList.at(0).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.CtlByte = (uint8_t)(convertValue);

                    convertValue   = RxMessageList.at(3).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.MaxSpeed = (uint16_t)(convertValue);
                #endif

                    convertValue   = RxMessageList.at(1).toInt(&convertResult);
                    if(convertResult)  this->VTP_UpdateAction(ActionOnCrossType_e(convertValue));

                    convertValue   = RxMessageList.at(4).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.PauseTime = (uint16_t)(convertValue);
                }
                //qDebug()<<"VTPA:"<<this->VTPInfo.CtlByte<<this->VTPInfo.setAction<<this->VTPInfo.StationName<<this->VTPInfo.MaxSpeed<<this->VTPInfo.PauseTime;
            }
        }
        else if(RxMessage.startsWith("VersionInfo:"))
        {
            QFile file;
            file.setFileName("/home/pi/ftrCartCtl/version");
            if(file.open(QIODevice::ReadOnly))
            {
                QString versionStr = file.readAll();
                this->tcpSocketSendMessageSlot(versionStr);

                file.close();
            }
        }
        //this->tcpSocketSendMessageSlot(RxMessage);
    }
}

void FTR_CTR3SpeedCtl::tcpSocketSendMessageSlot(QString message)
{
    this->tcpSocket->write(message.toLatin1());
}

void FTR_CTR3SpeedCtl::lowPowerToShutDownSystemSlot(void)
{
    qDebug()<<"LowPower ShutDown@:"<<this->batCapacityInfo.voltage;
    system("shutdown -h now");
}

void FTR_CTR3SpeedCtl::calcCapacitySlot(double voltage)
{
    const double batCoef[6] = {-0.0021095004 ,  0.4009820894 , -30.4476371198 ,  1154.4151180135 , -21853.1489934864 , 165216.3203402679};
    if(this->batCapacityInfo.voltage > voltage) this->batCapacityInfo.voltage = voltage;
    this->batCapacityInfo.Capacity = batCoef[0]*qPow(this->batCapacityInfo.voltage,5) + batCoef[1]*qPow(this->batCapacityInfo.voltage,4) + batCoef[2]*qPow(this->batCapacityInfo.voltage,3) + batCoef[3]*qPow(this->batCapacityInfo.voltage,2) + batCoef[4]*qPow(this->batCapacityInfo.voltage,1) + batCoef[5];
    this->batCapacityInfo.ratio = this->batCapacityInfo.Capacity/this->batCapacityInfo.theMaxCapacity*100;
    //qDebug()<<this->batCapacityInfo.voltage<<this->batCapacityInfo.Capacity<<this->batCapacityInfo.ratio;
    if(this->batCapacityInfo.ratio < 10)
    {
        this->batCapacityInfo.lessThan10Flag = true;
    }
    else if(this->batCapacityInfo.ratio < 20)
    {
        this->batCapacityInfo.lessThan10Flag =false;
        this->batCapacityInfo.lessThen20Flag =  true;
        //qDebug()<<"Less20:";
    }
    else
    {
        this->batCapacityInfo.lessThan10Flag = false;
        this->batCapacityInfo.lessThen20Flag = false;
    }
}
/*******
 * calc  an arc info pass 3 point
 * x0=(x1^2*y2 - x1^2*y3 - x2^2*y1 + x2^2*y3 + x3^2*y1 - x3^2*y2 + y1^2*y2 - y1^2*y3 - y1*y2^2 + y1*y3^2 + y2^2*y3 - y2*y3^2)/(2*(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2))
 * y0=(- x1^2*x2 + x1^2*x3 + x1*x2^2 - x1*x3^2 + x1*y2^2 - x1*y3^2 - x2^2*x3 + x2*x3^2 - x2*y1^2 + x2*y3^2 + x3*y1^2 - x3*y2^2)/(2*(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2))
 * R=((x1^2*y2 - x1^2*y3 - x2^2*y1 + x2^2*y3 + x3^2*y1 - x3^2*y2 + y1^2*y2 - y1^2*y3 - y1*y2^2 + y1*y3^2 + y2^2*y3 - y2*y3^2)^2/(4*(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2)^2) - (- x1^2*x2*y3 + x1^2*x3*y2 + x1*x2^2*y3 - x1*x3^2*y2 + x1*y2^2*y3 - x1*y2*y3^2 - x2^2*x3*y1 + x2*x3^2*y1 - x2*y1^2*y3 + x2*y1*y3^2 + x3*y1^2*y2 - x3*y1*y2^2)/(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2) + (- x1^2*x2 + x1^2*x3 + x1*x2^2 - x1*x3^2 + x1*y2^2 - x1*y3^2 - x2^2*x3 + x2*x3^2 - x2*y1^2 + x2*y3^2 + x3*y1^2 - x3*y2^2)^2/(4*(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2)^2))^(1/2)
 * *******/
ArcInfo_t FTR_CTR3SpeedCtl::CalcArcInfoSlot(PointAxis_t p1,PointAxis_t p2,PointAxis_t p3)
{
    ArcInfo_t arc;
    arc.ArcLocation = this->ArcInfo.ArcLocation;
    double x1 = p1.x;
    double y1 = p1.y;

    double x2 = p2.x;
    double y2 = p2.y;

    double x3 = p3.x;
    double y3 = p3.y;

    double A = (x1*x1*y2 - x1*x1*y3 - x2*x2*y1 + x2*x2*y3 + x3*x3*y1 - x3*x3*y2 + y1*y1*y2 - y1*y1*y3 - y1*y2*y2 + y1*y3*y3 + y2*y2*y3 - y2*y3*y3);
    double B = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
    double C = (- x1*x1*x2 + x1*x1*x3 + x1*x2*x2 - x1*x3*x3 + x1*y2*y2 - x1*y3*y3 - x2*x2*x3 + x2*x3*x3 - x2*y1*y1 + x2*y3*y3 + x3*y1*y1 - x3*y2*y2);
    double D =(- x1*x1*x2*y3 + x1*x1*x3*y2 + x1*x2*x2*y3 - x1*x3*x3*y2 + x1*y2*y2*y3 - x1*y2*y3*y3 - x2*x2*x3*y1 + x2*x3*x3*y1 - x2*y1*y1*y3 + x2*y1*y3*y3 + x3*y1*y1*y2 - x3*y1*y2*y2);

    arc.CenterPoint.x = A/(2*B);
    arc.CenterPoint.y = C/(2*B);
    arc.R             = sqrt(A*A/(4*B*B) - D/B + C*C/(4*B*B));

    //qDebug()<<x1<<y1<<x2<<y2<<x3<<y3;
    if((abs(arc.CenterPoint.x)<1000) && arc.R<1000)
    {
        if(this->theMinR > arc.R)
        {
            this->theMinR = arc.R;
            if(arc.CenterPoint.x < 0)
            {
               arc.ArcLocation = LEFT_LOCALTION; //1
               this->VTP_UpdateAction(ACTION_TURN_LEFT_GO);
            }
            else
            {
                arc.ArcLocation = RIGHT_LOCALTION; //2
                this->VTP_UpdateAction(ACTION_TURN_RIGHT_GO);
            }
        }
        qDebug()<<"ArcInfo:"<<arc.CenterPoint.x<<arc.R<<arc.ArcLocation;
    }

    return arc;
}

FTR_CTR3SpeedCtl::~FTR_CTR3SpeedCtl()
{    
    delete JsonFile;
    delete MainInputPipeFile;
    delete VTKOutputPipeFile;
    delete VTPOutputPipeFile;
    delete SaveLogFile;

    delete SettingJsonFile;

    delete fileWatcher;
    delete AppUART;

    delete Time2SendData;
    delete Time2Loop;
    #if(BLUETOOTH_SERIAL_USED)
        delete Time4RCTimeout;
    #endif

    delete SBProcess;
    delete RCProcess;
    delete VTKProcess;

    delete ReadVTKPipeProcess;
    delete ReadVTPPipeProcess;

    delete CartStateCtlProcess;
    qDebug()<<"End R3 Process";
}
