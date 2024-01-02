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
#include <memory>
#include <unordered_map>

#include "world.h"
#include "xenemy.h"

#define TEXT_OFFSET 15
#define TILE_SIZE 40
#define DEAD_OFFSET 25

class SpriteWithValue
: public QObject{ // public inheritance !!
public:
   SpriteWithValue();
   SpriteWithValue(std::shared_ptr<Protagonist> prog);
   SpriteWithValue(std::shared_ptr<Tile> entity);

   ~SpriteWithValue() = default;

   void setHealth(float health);
   void setDead(int spriteOffset = DEAD_OFFSET);
   void setAlive(float health);
   virtual void setPosition(int x, int y);
   int getX() const;
   int getY() const;

   static QPixmap scaleSprite(QPixmap sprite, bool stretch = false);

   std::shared_ptr<QGraphicsPixmapItem> sprite;
   std::shared_ptr<QGraphicsTextItem> text;
   std::unordered_map<std::string, QPixmap> spriteSet = {};

protected:
   int x, y;
};

#endif // SPRITEWITHVALUE_H
