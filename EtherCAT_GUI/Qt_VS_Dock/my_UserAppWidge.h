#ifndef MY_USERAPPWIDGET_H
#define MY_USERAPPWIDGET_H

#include <QtPlugin>
#include <QWidget>

//插件的头文件均为纯虚函数,否则会报错
class My_UserAppWidget
{
public:
//    virtual My_UserAppWidget() {}
    virtual ~My_UserAppWidget(){}
    virtual QWidget* getWidget() = 0;//纯虚函数
    virtual void Init_Apps() = 0;

    virtual bool Set_Address(uint8_t * address) = 0;
    virtual bool GetAddress(uint8_t * address) = 0;
    virtual bool GetSlaveAddress(uint8_t * address,int slaveNum) = 0;

};

#define UserApp_iid "org.qt-project.Qt.My_Interface_userAppWidget"

Q_DECLARE_INTERFACE(My_UserAppWidget, UserApp_iid) //定义接口

#endif // MY_USERAPPWIDGET_H


