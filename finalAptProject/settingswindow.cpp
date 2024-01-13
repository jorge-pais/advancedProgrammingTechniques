#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, std::shared_ptr<Settings> set)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
    , settings(set)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsWindow::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SettingsWindow::reject);

    connect(ui->pushButton,   &QPushButton::clicked, this, &SettingsWindow::chooseTile);

    ui->lineEdit->setText(QString::fromStdString(settings->getTileSet()));

    // Set levels in the 
    QStandardItemModel * model = new QStandardItemModel(settings->levels.size(), 2);

    for (int i = 0; i < model->rowCount(); i++){
        QStandardItem *level = new QStandardItem(QString::fromStdString(settings->levels.at(i).first));
        QStandardItem *overlay = new QStandardItem(QString::fromStdString(settings->levels.at(i).second));
        model->setItem(i, 0, level);
        model->setItem(i, 1, overlay);
    }

    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QObject::connect(model, &QStandardItemModel::itemChanged, [model](QStandardItem *item){
        int rowCount = model->rowCount();
        if(item->row() == rowCount - 1){
            model->insertRow(rowCount);
        }
    });
}

std::string SettingsWindow::chooseFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if(!fileName.isEmpty())
        return fileName.toStdString();

    return "";
}

void SettingsWindow::chooseTile(){ ui->lineEdit->setText(QString::fromStdString(chooseFile())); }

SettingsWindow::~SettingsWindow(){
    delete ui;
}

void SettingsWindow::accept(){
    settings->loadTileSet(ui->lineEdit->displayText().toStdString());

    setLevel();

    QDialog::accept();
}

void SettingsWindow::reject(){
    QDialog::reject();
}

void SettingsWindow::setLevel(){
    std::cout << "lol" << std::endl;
    auto model = ui->tableView->model();

    std::vector<std::pair<std::string, std::string>> levels;

    if(model){
        for (int i = 0; i < model->rowCount(); i++){
            auto level = model->data(model->index(i, 0), Qt::DisplayRole).toString().toStdString();
            auto overlay = model->data(model->index(i, 1), Qt::DisplayRole).toString().toStdString();
            if(level.empty() && overlay.empty()) continue;

            levels.push_back(std::make_pair(level, overlay));
        }
    }
    if(!levels.size())
        return;

    //Replace
    settings->levels = levels;     
}