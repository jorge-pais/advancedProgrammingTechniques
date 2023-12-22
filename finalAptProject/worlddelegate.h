#ifndef WORLDDELEGATE_H
#define WORLDDELEGATE_H

#include <QObject>
#include <memory>
#include <cmath>
#include "world.h"
#include "worldview.h"
#include "pathfinder_class.h"

class WorldView;
class Node : public Tile
{
public:
    float f, g, h;
    bool visited;
    Node* prev;

    Node(int xPosition, int yPosition, float tileWeight)
        : Tile(xPosition, yPosition, tileWeight), f(0), g(0), h(0), visited(false), prev(nullptr)
    {
    }
};

class Position : public Tile
{
public:
    Position(int xPosition, int yPosition)
        : Tile(xPosition, yPosition, 0)
    {
    }
};

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
    std::vector<std::shared_ptr<Tile>> poisonTiles;

    std::shared_ptr<Protagonist> protagonist;

    void moveOnPath(int newX, int newY);

public:
    WorldDelegate();
    WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world);
    void initializeWDelegate();
    std::shared_ptr<Tile> getTile(int x, int y);
    std::vector<std::shared_ptr<Tile>> getWorldTiles();
    std::vector<std::shared_ptr<Enemy>> getWorldEnemies();
    std::vector<std::shared_ptr<Tile>> getWorldHealthPacks();
    int getWorldRows() const;
    int getWorldColumns() const;
    std::shared_ptr<Protagonist> getWorldProtagonist() const;
    void addPoisonTile(int x, int y, float value);
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
