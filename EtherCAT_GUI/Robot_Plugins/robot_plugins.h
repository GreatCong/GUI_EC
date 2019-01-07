#ifndef ROBOT_PLUGINS_H
#define ROBOT_PLUGINS_H

#include <QGenericPlugin>
#include "../Qt_VS_Dock/my_UserAppWidge.h"

class Robot_Plugins : public QGenericPlugin,My_UserAppWidget
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID UserApp_iid FILE "Robot_Plugins.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(My_UserAppWidget)
public:
    Robot_Plugins(QObject *parent = 0);

    QObject *create(const QString &name, const QString &spec);
    QWidget *getWidget();
    void Init_Apps();

    bool Set_Address(uint8_t * address);
    bool GetAddress(uint8_t * address);
   bool GetSlaveAddress(uint8_t * address,int slaveNum);
private:
    QWidget *m_userWidget;
//    ~Robot_Plugins();//加入delete好像会出错
};

#endif // ROBOT_PLUGINS_H
