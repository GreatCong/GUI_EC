#ifndef ROBOT_PLUGINS_H
#define ROBOT_PLUGINS_H

#include <QGenericPlugin>
#include "../Qt_VS_Dock/EtherCAT_UserApp.h"

class Robot_Plugins : public QGenericPlugin,EtherCAT_UserApp
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID UserApp_iid FILE "Robot_Plugins.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(EtherCAT_UserApp)
public:
    Robot_Plugins(QObject *parent = 0);

    QObject *create(const QString &name, const QString &spec);

    void Init_Cores();
    void Destroy_Cores();
private:
    QWidget *m_userWidget;
//    ~Robot_Plugins();//加入delete好像会出错
};

#endif // ROBOT_PLUGINS_H
