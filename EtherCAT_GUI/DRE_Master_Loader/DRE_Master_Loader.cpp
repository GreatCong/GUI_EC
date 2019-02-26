#include "DRE_Master_Loader.h"

//#include "my_EthercatMaster.h"
#include <Qdir>
#include <QDebug>

Q_DECLARE_INTERFACE(DRE_Master, Master_iid) //定义接口


DRE_Master_Loader::DRE_Master_Loader(QObject *parent) : QObject(parent)
{

}

DRE_Master* DRE_Master_Loader::Master_load(const QString &fileName)
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
        if(plugin_type.compare("Master")){//不是主站
//            qDebug() <<"OK";
//            QMessageBox::information(this,tr("Information"),tr("Plugin type is not \"Master\" !"));
            return nullptr;
        }
       // 访问感兴趣的接口
       DRE_Master *master = qobject_cast<DRE_Master *>(pPlugin);
       if (master != Q_NULLPTR) {
           return master;
       } else {
           return nullptr;
       }
    }

    return nullptr;

}

bool DRE_Master_Loader::Master_unload()
{
    m_Plugin_Loader->unload();
    delete m_Plugin_Loader;

    return true;
}

void DRE_Master_Loader::set_pluginDir(const QString &dir)
{
    m_pluginDir = dir;
}
