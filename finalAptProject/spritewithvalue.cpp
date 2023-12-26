#include "spritewithvalue.h"

SpriteWithValue::SpriteWithValue() : sprite(nullptr), text(nullptr){}

SpriteWithValue::SpriteWithValue(std::shared_ptr<Tile> entity){

    text = new QGraphicsTextItem(QString::number(entity->getValue()));

    if(dynamic_cast<XEnemy*>(entity.get())){
        spriteSet["alive"] = QPixmap(":/images/resources/entities/captain_left-2.png");
        spriteSet["dead"] = QPixmap(":/images/resources/entities/cpt-squished-left.png");
    }else if(dynamic_cast<PEnemy*>(entity.get())){
        spriteSet["alive"] = QPixmap(":/images/resources/entities/smartball-2.png");
        spriteSet["dead"] = QPixmap(":/images/resources/entities/mrs-squished-left.png");
    }else if(dynamic_cast<Enemy*>(entity.get())){
        spriteSet["alive"] = QPixmap(":/images/resources/entities/snowball-2.png");
        spriteSet["dead"] = QPixmap(":/images/resources/entities/squished-left.png");
    }else{
        spriteSet["alive"] = QPixmap(":/images/resources/entities/platter.png");
        spriteSet["dead"] = QPixmap(); // disappear!
    }   

    spriteSet["alive"] = scaleSprite(spriteSet["alive"]);
    spriteSet["dead"] = scaleSprite(spriteSet["dead"]);

    // This will come in handy when loading the map from a serialized version and some entities might already be dead
    Enemy* aux = dynamic_cast<Enemy*>(entity.get());
    bool dead;
    if(aux)
        dead = aux->getDefeated();
    else
        dead = !(entity.get()->getValue());

    sprite = new QGraphicsPixmapItem(dead ? spriteSet["dead"] : spriteSet["alive"]);

    setPosition(entity->getXPos(), entity->getYPos());
    sprite->setZValue(2);
    text->setZValue(2);
}

int SpriteWithValue::getX() const { return x; }
int SpriteWithValue::getY() const { return y; }

/// TODO remove this and every other dumb pointer
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
    sprite->setPixmap(spriteSet["dead"]);
    sprite->setPos(x*TILE_SIZE, y*TILE_SIZE + 25);
    text->setPlainText("");
}

void SpriteWithValue::setAlive(float health){
    sprite->setPixmap(spriteSet["alive"]);
    text->setPlainText(QString::number(health));
    text->setZValue(2);
}

void SpriteWithValue::setHealth(float health){
    if(text) text->setPlainText(QString::number(health));
}

QPixmap SpriteWithValue::scaleSprite(QPixmap sprite){
    return sprite.scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio,Qt::SmoothTransformation);
}