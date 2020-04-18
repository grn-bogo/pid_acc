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
    EnvController(std::shared_ptr<InitialConditions> ic, QGraphicsScene* const envScene=NULL) :
        envScene_(envScene),
        initCnds_(ic)
    {}

    void init()
    {
        rd_ = std::make_unique<std::random_device>();
        mt_ = std::make_unique<std::mt19937>(rd_->operator()());
        dx_distro_ = std::make_unique<std::uniform_real_distribution<double> >(initCnds_->minCarDx, initCnds_->maxCarDx);
        gen_distro_ = std::make_unique<std::uniform_int_distribution<int> >(initCnds_->minGenInterval, initCnds_->maxGenInterval);
        connect(&envUpdate_, SIGNAL(timeout()), this, SLOT(update()));
        envUpdate_.start(initCnds_->updateInterval);
        QTimer::singleShot(initCnds_->startDelay, this, SLOT(trigger()));
    }

public slots:

    void updateCarView(int carID, double carPosX)
    {
        controllerMapping_[carID].second->setXPos(carPosX);
    }

    void triggerRemoveCar(int carID)
    {
        removeNextCycle_ = carID;
        //schedule for next event loop cycle
        QTimer::singleShot(0, this, SLOT(removeCar()));
    }

    const CarModel* getCarModel(int carID)
    {
        return controllerMapping_[carID].first;
    }

    void removeCar()
    {
        int carID = removeNextCycle_;
        if (removedCarIDs_.count(carID) == 1) return;

        auto it = find_if(cars_.rbegin(), cars_.rend(),
                          [&carID](const std::pair<CarModel*, QCarGraphics*>& pair) { return pair.first->id() == carID; });
        if (it == cars_.rend())
            return;

        removeCarGraphics(it->second);
        removeCarModel(it->first);
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
        CarModel* carPtr = new CarModel(initCnds_, randDx());
        connect(carPtr, SIGNAL(reachedLaneEnd(int)), this, SLOT(triggerRemoveCar(int)));
        connect(carPtr, SIGNAL(lanePosChanged(int, double)), this, SLOT(updateCarView(int, double)));
        connect(&envUpdate_, SIGNAL(timeout()), carPtr, SLOT(update()));
        if(envScene_) envScene_->addItem(carGrPtr);

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
            if (distanceToNext < initCnds_->distanceToNext) cars_[i].first->frontSensor(cars_[i + 1].first->preferredDx());
        }
    }

protected:

    void removeCarModel(CarModel* carModel)
    {
        envUpdate_.disconnect(carModel);
        this->disconnect(carModel);
        carModel->disconnect(this);
        carModel->deleteLater();
    }

    void removeCarGraphics(QCarGraphics* carGraphics)
    {
        if(envScene_) envScene_->removeItem(carGraphics);
        delete carGraphics;
    }

    double randDx() { return dx_distro_->operator()(*mt_.get()); }
    int randGenInterval() { return gen_distro_->operator()(*mt_.get()); }

    std::unique_ptr<std::random_device> rd_;
    std::unique_ptr<std::mt19937> mt_;
    std::unique_ptr<std::uniform_real_distribution<double> > dx_distro_;
    std::unique_ptr<std::uniform_int_distribution<int> >gen_distro_;

    QGraphicsScene* const envScene_;
    QTimer envUpdate_;

    std::vector<std::pair<CarModel*, QCarGraphics*>> cars_;
    std::unordered_map<int, std::pair<CarModel*, QCarGraphics*>> controllerMapping_;
    std::unordered_set<int> removedCarIDs_;
    int removeNextCycle_ = 0;

    std::shared_ptr<InitialConditions> initCnds_;
};

#endif // ENVCONTROLLER_H
