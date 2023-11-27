#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow), // Initialize with references
    world(std::make_shared<World>()),
    wView(std::make_shared<WorldView>()),
    worldDelegate(std::make_shared<WorldDelegate>(wView, world))
{
    ui->setupUi(this);
    srand(time(0));

    /// the world isn't loading when using the worldmap.png image
    /// idk why, worldmap4.png seems to work alright
    QString worldPath{":/images/resources/world_images/worldmap.png"};

    world->createWorld(worldPath, 0, 0, 0.0);

    //std::vector<std::unique_ptr<Tile>> map = world->getTiles();

    //for(int y = 0; y < world->getRows(); y++){
    //    for(int x = 0; x < world->getCols(); x++)
    //        std::cout << map[y*world->getCols() + x]->getValue() << " ";
    //    std::cout << std::endl;
    //}

    // Initialize GraphicalView
    QGraphicsScene * scene = new QGraphicsScene();
    gView = std::make_shared<GraphicalView>(ui->graphicsView, scene, worldDelegate);

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

    /// Calling this function for a second time causes the program to crash. This is due to the
    /// backasswards way that the world library returns a vector of unique_ptr to tiles when calling
    /// getTiles(), which causes the ownership of these to be in the scope of the function we call once???
    /// How come this is an actual good way to do things??? Now it is necessary to pass a reference,
    /// or copy, or something else, and I've lost 3 hours trying to make this work
    //gView->renderTiles();
}

MainWindow::~MainWindow()
{
    delete ui;
}
