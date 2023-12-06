#ifndef WORLDDELEGATE_H
#define WORLDDELEGATE_H

#include <QObject>
#include <memory>
#include "world.h"
#include "worldview.h"

class WorldView;
class WorldDelegate : public QObject
{
    Q_OBJECT

private:
    std::shared_ptr<WorldView> view;
    std::shared_ptr<World> world;

    // Copied from world possibly
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Tile>> healthPacks;

    std::shared_ptr<Protagonist> protagonist;

public:
    WorldDelegate();
    WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world);
    void initializeWDelegate();
    std::vector<std::shared_ptr<Tile>> getWorldTiles();
    std::vector<std::shared_ptr<Enemy>> getWorldEnemies();
    std::vector<std::shared_ptr<Tile>> getWorldHealthPacks();
    int getWorldRows() const;
    int getWorldColumns() const;
    std::shared_ptr<Protagonist> getWorldProtagonist() const;
    void setProtagonistHealth(float healthValue);
    void setProtagonistPosition(int newWorldX, int newWorldY);
    void setProtagonistEnergy(float energyValue);
    std::string enemyStatus(Enemy& enemy);
    void attack(std::shared_ptr<Enemy> enemy); // maybe we won't need this due to the slot
    void connectSlots();

signals:
    void poisonSignal();
public slots:
    void movedSlot(int x, int y);
};

#endif // WORLDDELEGATE_H
