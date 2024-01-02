#include "worldview.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory worldViewCat("worldView");

std::shared_ptr<WorldDelegate> WorldView::getDelegate() const
{
    return delegate;
}

WorldView::WorldView(MainWindow * mainWindow){
    window = mainWindow;
}

void WorldView::connectSlots(){
    qCDebug(worldViewCat) << "connectSlots() called";

    QObject::connect(delegate->getWorldProtagonist().get(), &Protagonist::posChanged, this, &WorldView::positionChangedSlot);
    QObject::connect(delegate->getWorldProtagonist().get(), &Protagonist::healthChanged, this, &WorldView::protagonistHealthChangedSlot);
    QObject::connect(delegate->getWorldProtagonist().get(), &Protagonist::energyChanged, this, &WorldView::protagonistEnergyChangedSlot);
    connect(this->window, &MainWindow::mainWindowEventSignal, this, &WorldView::mainWindowEventSlot);

    for(auto& enemy : delegate->getWorldEnemies()){
        QObject::connect(enemy.get(), &Enemy::dead, this, &WorldView::enemyDeadSlot);
        PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
        if(pEnemy){
            QObject::connect(pEnemy, &PEnemy::poisonLevelUpdated, this, &WorldView::poisonLevelUpdatedSlot);
        }
    }
}

void WorldView::setViews(std::shared_ptr<GraphicalView> graphic, std::shared_ptr<TextView> text){
    this->gView = graphic;
    this->tView = text;
}

void WorldView::setDelegate(std::shared_ptr<WorldDelegate> del){
    qCDebug(worldViewCat) << "setDelegate() called";
    this->delegate = del;
}

void WorldView::mainWindowEventSlot(QKeyEvent *event)
{
    int dx = 0, dy = 0;

    /// TODO Figure out why the arrow keys aren't working
    switch(event->key()){
    case Qt::Key_Plus:  // Ctrl + +
        if(event->modifiers() & Qt::ControlModifier)
            gView->zoom(true);
        return;
    case Qt::Key_Minus: // Ctrl - -
        if(event->modifiers() & Qt::ControlModifier)
            gView->zoom(false);
        return;
        
    case Qt::Key_Up:
    case Qt::Key_W:
        dy--;
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        dx--;
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        dy++;
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        dx++;
        break;
    default:
        return; //prevent other keystrokes from sending the signal?
    }
    //handle the events and then emit these signals with appropriate parameters
    emit playerMovedSignal(dx, dy);
}

void WorldView::attackNearestEnemy(){
    qCDebug(worldViewCat) << "attackNearestEnemy() called";
    //find nearest enemy and then use the pathfinder to send the protagonist there and attack
}

void WorldView::takeNearestHealthPack(){
    qCDebug(worldViewCat) << "takeNearestHealthPack() called";
    //find nearest healthpack and then use the pathfinder to send the protagonist there and increase health
}

void WorldView::poisonLevelUpdatedSlot(int value)
{
    auto enemies = this->delegate->getWorldEnemies();

    for(auto& enemy : enemies){
        if(this->delegate->enemyStatus(*enemy) == "PEnemy"){
            PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
            if(pEnemy->getPoisonLevel() == value){
                float initialPoison = pEnemy->getValue();
                float difference = (initialPoison - value)/10;

                for(int i = -difference; i < difference; i++){
                    for(int j = -difference; j < difference; j++){
                        if( abs(i) + abs(j) == difference -1){
                            int poisonX = pEnemy->getXPos() + i;
                            int poisonY = pEnemy->getYPos() + j;
                            if(poisonX < 0 || poisonY < 0 || (poisonX > this->delegate->getWorldColumns() - 1) || (poisonY > this->delegate->getWorldRows() - 1))
                            {} // change the condition
                            else{
                                this->gView->poisonTile(poisonX, poisonY, value);
                                this->delegate->addPoisonTile(poisonX, poisonY, value);
                            }
                        }
                    }
                }
            }
        }
    }
    qCDebug(worldViewCat) << "poisonLevelUpdatedSlot() called";
}

void WorldView::positionChangedSlot(int x, int y)
{
    qCDebug(worldViewCat) << "positionChangedSlot() called";
    // show the protagonist moving on screen
    gView->player->animate(ProtagonistSprite::MOVE);
    gView->player->setPosition(x, y);
    gView->centerView();

    // re-render everything on textView
    tView->renderTiles();
}

void WorldView::newWorldLoadedSlot(){
    disconnect(this->window, &MainWindow::mainWindowEventSignal, this, &WorldView::mainWindowEventSlot);
    this->connectSlots();
    gView->clearTiles();
    gView->renderTiles();
    gView->clearEntities();
    gView->renderEntities();
    gView->clearPlayer();
    gView->renderPlayer();
}

/// is this even connected to something other than the protagonist?
/// there is a pretty similar loop in world delegate
void WorldView::protagonistHealthChangedSlot(int h)
{
    qCDebug(worldViewCat) << "protagonistHealthChangeSlot() called";
    gView->player->setHealth(h <= 0 ? 0 : h);

    auto healthPacks = this->delegate->getWorldHealthPacks();
    for(auto& pack : healthPacks){
        if(pack->getValue() == 0){
            for(auto& healthPack : gView->entities){
                if(healthPack->getX() == pack->getXPos() && healthPack->getY() == pack->getYPos()){
                    healthPack->setDead();
                    gView->player->animate(ProtagonistSprite::HEAL, 0.50);
                }
            }
        }
    }

    if(h <= 0)
        deathScreen();
}

void WorldView::xEnemyStoleSlot(int x, int y, int oldX, int oldY, float health){
    for(auto& enemy : gView->entities){
        if(enemy->getX() == x && enemy->getY() == y){
            enemy->setPosition(oldX, oldY);
            enemy->setDead();
        }
        else if(enemy->getX() == oldX && enemy->getY() == oldY){
            enemy->setAlive(health);
            enemy->setPosition( x, y);
        }
    }
}

/// Only for graphical view as of now
void WorldView::protagonistEnergyChangedSlot(int e)
{
    qCDebug(worldViewCat) << "protagonistEnergyChangedSlot() called";
    
    // show the energy level changing on screen
    gView->player->setEnergy(e);
}

void WorldView::enemyDeadSlot()
{
    qCDebug(worldViewCat) << "enemyDeadSlot() called";
    auto worldEnemies = this->delegate->getWorldEnemies();
    for(auto& worldEnemy : worldEnemies){
        for(auto& enemy : gView->entities){
            if(enemy->getX() == worldEnemy->getXPos() && enemy->getY() == worldEnemy->getYPos() && worldEnemy->getDefeated()){
                enemy->setDead();
            }
        }
    }
    // show the enemy dying on screen

    // protagonist animation
    gView->player->animate(ProtagonistSprite::ATTACK, 0.4);
}

void WorldView::playerPoisoned(bool val){
    this->gView->player->tint(val);
}

void WorldView::deathScreen(){
    qCDebug(worldViewCat) << "deathScreenSlot() called!";

    gView->player->setDead(0);

    QMessageBox deadBox(QMessageBox::NoIcon, 
            "You died!",
            "You just died, maybe it's the devs' fault, maybe it's a skill issue");
    
    QAbstractButton * buttonRetry = deadBox.addButton("Retry", QMessageBox::YesRole);
    QAbstractButton * buttonQuit = deadBox.addButton("Quit", QMessageBox::NoRole);

    deadBox.exec();

    if(deadBox.clickedButton() == buttonRetry){
        qCDebug(worldViewCat) << "retry!";
    }
    else if(deadBox.clickedButton() == buttonQuit){
        qCDebug(worldViewCat) << "quit!";
    }
}
