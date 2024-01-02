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
    connect(ui->pushButton_2, &QPushButton::clicked, this, &SettingsWindow::chooseLevel);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &SettingsWindow::chooseOverlay);

    ui->lineEdit->setText(QString::fromStdString(settings->getTileSet()));
    ui->lineEdit_2->setText(QString::fromStdString(settings->getLevel()));
    ui->lineEdit_3->setText(QString::fromStdString(settings->getOverlay()));
}

std::string SettingsWindow::chooseFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if(!fileName.isEmpty())
        return fileName.toStdString();

    return "";
}

void SettingsWindow::chooseTile(){ ui->lineEdit->setText(QString::fromStdString(chooseFile())); }
void SettingsWindow::chooseLevel(){ ui->lineEdit_2->setText(QString::fromStdString(chooseFile())); }
void SettingsWindow::chooseOverlay(){ ui->lineEdit_3->setText(QString::fromStdString(chooseFile())); }

SettingsWindow::~SettingsWindow(){
    delete ui;
}

void SettingsWindow::accept(){
    settings->loadTileSet(ui->lineEdit->displayText().toStdString());
    
    QDialog::accept();
}

void SettingsWindow::reject(){
    QDialog::reject();
}