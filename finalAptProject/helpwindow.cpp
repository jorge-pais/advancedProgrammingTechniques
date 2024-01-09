#include "helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) : QDialog(parent), label(new QLabel(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    label->setText("This is the help text...");
    label->setWordWrap(true);
    layout->addWidget(label);
}

void HelpWindow::setHelpText(const QString &text) {
    label->setText(text);
}
