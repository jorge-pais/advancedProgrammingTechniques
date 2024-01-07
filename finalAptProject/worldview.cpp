#include "worldview.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory worldViewCat("worldView");

std::shared_ptr<WorldDelegate> WorldView::getDelegate() const {
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

    QObject::connect(otherDelegate->getWorldProtagonist().get(), &Protagonist::posChanged, this, &WorldView::positionChangedSlot);
    QObject::connect(otherDelegate->getWorldProtagonist().get(), &Protagonist::healthChanged, this, &WorldView::protagonistHealthChangedSlot);
    QObject::connect(otherDelegate->getWorldProtagonist().get(), &Protagonist::energyChanged, this, &WorldView::protagonistEnergyChangedSlot);

    connect(this->window, &MainWindow::mainWindowEventSignal, this, &WorldView::mainWindowEventSlot);
    connect(this->window, &MainWindow::autoplaySignal, this, &WorldView::autoplaySlot);

    for(auto& enemy : delegate->getWorldEnemies()){
        QObject::connect(enemy.get(), &Enemy::dead, this, &WorldView::enemyDeadSlot);
        PEnemy* pEnemy = dynamic_cast<PEnemy*>(enemy.get());
        if(pEnemy){
            QObject::connect(pEnemy, &PEnemy::poisonLevelUpdated, this, &WorldView::poisonLevelUpdatedSlot);
        }
    }

    for(auto& enemy : otherDelegate->getWorldEnemies()){
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

void WorldView::setDelegates(std::shared_ptr<WorldDelegate> del, std::shared_ptr<WorldDelegate> otherDel){
    qCDebug(worldViewCat) << "setDelegate() called";
    this->delegate = del;
    this->otherDelegate = otherDel;
}

void WorldView::mainWindowEventSlot(QKeyEvent *event) {
    int dx = 0, dy = 0;

    /// TODO Figure out why the arrow keys aren't working
    switch(event->key()){
    case Qt::Key_Plus:  // Ctrl +
        if(event->modifiers() & Qt::ControlModifier)
            gView->zoom(true);
        return;
    case Qt::Key_Minus: // Ctrl -
        if(event->modifiers() & Qt::ControlModifier)
            gView->zoom(false);
        return;
    // WASD movement 
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
    if(delegate->getWorldProtagonist()->getHealth() > 0)
        emit playerMovedSignal(dx, dy);
    
    gView->clearPath();
}

template <typename T> /// TODO: these functions are untested as of yet
void WorldView::goToNearestEntity(std::vector<std::shared_ptr<T>> entities){
    int progX = delegate->getWorldProtagonist()->getXPos();
    int progY = delegate->getWorldProtagonist()->getYPos();

    std::shared_ptr<T> closest = nullptr; int x, y; float min = __FLT_MAX__;
    for(auto & entity: entities){
        // hm spaghetti, idk why this doesn't work without the brackets
        if constexpr (std::is_same<T, Enemy>::value){
            if(entity->getDefeated())
                continue;
        }
        else{
            if(entity->getValue() == 0)
                continue;
        }

        x = entity->getXPos(); y = entity->getYPos();

        float dist = sqrt((x-progX)*(x-progX) + (y-progY)*(y-progY));
        if(dist < min){
            dist = min; 
            closest = entity;
        }
    }

    if(closest != nullptr)
        emit playerGotoSignal(closest->getXPos(), closest->getYPos());
}

void WorldView::attackNearestEnemy(){
    qCDebug(worldViewCat) << "attackNearestEnemy() called";
    //find nearest enemy and then use the pathfinder to send the protagonist there and attack
    goToNearestEntity(delegate->getWorldEnemies());
}

void WorldView::takeNearestHealthPack(){
    qCDebug(worldViewCat) << "takeNearestHealthPack() called";
    //find nearest healthpack and then use the pathfinder to send the protagonist there and increase health
    
    goToNearestEntity(delegate->getWorldHealthPacks());
}

void WorldView::poisonLevelUpdatedSlot(int value) {
    qCDebug(worldViewCat) << "poisonLevelUpdatedSlot() called";

    PEnemy * pEnemy = dynamic_cast<PEnemy*>(sender());

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
                        this->tView->poisonTile(poisonX, poisonY, value);
                        this->delegate->addPoisonTile(poisonX, poisonY, value);
                    }
                }
            }
        }
    }
}

void WorldView::positionChangedSlot(int x, int y) {
    qCDebug(worldViewCat) << "positionChangedSlot() called";
    // show the protagonist moving on screen
    gView->player->animate(ProtagonistSprite::MOVE);
    gView->player->setEnergy(delegate->getWorldProtagonist()->getEnergy());
    gView->player->setPosition(x, y);
    gView->centerView();

    // re-render everything on textView
    tView->renderTiles();
}

void WorldView::clearPath(){
    gView->clearPath();
    ///TODO: Something in textview also!
}

void WorldView::newWorldLoadedSlot(){

    delegate.swap(otherDelegate);
    delegate->connectSlots();

    delegate->setProtagonistHealth(otherDelegate->getWorldProtagonist()->getHealth());
    delegate->setProtagonistPosition(delegate->getDoor()->getXPos(),delegate->getDoor()->getYPos());

    gView->clearTiles();
    gView->clearEntities();
    gView->clearDoor();

    gView->renderTiles();
    gView->renderEntities();
    gView->renderDoor();
    gView->renderPoisonTiles();

    gView->centerView();
}

void WorldView::protagonistHealthChangedSlot(int h) {
    qCDebug(worldViewCat) << "protagonistHealthChangeSlot() called";
    gView->player->setHealth(h <= 0 ? 0 : h);
    tView->updateHealthDisplay(h);
    auto healthPacks = this->delegate->getWorldHealthPacks();
    for(auto& pack : healthPacks){
        if(pack->getValue() == 0 && pack->getXPos() == delegate->getWorldProtagonist()->getXPos() && pack->getYPos() == delegate->getWorldProtagonist()->getYPos()){
            for(auto& healthPack : gView->healthPacks){
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

void WorldView::protagonistEnergyChangedSlot(int e)
{
    qCDebug(worldViewCat) << "protagonistEnergyChangedSlot() called";
    
    // show the energy level changing on screen
    gView->player->setEnergy(e);
    tView->updateEnergyDisplay(e);
}

void WorldView::enemyDeadSlot()
{
    qCDebug(worldViewCat) << "enemyDeadSlot() called";

    auto worldEnemy = dynamic_cast<Enemy*>(sender());    
    if(!worldEnemy) return; // this happens because of the door??
    
    for(auto& enemy : gView->entities){
        if(enemy->getX() == worldEnemy->getXPos() && enemy->getY() == worldEnemy->getYPos() && worldEnemy->getDefeated()){
            // show the enemy dying on screen
            enemy->setDead();
        }
    }

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

        QCoreApplication::processEvents();
        window->createNewGame();
    }
    else if(deadBox.clickedButton() == buttonQuit){
        qCDebug(worldViewCat) << "quit!";

        exit(0);
    }
}

/// TODO: this is still quite buggy, but it works!
void WorldView::autoplaySlot(bool activate){
    qCDebug(worldViewCat) << "autoplaySlot() called " << activate;

    autoplayEnabled = activate;

    if(autoplayEnabled){
        QtConcurrent::run([this](){
            auto player = delegate->getWorldProtagonist();
            auto enemies = delegate->getWorldEnemies();

            // this is working more or less, 
            // there is a segmentation fault which happens when no
            // enemy to attack is found
            while(autoplayEnabled){
                std::shared_ptr<Enemy> attack = nullptr;

                for(auto enemy : enemies)
                    if((enemy->getValue() <= player->getHealth()) && !enemy->getDefeated()){
                        attack = enemy;
                    }

                // The problem with this strategy is that the the PEnemies get don't get defeated immediately
                if(attack == nullptr)
                    QMetaObject::invokeMethod(this, "takeNearestHealthPack", Qt::QueuedConnection);
                else
                    QMetaObject::invokeMethod(this, "playerGotoSignal", Qt::QueuedConnection, Q_ARG(int, attack->getXPos()), Q_ARG(int, attack->getYPos()));

                QThread::sleep(1);
                //return;
            }
        });
    }
}