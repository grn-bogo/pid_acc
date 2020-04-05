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
    CarModel(double dx = 2.5, int trackEnd = 1770):
        preferredDx_(dx),
        pid_(1, 0.04, -0.04),
        id_(CarModel::carID++),
        trackEnd_(trackEnd)

    {
        connect(&this->innerClock_, SIGNAL(timeout()), this, SLOT(update()));
        innerClock_.start(50);
    }

    inline static int carID = 1;
    int id() const { return id_; }
    double preferredDx() const { return preferredDx_; }
    double currentDx() const { return currentDx_; }
    double xPos() const { return xPos_; }

public slots:
    void frontSensor(double obstacleDistance, double obstacleDx)
    {
        if (preferredDx_ != obstacleDx)
        {
            pid_.reset();
            preferredDx_ = obstacleDx;
        }
    }

signals:
    void lanePosChanged(int carID, double newPos);
    void reachedLaneEnd(int carID);

protected slots:

    void update()
    {
        double dxAdjustment = pid_.calcAdjustment(preferredDx_, currentDx_);
        currentDx_ += dxAdjustment;
        if (currentDx_ < 0.0) currentDx_ = 0.02;
        xPos_ += currentDx_;
        emit lanePosChanged(id_, xPos_);
        if (xPos_ > trackEnd_) emit reachedLaneEnd(id_);
    }


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

