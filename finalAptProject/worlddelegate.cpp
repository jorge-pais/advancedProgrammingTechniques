#include "worlddelegate.h"
#include "xenemy.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory worldDelegateCat("worldDelegate");

//QLoggingCategory worldDelegateCategory("worlddelegate");

WorldDelegate::WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world)
{
    this->view = view;
    this->world = world;
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

    bool xEnemyMade = false;
    for(auto & enemy : world->getEnemies()){
        if(!xEnemyMade){
            if(enemyStatus(*enemy) == "Regular"){
                auto xEnemy = std::make_shared<XEnemy>(enemy->getXPos(), enemy->getYPos(), enemy->getValue());
                enemies.push_back(std::dynamic_pointer_cast<Enemy, XEnemy>(xEnemy));
                std::cout << enemy->getXPos() << " " << enemy->getYPos() << std::endl;
                xEnemyMade = true;
            }
        }
        else{
            std::shared_ptr<Enemy> sharedEnemy= std::move(enemy);
            enemies.push_back(sharedEnemy);
        }
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
    else if(dynamic_cast<XEnemy*>(&enemy))
        return "XEnemy";
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
    float originalHealth = protagonist->getHealth();

    protagonist->setHealth(originalHealth-enemy->getValue());
    if(enemy->getValue() < originalHealth){
        if(enemyType != "PEnemy"){
            protagonist->setPos(ex, ey);
            enemy->setDefeated(true);
        }
    }
    if(enemyType == "XEnemy" && enemy->getDefeated()){
        auto enemies = getWorldEnemies();
        for(const auto& potentialDead : enemies){
            if(potentialDead->getDefeated() && enemyStatus(*potentialDead) == "Regular"){
                std::cout << "potential leech found" << std::endl;
            }
        }
    }
}

void WorldDelegate::addPoisonTile(int x, int y, float value){
    auto tile = std::make_shared<Tile>( x, y, value);
    poisonTiles.push_back(tile);
}

void WorldDelegate::movedSlot(int x, int y) {
    qCDebug(worldDelegateCat) << "movedSlot() called";
    auto protagonist = getWorldProtagonist();
    auto enemies = getWorldEnemies();
    auto tiles = getWorldTiles();

    // Calculate new postition, check if it's valid
    int newX = protagonist->getXPos() + x;
    int newY = protagonist->getYPos() + y;
    if((x == 0 && y==0) || newX < 0 || newY < 0 || (newX > world->getCols() - 1) || (newY > world->getRows() - 1)) return;

    // If the difference in direction is more than 1, use the pathfinder
    if (sqrt(x*x + y*y) > 1) {
        // Fill the nodes vector with the tiles of your world
        qCDebug(worldDelegateCat) << "using pathfinder";
        moveOnPath(newX, newY);
        return;
    }
    qCDebug(worldDelegateCat) << "NOT using pathfinder";
    float difference = 0;

    // There has to be a more efficient way to do this
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
    if(difference < 0){
        difference = -difference;
    }
    if(protagonist->getEnergy() - difference < 0){
        return;
    }

    for(const auto& poisonTile : poisonTiles){
        if(poisonTile->getXPos() == newX && poisonTile->getYPos() == newY){
            protagonist->setHealth(protagonist->getHealth() - poisonTile->getValue());
        }
    }

    for(const auto& enemy : enemies){
        if(enemy->getXPos() == newX && enemy->getYPos() == newY && !enemy->getDefeated()){
            attack(const_cast<std::shared_ptr<Enemy>&>(enemy));
            return;
        }
    }

    auto healthpacks = getWorldHealthPacks();
    for(const auto& pack : healthpacks){
        if(pack->getXPos() == newX && pack->getYPos() == newY){
            float plusHealth = pack->getValue();
            pack->setValue(0);
            protagonist->setHealth(protagonist->getHealth() + plusHealth);
        }
    }

    protagonist->setPos(newX, newY);
    protagonist->setEnergy(protagonist->getEnergy() - difference);
    std::cout << protagonist->getEnergy() << std::endl;

}

void WorldDelegate::moveOnPath(int newX, int newY){
    auto protagonist = getWorldProtagonist();
    auto enemies = getWorldEnemies();
    auto tiles = getWorldTiles();
    auto healthpacks = getWorldHealthPacks();

    std::vector<Node> nodes;
    for (const auto& tile : getWorldTiles()) {
        nodes.push_back(Node(tile->getXPos(), tile->getYPos(), tile->getValue()));
    }

    // Position object for the start and destination
    Position start(protagonist->getXPos(), protagonist->getYPos());
    Position destination(newX, newY);

    //comparator definition
    Comparator<Node> comp = [](const Node& a, const Node& b) {
        return a.getValue() > b.getValue();
    };

    // width and heuristic weight
    unsigned int width = world->getCols();
    float heurWeight = 1.0f;

    PathFinder<Node, Position> pathFinder(nodes, &start, &destination, comp, width, heurWeight);
    std::vector<int> path = pathFinder.A_star();

    // Define the moveX and moveY arrays
    const int moveX[] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int moveY[] = {-1, -1, 0, 1, 1, 1, 0, -1};

    // Use the path to move the protagonist
    for (int move : path) {
        newX = protagonist->getXPos() + moveX[move];
        newY = protagonist->getYPos() + moveY[move];

        // Calculate the energy cost of the move
        float energyCost = 0;
        for(const auto& tile : tiles){
            if(tile->getXPos() == newX && tile->getYPos() == newY){
                energyCost += tile->getValue();
            }
        }

        if (protagonist->getEnergy() < energyCost) return;
        // If the protagonist's health is 0 or less, stop the loop and return
        if (protagonist->getHealth() <= 0) return;

        // Check if there's an enemy on the path
        for(const auto& enemy : enemies){
            if(enemy->getXPos() == newX && enemy->getYPos() == newY && !enemy->getDefeated()){
                attack(const_cast<std::shared_ptr<Enemy>&>(enemy));
                return;
            }
        }

        // Check if there's a health pack on the path
        for(const auto& pack : healthpacks){
            if(pack->getXPos() == newX && pack->getYPos() == newY){
                protagonist->setHealth(protagonist->getHealth() + pack->getValue());
            }
        }

        // Move the protagonist and update the energy
        protagonist->setPos(newX, newY);
        protagonist->setEnergy(protagonist->getEnergy() - energyCost);
    }
}

