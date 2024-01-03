#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QObject>
#include <QKeyEvent>

#include "world.h"
#include "worlddelegate.h"
#include "graphicalview.h"
#include "textview.h"
#include "mainwindow.h"

class WorldDelegate;
class MainWindow;

/// TODO: for now we're handling our inputs here
class WorldView : public QObject
{
    Q_OBJECT
private:
    std::shared_ptr<WorldDelegate> delegate = nullptr;
    //std::shared_ptr<GraphicalView> gView;
    std::shared_ptr<TextView> tView;

    template <typename T> void goToNearestEntity(std::vector<std::shared_ptr<T>> entities);

public:
    std::shared_ptr<GraphicalView> gView;

    //WorldView();
    WorldView(MainWindow * mainWindow);
    //WorldView(WorldDelegate& delegate);
    void setViews(std::shared_ptr<GraphicalView> graphic, std::shared_ptr<TextView> text);
    void setDelegate(std::shared_ptr<WorldDelegate> delegate);
    std::shared_ptr<WorldDelegate> getDelegate() const;
    void connectSlots();
    void attack();//what is this for?
    void attackNearestEnemy(); //used in text view
    void takeNearestHealthPack();
    void playerPoisoned(bool val);

    void deathScreen();

    MainWindow * window;

signals:
    void playerMovedSignal(int x, int y);
    void playerGotoSignal(int x, int y);

public slots:
    void poisonLevelUpdatedSlot(int value);
    void positionChangedSlot(int x, int y);
    void protagonistHealthChangedSlot(int h);
    void protagonistEnergyChangedSlot(int e);
    void enemyDeadSlot();
    void newWorldLoadedSlot();
    void mainWindowEventSlot(QKeyEvent *event);
    void xEnemyStoleSlot(int x, int y, int oldX, int oldY, float health);
};

#endif // WORLDVIEW_H
