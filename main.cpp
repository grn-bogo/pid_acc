#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <carmodel.h>
#include <envcontroller.h>
#include <qcargraphics.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene scene;
    EnvController envCtrl(scene);
    envCtrl.init();
    scene.setSceneRect(0, 0, 1820, 100);
    QGraphicsView view;
    view.fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
    view.setScene(&scene);
    view.show();

    return a.exec();
}
