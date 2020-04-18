#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <QDebug>


#include <initialconditions.h>

class PIDController
{

public:
    PIDController(
            std::shared_ptr<InitialConditions> ic,
            double setPoint
            ):
        initialConditions_(ic),
        setPoint_(setPoint)
    {}

    double calcAdjustment(double processValue)
    {
        double error = setPoint_ - processValue;

        double proportionalTerm = initialConditions_->pidProportional() * error;

        integralSum_ += error * initialConditions_->pidUpdateInterval();
        double integralTerm = initialConditions_->pidIntegral() * integralSum_;

        double derivative = (error - lastError_) / initialConditions_->pidUpdateInterval();
        double derivativeTerm = initialConditions_->pidDerivative() * derivative;

        double adjustment = proportionalTerm + integralTerm + derivativeTerm;

        return adjustInRange(adjustment);
    }

    double adjustInRange(double adjustment)
    {
        if (adjustment < initialConditions_->pidMinAdjust()) return initialConditions_->pidMinAdjust();
        else if (adjustment > initialConditions_->pidMaxAdjust()) return initialConditions_->pidMaxAdjust();
        else return adjustment;
    }

    void reset() { integralSum_ = 0.0; }

    void setPoint(double newSetPoint) { setPoint_ = newSetPoint; }
    double setPoint() { return setPoint_; }

private:
    std::shared_ptr<InitialConditions> initialConditions_;
    double setPoint_ = 0.0;
    double lastError_ = 0.0;
    double integralSum_ = 0.0;

};

#endif // PIDCONTROLLER_H
