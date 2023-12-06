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

class WorldView : public QObject
{
    Q_OBJECT
private:
    std::shared_ptr<WorldDelegate> delegate = nullptr;
    std::shared_ptr<GraphicalView> gView;
    std::shared_ptr<TextView> tView;

    MainWindow * window;

public:
    //WorldView();
    WorldView(MainWindow * mainWindow);
    //WorldView(WorldDelegate& delegate);
    void setViews(std::shared_ptr<GraphicalView> graphic, std::shared_ptr<TextView> text);
    void setTextView(std::shared_ptr<TextView> tView);
    void setDelegate(std::shared_ptr<WorldDelegate> delegate);
    std::shared_ptr<WorldDelegate> getDelegate() const;
    void connectSlots();
    void attack();//what is this for?
    void attackNearestEnemy(); //used in text view
    void takeNearestHealthPack();

    std::shared_ptr<WorldDelegate> getDelegate() const;

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
