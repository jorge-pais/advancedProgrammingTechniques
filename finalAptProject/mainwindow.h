#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsView>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QPushButton>
#include <QTransform>
#include <ctime>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include <QDirIterator>

#include "worlddelegate.h"
#include "world.h"
#include "graphicalview.h"
#include "world.h"
#include "worldview.h"
#include "settingswindow.h"
#include "settings.h"

//#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Settings;
class GraphicalView;
class WorldView;
class WorldDelegate;
class TextView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void mainWindowEventSignal(QKeyEvent *event);

private slots:
    void submitCommand();

public slots:
    void openSettings();
    void openHelp();
    void newGame();
    void toggleOverlay();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<World> world;
    std::shared_ptr<World> otherWorld;
    std::shared_ptr<WorldView> wView;
    std::shared_ptr<WorldDelegate> worldDelegate;
    std::shared_ptr<WorldDelegate> otherWorldDelegate;
    std::shared_ptr<GraphicalView> gView;
    std::shared_ptr<TextView> tView;
    std::shared_ptr<Settings> settings;

    void toolbarConfig();
    void setupViews();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
