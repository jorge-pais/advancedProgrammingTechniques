#ifndef GRAPHICALVIEW_H
#define GRAPHICALVIEW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>
#include <ctime>
#include <iostream>

#include "worlddelegate.h"
#include "spritewithvalue.h"

#define TILE_SIZE 40
#define TEXT_OFFSET 25

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

    // THESE METHODS ALL SAY READ, BUT ARE IN FACT JUST TO ADD THE SPRITES
    void renderTiles();
    void renderEntities();
    void renderPlayer();
    void poisonTile(int x, int y, int poisonLevel);

    QGraphicsView * view;

private:
    QGraphicsScene* scene;
    //std::shared_ptr<WorldDelegate> delegate;
    std::shared_ptr<WorldView> worldView;
};

#endif // GRAPHICALVIEW_H
