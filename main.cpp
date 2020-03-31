#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <icargraphics.h>
#include <qcargraphics.h>
#include <carmodel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCarGraphics carGraphics1;
    CarModel cm1(carGraphics1, 3.0);
    QCarGraphics carGraphics2;
    carGraphics2.setY(30);
    CarModel cm2(carGraphics2, 2.0);

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 1820,300);
    scene.addItem(&carGraphics1);
    scene.addItem(&carGraphics2);
    QGraphicsView view(&scene);
//    view.fitInView(scene.sceneRect());
    view.showMaximized();

    return a.exec();
}
