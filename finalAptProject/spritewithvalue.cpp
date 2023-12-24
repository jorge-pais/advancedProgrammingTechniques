#include "spritewithvalue.h"
#include "xenemy.h"

SpriteWithValue::SpriteWithValue() : sprite(nullptr), text(nullptr){}

SpriteWithValue::SpriteWithValue(std::shared_ptr<Protagonist> prog){
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

SpriteWithValue::SpriteWithValue(std::shared_ptr<Tile> entity){
    //SpriteWithValue(int x, int y, float value){

    text = new QGraphicsTextItem(QString::number(entity->getValue()));

    if(dynamic_cast<XEnemy*>(entity.get())){
        spritePixmap = QPixmap(":/images/resources/entities/captain_left-2.png");
        spriteDeadPixmap = QPixmap(":/images/resources/entities/cpt-squished-left.png");
    }
    else if(dynamic_cast<PEnemy*>(entity.get())){
        spritePixmap = QPixmap(":/images/resources/entities/smartball-2.png");
        spriteDeadPixmap = QPixmap(":/images/resources/entities/mrs-squished-left.png");
    }
    else if(dynamic_cast<Enemy*>(entity.get())){
        spritePixmap = QPixmap(":/images/resources/entities/snowball-2.png");
        spriteDeadPixmap = QPixmap(":/images/resources/entities/squished-left.png");
    }
    else{ // Healthpack
        spritePixmap = QPixmap(":/images/resources/entities/platter.png");
        spriteDeadPixmap = QPixmap();
    }

    spritePixmap = spritePixmap.scaled(TILE_SIZE, TILE_SIZE,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation); // facing

    spriteDeadPixmap = spriteDeadPixmap.scaled(TILE_SIZE, TILE_SIZE,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation); // facing

    sprite = new QGraphicsPixmapItem(spritePixmap);

    setPosition(entity->getXPos(), entity->getYPos());
    sprite->setZValue(1);
    text->setZValue(1);
}

//int getX() const { return x; }
//int getY() const { return y; }

SpriteWithValue::~SpriteWithValue(){
    delete sprite;
    delete text;
}

void SpriteWithValue::setPosition(int x, int y){
    if(sprite)
        sprite->setPos(x*TILE_SIZE, y*TILE_SIZE);
    if(text)
        text->setPos(x*TILE_SIZE, y*TILE_SIZE - TEXT_OFFSET);

    this->x = x; this->y = y;
}

void SpriteWithValue::setDead(){
    sprite->setPixmap(spriteDeadPixmap);
    sprite->setPos(x*TILE_SIZE, y*TILE_SIZE + 25);
    text->setPlainText("");
}

void SpriteWithValue::setAlive(float health){
    sprite->setPixmap(spritePixmap);
    text->setPlainText(QString::number(health));
    text->setZValue(1);
}

void SpriteWithValue::setHealth(float health){
    if(text){
        text->setPlainText(QString::number(health));
    }
}