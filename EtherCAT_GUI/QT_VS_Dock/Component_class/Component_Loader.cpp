#include "Component_Loader.h"

#include <QDir>
#include <QDebug>

//Q_DECLARE_INTERFACE(Component, Component_iid) //定义接口


Component_Loader::Component_Loader(QObject *parent) : QObject(parent)
{

}

Component* Component_Loader::Component_load(const QString &fileName)
{
//    My_EthercatMaster * master = new My_EthercatMaster();
//    return master;

    QDir pluginsDir(m_pluginDir);

    m_Plugin_Loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
//    qDebug() << pluginsDir.absoluteFilePath(fileName);

    // 返回插件的根组件对象
    QObject *pPlugin = m_Plugin_Loader->instance();
    if (pPlugin != Q_NULLPTR) {
        QJsonObject json = m_Plugin_Loader->metaData().value("MetaData").toObject();
        QString plugin_type =  json.value("type").toVariant().toString();
        if(plugin_type.compare("Component")){//不是组件
//            qDebug() <<"OK";
//            QMessageBox::information(this,tr("Information"),tr("Plugin type is not \"Master\" !"));
            return nullptr;
        }
       // 访问感兴趣的接口
       Component *component = qobject_cast<Component *>(pPlugin);
       if (component != Q_NULLPTR) {
           return component;
       } else {
           return nullptr;
       }
    }

    return nullptr;

}

bool Component_Loader::Component_unload()
{
    m_Plugin_Loader->unload();
    delete m_Plugin_Loader;

    return true;
}

void Component_Loader::set_pluginDir(const QString &dir)
{
    m_pluginDir = dir;
}
