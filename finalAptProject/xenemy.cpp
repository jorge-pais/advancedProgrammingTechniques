#include "xenemy.h"

XEnemy::XEnemy(int xPosition, int yPosition, float strength) : Enemy{xPosition, yPosition, strength}{}

void XEnemy::addEnemy(std::shared_ptr<Enemy> enemy){
    alreadyRevived.push_back(enemy);
}

std::vector<std::shared_ptr<Enemy>> XEnemy::getEnemies(){return alreadyRevived;}