#include "imu.h"
#include <QtMath>
#include <QFile>

imu::imu(QObject *parent) : QObject(parent)
{
    this->dt            = new QTime;
    this->Time2ReadData = new QTimer;
    this->pose.yaw      = 0;
    this->pose.pitch    = 0;
    this->pose.roll     = 0;

    this->imuDeviceInit();

    connect(this->Time2ReadData,SIGNAL(timeout()),this,SLOT(Time2ReadDataSlot()));
}


void imu::imuDeviceInit()
{
    wiringPiSetup () ;
    this->fd = wiringPiI2CSetup(SLAVE_ADDRESS);
    if(this->fd >=0)
    {
        qDebug("Init ok ID:0x%x",this->getID(WHO_AM_I));

        volatile uint8_t data[7]={0x00,0x07,0x06,GFS_SEL_VALUE,AFS_SEL_VALUE,0x00,0x02};

        wiringPiI2CWriteReg8(fd,PWR_MGMT_1,data[0]); // 开启温度检测 关闭休眠
        wiringPiI2CWriteReg8(fd,SMPLRT_DIV, data[1]);
        wiringPiI2CWriteReg8(fd,CONFIG, data[2]);
        //wiringPiI2CWriteReg8(fd,GYRO_CONFIG, data[3]);//0x18);
        //wiringPiI2CWriteReg8(fd,ACCEL_CONFIG, data[4]);//0x01);

        //report version to a file
        {
            QFile file;
            file.setFileName("/home/pi/ftrCartCtl/bias");
            if(file.exists())//load bias
            {
                this->LoadBias();
            }
            else//calc the bias and board angle
            {
                this->CalcBias();
            }
        }

        this->Time2ReadData->start(50);
    }
    else
    {
        qDebug()<<"Init fail";
    }
}
qint16  imu::getData(int reg_address)
{
    return (qint16)((((qint16)wiringPiI2CReadReg8(fd,reg_address)) << 8) + ((qint16)wiringPiI2CReadReg8(fd,reg_address + 1)));
}

quint8  imu::getID(int reg_address)
{
    return (wiringPiI2CReadReg8(fd,reg_address));
}

void imu::Time2ReadDataSlot()
{
    this->RawData.acc.x = this->getData(ACCEL_XOUT_H)*ACC_SENSITIVITY;
    this->RawData.acc.y = this->getData(ACCEL_YOUT_H)*ACC_SENSITIVITY;
    this->RawData.acc.z = this->getData(ACCEL_ZOUT_H)*ACC_SENSITIVITY;

    this->RawData.gyro.x = this->getData(GYRO_XOUT_H)*GYRO_SENSITIVITY;
    this->RawData.gyro.y = this->getData(GYRO_YOUT_H)*GYRO_SENSITIVITY;
    this->RawData.gyro.z = this->getData(GYRO_ZOUT_H)*GYRO_SENSITIVITY;

    if(this->CalcBiasFlag)//get bias
    {
        this->Bias.acc.x += this->RawData.acc.x;
        this->Bias.acc.y += this->RawData.acc.y;
        this->Bias.acc.z += this->RawData.acc.z;

        this->Bias.gyro.x += this->RawData.gyro.x;
        this->Bias.gyro.y += this->RawData.gyro.y;
        this->Bias.gyro.z += this->RawData.gyro.z;

        //qDebug()<<"R:"<<this->RawData.gyro.z;
        if(++this->Cnt4BiaseCalc >= 40)
        {
            this->Bias.acc.x /= 40;
            this->Bias.acc.y /= 40;
            this->Bias.acc.z /= 40;

            this->Bias.gyro.x /= 40;
            this->Bias.gyro.y /= 40;
            this->Bias.gyro.z /= 40;

            this->Cnt4BiaseCalc = 0;
            this->CalcBiasFlag = false;

            double norm = sqrt(this->Bias.acc.x*this->Bias.acc.x + this->Bias.acc.y*this->Bias.acc.y + this->Bias.acc.z*this->Bias.acc.z);


            qDebug()<<norm;

            double roll = (atan2(this->Bias.acc.x,this->Bias.acc.z)*57.3);
            double pitch  = (atan2(this->Bias.acc.y,this->Bias.acc.z)*57.3);

            //calc real bias

            this->Bias.acc.x = this->RawData.acc.x - norm*sin(pitch/57.3);
            this->Bias.acc.y = this->RawData.acc.y - norm*sin(roll/57.3);
            this->Bias.acc.z = this->RawData.acc.z - norm*cos(roll/57.3);

            this->BoardRoll = roll;
            this->BoardPitch= pitch;

            QFile file;
            file.setFileName("/home/pi/ftrCartCtl/bias");
            if(file.exists()) file.remove();
            if(file.open(QIODevice::WriteOnly))
            {
                QString BiasStr = QString("%1,%2,%3,%4,%5,%6,%7,%8").arg(pitch).arg(roll).arg(this->Bias.acc.x)
                                .arg(this->Bias.acc.y).arg(this->Bias.acc.z).arg(this->Bias.gyro.x).arg(this->Bias.gyro.y).arg(this->Bias.gyro.z);
                file.write(BiasStr.toLocal8Bit().data());
                file.flush();
                file.close();
                qDebug()<<BiasStr;
            }
        }
    }
    else
    {
//        this->RawData.acc.x -= this->Bias.acc.x;
//        this->RawData.acc.y -= this->Bias.acc.y;
//        this->RawData.acc.z -= this->Bias.acc.z;

        this->RawData.gyro.x -= this->Bias.gyro.x;
        this->RawData.gyro.y -= this->Bias.gyro.y;
        this->RawData.gyro.z -= this->Bias.gyro.z;

        if(abs(this->RawData.gyro.z) < 0.08) this->RawData.gyro.z = 0;

        double dt = this->dt->elapsed()/1000.0f;
        this->pose.yaw += dt*this->RawData.gyro.z/cos(this->BoardPitch/57.3);
        //qDebug()<<this->RawData.gyro.z;

        this->dt->restart();

        this->pose.norm = sqrt(this->RawData.acc.x*this->RawData.acc.x + this->RawData.acc.y*this->RawData.acc.y + this->RawData.acc.z*this->RawData.acc.z);

        this->pose.roll = (qint16)(atan2(this->RawData.acc.x,this->RawData.acc.z)*57.3) - this->BoardRoll;
        this->pose.pitch  = (qint16)(atan2(this->RawData.acc.y,this->RawData.acc.z)*57.3) - this->BoardPitch;
        //qDebug()<<norm<<this->pose.pitch<<this->pose.roll<<this->pose.yaw;

        this->UpdateInfo();
    }
}
void    imu::LoadBias(void)
{
    QFile file;
    file.setFileName("/home/pi/ftrCartCtl/bias");
    if(file.open(QIODevice::ReadOnly))
    {
        QString BiasStr = file.readLine();
        //qDebug()<<"LoadBias:"<<BiasStr;
        QStringList BiasStrList = BiasStr.split(",", QString::SkipEmptyParts);
        if(BiasStrList.size() == 8)
        {
            bool convertResult = false;

            /**************board bias***************/
            double  convertValue   = BiasStrList.at(0).toDouble(&convertResult);
            if(convertResult)   this->BoardPitch = convertValue;

            convertValue   = BiasStrList.at(1).toDouble(&convertResult);
            if(convertResult)   this->BoardRoll = convertValue;


            /*************acc bias******************/
            convertValue   = BiasStrList.at(2).toDouble(&convertResult);
            if(convertResult)   this->Bias.acc.x = convertValue;

            convertValue   = BiasStrList.at(3).toDouble(&convertResult);
            if(convertResult)   this->Bias.acc.y = convertValue;

            convertValue   = BiasStrList.at(4).toDouble(&convertResult);
            if(convertResult)   this->Bias.acc.z = convertValue;

            /***************gyro bias***************/
            convertValue   = BiasStrList.at(5).toDouble(&convertResult);
            if(convertResult)   this->Bias.gyro.x = convertValue;

            convertValue   = BiasStrList.at(6).toDouble(&convertResult);
            if(convertResult)   this->Bias.gyro.y = convertValue;

            convertValue   = BiasStrList.at(7).toDouble(&convertResult);
            if(convertResult)   this->Bias.gyro.z = convertValue;

            QString BiasStr = QString("%1,%2,%3,%4,%5,%6,%7,%8").arg(this->BoardPitch).arg(this->BoardRoll).arg(this->Bias.acc.x)
                            .arg(this->Bias.acc.y).arg(this->Bias.acc.z).arg(this->Bias.gyro.x).arg(this->Bias.gyro.y).arg(this->Bias.gyro.z);
            qDebug()<<"LoadBias:"<<BiasStr;

        }
        else
        {
            this->CalcBias();
        }
    }
}
void    imu::CalcBias(void)
{
    this->CalcBiasFlag = true;
    this->Bias.acc.x = 0;
    this->Bias.acc.y = 0;
    this->Bias.acc.z = 0;

    this->Bias.gyro.x = 0;
    this->Bias.gyro.y = 0;
    this->Bias.gyro.z = 0;

    this->Cnt4BiaseCalc = 0;
}

void    imu::UpdateInfo(void)
{
    emit UpdateInfoSignal(this->pose);
}

void    imu::ClearYaw(void)
{
    this->pose.yaw = 0;
    //qDebug()<<"ClearYaw:";
}
