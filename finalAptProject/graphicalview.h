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
//class WorldDelegate;

class GraphicalView
{
public:
    GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldView> worldView);

    ProtagonistSprite * player;
    std::vector<SpriteWithValue *> entities;
    std::vector<SpriteWithValue *> healthPack;

    void renderTiles();
    void renderEntities();
    void renderPlayer();
    void centerView();
    void poisonTile(int x, int y, int poisonLevel);
    void zoom(bool in);
    void addOverlay(QPixmap image);

private:
    QGraphicsView * view;
    QGraphicsScene* scene;
    //std::shared_ptr<WorldDelegate> delegate;
    std::shared_ptr<WorldView> worldView;
    QGraphicsPixmapItem * overlay = nullptr;

};

#endif // GRAPHICALVIEW_H
