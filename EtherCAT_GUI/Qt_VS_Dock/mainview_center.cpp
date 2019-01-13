#include "mainformview.h"

//用于解析G代码
#define PROGRESSMINLINES 10000 //G代码文件的最大行数
#define PROGRESSSTEP     1000



///
/// \brief 初始化中心内容框
///
void MainFormView::Init_FrameCenter_Content()
{
//    this->user_formComm = new FormComm();//定义父类的时候会调用show
//    ui->frame_center_main->layout()->addWidget(user_formComm);

    mTabWedget_center = new QTabWidget();
    QGridLayout *user_layout = new QGridLayout();
    mtabWeidgetItem_General = new QWidget();
    user_layout = new QGridLayout();
    mtabWeidgetItem_General->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_General,tr("General"));
//    QVBoxLayout *h_layout = new QVBoxLayout();
//    QFrame *xx1 = new QFrame();
//    QTableWidget *xx2 = new QTableWidget();
//    xx2->setColumnCount(2);
//    xx2->setHorizontalHeaderLabels(QStringList()<<tr("Index")<<tr("Content"));
//    xx2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽
//    h_layout->addWidget(xx1,1);
//    h_layout->addWidget(xx2,1);
//    mtabWeidgetItem_General->setLayout(h_layout);

    general_xx = new GeneralTab_P();
    mtabWeidgetItem_General->layout()->addWidget(general_xx->get_UIWidgetPtr());
    general_xx->Init_Cores();


//    mtabWeidgetItem_Messure = new QWidget();
//    user_layout = new QGridLayout();
//    mtabWeidgetItem_Messure->setLayout(user_layout);
//    mTabWedget_center->addTab(mtabWeidgetItem_Messure,tr("Messure"));

//    mtabWeidgetItem_Control = new QWidget();
//    user_layout = new QGridLayout();
//    mtabWeidgetItem_Control->setLayout(user_layout);
//    mTabWedget_center->addTab(mtabWeidgetItem_Control,tr("Control"));

//    control_xx = new ControlTab_P();
//    mtabWeidgetItem_Control->layout()->addWidget(control_xx->get_UIWidgetPtr());
//    control_xx->Init_Cores();
//    connect(control_xx,SIGNAL(MasterStop_Signal()),this,SLOT(Control_MasterStop_Signal()));
//    connect(control_xx,SIGNAL(StatusMessage_change(QString,int)),this,SLOT(Control_StatusMessage_change(QString,int)));
//    connect(control_xx,SIGNAL(BottomMessage_change(QString)),this,SLOT(Control_BottomMessage_change(QString)));

    mtabWeidgetItem_UserApps = new QWidget();
    user_layout = new QGridLayout();
    mtabWeidgetItem_UserApps->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_UserApps,tr("User Application"));

    get_FramePtr(Frame_center_f)->layout()->addWidget(mTabWedget_center);

//    QTableView *xx = new QTableView();
//    xx_model = new GCodeTableModel();
////    xx_model->setHorizontalHeaderLabels(QStringList()<<tr("项目"));
//    xx->setModel(xx_model);
//    mtabWeidgetItem_UserApps->layout()->addWidget(xx);
//    GCodeItem xx_item;
//    xx_item.command = "sss";
//    xx_item.state =  GCodeItem::InQueue;
//    xx_item.response = "OK";
//    xx_item.line = 1;
//    xx_model->data().append(xx_item);
//    GCodeItem xx_item1;
//    xx_item1.command = "sss";
//    xx_item1.state =  GCodeItem::InQueue;
//    xx_item1.response = "OK";
//    xx_item1.line = 2;
//    xx_model->data().append(xx_item1);
//     xx_item1.line = 3;
//     xx_model->data().append(xx_item1);
//     xx->selectRow(1);
//     xx_model->insertRow(xx_model->rowCount());


    //添加用户自定义的Application
    mTabWedget_center->hide();

    //添加slave信息显示的widget
    m_widget_slaveMSG = new QWidget();
    QVBoxLayout *v_layout = new QVBoxLayout();
    m_tableView_slaveMSG = new My_Table_SlaveMSG();
    m_tableView_slaveItemMSG = new My_Table_SlaveItemMSG();
    v_layout->addWidget(m_tableView_slaveMSG,1);
    v_layout->addWidget(m_tableView_slaveItemMSG,3);
    m_widget_slaveMSG->setLayout(v_layout);

    get_FramePtr(Frame_center_f)->layout()->addWidget(m_widget_slaveMSG);
    m_widget_slaveMSG->hide();//暂时不显示

    m_tableView_slaveMSG->init();
    m_tableView_slaveItemMSG->init();
}

void MainFormView::Control_StatusMessage_change(QString message,int interval){
   Set_StatusMessage(message,interval);
}
void MainFormView::Control_BottomMessage_change(QString message){
    m_bottomText->appendPlainText(message);
}
void MainFormView::Control_MasterStop_Signal(){
    Master_stop();
}

/************  Slots end ***************/
