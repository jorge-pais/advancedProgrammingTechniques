QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 DQT_NO_VERSION_TAGGING



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graphicalview.cpp \
    main.cpp \
    mainwindow.cpp \
    worlddelegate.cpp \
    worldview.cpp

HEADERS += \
    graphicalview.h \
    mainwindow.h \
    pathfinder.h \
    world.h \
    world_global.h \
    worlddelegate.h \
    worldview.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../worldLib/ -lworld
LIBS += -Wl,-rpath,/lib/x86_64-linux-gnu

INCLUDEPATH += $$PWD/../../worldLib
DEPENDPATH += $$PWD/../../worldLib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../worldlib/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../worldlib/debug/ -lworld
else:unix: LIBS += -L$$PWD/../../worldlib/ -lworld

INCLUDEPATH += $$PWD/../../worldlib
DEPENDPATH += $$PWD/../../worldlib

RESOURCES += \
    images.qrc
