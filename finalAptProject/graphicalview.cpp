#include "graphicalview.h"

GraphicalView::GraphicalView(QGraphicsView* qGraphicsView, QGraphicsScene * scene)
{
    // Create the scene
    qGraphicsView->setScene(scene);

    // Render the map on the screen
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            scene->addRect(i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                           QPen(Qt::NoPen),
                           QBrush(QColor(255, (rand()%20) * 10, 0)));
        }
    }

    QPixmap playerSprite = QPixmap(":/images/resources/entities/tux.png");

    //Resize the sprite
    playerSprite = playerSprite.scaled(TILE_SIZE, TILE_SIZE,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation); // facing right
    player = new QGraphicsPixmapItem(playerSprite);

    scene->addItem(player);

    // Spawn player in random position

    player->setPos(rand()%MAP_SIZE * TILE_SIZE, rand()%MAP_SIZE * TILE_SIZE);
    player->setZValue(1);
}
