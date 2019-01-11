#include "robot_plugins.h"
#include <QGridlayout>
#include <QPushButton>

#include <QQmlApplicationEngine>
#include <QIcon>
#include "turing.h"

#include <QtQuickWidgets/QQuickWidget>

Robot_Plugins::Robot_Plugins(QObject *parent) :
    QGenericPlugin(parent),EtherCAT_UserApp()
{
    //    m_userWidget = new QWidget();
    //    QGridLayout *user_layout = new QGridLayout();
    //    m_userWidget->setLayout(user_layout);
    //    m_userWidget->layout()->addWidget(new QPushButton("Robots"));

        qmlRegisterType<Turing>("io.qt.turing", 1, 0, "Turning");

        QQuickWidget *quickWidget = new QQuickWidget();
        quickWidget->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    //    quickWidget->show();

        m_userWidget = quickWidget;
    //    QQmlApplicationEngine engine;
        //    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    set_UIWidgetPtr(m_userWidget);
    set_CallbackPtr(nullptr);
    set_MessageObj(nullptr);
}

///
/// \brief QGenericPlugin中的虚函数实现
/// \param name
/// \param spec
/// \return
///
QObject* Robot_Plugins::create(const QString &name, const QString &spec){
    Q_UNUSED(name);
    Q_UNUSED(spec);

    return 0;
}

void Robot_Plugins::Init_Cores()
{

}

void Robot_Plugins::Destroy_Cores()
{

}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Robot_Plugins, Robot_Plugins)
#endif // QT_VERSION < 0x050000
