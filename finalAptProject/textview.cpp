#include "textview.h"

TextView::TextView(QTextBrowser* textView, std::shared_ptr<WorldDelegate> delegate)
{
    this->textView = textView;
    //this->scene = scene;
    this->delegate = delegate;
    // Create the scene
    //textView->setScene(scene);
}

void TextView::renderTiles()
{
    // Implement rendering logic for ASCII art-like representation
    // Use delegate to get information about the world state

    // Example:
    // scene->clear();
    // QString worldText = delegate->getWorldTextRepresentation();
    // QGraphicsTextItem* textItem = scene->addText(worldText);
    // textItem->setPos(0, 0);
}

void TextView::handleCommand(const QString& command)
{
    // Implement command handling logic
    // Parse the command and perform corresponding actions

    // Example:
    // if (command == "up")
    // {
    //     delegate->movePlayerUp();
    // }
    // else if (command == "right")
    // {
    //     delegate->movePlayerRight();
    // }
    // // Add other command handling logic as needed
}
