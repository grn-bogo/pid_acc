#ifndef CARMODEL_H
#define CARMODEL_H

#include <QDebug>

#include <QObject>
#include <QTimer>
#include <icargraphics.h>
#include <pidcontroller.h>

class CarModel : QObject
{
    Q_OBJECT

public:
    CarModel(ICarGraphics& carGraphics, double dx = 2.5):
        preferredDx_(dx),
        carGraphics_(carGraphics),
        pid_(1, 0.04, -0.04)
    {
        connect(&this->innerClock_, SIGNAL(timeout()), this, SLOT(update()));
        innerClock_.start(20);
    }

protected slots:
    void setPreferredDx(double newDx)
    {
        preferredDx_ = newDx;
    }
    void update()
    {
        double dxAdjustment = pid_.calcAdjustment(preferredDx_, currentDx_);
        currentDx_ += dxAdjustment;
        xPos_ += currentDx_;
        qDebug() << "CAR DX: " << currentDx_;
        carGraphics_.setXPos(xPos_);
    }
    void frontSensor(double obstacleDistance, double obstacleDx)
    {

    }

protected:
    double xPos_ = 0;
    double preferredDx_ = 0;
    double currentDx_ = 0;
    QTimer innerClock_;
    ICarGraphics& carGraphics_;
    PIDController pid_;
};

#endif // CARMODEL_H
