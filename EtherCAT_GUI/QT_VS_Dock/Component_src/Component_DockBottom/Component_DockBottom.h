#ifndef COMPONENT_DOCKBOTTOM_H
#define COMPONENT_DOCKBOTTOM_H

#include "Component_Private.h"

#include <QPlainTextEdit>

///
/// \brief The Component_DockBottom class
///
class Component_DockBottom:public QObject,public Component_Private
{
    Q_OBJECT
    Q_INTERFACES(Component_Private)
public:
    Component_DockBottom(QObject *parent = nullptr);
    virtual ~Component_DockBottom();
public:
    virtual bool Init_Cores();
    virtual bool Destroy_Cores();
    virtual bool Init_Object();
    virtual bool Init_Slots();
private:
    void set_bottomText_append(const QString &message);
private slots:
    void BottonText_Clear_clicked();
private:
    QWidget *m_DockWidget;
};

#endif // COMPONENT_DOCKBOTTOM_H
