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
#include <QApplication>

#include "worldview.h"

//#define TILE_SIZE 20

class WorldView;

class TextView: public QObject
{
    Q_OBJECT

public:
    //TextView(QTextBrowser* textView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate);
    TextView(QTextBrowser* textView, QLineEdit* lineEdit, std::shared_ptr<WorldView> view, QTextBrowser* healthBrowser, QTextBrowser* energyBrowser);
    void renderTiles();
    void resetColor();
    void resetColorAfterDelay();
    void updateHealthDisplay(float currentHealth);
    void updateEnergyDisplay(int currentEnergy);
    void processCommand(const QString& command);
    std::vector<std::shared_ptr<Tile>> worldTiles;
    std::vector<std::shared_ptr<Enemy>> worldEnemies;
    std::vector<std::shared_ptr<Tile>> worldHealthPacks;
    std::shared_ptr<Protagonist> protagonist;
    void centerPlayer();

private:
    QTextBrowser* textView;
    QTextBrowser* healthBrowser;
    QTextBrowser* energyBrowser;
    QCompleter* completer;
    std::shared_ptr<WorldView> view;
    QStringList availableCommands;
    QLineEdit* lineEdit;
    float previousHealth;
    QTimer* colorResetTimer;


    void printHelp();
    void printMessage(const QString& message);
    void printArgs();
    void printUnknownCommand();
};

#endif // TEXTVIEW_H
