#include "form_generaltab.h"
#include "ui_form_GeneralTab.h"

#include <QHeaderView>
#include <QCheckBox>

#include <QDebug>

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
    master = new My_EthercatMaster();
    ui->lineEdit_AdapterDesc->setPlaceholderText(tr("Description"));
    ui->lineEdit_AdapterName->setPlaceholderText(tr("Name"));
    ui->lineEdit_ActualState->setPlaceholderText(tr("Actual State"));
    ui->lineEdit_InquireState->setPlaceholderText(tr("Inquire State"));

//    ui->pushButton_State_Op->setEnabled(false);//禁止Op按钮
}

Form_GeneralTab::~Form_GeneralTab()
{
//    delete master;
    master->deleteLater();
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

/*********************** 槽函数 **************************************/

void Form_GeneralTab::on_pushButton_AdapterFind_clicked()
{
    master->Find_adapter();
    ui->listWidget_AdapterList->clear();
    My_LisetViewItem *item;
    int index = 0;
    foreach (const QString &str, master->get_AdapterDescription())
    {
        item = new My_LisetViewItem(str);
        item->current_index = index++;
        ui->listWidget_AdapterList->addItem(item);
//        qDebug() << QString("%1").arg(str);
    }
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
    master->m_adapterDescSelect = desc;
    master->m_adapterNameSelect = name;

    ui->lineEdit_AdapterDesc->setText(desc);
    ui->lineEdit_AdapterName->setText(name);
//    qDebug() << master->get_AdapterName().at(my_item->current_index);
}

void Form_GeneralTab::on_pushButton_State_Init_clicked()
{
   if(master->Master_getSlaveCount()>0){
       ui->lineEdit_InquireState->setText(master->Master_stateToString(My_EthercatMaster::STATE_INIT));
       int ret = master->Master_ChangeState(0,My_EthercatMaster::STATE_INIT);
       ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //   qDebug() << "init_Require" << master->Master_stateToString(ret);
    }
}

void Form_GeneralTab::on_pushButton_State_PreOp_clicked()
{
  if(master->Master_getSlaveCount()>0){
      ui->lineEdit_InquireState->setText(master->Master_stateToString(My_EthercatMaster::STATE_PRE_OP));
      int ret = master->Master_ChangeState(0,My_EthercatMaster::STATE_PRE_OP);
      ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //  qDebug() << "PreOp_Require" << master->Master_stateToString(ret);
  }
}

void Form_GeneralTab::on_pushButton_State_SafeOp_clicked()
{
  if(master->Master_getSlaveCount()>0){
      ui->lineEdit_InquireState->setText(master->Master_stateToString(My_EthercatMaster::STATE_SAFE_OP));
      int ret = master->Master_ChangeState(0,My_EthercatMaster::STATE_SAFE_OP);
      ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //  qDebug() << "SafeOp_Require" << master->Master_stateToString(ret);
   }
}

void Form_GeneralTab::on_pushButton_State_Op_clicked()
{
  if(master->Master_getSlaveCount()>0){
      ui->lineEdit_InquireState->setText(master->Master_stateToString(My_EthercatMaster::STATE_OPERATIONAL));
      int ret = master->Master_ChangeState(0,My_EthercatMaster::STATE_OPERATIONAL);
      ui->lineEdit_ActualState->setText(master->Master_stateToString(ret));
    //  qDebug() << "Op_Require" << master->Master_stateToString(ret);
   }
}

/*****************   槽函数 END   **************************/

