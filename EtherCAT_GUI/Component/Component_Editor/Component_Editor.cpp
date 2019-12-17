#include "Component_Editor.h"

#include <QDebug>
#include <QDateTime>
#include <QPixmap>
#include <QIcon>

///
/// \brief 构造函数
/// \param parent
///
Component_Editor::Component_Editor(QObject *parent):QObject(parent)
{
    //NOTE:如果要设置图标，在这个位置设置
    this->set_AppName("Code Editor");
    QPixmap pixmap(":/Icon/resource/Icons/VS.png");

    QIcon vs_icon;
    vs_icon.addPixmap(pixmap.copy(0, 0, 24, 24), QIcon::Normal, QIcon::Off);
    vs_icon.addPixmap(pixmap.copy(0, 24, 24, 24), QIcon::Normal, QIcon::On);
    vs_icon.addPixmap(pixmap.copy(24, 0, 24, 24), QIcon::Active, QIcon::On);
    vs_icon.addPixmap(pixmap.copy(24, 24, 24, 24), QIcon::Active, QIcon::Off);

    this->set_AppIcon(vs_icon);
}

///
/// \brief 析构函数
///
Component_Editor::~Component_Editor()
{

}

///
/// \brief 初始化应用
/// \return
///
bool Component_Editor::Component_Editor::Init_Cores()
{
//    qDebug() << "Init_Cores OK";
    m_codeEditor_form = new CodeEditor_form();//设置代码编辑器
    connect(m_codeEditor_form,SIGNAL(send_BottomMSG(QString)),this,SLOT(Plugins_codeEditor_MSG(QString)));

    //设置下图标
    QPixmap pixmap(":/Icon/resource/Icons/VS.png");

    QIcon vs_icon;
    vs_icon.addPixmap(pixmap.copy(0, 0, 24, 24), QIcon::Normal, QIcon::Off);
    vs_icon.addPixmap(pixmap.copy(0, 24, 24, 24), QIcon::Normal, QIcon::On);
    vs_icon.addPixmap(pixmap.copy(24, 0, 24, 24), QIcon::Active, QIcon::On);
    vs_icon.addPixmap(pixmap.copy(24, 24, 24, 24), QIcon::Active, QIcon::Off);

    m_codeEditor_form->setWindowIcon(vs_icon);

    this->set_componentWidgetPtr(m_codeEditor_form);
    return true;
}

///
/// \brief 销毁应用
/// \return
///
bool Component_Editor::Destroy_Cores()
{
    m_codeEditor_form->close();

    delete m_codeEditor_form;

    m_codeEditor_form = nullptr;
    m_componentWidget = nullptr;

    return true;
}

///
/// \brief 设置App路径
/// \param path
///
void Component_Editor::set_AppPath(const QString path){//重写下
    m_appPath = path;

    m_codeEditor_form->set_path(m_appPath);//让form也能够识别到
}

///
/// \brief 向系统的底部状态栏输出信息
/// \param message
///
void Component_Editor::System_BottomMessage(const QString &message)
{
    emit Sig_Send_MSG(message);
}

/****************** Slot begin ********************/

///
/// \brief 代码编辑器的底部信息栏
/// \param str
///
void Component_Editor::Plugins_codeEditor_MSG(const QString str){
    QString str_curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//获取当前的时间
    System_BottomMessage("Editor( "+str_curTime+" ): "+str);
}

/******************** Slot end *****************/

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(User_plugins, UserApp_plugin)
#endif // QT_VERSION < 0x050000
