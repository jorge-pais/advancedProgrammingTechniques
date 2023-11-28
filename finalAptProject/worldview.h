#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QObject>
#include <QKeyEvent>

#include "world.h"
#include "worlddelegate.h"
#include "graphicalview.h"
#include "mainwindow.h"

class WorldDelegate;
class WorldView : public QObject
{
    Q_OBJECT
private:
    std::shared_ptr<WorldDelegate> delegate = nullptr;
    std::shared_ptr<GraphicalView> gView;

public:
    WorldView();
    //WorldView(WorldDelegate& delegate);

    void setDelegate(std::shared_ptr<WorldDelegate> delegate);

signals:
    void playerMovedSignal(int x, int y);
    void attackSignal(std::shared_ptr<Enemy> enemy);

public slots:
    void poisonLevelUpdatedSlot(int value);
    void positionChangedSlot(int x, int y);
    void protagonistHealthChangedSlot(int h);
    void protagonistEnergyChangedSlot(int e);
    void enemyDeadSlot();
    void mainWindowEventSlot(QKeyEvent *event);
};

#endif // WORLDVIEW_H
