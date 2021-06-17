#include "ftr_ctr3speedctl.h"
#include <QThread>
//#include <QHostInfo>
#include <QNetworkInterface>
#include <QFileInfo>

class FTR_CTR3SpeedCtlData : public QSharedData
{
public:

};

FTR_CTR3SpeedCtl::FTR_CTR3SpeedCtl(QObject *parent) : QObject(parent), data(new FTR_CTR3SpeedCtlData)
{
    qDebug()<<"FtrCartCtl Thread ID:"<<QThread::currentThreadId()<<__DATE__<<__TIME__;
    qRegisterMetaType<MotorCtrlInfo>("MotorCtrlInfo");
    qRegisterMetaType<RxInfo_t>("RxInfo_t");
    qRegisterMetaType<VTKInfo_t>("VTKInfo_t");
    qRegisterMetaType<VTPInfo_t>("VTPInfo_t");
#if(IMU_USED)//PLATFORM == PLATFORM_R3)
    qRegisterMetaType<Pose_t>("Pose_t");
#endif

#if(UWB_USED)
    qRegisterMetaType<UWBInfo_t>("UWBInfo_t");
    this->uwbApp = new UWB_AOA;
    this->UWBRxInfo = {0};
    this->invalidTargetCnt = 0;
    this->lostTargetCnt = 0;
    this->itNeedComparisonFlag = false;
    this->UWBBeUsedFlag = false;
#else
    this->UWBBeUsedFlag = false;
#endif

    this->NeedInfoFromOutputPipeFlag = false;

    this->VTKInfo.ToPushFlag = false;
    this->VTPInfo.ToPushFlag = false;

    this->cnt4IntoConfigModePNGToggle = 0;
    this->cnt4ResetWIFIPNGToggle = 0;

    this->remainDistToStationTxToEBox = 0;
    this->stationInfoToSocket = "";
    this->retrySendStationInfoToSocketFlag = false;
    this->retrySendStationInfoCnt = 0;

    this->defaultFixedDist = DEFAULT_FIXED_DIST;
    this->remainDistToStation = 0;
    this->usedDefaultFixedDistFlag = true;

    this->inBuildMapModeFlag            = false;
    this->FaceDirFlag                   = true;
    this->StationName4VTP               = 0;
    this->PauseToSBInVTKTimeoutCnt      = PAUSE_TO_SB_TIME_DEFAULT;
    this->IdleToPauseInVTKTimeoutCnt     = IDLE_TO_PAUSE_TIME_DEFAULT;

    this->SettingJsonErrorFlag          = true;
    this->batCapacityInfo.voltage       = BAT_MAX_VOLTAGE;
    this->batCapacityInfo.lessThan10Flag = false;
    this->batCapacityInfo.lessThen20Flag = false;

    this->NeedOutODOCaliFlag            = false;
    this->InODOCaliFlag                 = false;
    this->NeedIntoODOCaliFlag           = false;
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

    //this->SBProcess = new SB_Thread;
    //this->RCProcess = new RC_Thread;
    //this->VTKProcess= new VTK_Thread;
#if(IMU_USED)//PLATFORM == PLATFORM_R3)
    {
        //QProcess用于启动外部程序
        QProcess process;

        process.start(GET_I2C);

        //等待命令执行结束
        process.waitForFinished();

        //获取命令执行的结果
        QByteArray result_ = process.readAllStandardOutput();

        if(result_.toUInt() == 1)
        {
             qDebug()<<"I2C State:"<<result_.toUInt()<<"Setting to Enable!";
            process.start("raspi-config nonint do_i2c 0");//enable i2c
            process.waitForFinished();
        }
        else
        {
            qDebug()<<"I2C State:"<<result_.toUInt()<<"is Enable!";
        }
    }

    this->imuData       = new imu();
#endif

#if(MANTAIN_JSON_JQ_USED)
    this->initAddJsonItemUsedJQ(this->addJsonItem);
#endif

#if(CREATE_UPDATEALLAPP_SCRIPT_USED)
    this->CreateUpdateScript();
#endif

    this->updateUi();
    this->updateAutoRunScript();
    this->checkSettingsJsonFile();

    this->fileWatcher   = new QFileSystemWatcher;

    this->Time2Loop                 = new QTimer;
    this->Time2SendData             = new QTimer;
    this->PauseToGoTimer            = new QTimer;
    this->RecoverTimer              = new QTimer;

    this->timeElapsedGotODOToMark       = new QTime;
    this->timeElapsedGotMarkToSendInfo  = new QTime;

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
//    this->GetPipeName(this->JsonFileName);
    this->SettingParameterFromJson.RCCtlByte.CtlByte = 0x00;
    this->SettingParameterFromJson.VTPCtlByte.CtlByte = 0x00;
    this->SettingParameterFromJson.VTKCtlByte.CtlByte = 0x00;
    this->SettingParameterFromJson.MarchCtlByte.CtlByte = 0x00;

    this->CartModeEnterParameter.VTPCtlByte.CtlByte = 0x00;

    this->GetSettingParameterFromJson(this->SettingJsonFileName);

    this->MainInputPipeName = MAIN_INPUT_DEFAULT_PIPE_NAME;
    this->VTKOutputPipeName = VTK_OUTPUT_DEFAULT_PIPE_NAME;
    this->VTPOutputPipeName = VTP_OUTPUT_DEFAULT_PIPE_NAME;

    this->MainInputPipeFile         = new QFile(this->MainInputPipeName);
    this->StateInfoInputPipeFile    = new QFile(FTRCARTCTL_INFO_OUT_TO_VISON_PIPE_NAME);
    //this->VTKOutputPipeFile         = new QFile(this->VTKOutputPipeName);//QFile(VTK_OUTPUT_DEFAULT_PIPE_NAME)
    //this->VTPOutputPipeFile         = new QFile(this->VTPOutputPipeName);//QFile(VTP_OUTPUT_DEFAULT_PIPE_NAME)

    /*************ftrCartCtl pipe************************************/
    this->ftrCartCtlInputPipeName   = FTRCARTCTL_IN_PIPE_NAME;
    this->ftrCartCtlOutputPipeName  = FTRCARTCTL_OUT_PIPE_NAME;
    this->ftrCartCtlInputPipeFile   = new QFile(this->ftrCartCtlInputPipeName);
    this->ftrCartCtlOutputPipeFile  = new QFile(this->ftrCartCtlOutputPipeName);
    /****************************************************************/

    this->ReadVTKPipeProcess        = new ReadVTKPipe_Thread(this->VTKOutputPipeName);
    if(this->ReadVTKPipeProcess->pipeFileName.isEmpty())
    {
        this->ReadVTKPipeProcess->SetPipeFileName(VTK_OUTPUT_DEFAULT_PIPE_NAME);
        qDebug()<<"Reset ReadVTKPipeProcess:";
    }
    this->ClearVTKPipeProcess       = new ClearPipe_Thread(this->VTKOutputPipeName);
    if(this->ClearVTKPipeProcess->pipeFileName.isEmpty())
    {
        this->ClearVTKPipeProcess->SetPipeFileName(VTK_OUTPUT_DEFAULT_PIPE_NAME);
        qDebug()<<"Reset ClearVTKPipeProcess:";
    }

    this->ReadVTPPipeProcess        = new ReadVTPPipe_Thread(this->VTPOutputPipeName);
    if(this->ReadVTPPipeProcess->pipeFileName.isEmpty())
    {
        this->ReadVTPPipeProcess->SetPipeFileName(VTP_OUTPUT_DEFAULT_PIPE_NAME);
        qDebug()<<"Reset ReadVTPPipeProcess:";
    }
    this->ClearVTPPipeProcess       = new ClearPipe_Thread(this->VTPOutputPipeName);
    if(this->ClearVTPPipeProcess->pipeFileName.isEmpty())
    {
        this->ClearVTPPipeProcess->SetPipeFileName(VTP_OUTPUT_DEFAULT_PIPE_NAME);
        qDebug()<<"Reset ClearVTPPipeProcess:";
    }

    this->ReadInputPipeProcess      = new ReadInputPipe_Thread(this->ftrCartCtlInputPipeName);
    if(this->ReadInputPipeProcess->pipeFileName.isEmpty())
    {
        this->ReadInputPipeProcess->SetPipeFileName(FTRCARTCTL_IN_PIPE_NAME);
        qDebug()<<"Reset ReadInputPipeProcess:";
    }

    this->theMinR                   = 1000;
    this->ArcInfo.ArcLocation       = NULL_LOCALTION;

#if(BLUETOOTH_SERIAL_USED)
    this->BTCtlProcess              = new BluetoothSerialPort;
#endif

#if(STREAMLIT_USED)
    this->streamlitKeepRunFlag = false;
    this->streamlitAppPID = 0;
    this->CreateStreamlitAppSlot();
    this->streamlitProcess = new QProcess;
    connect(this->streamlitProcess,SIGNAL(readyReadStandardOutput()) ,this, SLOT(on_readoutputSlot()) );

    this->StartStreamlitUISlot();
#endif

#if(AUTO_DETECT_FRP)
    this->autoDetectFRP();
#endif

#if(1)
    //wait pipe ready
    qDebug()<<"Wait Pipe file...";
    while(!(this->StateInfoInputPipeFile->exists()) && !(this->MainInputPipeFile->exists() && QFile(VTK_OUTPUT_DEFAULT_PIPE_NAME).exists() && QFile(VTP_OUTPUT_DEFAULT_PIPE_NAME).exists() && this->ftrCartCtlInputPipeFile->exists() && this->ftrCartCtlOutputPipeFile->exists()))
    {
        QThread::sleep(2);
        this->StartStreamlitUISlot();
    }
    qDebug()<<"Pipe file ready";
#endif

#if(GET_SSID_USED)
    this->updatefrpcFile();
    this->frpServerProcess = new QProcess;
    this->GetSSIDProcess = new QProcess;
    this->frpServerProcessID = 0;
    this->currentSSID = "";
    this->currentIPAddr="";
    connect(this->GetSSIDProcess,SIGNAL(readyReadStandardOutput()) ,this, SLOT(on_GetSSIDReadOutputSlot()));
    connect(this->frpServerProcess,SIGNAL(readyReadStandardOutput()) ,this, SLOT(on_frpServerReadOutputSlot()));

    this->get_ip_addr();
    this->getCartModel();
    this->getHostName();
#endif

#if(MULTIC_TCP_SUPPORT)
    this->SocketReadyFlag           = false;
    this->RemoteSocketReadyFlag     = false;
    this->tcpSocketHandle           = 0;
    this->RemoteTcpSocketHandle     = 0;

    this->tcpSocket                 = nullptr;
    this->RemoteTcpSocket           = nullptr;
    this->tcp_server_               = new TcpServer(this);
    if(!this->tcp_server_->listen(QHostAddress::Any,50006))
    {
        qDebug()<<"tcp Err:"<<this->tcp_server_->errorString();
    }
    this->tcp_server_->setMaxPendingConnections(1);//2
#else
//    this->tcpSocket               = new QTcpSocket(this);
    this->tcpServer                 = new QTcpServer(this);
    this->tcpServer->listen(QHostAddress::Any,50006);//smart phone
    this->tcpServer->setMaxPendingConnections(3);//2
#endif

#if(UDP_USED)
    this->udpSocket                 = new QUdpSocket(this);
    this->udpSocket->bind(LOCAL_PORT,QUdpSocket::ShareAddress);
#endif

    if(this->MainInputPipeFile->fileName().isEmpty())
    {
        this->MainInputPipeFile->setFileName(MAIN_INPUT_DEFAULT_PIPE_NAME);
        qDebug()<<"Reset MainInputPipeFile:"<<MAIN_INPUT_DEFAULT_PIPE_NAME;
    }

    if(this->StateInfoInputPipeFile->fileName().isEmpty())
    {
        this->StateInfoInputPipeFile->setFileName(FTRCARTCTL_INFO_OUT_TO_VISON_PIPE_NAME);
        qDebug()<<"Reset StateInfoInputPipeFile:"<<FTRCARTCTL_INFO_OUT_TO_VISON_PIPE_NAME;
    }

    if(this->ftrCartCtlOutputPipeFile->fileName().isEmpty())
    {
        this->ftrCartCtlOutputPipeFile->setFileName(FTRCARTCTL_OUT_PIPE_NAME);
        qDebug()<<"Reset ftrCartCtlOutputPipeFile:"<<FTRCARTCTL_OUT_PIPE_NAME;
    }

    this->OpenPipe(this->MainInputPipeFile,QIODevice::WriteOnly);
    this->OpenPipe(this->StateInfoInputPipeFile,QIODevice::WriteOnly);
    this->OpenPipe(this->ftrCartCtlOutputPipeFile,QIODevice::ReadWrite);
    //this->OpenPipe(this->VTKOutputPipeFile,QIODevice::ReadOnly);
    //this->OpenPipe(this->VTPOutputPipeFile,QIODevice::ReadOnly);



    //this->fileWatcher->addPath(this->MainInputPipeName);
    //this->fileWatcher->addPath(this->VTKOutputPipeName);
    //this->fileWatcher->addPath(this->VTPOutputPipeName);
    this->fileWatcher->addPath(this->SettingJsonFileName);
    this->fileWatcher->addPath(ROUTE_FILE_NAME);

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

    //delete old log files
    {
        QDir dir(LOG_PATH_NAME);
        dir.setFilter(QDir::Files);
        QStringList files = dir.entryList();
        QDateTime time;
        uint currentDateTime = QDateTime::currentDateTime().toTime_t();
        foreach(QString fileStr, files)
        {
            fileStr.replace(".log","");
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

#if(RT_LOG_MAINTAIN_USED)
    {
        QDir dir(RTLOG_FILE_PATH);
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
#endif

    //delete unuse files *.tgz,*.md5
    {
        QDir dir(WORKING_PATH_NAME);
        foreach(QFileInfo mfi, dir.entryInfoList())
        {
            if((mfi.isFile() && mfi.suffix() == "tgz") || (mfi.isFile() && mfi.suffix() == "md5"))
            {
                dir.remove(mfi.fileName());
            }
        }
    }

    this->CheckUpdateScript();
    this->getVisionVersionInfoSlot();

    {
        QFile *file = new QFile(SETTING_WHEEL_DIAM_JSON_FILE_NAME);
        if(!file->exists())
        {
            QJsonObject jsonObject;
            jsonObject.insert("left_wheel_diam",LEFT_WHEEL_DIAM);
            jsonObject.insert("right_wheel_diam",RIGHT_WHEEL_DIAM);
            jsonObject.insert("encoder_cnt",ENCODER_CNT);

            this->SettingParameterFromJson.LeftDiam = LEFT_WHEEL_DIAM;
            this->LeftWheelDiam = this->SettingParameterFromJson.LeftDiam;

            this->SettingParameterFromJson.RightDiam = RIGHT_WHEEL_DIAM;
            this->RightWheelDiam = this->SettingParameterFromJson.RightDiam;            

            QJsonDocument jsonDoc;
            jsonDoc.setObject(jsonObject);

            if(file->open(QIODevice::ReadWrite))
            {
                file->write(jsonDoc.toJson());
                file->close();
                delete file;
                qDebug()<<"Create setting_d.json ok:";
            }
            else
            {
                qDebug()<<"Create setting_d.json fail:";
            }
        }
        else
        {
            if(file->open(QIODevice::ReadOnly))
            {
                QByteArray data=file->readAll();
                file->close();
                delete file;

                //使用json文件对象加载字符串
                QJsonParseError json_error;
                QJsonDocument doc=QJsonDocument::fromJson(data,&json_error);

                if(json_error.error != QJsonParseError::NoError)
                {
                    qDebug()<<"Setting_d.json ERROR!";
                }

                //判断是否对象
                if(doc.isObject())
                {
                    //把json文档转换为json对象
                    QJsonObject obj=doc.object();
                #if(1)
                    //wheel diam
                    if(obj.contains("left_wheel_diam"))
                    {
                        this->SettingParameterFromJson.LeftDiam = obj.value("left_wheel_diam").toDouble();
                        this->LeftWheelDiam = this->SettingParameterFromJson.LeftDiam;
                        qDebug()<<"LWD"<<this->SettingParameterFromJson.LeftDiam;
                    }
                    else
                    {
                        this->SettingParameterFromJson.LeftDiam = LEFT_WHEEL_DIAM;
                        this->LeftWheelDiam = this->SettingParameterFromJson.LeftDiam;
                        qDebug()<<"LWD Used Default:"<<this->SettingParameterFromJson.LeftDiam;
                    }

                    if(obj.contains("right_wheel_diam"))
                    {
                        this->SettingParameterFromJson.RightDiam = obj.value("right_wheel_diam").toDouble();
                        this->RightWheelDiam = this->SettingParameterFromJson.RightDiam;
                         qDebug()<<"RWD"<<this->SettingParameterFromJson.RightDiam;
                    }
                    else
                    {
                        this->SettingParameterFromJson.RightDiam = RIGHT_WHEEL_DIAM;
                        this->RightWheelDiam = this->SettingParameterFromJson.RightDiam;
                         qDebug()<<"RWD Used Default:"<<this->SettingParameterFromJson.RightDiam;
                    }
                #if(0)
                    if(obj.contains("encoder_cnt"))
                    {
                        this->EncoderCnt = obj.value("encoder_cnt").toInt();
                        qDebug()<<"ECN"<<this->EncoderCnt;
                    }
                    //end wheel diam
                #endif
                #endif
                }
            }
        }

        if(!this->CartModel.compare(CART_MODEL_U200_A))
        {
            this->EncoderCnt = ENCODER_CNT_WDF;
        }
        else
        {
            this->EncoderCnt = ENCODER_CNT_QJM;
        }
        qDebug()<<"Def-Enc:"<<this->EncoderCnt;
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

    //connect(this,SIGNAL(ReadVTKInfoSignal()),this,SLOT(ReadVTKInfoFromPipeSlot()));
    //connect(this,SIGNAL(ReadVTPInfoSignal()),this,SLOT(ReadVTPInfoFromPipeSlot()));
    connect(this->Time2Loop,SIGNAL(timeout()),this,SLOT(Time2LoopSlot()));
    connect(this->Time2SendData,SIGNAL(timeout()),this,SLOT(Timer2SendDataSlot()));
    connect(this->PauseToGoTimer,SIGNAL(timeout()),this,SLOT(PauseToGoTimerSlot()));
    connect(this->RecoverTimer,SIGNAL(timeout()),this,SLOT(RecoverOATimerSlot()));

    connect(this->ReadVTKPipeProcess,SIGNAL(UpdateInfoSignal(VTKInfo_t)),this,SLOT(UpdateVTKInfoSlot(VTKInfo_t)));
    connect(this->ReadVTPPipeProcess,SIGNAL(UpdateInfoSignal(VTPInfo_t)),this,SLOT(UpdateVTPInfoSlot(VTPInfo_t)));
    connect(this->ReadInputPipeProcess,SIGNAL(UpdateInfoSignal(QString)),this,SLOT(UpdatePipeInputSlot(QString)));

#if(IMU_USED)//PLATFORM == PLATFORM_R3)
    connect(this->imuData,SIGNAL(UpdateInfoSignal(Pose_t)),this,SLOT(UpdateIMUInfoSlot(Pose_t)));
#endif

    connect(this->fileWatcher,SIGNAL(fileChanged(const QString &)),this,SLOT(fileChangedSlot(const QString &)));
    connect(this->CartStateCtlProcess,SIGNAL(BroadcastCartStateSignal(CartState_e)),this,SLOT(CartStateSyncSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(BroadcastCartStateSignal(CartState_e)),this,SLOT(WriteMainPipeSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(SettingOAToggleSignal(CartState_e)),this,SLOT(SettingOAToggleSlot(CartState_e)));
    connect(this->CartStateCtlProcess,SIGNAL(PNGButtonToggleSignal()),this,SLOT(PNGButtonToggleSlot()));
    connect(this->CartStateCtlProcess,SIGNAL(SetToPushInWorkSignal()),this,SLOT(SetToPushInWorkSlot()));
//    connect(this->CartStateCtlProcess,SIGNAL(TKeyClickedInVTKSignal()),this,SLOT(TKeyClickedInVTKSlot()));
#if(BLUETOOTH_SERIAL_USED)
    connect(this->BTCtlProcess,SIGNAL(BTRxRCCMDSignal(RCCMD_e)),this,SLOT(BTRCCmdSlot(RCCMD_e)));
    connect(this->Time4RCTimeout,SIGNAL(timeout()),this,SLOT(Time4RCTimeoutSlot()));
#endif

    //this->SBProcess->stop();
    //this->RCProcess->stop();
    //this->VTKProcess->stop();

    connect(this,SIGNAL(toCalcCapacitySignal(double)),this,SLOT(calcCapacitySlot(double)));
#if(MULTIC_TCP_SUPPORT)
    connect(this->tcp_server_,&TcpServer::ClientConnected,this,&FTR_CTR3SpeedCtl::ClientConnected);
    connect(this->tcp_server_,&TcpServer::ClientDisconnected,this,&FTR_CTR3SpeedCtl::ClientDisconnected);//监听
    connect(this, &FTR_CTR3SpeedCtl::ServerRecved, this, &FTR_CTR3SpeedCtl::ServerRecvedSlot);
#else
    connect(this->tcpServer,SIGNAL(newConnection()),this,SLOT(tcpServerConnectionSlot()));
#endif

#if(CREATE_MAP_USED)
    QFile *file = new QFile(MAP_FILE_NAME);
    if(file->exists())
    {
        if(QFile::copy(MAP_FILE_NAME,MAP_TEMP_FILE_NAME))
        {
            qDebug()<<"copy map.json to /tmp successful:";
        }
        else
        {
            qDebug()<<"copy map err:";
        }
    }
    else
    {
        qDebug()<<"map not exist:";
    }
    delete file;

    this->mapInfo   = new MapInfo();
#endif

#if(ROUNT_USED)
    this->stationsInRoute = 0;
    this->RouteRepeatFlag = false;
    this->loadRount(ROUTE_FILE_NAME);
#endif
}

#if(MANTAIN_JSON_JQ_USED)
void FTR_CTR3SpeedCtl::initAddJsonItemUsedJQ(QStringList jsonItem)
{
    for(quint8 i = 0;i<jsonItem.size();i++)
    {
        this->setValueInJsonUsedJQ(jsonItem.at(i),"false");
//        qDebug()<<jsonItem.at(i);
    }
}
QString FTR_CTR3SpeedCtl::getKeyValueInJsonUsedJQ(QString key)
{
    QString cmd = QString("jq .%1 /home/pi/ftrCartCtl/settings.json").arg(key);

    //QProcess用于启动外部程序
    QProcess process;

    process.start(cmd);

    //等待命令执行结束
    process.waitForFinished();

    //获取命令执行的结果
    QByteArray result_ = process.readAllStandardOutput().replace("\n","");

    qDebug()<<cmd<<result_;
    return result_;
}

void FTR_CTR3SpeedCtl::setValueInJsonUsedJQ(QString key,QString value)
{
    QString result = this->getKeyValueInJsonUsedJQ(key);
    if(QString::compare(result,value))
    {
        QString cmd = QString("sudo jq .%1=%2 /home/pi/ftrCartCtl/settings.json").arg(key).arg(value);

        QProcess process;

        process.start(cmd);

        //等待命令执行结束
        process.waitForFinished();
        //获取命令执行的结果
        QString result_ = process.readAllStandardOutput().replace("\n","");

        QJsonDocument jsonDocument = QJsonDocument::fromJson(result_.toLocal8Bit().data());
        if(jsonDocument.isNull())
        {
            qDebug()<< "String NULL"<< result_.toLocal8Bit().data();
        }
        else
        {
            QFile *jsonFileTemp = new QFile("/tmp/settings.json");
            if(jsonFileTemp->open(QIODevice::WriteOnly))
            {
                jsonFileTemp->seek(0);
                jsonFileTemp->write(jsonDocument.toJson());
                jsonFileTemp->flush();
                jsonFileTemp->close();

                cmd = "sudo cp -f /tmp/settings.json /home/pi/ftrCartCtl/settings.json";
                process.start(cmd);
                process.waitForFinished(1000);
                qDebug()<<process.readAllStandardOutput();
            }
            delete jsonFileTemp;
        }
        qDebug()<<cmd;
    }
    else//equ
    {
        qDebug()<<"not need to modify in json!";
    }
}
#endif

#if(AUTO_DETECT_FRP)
void FTR_CTR3SpeedCtl::autoDetectFRP(void)
{
    QDir dir(FRP_PATH);
    if(!dir.exists())
    {
        QFile *file = new QFile(FRP_FILE_NAME);
        if(file->exists())
        {
            if(file->copy(TARGET_FRP_FILE_NAME))
            {
                system("sync;sync;");
                QString cmd = "sudo tar -zxvf " + TARGET_FRP_FILE_NAME + " -C /home/pi";
                //QString cmd = "sudo tar -zxvf " + FRP_FILE_NAME + " -C /home/pi";
                qDebug()<<FRP_PATH<<" not exists"<<cmd;

                QProcess *process = new QProcess();
                process->start(cmd);
                process->waitForStarted();
                process->waitForFinished();

                process->start("sync");
                process->waitForStarted();
                process->waitForFinished();
                system("sync;sync;");
                system("sync;sync;");

                delete process;
            }
            else
            {
                qDebug()<<"Fail!!";
            }
        }
        delete file;
    }
    else
    {
        qDebug()<<FRP_PATH<<" is exists";
    }
}
#endif

#if(RT_LOG_MAINTAIN_USED)
void FTR_CTR3SpeedCtl::setRtlogFileName(QString fileName)
{
    this->rtlogFileName = fileName;
    qDebug()<<"rtlog file name:"<<this->rtlogFileName;
}
quint32 FTR_CTR3SpeedCtl::CheckRTLogSize()
{
    QString getSizeCmd = "du -sh "+ QString(this->rtlogFileName);
    //QProcess用于启动外部程序
    QProcess process;

    process.start(getSizeCmd);

    //等待命令执行结束
    process.waitForFinished();

    //获取命令执行的结果
    QByteArray result_ = process.readAllStandardOutput();
    result_.replace(this->rtlogFileName,"").replace("K\t\n","");
    quint32 rtlogSize = result_.toDouble()*1024;
    if(LOG_MAX_SIZE < rtlogSize)
    {
        QString rmLogFileCmd = "sudo rm -rf "+QString(this->rtlogFileName);
        process.start(rmLogFileCmd);
        //等待命令执行结束
        process.waitForFinished();

        qDebug()<<"Log File Too Large to del:"<<rmLogFileCmd;
    }
    qDebug()<<"rtlogSize:"<<rtlogSize;
    return rtlogSize;
}
#endif

#if(UWB_USED)
void FTR_CTR3SpeedCtl::UWBInfoSlot(UWBInfo_t info)
{    
    int8_t angleTemp = this->UWBRxInfo.angle;
    uint16_t distTemp = this->UWBRxInfo.dist;

    this->UWBRxInfo = info;
    this->UWBRxInfo.angle = (int8_t)(0.75*angleTemp + 0.25*info.angle);
    this->UWBRxInfo.dist  = (uint16_t)(0.70*distTemp + 0.30*info.dist);

//    this->UWBRxInfo.dist += 0;
    this->VTKUWBDataUpdateCnt = 0;
    this->VTKUWBDataUpdateTimeoutFlag = false;
#if(0)//dist Comparison
    if(((abs(this->VTKInfo.VTKDist - this->UWBRxInfo.dist) > 1000)) &&\
       (this->VTKInfo.VTKDist != 0) && (VTK_LOST_LEADER != this->VTKInfo.VTKDist) &&\
       this->itNeedComparisonFlag)//diff error
    {
        //if(((abs(this->UWBRxInfo.angle) > 60) || (++this->invalidTargetCnt > 3)))// && (!this->CartWantToPNGState))
        if(++this->invalidTargetCnt > 2)// && (!this->CartWantToPNGState))
        {
            this->invalidTargetCnt = 2;
            this->CartWantToPNGState = true;//want to pause state

            this->VTKDetectErrorTargetFlag = true;
            qDebug()<<"invalid target to Pause";
        }
        //qDebug()<<"invalid target:"<<this->UWBRxInfo.dist<<VTKInfo.VTKDist<<this->UWBRxInfo.angle<<VTKInfo.VTKAngle;
    }
    else
    {
        this->invalidTargetCnt = 0;

    }
#endif
    //qDebug()<<this->UWBRxInfo.dist<<this->UWBRxInfo.angle<<this->UWBRxInfo.quality<<this->VTKInfo.VTKDist;
    //qDebug()<<this->UWBRxInfo.dist<<this->UWBRxInfo.angle;
}
#endif

#if(STREAMLIT_USED)
void FTR_CTR3SpeedCtl::on_readoutputSlot()
{
//    this->ExternURLStr.append(this->streamlitProcess->readAllStandardOutput());
    QString str = this->streamlitProcess->readAllStandardOutput().data();   //将输出信息读取到编辑框
    QStringList strList = str.split("\n");
    //qDebug()<<strList.length()<<strList.at(strList.length() - 1);

}

void FTR_CTR3SpeedCtl::CreateStreamlitAppSlot(void)
{
    QFile *file = new QFile(STREAMLITAPP_FILE_NAME);
    if(!file->exists())
    {
        if(file->open(QIODevice::WriteOnly|QIODevice::Truncate))
        {
            file->write(STREAMLITAPP.toUtf8());
            file->flush();
            file->close();
            qDebug()<<STREAMLITAPP_FILE_NAME<<"Created!";

            QString cmd="sudo chmod 755 " + STREAMLITAPP_FILE_NAME;
            system(cmd.toUtf8());

        }
    }
    else
    {
        qDebug()<<"streamlitApp Exist:";
    }
    delete file;
}
void FTR_CTR3SpeedCtl::StartStreamlitUISlot(void)
{
//    if(!this->streamlitProcess->isOpen() || (0 ==this->streamlitAppPID))
    if(1)//!this->currentIPAddr.isEmpty())//not wifi be connected
    {
        if(0 ==this->streamlitAppPID)
        {
    //        this->streamlitProcess->startDetached("/home/pi/ftrCartCtl/streamlitApp run /home/pi/ftrCartCtl/ui.py");
            this->streamlitProcess->setProgram("/home/pi/ftrCartCtl/streamlitApp");
            this->streamlitProcess->setArguments({"run","/home/pi/ftrCartCtl/ui.py"});
            this->streamlitProcess->setWorkingDirectory("/home/pi/ftrCartCtl/");
            this->streamlitProcess->startDetached(&this->streamlitAppPID);

            this->streamlitProcess->waitForStarted();
            qDebug()<<"UI Start..."<<this->streamlitAppPID;
        }
        else
        {
            qDebug()<<"UI Has Started:";
        }
    }
    else
    {
        qDebug()<<"not wifi to start UI:";
    }
}
void FTR_CTR3SpeedCtl::StopStreamlitUISlot(void)
{
//    QString strCommand = "sudo kill $(ps -ef|grep streamlitApp |grep -v grep |awk '{print $2}')";

//    if(this->streamlitProcess->isOpen() || this->streamlitAppPID)
    if(this->streamlitAppPID)
    {
        QProcess::startDetached("kill",{QString::number(this->streamlitAppPID)});

//        this->streamlitProcess->close();
        this->streamlitAppPID = 0;
        qDebug()<<"Stop streamlit...";
    }
    else
    {
        qDebug()<<"UI no Run:";
    }
}
#endif

#if(GET_SSID_USED)
void FTR_CTR3SpeedCtl::get_ip_addr(void)
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if((address.protocol() == QAbstractSocket::IPv4Protocol) && address.toString().contains("192."))
        {
            this->currentIPAddr = address.toString();
            qDebug()<<"IP:"<<this->currentIPAddr;
        }
    }
}
void FTR_CTR3SpeedCtl::getCartModel(void)
{
    //QProcess用于启动外部程序
    QProcess process;

    process.start("cat /home/pi/CART_MODEL");

    //等待命令执行结束
    process.waitForFinished();

    //获取命令执行的结果
    QByteArray result_ = process.readAllStandardOutput();

    //打印结果
    this->CartModel = result_;
    this->CartModel.replace("\n","");

    qDebug()<<"CartModel:"<<this->CartModel;
}

void FTR_CTR3SpeedCtl::getHostName(void)
{
    //QProcess用于启动外部程序
    QProcess process;

    process.start("hostname");

    //等待命令执行结束
    process.waitForFinished();

    //获取命令执行的结果
    QByteArray result_ = process.readAllStandardOutput();

    //打印结果
    this->HostName = result_;
    this->HostName.replace("\n","");

    qDebug()<<"HostName:"<<this->HostName;
}
void FTR_CTR3SpeedCtl::on_GetSSIDReadOutputSlot(void)
{
    QString str = this->GetSSIDProcess->readAllStandardOutput().replace("\"","").replace("\n","").data();   //将输出信息读取到编辑框
    this->currentSSID = str.split(":").at(1);
    this->GetSSIDProcess->kill();
    //qDebug()<<this->currentSSID;
    qDebug()<<str;
}
void FTR_CTR3SpeedCtl::getSSIDScript(void)
{
//    //QString cmd = "sudo iwgetid\n";
//    QString cmd ="ifconfig wlan0 |awk '/inet 192/ {print $2};'";
//    this->GetSSIDProcess->start("./get_ip.sh");




}

void FTR_CTR3SpeedCtl::on_frpServerReadOutputSlot(void)
{
    QString str = this->frpServerProcess->readAllStandardOutput();
    qDebug()<<str;
}

void FTR_CTR3SpeedCtl::startFRPServer(void)
{
    QString cmd = "/home/pi/frp_0.34.0/frpc -c /home/pi/frp_0.34.0/frpc.ini";
    this->frpServerProcess->start(cmd);
    this->frpServerProcessID = this->frpServerProcess->processId();
    qDebug()<<cmd<<this->frpServerProcessID;
}

void FTR_CTR3SpeedCtl::killFRPServer(void)
{
    this->frpServerProcess->kill();
    this->frpServerProcessID = 0;
}
void FTR_CTR3SpeedCtl::updatefrpcFile(void)
{
    QString oldShVersion="";
    QFile *file = new QFile(FRPC_INI_DIST_DIR);
    if(file->exists())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            do
            {
                QString shStr = file->readLine();
                if(shStr.contains("# version:",Qt::CaseSensitive) || shStr.isNull() || shStr.isEmpty())
                {
                    oldShVersion = shStr.replace("# ","").replace("\n","").replace("\r","");
                    break;
                }
            }while(1);

            file->close();

            if(oldShVersion != FRPC_INI_VERSION_INFO)
            {
                qDebug()<<FRPC_INI_DIST_DIR<<oldShVersion<<" ==> "<<FRPC_INI_VERSION_INFO;
                QFile::remove(FRPC_INI_DIST_DIR);
                if(!QFile::copy(FRPC_INI_FILE_NAME,FRPC_INI_DIST_DIR)) qDebug()<<"Fail!!";

            }
            else
            {
                qDebug()<<FRPC_INI_DIST_DIR<<"is:"<<FRPC_INI_VERSION_INFO;
            }
        }
    }
    else
    {
        QFile::copy(FRPC_INI_FILE_NAME,FRPC_INI_DIST_DIR);
    }

    delete file;
}

#endif

void FTR_CTR3SpeedCtl::PNGButtonToggleSlot()
{
    //this->TaskFlag.TaskFlagNeedToChangePNGStete = true;
    this->CartWantToPNGState = !this->CartInPauseState;
    ++this->cnt4IntoConfigModePNGToggle;
    if(!this->Wait4CameraReadyIndecateFlag) ++this->cnt4ResetWIFIPNGToggle;
#if(0)
    if(this->SocketReadyFlag)
    {
        QString StationNameStr = "Pause&Go:"+QString::number(this->CartWantToPNGState);
        this->tcpSocketSendMessageSlot(StationNameStr);
    }
    //qDebug()<<"PnG:"<<this->CartWantToPNGState<< this->CartInPauseState;
#endif
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

bool FTR_CTR3SpeedCtl::WriteWheelDiamToBaseJsonSlot(void)
{
    //this->SettingJsonFile->setFileName(this->SettingJsonFileName);
    QFile *file = new QFile(SETTING_WHEEL_DIAM_JSON_FILE_NAME);
    if(!file->isOpen())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            QByteArray data=file->readAll();
            file->close();

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

            #if(1)
                if(obj.contains("left_wheel_diam")) obj.insert("left_wheel_diam",this->LeftWheelDiam);
                if(obj.contains("right_wheel_diam")) obj.insert("right_wheel_diam",this->RightWheelDiam);

                doc.setObject(obj);

                file->open(QIODevice::WriteOnly|QIODevice::Truncate);

                file->seek(0);
                file->write(doc.toJson());
                file->flush();
                file->close();
            #endif
                qDebug()<<"Save Diameter:"<<this->LeftWheelDiam<<this->RightWheelDiam<<"Successful!";
            }
            else
            {
                qDebug()<<"JSON no Object";
                delete file;
                return false;
            }
        }
        else
        {
            qDebug()<<"Open Setting Json faile";
        }
    }
    delete file;
    return true;
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
                if(obj.contains("oa_avoiding_vtk")) obj.insert("oa_avoiding_vtk",this->GlobaOAStateFlag);
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
                if(this->GlobaOAStateFlag)
                {
                    this->SendCMD(CMD_BEEP_SOUND,BEEP_TYPE_SHORT_BEEP_2);
                    if(this->SocketReadyFlag)
                    {
                        this->tcpSocketSendMessageSlot("QuickOA:1");
                    }
                }
                else
                {
                    this->SendCMD(CMD_BEEP_SOUND,BEEP_TYPE_SHORT_BEEP_4);
                    if(this->SocketReadyFlag)
                    {
                        this->tcpSocketSendMessageSlot("QuickOA:0");
                    }
                }
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

void FTR_CTR3SpeedCtl::Time2LoopSlot(void)//per 97ms
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

            if(this->CartState == STATE_VTK)
            {
                if(this->cnt4ResetWIFIPNGToggle >= 10)
                {
                    qDebug()<<"ResetWIFI TRIG:";
                    this->ResetWIFISlot();
                    this->cnt4ResetWIFIPNGToggle = 0;
                }
                if(!this->VTKInIdleFlag) this->cnt4ResetWIFIPNGToggle = 0;
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
                    this->cnt4ResetWIFIPNGToggle = 0;

                    this->ODOMark4VTPStationCalc = this->RxInfo.ODO;
                    this->VTKIdleIntoPauseFlag = false;

                    this->inBuildMapModeFlag   = false;

                    this->CartStateCtlProcess->SetVTKOAStateFlag(false);
                    this->CartStateCtlProcess->SetVTPOAStateFlag(false);

                #if(STREAMLIT_USED)
                    this->StartStreamlitUISlot();
                #endif

                #if(UWB_USED)
                    this->UWBRxInfo = {0};
                    #if(UWB_TEST_USED)
                        connect(this->uwbApp,&UWB_AOA::UpdateInfoSignal,this,&FTR_CTR3SpeedCtl::UWBInfoSlot);
                    #else
                        disconnect(this->uwbApp,&UWB_AOA::UpdateInfoSignal,this,&FTR_CTR3SpeedCtl::UWBInfoSlot);
                    #endif
                    this->itNeedComparisonFlag = true;

                    this->VTKUWBDataUpdateCnt = 0;
                    this->VTKUWBDataUpdateTimeoutFlag = true;
                    this->VTKDetectErrorTargetFlag = false;
                #else
                    this->UWBBeUsedFlag = false;
                #endif
                    this->VTKInfo.ToPushFlag = false;
                    this->VTPInfo.ToPushFlag = false;
                }
            #if(IMU_USED)//PLATFORM == PLATFORM_R3)
                else
                {
                    this->SB_RealTimeInfo();
                }
            #endif

                this->MarkCntRecord             = 0;                

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
                this->SpeedUpAndDownState       = false;
                this->startToCatchCrossFlag     = false;
                this->VTPInfo.itNeedToSpeedUpFromVisionFlag = false;

                //reset pipe name
                if(this->ClearVTKPipeProcess->pipeFileName.isEmpty()) this->ClearVTKPipeProcess->SetPipeFileName(VTK_OUTPUT_DEFAULT_PIPE_NAME);
                if(this->ClearVTPPipeProcess->pipeFileName.isEmpty()) this->ClearVTPPipeProcess->SetPipeFileName(VTP_OUTPUT_DEFAULT_PIPE_NAME);
                if(this->ReadVTKPipeProcess->pipeFileName.isEmpty())  this->ReadVTKPipeProcess->SetPipeFileName(VTK_OUTPUT_DEFAULT_PIPE_NAME);
                if(this->ReadVTPPipeProcess->pipeFileName.isEmpty())  this->ReadVTPPipeProcess->SetPipeFileName(VTP_OUTPUT_DEFAULT_PIPE_NAME);
                if(this->ReadInputPipeProcess->pipeFileName.isEmpty()) this->ReadInputPipeProcess->SetPipeFileName(FTRCARTCTL_IN_PIPE_NAME);


                if(!this->ClearVTKPipeProcess->isRunning()) this->ClearVTKPipeProcess->start();
                if(!this->ClearVTPPipeProcess->isRunning()) this->ClearVTPPipeProcess->start();
                if(!this->ReadInputPipeProcess->isRunning()) this->ReadInputPipeProcess->start();

                if((this->batCapacityInfo.lessThan10Flag) && (!this->SettingJsonErrorFlag))
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
                    printf("LowPowerToSB:\n");
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
                    printf("LowPowerToSB:\n");
                }

                //if(this->ReadInputPipeProcess->isRunning()) this->ReadInputPipeProcess->stop();
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

                    this->VTKIdleIntoPauseFlag = false;

                    this->CartStateCtlProcess->SetVTKOAStateFlag((bool)(0xC0 & this->VTKInfo.CtlByte));

                #if(STREAMLIT_USED)
                    if(!this->streamlitKeepRunFlag) this->StopStreamlitUISlot();
                #endif

                    this->VTKInfo.ToPushFlag = false;

                #if(UWB_USED)
                    this->UWBRxInfo = {0};
                    this->VTKInfo.VTKDist = this->UWBRxInfo.dist;
                    this->itNeedComparisonFlag = true;
                    this->VTKUWBDataUpdateCnt = 0;
                    this->VTKUWBDataUpdateTimeoutFlag = true;
                    this->VTKDetectErrorTargetFlag = false;

                    if(this->UWBBeUsedFlag) connect(this->uwbApp,&UWB_AOA::UpdateInfoSignal,this,&FTR_CTR3SpeedCtl::UWBInfoSlot);
                #else
                    this->UWBBeUsedFlag = false;
                #endif

                    if(this->ReadVTKPipeProcess->pipeFileName.isEmpty())  this->ReadVTKPipeProcess->SetPipeFileName(VTK_OUTPUT_DEFAULT_PIPE_NAME);
                }
                //if(this->ReadInputPipeProcess->isRunning()) this->ReadInputPipeProcess->stop();
                if(this->ClearVTKPipeProcess->isRunning()) this->ClearVTKPipeProcess->stop();

                if(!this->ReadVTKPipeProcess->isRunning() && !this->ClearVTKPipeProcess->isRunning()) this->ReadVTKPipeProcess->start();//fix sometimes change mode to VTK too fast,the thread not created

                if(this->batCapacityInfo.lessThan10Flag)
                {
                    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                    printf("LowPowerToSB:\n");
                }
            #if(UWB_USED)
                if((this->UWBBeUsedFlag) && (++this->VTKUWBDataUpdateCnt >= 20))//about 1s
                {
                    this->CartWantToPNGState = true;//want to pause state
                    this->VTKUWBDataUpdateCnt = 20;
                    this->VTKUWBDataUpdateTimeoutFlag = true;
                    qDebug()<<"UWBTO:";
                }
            #endif
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

                    this->ODOMark4VTPStationCalc = this->RxInfo.ODO;

                    this->CartStateCtlProcess->SetVTPOAStateFlag((bool)(0xC0 & this->VTPInfo.CtlByte));

                #if(STREAMLIT_USED)
                    if(!this->streamlitKeepRunFlag) this->StopStreamlitUISlot();
                #endif

                #if(ROUNT_SIMULATOR)

                    StationInfo_t currentStationInfo = this->GetActionBaseStationName(QString::number(0),true);//forword
                    this->VTPInfo.StationName   = currentStationInfo.name.toUInt();
                    this->VTPInfo.PauseTime     = currentStationInfo.pauseTime;
                    this->VTPInfo.MaxSpeed      = currentStationInfo.speed;
                    this->VTPInfo.ToStationDist = currentStationInfo.dist;
                    this->VTP_UpdateAction(currentStationInfo.action);
                    this->VTPInfo.CtlByte       = currentStationInfo.CtlByte;
                #endif

                //request the station information
                    this->remainDistToStationTxToEBox = 0;
                    this->remainDistToStation = 0;
                    this->stationInfoToSocket = "Station:"+QString::number(1) + "," +QString::number(this->remainDistToStation);
                    this->retrySendStationInfoToSocketFlag = true;

                    this->retrySendStationInfoCnt = 0;
                    this->RemainDistToCross = 0;
                    this->startToCatchCrossFlag = false;
                    this->isTheLastStationFlag  = false;

                    this->VTPInfo.ToPushFlag    = false;
                    this->VTPInfo.FirstIntoVTPFlag = true;
                    this->VTPInfo.itNeedToSpeedUpFromVisionFlag = false;

                    if(this->ReadVTPPipeProcess->pipeFileName.isEmpty())  this->ReadVTPPipeProcess->SetPipeFileName(VTP_OUTPUT_DEFAULT_PIPE_NAME);
                }
                //if(this->ReadInputPipeProcess->isRunning()) this->ReadInputPipeProcess->stop();
                if(this->ClearVTPPipeProcess->isRunning()) this->ClearVTPPipeProcess->stop();

                if(!this->ReadVTPPipeProcess->isRunning() && !this->ClearVTPPipeProcess->isRunning()) this->ReadVTPPipeProcess->start();

                if(this->batCapacityInfo.lessThan10Flag)
                {
                    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                    printf("LowPowerToSB:\n");
                }

                if(this->Wait4CameraReadyIndecateFlag) this->VTP_RealTimeInfo();//added for BLDC timeout error

            #if(ROUNT_SIMULATOR)
                if(this->isTheLastStationFlag && this->InPauseStateFlag)
                {
                    this->MarkCntRecord = 0;

                    StationInfo_t currentStationInfo = this->GetActionBaseStationName(QString::number(0),true);//forword
                    this->VTPInfo.StationName   = currentStationInfo.name.toUInt();
                    this->VTPInfo.PauseTime     = currentStationInfo.pauseTime;
                    this->VTPInfo.MaxSpeed      = currentStationInfo.speed;
                    this->VTPInfo.ToStationDist = currentStationInfo.dist;
                    this->VTP_UpdateAction(currentStationInfo.action);
                    this->VTPInfo.CtlByte       = currentStationInfo.CtlByte;

                    this->isTheLastStationFlag  = false;
                    this->EndActionFlag         = false;
                    qDebug()<<"Route End:";
                }
            #endif

                /*********calc dist to speed up&down***************************/
            #if(0)//for speed up test
                this->VTPInfo.ToStationDist = 15000;
            #endif

                qint32 walkDist = (this->RxInfo.ODO - this->ODOMark4VTPStationCalc);
                if(0 == this->VTPInfo.ToStationDist)
                {
                    this->startToCatchCrossFlag = true;
                    this->RemainDistToCross = 0;
                }
                else
                {
                    //target odo - walkDist
                    this->RemainDistToCross = this->VTPInfo.ToStationDist - walkDist;
                    this->startToCatchCrossFlag = (bool)(START_TO_SPEED_DOWN_DIST < this->RemainDistToCross);
                    if(this->RemainDistToCross <= 0)//
                    {
                        this->remainDistToStation = 0;
//                        this->remainDistToStationTxToEBox = 0;
                        this->ODOMark4VTPStationCalc = this->RxInfo.ODO;
                        this->stationInfoToSocket = "Station:"+QString::number(1) + "," +QString::number(0);

                        qDebug()<<"@Station Case Lost Cross:"<<this->VTPInfo.ToStationDist<<walkDist;
                        this->retrySendStationInfoToSocketFlag = true;
                    }
                }

                volatile uint16_t startToSpeedDownDist = START_TO_SPEED_DOWN_DIST;

                if(this->VTPInfo.MaxSpeed > 800)
                {
                    startToSpeedDownDist = START_TO_SPEED_DOWN_DIST_AT_HS;
//                    qDebug()<<this->VTPInfo.MaxSpeed<<startToSpeedDownDist;
                }

                if(((((!this->VTPInfo.FirstIntoVTPFlag) && (walkDist > START_TO_SPEED_UP_DIST_1)) || ((this->VTPInfo.FirstIntoVTPFlag) && (walkDist > START_TO_SPEED_UP_DIST))) &&\
                  (startToSpeedDownDist < this->RemainDistToCross) && (0 != this->VTPInfo.ToStationDist)) || (this->VTPInfo.itNeedToSpeedUpFromVisionFlag))
                {
                    this->VTPInfo.FirstIntoVTPFlag = false;
                    this->VTPInfo.SpeedCtl = SPEED_CTL_UP;
                    //qDebug()<<"SU:"<<walkDist;
                }
                else
                {
                    this->VTPInfo.SpeedCtl = SPEED_CTL_DOWN;
//                    qDebug()<<"SD:";
                }
                /**************************************************************/

                if(this->retrySendStationInfoToSocketFlag)
                {
                    if(++this->retrySendStationInfoCnt >= 10)
                    {
                        this->retrySendStationInfoToSocketFlag = false;
                        this->retrySendStationInfoCnt = 0;
                    }
                    this->tcpSocketSendMessageSlot(this->stationInfoToSocket);
                    qDebug()<<this->stationInfoToSocket;
                }
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

                #if(STREAMLIT_USED)
                    if(!this->streamlitKeepRunFlag) this->StopStreamlitUISlot();
                #endif
                }
            }

            if(this->batCapacityInfo.lessThan10Flag)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                printf("LowPowerToSB:\n");
            }
            break;
        default:
            {
                //this->SBProcess->stop();
                //this->RCProcess->stop();
                //this->VTKProcess->stop();
            }
            break;
        }
    }
}

void FTR_CTR3SpeedCtl::Timer2SendDataSlot(void)
{//about 1s per cycle
    static quint32 tickCntPerSecond;
    ++tickCntPerSecond;
    //qDebug()<<tickCntPerSecond;
    if(this->AppUART->isOpen())
    {        
    #if(1)
        //idle to pause
        static quint16 VTKIdleToPauseTimeoutCnt = 0;
        if((this->CartState ==  STATE_VTK) && !this->Wait4CameraReadyIndecateFlag && this->VTKInIdleFlag && !this->VTKIdleIntoPauseFlag && !this->CartInPauseState)
        {
            if(++VTKIdleToPauseTimeoutCnt >= this->IdleToPauseInVTKTimeoutCnt)
            {
                VTKIdleToPauseTimeoutCnt = 0;
                this->VTKIdleIntoPauseFlag = true;
                this->CartWantToPNGState = !this->CartInPauseState;
                qDebug("Want To PNG Case Idle");
            }
            //qDebug()<<"VTKIdleToPause:"<<VTKIdleToPauseTimeoutCnt<<this->CartWantToPNGState;
        }
        else
        {
            VTKIdleToPauseTimeoutCnt = 0;
        }
        if(this->VTKIdleIntoPauseFlag && !this->VTKInIdleFlag) this->VTKIdleIntoPauseFlag = false;
    #endif

        //pause to sb
        static quint16 VTKPauseToSBTimeoutCnt;
        if((this->CartState ==  STATE_VTK) && this->CartInPauseState)
        {
            if(++VTKPauseToSBTimeoutCnt >= this->PauseToSBInVTKTimeoutCnt)
            {
                this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                qDebug("Want Into SB More Than %d Sec:",this->PauseToSBInVTKTimeoutCnt);
            }
            //qDebug()<<"Want Into SB:"<<VTKPauseToSBTimeoutCnt;
        }
        else
        {
            VTKPauseToSBTimeoutCnt = 0;
        }

        if(this->NeedIntoODOCaliFlag)
        {
            this->SendCMD(CMD_DIAM_CALIBRATION,0x01);
        }
        else if(this->NeedOutODOCaliFlag)
        {
            this->SendCMD(CMD_DIAM_CALIBRATION,0x00);
        }

        if(this->NeedIntoTOFTestFlag)
        {
            this->SendCMD(CMD_REPORT_OA_INFO,0x01);
        }
        else if(this->NeedOutTOFTestFlag)
        {
            this->SendCMD(CMD_REPORT_OA_INFO,0x00);
            this->NeedOutTOFTestFlag = false;
        }

        if((tickCntPerSecond % 2) == 0)
        {
            if(this->Wait4CameraReadyIndecateFlag) this->SendCMD(CMD_BEEP_SOUND,BEEP_TYPE_SHORT_BEEP_1);

        #if(GET_SSID_USED)
        #if(FRP_SERVER_ALWAYS_ON)
            if(1)
        #else
            this->getSSIDScript();
            if(this->currentSSID == DEFAULT_SSID)
        #endif
            {
                if(this->frpServerProcessID == 0) this->startFRPServer();
            }
            else
            {
                if(this->frpServerProcessID != 0) this->killFRPServer();
            }
            if(this->currentIPAddr.isEmpty()) this->get_ip_addr();
        #endif
       #if(UDP_USED)
            if((!this->RemoteSocketReadyFlag) && (!this->currentIPAddr.isEmpty())) //udp broadcast cart info
            {
                this->updSendMessage(this->currentIPAddr+":50006,"+this->HostName+"\n");
                //qDebug()<<"UDPTX:";
            }
        #endif
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
        #if(PNG_INDECATE_OA)
            if( ((this->CartState == STATE_VTK) && (0xC0 & this->VTKInfo.CtlByte)) ||\
                ((this->CartState == STATE_VTP) && (0xC0 & this->VTPInfo.CtlByte)))
            {
                this->CartStateCtlProcess->BSP_LedToggle(PNG_LED);
            }
            else
            {
                this->CartStateCtlProcess->BSP_SetLed(PNG_LED,0);
            }
        #else
            if( ((this->CartState == STATE_VTK) && (!this->VTKInIdleFlag)) ||\
                ((this->CartState == STATE_VTP) && (0xC0 & this->VTPInfo.CtlByte)))
            {
                this->CartStateCtlProcess->BSP_LedToggle(PNG_LED);
            }
            else
            {
                this->CartStateCtlProcess->BSP_SetLed(PNG_LED,0);
            }
        #endif
        }

    #if(1)
        //add function to report info to vision
        {
            //order:state,p&g,s_h/l,arc_turnning,startAction,curAction,noTapeAction,ODO,toCatchCross
            QString InfoToVisionStr;
            this->toCloseCameraFlag = false;
        #if(1)
            if(this->RxInfo.PauseNGoState || this->VTKInfo.ToPushFlag)
            {
                this->toCloseCameraFlag = true;
            }
        #endif
            InfoToVisionStr = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9\n").arg(this->RxInfo.CartState).arg(this->toCloseCameraFlag)
                            .arg(this->SpeedUpAndDownState).arg(this->InArcTurningFlag).arg(this->StartActionFlag)
                            .arg(this->VTPInfo.setAction).arg(this->InLostTapeTurningFlag).arg(this->RxInfo.ODO)
                            .arg(this->startToCatchCrossFlag);
            //qDebug()<<InfoToVisionStr<<this->toCloseCameraFlag<<this->VTKInfo.ToPushFlag<<this->RxInfo.PauseNGoState;
            this->WriteInfoToVisionPipeSlot(InfoToVisionStr);
        }
    #endif
    #if(UWB_USED)
        if(this->UWBBeUsedFlag)
        {
            static quint32 recordTime2ComparisonCtl;
            if(this->VTKIdleIntoPauseFlag || this->VTKInIdleFlag) this->itNeedComparisonFlag = true;

            if((this->VTKInfo.numOfPeople == 1) &&  (this->VTKIdleIntoPauseFlag || this->VTKInIdleFlag))
            {
                recordTime2ComparisonCtl = tickCntPerSecond;
            }
            else if(this->itNeedComparisonFlag)
            {
            #if(!COMPARSION_REALTIME)
                if((tickCntPerSecond - recordTime2ComparisonCtl) >= 20)
                {
                    this->itNeedComparisonFlag = false;
                    recordTime2ComparisonCtl = tickCntPerSecond;
                    qDebug()<<"Cancle Comparsion!";
                }
            #endif
            }
            //qDebug()<<"ComparsionS："<<this->itNeedComparisonFlag;

            if(this->VTKDetectErrorTargetFlag)
            {
                if(abs(this->VTKInfo.VTKDist - this->UWBRxInfo.dist) < 700)
                {
                    this->CartWantToPNGState = false;//want to go state
                    this->VTKDetectErrorTargetFlag = false;
                }
            }
        }
    #endif
    #if(RT_LOG_MAINTAIN_USED)
        if((tickCntPerSecond % 300) == 0)
        {
            this->CheckRTLogSize();
        }
    #endif
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
                if(this->RemoteSocketReadyFlag)
                {
                    this->tcpSocketSendMessageToDeviceSlot(tr("Info:")+CartModel+","+HostName+"," +currentIPAddr+","+RxInfo+","+QString::number(quint8(this->batCapacityInfo.ratio+0.5))+tr("\n"));
                }

                if(this->NeedInfoFromOutputPipeFlag)
                {
                    this->NeedInfoFromOutputPipeFlag = false;
                    this->WriteOutPipeSlot(tr("Info:")+CartModel+","+HostName+"," +currentIPAddr+","+RxInfo+","+QString::number(quint8(this->batCapacityInfo.ratio+0.5))+tr(":EndInfo\n"));
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
                bool InStartActionFlag           = (bool)(this->RxInfo.MultiFunction & StartActionBit);
                this->InArcTurningFlag          = (bool)(this->RxInfo.MultiFunction & InArcTurningBit);
                this->InLostTapeTurningFlag     = (bool)(this->RxInfo.MultiFunction & LostTapeTurningBit);
                bool InTurningState             = (bool)(this->RxInfo.MultiFunction & InTurningStateBit);

                if(this->StartActionFlag != InStartActionFlag)
                {
                    if(this->StartActionFlag && !InStartActionFlag)
                    {
                        this->EndActionFlag = true;
                    }
                    else if(!this->StartActionFlag)
                    {
                        this->EndActionFlag = false;
                    }
                    this->StartActionFlag = InStartActionFlag;
                }

                if(this->InTurningStateFlag != InTurningState)
                {
                    if(this->InTurningStateFlag && !InTurningState)//turning finished
                    {                        
                        bool face = (this->FaceDirFlag)?(false):(true);

                        if(FaceDirFlag != face)
                        {
                            if(this->inBuildMapModeFlag && !this->mapIsSealTypeFlag)
                            {
                                if(face && !this->FaceDirFlag)
                                {
                                    this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
                                    qDebug()<<"BuileMap Finished:";
                                }

                                this->MarkCntRecord = 0;
                            }                            
                            this->FaceDirFlag = face;
                        }
                        qDebug()<<"TF,Ch Face";
                    }

                    this->InTurningStateFlag = InTurningState;
                }

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
                if((bool)(this->RxInfo.MultiFunction & InODOCaliStateBit))
                {
                    this->InODOCaliFlag         = true;
                    this->NeedIntoODOCaliFlag   = false;
                }
                else if(this->NeedOutODOCaliFlag)
                {
                    this->NeedOutODOCaliFlag    = false;
                    this->InODOCaliFlag         = false;
                }

                if(this->RxInfo.MultiFunction & AccGyroCaliBit) this->imuData->CalcBias();
            #else //R3
                if(this->VTKInIdleFlag != (bool)(this->RxInfo.MultiFunction & TKInIdleStateBit))
                {
                    this->VTKInIdleFlag = (bool)(this->RxInfo.MultiFunction & TKInIdleStateBit);
                    this->CartStateCtlProcess->SetVTKInIdleFlag(this->VTKInIdleFlag);
                }
                if((bool)(this->RxInfo.MultiFunction & InODOCaliStateBit))
                {
                    this->InODOCaliFlag         = true;
                    this->NeedIntoODOCaliFlag   = false;
                }
                else if(this->NeedOutODOCaliFlag)
                {
                    this->NeedOutODOCaliFlag    = false;
                    this->InODOCaliFlag         = false;
                }

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
            #if(REPORT_FAULT_USED)
                convertValue        = RxInfoList.at(RX_INFO_ORDER_FAULT).toInt(&convertResult);
                if(convertResult)    this->RxInfo.fault = (uint32_t)(convertValue);
            #endif
                this->timeElapsedGotODOToMark->start();
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
                #if(0)
                    saveLogStr = CurrentTimeStr.append(QString(":RS:(%1,%2);TS:(%3,%4);Vel:(%5,%6);RVTK:(%7,%8);TVTK:(%9,%10);OA(%11,%12,%13)\n")
                            .arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                            .arg(this->RxInfo.RightTargetSpeed).arg(this->RxInfo.MotorVelocity.linearV).arg(this->RxInfo.MotorVelocity.angularV)
                            .arg(this->RxInfo.VTKDist).arg(this->RxInfo.VTKAngle).arg(this->VTKInfo.VTKDist).arg(this->VTKInfo.VTKAngle)
                            .arg(this->RxInfo.TOFMinDist).arg(this->RxInfo.USMinDist).arg(this->RxInfo.MultiFunction & (ObstBeDetectedBy2DBit | ObstBeDetectedByUSBit)));
                #else
                    saveLogStr = (QString("%1,%2,%3,%4,%5,%6\n")
                            .arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                            .arg(this->RxInfo.RightTargetSpeed).arg(this->RxInfo.MotorVelocity.linearV).arg(this->RxInfo.MotorVelocity.angularV));
                #endif
                }
                else if(STATE_VTP == this->CartState)
                {
                    saveLogStr = CurrentTimeStr.append(QString(":RS:(%1,%2);TS:(%3,%4);SA(%5);IAT(%6);SUDS(%7);LTT(%8);ODO:(%9)\n")
                            .arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                            .arg(this->RxInfo.RightTargetSpeed).arg(this->StartActionFlag).arg(this->InArcTurningFlag)
                            .arg(this->SpeedUpAndDownState).arg(this->InLostTapeTurningFlag).arg(this->RxInfo.ODO));
                }
                else// if(0)
                {
                    saveLogStr = CurrentTimeStr.append(QString(":CS:(%1);MS:%2;RS:(%3,%4);TS:(%5,%6);Vel:(%7,%8);Temp:(%9,%10);Vol:%11;VTK:(%12,%13);PnGS:%14;OA:(%15,%16);ODO:(%17);Pose(%18,%19,%20);HSW2:(%21,%22)\n")
                            .arg(QString::number(this->RxInfo.CartState) + ":" + QString::number(this->CartState)).arg(this->RxInfo.MotorState).arg(this->RxInfo.LeftRealSpeed).arg(this->RxInfo.RightRealSpeed).arg(this->RxInfo.LeftTargetSpeed)
                            .arg(this->RxInfo.RightTargetSpeed).arg(this->RxInfo.MotorVelocity.linearV).arg(this->RxInfo.MotorVelocity.angularV).arg(this->RxInfo.LeftTemp)
                            .arg(this->RxInfo.RightTemp).arg(this->RxInfo.BatVoltage).arg(this->RxInfo.VTKDist).arg(this->RxInfo.VTKAngle).arg(this->RxInfo.PauseNGoState)
                            .arg(this->RxInfo.TOFMinDist).arg(this->RxInfo.USMinDist).arg(this->RxInfo.ODO).arg(this->RxInfo.roll).arg(this->RxInfo.pitch).arg(this->RxInfo.yaw).arg(this->HSWant2State).arg(this->RxInfo.HSWantToCartState));
                }
//                else
//                {
//                    saveLogStr = CurrentTimeStr.append(QString(":UWB:(%1,%2)\n").arg(this->UWBRxInfo.dist).arg(this->UWBRxInfo.angle));
//                }
            #endif

            #if(0)
                //add function to report info to vision
                {
                    //order:state,p&g,s_h/l,arc_turnning,startAction,curAction,noTapeAction,ODO
                    QString InfoToVisionStr;
                    InfoToVisionStr = QString("%1,%2,%3,%4,%5,%6,%7,%8\n").arg(this->RxInfo.CartState).arg(this->RxInfo.PauseNGoState)
                                    .arg(this->SpeedUpAndDownState).arg(this->InArcTurningFlag).arg(this->StartActionFlag)
                                    .arg(this->VTPInfo.setAction).arg(this->InLostTapeTurningFlag).arg(this->RxInfo.ODO);

                    this->WriteInfoToVisionPipeSlot(InfoToVisionStr);
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
    #if(TOF_DIST_READING_USED)
        else if(RxInfo.contains("TOF:"))
        {
            this->NeedIntoTOFTestFlag = false;
            this->WriteOutPipeSlot(RxInfo);
        }
    #endif
        else if(RxInfo.contains("ODO:") && (this->InODOCaliFlag))//ODO calibration
        {
            RxInfo = RxInfo.replace("ODO:","");
            RxInfo = RxInfo.replace("\n","");

            QStringList RxInfoList = RxInfo.split(",", QString::SkipEmptyParts);
            //qDebug()<<RxInfoList;

            if(RxInfoList.size() == 3)
            {
                bool convertResult = false;
                int converValue = RxInfoList.at(0).toInt(&convertResult);

                converValue = RxInfoList.at(0).toInt(&convertResult);
                if((converValue != 0) && (convertResult))
                {
                    this->LeftWheelDiam = this->WheelCaliDist*this->EncoderCnt/(converValue*M_PI);
                }

                converValue = RxInfoList.at(1).toInt(&convertResult);
                if((converValue != 0) && (convertResult))
                {
                    this->RightWheelDiam = this->WheelCaliDist*this->EncoderCnt/(converValue*M_PI);
                }
                QString str;
                str.append("WheelDiam:");
                str.append(QString::number(this->LeftWheelDiam,10,3));
                str.append("    ");
                str.append(QString::number(this->RightWheelDiam,10,3));
                str.append("\n");

                if(this->SocketReadyFlag)
                {
                    this->tcpSocketSendMessageSlot(str);
                    qDebug()<<str;
                }
                qDebug()<<RxInfoList<<str;
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

                        QString verStr = QString("Building: %1 %2\nwiPi Ver:%3.%4\n").arg(__DATE__).arg(__TIME__).arg(major).arg(minor);
                        verStr.append(this->visionVersionStr);
                        verStr.append(VERSION);
                        verStr.append(this->EboxVersionStr);

                        file.write(verStr.toLocal8Bit().data());
                        file.flush();
                        file.close();
                        this->AllVersionStr = verStr;
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
    //ByteArray.resize(8);

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
                this->SettingJsonErrorFlag = false;

                if(obj.contains("uwb_enable"))
                {
                    this->UWBBeUsedFlag = obj.value("uwb_enable").toBool();
                    qDebug()<<"uwb_enable:"<<this->UWBBeUsedFlag;
                }

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
                else
                {
                    this->batCapacityInfo.theMaxCapacity = BAT_CAPACITY;
                    qDebug()<<"Bat_Capacity Used Default:"<<this->batCapacityInfo.theMaxCapacity;
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
//                if(obj.contains("stations_num"))
//                {
//                    this->stationsNum = obj.value("stations_num").toInt();
//                    qDebug()<<"stations_num"<<this->stationsNum;
//                }
//                else
//                {
//                    this->stationsNum   = STATIONS_NUM;
//                }
                if(obj.contains("default_fixed_dist"))
                {
                    this->defaultFixedDist = (obj.value("default_fixed_dist").toInt());
                    qDebug()<<"default_fixed_dist"<<this->defaultFixedDist;
                }
                else
                {
                    this->defaultFixedDist = DEFAULT_FIXED_DIST;
                }

                if(obj.contains("used_fixed_dist"))
                {
                    this->usedDefaultFixedDistFlag = (obj.value("used_fixed_dist").toBool());
                    qDebug()<<"used_fixed_dist"<<this->usedDefaultFixedDistFlag;
                }
                else
                {
                    this->usedDefaultFixedDistFlag = true;
                }

                if(obj.contains("dist_bt_station"))
                {
                    this->distBtStation = (obj.value("dist_bt_station").toInt()>>1);
                    qDebug()<<"dist_bt_station"<<this->distBtStation;
                }
                else
                {
                    this->distBtStation = DIST_BT_STATION;
                }

                if(obj.contains("uturn_dir_clockwise"))
                {
                    this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.UturnDir = obj.value("uturn_dir_clockwise").toBool();
                    qDebug()<<"UturnDir"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.UturnDir;
                }

                if(obj.contains("default_action"))
                {
                    this->SettingParameterFromJson.actionEndTape = (ActionOnCrossType_e)(obj.value("default_action").toInt());
                    qDebug()<<"default_action"<<this->SettingParameterFromJson.actionEndTape;
                }

                if(obj.contains("default_action_mark"))
                {
                    this->SettingParameterFromJson.actionOnMark = (ActionOnCrossType_e)(obj.value("default_action_mark").toInt());
                    qDebug()<<"default_action_on_mark"<<this->SettingParameterFromJson.actionOnMark;
                }
            #if(USED_DEFAULT_UTURN_OA_DISABLE)
                this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OASetOnStation = true;//obj.value("uturn_oa_dis").toBool();
                qDebug()<<"uturn_oa_dis"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OASetOnStation;
            #else
                if(obj.contains("uturn_oa_dis"))
                {
                    this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OASetOnStation = obj.value("uturn_oa_dis").toBool();
                    qDebug()<<"uturn_oa_dis"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OASetOnStation;
                }
            #endif
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
                if(obj.contains("pause_to_sb_time"))
                {
                    this->PauseToSBInVTKTimeoutCnt = obj.value("pause_to_sb_time").toInt();
                    qDebug()<<"pause_to_sb_time"<<this->PauseToSBInVTKTimeoutCnt;
                }

                if(obj.contains("idle_to_pause_time"))
                {
                    this->IdleToPauseInVTKTimeoutCnt = obj.value("idle_to_pause_time").toInt();
                    qDebug()<<"idle_to_pause_time"<<this->IdleToPauseInVTKTimeoutCnt;
                }

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
                if(obj.contains("oa_avoiding_vtk"))
                {
                    this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.OAAvoidOn_Off = obj.value("oa_avoiding_vtk").toBool();
                    qDebug()<<"oa_avoiding_vtk"<<this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.OAAvoidOn_Off;
                }
            //end VTK parameter setting
                if(obj.contains("oa_beep_sound"))
                {
                    this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OABeepSound = obj.value("oa_beep_sound").toBool();
                    this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.OABeepSound = obj.value("oa_beep_sound").toBool();
                    qDebug()<<"oa_beep_sound"<<this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OABeepSound<<this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.OABeepSound;
                }
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
                //this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.UturnDir
                this->GlobaOAStateFlag = (this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.TOFOAOn_Off || this->SettingParameterFromJson.RCCtlByte.CtlByteFlag.USOAOn_Off ||\
                                          this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.TOFOAOn_Off || this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.USOAOn_Off ||\
                                          this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.TOFOAOn_Off || this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.USOAOn_Off || this->SettingParameterFromJson.VTKCtlByte.CtlByteFlag.OAAvoidOn_Off ||\
                                          this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.TOFOAOn_Off || this->SettingParameterFromJson.MarchCtlByte.CtlByteFlag.USOAOn_Off);
            }
            else
            {
                this->batCapacityInfo.theMaxCapacity = BAT_CAPACITY;
                qDebug()<<"JSON no Object"<<this->batCapacityInfo.theMaxCapacity;
                return false;
            }
        }
        else
        {
            this->batCapacityInfo.theMaxCapacity = BAT_CAPACITY;
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
            #if(0)
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
            #endif

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
#if(UDP_USED)
void FTR_CTR3SpeedCtl::updSendMessage(QString message)
{
    QByteArray data;
//    QDataStream out(&data,QIODevice::WriteOnly);//将二进制数据存入到io设备
//    out<<message;

    quint16 len = this->udpSocket->writeDatagram(message.toUtf8(),QHostAddress::Broadcast,DEST_PORT);
//    qDebug()<<message<<len;
}
#endif

#if(MULTIC_TCP_SUPPORT)
void FTR_CTR3SpeedCtl::ClientConnected(qintptr handle, QTcpSocket* socket)
{
    qDebug()<<socket->peerAddress().toString()<<socket->peerPort();
    if(socket->peerAddress().toString().contains("127.0.0.1"))//from local AppServer
    {
        this->SocketReadyFlag = true;
        this->tcpSocket = socket;
        this->tcpSocketHandle = handle;
    }
    else
    {
        this->RemoteSocketReadyFlag = true;
        this->RemoteTcpSocket = socket;
        this->RemoteTcpSocketHandle = handle;
        socket->write(this->AllVersionStr.toLatin1());
        //this->tcpSocketSendMessageToDeviceSlot(this->AllVersionStr);//report version
    }
    connect(socket, &QTcpSocket::readyRead,[=]() {
        emit ServerRecved(handle, socket, socket->readAll());
    });
}

void FTR_CTR3SpeedCtl::tcpSocketSendMessageSlot(QString message)
{
    QString CurrentTimeStr = QString((QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss"))));

    message.replace("\n","");
    QStringList messageList = message.split(":");

    if(messageList.size()>=1)
    {
        QString value = "0";
        if(messageList.size() == 2) value = messageList.at(1);
        QString toSendMessage = QString("%0$%1$%2$%3$%4$%5\r").arg(0).arg(0).arg(CurrentTimeStr).arg(0).arg(messageList.at(0)).arg(value);
        if(this->tcpSocket != nullptr) this->tcpSocket->write(toSendMessage.toLatin1());
    }
}

void FTR_CTR3SpeedCtl::tcpSocketSendMessageToDeviceSlot(QString message)
{
    QString CurrentTimeStr = QString((QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss"))));

    message.replace("\n","");
    QStringList messageList = message.split(":");

    if(messageList.size()>=1)
    {
        QString value = "0";
        if(messageList.size() == 2) value = messageList.at(1);
        QString toSendMessage = QString("%0$%1$%2$%3$%4$%5\r").arg(0).arg(0).arg(CurrentTimeStr).arg(0).arg(messageList.at(0)).arg(value);
        if(this->RemoteTcpSocket != nullptr) this->RemoteTcpSocket->write(toSendMessage.toLatin1());

    }
}

void FTR_CTR3SpeedCtl::ClientDisconnected(qintptr handle)
{
//    QTcpSocket* pQTcpSocket = static_cast<QTcpSocket*>(sender());
//    qDebug()<<"receive disconnect!"<<pQTcpSocket->peerAddress();
//    pQTcpSocket->deleteLater();

    qDebug()<<handle<<"Socket Disconnected";
    if(handle == this->tcpSocketHandle)
    {
        this->tcpSocketHandle   = 0;
        this->SocketReadyFlag   = false;
        this->tcpSocket         = nullptr;
    }
    else if(handle == this->RemoteTcpSocketHandle)
    {
        this->RemoteTcpSocketHandle = 0;
        this->RemoteSocketReadyFlag = false;
        this->RemoteTcpSocket = nullptr;
    }
}

void FTR_CTR3SpeedCtl::ServerRecvedSlot(qintptr handle,QTcpSocket *socket,const QByteArray &data)
{
    Q_UNUSED(handle);
//    qDebug()<<socket->peerAddress().toString()<<socket->peerPort()<<data;
//    QString send_data = QString("%1 %2 %3").
//            arg(socket->peerAddress().toString()).
//            arg(socket->peerPort()).
//            arg(QString(data));
    if(socket->peerAddress().toString().contains("127.0.0.1"))//from local AppServer
    {
        tcpSocketReadSlot(QString(data));
    }
    else
    {
        qDebug()<<"fromDevice:"<<(QString(data));
    }
//    socket->write(send_data.toLatin1());
}

void FTR_CTR3SpeedCtl::tcpSocketReadSlot(QString rxData)
{
    //if(this->tcpSocket->canReadLine())
    {
        //QTcpSocket *socket = ((QTcpSocket*)sender());
        //qDebug()<<socket->peerAddress().toString()<<socket->peerPort();

        QString RxMessage = rxData;//socket->readAll();
        //qDebug()<<RxMessage;

        if(RxMessage.startsWith("ModeChange:",Qt::CaseInsensitive))
        {
            RxMessage = RxMessage.replace("ModeChange:","");
            RxMessage = RxMessage.replace("\n","");

            bool convertResult = false;
            int CartState = RxMessage.toUInt(&convertResult);
            if(convertResult) this->CartStateCtlProcess->SetCartStateExternal((CartState_e)(CartState));
            qDebug()<<"ModeChange from Phone:"<<CartState;
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
            qDebug()<<"Beep from phone:";
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

            if((RxMessageList.size() == 5) || (RxMessageList.size() == 6))//ctybyte,action,stationName,MaxSpeed, PauseTime,dist
            {
                bool convertResult = false;

                int  convertValue   = RxMessageList.at(2).toInt(&convertResult);
                if(convertResult)   this->VTPInfo.StationName = (int8_t)(convertValue);
            #if(NUM_STATION_USED)
                if(this->VTPInfo.StationName != -1)
            #endif
                {
                #if(!USED_DEFAULT_PARAMETER_ON_STATION)
                    convertValue   = RxMessageList.at(0).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.CtlByte = (uint8_t)(convertValue);
                #endif
                    convertValue   = RxMessageList.at(3).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.MaxSpeed = (uint16_t)(convertValue);

                    convertValue   = RxMessageList.at(1).toInt(&convertResult);
                    if(convertResult)  this->VTP_UpdateAction(ActionOnCrossType_e(convertValue));
                #if(0)//!NUM_STATION_USED)
                    //6,5,14
                    if((ActionOnCrossType_e(convertValue) == ACTION_UTURN_GO) ||
                        (ActionOnCrossType_e(convertValue) == ACTION_UTURN_STOP) ||
                        (ActionOnCrossType_e(convertValue) == ACTION_UTURN_PAUSE))
                    {
                        this->FaceDirFlag = (this->FaceDirFlag)?(false):(true);
                    }
                    qDebug()<<"SetDirBaseAction:"<<convertValue<<this->FaceDirFlag;
                #endif
                    convertValue   = RxMessageList.at(4).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.PauseTime = (uint16_t)(convertValue);

                    if(RxMessageList.size() == 6)
                    {
                        convertValue   = RxMessageList.at(5).toInt(&convertResult);
                        if(convertResult)   this->VTPInfo.ToStationDist = (uint16_t)(convertValue);
                    }
                }
                if(this->retrySendStationInfoToSocketFlag)
                {
                    this->retrySendStationInfoToSocketFlag = false;
                    this->stationInfoToSocket = "";
                }
                this->stationInfoUpdateFlag = true;
                qDebug()<<"VTPA:"<<this->VTPInfo.CtlByte<<this->VTPInfo.setAction<<this->VTPInfo.StationName<<this->VTPInfo.MaxSpeed<<this->VTPInfo.PauseTime<<this->VTPInfo.ToStationDist;
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
        else if(RxMessage.contains("WheelCali:"))
        {
            //WheelCali:1,10000
            //WheelCali:0,0
            RxMessage = RxMessage.replace("WheelCali:","");
            RxMessage = RxMessage.replace("\n","");
            QStringList RxMessageList = RxMessage.split(",", QString::SkipEmptyParts);

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
            qDebug()<<"WC:"<<RxMessage;
        }
        else if(RxMessage.contains("CameraCali:"))
        {
            RxMessage = RxMessage.replace("CameraCali:","");
            RxMessage = RxMessage.replace("\n","");
            QStringList RxMessageList = RxMessage.split(",", QString::SkipEmptyParts);

            if(RxMessageList.size() == 2)
            {
                bool convertResult = false;
                int converValue = RxMessageList.at(0).toInt(&convertResult);
                if(convertResult)
                {
                    if(converValue)//start cali
                    {
                        converValue = RxMessageList.at(1).toInt(&convertResult);
                        if(convertResult)
                        {
                            if(converValue)
                            {
                                this->WriteMainPipeSlot(STATE_CAMERACALI_AUTO);//camera cali auto
                                this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,HIGH);
                                qDebug()<<"CCAuto:"<<STATE_CAMERACALI_AUTO;
                            }
                            else
                            {
                                this->WriteMainPipeSlot(STATE_CAMERACALI_MANUAL);//to release the camera
                                this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,HIGH);
                                qDebug()<<"CCManul:"<<STATE_CAMERACALI_MANUAL;
                            }
                        }

                    }
                    else
                    {
                        this->WriteMainPipeSlot(STATE_SB);
                        this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,LOW);
                    }
                }
            }
            qDebug()<<"CC:"<<RxMessage;
        }
        else if(RxMessage.contains("StartNewPath:"))
        {
        //request the station information
            this->remainDistToStation = 0;
            //this->remainDistToStationTxToEBox = 0;
            this->ODOMark4VTPStationCalc = this->RxInfo.ODO;
            this->stationInfoToSocket = "Station:"+QString::number(1) + "," +QString::number(this->remainDistToStation);

            this->retrySendStationInfoToSocketFlag = true;
            qDebug()<<"StartNewPath:";
        }
    #if(CREATE_MAP_USED)
        else if(RxMessage.contains("BuildMap:"))
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
            obj.insert("0->0","0,0");

            QJsonDocument doc;
            doc.setObject(obj);

            file->open(QIODevice::WriteOnly|QIODevice::Truncate);
            file->seek(0);
            file->write(doc.toJson());
            file->flush();
            file->close();

            delete file;
        }
        else if(RxMessage.contains("MapSave:"))
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
        else if(RxMessage.contains("MapCancle:"))
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
            this->inBuildMapModeFlag        = false;
        }
    #endif
    }
}

#else
void FTR_CTR3SpeedCtl::tcpServerConnectionSlot(void)
{
    this->SocketReadyFlag = true;
    this->tcpSocket = this->tcpServer->nextPendingConnection();

    QString ip = this->tcpSocket->peerAddress().toString();
    quint16 port = this->tcpSocket->peerPort();

    //qDebug()<<ip<<":"<<port<<"connect success";
    qDebug()<<"TCP Server Connected"<<ip<<":"<<port<<"connect success"<<this->tcpServer->maxPendingConnections();

    connect(this->tcpSocket,SIGNAL(readyRead()),this,SLOT(tcpSocketReadSlot()));
    connect(this->tcpSocket,SIGNAL(disconnected()),this,SLOT(tcpSocketDisconnectSlot()));
}

void FTR_CTR3SpeedCtl::tcpSocketDisconnectSlot()
{
    qDebug()<<"Socket Disconnect";
    this->SocketReadyFlag = false;
//    this->tcpSocket->deleteLater();
}

void FTR_CTR3SpeedCtl::tcpSocketReadSlot(qint8 i)
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
            qDebug()<<"ModeChange from Phone:"<<CartState;
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
            qDebug()<<"Beep from phone:";
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

            if((RxMessageList.size() == 5) || (RxMessageList.size() == 6))//ctybyte,action,stationName,MaxSpeed, PauseTime,dist
            {
                bool convertResult = false;

                int  convertValue   = RxMessageList.at(2).toInt(&convertResult);
                if(convertResult)   this->VTPInfo.StationName = (int8_t)(convertValue);
            #if(NUM_STATION_USED)
                if(this->VTPInfo.StationName != -1)
            #endif
                {
                #if(!USED_DEFAULT_PARAMETER_ON_STATION)
                    convertValue   = RxMessageList.at(0).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.CtlByte = (uint8_t)(convertValue);
                #endif
                    convertValue   = RxMessageList.at(3).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.MaxSpeed = (uint16_t)(convertValue);                

                    convertValue   = RxMessageList.at(1).toInt(&convertResult);
                    if(convertResult)  this->VTP_UpdateAction(ActionOnCrossType_e(convertValue));
                #if(0)//!NUM_STATION_USED)
                    //6,5,14
                    if((ActionOnCrossType_e(convertValue) == ACTION_UTURN_GO) ||
                        (ActionOnCrossType_e(convertValue) == ACTION_UTURN_STOP) ||
                        (ActionOnCrossType_e(convertValue) == ACTION_UTURN_PAUSE))
                    {
                        this->FaceDirFlag = (this->FaceDirFlag)?(false):(true);
                    }
                    qDebug()<<"SetDirBaseAction:"<<convertValue<<this->FaceDirFlag;
                #endif
                    convertValue   = RxMessageList.at(4).toInt(&convertResult);
                    if(convertResult)   this->VTPInfo.PauseTime = (uint16_t)(convertValue);

                    if(RxMessageList.size() == 6)
                    {
                        convertValue   = RxMessageList.at(5).toInt(&convertResult);
                        if(convertResult)   this->VTPInfo.ToStationDist = (uint16_t)(convertValue);
                    }
                }
                if(this->retrySendStationInfoToSocketFlag)
                {
                    this->retrySendStationInfoToSocketFlag = false;
                    this->stationInfoToSocket = "";
                }
                this->stationInfoUpdateFlag = true;
                qDebug()<<"VTPA:"<<this->VTPInfo.CtlByte<<this->VTPInfo.setAction<<this->VTPInfo.StationName<<this->VTPInfo.MaxSpeed<<this->VTPInfo.PauseTime<<this->VTPInfo.ToStationDist;
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
        else if(RxMessage.contains("WheelCali:"))
        {
            //WheelCali:1,10000
            //WheelCali:0,0
            RxMessage = RxMessage.replace("WheelCali:","");
            RxMessage = RxMessage.replace("\n","");
            QStringList RxMessageList = RxMessage.split(",", QString::SkipEmptyParts);

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
            qDebug()<<"WC:"<<RxMessage;
        }
        else if(RxMessage.contains("CameraCali:"))
        {
            RxMessage = RxMessage.replace("CameraCali:","");
            RxMessage = RxMessage.replace("\n","");
            QStringList RxMessageList = RxMessage.split(",", QString::SkipEmptyParts);

            if(RxMessageList.size() == 2)
            {
                bool convertResult = false;
                int converValue = RxMessageList.at(0).toInt(&convertResult);
                if(convertResult)
                {
                    if(converValue)//start cali
                    {
                        converValue = RxMessageList.at(1).toInt(&convertResult);
                        if(convertResult)
                        {
                            if(converValue)
                            {
                                this->WriteMainPipeSlot(STATE_CAMERACALI_AUTO);//camera cali auto
                                this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,HIGH);
                                qDebug()<<"CCAuto:"<<STATE_CAMERACALI_AUTO;
                            }
                            else
                            {
                                this->WriteMainPipeSlot(STATE_CAMERACALI_MANUAL);//to release the camera
                                this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,HIGH);
                                qDebug()<<"CCManul:"<<STATE_CAMERACALI_MANUAL;
                            }
                        }

                    }
                    else
                    {
                        this->WriteMainPipeSlot(STATE_SB);
                        this->CartStateCtlProcess->BSP_SetLed(LAMP_LED,LOW);
                    }
                }
            }
            qDebug()<<"CC:"<<RxMessage;
        }
        else if(RxMessage.contains("StartNewPath:"))
        {
        //request the station information
            this->remainDistToStation = 0;
            //this->remainDistToStationTxToEBox = 0;
            this->ODOMark4VTPStationCalc = this->RxInfo.ODO;
            this->stationInfoToSocket = "Station:"+QString::number(1) + "," +QString::number(this->remainDistToStation);

            this->retrySendStationInfoToSocketFlag = true;
            qDebug()<<"StartNewPath:";
        }
    #if(CREATE_MAP_USED)
        else if(RxMessage.contains("BuildMap:"))
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
            obj.insert("0->0","0,0");

            QJsonDocument doc;
            doc.setObject(obj);

            file->open(QIODevice::WriteOnly|QIODevice::Truncate);
            file->seek(0);
            file->write(doc.toJson());
            file->flush();
            file->close();

            delete file;
        }
        else if(RxMessage.contains("MapSave:"))
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
        else if(RxMessage.contains("MapCancle:"))
        {
            this->CartStateCtlProcess->SetCartStateExternal(STATE_SB);
            this->inBuildMapModeFlag        = false;
        }
    #endif
    }
}

void FTR_CTR3SpeedCtl::tcpSocketSendMessageSlot(QString message)
{
    QString CurrentTimeStr = QString((QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss"))));

    message.replace("\n","");
    QStringList messageList = message.split(":");

    if(messageList.size()>=1)
    {
        QString value = "0";
        if(messageList.size() == 2) value = messageList.at(1);
        QString toSendMessage = QString("%0$%1$%2$%3$%4$%5\r").arg(0).arg(0).arg(CurrentTimeStr).arg(0).arg(messageList.at(0)).arg(value);
        this->tcpSocket->write(toSendMessage.toLatin1());
    }
}
#endif

void FTR_CTR3SpeedCtl::lowPowerToShutDownSystemSlot(void)
{
    qDebug()<<"LowPower ShutDown@:"<<this->batCapacityInfo.voltage;
    system("shutdown -h now");
}

void FTR_CTR3SpeedCtl::ResetWIFISlot(void)
{
    QFile *file = new QFile("/home/pi/ftrCartCtl/wpa_supplicant.conf");
    if(file->exists()) file->remove();
    if(file->open(QIODevice::WriteOnly))
    {
        file->write(WPA_SUPPLICANT_CONF.toLocal8Bit().data());
        file->flush();
        file->close();
    }
    delete file;

    QString cmd = "sudo cp -f /home/pi/ftrCartCtl/wpa_supplicant.conf " + WPA_SUPPLICANT_CONF_FILE_NAME;
    qDebug()<<cmd;
    QProcess *process = new QProcess();
    process->start(cmd);
    process->waitForStarted();
    process->waitForFinished();

    process->start("sync");
    process->waitForStarted();
    process->waitForFinished();

    cmd = "wpa_cli -i wlan0 reconfig";
    qDebug()<<cmd;
    process->start(cmd);
    process->waitForStarted();
    process->waitForFinished();
    delete process;

    this->EmitBeepSound(BEEP_TYPE_LONG_BEEP_3);
}

void FTR_CTR3SpeedCtl::calcCapacitySlot(double voltage)
{
    const double batCoef[6] = {-0.0021095004 ,  0.4009820894 , -30.4476371198 ,  1154.4151180135 , -21853.1489934864 , 165216.3203402679};
    if(this->batCapacityInfo.voltage > voltage) this->batCapacityInfo.voltage = voltage;
    this->batCapacityInfo.Capacity = batCoef[0]*qPow(this->batCapacityInfo.voltage,5) + batCoef[1]*qPow(this->batCapacityInfo.voltage,4) + batCoef[2]*qPow(this->batCapacityInfo.voltage,3) + batCoef[3]*qPow(this->batCapacityInfo.voltage,2) + batCoef[4]*qPow(this->batCapacityInfo.voltage,1) + batCoef[5];
    if(this->batCapacityInfo.Capacity > this->batCapacityInfo.theMaxCapacity) this->batCapacityInfo.Capacity = this->batCapacityInfo.theMaxCapacity;
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
#if(CREATE_UPDATEALLAPP_SCRIPT_USED)
void FTR_CTR3SpeedCtl::CreateUpdateScript(void)
{
    QFile *file = new QFile(UPDATEALLAPP_SCRIPT_FILE_NAME);
    if(file->exists())
    {
        printf("UpdateScript Exist,Create into /tmp:");
        if(file->copy(UPDATEALLAPP_SCRIPT_DIST_DIR))
        {
            printf("Successful!\n");
            system("sync;sync;");
            system("sudo chmod 777 /tmp/updateAllApp");
        }
        else
        {
            printf("Fail!\n");
        }
    }
    else
    {
        printf("UpdateScript Not Exist:\n");
    }
}
#endif
void FTR_CTR3SpeedCtl::CheckUpdateScript(void)
{
    QString oldShVersion;

    QFile *file = new QFile(FTRCARTCTL_UPDATE_SH_FILE_NAME);
    if(file->exists())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            do
            {
                QString shStr = file->readLine();
                if(shStr.contains("version:"))
                {
                    oldShVersion = shStr.replace("#","").replace("\n","");
                    break;
                }
            }while(1);
            file->close();

            if(oldShVersion != FTRCARTCTL_UPDATE_SH_VERSION)
            {
                qDebug()<<FTRCARTCTL_UPDATE_SH_FILE_NAME<<oldShVersion<<" ==> "<<FTRCARTCTL_UPDATE_SH_VERSION;
                if(file->open(QIODevice::WriteOnly|QIODevice::Truncate))
                {
                    file->seek(0);
                    file->write(FTRCARTCTL_UPDATE_SH.toUtf8());
                    file->flush();
                    file->close();
                    qDebug()<<FTRCARTCTL_UPDATE_SH_FILE_NAME<<"Modified!";
                }
            }
            else
            {
                qDebug()<<FTRCARTCTL_UPDATE_SH_FILE_NAME<<"is:"<<FTRCARTCTL_UPDATE_SH_VERSION;
            }
        }
    }

    file->setFileName(EBOX_UPDATE_SH_FILE_NAME);
    if(file->exists())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            do
            {
                QString shStr = file->readLine();
                if(shStr.contains("version:"))
                {
                    oldShVersion = shStr.replace("#","").replace("\n","");
                    break;
                }
            }while(1);
            file->close();

            if(oldShVersion != EBOX_UPDATE_SH_VERSION)
            {
                qDebug()<<EBOX_UPDATE_SH_FILE_NAME<<oldShVersion<<" ==> "<<EBOX_UPDATE_SH_VERSION;
                if(file->open(QIODevice::WriteOnly|QIODevice::Truncate))
                {
                    file->seek(0);
                    file->write(EBOX_UPDATE_SH.toUtf8());
                    file->flush();
                    file->close();
                    qDebug()<<EBOX_UPDATE_SH_FILE_NAME<<"Modified!";
                }
            }
            else
            {
                qDebug()<<EBOX_UPDATE_SH_FILE_NAME<<"is:"<<EBOX_UPDATE_SH_VERSION;
            }
        }
    }

    //update Lz Python script
#if(0)
    QFileInfo *fileInfo= new QFileInfo("/home/pi/ftrCartCtl/UpDateApp.py");
    if(fileInfo->exists() && (fileInfo->size() != 0))
    {
        system("sudo cp -f /home/pi/ftrCartCtl/UpDateApp.py /home/pi/Lz/");
        system("sync;sync;");
        qDebug()<<"UpDateApp.py modified:";
    }
    else
    {
        qDebug()<<"UpDateApp.py File Err:";
    }
    delete fileInfo;
#endif
    delete file;
}

void FTR_CTR3SpeedCtl::getVisionVersionInfoSlot(void)
{
    QFile *file = new QFile(VISION_VERSION_FILE_NAME);
    if(file->exists())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            this->visionVersionStr = tr("vision:") + file->readAll();
            qDebug()<<"vision V:"<<this->visionVersionStr;
        }
    }

    delete file;
}

void FTR_CTR3SpeedCtl::updateUi(void)
{
    QString oldShVersion="";
    QFile *file = new QFile(UI_SCRIPT_DIST_DIR);
    if(file->exists())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            do
            {
                QString shStr = file->readLine();
                if(shStr.contains("# Version:",Qt::CaseSensitive) || shStr.isNull() || shStr.isEmpty())
                {
                    oldShVersion = shStr.replace("# ","").replace("\n","").replace("\r","");
                    break;
                }
            }while(1);

            file->close();

            if(oldShVersion != UI_SCRIPT_VERSION_INFO)
            {
                qDebug()<<UI_SCRIPT_DIST_DIR<<oldShVersion<<" ==> "<<UI_SCRIPT_VERSION_INFO;
                QFile::remove(UI_SCRIPT_DIST_DIR);
                if(!QFile::copy(UI_SCRIPT_FILE_NAME,UI_SCRIPT_DIST_DIR)) qDebug()<<"Fail!!";

            }
            else
            {
                qDebug()<<UI_SCRIPT_DIST_DIR<<"is:"<<UI_SCRIPT_VERSION_INFO;
            }
        }
    }
    else
    {
        QFile::copy(UI_SCRIPT_FILE_NAME,UI_SCRIPT_DIST_DIR);
    }

    delete file;
}
void FTR_CTR3SpeedCtl::updateAutoRunScript(void)
{
    QString oldShVersion="";
    QFile *file = new QFile(AUTO_RUN_DIST_DIR);
    if(file->exists())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            do
            {
                QString shStr = file->readLine();
                if(shStr.contains("# Version:",Qt::CaseSensitive) || shStr.isNull() || shStr.isEmpty())
                {
                    oldShVersion = shStr.replace("# ","").replace("\n","").replace("\r","");
                    break;
                }
            }while(1);

            file->close();

            if(oldShVersion != AUTO_RUN_VERSION_INFO)
            {
                qDebug()<<AUTO_RUN_DIST_DIR<<oldShVersion<<" ==> "<<AUTO_RUN_VERSION_INFO;
                QFile::remove(AUTO_RUN_DIST_DIR);
                if(!QFile::copy(AUTO_RUN_FILE_NAME,AUTO_RUN_DIST_DIR)) qDebug()<<"Fail!!";
                QFile::setPermissions(AUTO_RUN_DIST_DIR,QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser | QFileDevice::ReadGroup | QFileDevice::ExeGroup | QFileDevice::ReadOther | QFileDevice::ExeOther);

            }
            else
            {
                qDebug()<<AUTO_RUN_DIST_DIR<<"is:"<<AUTO_RUN_VERSION_INFO;
            }
        }
    }
    else
    {
        QFile::copy(AUTO_RUN_FILE_NAME,AUTO_RUN_DIST_DIR);
        QFile::setPermissions(AUTO_RUN_DIST_DIR,QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser | QFileDevice::ReadGroup | QFileDevice::ExeGroup | QFileDevice::ReadOther | QFileDevice::ExeOther);
    }

    delete file;
}
void FTR_CTR3SpeedCtl::checkSettingsJsonFile(void)
{
    QFile *file = new QFile(SETTINGS_DIST_DIR);
    if(file->exists())
    {
        if(file->size() == 0)
        {
            qDebug()<<SETTINGS_DIST_DIR<<"is Empty,Recover it now!";
            QFile::remove(SETTINGS_DIST_DIR);
            if(!QFile::copy(SETTINGS_FILE_NAME,SETTINGS_DIST_DIR)) qDebug()<<"Fail!!";
        }
        else
        {
            qDebug()<<SETTINGS_DIST_DIR<<"is good!";
        }
    }
    else
    {
        QFile::copy(SETTINGS_FILE_NAME,SETTINGS_DIST_DIR);
    }
}

void FTR_CTR3SpeedCtl::SetToPushInWorkSlot()
{    
    this->VTKInfo.ToPushFlag = !this->VTKInfo.ToPushFlag;
    this->VTPInfo.ToPushFlag = !this->VTPInfo.ToPushFlag;
    qDebug()<<"SetToPushInWork:"<<this->VTKInfo.ToPushFlag<<this->VTPInfo.ToPushFlag;
}

//void FTR_CTR3SpeedCtl::TKeyClickedInVTKSlot()
//{
//    this->VTKInfo.TKeyClickedFlag = true;
//    qDebug()<<"T Key:"<<this->VTKInfo.TKeyClickedFlag;
//}

#if(ROUNT_USED)
void FTR_CTR3SpeedCtl::loadRount(QString fileName)
{
    qDebug()<<"loadRount:"<<fileName;

    QFile *file = new QFile(fileName);
    if(!file->isOpen() && file->exists())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            QByteArray data = file->readAll();
            file->close();

            //使用json文件对象加载字符串
            QJsonParseError json_error;
            QJsonDocument doc=QJsonDocument::fromJson(data,&json_error);

            if(json_error.error != QJsonParseError::NoError)
            {
                qDebug()<<"Setting JSON ERROR!";
                return;
            }

            //判断是否对象
            if(doc.isObject())
            {
                //把json文档转换为json对象
                QJsonObject obj=doc.object();

                this->rountList.clear();
                QJsonObject::iterator iteratorJson = obj.begin();
                while(iteratorJson != obj.end())
                {
                    if(iteratorJson.key().contains("path"))
                    {
                        QStringList rountList = iteratorJson.value().toString().split(",");
                        for(quint16 i =0;i<rountList.size();i++)
                        {
                            QStringList stationInRountStrList = rountList.at(i).split("$");
                            if(stationInRountStrList.size() == 5)
                            {
                                StationInfo_t stationInfo;
                                stationInfo.name = stationInRountStrList.at(0);
                                stationInfo.dist = stationInRountStrList.at(1).toUInt();
                                stationInfo.speed= stationInRountStrList.at(2).toUInt();
                                stationInfo.action = (ActionOnCrossType_e)(stationInRountStrList.at(3).toUInt());
                                stationInfo.pauseTime = stationInRountStrList.at(4).toUInt();
                                this->rountList.append(stationInfo);
                            }
                        }
                    }
                    else if(iteratorJson.key().contains("repeat"))
                    {
                        this->RouteRepeatFlag = iteratorJson.value().toBool();
                    }
                    iteratorJson++;
                }
                this->stationsInRoute = this->rountList.size();
            }
        }
    }
    delete file;
    QList<StationInfo_t>::iterator i;//使用只读迭代器
    for(i=this->rountList.begin(); i !=this->rountList.end();i++)
    {
        qDebug()<<i->name<<i->dist<<i->speed<<i->action<<i->pauseTime;
    }
    qDebug()<<"Route Repeat:"<<this->RouteRepeatFlag;
}

StationInfo_t FTR_CTR3SpeedCtl::GetActionBaseStationName(QString name,bool Repeat)//1:Forward,0:Backward
{
    StationInfo_t currentStationInfo;

    bool convertResult = false;
    quint16 stationIndex = name.toUInt(&convertResult);
    if(convertResult && (stationIndex < this->stationsInRoute))
    {
        StationInfo_t station = this->rountList.at(stationIndex);

        currentStationInfo.name = name;
        currentStationInfo.dist = station.dist;
        currentStationInfo.speed= station.speed;
        currentStationInfo.action= station.action;
        currentStationInfo.pauseTime=station.pauseTime;

        qDebug()<<"SInfo:"<<name<<currentStationInfo.dist<<currentStationInfo.speed<<currentStationInfo.action<<currentStationInfo.pauseTime;
    }
    else
    {
        qDebug()<<"Station Name Err:"<<stationIndex<<this->stationsInRoute;
    }
    return currentStationInfo;
}
#endif

FTR_CTR3SpeedCtl::~FTR_CTR3SpeedCtl()
{    
    delete JsonFile;
    delete MainInputPipeFile;
    //delete VTKOutputPipeFile;
    //delete VTPOutputPipeFile;
    delete SaveLogFile;

    delete SettingJsonFile;

    delete fileWatcher;
    delete AppUART;

    delete Time2SendData;
    delete Time2Loop;
    #if(BLUETOOTH_SERIAL_USED)
        delete Time4RCTimeout;
    #endif

#if(STREAMLIT_USED)
    this->StopStreamlitUISlot();
#endif
    //delete SBProcess;
    //delete RCProcess;
    //delete VTKProcess;

    delete ReadVTKPipeProcess;
    delete ReadVTPPipeProcess;

    delete CartStateCtlProcess;
    qDebug()<<"End R3 Process";
}
