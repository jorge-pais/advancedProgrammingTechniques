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

void GraphicalView::zoom(bool in){
/// true for zoom in; false for zooming out
/// void GraphicalView::zoom(bool in, float scale = SCALE_FACTOR){
    qCDebug(graphicalViewCat) << "zoom() called";
    const float factor = in ? SCALE_FACTOR : 1/SCALE_FACTOR;
    this->view->scale(factor, factor);
    centerView();
}

void GraphicalView::addOverlay(QPixmap image){
    if(!overlay && !(!image)){
        overlay = new QGraphicsPixmapItem(image);
        return;
    }
}