#ifndef ENVCONTROLLER_H
#define ENVCONTROLLER_H

#include <QGraphicsScene>
#include <QObject>
#include <QTimer>

#include <carmodel.h>
#include <qcargraphics.h>
#include <random>
#include <unordered_map>
#include <utility>
#include <functional>

class EnvController : public QObject
{
    Q_OBJECT

public:
    EnvController(QGraphicsScene& envScene) :
        envScene_(envScene)
    {}

    void init()
    {
        rd_ = std::make_unique<std::random_device>();
        mt_ = std::make_unique<std::mt19937>(rd_->operator()());
        dx_distro_ = std::make_unique<std::uniform_real_distribution<double> >(MIN_CAR_DX, MAX_CAR_DX);
        gen_distro_ = std::make_unique<std::uniform_int_distribution<int> >(MIN_GEN_INTERVAL, MAX_GEN_INTERVAL);
        QTimer::singleShot(randGenInterval(), this, SLOT(trigger()));
    }

public slots:
    void removeCar(int carID)
    {
        envScene_.removeItem(cars_[carID].second.get());
        cars_.erase(carID);
    }

    void trigger()
    {
        addCar();
        int nextCarInMs = randGenInterval();
        QTimer::singleShot(nextCarInMs, this, SLOT(trigger()));
    }

    void addCar()
    {
        auto carGrPtr = std::make_unique<QCarGraphics>();
        auto carPtr = std::make_unique<CarModel>(randDx());
        connect(carPtr.get(), SIGNAL(reachedLaneEnd(int)), this, SLOT(removeCar(int)));
        connect(carPtr.get(), SIGNAL(lanePosChanged(double)), carGrPtr.get(), SLOT(setXPos(double)));
        envScene_.addItem(carGrPtr.get());
        cars_.insert(
                    std::make_pair(
                        carPtr->id(),
                        std::make_pair(std::move(carPtr), std::move(carGrPtr))));

    }

protected:

    double randDx() { return dx_distro_->operator()(*mt_.get()); }
    int randGenInterval() { return gen_distro_->operator()(*mt_.get()); }

    std::unique_ptr<std::random_device> rd_;
    std::unique_ptr<std::mt19937> mt_;
    std::unique_ptr<std::uniform_real_distribution<double> > dx_distro_;
    std::unique_ptr<std::uniform_int_distribution<int> >gen_distro_;
    QGraphicsScene& envScene_;
    std::unordered_map<int, std::pair<std::unique_ptr<CarModel>, std::unique_ptr<QCarGraphics>> > cars_;

    double MAX_CAR_DX = 1.5;
    double MIN_CAR_DX = 3.5;
    int MIN_GEN_INTERVAL = 3000;
    int MAX_GEN_INTERVAL = 6000;



};

#endif // ENVCONTROLLER_H
