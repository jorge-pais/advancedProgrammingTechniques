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
        for(auto& enemy : this->getWorldEnemies()){ // calling here world enemies makes it such that i can't get the enemies later on in the graphics views
            PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
            if(pEnemy){
                QObject::connect(this, &WorldDelegate::poisonSignal, pEnemy, &PEnemy::poison);
            }
        }
}

/// I believe this function is very necessary, since there is no way
/// easy way to move the unique_ptr back to the world vector, surprisingly
void WorldDelegate::initializeWorld(){
    //if (tiles == nullptr && healthPacks != nullptr && enemies != nullptr) return

    for(auto & tile : world->getTiles()){
        std::shared_ptr<Tile> sharedTile= std::move(tile);
        tiles.push_back(sharedTile);
    }

    for(auto & enemy : world->getEnemies()){
        std::shared_ptr<Enemy> sharedEnemy= std::move(enemy);
        enemies.push_back(sharedEnemy);
    }

    for(auto & healthPack : world->getTiles()){
        std::shared_ptr<Tile> sharedHealthPack= std::move(healthPack);
        tiles.push_back(sharedHealthPack);
    }

}

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldTiles()
{
    return this->tiles;
}

std::vector<std::shared_ptr<Enemy>> WorldDelegate::getWorldEnemies()
{
    return this->enemies;
}

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldHealthPacks()
{
    return this->healthPacks;
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
    if((px == ex && (py == ey - 1 || py == ey +1)) || (py == ey && (px == ex - 1 || px == ex +1))){
        protagonist->setHealth(protagonist->getHealth()-enemy->getValue());
        if(enemy->getValue() < protagonist->getHealth()){
            enemy->setDefeated(true);
        }
    }
}

void WorldDelegate::movedSlot(int x, int y)
{
    // TODO
}
