#ifndef PROTAGONISTSPRITE_H
#define PROTAGONISTSPRITE_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>
#include <QTimer>
#include <QGraphicsColorizeEffect>
#include "spritewithvalue.h"

#define ENERGY_OFFSET 8 // x offset for the energy bar

/// @class ProtagonistSprite
/// @brief A class that represents the protagonist's sprite in the game.
/// This class is a child of the SpriteWithValue class and is used to represent the protagonist's sprite in the game.
/// It includes additional functionality for animations and energy bar display.
class ProtagonistSprite : public SpriteWithValue
{
public:
    ProtagonistSprite(std::shared_ptr<Protagonist> prog);

    /// @brief Enum for tracking animation priority.
    enum aState {
        IDLE = 0,
        MOVE = 1,
        DEAD = 10, // the dead state isn't part of any animation
        ATTACK = 3,
        HEAL = 2
    } animationState;

    void setEnergy(float value);
    void setHealth(float health);
    //void animate(QPixmap start, QPixmap end, float time);
    void animate(aState nextState, float time = 0.10);
    void tint(bool poisoned);
    void setPosition(int x, int y);

    void setDead(int spriteOffset = DEAD_OFFSET);

    std::shared_ptr<QGraphicsRectItem> energyBar;

protected:
    QTimer* animationTimer = nullptr;
};

#endif // PROTAGONISTSPRITE_H
