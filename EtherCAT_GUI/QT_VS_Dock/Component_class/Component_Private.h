#ifndef COMPONENT_PRIVATE_H
#define COMPONENT_PRIVATE_H

#include "Component.h"
#include "DRE_Master.h"
#include "EtherCAT_UserApp.h"
#include "user_common.h" //防止中文乱码的

#include <QPlainTextEdit>
#include <QList>
#include <QWidget>

///
/// \brief The ComponentWidgets class
///
class ComponentWidgets:public QObject
{
    Q_OBJECT
public:
    ComponentWidgets() {
        QObject();
        m_tabWedget_center = nullptr;
        m_widget_slaveMSG = nullptr;
        m_tabWeidgetItem_General = nullptr;
        m_tabWeidgetItem_UserApps = nullptr;
        m_userPropertyWidget = nullptr;
        m_controllerPropertyWidget = nullptr;
        m_userAppPtr = nullptr;
    }
    virtual ~ComponentWidgets(){}
    void Set_StatusMessage(QString message, int interval)
    {
        emit sigStatusMessage_change(message,interval);//发出自定义信号
    }

    void Set_BottomMessage(QString message)
    {
        emit sigBottomMessage_change(message);//发出自定义信号
    }

    void Set_MasterStop()
    {
        emit sigMasterStop_Signal();//发出自定义信号
    }

    void Set_MasterState(int state)
    {
        emit sigMaster_setState(state);
    }

    void Set_AppPathChange(QString &appPath)
    {
        emit sigAppPath_change(appPath);
    }
public:
    typedef enum{
        Tab_General,
        Tab_UserApp
    }centerWidget_Tab_choose;

    typedef enum{
        Cmd_Master_scan,
        Cmd_Master_run,
        Cmd_Master_stop
    }Master_Require_e;

    QWidget *m_tabWedget_center;//中心主窗口
    QWidget *m_widget_slaveMSG;//slave信息窗口
    QWidget *m_tabWeidgetItem_General;//tab1 general
    QWidget *m_tabWeidgetItem_UserApps;//tab2 userApp
    QWidget *m_userPropertyWidget;//多个应用用的属性窗口
    QWidget *m_controllerPropertyWidget;//单个应用用的属性窗口
    EtherCAT_UserApp *m_userAppPtr;//从插件中加载的对象
signals:
    void sigMaster_setState(int state);
    void sigStatusMessage_change(QString message,int interval);//状态栏信息
    void sigBottomMessage_change(QString message);//bottom Text message
    void sigMasterStop_Signal();//stop master
    void sigAppPath_change(QString &appPath);//stop master
};

///
/// \brief 系统组件库(用于UI显示,增加Master的调用)
///
class Component_Private : public Component
{
public:
    Component_Private() {
        m_masterPtr = nullptr;
        m_MessageText = nullptr;
        m_ComponentList = nullptr;
        m_ComponentWidgets = nullptr;
    }
    virtual ~Component_Private(){}
    virtual bool Init_Object() { return true;}
    virtual bool Init_Slots()  { return true;}

    virtual bool Init_Cores() { return true;} //后面用到了List,要实例化
    virtual bool Destroy_Cores() { return true;} //后面用到了List,要实例化

    virtual DRE_Master *ComponentP_get_MasterPtr(){ return  m_masterPtr;}
    virtual void ComponentP_set_MasterPtr(DRE_Master *master){ m_masterPtr = master;}

    virtual void ComponentP_set_MessageTextPtr(QPlainTextEdit *messageText){ m_MessageText = messageText; }
    virtual QPlainTextEdit *ComponentP_get_MessageTextPtr(void){ return m_MessageText; }

    virtual ComponentWidgets *ComponentP_get_ComponentWidgetsPtr(){ return  m_ComponentWidgets;}
    virtual void ComponentP_set_ComponentWidgetsPtr(ComponentWidgets *widgets){ m_ComponentWidgets = widgets;}

    virtual void ComponentP_set_ComponentListPtr(QList<Component_Private *> *list){ m_ComponentList = list; }
    virtual QList<Component_Private *> *ComponentP_get_ComponentListPtr(void){ return m_ComponentList; }

    virtual void ComponentP_master_ScanStart(){}
    virtual void ComponentP_master_ScanFinish(){}
    virtual void ComponentP_master_RunStart(){}
    virtual void ComponentP_master_RunFinish(){}
    virtual void ComponentP_master_StopStart(){}
    virtual void ComponentP_master_StopFinish(){}
protected:
    DRE_Master *m_masterPtr;//主站指针
    QPlainTextEdit *m_MessageText;//输出框指针
    ComponentWidgets *m_ComponentWidgets;//对外窗口的集合
    QList<Component_Private *> *m_ComponentList;//存储每个组件的this指针（注意都是Component指针）
};

#define Component_Private_iid "DRE.My_Interface_Component_Private"//定义ID
Q_DECLARE_INTERFACE(Component_Private, Component_Private_iid) //定义接口

#endif // COMPONENT_PRIVATE_H
