#ifndef IMU_H
#define IMU_H

#include <QObject>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <PlatformParameter.h>

typedef struct {
    union {
        struct {
            double x;
            double y;
            double z;
        } acc;
        double acc_array[3];
    };
    union {
        struct {
            double x;
            double y;
            double z;
        } gyro;
        double gyro_array[3];
    };
} imu_output;

typedef enum {
    INVENSENSE_ACC_2G,
    INVENSENSE_ACC_4G,
    INVENSENSE_ACC_8G,
    INVENSENSE_ACC_16G,
} invensense_acc_range;

typedef enum {
    INVENSENSE_GYRO_250,
    INVENSENSE_GYRO_500,
    INVENSENSE_GYRO_1000,
    INVENSENSE_GYRO_2000,
} invensense_gyro_range;

#define GFS_SEL_VALUE								(uint8_t)(0<<3)	//(0<<3) 250deg/s (1<<3) 500deg/s (2<<3) 1000deg/s (3<<3) 2000deg/s
#define GYRO_SENSITIVITY							(double)(0.007633587786260)//(double)(0.0001332312383375742)//(double)(0.000007633587786260)//(0.007633587786260)/1000 case time	//0.007633587786260 = 1/131;0.015267175572519=1/65.5;0.030487804878049=1/32.8;0.060975609756098 = 1/16.4;

#define AFS_SEL_VALUE								(uint8_t)(0<<3)  //(0<<3) 2G (1<<3) 4G (2<<3) 8G (3<<3) 16G
#define ACC_SENSITIVITY								(double)(0.00006103515625)	//16384;8192;4096;2048;LSB/g 1/16384


#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

#define	SLAVE_ADDRESS	(0x68)


class imu : public QObject
{
    Q_OBJECT
public:
    explicit imu(QObject *parent = nullptr);

    void imuDeviceInit();
    qint16  getData(int reg_address);
    quint8  getID(int reg_address);
    void    CalcBias(void);
    void    LoadBias(void);

signals:
    void UpdateInfoSignal(Pose_t);

public slots:
    void Time2ReadDataSlot();

private:
    void UpdateInfo(void);

public:
    void ClearYaw();

private:
    int fd;

    QTime           *dt;
    QTimer          *Time2ReadData;
    imu_output      RawData;
    imu_output      Bias;
    double          BoardPitch;
    double          BoardRoll;

    quint8          Cnt4BiaseCalc;
    Pose_t          pose;

    bool            CalcBiasFlag;
};

#endif // IMU_H
