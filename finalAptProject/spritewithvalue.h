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
#include <QTimer>
#include <QGraphicsColorizeEffect>
#include <ctime>
#include <iostream>
#include <unordered_map>

#include "world.h"
#include "xenemy.h"

#define TILE_SIZE 40
#define TEXT_OFFSET 15

class SpriteWithValue
: public QObject{ // public inheritance !!
public:
    SpriteWithValue();

    SpriteWithValue(std::shared_ptr<Protagonist> prog);
    SpriteWithValue(std::shared_ptr<Tile> entity);
    ~SpriteWithValue();

    void setHealth(float health);
    void setDead();
    void setAlive(float health);
    virtual void setPosition(int x, int y);
    int getX() const;
    int getY() const;
    QPixmap scaleSprite(QPixmap sprite);

    QGraphicsPixmapItem * sprite;
    QGraphicsTextItem * text;
    std::unordered_map<std::string, QPixmap> spriteSet = {};

protected:
    int x, y;
};

#endif // SPRITEWITHVALUE_H
