#include "worlddelegate.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory worldDelegateCat("worldDelegate");

//QLoggingCategory worldDelegateCategory("worlddelegate");

WorldDelegate::WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world)
{
    this->view = view;
    this->world = world;

    //this->view->setDelegate(std::make_shared<WorldDelegate>(*this));
}

void WorldDelegate::connectSlots(){
    qCDebug(worldDelegateCat) << "connectSlots() called";

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
///
/// For now we're only copying the vectors, perhaps the protagonist
/// also needs to be copied.
void WorldDelegate::initializeWDelegate(){
    qCDebug(worldDelegateCat) << "initializeWorld() called";
    //if (tiles == nullptr && healthPacks != nullptr && enemies != nullptr) return
    for(auto & tile : world->getTiles()){
        std::shared_ptr<Tile> sharedTile= std::move(tile);
        tiles.push_back(sharedTile);
    }

    for(auto & enemy : world->getEnemies()){
        std::shared_ptr<Enemy> sharedEnemy= std::move(enemy);
        enemies.push_back(sharedEnemy);
    }

    for(auto & healthPack : world->getHealthPacks()){
        std::shared_ptr<Tile> sharedHealthPack= std::move(healthPack);
        healthPacks.push_back(sharedHealthPack);
    }
    this->protagonist = std::move(world->getProtagonist());
}

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldTiles()
{
    qCDebug(worldDelegateCat) << "getWorldTiles() called";
    return this->tiles;
}

std::vector<std::shared_ptr<Enemy>> WorldDelegate::getWorldEnemies()
{
    qCDebug(worldDelegateCat) << "getWorldEnemies() called";
    return this->enemies;
}

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldHealthPacks()
{
    qCDebug(worldDelegateCat) << "getWorldHealthPacks() called";
    return this->healthPacks;
}

int WorldDelegate::getWorldRows() const
{
    qCDebug(worldDelegateCat) << "getWorldRows() called";
    return world->getRows();
}

int WorldDelegate::getWorldColumns() const
{
    qCDebug(worldDelegateCat) << "getWorldColumns() called";
    return world->getCols();
}

std::shared_ptr<Protagonist> WorldDelegate::getWorldProtagonist() const
{
    qCDebug(worldDelegateCat) << "getWorldProtagonist() called";
    return this->protagonist;
}

void WorldDelegate::setProtagonistHealth(float healthValue)
{
    qCDebug(worldDelegateCat) << "setProtagonistHealth() called";
    auto protagonist = world->getProtagonist();
    protagonist->setHealth(healthValue);
}

void WorldDelegate::setProtagonistPosition(int newWorldX, int newWorldY)
{
    qCDebug(worldDelegateCat) << "setProtagonistPosition() called";
    auto protagonist = world->getProtagonist();
    protagonist->setPos(newWorldX, newWorldY);
}

void WorldDelegate::setProtagonistEnergy(float energyValue)
{
    qCDebug(worldDelegateCat) << "setProtagonistEnergy() called";
    auto protagonist = world->getProtagonist();
    protagonist->setEnergy(energyValue);
}

std::string WorldDelegate::enemyStatus(Enemy& enemy)
{
    qCDebug(worldDelegateCat) << "enemy() called";
    if (dynamic_cast<PEnemy*>(&enemy))
        return "PEnemy";
    else
        return "Regular";
}

void WorldDelegate::attack(std::shared_ptr<Enemy> enemy)
{
    qCDebug(worldDelegateCat) << "attack() called";
    std::string enemyType = enemyStatus(*enemy);
    if (enemyType == "PEnemy") {
        emit poisonSignal();
    }
    auto tiles = getWorldTiles();
    auto protagonist = getWorldProtagonist();
    int ex = enemy->getXPos();
    int ey = enemy->getYPos();

    protagonist->setHealth(protagonist->getHealth()-enemy->getValue());
    if(enemy->getValue() < protagonist->getHealth()){
        protagonist->setPos(ex, ey);
        enemy->setDefeated(true);
    }
}

void WorldDelegate::movedSlot(int x, int y)
{
    std::cout << "movedslot called with x: " << x << " and y: " << y << std::endl;
    auto protagonist = getWorldProtagonist();
    int newX = protagonist->getXPos() + x;
    int newY = protagonist->getYPos() + y;

    if(newX < 0 || newY < 0 || (newX > world->getCols()) || (newY > world->getRows())){
        return;
    }

    auto tiles = getWorldTiles();
    float difference = 0;
    for(const auto& tile : tiles){
        if(tile->getXPos() == x && tile->getYPos() == y){
            difference += tile->getValue();
        }
        if(tile->getXPos() == newX && tile->getYPos() == newY){
            difference -= tile->getValue();
        }
    }
    if(difference < 0){
        difference = -difference;
    }
    if((protagonist->getEnergy() - difference)*0.05 < 0){
        return;
    }

    auto enemies = getWorldEnemies();
    for(const auto& enemy : enemies){
        if(enemy->getXPos() == newX && enemy->getYPos() == newY){
            attack(const_cast<std::shared_ptr<Enemy>&>(enemy));
            return;
        }
    }

    auto healthpacks = getWorldHealthPacks();
    for(const auto& pack : healthpacks){
        if(pack->getXPos() == newX && pack->getYPos() == newY){
            protagonist->setHealth(protagonist->getHealth() + pack->getValue());
        }
    }

    protagonist->setPos(newX, newY);
    protagonist->setEnergy((protagonist->getEnergy() - difference)*0.05);
}
