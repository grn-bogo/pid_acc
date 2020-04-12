#ifndef ENVCTRLTEST_H
#define ENVCTRLTEST_H

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

#include <autotest.h>

#include <envcontroller.h>

class EnvCtrlTest : public QObject
{
    Q_OBJECT

public:
    EnvCtrlTest() {}

protected:
    EnvController envCtrl_;

private slots:
    void initTestCase()
    {
        envCtrl_.init();
    }

    void testSingleCarReachSpeed()
    {
        envCtrl_.addCar();
        QSignalSpy spy(envCtrl_.getCar(1), SIGNAL(lanePosChanged(int carID, double newPos)));
        for(int i= 0; i < 1000; ++i)
        {
            envCtrl_.update();
        }
        envCtrl_.removeCar();
    }

    void cleanupTestCase(){}
};

DECLARE_TEST(EnvCtrlTest)

#endif // ENVCTRLTEST_H
