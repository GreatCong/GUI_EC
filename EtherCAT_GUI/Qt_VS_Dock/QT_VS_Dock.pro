#-------------------------------------------------
#
# Date:
# Author: liucongjun
# Version: 1.1
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Qt_VS_Dock
TEMPLATE = app

CONFIG += console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
  LIBS += Winmm.lib
}

#EtherCAT
INCLUDEPATH += $$PWD/Ethercat_class

SOURCES += \
    $$PWD/Ethercat_class/MMTimer_RTwin.cpp \
    $$PWD/Ethercat_class/MMTimer_RT.cpp \
    $$PWD/Ethercat_class/Ethercat_Utile.cpp \
    $$PWD/Ethercat_class/RTTimer.cpp \
    $$PWD/Ethercat_class/DRE_Master_Loader.cpp \
    $$PWD/Ethercat_class/DRE_Master.cpp

HEADERS += \
    $$PWD/Ethercat_class/MMTimer_RTwin.h \
    $$PWD/Ethercat_class/MMTimer_RT.h \
    $$PWD/Ethercat_class/Ethercat_Utile.h \
    $$PWD/Ethercat_class/RTTimer.h \
    $$PWD/Ethercat_class/DRE_Master_Loader.h \
    $$PWD/Ethercat_class/DRE_Master.h

#plugin
INCLUDEPATH += $$PWD/Plugin_common

HEADERS += \
    $$PWD/Plugin_common/EtherCAT_UserApp.h

#GeneralTab
SOURCES += \
    GeneralTab_P.cpp \
    form_generaltab.cpp
HEADERS += \
    GeneralTab_P.h \
    form_generaltab.h

#Others
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    my_treeview_device.cpp \
    my_lisetview_plugin.cpp \
    mainformview.cpp \      
    my_Table_SlaveMSG.cpp \
    ethercat_apptest.cpp \
    mainview_left.cpp \
    mianview_right.cpp \
    mainview_center.cpp \
    mainview_bottom.cpp \
    mainview_utils.cpp \
    mainview_public.cpp \
    mainview_test.cpp \


HEADERS += \
        mainwindow.h \
    mainformview.h \
    my_treeview_device.h \
    user_common.h \
    my_lisetview_plugin.h \
    my_Table_SlaveMSG.h

FORMS += \
        mainwindow.ui \
    mainformview.ui \
    form_GeneralTab.ui

RESOURCES += \
    res_icons.qrc

#Add Application Icon
RC_FILE = $$PWD/Resource/Icons/res.rc
