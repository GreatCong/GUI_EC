#include "Measure_plugins.h"
#include <QGridlayout>
#include <QPushButton>

Measure_plugin::Measure_plugin(QObject *parent) :
    QObject(parent),EtherCAT_UserApp()
{
    m_userWidget = new Form_plot();

    set_UIWidgetPtr(m_userWidget);
    set_CallbackPtr(nullptr);
    set_MessageObj(nullptr);
}

Measure_plugin::~Measure_plugin(){
//    delete m_userWidget;
}

void Measure_plugin::Init_Cores()
{

}

void Measure_plugin::Destroy_Cores()
{

}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Measure_plugins, Measure_plugin)
#endif // QT_VERSION < 0x050000
