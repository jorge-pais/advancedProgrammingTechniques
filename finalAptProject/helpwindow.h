#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

class HelpWindow : public QDialog {
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *parent = nullptr);
    void setHelpText(const QString &text);

private:
    QLabel *label;
};


#endif // HELPWINDOW_H
