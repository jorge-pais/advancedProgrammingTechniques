#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <memory>

#include "settings.h"

class Settings;

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr, std::shared_ptr<Settings> settings = nullptr);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
    std::shared_ptr<Settings> settings;
};

#endif // SETTINGSWINDOW_H
