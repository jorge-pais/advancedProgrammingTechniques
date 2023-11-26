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

public:

    WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world);

    std::vector<std::unique_ptr<Tile>> getWorldTiles();
    std::vector<std::unique_ptr<Enemy>> getWorldEnemies();
    std::vector<std::unique_ptr<Tile>> getWorldHealthPacks();
    //void createWorld(String worldPath);
    int getWorldRows() const;
    int getWorldColumns() const;
    std::unique_ptr<Protagonist> getWorldProtagonist() const;
    void setProtagonistHealth(float healthValue);
    void setProtagonistPosition(int newWorldX, int newWorldY);
    void setProtagonistEnergy(float energyValue);
    std::string enemyStatus(Enemy& enemy);// what is this supposed to do?
    void attack(Enemy enemy); // maybe we won't need this due to the slot

signals:
    void poisonSignal();
public slots:
    void attackedSlot(std::shared_ptr<Enemy> enemy);
    void movedSlot(int x, int y);
};

#endif // WORLDDELEGATE_H
