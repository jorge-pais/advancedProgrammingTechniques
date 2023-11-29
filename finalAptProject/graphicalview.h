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

#define TILE_SIZE 20

/// TODO: WE SHOULD REFACTOR THIS CLASS NAME INTO
/// SOMETHING DIFFERENT FROM THE QT CLASSES

class WorldDelegate;

class GraphicalView
{
public:
    GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate);

    QGraphicsPixmapItem * player;

    void renderTiles();
    void addPlayer(int x, int y);
    //void addEntities();

    QGraphicsView * view;

private:
    QGraphicsScene* scene;
    std::shared_ptr<WorldDelegate> delegate;

};

#endif // GRAPHICALVIEW_H
