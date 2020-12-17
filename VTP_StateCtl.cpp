#include "ftr_ctr3speedctl.h"

void FTR_CTR3SpeedCtl::VTP_Exit()//CMD = 0x92
{
    this->SendCMD(CMD_EXIT_VTAPE);
}
void FTR_CTR3SpeedCtl::VTP_Enter()//CMD = 0x91
{
    QByteArray  ByteArray;
#if(SERIAL_TYPE == SERIAL_TYPE_USB)
    ByteArray[0]=SOP_USB;
    ByteArray[1]=CMD_ENTER_VTAPE;
    ByteArray[2]=0x00;
    ByteArray[3]=0x12;

    //minspeed
    ByteArray[4]=(this->CartModeEnterParameter.minSpeed >> 8) & 0xFF;
    ByteArray[5]=(this->CartModeEnterParameter.minSpeed & 0xFF);

    ByteArray[6]=(this->CartModeEnterParameter.maxSpeed >> 8) & 0xFF;
    ByteArray[7]=(this->CartModeEnterParameter.maxSpeed & 0xFF);

    ByteArray[8]= this->CartModeEnterParameter.VTPCtlByte.CtlByte;

    //diam
    quint32 LeftDiam = (quint32)(this->CartModeEnterParameter.LeftDiam*1000);
    quint32 RightDiam = (quint32)(this->CartModeEnterParameter.RightDiam*1000);
    ByteArray[9]=(LeftDiam >> 24) & 0xFF;
    ByteArray[10]=(LeftDiam >> 16) & 0xFF;
    ByteArray[11]=(LeftDiam >> 8) & 0xFF;
    ByteArray[12]=(LeftDiam & 0xFF);

    ByteArray[13]=(RightDiam >> 24) & 0xFF;
    ByteArray[14]=(RightDiam >> 16) & 0xFF;
    ByteArray[15]=(RightDiam >> 8) & 0xFF;
    ByteArray[16]=(RightDiam & 0xFF);

    if(this->inBuildMapModeFlag)
    {
        ByteArray[17]=ACTION_UTURN_GO;
    }
    else
    {
        ByteArray[17]=this->CartModeEnterParameter.actionEndTape;
    }

    ByteArray[18]= 0x00;
    ByteArray[19]= 0x00;
    ByteArray[20]= 0x00;
    ByteArray[21]= 0x00;

    ByteArray[22]=this->App_XOR(ByteArray);

//    qDebug("CtlByte:0x%x",this->CartModeEnterParameter.VTPCtlByte.CtlByte);
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
}
void FTR_CTR3SpeedCtl::VTP_RealTimeInfo()//CMD = 0x90:usb //CMD = 0xAA:ttl
{

    QByteArray  ByteArray;
#if(SERIAL_TYPE == SERIAL_TYPE_USB)
    ByteArray[0]=SOP_USB;
    ByteArray[1]=CMD_VTAPE_INFO;
    ByteArray[2]=0x00;
    ByteArray[3]=0x19;

    ByteArray[4]=(this->VTPInfo.PointOnTape[0]>>8) & 0xFF;
    ByteArray[5]=(this->VTPInfo.PointOnTape[0]);

    ByteArray[6]=(this->VTPInfo.PointOnTape[1]>>8) & 0xFF;
    ByteArray[7]=(this->VTPInfo.PointOnTape[1]);

    ByteArray[8]=(this->VTPInfo.PointOnTape[2]>>8) & 0xFF;
    ByteArray[9]=(this->VTPInfo.PointOnTape[2]);

    ByteArray[10]=(this->VTPInfo.PointOnTape[3]>>8) & 0xFF;
    ByteArray[11]=(this->VTPInfo.PointOnTape[3]);

    ByteArray[12]=(this->VTPInfo.PointOnTape[4]>>8) & 0xFF;
    ByteArray[13]=(this->VTPInfo.PointOnTape[4]);

    ByteArray[14]=(this->VTPInfo.PointOnTape[5]>>8) & 0xFF;
    ByteArray[15]=(this->VTPInfo.PointOnTape[5]);

    ByteArray[16]=(this->VTPInfo.PointOnTape[6]>>8) & 0xFF;
    ByteArray[17]=(this->VTPInfo.PointOnTape[6]);


    ByteArray[18]=this->VTPInfo.SpeedCtl;//1：加速，-1：減速，0：沒三角形

    ByteArray[19]=this->VTPInfo.CtlByte;

    if(this->VTPInfo.ToPushFlag)
    {
        ByteArray[19]=this->VTPInfo.CtlByte | 0x02;
    }
    else
    {
        ByteArray[19]=this->VTPInfo.CtlByte & (~0x02);
    }
//    qDebug("ctlByte:%x,%d",ByteArray.at(19),this->VTPInfo.ToPushFlag);

    ByteArray[20]=this->VTPInfo.setAction;
    ByteArray[21]=this->VTPInfo.StationName;

    ByteArray[22]=(this->VTPInfo.MaxSpeed>>8) & 0xFF;
    ByteArray[23]=this->VTPInfo.MaxSpeed & 0xFF;

    ByteArray[24]=this->ArcInfo.ArcLocation;

    ByteArray[25]=(this->VTPInfo.ToStationDist>>8) & 0xFF;
    ByteArray[26]=this->VTPInfo.ToStationDist & 0xFF;

    if(this->stationInfoUpdateFlag)
    {
        this->stationInfoUpdateFlag = false;
        quint16 detalODO = (quint16)(this->timeElapsedGotMarkToSendInfo->elapsed()*0.25 + 0.5);
        if(this->timeElapsedGotMarkToSendInfo->elapsed() < 300)
        {
            this->remainDistToStation -= detalODO;
        }

        qDebug()<<"USI:"<<this->timeElapsedGotMarkToSendInfo->elapsed()<<detalODO;

        this->remainDistToStationTxToEBox = this->remainDistToStation;
    }

    if(this->usedDefaultFixedDistFlag)
    {
        ByteArray[27]=(this->defaultFixedDist>>8) & 0xFF;
        ByteArray[28]=this->defaultFixedDist & 0xFF;
    }
    else
    {
        ByteArray[27]=(this->remainDistToStationTxToEBox>>8) & 0xFF;
        ByteArray[28]=this->remainDistToStationTxToEBox & 0xFF;
    }

    ByteArray[29]=this->App_XOR(ByteArray);

//    qDebug()<<"RTX:"<<this->VTPInfo.StationName<<this->VTPInfo.setAction;

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
    //QThread::msleep(5);
}
void FTR_CTR3SpeedCtl::VTP_SpeedUp()//CMD = 0x8F
{
    this->SendCMD(CMD_VTAPE_SPEED_UP);
}
void FTR_CTR3SpeedCtl::VTP_SpeedDown()//CMD = 0x8E
{
    this->SendCMD(CMD_VTAPE_SPEED_DOWN);
}
void FTR_CTR3SpeedCtl::VTP_SetActionOnState(ActionOnCrossType_e action)//CMD = 0x8D
{
    this->SendCMD(CMD_VTAPE_SETTING_ACTION,action);
}

void FTR_CTR3SpeedCtl:: VTP_UpdateAction(ActionOnCrossType_e setAction)
{
    this->VTPInfo.setAction = setAction;
}

void FTR_CTR3SpeedCtl::VTP_InitParameter()
{
    this->CartModeEnterParameter.minSpeed               = this->SettingParameterFromJson.minSpeed;//MIN_SPEED_4VTP;
    this->CartModeEnterParameter.maxSpeed               = this->SettingParameterFromJson.maxSpeed;//MAX_SPEED_4VTP;

    this->VTPInfo.MaxSpeed                              = this->SettingParameterFromJson.maxSpeed;
    this->VTPInfo.CtlByte                               = this->SettingParameterFromJson.VTPCtlByte.CtlByte;

    this->CartModeEnterParameter.VTPCtlByte.CtlByteFlag.TOFOAOn_Off     = this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.TOFOAOn_Off;//this->TOF_OAFlag;//false;//true;
    this->CartModeEnterParameter.VTPCtlByte.CtlByteFlag.USOAOn_Off      = this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.USOAOn_Off;//this->US_OAFlag;//true;
    this->CartModeEnterParameter.VTPCtlByte.CtlByteFlag.UturnDir        = this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.UturnDir;//this->UturnDirFlag;
    this->CartModeEnterParameter.VTPCtlByte.CtlByteFlag.OASetOnStation  = this->SettingParameterFromJson.VTPCtlByte.CtlByteFlag.OASetOnStation;

    this->CartModeEnterParameter.LeftDiam               = this->SettingParameterFromJson.LeftDiam;//this->LeftWheelDiameter;//(168.8850);
    this->CartModeEnterParameter.RightDiam              = this->SettingParameterFromJson.RightDiam;//this->RightWheelDiameter;//(168.9614);

    this->CartModeEnterParameter.actionEndTape                 = this->SettingParameterFromJson.actionEndTape;//DEFAULT_ACTION;
    this->VTP_UpdateAction(this->SettingParameterFromJson.actionOnMark);
}
