#include "Component_DockBottom.h"

#include <QPushButton>
#include <QGridLayout>

#include <QDebug>

///
/// \brief 构造函数
/// \param parent
///
Component_DockBottom::Component_DockBottom(QObject *parent):QObject(parent),Component_Private()
{
//    this->Init_Object();
     this->set_AppName("Component_DockBottom");
}

///
/// \brief 析构函数
///
Component_DockBottom::~Component_DockBottom()
{
    if(this->get_componentWidgetPtr()){
        delete this->get_componentWidgetPtr();
        this->set_componentWidgetPtr(nullptr);
    }
}

///
/// \brief 初始化
/// \return
///
bool Component_DockBottom::Init_Cores()
{
    QGridLayout *userGridLayout= new QGridLayout();
    this->get_componentWidgetPtr()->setLayout(userGridLayout);

    QPushButton *button_clear = new QPushButton(tr("Clear"),this->get_componentWidgetPtr());//定义父窗口，这样可以在父窗口析构时候析构
    this->get_componentWidgetPtr()->layout()->addWidget(button_clear);
    connect(button_clear,SIGNAL(clicked(bool)),this,SLOT(BottonText_Clear_clicked()));

    this->get_componentWidgetPtr()->layout()->addWidget(this->ComponentP_get_MessageTextPtr());

    this->ComponentP_get_MessageTextPtr()->clear();

    this->Init_Slots();

    return true;
}

///
/// \brief 销毁应用
/// \return
///
bool Component_DockBottom::Destroy_Cores()
{
    return true;
}

///
/// \brief 初始化内部对象
/// \return
///
bool Component_DockBottom::Init_Object()
{
    m_DockWidget = new QWidget();
    this->set_componentWidgetPtr(m_DockWidget);

    if(this->ComponentP_get_ComponentListPtr()){
         this->ComponentP_get_ComponentListPtr()->append(this);//将自己注册到List中
    }
    else{
        qDebug() << "Component_DockBottom:ComponentP_get_ComponentListPtr=nullptr";
    }

    return true;
}

///
/// \brief 初始化信号槽
/// \return
///
bool Component_DockBottom::Init_Slots()
{
   return true;
}

///
/// \brief 底部消息打印
/// \param message
///
void Component_DockBottom::set_bottomText_append(const QString &message)
{
    this->ComponentP_get_MessageTextPtr()->appendPlainText(message);
}

/************************ slots begin **********************/

///
/// \brief 清除输出信息 点击
///
void Component_DockBottom::BottonText_Clear_clicked()
{
   this->ComponentP_get_MessageTextPtr()->clear();
}

/************************ slots end **********************/
