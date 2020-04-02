#ifndef CARGRAPHICS_H
#define CARGRAPHICS_H

#include <QBrush>
#include <QGraphicsRectItem>

class QCarGraphics : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    QCarGraphics()
    {
        this->setRect(0, 0, 20, 10);
        this->setBrush(QBrush(Qt::black));
    };

public slots:
    void setXPos(double x)
    {
        this->setX(x);
    };

    void setYPos(double y)
    {
        this->setX(y);
    };


};

#endif // CARGRAPHICS_H
