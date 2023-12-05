#include "graphicalview.h"

GraphicalView::GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate) :
    view(graphicsView), scene(scene), delegate(delegate)
{   
    // Create the scene
    graphicsView->setScene(scene);
}

/// THIS function should be called only when loading the map i guess...
void GraphicalView::renderTiles(){
    std::cout << "renderTiles() called" << std::endl;

    std::vector<std::shared_ptr<Tile>> worldTiles = delegate->getWorldTiles();

    float value; int x, y;
    for(auto& tilePtr : worldTiles){ // This is how you pass the tiles by referece!
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

void GraphicalView::renderEntities(){
    std::cout << "renderEntities() called" << std::endl;

    for(const auto & enemyPtr : delegate->getWorldEnemies()){
        //std::shared_ptr<SpriteWithValue> a = std::make_shared<SpriteWithValue>(enemyPtr);
        SpriteWithValue* a = new SpriteWithValue(enemyPtr->getXPos(), enemyPtr->getYPos(), enemyPtr->getValue());
        entities.push_back(a);
        scene->addItem(a->sprite);
        scene->addItem(a->text);
    }
}

// Initialize the player, after this the render/update method should be called
void GraphicalView::renderPlayer(){
    player = new SpriteWithValue(delegate->getWorldProtagonist());
    scene->addItem(player->sprite);
    scene->addItem(player->text);
}
