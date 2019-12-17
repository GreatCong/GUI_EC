#include "Component_EtherCAT.h"
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

///
/// \brief 构造函数
/// \param parent
///
Component_EtherCAT::Component_EtherCAT(QObject *parent) : QObject(parent),Component_Private()
{
    this->Init_Object();
}

///
/// \brief 析构函数
///
Component_EtherCAT::~Component_EtherCAT()
{

}

///
/// \brief 初始化组件内容
/// \return
///
bool Component_EtherCAT::Init_Cores()
{
  this->Init_Slots();

  m_settingPath = "./config.ini";
  //Load Master
  this->Master_AttachMaster("./config.ini");//master的dll与插件在一个文件夹下

  if(this->ComponentP_get_MasterPtr()!= nullptr){//如果主站不为空
      Load_setting(m_settingPath);
  }

  return true;
}

///
/// \brief 销毁组件内容
/// \return
///
bool Component_EtherCAT::Destroy_Cores()
{
  if(this->ComponentP_get_MasterPtr()!= nullptr){//如果主站不为空
    Save_setting(m_settingPath);
    this->Master_DettachMaster();
    delete m_master_Loader;
  }

  return true;
}

///
/// \brief 初始化组件内的对象
/// \return
///
bool Component_EtherCAT::Init_Object()
{
    m_form_EtherCAT = new Form_GeneralTab();
    this->set_componentWidgetPtr(m_form_EtherCAT);//设置组件的UI

    return true;
}

///
/// \brief 初始化组件内的信号槽
/// \return
///
bool Component_EtherCAT::Init_Slots()
{
    connect(this->m_form_EtherCAT,SIGNAL(sig_pushButton_clicked(int)),this,SLOT(WidgetSig_pushButton_clicked(int)));
    connect(this->m_form_EtherCAT,SIGNAL(sig_ComboBox_changed(int,int)),this,SLOT(WidgetSig_ComboBox_changed(int,int)));
    connect(this->m_form_EtherCAT->get_ListWidgetPtr(Form_GeneralTab::Master_Aadapter_L),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(WidgetSig_AdapterList_itemDoubleClicked(QListWidgetItem*)));


    return true;
}

///
/// \brief 设置Master UI的运行状态(Run)
///
void Component_EtherCAT::Master_UI_RUn()
{
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(DRE_Master::Master_stateToString(DRE_Master::STATE_OPERATIONAL));
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(DRE_Master::Master_stateToString(DRE_Master::STATE_OPERATIONAL));
}

///
/// \brief 设置Master UI的运行状态(Run)
///
void Component_EtherCAT::Master_UI_Stop()
{
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(DRE_Master::Master_stateToString(DRE_Master::STATE_INIT));
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(DRE_Master::Master_stateToString(DRE_Master::STATE_INIT));

}

///
/// \brief 设置Master UI的运行状态(Stop)
///
void Component_EtherCAT::Master_UI_Loop(int state)
{
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(DRE_Master::Master_stateToString(state));
}

///
/// \brief 设置Master UI的运行状态(Scan)
///
void Component_EtherCAT::Master_UI_Scan()
{
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(DRE_Master::Master_stateToString(DRE_Master::STATE_SAFE_OP));
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(DRE_Master::Master_stateToString(DRE_Master::STATE_SAFE_OP));
}

///
/// \brief 加载Master的动态库
///
bool Component_EtherCAT::Master_AttachMaster(QString path)
{
    bool ret = true;
    QString pluginDir;//master的dll与插件在一个文件夹下

    QFile file(path);
    if(file.exists()){
        QSettings setting(path,QSettings::IniFormat);//读配置文件

        QString setting_pluginDir =  setting.value("Path/PluginPath").toString();
        QDir dir;
        dir= QDir(setting_pluginDir);
        if(dir.exists()){
            pluginDir = setting_pluginDir;
        }
    }
    else{
        pluginDir = "./";
    }

    m_master_Loader = new DRE_Master_Loader();//加载master.dll
    m_master_Loader->set_pluginDir(pluginDir);

     DRE_Master *master = nullptr;
     QString mastePlugin_fileName;
     QString masterPlugin_suffixName;
#ifdef Q_OS_WIN
    mastePlugin_fileName = "DRE_Master";
    masterPlugin_suffixName = ".dll";
    master = m_master_Loader->Master_load(mastePlugin_fileName+masterPlugin_suffixName);//"DRE_Master.dll"
    if(master == nullptr){
        master = m_master_Loader->Master_load(mastePlugin_fileName+"_d"+masterPlugin_suffixName);//方便调试库的加载 "DRE_Master_d.dll"
    }
#else//Q_OS_UNIX
    mastePlugin_fileName = "libDRE_Master";
    masterPlugin_suffixName = ".so";
    master = m_master_Loader->Master_load(mastePlugin_fileName+masterPlugin_suffixName);
    if(master == nullptr){
        master = m_master_Loader->Master_load(mastePlugin_fileName+"_debug"+masterPlugin_suffixName);//方便调试库的加载
    }
#endif

    if(master == nullptr){
        QMessageBox::critical(this->get_componentWidgetPtr(),tr("Information"),mastePlugin_fileName +
                                                                                  masterPlugin_suffixName +
                                                                                  " is not Found or Invalid!");
        this->m_master_Loader->Master_unload();
        delete m_master_Loader;
        ret = false;
    }

    this->ComponentP_set_MasterPtr(master);//传递主站地址

    return ret;
}

///
/// \brief 卸载Master
/// \return
///
bool Component_EtherCAT::Master_DettachMaster()
{
    return m_master_Loader->Master_unload();
}

///
/// \brief 加载配置文件
/// \param path
/// \return
///
int Component_EtherCAT::Load_setting(const QString &path)
{
        QFile file(path);
        if(file.exists()){
            QSettings setting(path,QSettings::IniFormat);//读配置文件

            QString master_adapterName = setting.value("EtherCAT/Adapter_Name").toString();
            QString master_adapterDesc = setting.value("EtherCAT/Adapter_Desc").toString();
            int plc_period = setting.value("EtherCAT/PLC_Period").toInt();
//            qDebug() << plc_period;
            if(plc_period < 1 || plc_period >10){
                QMessageBox::warning(this->get_componentWidgetPtr(),tr("Invalid PLC_period"),tr("Load default PLC_period!"));
                plc_period = 10;
            }

            this->ComponentP_get_MasterPtr()->set_PLC_Period(plc_period);
            m_form_EtherCAT->get_ComboBoxPtr(Form_GeneralTab::Master_periodPLC_c)->setCurrentIndex(plc_period-1);

            if(!master_adapterName.isEmpty()){//存在保存的适配器
                if(this->ComponentP_get_MasterPtr()->Find_adapter()){//判断存储的适配器是否有效
                    m_form_EtherCAT->Master_AdapterFind_Handle(this->ComponentP_get_MasterPtr()->get_AdapterDescription());//将list添加到UI

                    QStringList adapter_list = this->ComponentP_get_MasterPtr()->get_AdapterName();
                    if(adapter_list.contains(master_adapterName)){
                        m_form_EtherCAT->setMaster_adapterName(master_adapterName);
                        m_form_EtherCAT->setMaster_adapterDesc(master_adapterDesc);
                        //bind to master
                        this->ComponentP_get_MasterPtr()->set_CurrentAdapter(master_adapterName,master_adapterDesc);
                    }
                    else{
                        QMessageBox::warning(this->get_componentWidgetPtr(),tr("Invalid Adapter"),tr("Please Search again!"));
                    }
                }
                else{
                        QMessageBox::warning(this->get_componentWidgetPtr(),tr("Adapter is not found"),tr("Please Check it and Search again!"));
                }
            }


    //        qDebug() << m_GcodePath<<m_pluginDir;
        }

   return 0;
}

///
/// \brief 保存配置文件
/// \param path
/// \return
///
int Component_EtherCAT::Save_setting(const QString &path)
{
    QSettings setting(path,QSettings::IniFormat);//读配置文件

    setting.beginGroup(tr("EtherCAT"));
    setting.setValue("Adapter_Name",m_form_EtherCAT->getMaster_adapterName());//设置key和value，也就是参数和值
    setting.setValue("Adapter_Desc",m_form_EtherCAT->getMaster_adapterDesc());
    setting.setValue("PLC_Period",this->ComponentP_get_MasterPtr()->get_PLC_Period());
 //   setting.setValue("remeber",true);
    setting.endGroup();//节点结束

    return 0;
}

/******************** Slot begin ************************/

///
/// \brief UI界面中的pushButton响应
/// \param buttonID
///
void Component_EtherCAT::WidgetSig_pushButton_clicked(int buttonID)
{
    if(this->ComponentP_get_MasterPtr() == nullptr){
        QMessageBox::warning(this->get_componentWidgetPtr(),"Warning","No Master here,try to restart it!");
        return;
    }
    switch(buttonID){
        case Form_GeneralTab::Master_findAdapter_b:
        this->ComponentP_get_MasterPtr()->Find_adapter();
        m_form_EtherCAT->Master_AdapterFind_Handle(this->ComponentP_get_MasterPtr()->get_AdapterDescription());
        break;
    case Form_GeneralTab::Master_stateInit_b:
        if(this->ComponentP_get_MasterPtr()->Master_getSlaveCount()>0){
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(DRE_Master::STATE_INIT));
            int ret = this->ComponentP_get_MasterPtr()->Master_ChangeState(0,DRE_Master::STATE_INIT);
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(ret));
         }
        break;
        case Form_GeneralTab::Master_statePreOp_b:
        if(this->ComponentP_get_MasterPtr()->Master_getSlaveCount()>0){
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(DRE_Master::STATE_PRE_OP));
            int ret = this->ComponentP_get_MasterPtr()->Master_ChangeState(0,DRE_Master::STATE_PRE_OP);
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(ret));
         }
        break;
        case Form_GeneralTab::Master_stateSafeOp_b:
        if(this->ComponentP_get_MasterPtr()->Master_getSlaveCount()>0){
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(DRE_Master::STATE_SAFE_OP));
            int ret = this->ComponentP_get_MasterPtr()->Master_ChangeState(0,DRE_Master::STATE_SAFE_OP);
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(ret));
         }
        break;
        case Form_GeneralTab::Master_stateOp_b:
        if(this->ComponentP_get_MasterPtr()->Master_getSlaveCount()>0){
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(DRE_Master::STATE_OPERATIONAL));
            int ret = this->ComponentP_get_MasterPtr()->Master_ChangeState(0,DRE_Master::STATE_OPERATIONAL);
            m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(
                                             this->ComponentP_get_MasterPtr()->Master_stateToString(ret));
         }
        break;
    default:
        break;
    }
}

///
/// \brief 下拉框的槽响应
/// \param comboBox_id
/// \param index
///
void Component_EtherCAT::WidgetSig_ComboBox_changed(int comboBox_id, int index)
{
    switch(comboBox_id){
    case Form_GeneralTab::Master_periodPLC_c:
        if(this->ComponentP_get_MasterPtr()){
            this->ComponentP_get_MasterPtr()->set_PLC_Period(index+1);//设置EtherCAT周期 ms
        }
        break;
    default:
        break;
    }
}

///
/// \brief 适配器选择的槽响应
/// \param item
///
void Component_EtherCAT::WidgetSig_AdapterList_itemDoubleClicked(QListWidgetItem *item)
{
    if(this->ComponentP_get_MasterPtr() == nullptr){
        QMessageBox::warning(this->get_componentWidgetPtr(),"Warning","No Master here,try to restart it!");
        return;
    }

    My_ListViewItem *my_item = dynamic_cast<My_ListViewItem *>(item);
//    qDebug() << my_item->current_index;
    QString name = this->ComponentP_get_MasterPtr()->get_AdapterName().at(my_item->current_index);
    QString desc = item->text();
    this->ComponentP_get_MasterPtr()->set_CurrentAdapter(name,desc);

    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_adapterDesc_e)->setText(desc);
    m_form_EtherCAT->get_LineEditPtr(Form_GeneralTab::Master_adapterName_e)->setText(name);
}

/******************** Slot end  ************************/
