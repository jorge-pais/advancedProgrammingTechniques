#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(std::make_shared<World>()),
    wView(std::make_shared<WorldView>()),
    worldDelegate(std::make_shared<WorldDelegate>(wView, world))
{
    ui->setupUi(this);
    srand(time(0));

    wView->setDelegate(worldDelegate);

    QString worldPath{":/images/resources/world_images/worldmap.png"};
    world->createWorld(worldPath, 2, 0, 0.0);


    // Initialize GraphicalView
    QGraphicsScene * scene = new QGraphicsScene();
    gView = std::make_shared<GraphicalView>(ui->graphicsView, scene, worldDelegate);

    // Connect all signal/slots
    worldDelegate->connectSlots();
    wView->connectSlots();

    // render the background tiles;
    gView->renderTiles();
    gView->renderPlayer();
    gView->renderEntities();
}

/// THIS PART SHOULD SIGNAL TO SIGNAL THE WORLD DELEGATE
/// WHICH WOULD THEN SIGNAL THE WORLD TO UPDATE THE VIEW
void MainWindow::keyPressEvent(QKeyEvent *event){

    emit mainWindowEventSignal(event);

    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_W:
        gView->player->setPosition(5, 5);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        gView->player->setPosition(1, 1);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        break;
    }

    //gView->view->centerOn(gView->player);

    //gView->renderTiles();
}

MainWindow::~MainWindow()
{
    delete ui;
}
