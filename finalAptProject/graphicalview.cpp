#include "graphicalview.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory graphicalViewCat("graphicalView");

/// TODO:
///  - Create a load tileset method
///  - modify render tiles to use a tile a set also
///  - load overlay and toggle it (perhaps a keybind!)

GraphicalView::GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldView> worldView) :
    view(graphicsView), scene(scene), worldView(worldView)
{   
    // Create the scene
    graphicsView->setScene(scene);
}

void GraphicalView::renderTiles(){
    qCDebug(graphicalViewCat) << "renderTiles() called";

    std::vector<std::shared_ptr<Tile>> worldTiles = worldView->getDelegate()->getWorldTiles();

    float value; int x, y;
    for(auto& tilePtr : worldTiles){ // This is how you pass the tiles by referece!
        x = tilePtr->getXPos();
        y = tilePtr->getYPos();
        value = tilePtr->getValue();
        //std::cout << value << " ";

        /// This prints all tiles in grey scale,
        /// there should be an option to configure a tile set from different ranges
        scene->addRect(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE,
                       QPen(Qt::NoPen),
                       QBrush(QColor(255*value, 255*value, 255*value)));
    }

    //setOverlay(QPixmap("/home/jorgep/Pictures/a.jpg"));
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

void GraphicalView::renderPlayer(){
    // Initialize the player, after this the render/update method should be called
    qCDebug(graphicalViewCat) << "renderPlayer() called";
    player = new ProtagonistSprite(worldView->getDelegate()->getWorldProtagonist());
    scene->addItem(player->sprite);
    scene->addItem(player->text);
    scene->addItem(player->energyBar);
}

void GraphicalView::centerView(){
    qCDebug(graphicalViewCat) << "centerView() called";
    this->view->centerOn(this->player->sprite);
}

void GraphicalView::zoom(bool in, float factor){
/// true for zoom in; false for zooming out
    qCDebug(graphicalViewCat) << "zoom() called";
    const float f = in ? factor : 1 / factor;
    this->view->scale(f, f);
    centerView();
}

/// TODO: so this overlay is pretty basic, we should probably do some checks
void GraphicalView::setOverlay(QPixmap image){
    qCDebug(graphicalViewCat) << "setOverlay() called";
    if(overlay) // remove the current overlay
        scene->removeItem(overlay);

    overlay = new QGraphicsPixmapItem(image);
    overlay->setZValue(2); //above the tiles, below the entity sprites

    scene->addItem(overlay);
}

void GraphicalView::addTileSet(float low, float high, QPixmap tile){
    // tuple notation from C++11
    qCDebug(graphicalViewCat) << "addTileSet() called";

    for(const auto& tile : tileSet){
        if((low > tile.first.first && low < tile.first.second) ||
        (high > tile.first.first && high > tile.first.second)){
            qCWarning(graphicalViewCat) << "addTileSet() - range provided for tile overlaps with existing";
            return;
        }
    }

    tileSet[{low, high}] = tile; // add the tile if successful
}