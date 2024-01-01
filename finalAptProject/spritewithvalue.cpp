#include "spritewithvalue.h"

SpriteWithValue::SpriteWithValue() : sprite(nullptr), text(nullptr){}

SpriteWithValue::SpriteWithValue(std::shared_ptr<Tile> entity){

    text = std::make_shared<QGraphicsTextItem>(QString::number(entity->getValue()));

    // If we could modify the world classes, this would be more efficient,
    // as a tile method could be implemented and then overloaded by each entity class
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
        spriteSet["dead"] = QPixmap(); // puff! disappear!
    }   

    spriteSet["alive"] = scaleSprite(spriteSet["alive"]);
    spriteSet["dead"] = scaleSprite(spriteSet["dead"]);

    // This will come in handy when loading the map from a serialized version and some entities might already be dead
    Enemy* aux = dynamic_cast<Enemy*>(entity.get());

    bool dead = aux ? aux->getDefeated() : !(entity.get()->getValue());

    sprite = std::make_shared<QGraphicsPixmapItem>(dead ? spriteSet["dead"] : spriteSet["alive"]);

    setPosition(entity->getXPos(), entity->getYPos());
    sprite->setZValue(2);
    text->setZValue(2);
}

int SpriteWithValue::getX() const { return x; }
int SpriteWithValue::getY() const { return y; }

void SpriteWithValue::setPosition(int x, int y){
    if(sprite)
        sprite->setPos(x*TILE_SIZE, y*TILE_SIZE);
    if(text)
        text->setPos(x*TILE_SIZE, y*TILE_SIZE - TEXT_OFFSET);

    this->x = x; this->y = y;
}

void SpriteWithValue::setDead(int spriteOffset){
    sprite->setPixmap(spriteSet["dead"]);
    sprite->setPos(x*TILE_SIZE, y*TILE_SIZE + spriteOffset);
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

QPixmap SpriteWithValue::scaleSprite(QPixmap sprite, bool stretch) {
    return sprite.scaled(
            TILE_SIZE, TILE_SIZE, 
            (stretch ? Qt::IgnoreAspectRatio : Qt::KeepAspectRatio), 
            Qt::SmoothTransformation);
}