#-------------------------------------------------
#
# Date:
# Author: liucongjun
# Version: 1.1
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QT       += xml

TARGET = Qt_VS_Dock
TEMPLATE = app

#CONFIG += console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug,debug|release):{
 message($$TARGET "debug")
unix:TARGET=$$join(TARGET,,,_debug)
win32:TARGET=$$join(TARGET,,,_d)
CONFIG += console
#LIBS += $$PWD/Ethercat_class/DRE_Master_Loader_d.lib
}
else {
 message($$TARGET "release")
#LIBS += $$PWD/Ethercat_class/DRE_Master_Loader.lib
}

win32{
  LIBS += Winmm.lib
}

#plugin(only header)
include(Plugin_common/Plugin_common.pri)
#EtherCAT_class
include(Ethercat_class/Ethercat_class.pri)
#qt_propertybrowser
include(qt_propertybrowser/qt_propertybrowser.pri )
#Component_src
include(Component_src/Component_src.pri)
#Component_Class
include(Component_class/Component_class.pri)
#MainView
include(MainView/MainView.pri)

#user_common
HEADERS += $$PWD/user_common.h

#Others
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp

HEADERS += \
    $$PWD/mainwindow.h

FORMS += \
    $$PWD/mainwindow.ui

RESOURCES += \
    $$PWD/res_icons.qrc

#Add Application Icon
win32:RC_FILE = $$PWD/Resource/Icons/res.rc

message($$QT_INSTALL_PREFIX)
