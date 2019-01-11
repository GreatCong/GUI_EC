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

#unix
#{
#  unix:include("modules/SOEM-linux.pri")
#  #指定生成的应用程序放置的目录
#  unix: DESTDIR += $$PWD/build/linux/bin
#  #指定目标文件(obj)的存放目录
#  unix: OBJECTS_DIR += $$PWD/build/linux/obj
#}

win32
{
  win32:DEFINES +=_CRT_SECURE_NO_WARNINGS __STDC_LIMIT_MACROS
  # wincap中w2def.h与winsock.h定义冲突，定义WIN32_LEAN_AND_MEAN宏防止windows.h有可能包含winsock.h头文件,但是会有其他未定义的问题！！
#  win32:DEFINES += WIN32_LEAN_AND_MEAN
  win32:include("modules/SOEM-win.pri")
  win32:include("modules/pthread-win.pri")
#  win32:HEADERS += user/c_source/platform/ethercat_compatible_win.h
#  #指定生成的应用程序放置的目录
#  win32: DESTDIR += $$PWD/build/win32/bin
#  #指定目标文件(obj)的存放目录
#  win32: OBJECTS_DIR += $$PWD/build/win32/obj
}

# qcustomplot
include("modules/qcustomplot.pri")

SOURCES += \
    $$PWD/Ethercat_class/my_ethercatmaster.cpp \
    $$PWD/Ethercat_class/MMTimer_RTwin.cpp \
    $$PWD/Ethercat_class/MMTimer_RT.cpp \
    $$PWD/Ethercat_class/master_check.c \
    mainview_left.cpp \
    mianview_right.cpp \
    mainview_center.cpp \
    mainview_bottom.cpp \
    mainview_utils.cpp \
    mainview_public.cpp \
    mainview_test.cpp \
    tables/gcodetablemodel.cpp \
    thread_test.cpp \
    Ethercat_class/Ethercat_Utile.cpp \
    Ethercat_class/RTTimer.cpp \
    GeneralTab_P.cpp

HEADERS += \
    $$PWD/Ethercat_class/my_ethercatmaster.h \
    $$PWD/Ethercat_class/MMTimer_RTwin.h \
    $$PWD/Ethercat_class/MMTimer_RT.h \
    $$PWD/Ethercat_class/master_check.h \
    tables/gcodetablemodel.h \
    thread_test.h \
    Ethercat_class/Ethercat_Utile.h \
    Ethercat_class/RTTimer.h \
    EtherCAT_UserApp.h \
    GeneralTab_P.h

INCLUDEPATH += $$PWD/Ethercat_class
INCLUDEPATH += $$PWD/tables


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    my_treeview_device.cpp \
    my_lisetview_plugin.cpp \
    mainformview.cpp \   
    form_generaltab.cpp \    
    my_Table_SlaveMSG.cpp \
    ethercat_apptest.cpp


HEADERS += \
        mainwindow.h \
    mainformview.h \
    my_treeview_device.h \
    user_common.h \
    my_lisetview_plugin.h \
    form_generaltab.h \
    my_Table_SlaveMSG.h

FORMS += \
        mainwindow.ui \
    mainformview.ui \
    form_GeneralTab.ui

RESOURCES += \
    res_icons.qrc
