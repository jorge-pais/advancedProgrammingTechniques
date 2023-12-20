#include "graphicalview.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory graphicalViewCat("graphicalView");

GraphicalView::GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldView> worldView) :
    view(graphicsView), scene(scene), worldView(worldView)
{   
    // Create the scene
    graphicsView->setScene(scene);
}

/// THIS function should be called only when loading the map i guess...
void GraphicalView::renderTiles(){
    qCDebug(graphicalViewCat) << "renderTiles() called";

    std::vector<std::shared_ptr<Tile>> worldTiles = worldView->getDelegate()->getWorldTiles();

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

void GraphicalView::poisonTile(int x, int y, int poisonLevel){
    scene->addRect(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE,
               QPen(Qt::NoPen),
               QBrush(QColor(255-poisonLevel,80, 80)));
}

void GraphicalView::renderEntities(){
    qCDebug(graphicalViewCat) << "renderEntities() called";

    for(const auto & enemyPtr : worldView->getDelegate()->getWorldEnemies()){
        //std::shared_ptr<SpriteWithValue> a = std::make_shared<SpriteWithValue>(enemyPtr);
        SpriteWithValue* a = new SpriteWithValue(enemyPtr);
        entities.push_back(a);
        scene->addItem(a->sprite);
        scene->addItem(a->text);
    }

    for(const auto & healthPtr : worldView->getDelegate()->getWorldHealthPacks()){
        //std::shared_ptr<SpriteWithValue> a = std::make_shared<SpriteWithValue>(enemyPtr);
        SpriteWithValue* a = new SpriteWithValue(healthPtr);
        entities.push_back(a);
        scene->addItem(a->sprite);
        scene->addItem(a->text);
    }
}

// Initialize the player, after this the render/update method should be called
void GraphicalView::renderPlayer(){
    qCDebug(graphicalViewCat) << "renderPlayer() called";
    player = new SpriteWithValue(worldView->getDelegate()->getWorldProtagonist());
    scene->addItem(player->sprite);
    scene->addItem(player->text);
}
