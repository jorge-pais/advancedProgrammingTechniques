#include "mainwindow.h"
#include "ui_mainwindow.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory mainWindowCat("mainWindow");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(std::make_shared<World>()),
    otherWorld(std::make_shared<World>()),
    wView(std::make_shared<WorldView>(this)),
    worldDelegate(std::make_shared<WorldDelegate>(wView, world)),
    otherWorldDelegate(std::make_shared<WorldDelegate>(wView, otherWorld))
{
    ui->setupUi(this);
    srand(time(0));

    wView->setDelegates(worldDelegate, otherWorldDelegate);

    // Create the world from the file, this was to be
    QString worldPath{":/images/resources/world_images/worldmap.png"};
    world->createWorld(worldPath, 5, 6, 0.0);
    QString otherWorldPath{":/images/resources/world_images/worldmap2.png"};
    otherWorld->createWorld(otherWorldPath, 5, 6, 0.0);

    // initialize the worldDelegate
    worldDelegate->initializeWDelegate();
    otherWorldDelegate->initializeWDelegate();

    // Initialize GraphicalView
    QGraphicsScene* scene = new QGraphicsScene();
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
    completer->setCompletionMode(QCompleter::InlineCompletion);
    ui->lineEdit->setCompleter(completer);

    //submit command
    connect(ui->pushButton, &QPushButton::clicked,   this, &MainWindow::submitCommand);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::submitCommand);

    //connect slots and setup
    worldDelegate->addDoor(rand());
    otherWorldDelegate->addDoor(rand());
    worldDelegate->connectSignals();
    worldDelegate->connectSlots();
    otherWorldDelegate->connectSignals();
    wView->connectSlots();
    wView->setViews(gView, tView);

    // render the graphicalView tiles and entities;
    gView->renderTiles();
    gView->renderEntities();
    gView->renderPlayer();
    gView->renderDoor();

    tView->renderTiles();

    toolbarConfig();

    settings = std::make_shared<Settings>(gView);
}

void MainWindow::submitCommand(){
    // Get the command from the QLineEdit
    QString command = ui->lineEdit->text();

    // Process the command using the TextView
    tView->processCommand(command);

    // Clear the QLineEdit for the next input
    ui->lineEdit->clear();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    /// this part signals the view and gives it the event,
    /// the view then extracts what this event means and gives the data to the delegate,
    /// the delegate then applies game logic and sends data to model
    /// model then sends new game values to view to display
    /// view then gives these values to text or graphical view to render in appropriate way

    emit mainWindowEventSignal(event);
}

//////////////////////////
/// Toolbar menu stuff ///
//////////////////////////
void MainWindow::toolbarConfig(){
    QToolBar *toolbar = this->addToolBar("My Toolbar");

    QAction *newGameAction = new QAction("New game", this);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
    toolbar->addAction(newGameAction);

    QAction *overlayAction = new QAction("Overlay", this);
    overlayAction->setCheckable(true);
    connect(overlayAction, &QAction::triggered, this, &MainWindow::toggleOverlay);
    toolbar->addAction(overlayAction);

    QAction *settingsAction = new QAction("Settings", this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::openSettings);
    toolbar->addAction(settingsAction);

    QAction *helpAction = new QAction("Help", this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::openHelp);
    toolbar->addAction(helpAction);

    //contextMenu.exec(event->globalPos());
}

void MainWindow::newGame(){
    qCDebug(mainWindowCat) << "newGame()";

    // for now just list the maps, in order to load them to the new game window prompt
    QDirIterator it(":/images/resources/world_images", QDirIterator::Subdirectories);
    while(it.hasNext()){
        qCDebug(mainWindowCat) << it.next();
    }
}

void MainWindow::toggleOverlay(){
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
        gView->toggleOverlay(action->isChecked());
}

void MainWindow::openSettings(){
    SettingsWindow settingsWindow(nullptr, settings);
    settingsWindow.setModal(true); // set window modality
    settingsWindow.exec();
}

void MainWindow::openHelp(){
    qCDebug(mainWindowCat) << "openHelp() called";
}

MainWindow::~MainWindow()
{
    delete ui;
}
