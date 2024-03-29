#ifndef XENEMY_H
#define XENEMY_H

#include <sstream>

#include "world.h"

class XEnemy : public Enemy
{
public:
    XEnemy(int xPosition, int yPosition, float strength);
    ~XEnemy() override = default;
    void addEnemy(std::shared_ptr<Enemy> enemy);
    std::vector<std::shared_ptr<Enemy>> getEnemies();
    std::string serialize() override;

private:
    std::vector<std::shared_ptr<Enemy>> alreadyRevived;
};

#endif // XENEMY_H
