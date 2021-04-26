#include "cartstatesetting.h"
#include <iostream>
using namespace std;

CartStateSetting *SetCartState = new CartStateSetting;

CartStateSetting::CartStateSetting(QObject *parent) : QObject(parent)
{
    //qDebug()<<"Structed";
}

CartStateSetting::CartStateSetting(QString saveFileName)
{
    qDebug()<<saveFileName;
    this->saveFileNamea = saveFileName;
    this->JsonFile      = new QFile;
    this->JsonFile->setFileName(this->saveFileNamea);

    this->InConfigFlag  = false;
    this->CameraReadyFlag = false;
    this->VTKInIdleFlag = false;

    this->LoopTimer = new QTimer;
    this->LEDBlinkSpeedCtlTimer = new QTimer;
    this->CurrentCartState = STATE_SB;
    this->PreCartState     = STATE_SB;
    this->ModeChangeBaseJson("mode",0);//set mode to SB

    //this->KeyDetectProcess = new KeyDetectThread;
    this->KeyHoldElapsedTime = new QTime;
#if(PIN_MODE == PIN_MODE_BCM)
    qDebug()<<"wiringPiSetup:"<<wiringPiSetupSys();
#else
    qDebug()<<"wiringPiSetup:"<<wiringPiSetup();
#endif
#if(0)
    int major,minor;
    wiringPiVersion(&major,&minor);
    qDebug("wiPI Ver:%d.%d",major,minor);
#endif
    this->BSP_Init();

    wiringPiISR(this->KEY_PIN[SB_KEY],INT_EDGE_BOTH,&this->KeyISR);//INT_EDGE_BOTH,INT_EDGE_RISING
    wiringPiISR(this->KEY_PIN[VTK_KEY],INT_EDGE_BOTH,&this->KeyISR);//INT_EDGE_BOTH,INT_EDGE_RISING
    wiringPiISR(this->KEY_PIN[VTP_KEY],INT_EDGE_BOTH,&this->KeyISR);//INT_EDGE_BOTH,INT_EDGE_RISING

    wiringPiISR(this->KEY_PIN[PNG_KEY],INT_EDGE_BOTH,&this->KeyISR);//Pause and Go Button

    connect(this->LEDBlinkSpeedCtlTimer,SIGNAL(timeout()),this,SLOT(LEDBlinkSpeedCtlTimerSlot()));
    connect(this->LoopTimer,SIGNAL(timeout()),this,SLOT(LoopTimerSlot()));
    connect(SetCartState,SIGNAL(KeyISRSignl()),this,SLOT(KeyISRSlot()));

    this->LoopTimer->start(50);
    this->LEDBlinkSpeedCtlTimer->start(SLOW_BLINK_TIME);
}

void CartStateSetting::KeyISR()
{
    //qDebug()<<"KeyISR";
    CartStateSetting::emitKeyISRSignal();
}

void CartStateSetting::emitKeyISRSignal()
{
    emit SetCartState->KeyISRSignl();
}

void CartStateSetting::KeyISRSlot(void)
{
    //qDebug()<<"KeyISRSLot";
    QThread::msleep(50);//15
#if(PLATFORM == PLATFORM_U250)
    if(this->BSP_GetKeyValue(PNG_KEY))//HIGH
    {
        //qDebug()<<"PNG_KEY Stop"<<this->KeyHoldElapsedTime->elapsed();
    }
    else//LOW
    {
        //qDebug()<<"PNG_BUTTON";
        emit this->PNGButtonToggleSignal();
    }
#endif

    if(this->BSP_GetKeyValue(SB_KEY))//HIGH
    {
        //qDebug()<<"SB_KEY Stop"<<this->KeyHoldElapsedTime->elapsed();
    }
    else if(this->BSP_GetKeyValue(VTK_KEY) && this->BSP_GetKeyValue(VTP_KEY))//LOW
    {
        this->KeyHoldElapsedTime->start();
        this->WhichKey = tr("SB_KEY");
        //qDebug()<<"SB_KEY";
    }

    if(this->BSP_GetKeyValue(VTK_KEY))//HIGH
    {
        //qDebug()<<"VTK_KEY Stop"<<this->KeyHoldElapsedTime->elapsed();
    }
    else if(this->BSP_GetKeyValue(SB_KEY) && this->BSP_GetKeyValue(VTP_KEY))//LOW
    {
        this->KeyHoldElapsedTime->start();
        this->WhichKey = tr("VTK_KEY");
        //qDebug()<<"VTK_KEY";
    }
#if(PLATFORM == PLATFORM_U250)
    if(this->BSP_GetKeyValue(VTP_KEY))//HIGH
    {
        //qDebug()<<"VTP_KEY Stop"<<this->KeyHoldElapsedTime->elapsed();
    }
    else if(this->BSP_GetKeyValue(VTK_KEY) && this->BSP_GetKeyValue(SB_KEY))//LOW
    {
        this->KeyHoldElapsedTime->start();
        this->WhichKey = tr("VTP_KEY");
        //qDebug()<<"VTP_KEY";
    }
#else
    if(this->BSP_GetKeyValue(VTP_KEY))//HIGH
    {
        //qDebug()<<"VTP_KEY Stop"<<this->KeyHoldElapsedTime->elapsed();
    }
    else if(this->BSP_GetKeyValue(VTK_KEY) && this->BSP_GetKeyValue(SB_KEY))//LOW
    {
        //this->KeyHoldElapsedTime->start();
        //this->WhichKey = tr("VTP_KEY");
        //qDebug()<<"VTP_KEY";
        emit this->PNGButtonToggleSignal();
    }
#endif
}

void CartStateSetting::LEDBlinkSpeedCtlTimerSlot()
{
    switch(this->CurrentCartState)
    {
    case STATE_SB:
        {
            if(this->InConfigFlag)
            {
                this->BSP_LedToggle(SB_LED);
                this->BSP_LedToggle(RC_LED);
                this->BSP_LedToggle(VTP_LED);
            }
            else
            {
                this->BSP_LedToggle(SB_LED);
            }
        }
        break;
    case STATE_MOTOR_RELEASE:
        {
            this->BSP_LedToggle(SB_LED);
        }
        break;
    case STATE_VMARCH:
    case STATE_RC:
        {
            this->BSP_LedToggle(RC_LED);
        }
        break;
    case STATE_VTK:
        {
            if(this->CameraReadyFlag && (!this->VTKInIdleFlag))
            {
                this->BSP_LedToggle(RC_LED);
                if(this->OAStateFlag) this->BSP_LedToggle(SB_LED);
            }
            else
            {
                this->BSP_LedToggle(SB_LED);
                this->BSP_LedToggle(RC_LED);
                this->BSP_LedToggle(VTP_LED);
            }
        }
        break;
    case STATE_VTP:
        {
            if(this->CameraReadyFlag)
            {
                this->BSP_LedToggle(VTP_LED);
                if(this->OAStateFlag) this->BSP_LedToggle(SB_LED);
            }
            else
            {
                this->BSP_LedToggle(SB_LED);
                this->BSP_LedToggle(RC_LED);
                this->BSP_LedToggle(VTP_LED);
            }
        }
        break;

    case STATE_MARCH:
        {

        }
        break;
     default:
        break;
    }
}

int8_t CartStateSetting::BSP_LED_Init(LED_TypeDef Led)
{
    int8_t ret = BSP_ERROR_NONE;
    if(Led > LEDn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else //setting as gpio output
    {
        //qDebug()<<Led<<this->LED_PIN[Led]<<"output";
        pinMode(this->LED_PIN[Led],OUTPUT);
        digitalWrite(this->LED_PIN[Led],LOW);
    }
    return ret;
}
int8_t CartStateSetting::BSP_KEY_Init(KEY_TypeDef Key)
{
    int8_t ret = BSP_ERROR_NONE;
    if(Key > KEYn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else //setting as gpio input
    {
        pinMode(this->KEY_PIN[Key],INPUT);
        pullUpDnControl(this->KEY_PIN[Key],PUD_OFF);
        //qDebug()<<this->KEY_PIN[Key]<<PUD_UP;
    }
    return ret;
}

int8_t CartStateSetting::BSP_GetKeyValue(KEY_TypeDef Key)
{
    int8_t ret = BSP_ERROR_NONE;
    if(Key > KEYn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else
    {
        ret = digitalRead(this->KEY_PIN[Key]);
    }
    return ret;
}
void CartStateSetting::BSP_SetLed(LED_TypeDef Led,int value)
{
    int8_t ret = BSP_ERROR_NONE;
    if(Led > LEDn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else
    {
        digitalWrite(this->LED_PIN[Led],value);
    }
}
void CartStateSetting::BSP_LedToggle(LED_TypeDef Led)
{
    int8_t ret = BSP_ERROR_NONE;
    if(Led > LEDn)
    {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else
    {
        digitalWrite(this->LED_PIN[Led],!digitalRead(this->LED_PIN[Led]));
        //qDebug()<<Led<<this->LED_PIN[Led]<<value;
    }
}

void CartStateSetting::BSP_Init()
{
    this->BSP_LED_Init(SB_LED);
    this->BSP_LED_Init(RC_LED);
    this->BSP_LED_Init(VTP_LED);
    this->BSP_LED_Init(PNG_LED);
    this->BSP_LED_Init(LAMP_LED);

    this->BSP_KEY_Init(SB_KEY);
    this->BSP_KEY_Init(VTK_KEY);
    this->BSP_KEY_Init(VTP_KEY);
    this->BSP_KEY_Init(PNG_KEY);
}
void CartStateSetting::LoopTimerSlot()
{
    /*******key detecting*******************/
    if(!this->WhichKey.isEmpty())
    {
        if(this->KeyHoldElapsedTime->elapsed()>LONG_PRESS_TIME)//about 3s
        {
            if(this->WhichKey.contains("SB_KEY"))
            {
                 this->CurrentCartState = (this->PreCartState == STATE_SB)?(STATE_MOTOR_RELEASE):(STATE_SB);
                //qDebug()<<"MotorRelease";
            }
            else if(this->WhichKey.contains("VTK_KEY"))
            {
                if(this->PreCartState == STATE_VTK)
                {
                    emit this->SettingOAToggleSignal(STATE_VTK);
                }
            }
            else//"VTP_KEY
            {
                if(this->PreCartState == STATE_VTP)
                {
                    emit this->SettingOAToggleSignal(STATE_VTP);
                }
            }
            this->WhichKey.clear();
        }
        else if(this->KeyHoldElapsedTime->elapsed()>SHORT_PRESS_TIME)
        {
            QThread::msleep(SHORT_PRESS_TIME);
            if(this->BSP_GetKeyValue(SB_KEY) && this->BSP_GetKeyValue(VTK_KEY) && this->BSP_GetKeyValue(VTP_KEY))//key release
            {
                if(this->WhichKey.contains("SB_KEY"))
                {
                    this->CurrentCartState = STATE_SB;
                    //qDebug()<<"CartState SB"<<STATE_SB;
                }
                else if(this->WhichKey.contains("VTK_KEY"))
                {
                    //this->CurrentCartState = (this->PreCartState == STATE_SB)?(STATE_VTK):(STATE_SB);
                    if(this->PreCartState == STATE_SB)
                    {
                        this->CurrentCartState = STATE_VTK;
                    }
                    else if(this->CurrentCartState == STATE_VTP)
                    {
                        emit this->SetToPushInWorkSignal();
                    }
                }
                else//"VTP_KEY
                {
                    //this->CurrentCartState = (this->PreCartState == STATE_SB)?(STATE_VTP):(STATE_SB);
                    if(this->PreCartState == STATE_SB)
                    {
                        this->CurrentCartState = STATE_VTP;
                    }
                    else if(this->CurrentCartState == STATE_VTK)
                    {
//                        emit this->TKeyClickedInVTKSignal();
                        emit this->SetToPushInWorkSignal();
                    }
                }
                this->WhichKey.clear();
            }
        }
    }
    /****end key detecting*******************/
    //state sync
    if(this->CurrentCartState != this->PreCartState)
    {
        this->PreCartState = this->CurrentCartState;
        this->BSP_SetLed(SB_LED,LOW);
        this->BSP_SetLed(RC_LED,LOW);
        this->BSP_SetLed(VTP_LED,LOW);

        emit this->BroadcastCartStateSignal(this->CurrentCartState);

        //NONE = -1,STANDBY = 0,MOTORRELEASE = 1,VISIONFOLLOW = 2,VISIONTAPE = 3
        switch(this->CurrentCartState)
        {
        case STATE_SB:
            {
                this->ModeChangeBaseJson("mode",0);
                this->LEDBlinkSpeedCtlTimer->start(SLOW_BLINK_TIME);
                //qDebug()<<"CartState SB:"<<0;
            }
            break;
        case STATE_MOTOR_RELEASE:
            {
                this->ModeChangeBaseJson("mode",1);
                this->LEDBlinkSpeedCtlTimer->start(FAST_BLINK_TIME);
                //qDebug()<<"CartState MotorRelease:"<<1;
            }
            break;
        case STATE_VMARCH:
        case STATE_RC:
            {
                this->ModeChangeBaseJson("mode",0);
                this->LEDBlinkSpeedCtlTimer->start(FAST_BLINK_TIME);
                //qDebug()<<"CartState RC:"<<0;
            }
            break;
        case STATE_VTK:
            {
                this->ModeChangeBaseJson("mode",2);
                this->LEDBlinkSpeedCtlTimer->start(SLOW_BLINK_TIME);
                //qDebug()<<"CartState VTK:"<<2;
            }
            break;
        case STATE_VTP:
            {
                this->ModeChangeBaseJson("mode",3);
                this->LEDBlinkSpeedCtlTimer->start(SLOW_BLINK_TIME);
                //qDebug()<<"CartState VTP:"<<3;
            }
            break;

        case STATE_MARCH:
            {

            }
            break;
         default:
            break;
        }
    }
}

void CartStateSetting::SetCartStateExternal(CartState_e state)
{
    qDebug()<<"SCStateE:"<<this->PreCartState<<this->CurrentCartState;
    //printf("SCStateE:%d,%d\n",this->PreCartState,this->CurrentCartState);
    if(state != this->PreCartState)
    {
        this->CurrentCartState = (this->PreCartState == STATE_SB)?(state):(STATE_SB);
    }
}

void CartStateSetting::SetInConfigModeFlagSlot(bool state)
{
    this->InConfigFlag = state;
    this->BSP_SetLed(SB_LED,LOW);
    this->BSP_SetLed(RC_LED,LOW);
    this->BSP_SetLed(VTP_LED,LOW);
}

void CartStateSetting::SetCameraReadyFlagSlot(bool state)
{
    this->CameraReadyFlag = state;
    this->BSP_SetLed(SB_LED,LOW);
    this->BSP_SetLed(RC_LED,LOW);
    this->BSP_SetLed(VTP_LED,LOW);
}

void CartStateSetting::SetVTKInIdleFlag(bool state)
{
    this->VTKInIdleFlag = state;

    this->LEDBlinkSpeedCtlTimer->start((this->CameraReadyFlag && this->VTKInIdleFlag)?(FAST_BLINK_TIME):(SLOW_BLINK_TIME));

    this->BSP_SetLed(SB_LED,LOW);
    this->BSP_SetLed(RC_LED,LOW);
    this->BSP_SetLed(VTP_LED,LOW);
}
void CartStateSetting::SetVTKOAStateFlag(bool state)
{
    this->OAStateFlag = state;
    this->BSP_SetLed(SB_LED,LOW);
    this->BSP_SetLed(RC_LED,LOW);
    this->BSP_SetLed(VTP_LED,LOW);
}
void CartStateSetting::SetVTPOAStateFlag(bool state)
{
    this->OAStateFlag = state;
    this->BSP_SetLed(SB_LED,LOW);
    this->BSP_SetLed(RC_LED,LOW);
    this->BSP_SetLed(VTP_LED,LOW);
}


int8_t CartStateSetting::ModeChangeBaseJson(QString mode,int value)
{
#if(0)
    //打开文件
    //qDebug()<<this->JsonFile->fileName();
    if(!this->JsonFile->isOpen())
    {
        if(this->JsonFile->open(QIODevice::ReadWrite))
        {
            //qDebug()<<this->JsonFile->isReadable();
            //qDebug()<<this->JsonFile->isWritable();

            QByteArray data=this->JsonFile->readAll();
            //使用json文件对象加载字符串
            QJsonDocument doc=QJsonDocument::fromJson(data);

            //判断是否对象
            if(doc.isObject())
            {
                //把json文档转换为json对象
                QJsonObject obj=doc.object();
                if(obj.contains(mode))
                {
                    obj.take(mode);
                    obj.insert(mode,value);
                    doc.setObject(obj);

                    this->JsonFile->seek(0);
                    this->JsonFile->write(doc.toJson());
                    this->JsonFile->flush();
                    this->JsonFile->close();

                    //qDebug()<<"Mode Modify Successful!";
                }
            }
        }
        else
        {
            qDebug()<<"Open JsonFile Err";
        }
    }
#endif
    return 0;
}

CartStateSetting::~CartStateSetting()
{
    this->BSP_SetLed(SB_LED,LOW);
    this->BSP_SetLed(RC_LED,LOW);
    this->BSP_SetLed(VTP_LED,LOW);
    qDebug()<<"Exit:";
}
