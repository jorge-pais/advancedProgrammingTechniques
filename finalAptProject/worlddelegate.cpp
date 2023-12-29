#include "worlddelegate.h"
#include "xenemy.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory worldDelegateCat("worldDelegate");

WorldDelegate::WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world)
{
    this->view = view;
    this->world = world;
}

void WorldDelegate::connectSlots(){
    qCDebug(worldDelegateCat) << "connectSlots() called";

    QObject::connect(this->view.get(), &WorldView::playerMovedSignal, this, &WorldDelegate::movedSlot);
    QObject::connect(this->view.get(), &WorldView::playerGotoSignal, this, &WorldDelegate::gotoSlot);
    QObject::connect(this, &WorldDelegate::xEnemyStoleSignal, this->view.get(), &WorldView::xEnemyStoleSlot);

    if(this->getWorldEnemies().size() != 0){
        for(auto& enemy : this->getWorldEnemies()){ // calling here world enemies makes it such that i can't get the enemies later on in the graphics views
            PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
            if(pEnemy){
                QObject::connect(this, &WorldDelegate::poisonSignal, pEnemy, &PEnemy::poison);
            }
        }
    }
}

void WorldDelegate::initializeWDelegate(){
    qCDebug(worldDelegateCat) << "initializeWorld() called";
    //if (tiles != nullptr && healthPacks != nullptr && enemies != nullptr) return
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

    /// check if the protagonist is in a infinite energy tile
    if(std::isinf(getTile(protagonist->getXPos(), protagonist->getYPos())->getValue())){
        for(auto & tile : tiles){
            if(!std::isinf(tile->getValue())){
                this->protagonist->setPos(tile->getXPos(), tile->getYPos());
                break;
            }
        }
    }
}

std::shared_ptr<Tile> WorldDelegate::getTile(int x, int y){
    if((x < 0 || x >= world->getCols()) || (y < 0 || y >= world->getRows())) 
        return nullptr;

    int rows = getWorldRows();
    return this->tiles[rows*y + x];
}

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldTiles(){ return this->tiles; }

std::vector<std::shared_ptr<Enemy>> WorldDelegate::getWorldEnemies(){ return this->enemies; }

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldHealthPacks(){ return this->healthPacks; }

int WorldDelegate::getWorldRows() const { return world->getRows(); }

int WorldDelegate::getWorldColumns() const { return world->getCols(); }

std::shared_ptr<Protagonist> WorldDelegate::getWorldProtagonist() const { return this->protagonist; }

void WorldDelegate::setProtagonistHealth(float healthValue){
    this->protagonist->setHealth(healthValue >= 100 ? 100 : healthValue); 
}

void WorldDelegate::setProtagonistPosition(int newWorldX, int newWorldY) { this->protagonist->setPos(newWorldX, newWorldY); }

void WorldDelegate::setProtagonistEnergy(float energyValue){ this->protagonist->setEnergy(energyValue >= 100 ? 100 : energyValue); }

/// TODO: this could be an aux function associated with some enum
std::string WorldDelegate::enemyStatus(Enemy& enemy)
{
    qCDebug(worldDelegateCat) << "enemyStatus() called";
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

    protagonist->setHealth(originalHealth - enemy->getValue());
    if(enemy->getValue() < originalHealth){
        if(enemyType != "PEnemy"){
            protagonist->setPos(ex, ey);
            enemy->setDefeated(true);
        }
    }
    if(enemyType == "XEnemy" && enemy->getDefeated()){
        XEnemy* xEnemy = dynamic_cast<XEnemy*>(enemy.get());
        auto enemies = getWorldEnemies();
        auto alreadyStolen = xEnemy->getEnemies();
        for(const auto& potentialDead : enemies){
            bool found = false;
            for(const auto& stolen : alreadyStolen){
                if(stolen->getXPos() == potentialDead->getXPos() && stolen->getYPos() == potentialDead->getYPos()){
                    found = true;
                }
            }
            if(potentialDead->getDefeated() && enemyStatus(*potentialDead) == "Regular" && !found){
                int oldX = xEnemy->getXPos();
                int oldY = xEnemy->getYPos();
                xEnemy->setDefeated(false);
                xEnemy->setXPos(potentialDead->getXPos());
                xEnemy->setYPos(potentialDead->getYPos());
                xEnemy->setValue(potentialDead->getValue());
                emit xEnemyStoleSignal(xEnemy->getXPos(), xEnemy->getYPos(), oldX, oldY, xEnemy->getValue());
                potentialDead->setXPos(oldX);
                potentialDead->setYPos(oldY);
                xEnemy->addEnemy(potentialDead);
                return;
            }
        }
    }
}

void WorldDelegate::addPoisonTile(int x, int y, float value){
    auto tile = std::make_shared<Tile>( x, y, value);
    poisonTiles.push_back(tile);
    
    // check for player poison and that the player is still alive, otherwise
    // it will proc more than once
    if(protagonist->getXPos() == x && protagonist->getYPos() == y && protagonist->getHealth() > 0){
        setProtagonistHealth(protagonist->getHealth() - value);
        view->playerPoisoned(true);
    }
}

void WorldDelegate::movedSlot(int dx, int dy) {
    qCDebug(worldDelegateCat) << "movedSlot() called";

    if(protagonist->getHealth() <= 0) return;

    // Calculate new postition, check if it's valid
    int newX = protagonist->getXPos() + dx;
    int newY = protagonist->getYPos() + dy;
    if( (dx == 0 && dy==0) || newX < 0 || newY < 0 || 
        (newX > world->getCols() - 1) || (newY > world->getRows() - 1))
        return;

    float difference = 0;
    // There has to be a more efficient way to do this
    difference += getTile(newX, newY)->getValue();

    // Moving in the world will take energy defined by the value of the tile you are moving to
    if(difference < 0){
        difference = -difference;
    }

    if(protagonist->getEnergy() - difference < 0){
        return;
    }

    // check to see if new tile is poison
    bool isPoisoned = false;
    for(const auto& poisonTile : poisonTiles){
        if(poisonTile->getXPos() == newX && poisonTile->getYPos() == newY){
            setProtagonistHealth(protagonist->getHealth() - poisonTile->getValue());
            isPoisoned = true;
        }
    }
    view->playerPoisoned(isPoisoned);

    for(const auto& enemy : getWorldEnemies()){
        if(enemy->getXPos() == newX && enemy->getYPos() == newY && !enemy->getDefeated()){
            attack(const_cast<std::shared_ptr<Enemy>&>(enemy));
            return;
        }
    }

    for(const auto& pack : getWorldHealthPacks()){
        if(pack->getXPos() == newX && pack->getYPos() == newY){
            float plusHealth = pack->getValue();
            pack->setValue(0);
            setProtagonistHealth(protagonist->getHealth() + plusHealth);
        }
    }

    protagonist->setPos(newX, newY);
    setProtagonistEnergy(protagonist->getEnergy() - difference);
    std::cout << protagonist->getEnergy() << std::endl;
}

/// TODO: there could be some timing function to move the protagonist
/// step by step
void WorldDelegate::gotoSlot(int newX, int newY){
    /// idk why, but the command "goto 0 0", isn't working
    /// while debugging, it simply returns an empty path 
    qCDebug(worldDelegateCat) << "gotoSlot() called";

    std::vector<Node> nodes;
    for (const auto& tile : tiles)
        nodes.push_back(Node(tile->getXPos(), tile->getYPos(), tile->getValue()));

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
    const int moveX[] = {+0, -1, -1, -1, +0, +1, +1, +1}; // x here is flipped for some bizarre, unknown reason
    const int moveY[] = {-1, -1, +0, +1, +1, +1, +0, -1};

    int nextX, nextY;
    // Use the path to move the protagonist
    for (int move : path) {
        nextX = protagonist->getXPos() + moveX[move];
        nextY = protagonist->getYPos() + moveY[move];

        // Calculate the energy cost of the move
        float energyCost = getTile(nextX, nextY)->getValue();
        
        // If the protagonist's health is 0 or less, stop the loop and return
        if (protagonist->getEnergy() < energyCost && protagonist->getEnergy() <= 0) return;

        // Check if there's an enemy on the path
        for(const auto& enemy : getWorldEnemies()){
            if(enemy->getXPos() == nextX && enemy->getYPos() == nextY && !enemy->getDefeated()){
                attack(const_cast<std::shared_ptr<Enemy>&>(enemy));
                return; /// TODO: is this necessary?  
            }
        }

        // Check if there's a health pack on the path
        for(const auto& pack : getWorldHealthPacks()){
            if(pack->getXPos() == nextX && pack->getYPos() == nextY){
                setProtagonistHealth(protagonist->getHealth() + pack->getValue());
            }
        }

        // Move the protagonist and update the energy
        protagonist->setPos(nextX, nextY);
        protagonist->setEnergy(protagonist->getEnergy() - energyCost);
    }
}
