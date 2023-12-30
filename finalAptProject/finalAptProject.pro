QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20 DQT_NO_VERSION_TAGGING
QMAKE_CXXFLAGS += -std=c++20


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graphicalview.cpp \
    main.cpp \
    mainwindow.cpp \
    protagonistsprite.cpp \
    spritewithvalue.cpp \
    textview.cpp \
    worlddelegate.cpp \
    worldview.cpp \
    xenemy.cpp

HEADERS += \
    graphicalview.h \
    mainwindow.h \
    pathfinderfunction.h \
    pathfinder.h \
    pathfinder_class.h \
    protagonistsprite.h \
    spritewithvalue.h \
    textview.h \
    world.h \
    world_global.h \
    worlddelegate.h \
    worldview.h \
    xenemy.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../worldlib/ -lworld

INCLUDEPATH += $$PWD/../../worldlib
DEPENDPATH += $$PWD/../../worldlib

RESOURCES += \
    images.qrc

