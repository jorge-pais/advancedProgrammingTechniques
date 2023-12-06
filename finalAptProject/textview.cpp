#include "textview.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory textViewCat("textView");

TextView::TextView(QTextBrowser* textView, std::shared_ptr<WorldView> view)
{
    this->textView = textView;
    this->view = view;
    availableCommands << "up" << "right" << "down" << "left" << "goto" << "attack" << "take" << "help"; // Add available commands
    connect(dynamic_cast<MainWindow*>(parent()), &MainWindow::mainWindowEventSignal, this, &TextView::mainWindowEventSlot);
}

void TextView::renderTiles()
{
    qCDebug(textViewCat) << "renderTiles() called";
    // get the game world data
    std::shared_ptr<WorldDelegate> delegate = view->getDelegate();
    worldTiles = delegate->getWorldTiles();
    worldEnemies = delegate->getWorldEnemies();
    worldHealthPacks = delegate->getWorldHealthPacks();
    protagonist = delegate->getWorldProtagonist();

    textView->clear();
    std::vector<std::vector<char>> worldView(delegate->getWorldRows(), std::vector<char>(delegate->getWorldColumns(), ' '));

    for (const auto& enemy : worldEnemies) {
        if (dynamic_cast<PEnemy*>(enemy.get())) {
                   worldView[enemy->getYPos()][enemy->getXPos()] = 'Q';
               } else {
                   worldView[enemy->getYPos()][enemy->getXPos()] = 'E';
               }
    }

    worldView[protagonist->getYPos()][protagonist->getXPos()] = 'P';
    std::cout << protagonist->getYPos() << std::endl;

    for (const auto& healthPack : worldHealthPacks) {
         worldView[healthPack->getYPos()][healthPack->getXPos()] = 'H';
     }

    // grid
    for (const auto& row : worldView) {
        QString line;
        for (const auto& tile : row) {
            line += tile;
            line += " | "; // Add grid lines
        }
        // Append the line to the text view
        textView->append(line);
        textView->append(QString(line.size(), '-'));
    }
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
                emit view->playerMovedSignal(x,y);
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

void TextView::updateCommandSuggestions(const QString& partialCommand)
{
    QStringList suggestions;
    for (const QString& command : availableCommands) {
        if (command.startsWith(partialCommand, Qt::CaseInsensitive)) {
            suggestions << command;
        }
    }
    if (!suggestions.isEmpty()) {
        QString suggestionText = "Suggestions: " + suggestions.join(", ");
        printMessage(suggestionText);
    }
}

void TextView::mainWindowEventSlot(QKeyEvent* event)
{
    qCDebug(textViewCat) << "event slot";
    if (event->key() == Qt::Key_Tab) {
        qCDebug(textViewCat) << "tab";
        QString currentText = textView->toPlainText();
        QStringList lines = currentText.split("\n");
        QString lastLine = lines.last().simplified();
        QString partialCommand = lastLine.section(' ', -1).trimmed(); // Get the last word
        updateCommandSuggestions(partialCommand);
    }
}
