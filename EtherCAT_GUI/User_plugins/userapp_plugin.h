#ifndef USERAPP_PLUGIN_H
#define USERAPP_PLUGIN_H

#include <QGenericPlugin>
#include "../Qt_VS_Dock/my_UserAppWidge.h"
#include "form_plot.h"

class UserApp_plugin : public QGenericPlugin,My_UserAppWidget
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID UserApp_iid FILE "User_plugins.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(My_UserAppWidget)
private:
    QWidget *m_userWidget;
    ~UserApp_plugin();
public:
    UserApp_plugin(QObject *parent = 0);

    QObject *create(const QString &name, const QString &spec);
    QWidget *getWidget();
    void Init_Apps();

    bool Set_Address(uint8_t *address);
    bool GetAddress(uint8_t *address);
    bool GetSlaveAddress(uint8_t *address, int slaveNum);
};

#endif // USERAPP_PLUGIN_H
