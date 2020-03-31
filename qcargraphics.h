#ifndef CARGRAPHICS_H
#define CARGRAPHICS_H

#include <QBrush>
#include <QGraphicsRectItem>
#include <icargraphics.h>
class QCarGraphics : public ICarGraphics, public QGraphicsRectItem
{
public:
    QCarGraphics()
    {
        this->setRect(0, 0, 20, 10);
        this->setBrush(QBrush(Qt::black));
    };

    virtual void setXPos(double x) override
    {
        this->setX(x);
    };

    virtual void setYPos(double y)
    {
        this->setX(y);
    };
};

#endif // CARGRAPHICS_H
