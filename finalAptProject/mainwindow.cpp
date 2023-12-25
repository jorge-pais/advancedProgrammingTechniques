#include "mainwindow.h"
#include "ui_mainwindow.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory mainWindowCat("mainWindow");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(std::make_shared<World>()),
    wView(std::make_shared<WorldView>(this)),
    worldDelegate(std::make_shared<WorldDelegate>(wView, world))
////////////////////////////////////////////////////////////////
{
    ui->setupUi(this);
    srand(time(0));

    wView->setDelegate(worldDelegate);

    QString worldPath{":/images/resources/world_images/worldmap.png"};
    world->createWorld(worldPath, 5, 6, 0.0);

    // initialize the worldDelegate
    worldDelegate->initializeWDelegate();

    // Initialize GraphicalView
    QGraphicsScene * scene = new QGraphicsScene();
    gView = std::make_shared<GraphicalView>(ui->graphicsView, scene, wView);

    //Initialize TextView
    tView = std::make_shared<TextView>(ui->textBrowser, ui->lineEdit, wView);
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    ui->textBrowser->setFont(font);

    //autocomplete
    QStringList availableCommands = {"up", "right", "down", "left", "goto", "attack", "take", "help"};
    QCompleter* completer = new QCompleter(availableCommands, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit->setCompleter(completer);

    //submit command
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::submitCommand);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::submitCommand);

    //connect slots and setup
    worldDelegate->connectSlots();
    wView->connectSlots();
    wView->setViews(gView, tView);

    // render the graphicalView tiles and entities;
    gView->renderTiles();
    gView->renderEntities();
    gView->renderPlayer();

    tView->renderTiles();

    toolbarConfig();
}

void MainWindow::submitCommand(){
    // Get the command from the QLineEdit
    QString command = ui->lineEdit->text();

    // Process the command using the TextView
    tView->processCommand(command);

    // Clear the QLineEdit for the next input
    ui->lineEdit->clear();
}

/// this part signals the view and gives it the event,
/// the view then extracts what this event means and gives the data to the delegate,
/// the delegate then applies game logic and sends data to model
/// model then sends new game values to view to display
/// view then gives these values to text or graphical view to render in appropriate way
void MainWindow::keyPressEvent(QKeyEvent *event){
    emit mainWindowEventSignal(event);
}

//////////////////////////
/// Toolbar menu stuff ///
//////////////////////////
void MainWindow::toolbarConfig(){
    QToolBar *toolbar = this->addToolBar("My Toolbar");

    QAction *action1 = new QAction("Action 1", this);
    connect(action1, &QAction::triggered, this, &MainWindow::doAction1);
    toolbar->addAction(action1);

    QAction *action2 = new QAction("Action 2", this);
    connect(action2, &QAction::triggered, this, &MainWindow::doAction2);
    toolbar->addAction(action2);

    //contextMenu.exec(event->globalPos());
}

void MainWindow::doAction1(){
    std::cout << "doAction1() man!" << std::endl;
}

void MainWindow::doAction2(){
    std::cout << "doAction2() man!" << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}
