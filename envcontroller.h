#ifndef ENVCONTROLLER_H
#define ENVCONTROLLER_H

#include <QGraphicsScene>
#include <QObject>
#include <QTimer>

#include <carmodel.h>
#include <qcargraphics.h>
#include <random>
#include <unordered_map>
#include <unordered_set>
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
        connect(&envUpdate_, SIGNAL(timeout()), this, SLOT(update()));
        envUpdate_.start(50);
        QTimer::singleShot(randGenInterval(), this, SLOT(trigger()));
    }

public slots:

    void setCarXPos(int carID, double carPosX)
    {
        controllerMapping_[carID].second->setXPos(carPosX);
    }

    void removeCar(int carID)
    {
        if (removedCarIDs_.count(carID) == 1) return;

        auto it = find_if(cars_.rbegin(), cars_.rend(),
                          [&carID](const std::pair<CarModel*, QCarGraphics*>& pair) { return pair.first->id() == carID; });
        if (it == cars_.rend()) return;

        envScene_.removeItem(it->second);
        delete it->second;

        this->disconnect(it->first);
        it->first->disconnect(this);
        it->first->deleteLater();

        if (it == cars_.rbegin()) cars_.pop_back();
        else cars_.erase(it.base());

        removedCarIDs_.emplace(carID);
    }

    void trigger()
    {
        addCar();
        QTimer::singleShot(randGenInterval(), this, SLOT(trigger()));
    }

    void addCar()
    {
        QCarGraphics* carGrPtr = new QCarGraphics();
        CarModel* carPtr = new CarModel(randDx());
        connect(carPtr, SIGNAL(reachedLaneEnd(int)), this, SLOT(removeCar(int)));
        connect(carPtr, SIGNAL(lanePosChanged(int, double)), this, SLOT(setCarXPos(int, double)));
        envScene_.addItem(carGrPtr);
//        auto modelViewPair = std::make_pair(std::move(carPtr), std::move(carGrPtr));
        auto modelViewPair = std::make_pair(carPtr, carGrPtr);
        cars_.emplace_back(modelViewPair);
        controllerMapping_.insert(std::make_pair(carPtr->id(), modelViewPair));
    }

    void update()
    {
        if (cars_.size() < 2) return;
        std::sort(cars_.begin(),
                  cars_.end(),
                  [](const std::pair<CarModel*, QCarGraphics*>& carOne, const std::pair<CarModel*, QCarGraphics*>& carTwo)
                    { return carOne.first->xPos() < carTwo.first->xPos(); });
        for(std::size_t i = 0; i < cars_.size() - 2; ++i)
        {
            double distanceToNext = cars_[i + 1].first->xPos() - cars_[i].first->xPos();
            if (distanceToNext < 80.0) cars_[i].first->frontSensor(distanceToNext, cars_[i + 1].first->currentDx());
        }
    }

protected:

    double randDx() { return dx_distro_->operator()(*mt_.get()); }
    int randGenInterval() { return gen_distro_->operator()(*mt_.get()); }

    std::unique_ptr<std::random_device> rd_;
    std::unique_ptr<std::mt19937> mt_;
    std::unique_ptr<std::uniform_real_distribution<double> > dx_distro_;
    std::unique_ptr<std::uniform_int_distribution<int> >gen_distro_;

    QGraphicsScene& envScene_;
    QTimer envUpdate_;

    std::vector<std::pair<CarModel*, QCarGraphics*>> cars_;
    std::unordered_map<int, std::pair<CarModel*, QCarGraphics*>> controllerMapping_;
    std::unordered_set<int> removedCarIDs_;

    double MAX_CAR_DX = 1.5;
    double MIN_CAR_DX = 2.5;
    int MIN_GEN_INTERVAL = 3500;
    int MAX_GEN_INTERVAL = 5500;

};

#endif // ENVCONTROLLER_H
