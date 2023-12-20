#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QObject>
#include <QGraphicsScene>
#include <QBrush>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QLineEdit>
#include <QTransform>
#include <QCompleter>
#include <ctime>
#include <iostream>
#include <QScrollBar>

#include "worldview.h"

//#define TILE_SIZE 20

class WorldView;

class TextView: public QObject
{
    Q_OBJECT

public:
    //TextView(QTextBrowser* textView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate);
    TextView(QTextBrowser* textView, QLineEdit* lineEdit, std::shared_ptr<WorldView> view);
    void renderTiles();
    void resetColor();
    void processCommand(const QString& command);
    std::vector<std::shared_ptr<Tile>> worldTiles;
    std::vector<std::shared_ptr<Enemy>> worldEnemies;
    std::vector<std::shared_ptr<Tile>> worldHealthPacks;
    std::shared_ptr<Protagonist> protagonist;
    void centerPlayer();

<<<<<<< HEAD
private:
    QTextBrowser* textView;
    QCompleter* completer;
    std::shared_ptr<WorldView> view;
    QStringList availableCommands;
    QLineEdit* lineEdit;
    QTextCursor* protagonistCursor;
=======

    private:
        QTextBrowser* textView;
        QCompleter* completer;
        std::shared_ptr<WorldView> view;
        QStringList availableCommands;
        QLineEdit* lineEdit;
        float previousHealth;
        QTimer* colorResetTimer;
>>>>>>> 16e5623c29ad536442458629383f89efbb52baa1

    void printHelp();
    void printMessage(const QString& message);
    void printArgs();
    void printUnknownCommand();
};

#endif // TEXTVIEW_H
