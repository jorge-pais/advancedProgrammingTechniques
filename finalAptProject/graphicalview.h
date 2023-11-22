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

#define TILE_SIZE 40

/// TODO: WE SHOULD REFACTOR THIS CLASS NAME INTO
/// SOMETHING DIFFERENT FROM THE QT CLASSES

class GraphicalView
{
public:
    GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, WorldDelegate * delegate);

    QGraphicsPixmapItem * player;

    void renderTiles();

private:
    QGraphicsView * view;
    QGraphicsScene* scene;
    WorldDelegate * delegate;

};

#endif // GRAPHICALVIEW_H
