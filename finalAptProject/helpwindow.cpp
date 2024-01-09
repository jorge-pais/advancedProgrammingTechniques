#include <QFile>
#include <QTextStream>
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

void HelpWindow::loadHelpContent() {
    QFile helpFile(":/help.html"); // Path to your HTML file in the resources
    if (helpFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&helpFile);
        label->setText(stream.readAll()); // Set HTML content on QLabel
        helpFile.close();
    } else {
        label->setText("Help content could not be loaded.");
    }
}


