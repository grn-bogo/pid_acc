#ifndef INITIALCONDITIONS_H
#define INITIALCONDITIONS_H

struct PIDConf
{
    double proportional = 0.03;
    double derivative = 0.01;
    double integral = 0.001;
    double updateInterval = 1.0;
    double maxAdjustment = 0.1;
    double minAdjustment = -0.7;
};

struct InitialConditions
{
    PIDConf pidConsts;
    double minCarDx = 1.0;
    double maxCarDx = 2.0;
    double distanceToNext = 40.0;
    int minGenInterval = 3000;
    int maxGenInterval = 5500;
    int updateInterval = 40;
    int startDelay = 500;
    int laneLength = 1770;

    double pidProportional() { return  pidConsts.proportional; }
    double pidDerivative() { return  pidConsts.derivative; }
    double pidIntegral() { return  pidConsts.integral; }
    double pidUpdateInterval() { return pidConsts.updateInterval > 0 ? pidConsts.updateInterval : 1.0; }
    double pidMinAdjust() { return  pidConsts.minAdjustment; }
    double pidMaxAdjust() { return  pidConsts.maxAdjustment; }
};

#endif // INITIALCONDITIONS_H
