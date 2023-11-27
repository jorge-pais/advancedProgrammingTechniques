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

    std::cout << "renderTiles() called" << std::endl;

    int rows = delegate->getWorldRows();
    int cols = delegate->getWorldColumns();

    auto& worldTiles = delegate->getWorldTiles();

    float value; int x, y;
    for(const auto& tilePtr : worldTiles){
        x = tilePtr->getXPos();
        y = tilePtr->getYPos();
        value = tilePtr->getValue();

        //std::cout << value << " ";

        scene->addRect(y*TILE_SIZE, x*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                       QPen(Qt::NoPen),
                       QBrush(QColor(255*value, 255*value, 255*value)));

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
