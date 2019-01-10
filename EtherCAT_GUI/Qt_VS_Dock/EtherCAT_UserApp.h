/*
 * 用户调用EtherCAT应用的头文件
 */

#ifndef ETHERCAT_USERAPP_H
#define ETHERCAT_USERAPP_H

#include <Qwidget>

class Ethercat_Callback
{
public:
    Ethercat_Callback() {}
public:
    virtual void Master_AppLoop_callback() = 0;
    virtual void Master_AppStart_callback() = 0;
    virtual void Master_AppStop_callback() = 0;
    virtual void Master_ReleaseAddress() =0;
    virtual int Master_setAdressBase(char* address) = 0;

    int m_slaveCount;
};

//此对象主要包含2个部分 界面UI和操作回调callback
class EtherCAT_UserApp
{
public:
    EtherCAT_UserApp() {}
public:
    virtual QWidget *get_UIWidgetPtr() {return _UIWidget;}
    virtual Ethercat_Callback *get_CallbackPtr() {return _EC_callback;}
    virtual void Init_Cores() = 0;
    virtual void Destroy_Cores() = 0;
protected:
    virtual void set_UIWidgetPtr(QWidget *widget) {_UIWidget = widget;}
    virtual void set_CallbackPtr(Ethercat_Callback *callback) { _EC_callback = callback;}

    QWidget *_UIWidget;
    Ethercat_Callback *_EC_callback;
};

#endif // ETHERCAT_USERAPP_H
