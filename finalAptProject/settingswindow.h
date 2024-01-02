#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <memory>
#include <QDialogButtonBox>
#include <QFileDialog>

#include "settings.h"

class Settings;

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr, std::shared_ptr<Settings> set = nullptr);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
    std::shared_ptr<Settings> settings;

    std::string chooseFile();

private slots:
    void accept();
    void reject();

    void chooseTile();
    void chooseLevel();
    void chooseOverlay();

};

#endif // SETTINGSWINDOW_H
