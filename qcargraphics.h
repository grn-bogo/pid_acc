#ifndef CARGRAPHICS_H
#define CARGRAPHICS_H

#include <QBrush>
#include <QGraphicsRectItem>

class QCarGraphics : public QGraphicsRectItem
{

public:
    QCarGraphics()
    {
        setRect(0, 45, 20, 10);
        setBrush(QBrush(Qt::black));
    }

public slots:
    void setXPos(double x)
    {
        this->setX(x);
    }

    void setYPos(double y)
    {
        this->setX(y);
    }

};

#endif // CARGRAPHICS_H
