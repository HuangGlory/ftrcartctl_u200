#ifndef CARTSTATESETTING_H
#define CARTSTATESETTING_H

#include <QObject>
#include <QDebug>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <QTimer>
#include <QTime>
#include <QThread>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QFile>

#include <PlatformParameter.h>

/************communication ctl define*********************/
#define SOP                     (quint8)(0x6b)

#define MCB_BLDC_PKT_SOP        (quint8)(0x02)

#define SEND_DATA_PER   (quint8)(31)
#define LOOP_PER        (quint8)(51)
#if(PLATFORM == PLATFORM_U250)
//BSP define
typedef enum
{
    LED1 = 0,
    SB_LED=LED1,
    LED2 = 1,
    RC_LED=LED2,
    LED3 = 2,
    VTP_LED=LED3,
    LED4 = 3,
    PNG_LED=LED4,
    LED5 = 4,
    LAMP_LED=LED5,
    LEDn
}LED_TypeDef;
#else
//BSP define
typedef enum
{
    LED1 = 0,
    SB_LED=LED1,
    LED2 = 1,
    RC_LED=LED2,
    LED3 = 2,
    PNG_LED=LED3,
    LED4 = 3,
    VTP_LED=LED4,
    LED5 = 4,
    LAMP_LED=LED5,
    LEDn
}LED_TypeDef;
#endif
typedef enum
{
    KEY1 = 0,
    SB_KEY=KEY1,
    KEY2 = 1,
    VTK_KEY=KEY2,
    KEY3 = 2,
    VTP_KEY=KEY3,
    KEY4 = 3,
    PNG_KEY=KEY4,
    KEYn
}KEY_TypeDef;

#define BSP_ERROR_NONE                    0
#define BSP_ERROR_NO_INIT                -1
#define BSP_ERROR_WRONG_PARAM            -2
#define BSP_ERROR_BUSY                   -3
#define BSP_ERROR_PERIPH_FAILURE         -4
#define BSP_ERROR_COMPONENT_FAILURE      -5
#define BSP_ERROR_UNKNOWN_FAILURE        -6
#define BSP_ERROR_UNKNOWN_COMPONENT      -7
#define BSP_ERROR_BUS_FAILURE            -8
#define BSP_ERROR_CLOCK_FAILURE          -9
#define BSP_ERROR_MSP_FAILURE            -10
#define BSP_ERROR_FEATURE_NOT_SUPPORTED  -11

#define PIN_MODE_BCM    (1)
#define PIN_MODE_WPI    (2)
#define PIN_MODE        (PIN_MODE_WPI)
/*************************************************************************
 * +-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT5 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 0 | ALT5 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 0 | OUT  | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |  OUT | 0 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | OUT  | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |  OUT | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+
 * ***********************************************************************/
#if(PIN_MODE == PIN_MODE_BCM)
    #define SB_LED_PIN  (16)
    #define VTK_LED_PIN  (26)
    #define VTP_LED_PIN  (6)
    #define PNG_LED_PIN  (22)

    #define LAMP_PIN     (21)

    #define SB_KEY_PIN  (20)
    #define VTK_KEY_PIN  (12)
    #define VTP_KEY_PIN  (13)
    #define PNG_KEY_PIN  (5)
#else//wPi
    #define SB_LED_PIN  (27)
    #define VTK_LED_PIN  (25)
    #define VTP_LED_PIN  (22)
    #define PNG_LED_PIN  (3)

    #define LAMP_PIN     (29)

    #define SB_KEY_PIN  (28)
    #define VTK_KEY_PIN  (26)
    #define VTP_KEY_PIN  (23)
    #define PNG_KEY_PIN  (21)
#endif

#define SLOW_BLINK_TIME (1500)
#define FAST_BLINK_TIME (300)

//#define LONG_LONG_PRESS_TIME (5000)
#define LONG_PRESS_TIME (1000)
#define SHORT_PRESS_TIME (200)

class CartStateSetting : public QObject
{
    Q_OBJECT
public:
    explicit CartStateSetting(QObject *parent = nullptr);
    CartStateSetting(QString saveFileName);
    ~CartStateSetting();
    int8_t BSP_LED_Init(LED_TypeDef Led);
    int8_t BSP_KEY_Init(KEY_TypeDef Key);
    int8_t BSP_GetKeyValue(KEY_TypeDef Key);
    void BSP_SetLed(LED_TypeDef Led,int value);
    void BSP_LedToggle(LED_TypeDef Led);
    void BSP_Init(void);

    static  void KeyISR(void);
    static  void emitKeyISRSignal(void);

    int8_t ModeChangeBaseJson(QString mode,int value);
protected:

signals:
    void KeyISRSignl(void);
    void BroadcastCartStateSignal(CartState_e);
    void PNGButtonToggleSignal();
    void SettingOAToggleSignal(CartState_e);
    void SetToPushInWorkSignal();
//    void TKeyClickedInVTKSignal();
public slots:
    void LoopTimerSlot(void);
    void LEDBlinkSpeedCtlTimerSlot(void);
    void KeyISRSlot(void);

    void SetCartStateExternal(CartState_e);

    void SetInConfigModeFlagSlot(bool state);
    void SetCameraReadyFlagSlot(bool state);
    void SetVTKInIdleFlag(bool state);
    void SetVTKOAStateFlag(bool state);
    void SetVTPOAStateFlag(bool state);
public:
    QString saveFileNamea;
    QTimer *LoopTimer;
    QTimer *LEDBlinkSpeedCtlTimer;
    QTime  *KeyHoldElapsedTime;
    QString WhichKey;

    QFile *JsonFile;

    CartState_e CurrentCartState;
    CartState_e PreCartState;

    int LED_PIN[LEDn] ={SB_LED_PIN,VTK_LED_PIN,VTP_LED_PIN,PNG_LED_PIN,LAMP_PIN};
    int KEY_PIN[KEYn] ={SB_KEY_PIN,VTK_KEY_PIN,VTP_KEY_PIN,PNG_KEY_PIN};

    bool InConfigFlag;
    bool CameraReadyFlag;
    bool VTKInIdleFlag;
    bool OAStateFlag;
};

#endif // CARTSTATESETTING_H
