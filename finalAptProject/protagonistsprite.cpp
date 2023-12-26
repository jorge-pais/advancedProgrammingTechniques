#include "protagonistsprite.h"

ProtagonistSprite::ProtagonistSprite(std::shared_ptr<Protagonist> prog){
    spriteSet["alive"] = QPixmap(":/images/resources/entities/tux.png");
    spriteSet["alive"] = scaleSprite(spriteSet["alive"]);

    spriteSet["move"] = QPixmap(":/images/resources/entities/tux2.png");
    spriteSet["move"] = scaleSprite(spriteSet["move"]);

    spriteSet["attack"] = QPixmap(":/images/resources/entities/tux3.png");
    spriteSet["attack"] = scaleSprite(spriteSet["attack"]);

    sprite = new QGraphicsPixmapItem(spriteSet["alive"]);
    text = new QGraphicsTextItem(QString::number(prog->getHealth()));
    
    energyBar = new QGraphicsRectItem(0, 0, TILE_SIZE/5, TILE_SIZE);
    QBrush brush(Qt::yellow); QPen pen(Qt::NoPen);
    energyBar->setBrush(brush); energyBar->setPen(pen);

    setPosition(prog->getXPos(), prog->getYPos());
    sprite->setZValue(2);
    text->setZValue(2);
    energyBar->setZValue(2);
}

void ProtagonistSprite::setEnergy(float value){
    int newH = static_cast<int> (TILE_SIZE * value/100.0);
    energyBar->setRect(energyBar->rect().x(), energyBar->rect().y(), energyBar->rect().width(), newH);
}

void ProtagonistSprite::setPosition(int x, int y){
    if(sprite)
        sprite->setPos(x*TILE_SIZE, y*TILE_SIZE);
    if(text)
        text->setPos(x*TILE_SIZE, y*TILE_SIZE - TEXT_OFFSET);
    if(energyBar)
        energyBar->setPos(x*TILE_SIZE - ENERGY_OFFSET, y*TILE_SIZE);

    this->x = x; this->y = y;
};

/// these classes are only called in the protagonist
/// perhaps we should do some inheritance right??
void ProtagonistSprite::animate(QPixmap start, QPixmap end, float time){
    sprite->setPixmap(start);

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, [this, end, timer](){
        sprite->setPixmap(end);
        timer->deleteLater();
    });

    timer->start(time*1000);
}

void ProtagonistSprite::tint(bool poisoned){
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
