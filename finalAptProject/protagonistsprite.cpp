#include "protagonistsprite.h"

//#include "qloggingcategory.h"

/// all the if(this == nullptr) return; lines are such
/// a bodge due to smart pointers not working as intended!
/// this causes the compile with -Wall to complain
/// about the nonnull object being compared to NULL (-Wnonnull-compare)

/// @brief Constructor for the ProtagonistSprite class.
/// @param prog Shared pointer to the Protagonist object.
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
    sprite->setZValue(5);
    text->setZValue(5);
    energyBar->setZValue(5);
}

/// @brief Sets the energy value for the protagonist.
/// @param value The energy value to be set.
void ProtagonistSprite::setEnergy(float value){
    if(this == nullptr) return;
    int newH = static_cast<int> (TILE_SIZE * value/100.0);
    energyBar->setRect(energyBar->rect().x(), energyBar->rect().y(), energyBar->rect().width(), newH);
}

/// @brief Sets the health of the sprite.
/// @param health The health value to set.
void ProtagonistSprite::setHealth(float health){
    if(this == nullptr) return;
    if(text) text->setPlainText(QString::number(health));
}

/// @brief Sets the position of the protagonist sprite.
/// @param x The x-coordinate of the position.
/// @param y The y-coordinate of the position.
void ProtagonistSprite::setPosition(int x, int y){
    if(this == nullptr) return;
    //if(sprite)
    sprite->setPos(x*TILE_SIZE, y*TILE_SIZE);
    //if(text)
    text->setPos(x*TILE_SIZE, y*TILE_SIZE - TEXT_OFFSET);
    //if(energyBar)
    energyBar->setPos(x*TILE_SIZE - ENERGY_OFFSET, y*TILE_SIZE + (TILE_SIZE-energyBar->rect().height()));

    this->x = x; this->y = y;
};

/// @brief Animates the protagonist sprite based on the next state.
/// @param nextState The next state of the animation.
/// @param time The time duration for the animation.
void ProtagonistSprite::animate(aState nextState, float time){
    if(this == nullptr) return;
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

/// @brief Sets the protagonist sprite to the dead state.
/// @param spriteOffset The sprite offset for the dead state.
void ProtagonistSprite::setDead(int spriteOffset){
    if(this == nullptr) return;

    animationState = DEAD;
    SpriteWithValue::setDead(spriteOffset);
    energyBar->setRect(0, 0, 0, 0);
}

/// @brief Tints the protagonist sprite based on the poisoned state.
/// @param poisoned The poisoned state of the protagonist.
void ProtagonistSprite::tint(bool poisoned){
    if(this == nullptr) return;

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
