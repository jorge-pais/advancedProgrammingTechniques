#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, std::shared_ptr<Settings> settings)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
    , settings(settings)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}
