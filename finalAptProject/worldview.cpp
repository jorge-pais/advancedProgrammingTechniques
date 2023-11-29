#include "worldview.h"

WorldView::WorldView(){

}

//WorldView::WorldView(WorldDelegate& delegate) : delegate(delegate){}

void WorldView::connectSlots(){
    QObject::connect(delegate->getWorldProtagonist().get(), &Protagonist::posChanged, this, &WorldView::positionChangedSlot);
    QObject::connect(delegate->getWorldProtagonist().get(), &Protagonist::healthChanged, this, &WorldView::protagonistHealthChangedSlot);
    QObject::connect(delegate->getWorldProtagonist().get(), &Protagonist::energyChanged, this, &WorldView::protagonistEnergyChangedSlot);
    connect(dynamic_cast<MainWindow*>(parent()), &MainWindow::mainWindowEventSignal, this, &WorldView::mainWindowEventSlot);

    for(auto& enemy : delegate->getWorldEnemies()){
        QObject::connect(enemy.get(), &Enemy::dead, this, &WorldView::enemyDeadSlot);
        PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
        if(pEnemy){
            QObject::connect(pEnemy, &PEnemy::poisonLevelUpdated, this, &WorldView::poisonLevelUpdatedSlot);
        }
    }
}

void WorldView::setDelegate(std::shared_ptr<WorldDelegate> del){
    this->delegate = del;
}

void WorldView::mainWindowEventSlot(QKeyEvent *event)
{
    int x = 0, y = 0;
    /// TODO Figure out why the arrow keys aren't working
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_W:
        y++;
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        x--;
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        y--;
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        x++;
        break;       
    }
    //handle the events and then emit these signals with appropriate parameters
    emit playerMovedSignal(x, y);
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
