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
    }

    void cleanupTestCase(){}
};

DECLARE_TEST(EnvCtrlTest)

#endif // ENVCTRLTEST_H
