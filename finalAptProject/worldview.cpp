#include "worldview.h"

WorldView::WorldView(WorldDelegate& delegate) : delegate(delegate)
{
    QObject::connect(delegate.getWorldProtagonist().get(), &Protagonist::posChanged, this, &WorldView::positionChangedSlot);
    QObject::connect(delegate.getWorldProtagonist().get(), &Protagonist::healthChanged, this, &WorldView::protagonistHealthChangedSlot);
    QObject::connect(delegate.getWorldProtagonist().get(), &Protagonist::energyChanged, this, &WorldView::protagonistEnergyChangedSlot);
    connect(dynamic_cast<MainWindow*>(parent()), &MainWindow::mainWindowEventSignal, this, &WorldView::mainWindowEventSlot);

    for(auto& enemy : delegate.getWorldEnemies()){
        QObject::connect(enemy.get(), &Enemy::dead, this, &WorldView::enemyDeadSlot);
        PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
        if(pEnemy){
            QObject::connect(pEnemy, &PEnemy::poisonLevelUpdated, this, &WorldView::poisonLevelUpdatedSlot);
        }
    }
}

void WorldView::mainWindowEventSlot(QKeyEvent *event)
{
    //handle the events and then emit these signals with appropriate parameters
    emit playerMovedSignal(0, 0);
    emit attackSignal(nullptr);
}
void WorldView::poisonLevelUpdatedSlot(int value)
{
    // show the poision on screen
}

void WorldView::positionChangedSlot(int x, int y)
{
    // show the protagonist moving on screen
}

void WorldView::protagonistHealthChangedSlot(int h)
{
    // show the health bar changing on screen
}

void WorldView::protagonistEnergyChangedSlot(int e)
{
    // show the energy level changing on screen
}

void WorldView::enemyDeadSlot()
{
    // show the enemy dying on screen
}

void WorldView::renderTiles(){}
