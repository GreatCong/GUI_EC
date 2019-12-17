#include "form_generaltab.h"
#include "ui_form_GeneralTab.h"

#include <QMessageBox>

#include <QHeaderView>
#include <QCheckBox>

#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QMovie>

My_ListViewItem::My_ListViewItem(QListWidget *view):QListWidgetItem(view){}
My_ListViewItem::My_ListViewItem(const QString &text ,QListWidget *view):QListWidgetItem(text,view){}

///
/// \brief 构造函数
/// \param parent
///
Form_GeneralTab::Form_GeneralTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_GeneralTab)
{
    ui->setupUi(this);

    this->Init_cores();
}

///
/// \brief 析构函数
///
Form_GeneralTab::~Form_GeneralTab()
{
    this->Destroy_cores();

    delete ui;
}

///
/// \brief 初始化应用
///
void Form_GeneralTab::Init_cores(){
    ui->lineEdit_AdapterDesc->setPlaceholderText(tr("Description"));
    ui->lineEdit_AdapterName->setPlaceholderText(tr("Name"));
    ui->lineEdit_ActualState->setPlaceholderText(tr("Actual State"));
    ui->lineEdit_InquireState->setPlaceholderText(tr("Inquire State"));

    ui->comboBox_PLCPeriod->setCurrentIndex(9);//设置初始的PLC周期为10ms

//    ui->pushButton_State_Op->setEnabled(false);//禁止Op按钮

    ui->frame_tables->setObjectName("frame_ethercat_icon");//设置名字才能生效
    ui->frame_tables->setStyleSheet("QFrame#frame_ethercat_icon{ border-image : url(:/image1/Resource/Icons/EtherCAT_Icon.png)}" );
    ui->frame_tables->show();
}

///
/// \brief 销毁应用
///
void Form_GeneralTab::Destroy_cores()
{

}

///
/// \brief 获取适配器名字
/// \return
///
const QString Form_GeneralTab::getMaster_adapterName(){
    return ui->lineEdit_AdapterName->text();
}

///
/// \brief 获取适配器的描述
/// \return
///
const QString Form_GeneralTab::getMaster_adapterDesc(){
    return ui->lineEdit_AdapterDesc->text();
}

///
/// \brief 设置适配器的名字
/// \param str
///
void Form_GeneralTab::setMaster_adapterName(const QString &str ){
    ui->lineEdit_AdapterName->setText(str);
}

///
/// \brief 设置适配器的描述
/// \param str
///
void Form_GeneralTab::setMaster_adapterDesc(const QString &str){
   ui->lineEdit_AdapterDesc->setText(str);
}

///
/// \brief 获取ButtonPtr
/// \param choose
/// \return
///
QPushButton *Form_GeneralTab::get_ButtonPtr(buttons_choose choose)
{
    switch(choose){
    case Master_findAdapter_b:
        return ui->pushButton_AdapterFind;
        break;
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

///
/// \brief 获取LineEditPtr
/// \param choose
/// \return
///
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

///
/// \brief 获取ListWidgetPtr
/// \param choose
/// \return
///
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

///
/// \brief 获取ComboBoxPtr
/// \param choose
/// \return
///
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

///
/// \brief 查找适配器的处理
/// \param adapterList
///
void Form_GeneralTab::Master_AdapterFind_Handle(QStringList adapterList){
    get_ListWidgetPtr(Master_Aadapter_L)->clear();
    My_ListViewItem *item;
    int index = 0;
    foreach (const QString &str, adapterList)
    {
        item = new My_ListViewItem(str);
        item->current_index = index++;
        get_ListWidgetPtr(Master_Aadapter_L)->addItem(item);
//        qDebug() << QString("%1").arg(str);
    }
}

/*********************** 槽函数 **************************************/
///
/// \brief 点击事件-查找适配器
///
void Form_GeneralTab::on_pushButton_AdapterFind_clicked()
{
    emit sig_pushButton_clicked(Master_findAdapter_b);
}

///
/// \brief 点击事件-设置Master状态切换到Init
///
void Form_GeneralTab::on_pushButton_State_Init_clicked()
{
    emit sig_pushButton_clicked(Master_stateInit_b);
}

///
/// \brief 点击事件-设置Master状态切换到PreOp
///
void Form_GeneralTab::on_pushButton_State_PreOp_clicked()
{
    emit sig_pushButton_clicked(Master_statePreOp_b);
}

///
/// \brief 点击事件-设置Master状态切换到Op
///
void Form_GeneralTab::on_pushButton_State_SafeOp_clicked()
{
    emit sig_pushButton_clicked(Master_stateSafeOp_b);
}

///
/// \brief 点击事件-设置Master状态切换到SafeOp
///
void Form_GeneralTab::on_pushButton_State_Op_clicked()
{
    emit sig_pushButton_clicked(Master_stateOp_b);
}

///
/// \brief 设置PLC周期
/// \param index
///
void Form_GeneralTab::on_comboBox_PLCPeriod_currentIndexChanged(int index)
{
    if(index < 0){
        return;
    }

    emit sig_ComboBox_changed(Master_periodPLC_c,index);
}

/*****************   槽函数 END   **************************/

