#include "mainwindow.h"
#include "ui_mainwindow.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory mainWindowCat("mainWindow");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(std::make_shared<World>()),
    wView(std::make_shared<WorldView>()),
    worldDelegate(std::make_shared<WorldDelegate>(wView, world))

{
    ui->setupUi(this);
    srand(time(0));

    wView->setDelegate(worldDelegate);

    QString worldPath{":/images/resources/world_images/worldmap.png"};
    world->createWorld(worldPath, 2, 0, 0.0);

    // INITIALIZE THE WORLDDELEGATE, WE COPY EVERYTHING
    worldDelegate->initializeWorld();

    // Initialize GraphicalView
    QGraphicsScene * scene = new QGraphicsScene();
    gView = std::make_shared<GraphicalView>(ui->graphicsView, scene, worldDelegate);

    //Initialize TextView
    tView = std::make_shared<TextView>(ui->textBrowser, wView);
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->textBrowser->setFont(font);


    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::submitCommand);

    worldDelegate->connectSlots();
    wView->connectSlots();

    // render the background tiles;
    gView->renderTiles();
    gView->renderPlayer();
    gView->renderEntities();

    tView->renderTiles();
}

void MainWindow::submitCommand()
{
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

    //gView->view->centerOn(gView->player);

    //gView->renderTiles();
}

MainWindow::~MainWindow()
{
    delete ui;
}
