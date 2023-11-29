#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QObject>
#include "world.h"
#include "worlddelegate.h"

class WorldDelegate;
class WorldView : public QObject
{
    Q_OBJECT
private:
    WorldDelegate& delegate;

public:
    WorldView();
    WorldView(WorldDelegate& delegate);

    virtual void renderTiles();

signals:
    void playerMovedSignal(int x, int y);
    void attackSignal(std::shared_ptr<Enemy> enemy);

public slots:
    void poisonLevelUpdatedSlot(int value);
    void positionChangedSlot(int x, int y);
    void protagonistHealthChangedSlot(int h);
    void protagonistEnergyChangedSlot(int e);
    void enemyDeadSlot();
};

#endif // WORLDVIEW_H
