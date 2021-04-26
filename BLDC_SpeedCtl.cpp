#include "ftr_ctr3speedctl.h"

void FTR_CTR3SpeedCtl::BLDC_StateSB_VelocityTxPreprocess(int16_t lvel, int16_t avel, int16_t *pSpeed_left, int16_t *pSpeed_right)
{
    double delta = MOTOR_MOTOR_DIST / 2.0 * ANGLE_DEGREE2RADIAN(avel);
    *pSpeed_left = lvel + delta;
    *pSpeed_right = lvel - delta;
}
void FTR_CTR3SpeedCtl::BLDC_StateRC_VelocityTxPreprocess(int16_t lvel, int16_t avel, int16_t *pSpeed_left, int16_t *pSpeed_right)
{
    double delta = MOTOR_MOTOR_DIST / 2.0 * ANGLE_DEGREE2RADIAN(avel);

#if(RC_REDIRECT)
    *pSpeed_left = lvel + delta;
    *pSpeed_right = lvel - delta;
#else
    int16_t tmp;
    *pSpeed_left = lvel + delta;
    *pSpeed_right = lvel - delta;

    tmp = (-1) * (*pSpeed_left);
    *pSpeed_left = (-1) * (*pSpeed_right);
    *pSpeed_right = tmp;
#endif
}

void FTR_CTR3SpeedCtl::TxToBLDC_RTData(int16_t speed_l,int16_t speed_r,uint8_t CtlByte,double LeftODOFactor,double RightODOFactor,CartState_e CartState,int8_t PIDStop)
{
    QByteArray BLDCTxBuffer;
#if(SERIAL_BYPASS)
    //BLDCTxBuffer.resize(5);
    BLDCTxBuffer[0] = SOP;
    BLDCTxBuffer[1] = 0xEF;
    BLDCTxBuffer[2] = 0x00;
    BLDCTxBuffer[3] = 0x00;

    BLDCTxBuffer.append(this->App_XOR(BLDCTxBuffer));
    this->AppUART->write(BLDCTxBuffer);
    this->AppUART->flush();
#else
    BLDCTxBuffer[0] = MCB_BLDC_PKT_SOP;
    BLDCTxBuffer[1] = CMD_MC_RTDATA;
    BLDCTxBuffer[2] = 0x0C;

    BLDCTxBuffer[3] = CartState;
    BLDCTxBuffer[4] = CtlByte;

    BLDCTxBuffer[5] = (speed_l>>8) & 0xFF;
    BLDCTxBuffer[6] = speed_l & 0xFF;

    BLDCTxBuffer[7] = (speed_r>>8) & 0xFF;
    BLDCTxBuffer[8] = speed_r & 0xFF;

    uint16_t LeftFactor =LeftODOFactor * 100000;
    uint16_t RightFactor=RightODOFactor * 100000;

    BLDCTxBuffer[9] = (LeftFactor >> 8) & 0x00FF; //High
    BLDCTxBuffer[10] = LeftFactor & 0x00FF; //low
    BLDCTxBuffer[11] = (RightFactor >> 8) & 0x00FF; //High
    BLDCTxBuffer[12] = RightFactor & 0x00FF; //low

    BLDCTxBuffer[13] = 0x00;
    BLDCTxBuffer[14] = 0x00;

    uint16_t crc = this->App_CRC(BLDCTxBuffer);
    BLDCTxBuffer[15] = (crc>>8) & 0xFF;
    BLDCTxBuffer[16] = crc & 0xFF;
#endif
    //this->AppUART->write(BLDCTxBuffer);
    //this->AppUART->flush();
}

void FTR_CTR3SpeedCtl::SetMotorRelease()//CMD = 0xE7
{
    this->SendCMD(CMD_MOTOR_RELEASE);
}
