#ifndef GRAPHICALVIEW_H
#define GRAPHICALVIEW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QTransform>
#include <ctime>
#include <iostream>

#include "xenemy.h"
#include "worlddelegate.h"
#include "spritewithvalue.h"
#include "protagonistsprite.h"

#define SCALE_FACTOR 1.25

class WorldView;

// Custom comparator for the tile values
/// perhaps change this to a lambda 
struct CompareFirstVal{
    bool operator()(const std::pair<float, float> & op1, const std::pair<float, float> & op2) const{
        return op1.first < op2.first;
    }
};

class GraphicalView
{
public:
    GraphicalView(QGraphicsView* graphicsView, std::shared_ptr<WorldView> worldView);
    ~GraphicalView();

    std::shared_ptr<ProtagonistSprite> player;
    std::vector<std::shared_ptr<SpriteWithValue>> entities;
    std::vector<std::shared_ptr<SpriteWithValue>> healthPacks;
    std::vector<QGraphicsItem *> path;

    void renderTiles();
    void renderEntities();
    void renderPlayer();
    void centerView();
    void poisonTile(int x, int y, int poisonLevel);
    void zoom(bool in, float factor = SCALE_FACTOR);
    QPixmap getTile(float value);
    void addTileSet(float low, float high, QPixmap tile);
    
    void setOverlay(std::string image);
    void toggleOverlay(bool v);
    void toggleTiles(bool useTile = false);

    void clearTileSet();
    void clearTiles();
    void clearEntities();
    void clearPlayer();
    void clearDoors();
    void renderDoors();
    void renderPoisonTiles();
    
    void pathTile(int x, int y);
    void clearPath();

private:
    QGraphicsView * view;
    QGraphicsScene * scene;
    std::shared_ptr<WorldView> worldView;
    QGraphicsPixmapItem * overlay = nullptr;
    bool overlayOn = false;
    bool tileOn = false;
    std::vector<QGraphicsPixmapItem *> doors;

    std::vector<QGraphicsItem *> tiles;
    std::vector<QGraphicsItem *> poisonTiles;

    // this might have been a convoluted way to do this, since we 
    // could have simply stored the lowest value in the range. oh well.
    std::map<std::pair<float, float>, QPixmap, CompareFirstVal> tileSet = {};
};

#endif // GRAPHICALVIEW_H
