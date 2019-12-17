#ifndef DRE_MASTER_LOADER_H
#define DRE_MASTER_LOADER_H

#include <QObject>
#include <QPluginLoader>

#include "DRE_Master.h"

class DRE_Master_Loader : public QObject
{
    Q_OBJECT
public:
    explicit DRE_Master_Loader(QObject *parent = nullptr);
public:
    DRE_Master *Master_load(const QString &fileName);
    bool Master_unload();
    void set_pluginDir(const QString &dir);
protected:
    QString m_pluginDir;
    QPluginLoader *m_Plugin_Loader;
signals:

public slots:
};

#endif // DRE_MASTER_LOADER_H
