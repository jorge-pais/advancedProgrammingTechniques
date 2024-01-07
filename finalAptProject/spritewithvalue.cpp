#include "spritewithvalue.h"

/// @brief Default constructor, not really used.
SpriteWithValue::SpriteWithValue() : sprite(nullptr), text(nullptr){}

/// @brief Constructor that takes a shared pointer to a Tile.
/// @param entity A shared pointer to a Tile.
SpriteWithValue::SpriteWithValue(std::shared_ptr<Tile> entity){

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

    text = std::make_shared<QGraphicsTextItem>(QString::number(entity->getValue()));
    sprite = std::make_shared<QGraphicsPixmapItem>(spriteSet["alive"]);

    setPosition(entity->getXPos(), entity->getYPos());

    sprite->setZValue(4);
    text->setZValue(4);

    Enemy* aux = dynamic_cast<Enemy*>(entity.get());
    if(aux ? aux->getDefeated() : (entity.get()->getValue() == 0))
        setDead();
}

/// @brief Gets the x-coordinate of the sprite.
/// @return The x-coordinate of the sprite.
int SpriteWithValue::getX() const { return x; }

/// @brief Gets the y-coordinate of the sprite. 
/// @return The y-coordinate of the sprite.
int SpriteWithValue::getY() const { return y; }

/// @brief Sets the position of the sprite.
/// @param x The x-coordinate to set.
/// @param y The y-coordinate to set.
void SpriteWithValue::setPosition(int x, int y){
    if(sprite)
        sprite->setPos(x*TILE_SIZE, y*TILE_SIZE);
    if(text)
        text->setPos(x*TILE_SIZE, y*TILE_SIZE - TEXT_OFFSET);

    this->x = x; this->y = y;
}

/// @brief Sets the sprite as dead.
/// @param spriteOffset The offset to apply to the sprite when it's dead.
void SpriteWithValue::setDead(int spriteOffset){
    sprite->setPixmap(spriteSet["dead"]);
    sprite->setPos(x*TILE_SIZE, y*TILE_SIZE + spriteOffset);
    text->setPlainText("");
}

/// @brief Sets the sprite as alive.
/// @param health The health value to set.
void SpriteWithValue::setAlive(float health){
    sprite->setPixmap(spriteSet["alive"]);
    text->setPlainText(QString::number(health));
    text->setZValue(4);
}

/// @brief Sets the health of the sprite.
/// @param health The health value to set.
void SpriteWithValue::setHealth(float health){
    if(text) text->setPlainText(QString::number(health));
}

/// @brief Scales the sprite.
/// @param sprite The sprite to scale.
/// @param stretch Whether to stretch the sprite or not.
/// @return The scaled sprite.
QPixmap SpriteWithValue::scaleSprite(QPixmap sprite, bool stretch) {
    return sprite.scaled(
            TILE_SIZE, TILE_SIZE, 
            (stretch ? Qt::IgnoreAspectRatio : Qt::KeepAspectRatio), 
            Qt::SmoothTransformation);
}
