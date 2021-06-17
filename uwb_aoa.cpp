#include "uwb_aoa.h"
#include <QtMath>

UWB_AOA::UWB_AOA()
{
    qDebug()<<"UWB_AOA Thread!";
#if(DETECT_TYPE == BT_TYPE)
    this->timer_1s = new QTimer;
    this->atSettingStep = AT_STEP_SCANSTOP;
    this->rssi = 0;
#endif

    this->UartName = PORT_NAME;
    this->AppUART = new QSerialPort;
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//        qDebug() << "Name : " << info.portName();
//        qDebug() << "Description : " << info.description();
//        qDebug() << "Manufacturer: " << info.manufacturer();
//        qDebug() << "Serial Number: " << info.serialNumber();
//        qDebug() << "System Location: " << info.systemLocation();
//        if(info.description().contains("Prolific"))
//        {
//            //this->ui_MainWindow->UartComboBox->addItem(info.portName());
//            this->UartName = info.systemLocation();

//            break;
//        }
//        //this->UartName = this->ui_MainWindow->UartComboBox->itemText(0);
//    }
    this->AppUART->setPortName(this->UartName);
    if(this->AppUART->open(QIODevice::ReadWrite))
    {
        printf("UWB Uart Open Ok!");
        this->AppUART->reset();
        this->AppUART->setBaudRate(QSerialPort::Baud115200);
        this->AppUART->setParity(QSerialPort::NoParity);
        this->AppUART->setDataBits(QSerialPort::Data8);
        this->AppUART->setStopBits(QSerialPort::OneStop);
        this->AppUART->setFlowControl(QSerialPort::NoFlowControl);

        connect(this->AppUART,&QSerialPort::readyRead,this,&UWB_AOA::ReadUARTSlot);
        connect(this,&UWB_AOA::startDataAnalysisSignal,this,&UWB_AOA::AnalysisDealSlot);
    }
    else
    {
        printf("UWB Uart Open Error!\n");
    }
#if(DETECT_TYPE == BT_TYPE)
    connect(this->timer_1s,&QTimer::timeout,this,&UWB_AOA::timer1sSlot);
    this->timer_1s->start(1000);
#endif
}

#define A   (double)(44.9696)
#define n   (1)
void UWB_AOA::ReadUARTSlot(void)
{
#if(DETECT_TYPE == BT_TYPE)
    if(this->AppUART->canReadLine())
    {
        QString RxInfo = this->AppUART->readLine();
//        qDebug()<<RxInfo;
        if(RxInfo.contains(DEVICE_NAME) && (this->atSettingStep == AT_STEP_NULL))
        {
            QStringList RxInfoList = RxInfo.split(" ");
            RxInfoList = RxInfoList.at(1).split("\t");
            RxInfo = RxInfoList.at(0);
            RxInfo.replace("RSSI:","");
            double iRssi = RxInfo.toInt();
            if(this->rssi == 0) this->rssi = iRssi;
            if(iRssi < 0)
            {
                this->rssi = 0.55*this->rssi + 0.45*iRssi;
                qDebug()<<this->rssi;
            }
        }
        else if(!RxInfo.contains("NAME:"))
        {
//            qDebug()<<"RX"<<RxInfo;
            if(RxInfo.contains("AT+SCANSTOP=OK"))
            {
                this->atSettingStep = AT_STEP_SCANINTERVAL;
            }
            else if(RxInfo.contains("SCANINTERVAL"))
            {
                this->atSettingStep = AT_STEP_SCANTIME;
            }
            else if(RxInfo.contains("SCANTIME"))
            {
                this->atSettingStep = AT_STEP_SETTYPE;
            }
            else if(RxInfo.contains("TYPE"))
            {
                this->atSettingStep = AT_STEP_SCANSTART;
            }
            else if(RxInfo.contains("SCANSTART"))
            {
                this->atSettingStep = AT_STEP_NULL;
            }
        }
    }
#else
    if(this->AppUART->bytesAvailable() >= PACKAGE_LEN)
    {
        QByteArray  data;
        data = this->AppUART->readAll();
        if((data.size() == PACKAGE_LEN) && data.endsWith(PACKAGE_TAIL) && data.startsWith(PACKAGE_HEAD))
        {
            //qDebug()<<data.toHex()<<hex<<this->CalcCRC16(data);
            this->startDataAnalysisSignal(data);
        }
        else
        {
            this->AppUART->clear();
        }
    }
#endif
}
#if(DETECT_TYPE == BT_TYPE)
//typedef enum
//{
//    AT_STEP_NULL= 0,
//    AT_STEP_SCANSTOP,
//    AT_STEP_SCANINTERVAL,
//    AT_STEP_SCANTIME,
//    AT_STEP_SETTYPE,
//    AT_STEP_SCANSTART,
//}SettingStep_e;

void UWB_AOA::timer1sSlot(void)
{
    switch(this->atSettingStep)
    {
    case AT_STEP_SCANSTOP:
        this->atScanStop();
        break;
    case AT_STEP_SCANINTERVAL:
        this->atScanInterval();
        break;
    case AT_STEP_SCANTIME:
        this->atScanTime();
        break;
    case AT_STEP_SETTYPE:
        this->atType2();
        break;
    case AT_STEP_SCANSTART:
        this->atScanStart();
        break;
    case AT_STEP_NULL:
        this->atRestart();
        this->timer_1s->stop();
    default:
        break;
    }

}
void UWB_AOA::atScanStop(void)
{
    QString str ="AT+SCANSTOP\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();
    qDebug()<<"Tx"<<str;
}
void    UWB_AOA::atTest(void)
{
    QString str ="AT+TEST\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();

}
void    UWB_AOA::atScanInterval(void)
{
    QString str ="AT+SCANINTERVAL=200\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();
    qDebug()<<"Tx"<<str;
}
void    UWB_AOA::atScanTime(void)
{
    QString str ="AT+SCANTIME=100\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();
    qDebug()<<"Tx"<<str;
}
void    UWB_AOA::atReset(void)
{
    QString str ="AT+RESET\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();
}
void    UWB_AOA::atRestart(void)
{
    QString str ="AT+RESTART\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();
}
void    UWB_AOA::atScanStart(void)
{
    QString str ="AT+SCANSTART\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();
    qDebug()<<"Tx"<<str;
}
void    UWB_AOA::atType2(void)
{
    QString str ="AT+TYPE=2\n";
    this->AppUART->write(str.toLatin1());
    this->AppUART->flush();
}
#endif

void UWB_AOA::AnalysisDealSlot(QByteArray data)
{
    uint16_t CalcCRC16 = this->CalcCRC16(data);
    uint16_t GotCRC16  = data.at(17)<<8 | data.at(16);
    if(CalcCRC16 == GotCRC16)
    {
//        qDebug()<<data.toHex()<<hex<<this->CalcCRC16(data);
        switch(data.at(2))
        {
        case 0x63:
        {
            AOA_Report_t *RxReport = (AOA_Report_t *)data.data();
//            qDebug()<<RxReport->dist*10<<RxReport->angle*180/1000.0/M_PI<<RxReport->quality;//un:mmss
            UWBInfo_t UWBInfo={0};
            UWBInfo.dist = RxReport->dist*10;
            UWBInfo.angle = RxReport->angle*180/1000.0/M_PI;
            UWBInfo.battery = RxReport->battery;
            UWBInfo.keys = RxReport->keys;
            UWBInfo.Tag_ID = RxReport->Tag_ID;
            UWBInfo.anchor_status = RxReport->anchor_status;
            UWBInfo.quality = RxReport->quality;
            this->UpdateInfoSignal(UWBInfo);
            //qDebug()<<UWBInfo.dist<<UWBInfo.angle;
            break;
        }

        default:
            break;
        }


    }
}

quint16 UWB_AOA::CalcCRC16(QByteArray data)
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    quint8 iIndex;
    for(quint16 i = 0;i<data.size() - 3;i++)
    {
        iIndex = ucCRCLo ^ (data.at(i));
        ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}

void UWB_AOA::run()
{

}
UWB_AOA::~UWB_AOA()
{
    delete this->AppUART;

    qDebug()<<"Quit UWB_AOA";
}
