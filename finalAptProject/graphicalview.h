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
#define TEXT_OFFSET 25

/// MAKE THIS INTO ANOTHER CLASS
struct SpriteWithValue{
    QGraphicsPixmapItem * sprite;
    QGraphicsTextItem * text;

    SpriteWithValue() : sprite(nullptr), text(nullptr){}

    void setPosition(int x, int y){
        if(sprite)
            sprite->setPos(x*TILE_SIZE, y*TILE_SIZE);
        if(text)
            text->setPos(x*TILE_SIZE, y*TILE_SIZE - TEXT_OFFSET);
    }

    void setHealth(float health){
        if(text){
            text->setPlainText(QString::number(health));
        }
    }

    SpriteWithValue(std::shared_ptr<Protagonist> prog){
        QPixmap playerSprite = QPixmap(":/images/resources/entities/tux.png");
        playerSprite = playerSprite.scaled(TILE_SIZE, TILE_SIZE,
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation); // facing

        sprite = new QGraphicsPixmapItem(playerSprite);
        text = new QGraphicsTextItem(QString::number(prog->getHealth()));

        setPosition(prog->getXPos(), prog->getYPos());
        sprite->setZValue(1);
        text->setZValue(1);
    }

    SpriteWithValue(std::shared_ptr<Tile> entity){
    //SpriteWithValue(int x, int y, float value){

        QPixmap spritePixmap;

        text = new QGraphicsTextItem(QString::number(entity->getValue()));

        if(dynamic_cast<PEnemy*>(entity.get())){
            spritePixmap = QPixmap(":/images/resources/entities/captain_left-2.png");
        }else if(dynamic_cast<Enemy*>(entity.get())){
            spritePixmap = QPixmap(":/images/resources/entities/smartball-2.png");
        }else{
            spritePixmap = QPixmap(":/images/resources/entities/platter.png");
        }

        spritePixmap = spritePixmap.scaled(TILE_SIZE, TILE_SIZE,
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation); // facing

        sprite = new QGraphicsPixmapItem(spritePixmap);

        setPosition(entity->getXPos(), entity->getYPos());
        sprite->setZValue(1);
        text->setZValue(1);
    }

    ~SpriteWithValue(){
        delete sprite;
        delete text;
    }
};

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

    QGraphicsView * view;

private:
    QGraphicsScene* scene;
    //std::shared_ptr<WorldDelegate> delegate;
    std::shared_ptr<WorldView> worldView;
};

#endif // GRAPHICALVIEW_H
