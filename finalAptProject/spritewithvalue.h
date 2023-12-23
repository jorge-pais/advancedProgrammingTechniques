#ifndef SPRITEWITHVALUE_H
#define SPRITEWITHVALUE_H

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
#include "world.h"

#define TILE_SIZE 40
#define TEXT_OFFSET 25

class SpriteWithValue
{
public:
    SpriteWithValue();
    SpriteWithValue(std::shared_ptr<Protagonist> prog);
    SpriteWithValue(std::shared_ptr<Tile> entity);
    ~SpriteWithValue();

    void setHealth(float health);
    void setDead();
    void setPosition(int x, int y);

    QGraphicsPixmapItem * sprite;
    QGraphicsTextItem * text;
    QPixmap spriteDeadPixmap;
    int x, y;
};

#endif // SPRITEWITHVALUE_H
