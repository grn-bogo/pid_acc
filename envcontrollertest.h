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
        const CarModel* carModel = envCtrl_.getCarModel(1);
        QSignalSpy spy(carModel, SIGNAL(lanePosChanged(int, double)));
        for(int i= 0; i < 1000; ++i)
        {
            envCtrl_.update();
        }
        QCOMPARE(carModel->currentDx(), carModel->preferredDx());
        envCtrl_.removeCar();
    }

    void cleanupTestCase(){}
};

DECLARE_TEST(EnvCtrlTest)

#endif // ENVCTRLTEST_H
