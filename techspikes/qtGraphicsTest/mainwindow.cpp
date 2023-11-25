#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TILE_SIZE 40

/// TODO FIND A SOLUTION TO FETCH THIS TYPE OF RESOURCE
/// AND MAYBE INCLUDE IT IN THE FINAL BINARY

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(0));

    World gameWorld{};
    gameWorld.createWorld("/Documents/QtGame/team-d6-fa/techspikes/qtGraphicsTest/resources$");

    Map map;
    map.generateRandomMap();

    // Create the scene
    QGraphicsScene *scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    // Render the map on the screen
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            scene->addRect(i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                           QPen(Qt::NoPen),
                           QBrush(QColor(255, map.mapArray[i][j]*10, 0)));
        }
    }

    QPixmap playerSprite = QPixmap("/home/jorgep/repos/team-d6-fa/techspikes/qtGraphicsTest/resources/tux.png");

    //Resize the sprite
    playerSprite = playerSprite.scaled(TILE_SIZE, TILE_SIZE,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation); // facing right
    player = new QGraphicsPixmapItem(playerSprite);

    scene->addItem(player); player->setZValue(1);

    // Spawn player in random position
    player->setPos(rand()%MAP_SIZE * TILE_SIZE, rand()%MAP_SIZE * TILE_SIZE);
}

void MainWindow::keyPressEvent(QKeyEvent *event){

    /// TODO Figure out why the arrow keys aren't working
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_W:
        player->setY(player->pos().y() - TILE_SIZE);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        player->setX(player->pos().x() - TILE_SIZE);
        //player->setPixmap(playerFlipped);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        player->setY(player->pos().y() + TILE_SIZE);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        player->setX(player->pos().x() + TILE_SIZE);
        //player->setPixmap(playerSprite);
        break;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

