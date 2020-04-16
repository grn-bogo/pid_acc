#ifndef CARMODELTEST_H
#define CARMODELTEST_H

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtMath>

#include <autotest.h>

#include <carmodel.h>

class CarModelTest : public QObject
{
    Q_OBJECT

public:
    CarModelTest() {}

private slots:

    void testCarReachSpeed1000()
    {
        CarModel carModel;
        double tolerance = 0.0001;
        for(int i= 0; i < 1000; ++i) carModel.update();
        double epsilon = qFabs(carModel.currentDx() - carModel.preferredDx());
        QVERIFY(epsilon < tolerance);
    }

    void testCarReachSpeed400()
    {
        CarModel carModel;
        double tolerance = 0.1;
        for(int i= 0; i < 400; ++i) carModel.update();
        double epsilon = qFabs(carModel.currentDx() - carModel.preferredDx());
        QVERIFY(epsilon < tolerance);
    }

    void testCarPosUpdate()
    {
        CarModel carModel;
        QSignalSpy posChangedSpy(&carModel, SIGNAL(lanePosChanged(int, double)));
        for(int i= 0; i < 2000; ++i) carModel.update();
    }

    void testCarReachTrackEnd()
    {
        CarModel carModel;
        QSignalSpy laneEndspy(&carModel, SIGNAL(eachedLaneEnd(int)));
        for(int i= 0; i < 2000; ++i) carModel.update();
    }

};

DECLARE_TEST(CarModelTest)

#endif // CARMODELTEST_H
