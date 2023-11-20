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
    World* gameWorld = new World();

    QString worldPath{":/images/resources/world_images/worldmap.png"};

    gameWorld->createWorld(worldPath, 5, 2, 0.25);

    QGraphicsScene *scene = new QGraphicsScene(this);
    this->gView = new GraphicalView(ui->graphicsView, scene);

}


/// THIS PART SHOULD SIGNAL TO SIGNAL THE WORLD DELEGATE
void MainWindow::keyPressEvent(QKeyEvent *event){

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

