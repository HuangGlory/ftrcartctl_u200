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
#include <clearpipe_thread.h>
//#include <bluetoothserialport.h>
#include <QProcess>
#include <QDir>

#include <QTcpServer>  //监听套接字
#include <QTcpSocket> //通信套接字

#if(PLATFORM == PLATFORM_R3)
#include <imu.h>
#endif

#include <iostream>
using namespace std;

//Json
#define JSON_FILE_NAME      tr("/home/pi/vision/data/config.json")
#define SETTING_JSON_FILE_NAME tr("/home/pi/ftrCartCtl/settings.json")

#define MAIN_INPUT_DEFAULT_PIPE_NAME    tr("/tmp/vision_pipe_input")
#define VTK_OUTPUT_DEFAULT_PIPE_NAME    tr("/tmp/person_pipe_output")
#define VTP_OUTPUT_DEFAULT_PIPE_NAME    tr("/tmp/tape_pipe_output")

#define LOG_PATH_NAME                   ("/home/pi/ftrCartCtl/log/")

#if(PLATFORM == PLATFORM_U250)
#define VERSION                         tr("ftrCartCtl Ver:0.0.3.00.U200@20200817\n\n")
/***********************
 * log:
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
#define VERSION                         tr("ftrCartCtl Ver:0.0.2.9.R3@20200629\n\n")
/***********************
 * log:
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

#define SEND_DATA_PER   (quint16)(1000)//(31) (97)
#define LOOP_PER        (quint8)(97)//(51)
#define RC_COMM_TIMEOUT (quint16)(1000)

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
    void ClosePipe(QFile *pipeFile);
    void ClearPipe(QFile *pipeFile);

    //message output
    //static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

public slots:
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
    void ReadVTPInfoFromPipeSlot();
    void ReadVTKInfoFromPipeSlot();
    void fileChangedSlot(const QString & path);

    void UpdateVTKInfoSlot(VTKInfo_t VTKInfo);
    void UpdateVTPInfoSlot(VTPInfo_t VTPInfo);
#if(PLATFORM == PLATFORM_R3)
    void UpdateIMUInfoSlot(Pose_t pose);
#endif
    //pause and go button slot
    void PNGButtonToggleSlot();

    //quickly setting OA slot
    void SettingOAToggleSlot(CartState_e cartState);
    bool SettingOAGlobalBaseJsonSlot(void);

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
signals:
    //void BroadcastCartStateSignal(CartState_e);
    void ReadVTPInfoSignal();
    void ReadVTKInfoSignal();
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

    SB_Thread       *SBProcess;
    RC_Thread       *RCProcess;
    VTK_Thread      *VTKProcess;

    //Pipe thread
    ReadVTKPipe_Thread *ReadVTKPipeProcess;
    ReadVTPPipe_Thread *ReadVTPPipeProcess;
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
    bool            SpeedUpAndDownState;
    bool            StartActionOnEndTapeFlag;
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
    QString         VTKOutputPipeName;
    QString         VTPOutputPipeName;

    QString         EboxVersionStr;

    QFile           *JsonFile;
    QFile           *MainInputPipeFile;
    QFile           *VTKOutputPipeFile;
    QFile           *VTPOutputPipeFile;
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
};

#endif // FTR_CTR3SPEEDCTL_H
