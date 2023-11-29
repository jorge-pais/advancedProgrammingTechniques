#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsView>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>
#include <ctime>
#include <memory>

#include <iostream>
#include <sstream>
#include <string>

#include "worlddelegate.h"
#include "world.h"
#include "graphicalview.h"
#include "world.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void mainWindowEventSignal(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    //World world;
    //WorldView wView;
    //WorldDelegate worldDelegate; // Changed to non-pointer
    GraphicalView *gView; // Pointer, as GraphicalView needs a QGraphicsView

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
