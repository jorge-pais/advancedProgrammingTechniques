#include "mainwindow.h"
#include "world.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    WorldView& view();
    World& world();
    std::shared_ptr<WorldDelegate> delegate = std::make_shared<WorldDelegate>(view, world);
    //WorldDelegate delegate(WorldView& view, World& world);
    w.addDelegate(delegate);
    w.show();
    return a.exec();
}
