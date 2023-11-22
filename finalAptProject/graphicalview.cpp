#include "graphicalview.h"

GraphicalView::GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, WorldDelegate * delegate)
{
    this->view = graphicsView;
    this->scene = scene;
    this->delegate = delegate;
    // Create the scene
    graphicsView->setScene(scene);
}

/// THIS function should be called only when loading the map i guess...
void GraphicalView::renderTiles(){
    int rows = ((WorldDelegate*)delegate)->getWorldRows();
    int cols = ((WorldDelegate*)delegate)->getWorldColumns();
    std::vector<std::unique_ptr<Tile>> worldTiles = delegate->getWorldTiles();

    // Render the map on the screen
    int tileValue;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            tileValue = worldTiles[i*cols + j]->getValue();

            // Print a rectangle there
            /// TODO this should call another function to figure out the
            /// what tile sprite to use
            scene->addRect(i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                           QPen(Qt::NoPen),
                           QBrush(QColor(255, tileValue, 0)));
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

    player->setPos(rand()%cols * TILE_SIZE, rand()%rows * TILE_SIZE);
    player->setZValue(1);
    return;
}
