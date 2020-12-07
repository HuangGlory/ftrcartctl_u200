#ifndef FTR_CTR3SPEEDCTL_H
#define FTR_CTR3SPEEDCTL_H

#include <QObject>
//#include <QSharedDataPointer>
#include <QtDebug>
#include <sb_thread.h>
#include <rc_thread.h>
#include <vtk_thread.h>
#include "motorctrlinfo.h"
#include "PlatformParameter.h"
#include <wiringPi.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtMath>

#include <cartstatesetting.h>
#include <QFileSystemWatcher>
#include <readvtkpipe_thread.h>
#include <readvtppipe_thread.h>
#include <readinputpipe_thread.h>

#include <clearpipe_thread.h>
//#include <bluetoothserialport.h>
#include <QProcess>
#include <QDir>

#include <updateScript.h>
#include <wifiConf.h>
#include <streamlitApp.h>

#include <QTcpServer>  //监听套接字
#include <QTcpSocket> //通信套接字

#if(PLATFORM == PLATFORM_R3)
#include <imu.h>
#endif

#if(CREATE_MAP_USED)
#include "mapinfo.h"
#endif

#include <iostream>
using namespace std;

//map
#define MAP_TEMP_FILE_NAME tr("/tmp/map.json")
#define MAP_FILE_NAME      tr("/home/pi/ftrCartCtl/map.json")

//Json
#define JSON_FILE_NAME      tr("/home/pi/vision/data/config.json")
#define SETTING_JSON_FILE_NAME tr("/home/pi/ftrCartCtl/settings.json")
#define SETTING_WHEEL_DIAM_JSON_FILE_NAME tr("/home/pi/ftrCartCtl/setting_d.json")

#define MAIN_INPUT_DEFAULT_PIPE_NAME    tr("/tmp/vision_pipe_input")
#define VTK_OUTPUT_DEFAULT_PIPE_NAME    tr("/tmp/person_pipe_output")
#define VTP_OUTPUT_DEFAULT_PIPE_NAME    tr("/tmp/tape_pipe_output")

#define FTRCARTCTL_IN_PIPE_NAME         ("/tmp/ftrCartCtl_pipe_input")
#define FTRCARTCTL_OUT_PIPE_NAME        ("/tmp/ftrCartCtl_pipe_output")
#define FTRCARTCTL_INFO_OUT_TO_VISON_PIPE_NAME ("/tmp/cart_status_vision_pipe_input")

#define LOG_PATH_NAME                   ("/home/pi/ftrCartCtl/log/")

#if(PLATFORM == PLATFORM_U250)
#define USED_DEFAULT_PARAMETER_ON_STATION   (1)

#define VERSION                         tr("ftrCartCtl Ver:0.0.8.00.U200@20201123\n\n")
/***********************
 * log:
 * ftrCartCtl Ver:0.0.8.00.U200@20201123
 * 1.增加route information
 * 2.中间层加发到下一站的距离信息
 * 3.发的vision的数据加一个位置，startToCatchCross标志
 * 4.用行走的ODO来计算加减带时机
 * 5.增加route simulator
 * 6.map增加弧形标志
 * 7.发给Socket的Station信息，原来的--dist：改为逗号
 * 8.VTP增加发送两个字节，表示到站remain dist
 * 9.retry to send station info to socket per 100ms
 * 10.when lost the cross,用odo换站
 *
 * Ver:0.0.7.03.U200@20201112
 * 1.启动时会开始一次streamlit，fix有时第一次启动streamlit不work问题
 * 2.Create map automatic
 * 3.启动后将map.json copy到/tmp下
 * 4.VTK下可以兼容vision的2/3笔数据
 * 5.VTK下侦测T key，可以在VTK下解除不近距离不能原地旋转问题
 *
 * Ver:0.0.7.02.U200@20201110
 * 1.增加过滤无效点信息at vtp
 * 2.重定义加减速
 * Ver:0.0.7.01.U200@20201103
 * 1.增加更新软件时断点继传
 * 2.settings.json增加绕障开关oa_avoiding_vtk
 * 3.在VTK or VTP mode下可以motor release
 *
 * Ver:0.0.7.00.U200@20201030
 * 1.增加streamlit做ui
 * 2.不读config.json的三个pipe的名称
 * 3.修改OA状态提示灯，原来用P&G灯提示，改为用SBled提示
 * 4.修改P&G灯用来提示Follow的状态（Pause/Idle/Normal)
 *
 * Ver:0.0.6.05.U200@20201023
 * 1.增加重置wifi功能
 * 2.ftrCartCtl 更新脚本增加check autoRun.sh file 是否为空
 *
 * Ver:0.0.6.04.U200@20201019
 * 1.follow增加一笔身高距离
 *
 * Ver:0.0.6.03.U200@20201015
 * 1.取消转弯时不看mark的条件
 *
 * Ver:0.0.6.02.U200@20200929
 * 1.修改LZ updateApp.py脚本
 *
 * Ver:0.0.6.01.U200@20200924
 * 1.修改update.sh,增加check文件大小为空的问题
 * 2.修改socket发数据格式
 *
 * Ver:0.0.6.00.U200@20200923
 * 1.增加pipe_input时WheelCali可以cancle
 * 2.fix 第一次进入VTK时，不active时，超过30s不会到Pause的问题
 *
 * Ver:0.0.5.01.U200@20200922
 * 1.fix 电池显示当充满电时会超过100%的问题
 * 2.去更新软件更新脚本，增加sync
 *
 * Ver:0.0.5.00.U200@20200916
 * 1.增加向vision用pipe送车子状态，p&g，加减速，等信息
 * 2.在VTK下in idle超过1s进入Pause状态，在Pause状态时camera不使用推理捧，要再使用时需要按P&G and active
 * 3.在VTK下in pause超过一定时间后退出SB模式
 *
 * Ver:0.0.4.01.U200@20200909
 * 1.增加report从phone来的信息
 * 2.增加按键消抖时间，15->50
 *
 * Ver:0.0.4.00.U200@20200908
 * 1.使用10*10做站点
 * 2.增加没线转弯上报信息
 * 3.SB_Enter时增加发送电机直径
 * 4.电机直径独立一个json文件
 *
 * Ver:0.0.3.05.U200@20200829
 * 1.使用MAC加密，暂时不用
 * 2.修改直径校正
 *
 * Ver:0.0.3.04.U200@20200826
 * 1.added default action on mark
 *
 * Ver:0.0.3.03.U200@20200824
 * 1.增加电机直径校正
 * 2.增加In/Out pipe
 *
 * Ver:0.0.3.02.U200@20200821
 * 1.控制P&G不同的闪灯功能提示
 * 2.修改进入配置模式，SB下按三下P&G；
 * 3.VTK下，进入Idle时，加灯号提示
 *
 * Ver:0.0.3.01.U200@20200821
 * 1.看到站点时暂时使用全局配置信息
 * 2.action/arc turning/ 不看站点信息，也不看三角形
 * 3.report version info to socket when receive "VersionInfo:"
 *
 * Ver:0.0.3.00.U200@20200817
 * 1.add settting OA quickly in every mode
 * 2.增加有OA用P&G灯提示，没OA时灯灭，有OA时灯1S闪一次
 * 3.增加tx_log into settings.json
 * 4.增加在SB下，可以全局设置OA开or 关
 * 5.增加在站点上可以设置oa开关
 *
 * Ver:0.0.2.19.U200@20200815
 * 1.fix VTP speed up bug
 *
 * Ver:0.0.2.18.U200@20200808
 * 1.VTP 点数据用int16表示
 * 2.added report battery ratio to socket
 * 3.can setting lamp enable or disable use the setting.json file
 *
 * Ver:0.0.2.17.U200@20200806
 * 1.增加pause then GO,when pause case action,the P&G LEN will be turn on,and timeout turn off to indecate
 * 2.socket infomation 加前缀
 * 3.增加定时发送信息给socket，如果socket可用
 *
 * Ver:0.0.2.16.U200@20200803
 * 1.解决VTP下Uturn方向不能设置问题
 * 2.解决VTP下Camera还没ready时，车子有时会误动作问题
 *
 * Ver:0.0.2.15.U200@20200731
 * 1.解决VTP下camera还没ready时，BLDC会产生timeout异常的问题
 *
 *
 * Ver:0.0.2.14.U200@20200727
 * 1.mantain log files,log只保存最近5天的数据；
 * 2.增加没电关机功能,电池容量计算
 *
 * Ver:0.0.2.13.U200@20200723
 * 1.增加可以在setting.json中设定没线时的默认action
 * 2.setting.json中的oa_log修改为mix_log
 * 3.增加VTP log
 *
 * Ver:0.0.2.12.U200@20200717
 * 1.remove RC of phone
 * 2.增加站点Action，计算arc radius，自动计算弧形转方向
 * 3.看到数字站点时增加到站点的距离
 * 4.VTP没线时默认uturn-go
 * 5.增加phone可以Pause or Go
 *
 * Ver:0.0.2.11.U200@20200714
 * 1.增加TCP通信，for smart phone
 * 2.可以使用phone做rc控制
 * 3.增加VTP下看站点功能，线段OA开关功能，到站发action功能。
 *
 * Ver:0.0.2.9.U200@20200629
 * 1.解决有时候handset按键没响应问题；
 * 2.解决有时Raspi状态与Cart状态不同步问题
 * 4.added read version infomation of E-BOX
 *
 * Ver:0.0.2.9@20200629
 * 1.解决action后速度不会回到最低速的bug
 *
 * Ver:0.0.2.8@20200622
 * 1.added speed up&down cmd for tape track
 *
 * Ver:0.0.2.7@20200619
 * 1.clear pipe
 *
 * Ver:0.0.2.6@20200617
 * 1.added mpu6050 data
 *
 * Ver:0.0.2.5@20200616
 * 1.退出VTK or VTP时清掉pipe的旧数据
 *
 * Ver:0.0.2.4@20200605
 * 1.added beep type
 * 2.added E-Box program
 *
 * Ver:0.0.2.3@20200603
 * 1.fix sometimes change mode to VTK too fast,the thread not created
 *
 * Ver:0.0.2.2@20200529
 * 1.log增加时间，log增加一个回车
 *
 * Ver:0.0.2.1@20200521
 * 1.增加HS rc与模式切换
 *
 * Ver:0.0.2.0@20200520
 * 1.收到串口数据后不用QTestStream写log，用QTestStream写log似乎会引起mem leak
 * 2.析构函数增加delete 被new过的变量
 *
 *
 *
 * Ver:0.0.1.0@20200424:
 * 初版本
 * ********************/
#else
#define VERSION                         tr("ftrCartCtl Ver:0.0.4.00.R3@20200907\n\n")
/***********************
 * log:
 * Ver:0.0.4.00.R3@20200907
 * 1.与U200同步
 * 2.VTP加一个字节，调整pose位置
 * 3.使能licese功能
 * 4.SB_Enter时增加发送电机直径
 *
 * Ver:0.0.3.04.R3@20200828
 * 1.大部分功能与U200一致
 *
 * Ver:0.0.2.9.R3@20200629
 * 1.added imu
 * 2.解决有时候handset按键没响应问题；
 * 3.解决有时Raspi状态与Cart状态不同步问题
 * 4.added read version infomation of E-BOX
 *
 * Ver:0.0.2.9@20200629
 * 1.解决action后速度不会回到最低速的bug
 *
 * Ver:0.0.2.8@20200622
 * 1.added speed up&down cmd for tape track
 *
 * Ver:0.0.2.7@20200619
 * 1.clear pipe
 *
 * Ver:0.0.2.6@20200617
 * 1.added mpu6050 data
 *
 * Ver:0.0.2.5@20200616
 * 1.退出VTK or VTP时清掉pipe的旧数据
 *
 * Ver:0.0.2.4@20200605
 * 1.added beep type
 * 2.added E-Box program
 *
 * Ver:0.0.2.3@20200603
 * 1.fix sometimes change mode to VTK too fast,the thread not created
 *
 * Ver:0.0.2.2@20200529
 * 1.log增加时间，log增加一个回车
 *
 * Ver:0.0.2.1@20200521
 * 1.增加HS rc与模式切换
 *
 * Ver:0.0.2.0@20200520
 * 1.收到串口数据后不用QTestStream写log，用QTestStream写log似乎会引起mem leak
 * 2.析构函数增加delete 被new过的变量
 *
 *
 *
 * Ver:0.0.1.0@20200424:
 * 初版本
 * ********************/

#endif
/************communication ctl define*********************/
#define SOP_USB                     (quint8)(0x6b)
#define SOP_TTL                     (quint8)(0x55)

#define MCB_BLDC_PKT_SOP        (quint8)(0x02)

#define PAUSE_TO_SB_TIME_DEFAULT  (quint16)(300)//about 5min
#define IDLE_TO_PAUSE_TIME_DEFAULT  (quint16)(60)//about 5min

#define SEND_DATA_PER   (quint16)(1000)//(31) (97)
#define LOOP_PER        (quint8)(97)//(51)
#define RC_COMM_TIMEOUT (quint16)(1000)

#if(CREATE_MAP_USED)
#define DIST_CALI_4_VTP_MAP_CREATE  (qint16)(850) //about 1m
#endif

/*********end communication ctl define********************/

class FTR_CTR3SpeedCtlData;

class FTR_CTR3SpeedCtl : public QObject
{
    Q_OBJECT
public:
    explicit FTR_CTR3SpeedCtl(QObject *parent = nullptr);
    FTR_CTR3SpeedCtl(const FTR_CTR3SpeedCtl &);
    FTR_CTR3SpeedCtl &operator=(const FTR_CTR3SpeedCtl &);
    ~FTR_CTR3SpeedCtl();

    void    SendCMD(UserCmd cmd,quint8 data1 = 0x00,quint8 data2 = 0x00,quint8 data3 = 0x00);

    //check&modify update.sh
    void CheckUpdateScript(void);
    //tx cmd function
    void SB_Enter();
    void SB_RealTimeInfo(void);

    void RC_Enter();
    void RC_RealTimeInfo(void);

    void VTK_Exit();
    void VTK_Enter();
    void VTK_RealTimeInfo();
    void SetMotorRelease();
    void EmitBeepSound(BeepType_t BeepType);

    void VTP_Exit();
    void VTP_Enter();
    void VTP_RealTimeInfo();
    void VTP_SpeedUp();
    void VTP_SpeedDown();
    void VTP_SetActionOnState(ActionOnCrossType_e action);
    void VTP_InitParameter();
    void VTP_UpdateAction(ActionOnCrossType_e setAction);

    //json operate
    bool GetPipeName(QString jsonFile);
    bool GetSettingParameterFromJson(QString jsonFileName);

    //communication with vision application
    //NONE = -1,STANDBY = 0,MOTORRELEASE = 1,VISIONFOLLOW = 2,VISIONTAPE = 3
    void OpenPipe(QFile *pipeFile,QIODevice::OpenMode flags);
    //void ClosePipe(QFile *pipeFile);
    //void ClearPipe(QFile *pipeFile);

    //message output
    //static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

public slots:
    void SetToPushInWorkSlot();
    void TKeyClickedInVTKSlot();
#if(CREATE_MAP_USED)
    void CreateMapSlot(QString station,quint16 dist,qint16 maxSpeed,bool face,bool isFirstStation);
    void loadRount(QString fileName);
    StationInfo_t GetActionBaseStationName(QString name,bool Repeat);//1:Forward,0:Backward
#endif

#if(STREAMLIT_USED)
    void on_readoutputSlot(void);
    void CreateStreamlitAppSlot(void);
    void StartStreamlitUISlot(void);
    void StopStreamlitUISlot(void);
#endif

#if(BLUETOOTH_SERIAL_USED)
    void Time4RCTimeoutSlot(void);
#endif
    void Time2LoopSlot(void);
    void Timer2SendDataSlot(void);
    void PauseToGoTimerSlot(void);
    void RecoverOATimerSlot(void);

    void ReadUARTSlot(void);
    void UpdateVelocitySlot(MotorCtrlInfo);
    void CartStateSyncSlot(CartState_e CartState);

    //communication with vision application
    void WriteMainPipeSlot(CartState_e cartState);
    void WriteInfoToVisionPipeSlot(QString info);
    //void ReadVTPInfoFromPipeSlot();
    //void ReadVTKInfoFromPipeSlot();
    void fileChangedSlot(const QString & path);

    void WriteOutPipeSlot(QString str);

    void UpdateVTKInfoSlot(VTKInfo_t VTKInfo);
    void UpdateVTPInfoSlot(VTPInfo_t VTPInfo);
    void UpdatePipeInputSlot(QString str);
#if(PLATFORM == PLATFORM_R3)
    void UpdateIMUInfoSlot(Pose_t pose);
#endif
    //pause and go button slot
    void PNGButtonToggleSlot();

    //quickly setting OA slot
    void SettingOAToggleSlot(CartState_e cartState);
    bool SettingOAGlobalBaseJsonSlot(void);

    //set wheel diam
    bool WriteWheelDiamToBaseJsonSlot(void);

    ArcInfo_t CalcArcInfoSlot(PointAxis_t p1,PointAxis_t p2,PointAxis_t p3);

#if(BLUETOOTH_SERIAL_USED)
    //bluetooth rx rc cmd
    void BTRCCmdSlot(RCCMD_e);
#endif

    void tcpServerConnectionSlot(void);
    void tcpSocketReadSlot(void);
    void tcpSocketSendMessageSlot(QString message);
    void tcpSocketDisconnectSlot();

    void lowPowerToShutDownSystemSlot(void);
    void calcCapacitySlot(double voltage);

    void ResetWIFISlot(void);
signals:
    //void BroadcastCartStateSignal(CartState_e);
    //void ReadVTPInfoSignal();
    //void ReadVTKInfoSignal();
    void toCalcCapacitySignal(double voltage);
private:
    void SetODOFactor(double LeftODOFactor = ODO_FACTOR,double RightODOFactor = ODO_FACTOR);
    uint8_t  App_XOR(QByteArray data);
    uint8_t  App_CheckSum(QByteArray data);
    uint16_t App_CRC(QByteArray data);
    void SendPNGCMD(quint8 needState);
    //void BroadcastCartState(void);
    void SetCartState(CartState_e CartState);
    CartState_e GetCartState(void);
    void BLDC_StateSB_VelocityTxPreprocess(int16_t lvel, int16_t avel, int16_t *pSpeed_left, int16_t *pSpeed_right);
    void BLDC_StateRC_VelocityTxPreprocess(int16_t lvel, int16_t avel, int16_t *pSpeed_left, int16_t *pSpeed_right);
    //void BLDC_StateTK_VelocityTxPreprocess(int16_t lvel, int16_t avel, int16_t *pSpeed_left, int16_t *pSpeed_right);
    //void BLDC_StateMarch_VelocityTxPreprocess(int16_t lvel, int16_t avel, int16_t *pSpeed_left, int16_t *pSpeed_right);
    void TxToBLDC_RTData(int16_t speed_l,int16_t speed_r,uint8_t CtlByte,double LeftODOFactor,double RightODOFactor,CartState_e CartState,int8_t PIDStop);

    void ReadVersionOfEBox(void);
private:
    QSharedDataPointer<FTR_CTR3SpeedCtlData> data;

    //SB_Thread       *SBProcess;
    //RC_Thread       *RCProcess;
    //VTK_Thread      *VTKProcess;

    //Pipe thread
    ReadVTKPipe_Thread *ReadVTKPipeProcess;
    ReadVTPPipe_Thread *ReadVTPPipeProcess;
    ReadInputPipe_Thread *ReadInputPipeProcess;

    ClearPipe_Thread   *ClearVTKPipeProcess;
    ClearPipe_Thread   *ClearVTPPipeProcess;

    CartStateSetting *CartStateCtlProcess;

#if(BLUETOOTH_SERIAL_USED)
    BluetoothSerialPort *BTCtlProcess;
#endif

    QString         UartName;
    QSerialPort     *AppUART;

    QTimer          *Time2SendData;
    QTimer          *Time2Loop;
    QTimer          *PauseToGoTimer;
    QTimer          *RecoverTimer;

#if(BLUETOOTH_SERIAL_USED)
    QTimer          *Time4RCTimeout;
#endif

    CartState_e     PreCartState;
    CartState_e     CartState;
    CartState_e     HSWant2State;
    bool            CartInPauseState;
    bool            CartWantToPNGState;
    bool            HSBeUsedFlag;
    bool            VTKInIdleFlag;
    bool            VTKIdleIntoPauseFlag;
    bool            SpeedUpAndDownState;
    bool            StartActionFlag;
    bool            EndActionFlag;
    bool            InArcTurningFlag;
    bool            InLostTapeTurningFlag;
    bool            InTurningStateFlag;
    bool            InPauseStateFlag;
    bool            GetVersionOfEBoxFlag;
    bool            Wait4CameraReadyIndecateFlag;
    bool            IntoConfigureModeFlag;
    bool            InSetOAGlobalFlag;
    bool            SocketReadyFlag;
    bool            itNeedSendInfoToPhoneFlag;
    bool            EboxReadyFlag;

    bool            LampEnInVTKFlag;
    bool            LampEnInVTPFlag;

    bool            GlobaOAStateFlag;

    bool            NeedIntoODOCaliFlag;
    bool            NeedOutODOCaliFlag;
    bool            InODOCaliFlag;

    bool            SettingJsonErrorFlag;

    quint16         IdleToPauseInVTKTimeoutCnt;
    quint16         PauseToSBInVTKTimeoutCnt;
    quint16         WheelCaliDist;
    quint16         EncoderCnt;
    double          LeftWheelDiam;
    double          RightWheelDiam;
    quint8          cnt4IntoConfigModePNGToggle;
    quint8          cnt4ResetWIFIPNGToggle;
    quint8          RecoverOATimeout;
    int16_t         LeftSettingSpeed;
    int16_t         RightSettingSpeed;
    double          LeftODOFactor;
    double          RightODOFactor;
    double          AvgODOFactor;

    RxInfo_t        RxInfo;
    TaskFlag_t      TaskFlag;
    VTKInfo_t       VTKInfo;
    VTPInfo_t       VTPInfo;
    CartModeEnterParameter_t CartModeEnterParameter;
    CartModeEnterParameter_t SettingParameterFromJson;

    ShowLogFlag_t   ShowLogFlag;

    //double          LeftWheelDiameter;
    //double          RightWheelDiameter;

    QString         JsonFileName;
    QString         SettingJsonFileName;
    QString         MainInputPipeName;
    //QString         StateInfoInputPipeName;
    QString         VTKOutputPipeName;
    QString         VTPOutputPipeName;
    QString         ftrCartCtlInputPipeName;
    QString         ftrCartCtlOutputPipeName;

    QString         EboxVersionStr;

    QFile           *JsonFile;
    QFile           *MainInputPipeFile;
    QFile           *StateInfoInputPipeFile;
    QFile           *VTKOutputPipeFile;
    QFile           *VTPOutputPipeFile;
    QFile           *ftrCartCtlInputPipeFile;
    QFile           *ftrCartCtlOutputPipeFile;

    QFile           *SaveLogFile;

    QFile           *SettingJsonFile;

    QFileSystemWatcher *fileWatcher;

    //bool            TOF_OAFlag,US_OAFlag,UturnDirFlag;
#if(PLATFORM == PLATFORM_R3)
    imu             *imuData;
    Pose_t          pose;
#endif

    const double    CameraYAxisTable[7]={FW_Y_AXIS0,FW_Y_AXIS1,FW_Y_AXIS2,FW_Y_AXIS3,FW_Y_AXIS4,FW_Y_AXIS5,FW_Y_AXIS6};
    PointAxis_t     PointAxis4CalcArc[3];
    ArcInfo_t       ArcInfo;
    quint16         theMinR;

    QTcpServer *tcpServer; //定义监听套接字tcpServer
    QTcpSocket *tcpSocket; //定义通信套接字tcpSocket

    BatCapacityInfo_t batCapacityInfo;

    qint32          ODOMark4VTPStationCalc;
    quint16         MarkCntRecord;
    quint16         StationName4VTP;
    bool            FaceDirFlag;
    bool            usedDefaultFixedDistFlag;
    quint16         defaultFixedDist;
    quint16         distBtStation;
    quint16         remainDistToStation;

#if(STREAMLIT_USED)
    QProcess *streamlitProcess;
    qint64  streamlitAppPID;
    bool    streamlitKeepRunFlag;
#endif

#if(CREATE_MAP_USED)
    bool            inBuildMapModeFlag;
    bool            mapIsSealTypeFlag;
    quint16         totalStationsNum;
    MapInfo         *mapInfo;
#endif

#if(ROUNT_USED)
    QList<StationInfo_t> rountList;
    qint32          RemainDistToCross;
    quint8          stationsInRoute;
    bool            RouteRepeatFlag;
    bool            isTheLastStationFlag;
    bool            startToCatchCrossFlag;
#endif

    QString         stationInfoToSocket;
    bool            retrySendStationInfoToSocketFlag;
};

#endif // FTR_CTR3SPEEDCTL_H
