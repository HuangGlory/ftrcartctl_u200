#include "ftr_ctr3speedctl.h"

void FTR_CTR3SpeedCtl::VTK_Exit()//CMD = 0xEE
{
    this->SendCMD(CMD_EXIT_VTK);
}
void FTR_CTR3SpeedCtl::VTK_Enter()//CMD = 0xED
{
    this->SendCMD(CMD_ENTER_VTK,this->SettingParameterFromJson.VTKCtlByte.CtlByte);
    this->VTKInfo.CtlByte = this->SettingParameterFromJson.VTKCtlByte.CtlByte;
    qDebug()<<"InitCtlByte:"<<hex<<this->SettingParameterFromJson.VTKCtlByte.CtlByte;
}
void FTR_CTR3SpeedCtl::VTK_RealTimeInfo()//CMD = 0xEC
{
    //this->SendCMD(CMD_VTK_INFO,this->VTKInfo.VTKAngle,(this->VTKInfo.VTKDist >> 8) & 0xFF,this->VTKInfo.VTKDist & 0xFF);

    QByteArray  ByteArray;    
#if(SERIAL_TYPE == SERIAL_TYPE_USB)
    ByteArray[0]=SOP_USB;
    ByteArray[1]=CMD_VTK_INFO;
    ByteArray[2]=0x00;
    ByteArray[3]=0x09;
    ByteArray[4]=this->VTKInfo.VTKAngle;
    ByteArray[5]=(this->VTKInfo.VTKDist >> 8) & 0xFF;
    ByteArray[6]=this->VTKInfo.VTKDist & 0xFF;
    ByteArray[7]=this->VTKInfo.CtlByte;

    if(this->VTKInfo.ToPushFlag)
    {
        ByteArray[7]=this->VTKInfo.CtlByte | 0x08;
    }
    else
    {
        ByteArray[7]=this->VTKInfo.CtlByte & (~0x08);
    }

//    if(this->VTKInfo.TKeyClickedFlag)
//    {
//        this->VTKInfo.TKeyClickedFlag = false;
//        ByteArray[7]=this->VTKInfo.CtlByte | 0x02;
//    }

//    qDebug("ctlByte:%x,%d",ByteArray.at(7),this->VTKInfo.ToPushFlag);
#if(PLATFORM == PLATFORM_R3)
    ByteArray[8]=(this->pose.pitch >> 8) & 0xFF;
    ByteArray[9]=this->pose.pitch & 0xFF;

    ByteArray[10]=(this->pose.roll >> 8) & 0xFF;
    ByteArray[11]=this->pose.roll & 0xFF;

    ByteArray[12]=((qint16)(this->pose.yaw + 0.5) >> 8) & 0xFF;
    ByteArray[13]=(qint16)(this->pose.yaw + 0.5) & 0xFF;
#else    
    ByteArray[8]=0x00;
    ByteArray[9]=0x00;

    ByteArray[10]=0x00;
    ByteArray[11]=0x00;

    ByteArray[12]=0x00;
    ByteArray[13]=0x00;
#endif

    ByteArray[14]=this->App_XOR(ByteArray);
    //qDebug()<<this->VTKInfo.VTKDist<<this->VTKInfo.VTKAngle;
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
    QThread::msleep(5);
}
