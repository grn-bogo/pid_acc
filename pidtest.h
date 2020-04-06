#ifndef PIDTEST_H
#define PIDTEST_H

#include <QtTest/QtTest>

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
    int stepsToSetPoint_ = 2000;

private slots:
    void testStablization()
    {
        for(int i = 0; i < stepsToSetPoint_; ++i)
            processValue_ += pid_.calcAdjustment(setPoint_, processValue_);
        QVERIFY(processValue_ == setPoint_);
    }
};

QTEST_MAIN(PIDTest)

#endif // PIDTEST_H
