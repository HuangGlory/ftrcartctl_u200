#ifndef PLATFORMPARAMETER_H
#define PLATFORMPARAMETER_H

#include <QObject>
#include <motorctrlinfo.h>

//function define
#define RC_REDIRECT						(0)
#define SERIAL_BYPASS                   (1)
#define BLUETOOTH_SERIAL_USED           (0)

#define SERIAL_TYPE_USB                 (1)
#define SERIAL_TYPE_TTL                 (2)
#define SERIAL_TYPE                     (SERIAL_TYPE_USB)
#define TTL_UART_NAME                   tr("/dev/ttyAMA0")//tr("/dev/ttyUSB0")//tr("/dev/ttyAMA0")//ttyS0

#define PLATFORM_R3                     (1)
#define PLATFORM_U250                   (2)
//#define PLATFORM                        (PLATFORM_R3)
#define PLATFORM                        (PLATFORM_U250)

#if(PLATFORM == PLATFORM_U250)
    #define BAT_MAX_VOLTAGE         (43.0)
    #define BAT_MIN_VOLTAGE         (34.0)
    #define TAPE_ANGLE_LOST_VALUE 	(int8_t)(127)


    #define MOTOR_MOTOR_DIST	  	(520)//(680)//(560)//(700.0)//(520.0)   //cart width between center of tires
    #define ANGLE_ADJUST_FACTOR		(double)(0.9)

    #define STATETK_ULTRASONIC_FIXEDSIDE_DISTANCE   (400)//(390)//(420)//(430)     //sensor width between center
    #define STK_HANDSET_HEIGHT    	(950.0)
    #define STK_SENSOR_HEIGHT    	(240.0)

    //define for tape trace
    #define MOTOR_SHAFT_TO_FW_PANEL_DIST (450)//un:mm
    #define FW_PANNEL_TO_IMAGE_DIST		 (350)//un:mm
    //#define MOTOR_SHAFT_TO_BW_PANEL_DIST (610)//un:mm
    //pix 640*480(驴铆*鲁陇)->(MOTOR_MOTOR_DIST + 370) * 1000,un:mm

    //#define MOTOR_SHAFT_TO_BW_PANEL_DIST (610)//un:mm
    //pix 640*480(脗驴脙颅*脗鲁脗陇)->(MOTOR_MOTOR_DIST + 370) * 1000,un:mm
    #define FW_DIST_PER_PIX_XAXIS			(double)(3.084337349397591)	//640/(MOTOR_MOTOR_DIST + 370)*4
    #define FW_DIST_PER_PIX_YAXIS			(double)(2.083333333333334)//1000/480

    #define BW_DIST_PER_PIX_XAXIS			(double)(2.313253012048193)	//480/(MOTOR_MOTOR_DIST + 370)*4
    #define BW_DIST_PER_PIX_YAXIS			(double)(1.5625)//1000/640

    //1111.333333333334   990.5   819.666666666667   569.666666666667   411.333333333333   390.5   211.333333333333
    //2013.333333333334   1892.500000000000   1721.666666666667   1471.666666666667   1313.333333333334   1292.500000000000   1113.333333333333
    #define FW_Y_AXIS0					(double)(1733.333333333333)	//((MAX_FW_Y - 32)*FW_DIST_PER_PIX_YAXIS + FW_PANNEL_TO_IMAGE_DIST + MOTOR_SHAFT_TO_FW_PANEL_DIST)//脙鈥澝偮睹偮睹冣??
    #define FW_Y_AXIS1					(double)(1612.5)			//((MAX_FW_Y - 90)*FW_DIST_PER_PIX_YAXIS + FW_PANNEL_TO_IMAGE_DIST + MOTOR_SHAFT_TO_FW_PANEL_DIST)
    #define FW_Y_AXIS2					(double)(1441.666666666667)	//((MAX_FW_Y - 172)*FW_DIST_PER_PIX_YAXIS + FW_PANNEL_TO_IMAGE_DIST + MOTOR_SHAFT_TO_FW_PANEL_DIST)
    #define FW_Y_AXIS3					(double)(1191.666666666667)	//((MAX_FW_Y - 292)*FW_DIST_PER_PIX_YAXIS + FW_PANNEL_TO_IMAGE_DIST + MOTOR_SHAFT_TO_FW_PANEL_DIST)
    #define FW_Y_AXIS4					(double)(1033.333333333333)	//((MAX_FW_Y - 368)*FW_DIST_PER_PIX_YAXIS + FW_PANNEL_TO_IMAGE_DIST + MOTOR_SHAFT_TO_FW_PANEL_DIST)
    #define FW_Y_AXIS5					(double)(1012.500000000000)				//((MAX_FW_Y - 378)*FW_DIST_PER_PIX_YAXIS + FW_PANNEL_TO_IMAGE_DIST + MOTOR_SHAFT_TO_FW_PANEL_DIST)
    #define FW_Y_AXIS6					(double)(833.333333333333)	//((MAX_FW_Y - 464)*FW_DIST_PER_PIX_YAXIS + FW_PANNEL_TO_IMAGE_DIST + MOTOR_SHAFT_TO_FW_PANEL_DIST)

    //1810.578313253012   1560.746987951807   1190.626506024096   952.361445783132   922.289156626506   656.265060240964
    //#define BW_Y_AXIS1					(double)(1810.578313253012)
    //#define BW_Y_AXIS2					(double)(1560.746987951807)
    //#define BW_Y_AXIS3					(double)(1190.626506024096)
    //#define BW_Y_AXIS4					(double)(952.361445783132)			//((MAX_BW_Y - 368)*BW_DIST_PER_PIX_XAXIS + MOTOR_SHAFT_TO_BW_PANEL_DIST)
    //#define BW_Y_AXIS5					(double)(922.289156626506)			//((MAX_BW_Y - 378)*BW_DIST_PER_PIX_XAXIS + MOTOR_SHAFT_TO_BW_PANEL_DIST)
    //#define BW_Y_AXIS6					(double)(656.265060240964)			//((MAX_BW_Y - 464)*BW_DIST_PER_PIX_XAXIS + MOTOR_SHAFT_TO_BW_PANEL_DIST)


    #define Y_AXIS7						(double)(0)					// MOTOR_SHAFT

#endif

//Rx information Order
typedef enum
{
    RX_INFO_ORDER_CART_MODE				= 0,//车子状态
    RX_INFO_ORDER_MOTOR_STATE           = 1,
    RX_INFO_ORDER_LEFT_REAL_SPEED       = 2,
    RX_INFO_ORDER_RIGHT_REAL_SPEED      = 3,
    RX_INFO_ORDER_LEFT_TARGET_SPEED     = 4,
    RX_INFO_ORDER_RIGHT_TARGET_SPEED    = 5,

    RX_INFO_ORDER_LINEAR_SPEED          = 6,
    RX_INFO_ORDER_ANGULAR_SPEED         = 7,
    RX_INFO_ORDER_LEFT_TEMP             = 8,
    RX_INFO_ORDER_RIGHT_TEMP            = 9,
    RX_INFO_ORDER_BAT_VOL               = 10,

    RX_INFO_ORDER_VTK_DIST              = 11,
    RX_INFO_ORDER_VTK_ANGLE             = 12,
    RX_INFO_ORDER_PNG                   = 13,
    //RX_INFO_ORDER_VTK_INIDLE            = 14,
    RX_INFO_ORDER_MULTI_FUN             = 14,

    RX_INFO_ORDER_TOF_OA_MIN_DIST       = 15,
    RX_INFO_ORDER_US_OA_MIN_DIST        = 16,
    RX_INFO_ORDER_ODO                   = 17,
    RX_INFO_ORDER_CURRENT_ACTION        = 18,

    RX_INFO_ORDER_HS_WANT_2_STATE       = 19,

    RX_INFO_ORDER_ROLL                  = 20,
    RX_INFO_ORDER_PITCH                 = 21,
    RX_INFO_ORDER_YAW                   = 22,

    RX_INFO_LEN
}RxInfo_e;

//parameter define
#define MOTOR_MOTOR_DIST                (500)

#define PI  (3.141592653589793)
#define RATIO_ANGLE2DEGREE              (57.29583) //180.0/M_PI
#define RATIO_DEGREE2ANGLE              (0.01745)  //M_PI/180.0
#define ANGLE_RADIAN2DEGREE(angle_radian)    ((angle_radian) * RATIO_ANGLE2DEGREE)
#define ANGLE_DEGREE2RADIAN(angle_degree)    ((angle_degree) * RATIO_DEGREE2ANGLE)

/*******wheel define**************************************/
#if(BIG_TORQUE_MOTOR_BE_USED)
    #define WHEEL_SIZE                      (double)(6.3)
    //160 170   208   259   310   411
    #define WHEEL_DIAM                      (uint32_t)(160000)           //mm 25.4*WHEEL_SIZE
    #define SPEED_FACTOR_6_3                (double)(61.3592/SPEED_READ_PER)       //(PI*WHEEL_DIAM*1000/(2048*4))/SPEED_READ_PER
    //0.1428    0.1904    0.2380    0.2856    0.3808 (1.05)
    #define ODO_FACTOR_6_3                  (double)(0.0613592)//(0.1360)*1.0956

    //#define SPEED_FACTOR                    (double)(SPEED_FACTOR_6_3)
    #define ODO_FACTOR                      (double)(ODO_FACTOR_6_3)
    #define ENCODER_CNT						(8192)//(3520)
#else
    //#define PI                              (double)(3.1415926)
    #define WHEEL_SIZE                      (double)(6.5)
    //170   208   259   310   411
    #define WHEEL_DIAM                      (double)(168000)           //mm 25.4*WHEEL_SIZE
    //151.7246  185.6396  231.1570  276.6744  366.8166
    #define SPEED_FACTOR_6_5                (double)(151.7246/SPEED_READ_PER)       //(PI*WHEEL_DIAM*1000/(880*4))/SPEED_READ_PER
    #define SPEED_FACTOR_8                  (double)(185.6396/SPEED_READ_PER)       //(PI*WHEEL_DIAM*1000/(880*4))/SPEED_READ_PER
    #define SPEED_FACTOR_10                 (double)(231.1570/SPEED_READ_PER)       //(PI*WHEEL_DIAM*1000/(880*4))/SPEED_READ_PER
    #define SPEED_FACTOR_12                 (double)(276.6744/SPEED_READ_PER)       //(PI*WHEEL_DIAM*1000/(880*4))/SPEED_READ_PER
    #define SPEED_FACTOR_16                 (double)(366.8166/SPEED_READ_PER)       //(PI*WHEEL_DIAM*1000/(880*4))/SPEED_READ_PER
    //0.1428    0.1904    0.2380    0.2856    0.3808 (1.05)
    #define ODO_FACTOR_6_5                  (double)(0.1517246)//(0.1360)*1.0956
    #define ODO_FACTOR_8                    (double)(0.1856396)//(0.1814)
    #define ODO_FACTOR_10                   (double)(0.2311570)//(0.2267)
    #define ODO_FACTOR_12                   (double)(0.2766744)//(0.2720)
    #define ODO_FACTOR_16                   (double)(0.3668166)//(0.3627)

    //#define SPEED_FACTOR                    (SPEED_FACTOR_6_5)
    #define ODO_FACTOR                      (ODO_FACTOR_6_5)
    #define ENCODER_CNT						(3520)
#endif
/*********************************************************/

//VTP
#define MIN_SPEED_4VTP                      (quint8)(250)
#define MAX_SPEED_4VTP                      (quint16)(500)//3kmph
#define DEFAULT_ACTION                      (ACTION_UTURN_GO)

#define FunctionJoystickUsedBit                    	(uint16_t)(0x0001)//BIT(0)
#define ObstBeDetectedBy2DBit                  		(uint16_t)(0x0002)//BIT(1)
#define ObstBeDetectedByUSBit                      	(uint16_t)(0x0004)//BIT(2)
#define OAEnableStateBit							(uint16_t)(0x0008)//BIT(3)
#define HSEnableStateBit							(uint16_t)(0x0010)//BIT(4)
#define CartStateStopOrNGoBit						(uint16_t)(0x0020)//BIT(5)
#define GotPauseButtonBit						    (uint16_t)(0x0040)//BIT(6)
#define MotorReleaseStateBit						(uint16_t)(0x0080)//BIT(7)

#define TKInIdleStateBit                            (uint16_t)(0x0100)//BIT(8)
#define HSBeUsedBit                                 (uint16_t)(0x0200)//BIT(9)
#define SpeedUpAndDownBit                           (uint16_t)(0x0400)//BIT(10)
#define StartActionBit                              (uint16_t)(0x0800)//BIT(11)
#define ClearYawBit                                 (uint16_t)(0x1000)//BIT(12)
#define InPauseStateBit                             (uint16_t)(0x2000)//BIT(13)
#define InArcTurningBit                             (uint16_t)(0x4000)//BIT(14)
#define InODOCaliStateBit                           (uint16_t)(0x8000)//BIT(15)

typedef struct{
    double norm;
    qint16 pitch;
    qint16 roll;
    double yaw;
}Pose_t;

typedef enum _GobotCmd
{
    // GCB2530-Only CMDs
    CMD_GCB2530_GET_VERSION = 0xC2,
    CMD_GCB2530_GET_VERSION_ACK = 0xC3,

    CMD_MCB_STM32_VERSIOM = 0xC4,
    CMD_MCB_STM32_VERSIOM_ACK	= 0xC5,

    CMD_GCB2530_RTDATA = 0x2A,	// GCB2530 RealTime Data
    CMD_IEEEADDR_INIT_GCB2530 = 0x60,
    CMD_IEEEADDR_INIT_GCB2530_ACK = 0x61,
    CMD_IEEEADDR_GCB2530_REQ = 0x62,	 // means GCB-cc2530 restarted
    CMD_GET_GCBIEEEADDR = 0x63,
    CMD_GET_GCBIEEEADDR_ACK = 0x64,

    // HandSet CMDs
    CMD_HANDSET_CTL_CENTER = 0x80,
    CMD_HANDSET_CTL_LEFT = 0x81,
    CMD_HANDSET_CTL_RIGHT = 0x82,
    CMD_HANDSET_CTL_FORWARD = 0x83,
    CMD_HANDSET_CTL_BACKWARD = 0x84,
    CMD_HANDSET_CTL_ACCCALIBRATION = 0x85,
    CMD_HANDSET_CTL_ACCCALIBRATION_ACK = 0x86,
    CMD_HANDSET_STATE_TK_OA = 0x87,
    CMD_HANDSET_CTL_STOP = 0x8B,
    CMD_HANDSET_CTL_STOP_ACK = 0x8C,

    CMD_HANDSET_OA_FUN_ENABLE_SETTING = 0x8D,
    CMD_HANDSET_OA_FUN_ENABLE_SETTING_ACK = 0x8E,

    CMD_HANDSET_GET_VERSION = 0xC0,
    CMD_HANDSET_GET_VERSION_ACK = 0xC1,
    CMD_HANDSET_STATECHANGE_RC = 0x90,
    CMD_HANDSET_STATECHANGE_RC_ACK = 0x91,
    CMD_HANDSET_STATESYNC = 0x92,
    CMD_HANDSET_STATESYNC_ACK = 0x93,
    CMD_HANDSET_STATECHANGE_TK = 0x94,
    CMD_HANDSET_STATECHANGE_TK_ACK = 0x95,

    CMD_HANDSET_STATECHANGE_OtherMode = 0x96,
    CMD_HANDSET_STATECHANGE_OtherMode_ACK = 0x97,

    CMD_HANDSET_STATECHANGE_PUSH = 0x98,
    CMD_HANDSET_STATECHANGE_PUSH_ACK = 0x99,   //added by yivi@141219

    CMD_HANDSET_STATECHANGE_GPSCTL_RCTTM = 0x9A,
    CMD_HANDSET_STATECHANGE_GPSCTL_RCTTM_ACK = 0x9B,
    CMD_HANDSET_GPS_CALIBRATION = 0x9C,
    CMD_HANDSET_GPS_CALIBRATION_ACK = 0x9D,
    CMD_HANDSET_GPS_CALIBRATION_STOP = 0x9E,
    CMD_HANDSET_GPS_CALIBRATION_STOP_ACK = 0x9F,

    CMD_HANDSET_STATECHANGE_TTM	= 0xA0,
    CMD_HANDSET_STATECHANGE_TTM_ACK=0xA1,

    CMD_HANDSET_BEEP = 0xB0,
    CMD_HANDSET_BEEP_ACK = 0xB1,

    CMD_IEEEADDR_REBOOTUP_BROADCAST = 0xE0,
    CMD_IEEEADDR_REBOOTUP_BROADCAST_ACK = 0xE1,

    CMD_IEEEADDR_BOOTUP_BROADCAST = 0xE2,
    CMD_IEEEADDR_BOOTUP_BROADCAST_ACK = 0xE3,
    CMD_HS_IEEEADDR_REGISTER = 0xE4,
    CMD_HS_IEEEADDR_REGISTER_ACK = 0xE5,

    CMD_HS_RFCHNSET_GRAY = 0xE6,
    CMD_HS_RFCHNSET_GRAY_ACK = 0xE7,

    CMD_HS_RFCHNSET_PINK = 0xE8,
    CMD_HS_RFCHNSET_PINK_ACK = 0xE9,

    CMD_HS_RFCHNSET_YELLOW = 0xEA,
    CMD_HS_RFCHNSET_YELLOW_ACK = 0xEB,

    CMD_TO_BLUETOOTH_INFO   = 0xE6,

    // STM32
    CMD_RC_FORWARD_MODE_SETTING = 0xA0,
    CMD_RC_FORWARD_MODE_SETTING_ACK = 0xA1,

    //CMD_MC_CLEAR_PWM = 0xAA,
    CMD_HANDSET_STATECHANGE_VTK = 0xAA,
    CMD_MC_CLEAR_PWM_ACK =0xBB,

    CMD_MC_TX_LINUX = 0xF1,
    CMD_MC_TX_ROLL_PITCH = 0xF2,
    CMD_MC_RTDATA = 0xF3,
    CMD_MC_TX_ACC = 0xF4,       //send acc to stm32
    CMD_MC_TX_ACC_ACK = 0xF5,   //send acc ack to linux

    CMD_MC_CFG_MOTOR_V_PID = 0xA7,
    CMD_MC_CFG_DIFF_V_PID = 0xA8,
    CMD_MC_CFG_EXIT = 0xA9,

    CMD_MC_CFG_MOTOR_V_PID_ACK = 0xB7,
    CMD_MC_CFG_DIFF_V_PID_ACK = 0xB8,
    CMD_MC_CFG_EXIT_ACK = 0xB9,

    CMD_MC_BAT_RECHARGE = 0xF6,
    CMD_MC_BAT_RECHARGE_ACK = 0xF7,

    CMD_MC_GET_VERSION = 0x10,
    CMD_MC_GET_VERSION_ACK = 0x11,

    CMD_GCB2530_RECOVER_LIST = 0xF0,    //addition by yivi
    CMD_GCB2530_RECOVER_LIST_ACK = 0xF1,
    CMD_LIST_INFO_REQ        = 0xF2,
    CMD_WRITE_LIST_INFO_REQ  = 0xF3,
    CMD_WRITE_LIST_INFO_REQ_ACK=0xF4,

    CMD_HOP_TO_PRIVATE_CHANNEL = 0xF7,
    CMD_HOP_TO_PRIVATE_CHANNEL_ACK= 0xF8,

    CMD_BC_GPS_OFFSET = 0xF9,

    CMD_NULL = 0xFF

}GobotCmd;

typedef enum
{
    // User
    CMD_USER_GET_SW_VERSION         = 0x10,
    CMD_USER_GCB_RESET              = 0x11,

    CMD_USER_PAUSE_GO               = 0x13,

    CMD_USER_TO_SET_RFCHANNEL_GRAY  = 0xE0,
    CMD_USER_TO_SET_RFCHANNEL_PINK  = 0xE1,
    CMD_USER_TO_SET_RFCHANNEL_YELLOW= 0xE2,
    CMD_USER_SET_HS_BE_USED_STATE   = 0xE3,
    CMD_USER_SET_OA_FUNCTION_STATE  = 0xE4,
    CMD_USER_COMM4_TIMEOUT_DEBUG	= 0xE5,
    CMD_USER_COMM5_TIMEOUT_DEBUG	= 0xE6,
    CMD_MOTOR_RELEASE				= 0xE7,
    CMD_RASPI_SETTING               = 0xE7,

    CMD_ENTER_TK_MODE				= 0xE8,
    CMD_EXIT_TK_MODE				= 0xE9,
    CMD_GO_IN_TK					= 0xEA,
    CMD_PAUSE_IN_TK					= 0xEB,

    CMD_VTK_INFO					= 0xEC,
    CMD_ENTER_VTK					= 0xED,
    CMD_EXIT_VTK					= 0xEE,

    CMD_BEEP_SOUND                  = 0xEF,

    CMD_SET_OA_FUNCTION             = 0xF0,

    CMD_USER_TEST_MODE              = 0xF1,
    CMD_USER_CTL_GPIO_TO_HIGH_IN_TEST_MODE = 0xF2,
    CMD_USER_CTL_GPIO_TO_LOW_IN_TEST_MODE = 0xF3,
    CMD_GPS_LOADING                 = 0xF4,
    CMD_GPS_READING                 = 0xF5,
    CMD_BLUETOOTH_HEAD              = 0xF6,
    CMD_CAMMERA_ANGLE_DETECT        = 0xF7,
    CMD_CLEAR_FAULT_FLAG	        = 0xF8,
    CMD_REPORT_OA_INFO		        = 0xF9,
    CMD_REPORT_RX_ACC_OF_MOTOR      = 0xFA,
    CMD_REPORT_INFO_RASPI	        = 0xFB,
    CMD_REPORT_INFO_YAW		        = 0xFC,
    CMD_OA_US_SETTING		        = 0xFD,
    CMD_REPOT_INFO_SETTINGV	        = 0xFE,

    CMD_VTAPE_SETTING_ACTION        = 0x8D,
    CMD_VTAPE_SPEED_DOWN            = 0x8E,
    CMD_VTAPE_SPEED_UP              = 0x8F,
    CMD_VTAPE_INFO				    = 0x90,
    CMD_ENTER_VTAPE				    = 0x91,
    CMD_EXIT_VTAPE				    = 0x92,

    CMD_BLUETOOTH_STATE_SYNC_ACK    = 0x93,
    CMD_DIAM_CALIBRATION	        = 0x94,
    CMD_ACC_CALIBRATION		        = 0x95,
    CMD_WALK_DIST_SET		        = 0x96,
    CMD_42D_DEBUG                   = 0x97,

    CMD_USER_GET_TOF_CHIP_INFO      = 0x98,//0xE7,
    CMD_USER_GET_US_INFO            = 0x99,//0xE8,
    CMD_USER_GET_2D_INFO            = 0x9A,//0xE9,
    CMD_USER_GET_IMU_INFO           = 0x9B,//0xEA,

    CMD_USER_SET_TFMINI_TOF_ADDR    = 0x9C,

    CMD_POSE_INFO                   = 0x9D,
    CMD_USER_ENTER_SB               = 0x9E,

    CMD_USER_NULL                   = 0xFF
}UserCmd;

// Cart state
typedef enum
{
    STATE_DUMP      = 0,
    STATE_RF_PAIRING = 0,
    STATE_SB = 1,
    STATE_RC = 2,
    STATE_TK = 3,
    STATE_OA = 4,//VTK
    STATE_VTK = 4,//VTK
    STATE_MARCH = 5,//added@141205 by yivi
    STATE_VTP = 6,
    STATE_POWERON=7,
    STATE_IDLE=8,
    STATE_MOTOR_RELEASE = 9,
    STATE_CAMERA_RELEASE = 9,
    STATE_GPSCTL_RCTT = 10,
    STATE_VMARCH = 10,
    STATE_AGV_RUN=11,
    STATE_AGV_TURN=12,
    STATE_AGV_STOP=13,
}CartState_e;

typedef enum
{
    PNGSTATE_DUMP      = 0,
    PNGSTATE_GO        = 1,
    PNGSTATE_PAUSE     = 2,
}PNGState_e;

typedef enum
{
    ACTION_TBD 					= 0,
    ACTION_STOP					= 1,
    //ACTION_BACK_STOP			= 2,
    ACTION_BACK_PASS			= 2,
    ACTION_QUICK_PASS			= 3,
    ACTION_PAUSE				= 4,

    ACTION_UTURN_STOP 			= 5,
    ACTION_UTURN_GO				= 6,
    ACTION_UTURN_BACK			= 7,
    ACTION_UTURN_RIGHT_STOP		= 8,
    ACTION_UTURN_LEFT_STOP		= 9,
    ACTION_UTURN_RIGHT_GO		= 10,
    ACTION_UTURN_LEFT_GO		= 11,
    ACTION_UTURN_LEFT_BACK		= 12,
    ACTION_UTURN_RIGHT_BACK		= 13,
    ACTION_UTURN_PAUSE			= 14,
    ACTION_UTURN_RIGHT_PAUSE	= 15,
    ACTION_UTURN_LEFT_PAUSE		= 16,

    ACTION_TURN_LEFT_GO			= 17,
    ACTION_TURN_RIGHT_GO		= 18,
    ACTION_TURN_LEFT_STOP		= 19,
    ACTION_TURN_RIGHT_STOP		= 20,
    ACTION_TURN_LEFT_BACK		= 21,
    ACTION_TURN_RIGHT_BACK		= 22,
    ACTION_TURN_RIGHT_PAUSE		= 23,
    ACTION_TURN_LEFT_PAUSE		= 24,

    ACTION_PASS					= 25,

    ACTION_MIDWAY_STOP			= 26,
    ACTION_4RESUME				= 27,
    ACTION_BACK_PAUSE			= 28,

    //absolute
    ACTION_ABSOLUTE_ANGLE_STOP  = 29,
    ACTION_ABSOLUTE_ANGLE_GO    = 30,
    ACTION_ABSOLUTE_ANGLE_PAUSE = 31,
    ACTION_ABSOLUTE_ANGLE_BACK  = 32,
    ACTION_ABSOLUTE_ANGLE_LAST_STATION_TOPARALLEL = 33,

    //arc turn 4vtp
    ACTION_ARC_TURN_4VTP      = 34,
}ActionOnCrossType_e;

typedef enum
{
    MS_IDLE = 0,
    MS_NORMAL = 1,
    MS_OVER_CURRENT = 2,
    MS_BLOCKED_ROTOR = 3,
    MS_COM_ERROR = 4,
    MS_LEFT_ENCODE_ERROR = 5,
    MS_RIGHT_ENCODE_ERROR = 6,

    MS_LEFT_HALL_ERROR = 7,
    MS_RIGHT_HALL_ERROR = 8
}MotorState_e;

typedef enum
{
    RC_NON = 0x00,
    RC_UP  = 0x02,
    RC_DOWN= 0x03,
    RC_LEFT= 0x04,
    RC_RIGHT= 0x05,
    RC_STOP= 0x06,
    RC_EXIT= 0x07,
    RC_ENTER= 0x08,

    VTK_ENTER= 0x09,
    VTP_ENTER= 0x0A,
    MARCH_ENTER= 0x0B,
    SB_ENTER= 0x0C,
    MR_ENTER= 0x0D,
}RCCMD_e;

typedef struct _BatCapacityInfo_t
{
    double theMaxCapacity;
    double voltage;
    double Capacity;
    double ratio;
    bool   lessThan10Flag;
    bool   lessThen20Flag;
}BatCapacityInfo_t;

typedef struct _RxInfo_t
{
    CartState_e CartState;
    MotorState_e MotorState;

    int16_t LeftRealSpeed;
    int16_t RightRealSpeed;
    int16_t LeftTargetSpeed;
    int16_t RightTargetSpeed;

    MotorVelocity_t MotorVelocity;

    uint8_t LeftTemp;
    uint8_t RightTemp;
    double BatVoltage;

    uint16_t VTKDist;
    int8_t  VTKAngle;

    bool    PauseNGoState;//MotorReleaseState;
    //bool    VTKInIdleState;
    uint16_t MultiFunction;

    uint16_t TOFMinDist;
    uint16_t USMinDist;
    int32_t ODO;

    ActionOnCrossType_e currentAction;

    CartState_e HSWantToCartState;

    int16_t roll;
    int16_t pitch;
    int16_t yaw;
}RxInfo_t;

typedef struct _TaskFlag_t
{
    quint32 TaskFlagKeepSendModeChange : 1;
    quint32 TaskFlagNeedToChangePNGStete : 1;
}TaskFlag_t;

typedef enum
{
    SPEED_CTL_DOWN = -1,
    SPEED_CTL_NULL = 0,
    SPEED_CTL_UP   = 1
}SpeedCtl_e;

enum ArcLocation_e
{
    NULL_LOCALTION = 0,
    LEFT_LOCALTION = 1,
    RIGHT_LOCALTION = 2,
};

typedef struct _PointAxis_t
{
    double x;
    double y;
}PointAxis_t;

typedef struct _ArcInfo_t
{
    ArcLocation_e ArcLocation;
    PointAxis_t CenterPoint;
    double      R;
}ArcInfo_t;

typedef struct _VTPInfo_t
{
    int16_t PointOnTape[7];//far -> near
    uint8_t CtlByte;
    int8_t StationName;
    uint16_t ToStationDist;
    uint16_t MaxSpeed;
    uint16_t PauseTime;
    SpeedCtl_e SpeedCtl;
    ActionOnCrossType_e setAction;

}VTPInfo_t;

typedef struct _VTKInfo_t
{
    uint16_t VTKDist;
    int8_t   VTKAngle;
    uint8_t  CtlByte;
}VTKInfo_t;

typedef struct _CtlByteFlag_t
{
    quint8 dump0 : 1;
    quint8 dump1 : 1;
    quint8 OASetOnStation : 1;
    quint8 dump3 : 1;

    quint8 dump4 : 1;
    quint8 UturnDir : 1;//bit5
    quint8 USOAOn_Off : 1;//bit6
    quint8 TOFOAOn_Off : 1;//bit7
}CtlByteFlag_t;

typedef struct _CartModeEnterParameter_t
{
    uint16_t minSpeed;
    uint16_t maxSpeed;
    union
    {
        uint8_t CtlByte;
        CtlByteFlag_t CtlByteFlag;
    }RCCtlByte;

    union
    {
        uint8_t CtlByte;
        CtlByteFlag_t CtlByteFlag;
    }VTKCtlByte;

    union
    {
        uint8_t CtlByte;
        CtlByteFlag_t CtlByteFlag;
    }VTPCtlByte;

    union
    {
        uint8_t CtlByte;
        CtlByteFlag_t CtlByteFlag;
    }MarchCtlByte;

    double LeftDiam;
    double RightDiam;
    ActionOnCrossType_e actionEndTape;
    ActionOnCrossType_e actionOnMark;
}CartModeEnterParameter_t;

typedef struct _ShowLogFlag_t
{
    uint8_t ShowRtLogFlag       : 1;
    uint8_t ShowMixLogFlag      : 1;
    uint8_t SaveLog2FileFlag    : 1;
    uint8_t vtpRtLogFlag        : 1;
    uint8_t ShowTxLogFlag       : 1;
}ShowLogFlag_t;

enum BeepType_t
{
    BEEP_TYPE_NULL = 0,
    BEEP_TYPE_SHORT_BEEP_1,
    BEEP_TYPE_SHORT_BEEP_2,
    BEEP_TYPE_SHORT_BEEP_3,
    BEEP_TYPE_SHORT_BEEP_4,

    BEEP_TYPE_LONG_BEEP_1,
    BEEP_TYPE_LONG_BEEP_3,
    BEEP_TYPE_LONG_LONG_BEEP_3,

    BEEP_TYPE_L1_S1_L1_S1,
    BEEP_TYPE_CONTINUOUS,
    BEEP_TYPE_DISABLE = 250,
};

#endif // PLATFORMPARAMETER_H
