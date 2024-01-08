#include "textview.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory textViewCat("textView");

TextView::TextView(QTextBrowser* textView, QLineEdit* lineEdit, std::shared_ptr<WorldView> view, QTextBrowser* healthBrowser, QTextBrowser* energyBrowser)
{
    this->textView = textView;
    this->view = view;
    this->lineEdit = lineEdit;
    this->healthBrowser = healthBrowser;
    this->energyBrowser = energyBrowser;
    QTextCursor cursor(textView->document());

    textView->setTextCursor(cursor);

    previousHealth = view->getDelegate()->getWorldProtagonist()->getHealth();
    colorResetTimer = new QTimer(this);
    connect(colorResetTimer, &QTimer::timeout, this, &TextView::resetColor);
}

void TextView::renderTiles() {
    qCDebug(textViewCat) << "renderTiles() called";

    // get the game world data
    std::shared_ptr<WorldDelegate> delegate = view->getDelegate();
    worldTiles = delegate->getWorldTiles();
    worldEnemies = delegate->getWorldEnemies();
    worldHealthPacks = delegate->getWorldHealthPacks();
    protagonist = delegate->getWorldProtagonist();
    //auto poisonedTiles = delegate->getPoisonedTiles();
    textView->clear();
    std::vector<std::vector<char>> worldView(delegate->getWorldRows(), std::vector<char>(delegate->getWorldColumns(), ' '));

    // Defeated enemies show up as lower case
    for (const auto& enemy : worldEnemies) {
        char textEnemy = 'E'; // Default character for regular enemies

        if (auto pEnemy = dynamic_cast<PEnemy*>(enemy.get())) {
            textEnemy = pEnemy->getDefeated() ? 'q' : 'Q'; // lower case for dead
        } else if (auto xEnemy = dynamic_cast<XEnemy*>(enemy.get())) {
            textEnemy = xEnemy->getDefeated() ? 'x' : 'X';
        } else {
            textEnemy = enemy->getDefeated() ? 'e' : 'E';
        }

        worldView[enemy->getYPos()][enemy->getXPos()] = textEnemy;
    }

    for (const auto& healthPack : worldHealthPacks) {
        worldView[healthPack->getYPos()][healthPack->getXPos()] = healthPack->getValue() ? 'H' : ' ';
    }


    // Place door on the grid
    for(const auto& door : delegate->getDoors()){
        worldView[door->getYPos()][door->getXPos()] = 'D';
    }

    worldView[protagonist->getYPos()][protagonist->getXPos()] = 'P';

    for (const auto& tile : delegate->getPoisonTiles()) {
        //textView->setTextColor(Qt::red);
        worldView[tile->getYPos()][tile->getXPos()] = '*';
        //textView->setTextColor(Qt::black);// tile is a pair<x, y>
    }

    textView->disconnect(SIGNAL(textChanged()));

    // grid
    QString line;
    for (const auto& row : worldView) {
        QString line = "| ";
        for (const auto& tile : row) {
            line += tile;
            line += " | "; // Add grid lines
        }
        // Append the line to the text view
        textView->append(line);
        textView->append(QString(line.size(), '-'));
    }
    //this->centerPlayer();
    QFontMetrics metrics(textView->font());
    int lineHeight = metrics.lineSpacing();
    int scrollPos= protagonist->getYPos() * lineHeight;
    textView->verticalScrollBar()->setValue(scrollPos);

}

void TextView::resetColorAfterDelay() {
    colorResetTimer->stop();
    colorResetTimer->start(2000); //ms
    qCDebug(textViewCat) << "resetColorAfterDelay() is called";

}

void TextView::resetColor(){
    updateHealthDisplay(view->getDelegate()->getWorldProtagonist()->getHealth());
}
void TextView::poisonTile(int x, int y, int poisonLevel){
    poisonedTiles.insert({x, y});
}

void TextView::updateHealthDisplay(float currentHealth) {
    QString healthText = QString::number(currentHealth);


    if (currentHealth < previousHealth) {
        healthBrowser->setTextColor(Qt::red);
        healthBrowser->setText(healthText);
        resetColorAfterDelay();
    } else if (currentHealth > previousHealth) {
        healthBrowser->setTextColor(Qt::green);
        healthBrowser->setText(healthText);
        resetColorAfterDelay();
    }
    else{
        healthBrowser->setTextColor(Qt::black);
        healthBrowser->setText(healthText);
    }
    previousHealth = currentHealth;
}

void TextView::updateEnergyDisplay(int currentEnergy){
    QString energyText = QString::number(currentEnergy);
    energyBrowser->setTextColor(Qt::blue);
    energyBrowser->setText(energyText);
}

void TextView::processCommand(const QString& command)
{
    QString lowerCommand = command.toLower();
    QStringList commandParts = lowerCommand.split(" ", Qt::SkipEmptyParts);
    QString mainCommand = commandParts.value(0);//first word in command for recognition

    // Map of commands to corresponding functions
    QMap<QString, std::function<void(const QStringList&)>> commandHandlers;

    commandHandlers["goto"] = [this](const QStringList& args) {
        qCDebug(textViewCat) << "goto";
        // handle "goto x y"
        if (args.size() == 3) {
            int x = args[1].toInt();
            int y = args[2].toInt();
            emit view->playerGotoSignal(x,y);
        } else {
            printArgs();// invalid number of arguments
        }
    };
    int dx = 0, dy = 0;
    commandHandlers["up"] = [&](const QStringList& args) {
        //emit playerMovedSignal with y++
        qCDebug(textViewCat) << "up";
        dy--;
        emit view->playerMovedSignal(dx,dy);

    };

    commandHandlers["right"] = [&](const QStringList& args) {
        // emit playerMovedSignal with x++
        qCDebug(textViewCat) << "right";
        dx++;
        emit view->playerMovedSignal(dx,dy);
    };

    commandHandlers["down"] = [&](const QStringList& args) {
       // emit playerMovedSignal with y--
       qCDebug(textViewCat) << "down";
       dy++;
       emit view->playerMovedSignal(dx,dy);
    };

    commandHandlers["left"] = [&](const QStringList& args) {
        // emit playerMovedSignal with x--
       qCDebug(textViewCat) << "left";
       dx--;
       emit view->playerMovedSignal(dx,dy);
    };

    commandHandlers["attack"] = [this](const QStringList& args) {
        qCDebug(textViewCat) << "attack";
        view->attackNearestEnemy();
    };

    commandHandlers["take"] = [this](const QStringList& args) {
        qCDebug(textViewCat) << "take";
        view->takeNearestHealthPack();
    };

    commandHandlers["help"] = [this](const QStringList& args) {
        qCDebug(textViewCat) << "help";
        printHelp();// print list of available commands
    };

    // Add additional command handlers as needed

    // Check if the main command is in the map
    if (commandHandlers.contains(mainCommand)) {
        // Call the corresponding handler function
        commandHandlers[mainCommand](commandParts);
    } else {
        // Unknown command
        printUnknownCommand();
    }
}

void TextView::printHelp()
{
    // Print a list of available commands
    // Customize this function based on your application's specific commands
    QString helpText = "Available commands:\n"
                       "  up\n"
                       "  right\n"
                       "  down\n"
                       "  left\n"
                       "  goto x y\n"
                       "  attack nearest enemy\n"
                       "  take nearest health pack\n"
                       "  help\n";
    printMessage(helpText);
}

void TextView::printMessage(const QString& message)
{
    // Print a message to the text view
    textView->append(message);
    qCDebug(textViewCat) << "print";
}

void TextView::printArgs()
{
    printMessage("Invalid number of arguments. Check the command syntax and try again.");
}

void TextView::printUnknownCommand()
{
    printMessage("Unknown command. Type 'help' for a list of available commands.");
}