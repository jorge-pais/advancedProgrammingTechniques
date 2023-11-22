#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "world.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(0));

    QGraphicsScene *scene = new QGraphicsScene(this);

    this->gView = new GraphicalView(ui->graphicsView, scene);

}

void MainWindow::keyPressEvent(QKeyEvent *event){

    /// TODO Figure out why the arrow keys aren't working
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_W:
        gView->player->setY(gView->player->pos().y() - TILE_SIZE);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        gView->player->setX(gView->player->pos().x() - TILE_SIZE);
        //player->setPixmap(playerFlipped);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        gView->player->setY(gView->player->pos().y() + TILE_SIZE);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        delegate->rightArrowFunction();
        //emit gView->rightArrowSignal();
        gView->player->setX(gView->player->pos().x() + TILE_SIZE);
        //player->setPixmap(playerSprite);
        break;
    }
}

void MainWindow::addDelegate(std::shared_ptr<WorldDelegate> wDelegate){
    delegate = wDelegate;
}

MainWindow::~MainWindow()
{
    delete ui;
}

