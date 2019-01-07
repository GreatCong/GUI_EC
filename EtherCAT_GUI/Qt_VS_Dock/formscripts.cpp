#include "formscripts.h"
#include "ui_formscripts.h"
#include <QHeaderView>
#include <QCheckBox>

FormScripts::FormScripts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormScripts)
{
    ui->setupUi(this);

    table = new QTableWidget(this);
//    QGridLayout *layout = new QGridLayout();//通过UI创建的，自带一个layout
//    ui->frame_tables->setLayout(layout);
//    ui->frame_tables->layout()->addWidget(table);
//    table->setColumnCount(5);
//    table->setHorizontalHeaderLabels(QStringList()<<tr("Master")<<tr("Slave")<<tr("Name")<<tr("Alias")<<tr("State"));
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽

//    table->setRowCount(3);
//    QTableWidgetItem *tableItem = new QTableWidgetItem(tr("1"));
//    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    table->setItem(0,0,tableItem);

//    tableItem = new QTableWidgetItem(tr("1"));
//    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    table->setItem(0,1,tableItem);

//    tableItem = new QTableWidgetItem(tr("Input1"));
//    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    table->setItem(0,2,tableItem);

//    tableItem = new QTableWidgetItem(tr("My_Input1"));
//    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    table->setItem(0,3,tableItem);

//    QCheckBox *checkBox =new QCheckBox(tr("isOutput"));
//    checkBox->setChecked(true);
//    table->setCellWidget(0,4,checkBox);

//    tableItem = new QTableWidgetItem(tr("..."));
//    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    table->setItem(1,0,tableItem);

}

FormScripts::~FormScripts()
{
    delete ui;
}
