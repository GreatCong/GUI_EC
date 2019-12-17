#include "mainformview.h"
#include <QDateTime>
#include <QDebug>

///
/// \brief 初始化左边的内容框
///
void MainFormView::Init_FrameLeft_Content()
{
    m_dockObj_left = new Component_DockLeft();
    m_dockObj_left->ComponentP_set_ComponentListPtr(this->m_ComponentList);
    m_dockObj_left->Init_Object();

    this->get_FramePtr(Frame_left_f)->layout()->addWidget(m_dockObj_left->get_componentWidgetPtr());
//    m_dockObj_left->Init_Cores();
}

///
/// \brief 初始化下面的内容框
///
void MainFormView::Init_FrameBottom_Content()
{
    m_dockObj_bottom = new Component_DockBottom();
    m_dockObj_bottom->ComponentP_set_ComponentListPtr(this->m_ComponentList);
    m_dockObj_bottom->Init_Object();

    this->get_FramePtr(Frame_bottom_f)->layout()->addWidget(m_dockObj_bottom->get_componentWidgetPtr());
//    m_dockObj_bottom->Init_Cores();
}

///
/// \brief 初始化右边的内容框
///
void MainFormView::Init_FrameRight_Content(){
    m_dockObj_right = new Component_DockRight();
    m_dockObj_right->ComponentP_set_ComponentListPtr(this->m_ComponentList);
    m_dockObj_right->Init_Object();

    this->get_FramePtr(Frame_right_f)->layout()->addWidget(m_dockObj_right->get_componentWidgetPtr());
//    m_dockObj_right->Init_Cores();
}

///
/// \brief 初始化中心内容框
///
void MainFormView::Init_FrameCenter_Content()
{
    mTabWedget_center = new QTabWidget(this);
    QGridLayout *user_layout = new QGridLayout();
//    mtabWeidgetItem_General = new QWidget(this);
    //NOTE:换成Scroll area,重新显示不会改变size
    mtabWeidgetItem_General = new QScrollArea(this);//换成Scroll area
    user_layout = new QGridLayout();
    mtabWeidgetItem_General->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_General,tr("General"));

    m_component_EtherCAT = new Component_EtherCAT(this);
    mtabWeidgetItem_General->layout()->addWidget(m_component_EtherCAT->get_componentWidgetPtr());
    m_component_EtherCAT->Init_Cores();

    mtabWeidgetItem_UserApps = new  QScrollArea();//换成Scroll area
    user_layout = new QGridLayout();
    mtabWeidgetItem_UserApps->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_UserApps,tr("User Application"));

    get_FramePtr(Frame_center_f)->layout()->addWidget(mTabWedget_center);

    //添加用户自定义的Application
    mTabWedget_center->hide();

    //添加slave信息显示的widget
    m_widget_slaveMSG = new QWidget(this);
    QVBoxLayout *v_layout = new QVBoxLayout();
    m_widget_slaveMSG->setLayout(v_layout);
    get_FramePtr(Frame_center_f)->layout()->addWidget(m_widget_slaveMSG);
    m_widget_slaveMSG->hide();//暂时不显示
}

/************* slot begin **************************/

/**** 下面是用于插件的回调的slot  ****/

void MainFormView::Control_StatusMessage_change(QString message,int interval){
   Set_StatusMessage(message,interval);
}

void MainFormView::Control_BottomMessage_change(QString message){
    QString str_curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//获取当前的时间
//    m_bottomText->appendPlainText("[ "+str_curTime+" ]");//便于像日志一样的显示
    this->m_MessageText->appendPlainText("[ "+str_curTime+" ]\t"+message);
}

void MainFormView::Control_MasterStop_Signal(){
    Master_stop();
}

void MainFormView::ComponentWidgetsSigMaster_setState(int state){
    switch(state){
        case ComponentWidgets::Cmd_Master_scan:
            this->Master_scan();
            break;
        case ComponentWidgets::Cmd_Master_run:
            this->Master_run();
            break;
        case ComponentWidgets::Cmd_Master_stop:
            this->Master_stop();
            break;
        default:
            qDebug() << "ComponentWidgetsSigMaster_setState error!";
            break;
        }
}

void MainFormView::ComponentWidgetsSigAppPath_change(QString &appPath){
    this->m_pluginDir = appPath;
}
/************  Slots end ***************/

