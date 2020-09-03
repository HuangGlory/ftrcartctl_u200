#include "kalman.h"
#include <QDebug>

Kalman::Kalman():Q(0),R(0),Result(0),factor1(0),factor2(0)
{

}

Kalman::Kalman(double q,double r, double Result)
{
    this->Q = q;
    this->R = r;
    this->Result = Result;
}
void Kalman::Init(double q,double r, double Result)
{
    this->Q = q;
    this->R = r;
    this->Result = Result;
}

double Kalman::KalmanCalc(double NewData)
{
    double Pre_Result = this->Result;
    double Factor1Temp = 0;

    Factor1Temp = this->factor1 + this->Q;
    this->factor2 = Factor1Temp / ( Factor1Temp + this->R );
    this->Result = Pre_Result + this->factor2*(NewData - Pre_Result);
    this->factor1 = (1 - this->factor2)*Factor1Temp;
    return this->Result;
}
