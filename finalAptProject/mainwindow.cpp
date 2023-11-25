#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // Initialize with references
{
    ui->setupUi(this);
    srand(time(0));

    World world{};
    World * r_world{&world};
    WorldView wView{};
    WorldView* r_wView{&wView};
    WorldDelegate worldDelegate(r_wView, r_world);
    WorldDelegate * r_worldDelegate{&worldDelegate};


    /// the world isn't loading when using the worldmap.png image
    /// idk why, worldmap4.png seems to work alright
    QString worldPath{":/images/resources/world_images/worldmap.png"};

    world.createWorld(worldPath, 0, 0, 0.0);

    // Initialize GraphicalView
    QGraphicsScene * scene = new QGraphicsScene();
    gView = new GraphicalView(ui->graphicsView, scene, r_worldDelegate);

    gView->renderTiles();
}

/// THIS PART SHOULD SIGNAL TO SIGNAL THE WORLD DELEGATE
/// WHICH WOULD THEN SIGNAL THE WORLD TO UPDATE THE VIEW
void MainWindow::keyPressEvent(QKeyEvent *event){

    emit mainWindowEventSignal(event);
    /// TODO Figure out why the arrow keys aren't working
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_W:
        gView->player->setY(gView->player->pos().y() - TILE_SIZE);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        gView->player->setX(gView->player->pos().x() - TILE_SIZE);
        //player->setPixmap(playerFlipped);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        gView->player->setY(gView->player->pos().y() + TILE_SIZE);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        gView->player->setX(gView->player->pos().x() + TILE_SIZE);
        //player->setPixmap(playerSprite);
        break;
    }
    gView->view->centerOn(gView->player);
}

MainWindow::~MainWindow()
{
    delete ui;
}
