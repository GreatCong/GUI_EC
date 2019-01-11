#include "GeneralTab_P.h"
#include <QSettings>
#include <Qfile>

GeneralTab_P::GeneralTab_P(QObject *parent) : QObject(parent)
{
   user_form_generalTab = new Form_GeneralTab();

   set_UIWidgetPtr(user_form_generalTab);
   set_CallbackPtr(nullptr);
}

void GeneralTab_P::Init_Cores()
{
  m_settingPath = "./config.ini";
  Load_setting(m_settingPath);
}

void GeneralTab_P::Destroy_Cores()
{
  Save_setting(m_settingPath);
}

My_EthercatMaster *GeneralTab_P::get_MasterPtr()
{
    return user_form_generalTab->master;
}

void GeneralTab_P::Master_UI_RUn()
{
    user_form_generalTab->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_OPERATIONAL));
    user_form_generalTab->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_OPERATIONAL));
}

void GeneralTab_P::Master_UI_Stop()
{
    user_form_generalTab->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_INIT));
    user_form_generalTab->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_INIT));

}

void GeneralTab_P::Master_UI_Loop(int state)
{
    user_form_generalTab->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(My_EthercatMaster::Master_stateToString(state));
}

void GeneralTab_P::Master_UI_Scan()
{
    user_form_generalTab->get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_SAFE_OP));
    user_form_generalTab->get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_SAFE_OP));
}

int GeneralTab_P::Load_setting(const QString &path)
{
    //    QFile file("./config.ini");
        QFile file(path);
        if(file.exists()){
            QSettings setting(path,QSettings::IniFormat);//读配置文件

            QString master_adapterName = setting.value("EtherCAT/Adapter_Name").toString();
            QString master_adapterDesc = setting.value("EtherCAT/Adapter_Desc").toString();
            user_form_generalTab->setMaster_adapterName(master_adapterName);
            user_form_generalTab->setMaster_adapterDesc(master_adapterDesc);
            //bind to master
            user_form_generalTab->master->m_adapterDescSelect = master_adapterDesc;
            user_form_generalTab->master->m_adapterNameSelect = master_adapterName;
    //        qDebug() << m_GcodePath<<m_pluginDir;
        }

   return 0;
}

int GeneralTab_P::Save_setting(const QString &path)
{
    QSettings setting(path,QSettings::IniFormat);//读配置文件

    setting.beginGroup(tr("EtherCAT"));
    setting.setValue("Adapter_Name",user_form_generalTab->getMaster_adapterName());//设置key和value，也就是参数和值
    setting.setValue("Adapter_Desc",user_form_generalTab->getMaster_adapterDesc());
 //   setting.setValue("remeber",true);
    setting.endGroup();//节点结束

  return 0;
}
