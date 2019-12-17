/*
* Main_View
*/
#include "mainformview.h"
#include "ui_mainformview.h"

#include <QPainter>
#include <QMessageBox>

///
/// \brief 构造函数
/// \param parent
///
MainFormView::MainFormView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFormView)
{
    ui->setupUi(this);

    this->Init_Cores();
    this->Init_TestCores();//测试的对象初始化
}

///
/// \brief 析构函数
///
MainFormView::~MainFormView()
{
    if(m_master){
       this->EthercatApp_destroy();
    }
    m_component_EtherCAT->Destroy_Cores();

   delete ui;
}


///
/// \brief 获取frame指针
/// \param choose
/// \return
///
QFrame *MainFormView::get_FramePtr(Frame_choose choose){
    switch(choose){
    case Frame_left_f:
        return ui->frame_left_document;
        break;
    case Frame_right_f:
        return ui->frame_right_property;
        break;
    case Frame_center_f:
        return ui->frame_center_main;
        break;
    case Frame_bottom_f:
        return ui->frame_bottom_message;
        break;
    default:
        QMessageBox::critical(this,tr("MainFormView Frame"),tr("Return NULL!"));
        return NULL;
        break;
    }
}

///
/// \brief 获取进度条指针
/// \param choose
/// \return
///
QProgressBar *MainFormView::get_ProcessBarPtr(ProcessBar_choose choose)
{
    switch(choose){
    case ProcessBar_ScanWait_p:
        return m_scanWait_processBar;
        break;
    default:
        QMessageBox::critical(this,tr("MainFormView ProcessBar"),tr("Return NULL!"));
        return NULL;
        break;
    }
}

///
/// \brief 获取label的指针
/// \param choose
/// \return
///
QLabel *MainFormView::get_LabelPtr(Label_choose choose)
{
    switch(choose){
    case Label_status_l:
        return m_status_label;
        break;
    default:
        QMessageBox::critical(this,tr("MainFormView Label"),tr("Return NULL!"));
        return NULL;
        break;
    }
}

///
/// \brief 用于测试的对象初始化
///
void MainFormView::Init_TestCores(){

}

///
/// \brief 初始化
///
void MainFormView::Init_Cores()
{
    QGridLayout *userGridLayout= new QGridLayout();
    this->get_FramePtr(Frame_left_f)->setLayout(userGridLayout);
    this->get_FramePtr(Frame_left_f)->layout()->setMargin(0);

    userGridLayout= new QGridLayout();
    this->get_FramePtr(Frame_right_f)->setLayout(userGridLayout);
    this->get_FramePtr(Frame_right_f)->layout()->setMargin(0);

    userGridLayout= new QGridLayout();
    this->get_FramePtr(Frame_center_f)->setLayout(userGridLayout);
    this->get_FramePtr(Frame_center_f)->layout()->setMargin(0);

    userGridLayout= new QGridLayout();
    this->get_FramePtr(Frame_bottom_f)->setLayout(userGridLayout);
    this->get_FramePtr(Frame_bottom_f)->layout()->setMargin(0);

    this->m_ComponentList = new QList<Component_Private *>;//存储每个组件的this指针（注意都是Component_Private指针）

    this->Init_FrameBottom_Content();//先初始化bottom
    this->Init_FrameLeft_Content();
    this->Init_FrameRight_Content();
    this->Init_FrameCenter_Content();

    //初始化进度条
    m_scanWait_processBar = new QProgressBar(this);
    m_scanWait_processBar->setMaximum(100);//设置进度条最大为100
    m_scanWait_processBar->setMinimum(0);
    m_scanWait_processBar->setValue(0);
    m_scanWait_processBar->hide();//开始不显示

    //初始化底部状态栏的label
    m_status_label = new QLabel("Ready",this);

    //指向顶部的工具栏的指针
    m_arrayActionMenuBar = new QVector<QAction*>();

    //用于进度条显示的定时器
    mm_time = new QTimer();
    connect(mm_time,SIGNAL(timeout()),this,SLOT(mm_timerOut()));

}

///
/// \brief 析构前调用
///
void MainFormView::Destroy_Cores()
{
    if(m_master){
       this->EthercatApp_destroy();
    }

    //对组件循环遍历
    foreach (Component_Private* component, *(this->m_ComponentList)) {
        component->Destroy_Cores();
    }
}

///
/// \brief 初始化组件
/// \return
///
bool MainFormView::Init_Components()
{
    this->m_MessageText = new QPlainTextEdit(this);
    this->m_ComponentWidgets = new ComponentWidgets();
    this->m_ComponentWidgets->setParent(this);

    this->m_ComponentWidgets->m_tabWedget_center = this->mTabWedget_center;
    this->m_ComponentWidgets->m_widget_slaveMSG = this->m_widget_slaveMSG;
    this->m_ComponentWidgets->m_tabWeidgetItem_General = this->mtabWeidgetItem_General;
    this->m_ComponentWidgets->m_tabWeidgetItem_UserApps = this->mtabWeidgetItem_UserApps;

    connect(this->m_ComponentWidgets,SIGNAL(sigMasterStop_Signal()),this,SLOT(Control_MasterStop_Signal()));
    connect(this->m_ComponentWidgets,SIGNAL(sigStatusMessage_change(QString,int)),this,SLOT(Control_StatusMessage_change(QString,int)));
    connect(this->m_ComponentWidgets,SIGNAL(sigBottomMessage_change(QString)),this,SLOT(Control_BottomMessage_change(QString)));
    connect(this->m_ComponentWidgets,SIGNAL(sigMaster_setState(int)),this,SLOT(ComponentWidgetsSigMaster_setState(int)));
    connect(this->m_ComponentWidgets,SIGNAL(sigAppPath_change(QString&)),this,SLOT(ComponentWidgetsSigAppPath_change(QString&)));

    this->EthercatApp_init();//Attach master

    //对组件循环遍历
    foreach (Component_Private* component, *(this->m_ComponentList)) {
        component->ComponentP_set_MessageTextPtr(this->m_MessageText);//设置信息输出框的指针
        component->ComponentP_set_MasterPtr(this->m_master);
        component->ComponentP_set_ComponentWidgetsPtr(this->m_ComponentWidgets);

        component->Init_Cores();
        this->System_BottomMessage(component->get_AppName() + " Init OK");
    }

    //输出版本信息
//    m_bottomText->clear();
    this->System_BottomMessage(tr(MASTER_VERSION_CODE)+tr(" by lcj"));
    this->System_BottomMessage(tr("Please load plugin first!"));

    return true;
}

/************  Slots *******************/

///
/// \brief 用于进度条显示的定时器回调
///
void MainFormView::mm_timerOut(){
    if(timer_num < 50){
        m_scanWait_processBar->setValue(timer_num);
        timer_num++;
    }
    else if(timer_num < 100){
        if(timer_num > 55){
            m_scanWait_processBar->setValue(timer_num);
            timer_num++;
        }

    }
    else{
      ProcessBar_stopEvent();
   }

}

/************  Slots End *******************/

///
/// \brief 进度条的停止事件
///
void MainFormView::ProcessBar_stopEvent(){
    mm_time->stop();
    timer_num = 0;
    m_scanWait_processBar->setValue(timer_num);
    m_scanWait_processBar->hide();
    m_status_label->setText(tr("Ready"));

}

///
/// \brief 进度条的开始事件
///
void MainFormView::ProcessBar_startEvent(){
    mm_time->start(10);//1000ms

    timer_num = 0;
    m_scanWait_processBar->show();
  //  Set_StatusMessage(tr("Waiting for Scanning!"),3000);
    m_status_label->setText(tr("Scanning..."));
}



