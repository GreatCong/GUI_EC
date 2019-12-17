#ifndef COMPONENT_LOADER_H
#define COMPONENT_LOADER_H

#include <QObject>
#include <QPluginLoader>

#include "Component.h"

class Component_Loader : public QObject
{
    Q_OBJECT
public:
    explicit Component_Loader(QObject *parent = nullptr);
public:
    Component *Component_load(const QString &fileName);
    bool Component_unload();
    void set_pluginDir(const QString &dir);
protected:
    QString m_pluginDir;
    QPluginLoader *m_Plugin_Loader;
signals:

public slots:
};

#endif // COMPONENT_LOADER_H
