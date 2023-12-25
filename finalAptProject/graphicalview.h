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

//#define TILE_SIZE 40
//#define TEXT_OFFSET 15
#define SCALE_FACTOR 1.25

class WorldView;
class WorldDelegate;

/// TODO: WE SHOULD REFACTOR THIS CLASS NAME INTO
/// SOMETHING DIFFERENT FROM THE QT CLASSES
class GraphicalView
{
public:
    GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldView> worldView);

    SpriteWithValue * player;
    std::vector<SpriteWithValue *> entities;
    std::vector<SpriteWithValue *> healthPack;

    void renderTiles();
    void renderEntities();
    void renderPlayer();
    void centerView();
    void poisonTile(int x, int y, int poisonLevel);

    void zoom(bool in);

private:
    QGraphicsView * view;
    QGraphicsScene* scene;
    //std::shared_ptr<WorldDelegate> delegate;
    std::shared_ptr<WorldView> worldView;

};

#endif // GRAPHICALVIEW_H
