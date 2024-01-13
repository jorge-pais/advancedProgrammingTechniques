
#include "helpwindow.h"

#include "qloggingcategory.h"
QLoggingCategory helpWindowCat("helpWindow");

HelpWindow::HelpWindow(QWidget *parent) : QDialog(parent), label(new QLabel(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    label->setText("This is the help text...");
    label->setWordWrap(true);

    // Create a QScrollArea to make the QLabel scrollable
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // Make the scroll area resizable
    scrollArea->setWidget(label); // Set the QLabel as the widget for the scroll area

    layout->addWidget(scrollArea); // Add the scroll area to the layout instead of the label
}


void HelpWindow::setHelpText(const QString &text) {
    label->setText(text);
}

void HelpWindow::loadHelpContent() {
    qCDebug(helpWindowCat) << "loadHelpContent() is called";
       QString filePath = ":/help.html"; // If it's a file in the application's directory
       QFile helpFile(filePath);
       if (helpFile.exists()) {
           qCDebug(helpWindowCat) << "File exists: " << filePath;
           if (helpFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
               qCDebug(helpWindowCat) << "File open";
               QTextStream stream(&helpFile);
               label->setText(stream.readAll()); // Set HTML content on QLabel
               helpFile.close();
           } else {
               qCDebug(helpWindowCat) << "Failed to open file";
               label->setText("Help content could not be loaded.");
           }
       } else {
           qCDebug(helpWindowCat) << "File does not exist: " << filePath;
           label->setText("Help file not found.");
       }
}


