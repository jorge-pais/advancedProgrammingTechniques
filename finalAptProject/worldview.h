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
    void setGraphicalView(std::shared_ptr<GraphicalView> gView);
    void setTextView(std::shared_ptr<TextView> tView);
    void setDelegate(std::shared_ptr<WorldDelegate> delegate);
    void connectSlots();
    void attack();

signals:
    void playerMovedSignal(int x, int y);

public slots:
    void poisonLevelUpdatedSlot(int value);
    void positionChangedSlot(int x, int y);
    void protagonistHealthChangedSlot(int h);
    void protagonistEnergyChangedSlot(int e);
    void enemyDeadSlot();
    void mainWindowEventSlot(QKeyEvent *event);
};

#endif // WORLDVIEW_H
