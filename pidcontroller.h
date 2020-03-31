#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <QDebug>

class PIDController
{
public:
    PIDController(
            double updateInterval,
            double maxAdjust, double minAdjust,
            double Kp = 0.03, double Kd = 0.01, double Ki = 0.001
            ):
        updateInterval_(updateInterval),
        maxCtrlAdjust_(maxAdjust), minCtrlAdjust_(minAdjust),
        Kp_(Kp), Kd_(Kd), Ki_(Ki)
    {};

    double calcAdjustment(double setPoint, double processValue)
    {
        qDebug() << "SETPOINT: " << setPoint << "PROCESSVALUE" << processValue;
        double error = setPoint - processValue;
        qDebug() << "ERROR: " << error;

        double proportionalTerm = Kp_ * error;

        integralSum_ += error * updateInterval_;
        double integralTerm = Ki_ * integralSum_;

        double derivative = (error - lastError_) / updateInterval_;
        double derivativeTerm = Kd_ * derivative;

        double adjustment = proportionalTerm + integralTerm + derivativeTerm;

        qDebug() << "ADJUSTMENT: " << adjustment;
        return adjustInRange(adjustment);
    };

    double adjustInRange(double adjustment)
    {
        if (adjustment < minCtrlAdjust_) return minCtrlAdjust_;
        else if (adjustment > maxCtrlAdjust_) return maxCtrlAdjust_;
        else return adjustment;
    }

private:
    double updateInterval_;
    double maxCtrlAdjust_;
    double minCtrlAdjust_;
    double Kp_;
    double Kd_;
    double Ki_;
    double lastError_ = 0.0;
    double integralSum_ = 0.0;

};

#endif // PIDCONTROLLER_H
