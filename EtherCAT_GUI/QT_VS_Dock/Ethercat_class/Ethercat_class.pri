
#LIBS += $$PWD/Ethercat_class/DRE_Master_Loader.lib
#EtherCAT
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/Ethercat_Utile.cpp \
    $$PWD/DRE_Master_Loader.cpp \
    $$PWD/DRE_Master.cpp

HEADERS += \
    $$PWD/Ethercat_Utile.h \
    $$PWD/DRE_Master_Loader.h \
    $$PWD/DRE_Master.h

win32{
# MMTimer_RTwin
HEADERS += \
    $$PWD/MMTimer_RTwin/RTTimer.h

SOURCES += \
    $$PWD/MMTimer_RTwin/RTTimer.cpp
}
