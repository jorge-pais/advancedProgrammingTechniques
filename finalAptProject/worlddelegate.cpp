#include "worlddelegate.h"

//QLoggingCategory worldDelegateCategory("worlddelegate");

WorldDelegate::WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world)
{
    this->view = view;
    this->world = world;

    //this->view->setDelegate(std::make_shared<WorldDelegate>(*this));
}

void WorldDelegate::connectSlots(){
    QObject::connect(this->view.get(), &WorldView::playerMovedSignal, this, &WorldDelegate::movedSlot);

    if(this->getWorldEnemies().size() != 0)
        for(auto& enemy : this->getWorldEnemies()){
            PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
            if(pEnemy){
                QObject::connect(this, &WorldDelegate::poisonSignal, pEnemy, &PEnemy::poison);
            }
        }
}

std::vector<std::unique_ptr<Tile>> WorldDelegate::getWorldTiles()
{
    return world->getTiles();
}

std::vector<std::unique_ptr<Enemy>> WorldDelegate::getWorldEnemies()
{
    return world->getEnemies();
}

std::vector<std::unique_ptr<Tile>> WorldDelegate::getWorldHealthPacks()
{
    return world->getHealthPacks();
}

int WorldDelegate::getWorldRows() const
{
    return world->getRows();
}

int WorldDelegate::getWorldColumns() const
{
    return world->getCols();
}

std::unique_ptr<Protagonist> WorldDelegate::getWorldProtagonist() const
{
    return world->getProtagonist();
}

void WorldDelegate::setProtagonistHealth(float healthValue)
{
    auto protagonist = world->getProtagonist();
    protagonist->setHealth(healthValue);
}

void WorldDelegate::setProtagonistPosition(int newWorldX, int newWorldY)
{
    auto protagonist = world->getProtagonist();
    protagonist->setPos(newWorldX, newWorldY);
}

void WorldDelegate::setProtagonistEnergy(float energyValue)
{
    auto protagonist = world->getProtagonist();
    protagonist->setEnergy(energyValue);
}

std::string WorldDelegate::enemyStatus(Enemy& enemy)
{
    if (dynamic_cast<PEnemy*>(&enemy))
    {
        return "PEnemy";
    }
    else
    {
        return "Regular";
    }
}

void WorldDelegate::attack(std::shared_ptr<Enemy> enemy)
{
    std::string enemyType = enemyStatus(*enemy);
    if (enemyType == "PEnemy") {
        emit poisonSignal();
    }
    auto tiles = world->getTiles();
    auto protagonist = world->getProtagonist();
    int px = protagonist->getXPos();
    int py = protagonist->getYPos();
    int ex = enemy->getXPos();
    int ey = enemy->getYPos();

    protagonist->setHealth(protagonist->getHealth()-enemy->getValue());
    if(enemy->getValue() < protagonist->getHealth()){
        protagonist->setPos(enemy->getXPos(), enemy->getYPos());
        enemy->setDefeated(true);
    }
}

void WorldDelegate::movedSlot(int x, int y)
{
    auto protagonist = world->getProtagonist();
    int newX = protagonist->getXPos() + x;
    int newY = protagonist->getYPos() + y;

    if(newX < 0 || newY < 0 || (newX > world->getCols()) || (newY > world->getRows())){
        return;
    }

    auto enemies = world->getEnemies();
    for(const auto& enemy : enemies){
        if(enemy->getXPos() == newX && enemy->getYPos() == newY){
            attack(std::move(const_cast<std::unique_ptr<Enemy>&>(enemy)));
            return;
        }
    }

    auto healthpacks = world->getHealthPacks();
    for(const auto& pack : healthpacks){
        if(pack->getXPos() == newX && pack->getYPos() == newY){
            protagonist->healthChanged(pack->getValue());
        }
    }

    protagonist->setPos(newX, newY);
}
