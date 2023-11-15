#include "worlddelegate.h"

WorldDelegate::WorldDelegate(WorldView& view, World& world) : view(view), world(world)
{
    //QObject::connect(&view, &WorldView::attackSignal, this, &WorldDelegate::attackedSlot);
    QObject::connect(&view, &WorldView::playerMovedSignal, this, &WorldDelegate::movedSlot);
}

std::vector<std::unique_ptr<Tile>> WorldDelegate::getWorldTiles()
{
    return world.getTiles();
}

std::vector<std::unique_ptr<Enemy>> WorldDelegate::getWorldEnemies()
{
    return world.getEnemies();
}

std::vector<std::unique_ptr<Tile>> WorldDelegate::getWorldHealthPacks()
{
    return world.getHealthPacks();
}

int WorldDelegate::getWorldRows() const
{
    return world.getRows();
}

int WorldDelegate::getWorldColumns() const
{
    return world.getCols();
}

std::unique_ptr<Protagonist> WorldDelegate::getWorldProtagonist() const
{
    return world.getProtagonist();
}

void WorldDelegate::setProtagonistHealth(float healthValue)
{
    auto protagonist = world.getProtagonist();
    protagonist->setHealth(healthValue);
}

void WorldDelegate::setProtagonistPosition(int newWorldX, int newWorldY)
{
    auto protagonist = world.getProtagonist();
    protagonist->setPos(newWorldX, newWorldY);
}

void WorldDelegate::setProtagonistEnergy(float energyValue)
{
    auto protagonist = world.getProtagonist();
    protagonist->setEnergy(energyValue);
}

void WorldDelegate::enemyStatus()
{
    //TODO
}

void WorldDelegate::attack(Enemy enemy)
{
    auto tiles = world.getTiles();
    auto protagonist = world.getProtagonist();
    int px = protagonist->getXPos();
    int py = protagonist->getYPos();
    int ex = enemy.getXPos();
    int ey = enemy.getYPos();
    if((px == ex && (py == ey - 1 || py == ey +1)) || (py == ey && (px == ex - 1 || px == ex +1))){
        protagonist->setHealth(protagonist->getHealth()-enemy.getValue());
        if(enemy.getValue() < protagonist->getHealth()){
            enemy.setDefeated(true);
        }
    }
}
/*
void WorldDelegate::attackedSlot(QPointer<Enemy> enemy)
{
    // TODO
}
*/
void WorldDelegate::movedSlot(int x, int y)
{
    // TODO
}
