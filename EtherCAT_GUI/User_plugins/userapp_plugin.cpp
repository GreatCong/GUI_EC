#include "userapp_plugin.h"
#include <QGridlayout>
#include <QPushButton>

UserApp_plugin::UserApp_plugin(QObject *parent) :
    QGenericPlugin(parent),EtherCAT_UserApp()
{
    m_userWidget = new Form_plot();

    set_UIWidgetPtr(m_userWidget);
    set_CallbackPtr(nullptr);
    set_MessageObj(nullptr);
}

UserApp_plugin::~UserApp_plugin(){
//    delete m_userWidget;
}

///
/// \brief QGenericPlugin中的虚函数实现
/// \param name
/// \param spec
/// \return
///
QObject* UserApp_plugin::create(const QString &name, const QString &spec){
    Q_UNUSED(name);
    Q_UNUSED(spec);

    return 0;
}

void UserApp_plugin::Init_Cores()
{

}

void UserApp_plugin::Destroy_Cores()
{

}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(User_plugins, UserApp_plugin)
#endif // QT_VERSION < 0x050000
