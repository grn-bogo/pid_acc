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
    std::unique_ptr<EnvController> envCtrl_;
    std::shared_ptr<InitialConditions> initCnds_;

private slots:
    void initTestCase()
    {
        initCnds_ = std::make_shared<InitialConditions>();
        envCtrl_ = std::make_unique<EnvController>(initCnds_);
    }

    void testSingleCarReachSpeed()
    {
    }

    void cleanupTestCase(){}
};

DECLARE_TEST(EnvCtrlTest)

#endif // ENVCTRLTEST_H
