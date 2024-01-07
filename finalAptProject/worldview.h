#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QObject>
#include <QKeyEvent>
#include <QtConcurrent/QtConcurrent>
#include <unistd.h>
#include <type_traits>

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
    std::shared_ptr<WorldDelegate> otherDelegate = nullptr;
    //std::shared_ptr<GraphicalView> gView;
    std::shared_ptr<TextView> tView;

    template <typename T> void goToNearestEntity(std::vector<std::shared_ptr<T>> entities);
    //float calculateDistance(const std::pair<int, int>& pos1, const std::pair<int, int>& pos2) {
    //    return std::abs(pos1.first - pos2.first) + std::abs(pos1.second - pos2.second);
    //}

    bool autoplayEnabled = false;

public:
    std::shared_ptr<GraphicalView> gView;

    //WorldView();
    WorldView(MainWindow * mainWindow);
    //WorldView(WorldDelegate& delegate);
    void setViews(std::shared_ptr<GraphicalView> graphic, std::shared_ptr<TextView> text);
    void setDelegates(std::shared_ptr<WorldDelegate> delegate, std::shared_ptr<WorldDelegate> otherDelegate);
    std::shared_ptr<WorldDelegate> getDelegate() const;
    void connectSlots();
    //void attack(); //what is this for?
    void playerPoisoned(bool val);
    void deathScreen();
    void clearPath();

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
    void autoplaySlot(bool activate);
    void attackNearestEnemy(); //used in text view
    void takeNearestHealthPack();
};

#endif // WORLDVIEW_H
