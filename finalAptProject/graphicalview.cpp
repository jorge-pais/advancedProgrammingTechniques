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

    std::vector<std::unique_ptr<Tile>> worldTiles = delegate->getWorldTiles();

    float value; int x, y;
    for(const auto& tilePtr : worldTiles){ // This is how you pass the tiles by referece!
        x = tilePtr->getXPos();
        y = tilePtr->getYPos();
        value = tilePtr->getValue();
        //std::cout << value << " ";
        scene->addRect(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                       QPen(Qt::NoPen),
                       QBrush(QColor(255*value, 255*value, 255*value)));
    }
    return;
}

void GraphicalView::addPlayer(int xStart, int yStart){
    QPixmap playerSprite = QPixmap(":/images/resources/entities/tux.png");

    //Resize the sprite
    playerSprite = playerSprite.scaled(TILE_SIZE, TILE_SIZE,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation); // facing right
    player = new QGraphicsPixmapItem(playerSprite);

    scene->addItem(player);

    // Spawn player in random position

    player->setPos(yStart, xStart);
    player->setZValue(1);
}
