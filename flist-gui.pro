QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += "$$PWD/include"

VPATH += "$$PWD/src" "$$PWD/include"

SOURCES += main.cpp \
    gui.cpp \
    filelist.cpp \
    config.cpp

HEADERS += \
    gui.h \
    filelist.h \
    config.h

DEFINES += "FLISTVERSION='\"3.1.4\"'"
