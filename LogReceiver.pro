#-------------------------------------------------
#
# Project created by QtCreator 2013-08-07T17:05:37
#
#-------------------------------------------------

QT       += core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogReceiver
TEMPLATE = app

INCLUDEPATH += ./  utils

SOURCES += main.cpp\
        mainwindow.cpp \
    netreceiver.cpp \
    utils/filterlineedit.cpp \
    utils/fancylineedit.cpp \
    utils/historycompleter.cpp \
    utils/qtcassert.cpp \
    settingsdialog.cpp \
    utils/qcolorbutton.cpp

HEADERS  += mainwindow.h \
    netreceiver.h \
    utils/filterlineedit.h \
    utils/fancylineedit.h \
    utils/historycompleter.h \
    utils/qtcassert.h \
    settingsdialog.h \
    utils/qcolorbutton.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    res.qrc

RC_FILE += LogReceiver.rc
