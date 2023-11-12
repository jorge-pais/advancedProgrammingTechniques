#ifndef WORLDDELEGATE_H
#define WORLDDELEGATE_H

#include <QObject>
#include "world.h"
#include "worldview.h"

class WorldView;
class WorldDelegate : public QObject
{
    Q_OBJECT

private:
    WorldView& view;
    World& world;

public:
    WorldDelegate(WorldView& view, World& world);

    std::vector<std::unique_ptr<Tile>> getWorldTiles();
    std::vector<std::unique_ptr<Enemy>> getWorldEnemies();
    std::vector<std::unique_ptr<Tile>> getWorldHealthPacks();
    int getWorldRows() const;
    int getWorldColumns() const;
    std::unique_ptr<Protagonist> getWorldProtagonist() const;
    void setProtagonistHealth(float healthValue);
    void setProtagonistPosition(int newWorldX, int newWorldY);
    void setProtagonistEnergy(float energyValue);
    void enemyStatus();// what is this supposed to do?
    void attack(Enemy enemy); // maybe we won't need this due to the slot

public slots:
    //void attackedSlot(QPointer<Enemy> enemy);
    void movedSlot(int x, int y);
};

#endif // WORLDDELEGATE_H
