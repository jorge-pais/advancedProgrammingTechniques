#include "textview.h"

TextView::TextView(QTextBrowser* textView, std::shared_ptr<WorldView> view)
{
    this->textView = textView;
    this->view = view;
    availableCommands << "up" << "right" << "down" << "left" << "goto" << "attack" << "take" << "help"; // Add available commands
    connect(dynamic_cast<MainWindow*>(parent()), &MainWindow::mainWindowEventSignal, this, &TextView::mainWindowEventSlot);
}

void TextView::renderTiles()
{
    // Implement rendering logic for ASCII art-like representation
    // Use worldview to get info about the world state from the model
}

void TextView::processCommand(const QString& command)
{
        QString lowerCommand = command.toLower();
        QStringList commandParts = lowerCommand.split(" ", Qt::SkipEmptyParts);
        QString mainCommand = commandParts.value(0);//first word in command for recognition

        // Map of commands to corresponding functions
        QMap<QString, std::function<void(const QStringList&)>> commandHandlers;

        commandHandlers["goto"] = [this](const QStringList& args) {
            // handle "goto x y"
            if (args.size() == 3) {
                int x = args[1].toInt();
                int y = args[2].toInt();
                emit view->playerMovedSignal(x,y);
            } else {
                printArgs();// invalid number of arguments
            }
        };

        commandHandlers["up"] = [this](const QStringList& args) {
                // emit playerMovedSignal with y++
        };

        commandHandlers["right"] = [this](const QStringList& args) {
                 // emit playerMovedSignal with x++
        };

        commandHandlers["down"] = [this](const QStringList& args) {
                 // emit playerMovedSignal with y--
        };

        commandHandlers["left"] = [this](const QStringList& args) {
                 // emit playerMovedSignal with x--
        };

        commandHandlers["attack"] = [this](const QStringList& args) {
            view->attackNearestEnemy();
        };

        commandHandlers["take"] = [this](const QStringList& args) {
            view->takeNearestHealthPack();
        };

        commandHandlers["help"] = [this](const QStringList& args) {
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
    if (event->key() == Qt::Key_Tab) {
        QString currentText = textView->toPlainText();
        QStringList lines = currentText.split("\n");
        QString lastLine = lines.last().simplified();
        QString partialCommand = lastLine.section(' ', -1).trimmed(); // Get the last word
        updateCommandSuggestions(partialCommand);
    }
}
