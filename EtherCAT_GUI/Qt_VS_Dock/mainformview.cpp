/*
* Main_View
*/
#include "mainformview.h"
#include "ui_mainformview.h"

#include <QPainter>

#include "thread_test.h"

MainFormView::MainFormView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFormView)
{
    ui->setupUi(this);

    this->Init_Cores();
    this->Init_TestCores();//测试的对象初始化
    this->EthercatApp_init();   


//    thread_test *xx = new thread_test();
//    thread = new QThread();
//    xx->moveToThread(thread);
//    connect(thread,SIGNAL(started()),xx,SLOT(debug()));

//    user_formComm = new FormComm(ui->frame_center_main);
//    ui->frame_center_main->layout()->addWidget(user_formComm);
//    user_form_scripts = new FormScripts(ui->frame_center_main);
//    ui->frame_center_main->layout()->addWidget(user_form_scripts);

}

MainFormView::~MainFormView()
{
   this->EthercatApp_destroy();
   user_form_generalTab->deleteLater();
   control_xx->Destroy_Cores();
    delete ui;
}

///
/// \brief MainFormView::Init_Cores
///
void MainFormView::Init_Cores()
{

    QGridLayout *userGridLayout= new QGridLayout();
    ui->frame_left_document->setLayout(userGridLayout);

    userGridLayout= new QGridLayout();
    ui->frame_right_property->setLayout(userGridLayout);

    userGridLayout= new QGridLayout();
    ui->frame_center_main->setLayout(userGridLayout);

    userGridLayout= new QGridLayout();
    ui->frame_bottom_message->setLayout(userGridLayout);

    Init_FrameLeft_Content();
    Init_FrameRight_Content();
    Init_FrameCenter_Content();
    Init_FrameBottom_Content();

    m_scanWait_processBar = new QProgressBar(this);
    m_scanWait_processBar->setMaximum(100);
    m_scanWait_processBar->setMinimum(0);
    m_scanWait_processBar->setValue(0);
    m_scanWait_processBar->hide();//开始不显示

    m_status_label = new QLabel("Ready",this);

   // controlTab_isTheta_display = false;
//    m_status_label->setFrameStyle(QFrame::NoFrame);//无边框

//    Set_StatusWidget(m_scanWait_processBar,true);//会出错

   // m_pluginDir = qApp->applicationDirPath() + tr("/../../../plugins");
//    qDebug() << m_pluginDir;

}

int MainFormView::Load_setting(const QString &path){

//    QFile file("./config.ini");
    QFile file(path);
    if(file.exists()){
        QSettings setting(path,QSettings::IniFormat);//读配置文件

        QString setting_pluginDir =  setting.value("Path/PluginPath").toString();
        QDir dir;
        dir= QDir(setting_pluginDir);
        if(dir.exists()){
            m_pluginDir = setting_pluginDir;
        }
        else{
            QMessageBox::warning(this,tr("Path Error!"),"PluginPath is Invalid,loading default path..");
        }

        QString master_adapterName = setting.value("EtherCAT/Adapter_Name").toString();
        QString master_adapterDesc = setting.value("EtherCAT/Adapter_Desc").toString();
        user_form_generalTab->setMaster_adapterName(master_adapterName);
        user_form_generalTab->setMaster_adapterDesc(master_adapterDesc);
        //bind to master
        user_form_generalTab->master->m_adapterDescSelect = master_adapterDesc;
        user_form_generalTab->master->m_adapterNameSelect = master_adapterName;
//        qDebug() << m_GcodePath<<m_pluginDir;
    }
    else{
//        m_GcodePath = "./";
        m_pluginDir = "./";
//        qDebug() << "Load default setting!";
        Set_StatusMessage(tr("Load default setting!"),3000);
    }



    return 0;

}

int MainFormView::Save_setting(const QString &path){

   QSettings setting(path,QSettings::IniFormat);//读配置文件
   setting.beginGroup(tr("Login"));
   setting.setValue("account","DRE");//设置key和value，也就是参数和值
   setting.setValue("password","123456");
//   setting.setValue("remeber",true);
   setting.endGroup();//节点结束

   setting.beginGroup(tr("Path"));
//   setting.setValue("GcodePath",m_GcodePath);//设置key和value，也就是参数和值
   setting.setValue("PluginPath",m_pluginDir);
//   setting.setValue("remeber",true);
   setting.endGroup();//节点结束

   setting.beginGroup(tr("EtherCAT"));
   setting.setValue("Adapter_Name",user_form_generalTab->getMaster_adapterName());//设置key和value，也就是参数和值
   setting.setValue("Adapter_Desc",user_form_generalTab->getMaster_adapterDesc());
//   setting.setValue("remeber",true);
   setting.endGroup();//节点结束

    return 0;
}


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

/************  Slots *******************/

void MainFormView::mm_timerOut(){
    if(timer_num++ < 100){
        m_scanWait_processBar->setValue(timer_num);
    }
    else{
      ProcessBar_stopEvent();
   }

}

void MainFormView::Plugins_OpenEditor_clicked(){
//   mm_time->start(300);
//   timer_num = 0;
//    if(thread->isRunning())
//    {
//        thread->quit();
//    }
//     qDebug()<<"from main thread slot:" <<QThread::currentThreadId();
//    thread->start();
}

/************  Slots End *******************/



///
/// \brief 加载所有插件（测试）
/// \return
///
int MainFormView::LoadPlugins()
{
    int plugin_num = 0;

    QDir pluginsDir(qApp->applicationDirPath());

    pluginsDir.cd("../../../plugins");

       // 查找目录中的所有插件
       foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
           QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
           // 返回插件的根组件对象
           QObject *pPlugin = loader.instance();
           if (pPlugin != Q_NULLPTR) {

               // 获取元数据（名称、版本、依赖）
               QJsonObject json = loader.metaData().value("MetaData").toObject();
               qDebug() << "********** MetaData **********";
               qDebug() << json.value("author").toVariant();
               qDebug() << json.value("date").toVariant();
               qDebug() << json.value("name").toVariant();
               qDebug() << json.value("version").toVariant();
               qDebug() << json.value("dependencies").toArray().toVariantList();

               // 访问感兴趣的接口
               My_UserAppWidget *userApps = qobject_cast<My_UserAppWidget *>(pPlugin);
               if (userApps != Q_NULLPTR) {
                   userApps->Init_Apps();
                   mtabWeidgetItem_UserApps->layout()->addWidget(userApps->getWidget());
               } else {
                   qWarning() << "qobject_cast falied";
               }
           }
       }

       return plugin_num;
}



///
/// \brief 加载指定的插件
/// \param fileName
/// \return
///
bool MainFormView::LoadPlugins(const QString &fileName)
{
    QDir pluginsDir(m_pluginDir);

    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    // 返回插件的根组件对象
    QObject *pPlugin = loader.instance();
    if (pPlugin != Q_NULLPTR) {
       // 访问感兴趣的接口
       My_UserAppWidget *userApps = qobject_cast<My_UserAppWidget *>(pPlugin);
       if (userApps != Q_NULLPTR) {
           userApps->Init_Apps();
           mtabWeidgetItem_UserApps->layout()->addWidget(userApps->getWidget());
       } else {
           //qWarning() << "qobject_cast falied";
           m_bottomText->appendPlainText(tr("qobject_cast falied"));
           return false;
       }
    }

    return true;
}

///
/// \brief 扫描插件
/// \param plugin_dir
/// \return
///
int MainFormView::ScanPlugins(const QString &plugin_dir)
{
    int plugin_num = 0;

    QDir pluginsDir(plugin_dir);

       // 查找目录中的所有插件
       foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
           QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
           // 返回插件的根组件对象
           QObject *pPlugin = loader.instance();
           if (pPlugin != Q_NULLPTR) {
               plugin_num ++;

//               // 获取元数据（名称、版本、依赖）
//               QJsonObject json = loader.metaData().value("MetaData").toObject();
//               qDebug() << "********** MetaData **********";
//               qDebug() << json.value("author").toVariant();
//               qDebug() << json.value("date").toVariant();
//               qDebug() << json.value("name").toVariant();
//               qDebug() << json.value("version").toVariant();
//               qDebug() << json.value("dependencies").toArray().toVariantList();

//               QListWidgetItem *item = new QListWidgetItem(fileName);
               My_LisetViewItem_Plugin *item = new My_LisetViewItem_Plugin(fileName);
               item->plugin_jsonMSG = loader.metaData().value("MetaData").toObject();
               m_pluginList->addItem(item);
           }
       }

       return plugin_num;
}

void MainFormView::ProcessBar_stopEvent(){
    mm_time->stop();
    timer_num = 0;
     m_scanWait_processBar->setValue(timer_num);

     m_scanWait_processBar->hide();
     m_status_label->setText(tr("Ready"));
}

void MainFormView::ProcessBar_startEvent(){
    mm_time->start(20);//1000ms
    timer_num = 0;
    m_scanWait_processBar->show();
  //  Set_StatusMessage(tr("Waiting for Scanning!"),3000);
    m_status_label->setText(tr("Scanning..."));
}


