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
    world->createWorld(worldPath, 0, 0, 0.0);


    // Initialize GraphicalView
    QGraphicsScene * scene = new QGraphicsScene();
    gView = std::make_shared<GraphicalView>(ui->graphicsView, scene, worldDelegate);

    worldDelegate->connectSlots();
    wView->connectSlots();


    gView->renderTiles();
}

/// THIS PART SHOULD SIGNAL TO SIGNAL THE WORLD DELEGATE
/// WHICH WOULD THEN SIGNAL THE WORLD TO UPDATE THE VIEW
void MainWindow::keyPressEvent(QKeyEvent *event){

    emit mainWindowEventSignal(event);

    //gView->view->centerOn(gView->player);

    //gView->renderTiles();
}

MainWindow::~MainWindow()
{
    delete ui;
}
