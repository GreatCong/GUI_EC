#include "form_generaltab.h"
#include "ui_form_GeneralTab.h"

#include <QMessageBox>

#include <QHeaderView>
#include <QCheckBox>

#include <QDebug>
#include <QSettings>
#include <QDir>

class My_LisetViewItem : public QListWidgetItem
{
//    Q_OBJECT
public:
    explicit My_LisetViewItem(QListWidget *view = nullptr);
    explicit My_LisetViewItem(const QString &text ,QListWidget *view = nullptr);
public:
   int current_index;
//signals:

//public slots:
};

My_LisetViewItem::My_LisetViewItem(QListWidget *view) : QListWidgetItem(view)
{

}

My_LisetViewItem::My_LisetViewItem(const QString &text, QListWidget *view): QListWidgetItem(text,view)
{

}

Form_GeneralTab::Form_GeneralTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_GeneralTab)
{
    ui->setupUi(this);

    Init_cores();

    ui->lineEdit_AdapterDesc->setPlaceholderText(tr("Description"));
    ui->lineEdit_AdapterName->setPlaceholderText(tr("Name"));
    ui->lineEdit_ActualState->setPlaceholderText(tr("Actual State"));
    ui->lineEdit_InquireState->setPlaceholderText(tr("Inquire State"));

//    get_LineEditPtr(Form_GeneralTab::Master_InquireState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_INIT));
//    get_LineEditPtr(Form_GeneralTab::Master_ActualState_e)->setText(My_EthercatMaster::Master_stateToString(My_EthercatMaster::STATE_INIT));

    //    master = new My_EthercatMaster();

    Load_master();

    ui->comboBox_PLCPeriod->setCurrentIndex(9);//设置初始的PLC周期为10ms

//    ui->pushButton_State_Op->setEnabled(false);//禁止Op按钮
}


Form_GeneralTab::~Form_GeneralTab()
{
//    delete master;
    m_master_Loader->Master_unload();
//    master->deleteLater();
    delete ui;
}


void Form_GeneralTab::Init_cores(){
    table = new QTableWidget(this);
    QGridLayout *layout = new QGridLayout();//通过UI创建的，自带一个layout
    ui->frame_tables->setLayout(layout);
    ui->frame_tables->layout()->addWidget(table);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(QStringList()<<tr("Master")<<tr("Slave")<<tr("Name")<<tr("Alias")<<tr("State"));
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽

    table->setRowCount(3);
    QTableWidgetItem *tableItem = new QTableWidgetItem(tr("1"));
    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    table->setItem(0,0,tableItem);

    tableItem = new QTableWidgetItem(tr("1"));
    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    table->setItem(0,1,tableItem);

    tableItem = new QTableWidgetItem(tr("Input1"));
    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    table->setItem(0,2,tableItem);

    tableItem = new QTableWidgetItem(tr("My_Input1"));
    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    table->setItem(0,3,tableItem);

    QCheckBox *checkBox =new QCheckBox(tr("isOutput"));
    checkBox->setChecked(true);
    table->setCellWidget(0,4,checkBox);

    tableItem = new QTableWidgetItem(tr("..."));
    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    table->setItem(1,0,tableItem);
}


void Form_GeneralTab::Load_master(){
    QString pluginDir;//master的dll与插件在一个文件夹下
    QString path = "./config.ini";

    QFile file(path);
    if(file.exists()){
        QSettings setting(path,QSettings::IniFormat);//读配置文件

        QString setting_pluginDir =  setting.value("Path/PluginPath").toString();
        QDir dir;
        dir= QDir(setting_pluginDir);
        if(dir.exists()){
            pluginDir = setting_pluginDir;
        }
//        else{
//            QMessageBox::warning(this,tr("Path Error!"),"PluginPath is Invalid,loading default path..");
//        }

//        QString master_adapterName = setting.value("EtherCAT/Adapter_Name").toString();
//        QString master_adapterDesc = setting.value("EtherCAT/Adapter_Desc").toString();
//        general_xx->get_UIWidgetPtr()->setMaster_adapterName(master_adapterName);
//        general_xx->get_UIWidgetPtr()->setMaster_adapterDesc(master_adapterDesc);
//        //bind to master
//        general_xx->get_UIWidgetPtr()->master->m_adapterDescSelect = master_adapterDesc;
//        general_xx->get_UIWidgetPtr()->master->m_adapterNameSelect = master_adapterName;
//        qDebug() << m_GcodePath<<m_pluginDir;
    }
    else{
//        m_GcodePath = "./";
        pluginDir = "./";
//        qDebug() << "Load default setting!";
//        Set_StatusMessage(tr("Load default setting!"),3000);
    }

    m_master_Loader = new  DRE_Master_Loader();//加载master.dll
//        m_master_Loader->set_pluginDir("./");
//     m_master_Loader->set_pluginDir("../../../plugins/");
     m_master_Loader->set_pluginDir(pluginDir);
    master = m_master_Loader->Master_load("DRE_Master.dll");
    if(master == nullptr){
        QMessageBox::critical(this,tr("Information"),tr("DRE_Master.dll is not Found or Invalid!"));
    }

}

const QString Form_GeneralTab::getMaster_adapterName(){
    return ui->lineEdit_AdapterName->text();
}

const QString Form_GeneralTab::getMaster_adapterDesc(){
    return ui->lineEdit_AdapterDesc->text();
}

void Form_GeneralTab::setMaster_adapterName(const QString &str ){
    ui->lineEdit_AdapterName->setText(str);
}

void Form_GeneralTab::setMaster_adapterDesc(const QString &str){
   ui->lineEdit_AdapterDesc->setText(str);
}

QPushButton *Form_GeneralTab::get_ButtonPtr(buttons_choose choose)
{
    switch(choose){
    case Master_stateInit_b:
        return ui->pushButton_State_Init;
        break;
    case Master_statePreOp_b:
        return ui->pushButton_State_PreOp;
        break;
    case Master_stateSafeOp_b:
        return ui->pushButton_State_SafeOp;
        break;
    case Master_stateOp_b:
        return ui->pushButton_State_Op;
        break;
    default:
        QMessageBox::critical(this,tr("Form_GeneralTab Button"),tr("Return NULL!"));
        return NULL;
        break;
    }
}

QLineEdit *Form_GeneralTab::get_LineEditPtr(lineEdits_choose choose)
{
    switch(choose){
    case Master_adapterDesc_e:
        return ui->lineEdit_AdapterDesc;
        break;
    case Master_adapterName_e:
        return ui->lineEdit_AdapterName;
        break;
    case Master_InquireState_e:
        return ui->lineEdit_InquireState;
        break;
    case Master_ActualState_e:
        return ui->lineEdit_ActualState;
        break;
    default:
        QMessageBox::critical(this,tr("Form_GeneralTab LineEdit"),tr("Return NULL!"));
        return NULL;
        break;
    }
}

QListWidget *Form_GeneralTab::get_ListWidgetPtr(ListWidget_choose choose)
{
    switch(choose){
    case Master_Aadapter_L:
        return ui->listWidget_AdapterList;
        break;
    default:
        QMessageBox::critical(this,tr("Form_GeneralTab ListWidget"),tr("Return NULL!"));
        return NULL;
        break;
    }
}

QComboBox *Form_GeneralTab::get_ComboBoxPtr(ComboBox_choose choose)
{
    switch(choose){
    case Master_periodPLC_c:
        return ui->comboBox_PLCPeriod;
        break;
    default:
        QMessageBox::critical(this,tr("Form_GeneralTab ComboBox"),tr("Return NULL!"));
        return NULL;
        break;
    }
}

void Form_GeneralTab::Master_AdapterFind_Handle(){
    get_ListWidgetPtr(Master_Aadapter_L)->clear();
//    ui->listWidget_AdapterList->clear();
    My_LisetViewItem *item;
    int index = 0;
    foreach (const QString &str, master->get_AdapterDescription())
    {
        item = new My_LisetViewItem(str);
        item->current_index = index++;
        get_ListWidgetPtr(Master_Aadapter_L)->addItem(item);
//        qDebug() << QString("%1").arg(str);
    }
}


/*********************** 槽函数 **************************************/

void Form_GeneralTab::on_pushButton_AdapterFind_clicked()
{
    master->Find_adapter();
   Master_AdapterFind_Handle();
}

///
/// \brief 双击选择网卡
/// \param item
///
void Form_GeneralTab::on_listWidget_AdapterList_itemDoubleClicked(QListWidgetItem *item)
{
    My_LisetViewItem *my_item = dynamic_cast<My_LisetViewItem *>(item);
//    qDebug() << my_item->current_index;
    QString name = master->get_AdapterName().at(my_item->current_index);
    QString desc = item->text();
    master->set_CurrentAdapter(name,desc);

    ui->lineEdit_AdapterDesc->setText(desc);
    ui->lineEdit_AdapterName->setText(name);
//    qDebug() << master->get_AdapterName().at(my_item->current_index);
}

void Form_GeneralTab::on_pushButton_State_Init_clicked()
{
   if(master->Master_getSlaveCount()>0){
       ui->lineEdit_InquireState->setText(master->Master_stateToString(DRE_Master::STATE_INIT));
       int ret = master->Master_ChangeState(0,DRE_Master::STATE_INIT);
       ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //   qDebug() << "init_Require" << master->Master_stateToString(ret);
    }
}

void Form_GeneralTab::on_pushButton_State_PreOp_clicked()
{
  if(master->Master_getSlaveCount()>0){
      ui->lineEdit_InquireState->setText(master->Master_stateToString(DRE_Master::STATE_PRE_OP));
      int ret = master->Master_ChangeState(0,DRE_Master::STATE_PRE_OP);
      ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //  qDebug() << "PreOp_Require" << master->Master_stateToString(ret);
  }
}

void Form_GeneralTab::on_pushButton_State_SafeOp_clicked()
{
  if(master->Master_getSlaveCount()>0){
      ui->lineEdit_InquireState->setText(master->Master_stateToString(DRE_Master::STATE_SAFE_OP));
      int ret = master->Master_ChangeState(0,DRE_Master::STATE_SAFE_OP);
      ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //  qDebug() << "SafeOp_Require" << master->Master_stateToString(ret);
   }
}

void Form_GeneralTab::on_pushButton_State_Op_clicked()
{
  if(master->Master_getSlaveCount()>0){
      ui->lineEdit_InquireState->setText(master->Master_stateToString(DRE_Master::STATE_OPERATIONAL));
      int ret = master->Master_ChangeState(0,DRE_Master::STATE_OPERATIONAL);
      ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //  qDebug() << "Op_Require" << master->Master_stateToString(ret);
   }
}

void Form_GeneralTab::on_comboBox_PLCPeriod_currentIndexChanged(int index)
{
    //qDebug() << index;
    if(master){
        master->set_PLC_Period(index+1);//设置EtherCAT周期 ms
    }

}

/*****************   槽函数 END   **************************/

