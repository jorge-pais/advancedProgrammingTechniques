#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QScrollArea>

class HelpWindow : public QDialog {
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *parent = nullptr);
    void setHelpText(const QString &text);
    void loadHelpContent();

private:
    QLabel *label;
};


#endif // HELPWINDOW_H
