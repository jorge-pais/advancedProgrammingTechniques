#include "protagonistsprite.h"

//#include "qloggingcategory.h"

ProtagonistSprite::ProtagonistSprite(std::shared_ptr<Protagonist> prog){
    spriteSet["alive"] = QPixmap(":/images/resources/entities/tux.png");
    spriteSet["alive"] = scaleSprite(spriteSet["alive"]);

    spriteSet["move"] = QPixmap(":/images/resources/entities/tux2.png");
    spriteSet["move"] = scaleSprite(spriteSet["move"]);

    spriteSet["attack"] = QPixmap(":/images/resources/entities/tux3.png");
    spriteSet["attack"] = scaleSprite(spriteSet["attack"]);

    spriteSet["dead"] = QPixmap(":/images/resources/entities/tux4.png");
    spriteSet["dead"] = scaleSprite(spriteSet["dead"]);

    spriteSet["heal"] = QPixmap(":/images/resources/entities/tux5.png");
    spriteSet["heal"] = scaleSprite(spriteSet["heal"]);

    sprite = std::make_shared<QGraphicsPixmapItem>(spriteSet["alive"]);
    animationState = IDLE;
    
    text = std::make_shared<QGraphicsTextItem>(QString::number(prog->getHealth()));
    
    energyBar = std::make_shared<QGraphicsRectItem>(0, 0, TILE_SIZE/5, TILE_SIZE);
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
    //if(sprite)
    sprite->setPos(x*TILE_SIZE, y*TILE_SIZE);
    //if(text)
    text->setPos(x*TILE_SIZE, y*TILE_SIZE - TEXT_OFFSET);
    //if(energyBar)
    energyBar->setPos(x*TILE_SIZE - ENERGY_OFFSET, y*TILE_SIZE + (TILE_SIZE-energyBar->rect().height()));

    this->x = x; this->y = y;
};

void ProtagonistSprite::animate(aState nextState, float time){
    QPixmap start, end = spriteSet["alive"];

    if(nextState < animationState)
        return;
    
    if(animationTimer == nullptr)
        animationTimer = new QTimer(this);
    else
        animationTimer->stop();
    
    animationState = nextState;

    switch (nextState){
    case MOVE:
        start = spriteSet["move"];
        break;
    case ATTACK:
        start = spriteSet["attack"];
        break;
    case HEAL:
        start = spriteSet["heal"];
        break;
    default:
        start = spriteSet["idle"];
        break;
    }

    sprite->setPixmap(start);    

    connect(animationTimer, &QTimer::timeout, [this, end](){
        sprite->setPixmap(end);
        animationTimer->deleteLater();
        animationTimer = nullptr;
        animationState = IDLE;
    });

    animationTimer->start(time*1000);
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
