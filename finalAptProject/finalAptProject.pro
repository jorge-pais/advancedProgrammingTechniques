QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
DEFINES += QT_NO_VERSION_TAGGING


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    worlddelegate.cpp \
    worldview.cpp

HEADERS += \
    mainwindow.h \
    worlddelegate.h \
    worldview.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../worldlib/ -lworld
LIBS += -Wl,-rpath,/lib/x86_64-linux-gnu

INCLUDEPATH += $$PWD/../../worldlib
DEPENDPATH += $$PWD/../../worldlib

