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

#define TILE_SIZE 40 ///TODO this define is different across files
#define TEXT_OFFSET 25

struct SpriteWithValue{
    QGraphicsPixmapItem *sprite;
    QGraphicsTextItem *text;
    Tile* tile;

    SpriteWithValue() : sprite(nullptr), text(nullptr), tile(nullptr){}

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

    SpriteWithValue(std::unique_ptr<Protagonist> prog){
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

    SpriteWithValue(int x, int y, float value){
        QPixmap enemySprite = QPixmap(":/images/resources/entities/smartball-2.png");

        //if(dynamic_cast<PEnemy*>(entity))
        //    enemySprite = QPixmap(":/images/resources/entities/tux.png");
        //else
        //enemySprite = QPixmap(":/images/resources/entities/smartball-2.png");

        enemySprite = enemySprite.scaled(TILE_SIZE, TILE_SIZE,
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation); // facing

        sprite = new QGraphicsPixmapItem(enemySprite);
        text = new QGraphicsTextItem(QString::number(value));

        setPosition(x, y);
        sprite->setZValue(1);
        text->setZValue(1);
    }

    ~SpriteWithValue(){
        delete sprite;
        delete text;
    }
};

class WorldDelegate;

/// TODO: WE SHOULD REFACTOR THIS CLASS NAME INTO
/// SOMETHING DIFFERENT FROM THE QT CLASSES
class GraphicalView
{
public:
    GraphicalView(QGraphicsView* graphicsView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate);

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
    std::shared_ptr<WorldDelegate> delegate;
};

#endif // GRAPHICALVIEW_H
