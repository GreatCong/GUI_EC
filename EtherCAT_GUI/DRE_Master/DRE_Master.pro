#-------------------------------------------------
#
# Date:
# Author: liucongjun
# Version: 1.0
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DRE_Master
TEMPLATE = lib
CONFIG += plugin

#不在安装目录中生成
#DESTDIR = $$[QT_INSTALL_PLUGINS]/generic

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
unix:TARGET=$$join(TARGET,,,_debug)
win32:TARGET=$$join(TARGET,,,_d)
}

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

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/Ethercat_class

DISTFILES += \
    DRE_Master.json

SUBDIRS += \
    DRE_Master.pro

HEADERS += \
    my_EthercatMaster.h \
#    userapp_plugin.h \
    Ethercat_class/DRE_Master.h \
    Ethercat_class/master_check.h

SOURCES += \
    my_ethercatmaster.cpp \
#    userapp_plugin.cpp \
    Ethercat_class/DRE_Master.cpp \
    Ethercat_class/master_check.c
