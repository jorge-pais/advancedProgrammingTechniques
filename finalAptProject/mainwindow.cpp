#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(0));

    // I'm very lost on why this isn't loading the picture
    // I've tried everything i know

    //QString worldPath();
    QString worldPath{":/images/resources/world_images/worldmap.png"};

    std::cout << "test, the filepath is this: " << worldPath.toStdString() << std::endl;

    /// TEST: Open the image from the path
    //std::ostringstream oss;
    //oss << "feh --zoom 1000 " << worldPath.toStdString() << " &!";
    //std::system(oss.str().c_str());

    World world{};
    world.createWorld(worldPath, 0, 0, 0.0);

    std::cout << "test?" << std::endl;

    //WorldView wView{};

    //WorldDelegate worldDelegate(&wView, &gameWorld);
    //worldDelegate.getWorldTiles();

    //QGraphicsScene scene = new QGraphicsScene(this);
    //GraphicalView gView {ui->graphicsView, scene};
}

/// THIS PART SHOULD SIGNAL TO SIGNAL THE WORLD DELEGATE
/// WHICH WOULD THEN SIGNAL THE WORLD TO UPDATE THE VIEW
void MainWindow::keyPressEvent(QKeyEvent *event){

    /// TODO Figure out why the arrow keys aren't working
    //switch(event->key()){
    //case Qt::Key_Up:
    //case Qt::Key_W:
    //    gView->player->setY(gView->player->pos().y() - TILE_SIZE);
    //    break;
    //case Qt::Key_Left:
    //case Qt::Key_A:
    //    gView->player->setX(gView->player->pos().x() - TILE_SIZE);
    //    //player->setPixmap(playerFlipped);
    //    break;
    //case Qt::Key_Down:
    //case Qt::Key_S:
    //    gView->player->setY(gView->player->pos().y() + TILE_SIZE);
    //    break;
    //case Qt::Key_Right:
    //case Qt::Key_D:
    //    gView->player->setX(gView->player->pos().x() + TILE_SIZE);
    //    //player->setPixmap(playerSprite);
    //    break;
    //}
}

MainWindow::~MainWindow()
{
    delete ui;
}
