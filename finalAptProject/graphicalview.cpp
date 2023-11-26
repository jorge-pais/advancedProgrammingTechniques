#include "graphicalview.h"

GraphicalView::GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate)
{
    this->view = graphicsView;
    this->scene = scene;
    this->delegate = delegate;
    // Create the scene
    graphicsView->setScene(scene);
}

/// THIS function should be called only when loading the map i guess...
void GraphicalView::renderTiles(){
    int rows = delegate->getWorldRows();
    int cols = delegate->getWorldColumns();
    std::vector<std::unique_ptr<Tile>> worldTiles = delegate->getWorldTiles()d;

    std::cout << "renderTiles() called" << std::endl;
    // Render the map on the screen
    float tileValue;
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            tileValue = worldTiles[y*rows + x]->getValue();

            //std::cout << tileValue << " ";
            /// TODO this should call another function to figure out the
            /// what tile sprite to use
            scene->addRect(y*TILE_SIZE, x*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                           QPen(Qt::NoPen),
                           QBrush(QColor(255*tileValue, 255*tileValue, 255*tileValue)));
        }
        std::cout << std::endl;
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

//void GraphicalView::renderPlayer(){
//
//}
//
//void GraphicalView::renderEntities(){
//
//}


