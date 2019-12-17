#ifndef COMPONENT_DOCKLEFT_H
#define COMPONENT_DOCKLEFT_H

#include "Component_Private.h"
#include "my_treeview_device.h"
#include "objectcontroller/objectcontroller.h"
#include "my_Table_SlaveMSG.h"
#include "EtherCAT_UserApp.h"
#include "VirtualDevice_Item.h"

#include <QScrollArea>
#include <QTabWidget>
#include <QMenu>
#include <QListWidget>
#include <QGroupBox>

class Component_DockLeft:public QObject,public Component_Private
{
    Q_OBJECT
    Q_INTERFACES(Component_Private)
public:
    Component_DockLeft(QObject *parent = nullptr);
    virtual ~Component_DockLeft();
public:
    virtual bool Init_Cores();
    virtual bool Destroy_Cores();
    virtual bool Init_Object();
    virtual bool Init_Slots();

    virtual void ComponentP_master_ScanStart();
    virtual void ComponentP_master_ScanFinish();
private:
    void Init_DeviceTreeItem_menu();
    int VirtualDevice_LoadXMLs(QString filePath);
    VirtualDevice_item *VirtualDevice_ParseFileXML(QString fileName);
    void setBottom_message(QString str);
    int VirtualDevice_ParseTypeXML(QString str_type);
    int VirtualDevice_RegistMaster();
    void Init_DeviceTree_slots();

    My_TreeView_Device *get_DeviceTreePtr();
    void set_DeviceTreePtr(My_TreeView_Device *deviceTree);

    void set_WidgetObjectController(ObjectController *widget){ m_widgetObjectController = widget;}
    ObjectController *get_WidgetObjectController(){ return m_widgetObjectController; }

    EtherCAT_UserApp *get_userAppPtr() { return this->ComponentP_get_ComponentWidgetsPtr()->m_userAppPtr; }
    void set_userAppPtr(EtherCAT_UserApp *userApp) { this->ComponentP_get_ComponentWidgetsPtr()->m_userAppPtr = userApp; }
private:
    typedef enum
    {
       DRE_ECT_BOOLEAN         = 0x0001,
       DRE_ECT_INTEGER8        = 0x0002,
       DRE_ECT_INTEGER16       = 0x0003,
       DRE_ECT_INTEGER32       = 0x0004,
       DRE_ECT_UNSIGNED8       = 0x0005,
       DRE_ECT_UNSIGNED16      = 0x0006,
       DRE_ECT_UNSIGNED32      = 0x0007,
       DRE_ECT_REAL32          = 0x0008,
       DRE_ECT_VISIBLE_STRING  = 0x0009,
       DRE_ECT_OCTET_STRING    = 0x000A,
       DRE_ECT_UNICODE_STRING  = 0x000B,
       DRE_ECT_TIME_OF_DAY     = 0x000C,
       DRE_ECT_TIME_DIFFERENCE = 0x000D,
       DRE_ECT_DOMAIN          = 0x000F,
       DRE_ECT_INTEGER24       = 0x0010,
       DRE_ECT_REAL64          = 0x0011,
       DRE_ECT_INTEGER64       = 0x0015,
       DRE_ECT_UNSIGNED24      = 0x0016,
       DRE_ECT_UNSIGNED64      = 0x001B,
       DRE_ECT_BIT1            = 0x0030,
       DRE_ECT_BIT2            = 0x0031,
       DRE_ECT_BIT3            = 0x0032,
       DRE_ECT_BIT4            = 0x0033,
       DRE_ECT_BIT5            = 0x0034,
       DRE_ECT_BIT6            = 0x0035,
       DRE_ECT_BIT7            = 0x0036,
       DRE_ECT_BIT8            = 0x0037
    } DRE_datatype;

    My_TreeView_Device *m_DeviceTree;//设备树
    ObjectController *m_widgetObjectController;//独立插件的属性窗口
    My_Table_SlaveMSG *m_tableView_slaveMSG;
    My_Table_SlaveItemMSG *m_tableView_slaveItemMSG;
    QScrollArea *m_userPropertyWidget;//插件部分的属性窗口

    QTabWidget *m_tabWidget;
    QScrollArea *m_tabItem_DeviceTree;
    QScrollArea *m_tabItem_virtualDevice;

    QListWidget *m_ListWidget_virtualDevice_xml;
    QListWidget *m_ListWidget_virtualDevice_select;
    QList<VirtualDevice_item *> m_virutalDevice_selectList;

    QMenu* m_projectMenu;
    QMenu* m_itemMenu;

    QWidget *m_DockWidget;
private slots:
    void mDeviceTree_customContextMenuRequested(const QPoint &pos);
    void mDeviceTree_DoubleClicked(const QModelIndex index);
    void mDeviceTree_expanded(const QModelIndex index);

    void virtualDeviceXML_itemDoubleClicked(QListWidgetItem* item);
    void virtualDeviceSelect_itemDoubleClicked(QListWidgetItem* item);

    /*** 测试开始 ***/
    void mDeviceTree_EcScan_clicked();
    void AddTree_Master_clicked();
    void AddTree_Slave_clicked();
    void AddTree_SlaveItem_clicked();
    void AddTree_SlaveMessageInput_clicked();
    void AddTree_SlaveMessageOutput_clicked();
    /*** 测试结束 ***/
signals:
};

#endif // COMPONENT_DOCKLEFT_H
