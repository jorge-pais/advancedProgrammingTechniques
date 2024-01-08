#include "worlddelegate.h"
#include <stdlib.h>
#include "xenemy.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory worldDelegateCat("worldDelegate");

WorldDelegate::WorldDelegate(std::shared_ptr<WorldView> view, std::shared_ptr<World> world)
{
    this->view = view;
    this->world = world;
}

void WorldDelegate::connectSignals(){

    QObject::connect(this, &WorldDelegate::xEnemyStoleSignal, this->view.get(), &WorldView::xEnemyStoleSlot);
    QObject::connect(this, &WorldDelegate::newWorldLoadedSignal, this->view.get(), &WorldView::newWorldLoadedSlot);

    if(this->getWorldEnemies().size() != 0){
        for(auto& enemy : this->getWorldEnemies()){ // calling here world enemies makes it such that i can't get the enemies later on in the graphics views
            PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
            if(pEnemy){
                QObject::connect(this, &WorldDelegate::poisonSignal, pEnemy, &PEnemy::poison);
            }
        }
    }
}

void WorldDelegate::connectSlots(){
    qCDebug(worldDelegateCat) << "connectSlots() called";

    QObject::connect(this->view.get(), &WorldView::playerMovedSignal, this, &WorldDelegate::movedSlot);
    QObject::connect(this->view.get(), &WorldView::playerGotoSignal, this, &WorldDelegate::gotoSlot);
}

/* void WorldDelegate::terminate(){
    tiles.clear();
    enemies.clear();
    healthPacks.clear();
    poisonTiles.clear();
    QObject::disconnect(this, nullptr, nullptr, nullptr);
} */

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
    /* if(std::isinf(getTile(protagonist->getXPos(), protagonist->getYPos())->getValue())){
        for(auto & tile : tiles){
            if(!std::isinf(tile->getValue())){
                this->protagonist->setPos(tile->getXPos(), tile->getYPos());
                break;
            }
        }
    } */

    rows = world->getRows();
    cols = world->getCols();
}

std::shared_ptr<Tile> WorldDelegate::getTile(int x, int y){
    if((x < 0 || x >= cols) || (y < 0 || y >= rows)) 
        return nullptr;

    return this->tiles[rows*y + x];
}

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldTiles(){ return this->tiles; }

std::vector<std::shared_ptr<Tile>> WorldDelegate::getPoisonTiles(){ return this->poisonTiles; }

std::vector<std::shared_ptr<Enemy>> WorldDelegate::getWorldEnemies(){ return this->enemies; }

std::vector<std::shared_ptr<Tile>> WorldDelegate::getWorldHealthPacks(){ return this->healthPacks; }

int WorldDelegate::getWorldRows() const { return this->rows; }

int WorldDelegate::getWorldColumns() const { return this->cols; }

std::shared_ptr<Protagonist> WorldDelegate::getWorldProtagonist() const { return this->protagonist; }

void WorldDelegate::setProtagonistHealth(float healthValue){
    this->protagonist->setHealth(healthValue >= 100 ? 100 : healthValue); 
}

void WorldDelegate::setProtagonistPosition(int newWorldX, int newWorldY) { 
    this->protagonist->setPos(newWorldX, newWorldY); 
}

void WorldDelegate::setProtagonistEnergy(float energyValue){ 
    this->protagonist->setEnergy(energyValue >= 100 ? 100 : energyValue); 
    std::cout << protagonist->getEnergy() << std::endl;
}

std::vector<std::shared_ptr<Tile>> WorldDelegate::getDoors(){
    return doors;
}

void WorldDelegate::addDoor(int seed, int destination){
    srand(seed);
    bool done = false;
    bool found = false;
    int x;
    int y;
    while(!done){
        found = false;
        x = rand() % getWorldColumns();
        y = rand() % getWorldRows();

        for(const auto& healthpack : getWorldHealthPacks()){
            if(healthpack->getXPos() == x && healthpack->getYPos() == y){
                found = true;
            }
        }
        for(const auto& enemy : getWorldEnemies()){
            if(enemy->getXPos() == x && enemy->getYPos() == y){
                found = true;
            }
        }
        for(const auto& door : doors){
            if(door->getXPos() == x && door->getYPos() == y){
                found = true;
            }
        }
        if(x == 0 && y == 0){
            found = true;;
        }
        if(!found){
            done = true;
        }
    }

    doors.push_back(std::make_shared<Tile>(x, y, destination));

}

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
    if(protagonist->getHealth() <= 1e-4)
        return;
        
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

void WorldDelegate::activateDoor(int destination){

    QObject::disconnect(this->view.get(), &WorldView::playerMovedSignal, this, &WorldDelegate::movedSlot);
    QObject::disconnect(this->view.get(), &WorldView::playerGotoSignal, this, &WorldDelegate::gotoSlot);

    emit newWorldLoadedSignal(destination);
}

void WorldDelegate::movedSlot(int dx, int dy) {
    qCDebug(worldDelegateCat) << "movedSlot() called";

    if(protagonist->getHealth() <= 1e-4) return;

    // Calculate new postition, check if it's valid
    int newX = protagonist->getXPos() + dx;
    int newY = protagonist->getYPos() + dy;
    if( (dx == 0 && dy==0) || newX < 0 || newY < 0 || 
        (newX > cols - 1) || (newY > rows - 1))
        return;
    for(const auto& door : doors){
        if(newX == door->getXPos() && newY == door->getYPos()){
            activateDoor(door->getValue());
        return;
        }
    }

    singleMove(newX, newY);
}

/// TODO: there could be some timing function to move the protagonist
/// step by step
void WorldDelegate::gotoSlot(int newX, int newY){
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
    unsigned int width = cols;
    float heurWeight = 1.0f;

    PathFinder<Node, Position> pathFinder(nodes, &start, &destination, comp, width, heurWeight);
    std::vector<int> path = pathFinder.A_star();

    // Define the moveX and moveY arrays
    const int moveX[] = {+0, -1, -1, -1, +0, +1, +1, +1}; // x here is flipped for some bizarre, unknown reason
    const int moveY[] = {-1, -1, +0, +1, +1, +1, +0, -1};

    int nextX, nextY;
    view->clearPath();

    // Use the path to move the protagonist
    for (int move : path) {
        nextX = protagonist->getXPos() + moveX[move];
        nextY = protagonist->getYPos() + moveY[move];

        // If we find an enemy or run out energy, stop taking the path
        if(singleMove(nextX, nextY))
            return;

        view->gView->pathTile(nextX, nextY);
    }
}

/// Aux function for the movement slots
int WorldDelegate::singleMove(int x, int y){
    // Calculate the energy cost of the move
    float energyCost = getTile(x, y)->getValue();
    
    // If the protagonist's health is 0 or less, stop the loop and return
    if (protagonist->getEnergy() < energyCost && protagonist->getEnergy() <= 0)  
        return 1;

    if(protagonist->getHealth() <= 1e-4)
        return 1;

    // check for poison tile
    bool isPoisoned = false;
    for(const auto& poisonTile : poisonTiles){
        if(poisonTile->getXPos() == x && poisonTile->getYPos() == y){
            setProtagonistHealth(protagonist->getHealth() - poisonTile->getValue());
            isPoisoned = true;
        }
    }

    view->playerPoisoned(isPoisoned);

    // Check if there's an enemy on the path
    for(const auto& enemy : enemies){
        if(enemy->getXPos() == x && enemy->getYPos() == y && !enemy->getDefeated()){
            attack(const_cast<std::shared_ptr<Enemy>&>(enemy));
            return 1; // flag to stop path
        }
    }

    // Move the protagonist and update the energy
    setProtagonistPosition(x, y);
    setProtagonistEnergy(protagonist->getEnergy() - energyCost);

    // Check if there's a health pack on the path
    for(const auto& pack : healthPacks){
        if(pack->getXPos() == x && pack->getYPos() == y){
            float plusHealth = pack->getValue();
            pack->setValue(0);
            setProtagonistHealth(protagonist->getHealth() + plusHealth);
        }
    }

    return 0;
}

std::string WorldDelegate::serialize(){
    /// TODO: this method is untested as of yet!
    /// also unused
    std::stringstream out;

    out << "/----TILES----/\n";
    for(auto tile : tiles) 
        out << tile->serialize() << '\n';

    out << "/-PROTAGONIST-/\n";
    out << protagonist->serialize() << '\n';

    out << "/---ENEMIES---/\n";
    for(auto enemy : enemies)
        if(!dynamic_cast<XEnemy*>(enemy.get())) // not XEnemy
            out << enemy->serialize() << '\n';

    out << "/--XENEMIES---/\n";
    for(auto enemy : enemies)
        if(dynamic_cast<XEnemy*>(enemy.get()))
            out << enemy->serialize() << '\n';

    return out.str();
}
