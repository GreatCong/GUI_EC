# Component_src

#Dock_obj
SOURCES += \
    $$PWD/Dock_Obj_Utils.cpp

HEADERS += \
    $$PWD/Dock_Obj_Utils.h

INCLUDEPATH += $$PWD


#GeneralTab
SOURCES += \
    $$PWD/General_tab/form_generaltab.cpp \
    $$PWD/General_tab/Component_EtherCAT.cpp \
    $$PWD/General_tab/EtherCAT_ScanThread.cpp

HEADERS += \
    $$PWD/General_tab/form_generaltab.h \
    $$PWD/General_tab/Component_EtherCAT.h \
    $$PWD/General_tab/EtherCAT_ScanThread.h

FORMS += $$PWD/General_tab/form_GeneralTab.ui
INCLUDEPATH += $$PWD/General_tab

#Component_DockBottom
SOURCES += \
    $$PWD/Component_DockBottom/Component_DockBottom.cpp

HEADERS += \
    $$PWD/Component_DockBottom/Component_DockBottom.h

INCLUDEPATH += $$PWD/Component_DockBottom

#Component_DockLeft
SOURCES += \
    $$PWD/Component_DockLeft/Component_DockLeft.cpp \
    $$PWD/Component_DockLeft/my_treeview_device.cpp \
    $$PWD/Component_DockLeft/my_Table_SlaveMSG.cpp \
    $$PWD/Component_DockLeft/VirtualDevice_Item.cpp

HEADERS += \
    $$PWD/Component_DockLeft/Component_DockLeft.h \
    $$PWD/Component_DockLeft/my_treeview_device.h \
    $$PWD/Component_DockLeft/my_Table_SlaveMSG.h \
    $$PWD/Component_DockLeft/VirtualDevice_Item.h

INCLUDEPATH += $$PWD/Component_DockLeft

#Component_DockRight
SOURCES += \
    $$PWD/Component_DockRight/Component_DockRight.cpp \
    $$PWD/Component_DockRight/my_ListView_plugin.cpp

HEADERS += \
    $$PWD/Component_DockRight/Component_DockRight.h \
    $$PWD/Component_DockRight/my_ListView_plugin.h

INCLUDEPATH += $$PWD/Component_DockRight
