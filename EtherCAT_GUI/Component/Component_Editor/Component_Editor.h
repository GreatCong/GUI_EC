#ifndef COMPONENT_EDITOR_H
#define COMPONENT_EDITOR_H

#include <QObject>
#include "Component.h"
#include "Code_editor/CodeEditor_form.h"

Q_DECLARE_INTERFACE(Component, Component_iid) //定义接口

///
/// \brief 代码编辑器组件
///
class Component_Editor : public QObject,public Component
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID Component_iid FILE "Component_Editor.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(Component)

public:
    explicit Component_Editor(QObject *parent = nullptr);
    ~Component_Editor();

public:
    bool Init_Cores();
    bool Destroy_Cores();

    virtual void set_AppPath(const QString path);//重写下
private:
    CodeEditor_form *m_codeEditor_form;
    void System_BottomMessage(const QString &message);
signals:
    void Sig_Send_MSG(const QString &message);
public slots:
private slots:
    void Plugins_codeEditor_MSG(const QString str);
};

#endif // COMPONENT_EDITOR_H
