#include "userapp_plugin.h"
#include <QGridlayout>
#include <QPushButton>

UserApp_plugin::UserApp_plugin(QObject *parent) :
    QGenericPlugin(parent)
{
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

///
/// \brief UserApp_plugin::widget_contents 虚函数实现
/// \return
///
QWidget* UserApp_plugin::getWidget(){

    return m_userWidget;
}

///
/// \brief UserApp_plugin::Init_Apps
///
void UserApp_plugin::Init_Apps(){
//    m_userWidget = new QWidget();
//    QGridLayout *user_layout = new QGridLayout();
//    m_userWidget->setLayout(user_layout);
//    m_userWidget->layout()->addWidget(new QPushButton("User_Apps"));
    m_userWidget = new Form_plot();
}

bool UserApp_plugin::Set_Address(uint8_t *address)
{
    Q_UNUSED(address);
    return true;

}

bool UserApp_plugin::GetAddress(uint8_t *address)
{
    Q_UNUSED(address);
    return true;
}

bool UserApp_plugin::GetSlaveAddress(uint8_t *address, int slaveNum)
{
    Q_UNUSED(address);
    Q_UNUSED(slaveNum);
    return true;
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(User_plugins, UserApp_plugin)
#endif // QT_VERSION < 0x050000
