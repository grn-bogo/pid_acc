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
        double tolerance = 0.00001;
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

    void testCarReachTrackEnd()
    {
        CarModel carModel(2.5, 2500); //dx: 2.5, track end: 2500
        int stepsToTrackEnd = 1100;
        QSignalSpy laneEndSpy(&carModel, SIGNAL(reachedLaneEnd(int)));
        for(int i= 0; i < stepsToTrackEnd; ++i) carModel.update();
        QVERIFY(laneEndSpy.count() > 1);
    }

};

DECLARE_TEST(CarModelTest)

#endif // CARMODELTEST_H
