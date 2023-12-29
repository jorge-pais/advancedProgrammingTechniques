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
    GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldView> worldView);

    ProtagonistSprite * player;
    std::vector<SpriteWithValue *> entities;
    std::vector<SpriteWithValue *> healthPacks;

    void renderTiles(bool useTile = false);
    void renderEntities();
    void renderPlayer();
    void centerView();
    void poisonTile(int x, int y, int poisonLevel);
    void zoom(bool in, float factor = SCALE_FACTOR);
    void setOverlay(QPixmap image);
    QPixmap getTile(float value);
    void addTileSet(float low, float high, QPixmap tile);
    void clearTiles();

private:
    QGraphicsView * view;
    QGraphicsScene * scene;
    std::shared_ptr<WorldView> worldView;
    QGraphicsPixmapItem * overlay = nullptr;

    std::vector<QGraphicsItem *> tiles;

    std::map<std::pair<float, float>, QPixmap, CompareFirstVal> tileSet = {};
};

#endif // GRAPHICALVIEW_H
