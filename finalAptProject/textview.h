#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QObject>
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>
#include <ctime>
#include <iostream>

#include "worldview.h"

//#define TILE_SIZE 20

class WorldView;

class TextView: public QObject
{
    Q_OBJECT

public:
    //TextView(QTextBrowser* textView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate);
    TextView(QTextBrowser* textView, std::shared_ptr<WorldView> view);
    void renderTiles();
    void processCommand(const QString& command);

    private:
        QTextBrowser* textView;
        //QGraphicsScene* scene;
        std::shared_ptr<WorldView> view;
        QStringList availableCommands;

        void printHelp();
        void printMessage(const QString& message);
        void printArgs();
        void printUnknownCommand();
        void updateCommandSuggestions(const QString& partialCommand);

public slots:
        void mainWindowEventSlot(QKeyEvent *event);
};

#endif // TEXTVIEW_H
