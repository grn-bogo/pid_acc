#ifndef PIDTEST_H
#define PIDTEST_H

#include <QtTest/QtTest>
#include <autotest.h>

#include <pidcontroller.h>

class PIDTest : public QObject
{
    Q_OBJECT

public:
    PIDTest() {}
protected:
    std::shared_ptr<InitialConditions> initCnds_;
    double targetvValue_ = 2.5;
    double processValue_ = 0.0;

private slots:
    void initTestCase()
    {
        initCnds_ = std::make_shared<InitialConditions>();
    }

    void testStablizationIn2000()
    {
        PIDController pid(initCnds_, targetvValue_);
        double stepsToSetPoint = 2000;
        for(int i = 0; i < stepsToSetPoint; ++i)
            processValue_ += pid.calcAdjustment(processValue_);
        QVERIFY(processValue_ == targetvValue_);
    }

    void testStablizationIn1000()
    {
        PIDController pid(initCnds_, targetvValue_);
        double stepsToSetPoint = 1000;
        for(int i = 0; i < stepsToSetPoint; ++i)
            processValue_ += pid.calcAdjustment(processValue_);
        QVERIFY(processValue_ == targetvValue_);
    }

    void testStablizationIn400()
    {
        PIDController pid(initCnds_, targetvValue_);
        double stepsToSetPoint = 400;
        for(int i = 0; i < stepsToSetPoint; ++i)
            processValue_ += pid.calcAdjustment(processValue_);
        QVERIFY(processValue_ == targetvValue_);
    }
};

DECLARE_TEST(PIDTest)

#endif // PIDTEST_H
