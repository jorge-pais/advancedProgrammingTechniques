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

#define ENERGY_OFFSET 8

class ProtagonistSprite : public SpriteWithValue
{
public:
    ProtagonistSprite(std::shared_ptr<Protagonist> prog);

    void setEnergy(float value);
    void animate(QPixmap start, QPixmap end, float time);
    void tint(bool poisoned);
    void setPosition(int x, int y);

    QGraphicsRectItem * energyBar;
};

#endif // PROTAGONISTSPRITE_H
