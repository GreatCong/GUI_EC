#ifndef USERAPP_PLUGIN_H
#define USERAPP_PLUGIN_H

#include <QGenericPlugin>
#include "../Qt_VS_Dock/EtherCAT_UserApp.h"
#include "form_plot.h"

class UserApp_plugin : public QGenericPlugin,EtherCAT_UserApp
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID UserApp_iid FILE "User_plugins.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(EtherCAT_UserApp)
private:
    QWidget *m_userWidget;
    ~UserApp_plugin();
public:
    UserApp_plugin(QObject *parent = 0);

    QObject *create(const QString &name, const QString &spec);
    void Init_Cores();
    void Destroy_Cores();
};

#endif // USERAPP_PLUGIN_H
