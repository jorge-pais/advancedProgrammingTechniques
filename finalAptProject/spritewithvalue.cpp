#include "spritewithvalue.h"

SpriteWithValue::SpriteWithValue() : sprite(nullptr), text(nullptr){}

SpriteWithValue::SpriteWithValue(std::shared_ptr<Protagonist> prog){
    spriteSet["alive"] = QPixmap(":/images/resources/entities/tux.png");
    spriteSet["alive"] = scaleSprite(spriteSet["alive"]);

    spriteSet["move"] = QPixmap(":/images/resources/entities/tux2.png");
    spriteSet["move"] = scaleSprite(spriteSet["move"]);

    sprite = new QGraphicsPixmapItem(spriteSet["alive"]);
    text = new QGraphicsTextItem(QString::number(prog->getHealth()));

    setPosition(prog->getXPos(), prog->getYPos());
    sprite->setZValue(1);
    text->setZValue(1);
}

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

    sprite = new QGraphicsPixmapItem(spriteSet["alive"]);

    setPosition(entity->getXPos(), entity->getYPos());
    sprite->setZValue(1);
    text->setZValue(1);
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
    text->setZValue(1);
}

void SpriteWithValue::setHealth(float health){
    if(text) text->setPlainText(QString::number(health));
}

/// these classes are only called in the protagonist
/// perhaps we should do some inheritance right??
void SpriteWithValue::animate(QPixmap start, QPixmap end, float time){
    sprite->setPixmap(start);

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, [this, end, timer](){
        sprite->setPixmap(end);
        timer->deleteLater();
    });

    timer->start(time*1000);
}

void SpriteWithValue::tint(bool poisoned){
    QGraphicsColorizeEffect * colourEffect = dynamic_cast<QGraphicsColorizeEffect*>(sprite->graphicsEffect());
    
    if(!poisoned){
        this->sprite->setGraphicsEffect(nullptr); 
    }else{
        colourEffect = new QGraphicsColorizeEffect();
        colourEffect->setColor(Qt::green);
        colourEffect->setStrength(0.5);
        sprite->setGraphicsEffect(colourEffect);
    }
}