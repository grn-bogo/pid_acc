#ifndef CARMODEL_H
#define CARMODEL_H

#include <QDebug>

#include <QObject>
#include <QTimer>
#include <qcargraphics.h>
#include <pidcontroller.h>


class CarModel : public QObject
{
    Q_OBJECT

public:
    CarModel(double dx = 2.5, int trackEnd = 1800):
        preferredDx_(dx),
        pid_(1, 0.05, -0.05),
        id_(CarModel::carID++),
        trackEnd_(trackEnd)

    {
        connect(&this->innerClock_, SIGNAL(timeout()), this, SLOT(update()));
        innerClock_.start(30);
    }

    inline static int carID = 1;
    int id() { return id_; }

signals:
    void lanePosChanged(double newPos);
    void reachedLaneEnd(int carID);

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
        emit lanePosChanged(xPos_);
//        qDebug() << "CAR DX: " << currentDx_;
        if (xPos_ > trackEnd_) emit reachedLaneEnd(id_);
    }
//    void frontSensor(double obstacleDistance, double obstacleDx)
//    {

//    }

protected:
    double xPos_ = 0;
    double preferredDx_ = 0;
    double currentDx_ = 0;
    QTimer innerClock_;
    PIDController pid_;
    int id_;
    int trackEnd_;
};



#endif // CARMODEL_H
