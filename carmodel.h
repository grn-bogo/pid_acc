#ifndef CARMODEL_H
#define CARMODEL_H

#include <QDebug>

#include <QObject>
#include <QTimer>

#include <initialconditions.h>
#include <qcargraphics.h>
#include <pidcontroller.h>


class CarModel : public QObject
{
    Q_OBJECT

public:
    CarModel(std::shared_ptr<InitialConditions> ic, double dx = 2.5):
        pid_(ic, dx),
        id_(CarModel::carID++),
        trackEnd_(ic->laneLength)
    {}

    inline static int carID = 1;
    int id() const { return id_; }
    double preferredDx() const { return pid_.setPoint(); }
    double currentDx() const { return currentDx_; }
    double xPos() const { return xPos_; }

public slots:
    void frontSensor(double obstacleDx)
    {
        if (pid_.setPoint() != obstacleDx)
        {
            pid_.reset();
            pid_.setPoint(obstacleDx);
        }
    }

    void update()
    {
        double dxAdjustment = pid_.calcAdjustment(currentDx_);
        currentDx_ += dxAdjustment;
        if (currentDx_ < minDx_) currentDx_ = minDx_;
        xPos_ += currentDx_;
        if (xPos_ > trackEnd_) emit reachedLaneEnd(id_);
        else emit lanePosChanged(id_, xPos_);
    }

signals:
    void lanePosChanged(int carID, double newPos);
    void reachedLaneEnd(int carID);

protected:
    double xPos_ = 0.0;
    double prefferedDx_ = 0.0;
    double currentDx_ = 0.0;
    PIDController pid_;
    int id_;
    int trackEnd_;    

private:
    double minDx_ = 0.002;

};

#endif // CARMODEL_H

