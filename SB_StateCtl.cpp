#include "ftr_ctr3speedctl.h"

void FTR_CTR3SpeedCtl::SB_Enter()//CMD = 0x12
{
//    this->SendCMD(CMD_USER_ENTER_SB);
#if(1)
    QByteArray  ByteArray;
    //ByteArray.resize(13);

    ByteArray[0]=SOP_USB;
    ByteArray[1]=CMD_USER_ENTER_SB;
    ByteArray[2]=0x00;
    ByteArray[3]=0x08;

    //diam
    quint32 LeftDiam = (quint32)(this->LeftWheelDiam*1000);
    quint32 RightDiam = (quint32)(this->RightWheelDiam*1000);
    ByteArray[4]=(LeftDiam >> 24) & 0xFF;
    ByteArray[5]=(LeftDiam >> 16) & 0xFF;
    ByteArray[6]=(LeftDiam >> 8) & 0xFF;
    ByteArray[7]=(LeftDiam & 0xFF);

    ByteArray[8]=(RightDiam >> 24) & 0xFF;
    ByteArray[9]=(RightDiam >> 16) & 0xFF;
    ByteArray[10]=(RightDiam >> 8) & 0xFF;
    ByteArray[11]=(RightDiam & 0xFF);

    ByteArray[12]=this->App_XOR(ByteArray);

    this->AppUART->write(ByteArray);
    this->AppUART->flush();

    if(this->ShowLogFlag.ShowMixLogFlag) qDebug()<<"TX:"<<ByteArray.toHex();
#endif

    this->VTKInfo.VTKDist = 0;
    this->VTKInfo.VTKAngle= 0;

    for(quint8 i=0;i<7;i++) this->VTPInfo.PointOnTape[i] = 0x00;
    this->VTPInfo.StationName = 0;
}

void FTR_CTR3SpeedCtl::SB_RealTimeInfo()
{
    QByteArray  ByteArray;
#if(SERIAL_TYPE == SERIAL_TYPE_USB)
    //ByteArray.resize(14);
    ByteArray[0]=SOP_USB;
    ByteArray[1]=CMD_POSE_INFO;
    ByteArray[2]=0x00;
    ByteArray[3]=0x09;
    ByteArray[4]=0x00;
    ByteArray[5]=0x00;
    ByteArray[6]=0x00;

    #if(PLATFORM == PLATFORM_R3)
        ByteArray[7]=(this->pose.pitch >> 8) & 0xFF;
        ByteArray[8]=this->pose.pitch & 0xFF;

        ByteArray[9]=(this->pose.roll >> 8) & 0xFF;
        ByteArray[10]=this->pose.roll & 0xFF;

        ByteArray[11]=((qint16)(this->pose.yaw + 0.5) >> 8) & 0xFF;
        ByteArray[12]=(qint16)(this->pose.yaw + 0.5) & 0xFF;
    #else
        ByteArray[7]=0x00;
        ByteArray[8]=0x00;

        ByteArray[9]=0x00;
        ByteArray[10]=0x00;

        ByteArray[11]=0x00;
        ByteArray[12]=0x00;
    #endif

    ByteArray[13]=this->App_XOR(ByteArray);
#endif

    this->AppUART->write(ByteArray);
    this->AppUART->flush();
}
