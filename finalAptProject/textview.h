#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>
#include <ctime>
#include <iostream>

#include "worlddelegate.h"

#define TILE_SIZE 20

class WorldDelegate;

class TextView
{
public:
    //TextView(QTextBrowser* textView, QGraphicsScene * scene, std::shared_ptr<WorldDelegate> delegate);
    TextView(QTextBrowser* textView, std::shared_ptr<WorldDelegate> delegate);
    void renderTiles();
    void handleCommand(const QString& command);

    private:
        QTextBrowser* textView;
        //QGraphicsScene* scene;
        std::shared_ptr<WorldDelegate> delegate;

};

#endif // TEXTVIEW_H
