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

    void createNewGame();

signals:
    void mainWindowEventSignal(QKeyEvent *event);
    void autoplaySignal(bool activate);

private slots:
    void submitCommand();

public slots:
    void openSettings();
    void openHelp();
    void newGame();
    void toggleOverlay();
    void autoplay();

private:
    Ui::MainWindow *ui;

    std::vector<QString> worldStrings;
    std::vector<std::shared_ptr<World>> worlds;
    std::shared_ptr<WorldView> wView;
    std::vector<std::shared_ptr<WorldDelegate>> worldDelegates;
    std::shared_ptr<GraphicalView> gView;
    std::shared_ptr<TextView> tView;
    std::shared_ptr<Settings> settings;
    bool linear;

    void toolbarConfig();
    void setupViews();
    void render();
    void setup();
    void initialize();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
