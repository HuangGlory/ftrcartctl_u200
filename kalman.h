#ifndef KALMAN_H
#define KALMAN_H


class Kalman
{
public:
    Kalman();
    Kalman(double q,double r, double Result);
    void Init(double q,double r, double Result);
    double KalmanCalc(double NewData);
    void setResult(double Result);
    double getResult(void);
private:
    double Q;
    double R;

    double Result;
    double factor1;
    double factor2;
};

#endif // KALMAN_H
