#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <carmodel.h>
#include <envcontroller.h>
#include <initialconditions.h>
#include <qcargraphics.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene scene;
    std::shared_ptr<InitialConditions> initialConditions = std::make_shared<InitialConditions>();
    EnvController envCtrl(initialConditions, &scene);
    envCtrl.init();
    scene.setSceneRect(0, 0, initialConditions->laneLength + initialConditions->distanceToNext, 100);
    QGraphicsView view;
    view.fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
    view.setScene(&scene);
    view.show();

    return a.exec();
}
