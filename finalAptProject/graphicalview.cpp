#include "graphicalview.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory graphicalViewCat("graphicalView");

/// @brief Constructor for the GraphicalView class.
/// @param graphicsView The QGraphicsView object to be used.
/// @param scene The QGraphicsScene object to be used.
/// @param worldView The WorldView object to be used.
GraphicalView::GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldView> worldView) :
    view(graphicsView), scene(scene), worldView(worldView)
{   
    // Create the scene
    graphicsView->setScene(scene);
}

/// @brief Renders the tiles of the game world.
/// @param useTile If true, uses the tileset for rendering. If false, uses grayscale squares.
void GraphicalView::renderTiles(bool useTile){
    qCDebug(graphicalViewCat) << "renderTiles() called";

    std::vector<std::shared_ptr<Tile>> worldTiles = worldView->getDelegate()->getWorldTiles();

    float value; int x, y;
    for(auto& tilePtr : worldTiles){ 
        x = tilePtr->getXPos();
        y = tilePtr->getYPos();
        value = tilePtr->getValue();

        if(useTile){ // hopefully the compiler optimizes this
            auto pixTile = scene->addPixmap(getTile(value));
            pixTile->setPos(x*TILE_SIZE, y*TILE_SIZE);
            tiles.push_back(pixTile);
        }
        else{ // print background tiles in greyscale squares
            auto rect = scene->addRect(
                    x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                    QPen(Qt::NoPen),
                    QBrush(QColor(255*value, 255*value, 255*value)));
            tiles.push_back(rect);
        }
    }

    return;
}

/// @brief Clears all tiles from the scene.
void GraphicalView::clearTiles(){
    for(auto tile : tiles)
        scene->removeItem(tile);
    tiles.clear();
    for(auto tile : poisonTiles)
        scene->removeItem(tile);
    poisonTiles.clear();
}

/// @brief Clears all entities and healthpacks from the scene
void GraphicalView::clearEntities(){
    for(auto entity : entities){
        scene->removeItem(entity->sprite.get());
        scene->removeItem(entity->text.get());
    }

    for(auto pack : healthPacks){
        scene->removeItem(pack->sprite.get());
        scene->removeItem(pack->text.get());
    }

    entities.clear();
    healthPacks.clear();
}

/// @brief Applies a "poison" effect to a tile.
/// @param x The x-coordinate of the tile.
/// @param y The y-coordinate of the tile.
/// @param poisonLevel The intensity of the poison effect.
void GraphicalView::poisonTile(int x, int y, int poisonLevel){
    QGraphicsRectItem * rect = scene->addRect(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                QPen(Qt::NoPen),
                QBrush(QColor(255-poisonLevel,80, 80, 127)));

    poisonTiles.push_back(rect);
    rect->setZValue(3);
}

void GraphicalView::renderPoisonTiles(){
    for(const auto& tile : this->worldView->getDelegate()->getPoisonTiles()){
        poisonTile(tile->getXPos(), tile->getYPos(), tile->getValue());
    }
}

/// @brief Renders the entities in the game world.
void GraphicalView::renderEntities(){
    qCDebug(graphicalViewCat) << "renderEntities() called";

    for(const auto & enemyPtr : worldView->getDelegate()->getWorldEnemies()){
        //std::shared_ptr<SpriteWithValue> a = std::make_shared<SpriteWithValue>(enemyPtr);
        SpriteWithValue* a = new SpriteWithValue(enemyPtr);
        entities.push_back(a);
        scene->addItem(a->sprite.get());
        scene->addItem(a->text.get());
    }

    for(const auto & healthPtr : worldView->getDelegate()->getWorldHealthPacks()){
        //std::shared_ptr<SpriteWithValue> a = std::make_shared<SpriteWithValue>(enemyPtr);
        SpriteWithValue* a = new SpriteWithValue(healthPtr);
        healthPacks.push_back(a);
        scene->addItem(a->sprite.get());
        scene->addItem(a->text.get());
    }
}

/// @brief Renders the door entity on the scene.
void GraphicalView::renderDoor(){
    auto sprite = QPixmap(":/images/resources/entities/door.png");
    sprite = sprite.scaled(
            TILE_SIZE, TILE_SIZE,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation);
    door = new QGraphicsPixmapItem(sprite);
    int x = this->worldView->getDelegate()->getDoor()->getXPos();
    int y = this->worldView->getDelegate()->getDoor()->getYPos();
    door->setPos(x*TILE_SIZE, y*TILE_SIZE);

    scene->addItem(door);
}

/// @brief Removes the door from the scene.
void GraphicalView::clearDoor(){
    scene->removeItem(door);
}

/// @brief Removes the player character from the scene.
void GraphicalView::clearPlayer(){
    scene->removeItem(player->energyBar.get());
    scene->removeItem(player->sprite.get());
    scene->removeItem(player->text.get());
    player = NULL;
}
/// @brief Renders the player character.
void GraphicalView::renderPlayer(){
    // Initialize the player, after this the render/update method should be called
    qCDebug(graphicalViewCat) << "renderPlayer() called";
    player = new ProtagonistSprite(worldView->getDelegate()->getWorldProtagonist());
    scene->addItem(player->sprite.get());
    scene->addItem(player->text.get());
    scene->addItem(player->energyBar.get());
}

/// @brief Centers the view on the player character.
void GraphicalView::centerView(){
    qCDebug(graphicalViewCat) << "centerView() called";
    this->view->centerOn(this->player->sprite.get());
}

/// @brief Zooms the view in or out.
/// @param in If true, zooms in. If false, zooms out.
/// @param factor The zoom factor.
void GraphicalView::zoom(bool in, float factor){
/// true for zoom in; false for zooming out
    qCDebug(graphicalViewCat) << "zoom() called";
    const float f = in ? factor : 1 / factor;
    this->view->scale(f, f);
    centerView();
}

/// @brief Adds a tile to the tileset
/// @param low The lower bound of the tile's range.
/// @param high The upper bound of the tile's range.
/// @param tile The QPixmap object representing the tile
void GraphicalView::addTileSet(float low, float high, QPixmap tile){
    // tuple notation from C++11
    qCDebug(graphicalViewCat) << "addTileSet() called";

    for(const auto& tile : tileSet){
        if((low > tile.first.first && low < tile.first.second) ||
        (high > tile.first.first && high < tile.first.second)){
            qCWarning(graphicalViewCat) << "addTileSet() - range provided for tile overlaps with existing";
            return;
        }
    }

    QPixmap newTile = tile.scaled(TILE_SIZE, TILE_SIZE);

    tileSet[{low, high}] = newTile; // add the tile if successful
}

/// @brief Removes all tile sprites from the tileset.
void GraphicalView::clearTileSet(){
    tileSet.clear();
}

/// @brief Returns the tile from the tileset corresponding to a given value.
/// @param value The value to be used for determining the tile.
/// @return QPixmap object representing the tile.
QPixmap GraphicalView::getTile(float value){
    qDebug(graphicalViewCat) << value;
    for(const auto & sprite: tileSet) // determine the tile
        if (value >= sprite.first.first && value < sprite.first.second)
            return sprite.second;

    return QPixmap("");
}

/// @brief Sets an overlay image for the game world.
/// @param image The QPixmap object representing the overlay image.
void GraphicalView::setOverlay(QPixmap image){
    qCDebug(graphicalViewCat) << "setOverlay() called";

    if(overlay) // remove the current overlay
        scene->removeItem(overlay);
    
    if(image.isNull()) return;

    int h = worldView->getDelegate()->getWorldColumns()*TILE_SIZE;
    int w = worldView->getDelegate()->getWorldRows()*TILE_SIZE;
    QPixmap img = image.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    overlay = new QGraphicsPixmapItem(img);
    overlay->setZValue(2); //above the tiles, below the entity sprites
    
    if(overlayOn)
        scene->addItem(overlay);
}

/// @brief Toggle the overlay over the game scene.
/// @param show Display/hide.
void GraphicalView::toggleOverlay(bool show){
    overlayOn = show;
    if(overlayOn)
        scene->addItem(overlay);
    else
        scene->removeItem(overlay);
}

/// @brief Creates a pathfinding tile and adds it to the scene.
/// @param x The x-coordinate of the tile.
/// @param y The y-coordinate of the tile.
void GraphicalView::pathTile(int x, int y){
    QGraphicsItem* rect = scene->addRect(
                (x+0.2)*TILE_SIZE, (y+0.2)*TILE_SIZE, 
                0.6*TILE_SIZE, 0.6*TILE_SIZE,
                QPen(Qt::NoPen),
                QBrush(QColor(255, 153, 51, 127)));

    rect->setZValue(3);
    path.push_back(rect);
}

/// @brief Remove all pathfinding tiles from the scene.
void GraphicalView::clearPath(){
    for(auto tile: path)
        scene->removeItem(tile);
}
