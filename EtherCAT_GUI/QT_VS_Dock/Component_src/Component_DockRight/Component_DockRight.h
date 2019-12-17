#ifndef COMPONENT_RIGHT_H
#define COMPONENT_RIGHT_H

#include "Component_Private.h"
#include "my_ListView_plugin.h"

#include "Component_Loader.h"
#include "EtherCAT_UserApp.h"

#include <QTableWidget>

///
/// \brief The Component_DockRight class
///
class Component_DockRight:public QObject,public Component_Private
{
    Q_OBJECT
    Q_INTERFACES(Component_Private)
public:
    Component_DockRight(QObject *parent = nullptr);
    virtual ~Component_DockRight();
public:
    virtual bool Init_Cores();
    virtual bool Destroy_Cores();
    virtual bool Init_Object();
    virtual bool Init_Slots();
private:
    int LoadPlugins();
    bool LoadPlugins(const QString &fileName);
    int ScanPlugins(const QString &plugin_dir);
    bool Init_Cores_Editor();
    void System_BottomMessage(const QString &message);

    EtherCAT_UserApp *get_userAppPtr() { return this->ComponentP_get_ComponentWidgetsPtr()->m_userAppPtr; }
    void set_userAppPtr(EtherCAT_UserApp *userApp) { this->ComponentP_get_ComponentWidgetsPtr()->m_userAppPtr = userApp; }

private:
    My_LisetView_Plugin *m_pluginList;
    QTableWidget *mTableWidget_property;

    Component_Loader *m_component_loader;
    Component *m_component_Editor;

    QWidget *m_DockWidget;
    QPluginLoader *m_Plugin_Loader;
private slots:
    void Plugins_Manager_clicked();
    void mPlugins_item_DoubleClicked(QListWidgetItem *item);
    void mPlugins_item_Clicked(QListWidgetItem *item);
    void Plugins_OpenEditor_clicked();
    void Plugins_codeEditor_MSG(const QString str);
    void Plugins_Add_clicked();
    void Control_StatusMessage_change(QString message, int interval);
    void Control_BottomMessage_change(QString message);
    void Control_MasterStop_Signal();
signals:
};

#endif // COMPONENT_RIGHT_H
