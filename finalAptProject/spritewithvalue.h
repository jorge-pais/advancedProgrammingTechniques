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
#include <ctime>
#include <iostream>
#include <unordered_map>

#include "world.h"
#include "xenemy.h"

#define TILE_SIZE 40
#define TEXT_OFFSET 15

class SpriteWithValue
: QObject{
public:
    SpriteWithValue();

    SpriteWithValue(std::shared_ptr<Protagonist> prog);
    SpriteWithValue(std::shared_ptr<Tile> entity);
    ~SpriteWithValue();

    void setHealth(float health);
    void setDead();
    void setAlive(float health);
    void setPosition(int x, int y);
    void animate(QPixmap start, QPixmap end, float time);
    int getX() const;
    int getY() const;

    QGraphicsPixmapItem * sprite;
    QGraphicsTextItem * text;
    std::unordered_map<std::string, QPixmap> spriteSet = {};

private:
    std::function<QPixmap(QPixmap)> scaleSprite = [](QPixmap sprite) { return sprite.scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio,Qt::SmoothTransformation); };

    int x, y;
};

#endif // SPRITEWITHVALUE_H
