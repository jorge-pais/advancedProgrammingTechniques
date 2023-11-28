#ifndef GRAPHICALVIEW_H
#define GRAPHICALVIEW_H

#include "worldview.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>
#include <ctime>

#define MAP_SIZE 20
#define TILE_SIZE 40

class GraphicalView : public WorldView
{
public:
    GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene);
    QGraphicsPixmapItem * player;
private:
    //QGraphicsPixmapItem ** enemies; // Replace this be with a smart pointer to the array
};

#endif // GRAPHICALVIEW_H
