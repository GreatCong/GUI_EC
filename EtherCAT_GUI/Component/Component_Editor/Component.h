#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QWidget>
#include <QApplication>
#include <QStyle>

#define Component_iid "DRE.My_Interface_Component"//定义ID

class Component
{
public:
    Component() {
     m_componentWidget = nullptr;
     m_appIcon = QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon);
     m_appName = "User_Component";
    }
    virtual ~Component(){}
    virtual bool Init_Cores() = 0;
    virtual bool Destroy_Cores() = 0;

    virtual QString get_ApppPath(){ return m_appPath;}
    virtual void set_AppPath(const QString path){m_appPath = path;}

    virtual QIcon get_AppIcon(){ return m_appIcon; }
    virtual void set_AppIcon(const QIcon icon) { m_appIcon = icon; }

    virtual QString get_AppName(){ return m_appName;}
    virtual void set_AppName(const QString appame){ m_appName = appame; }

    virtual QWidget *get_componentWidgetPtr(){return m_componentWidget;}
    virtual void set_componentWidgetPtr(QWidget *widget){ m_componentWidget = widget;}
protected:
    QString m_appPath;
    QWidget *m_componentWidget;
    QIcon m_appIcon;
    QString m_appName;
};

#endif // COMPONENT_H
