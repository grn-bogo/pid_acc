#ifndef PIDTEST_H
#define PIDTEST_H

#include <QtTest/QtTest>
#include <autotest.h>

#include <pidcontroller.h>

class PIDTest : public QObject
{
    Q_OBJECT

public:
    PIDTest() :
        pid_(1, 0.04, -0.7)
    {}
protected:
    PIDController pid_;
    double setPoint_ = 2.5;
    double processValue_ = 0.0;

private slots:
    void testStablizationIn2000()
    {
        double stepsToSetPoint = 2000;
        for(int i = 0; i < stepsToSetPoint; ++i)
            processValue_ += pid_.calcAdjustment(setPoint_, processValue_);
        QVERIFY(processValue_ == setPoint_);
    }

    void testStablizationIn1000()
    {
        double stepsToSetPoint = 1000;
        for(int i = 0; i < stepsToSetPoint; ++i)
            processValue_ += pid_.calcAdjustment(setPoint_, processValue_);
        QVERIFY(processValue_ == setPoint_);
    }

    void testStablizationIn400()
    {
        double stepsToSetPoint = 400;
        for(int i = 0; i < stepsToSetPoint; ++i)
            processValue_ += pid_.calcAdjustment(setPoint_, processValue_);
        QVERIFY(processValue_ == setPoint_);
    }
};

DECLARE_TEST(PIDTest)

#endif // PIDTEST_H
