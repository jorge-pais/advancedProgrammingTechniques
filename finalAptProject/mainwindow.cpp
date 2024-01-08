#include "mainwindow.h"
#include "ui_mainwindow.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory mainWindowCat("mainWindow");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    wView(std::make_shared<WorldView>(this))
{
    ui->setupUi(this);
    srand(time(0));

    initialize();

    // Initialize GraphicalView
    gView = std::make_shared<GraphicalView>(ui->graphicsView, wView);

    //Initialize TextView
    tView = std::make_shared<TextView>(ui->textBrowser, ui->lineEdit, wView, ui->healthBrowser, ui->energyBroswer);
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
    setup();
    render();

    toolbarConfig();

    settings = std::make_shared<Settings>(gView);
}

void MainWindow::initialize(){

    world = std::make_shared<World>();
    otherWorlds.push_back(std::make_shared<World>());
    worldDelegate = std::make_shared<WorldDelegate>(wView, world);
    otherWorldDelegates.clear();
    otherWorldDelegates.push_back(std::make_shared<WorldDelegate>(wView, otherWorlds.at(0)));

    wView->setDelegates(worldDelegate, otherWorldDelegates);

    // Create the world from the file, this was to be
    QString worldPath{":/images/resources/world_images/worldmap.png"};
    world->createWorld(worldPath, 5, 6, 0.0);
    QString otherWorldPath{":/images/resources/world_images/worldmap2.png"};
    otherWorlds.at(0)->createWorld(otherWorldPath, 5, 6, 0.0);

    // initialize the worldDelegate
    worldDelegate->initializeWDelegate();
    for(auto& del : otherWorldDelegates){
        del->initializeWDelegate();
    }

}

void MainWindow::setup(){
    //add doors
    worldDelegate->addDoor(rand(), 1);
    worldDelegate->addDoor(rand(), 2);
    otherWorldDelegates.at(0)->addDoor(rand(), 1);
    otherWorldDelegates.at(0)->addDoor(rand(), 2);

    //connect signals/slots
    worldDelegate->connectSignals();
    worldDelegate->connectSlots();
    for(auto& del : otherWorldDelegates){
        del->connectSignals();
    }
    wView->connectSlots();
    wView->setViews(gView, tView);

}

void MainWindow::render(){
    // render the graphicalView tiles and entities;
    gView->renderTiles();
    gView->renderEntities();
    gView->renderPlayer();
    gView->renderDoors();

    gView->centerView();

    tView->renderTiles();
    //show health and energy from the start
    tView->updateHealthDisplay(worldDelegate->getWorldProtagonist()->getHealth());
    tView->updateEnergyDisplay(worldDelegate->getWorldProtagonist()->getEnergy());
}

void MainWindow::createNewGame(){
    // clear everything on the graphical view
    gView->clearDoors();
    gView->clearEntities();
    gView->clearPlayer();
    gView->clearTiles();


    worldDelegate->disconnect();
    for(auto& del : otherWorldDelegates){
        del->disconnect();
    }
    QObject::disconnect(this, nullptr, nullptr, nullptr);
    QObject::disconnect(wView.get(), nullptr, nullptr, nullptr);
    wView->disconnect(); // attempt to disconnect all signals in order to prevent double movement

    // Hopefully after doing this, none of the previous objects is retained,
    // and the heap memory associated with them will be released,
    // as none of these objects can be referenced (right?)
    wView = std::make_shared<WorldView>(this);
    gView = std::make_shared<GraphicalView>(ui->graphicsView, wView);

    initialize();

    //connect slots and setup
    setup();

    render();
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

    QAction *autoplayAction = new QAction("Autoplay", this);
    autoplayAction->setCheckable(true);
    connect(autoplayAction, &QAction::triggered, this, &MainWindow::autoplay);
    toolbar->addAction(autoplayAction);
}

void MainWindow::newGame(){
    qCDebug(mainWindowCat) << "newGame()";

    // for now just list the maps, in order to load them to the new game window prompt
    /* QDirIterator it(":/images/resources/world_images", QDirIterator::Subdirectories);
    while(it.hasNext()){
        qCDebug(mainWindowCat) << it.next();
    } */

    createNewGame();
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

void MainWindow::autoplay(){
    qCDebug(mainWindowCat) << "autoplay() called";

    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
        emit autoplaySignal(action->isChecked());
}

MainWindow::~MainWindow()
{
    delete ui;
}
