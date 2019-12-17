#include "my_Table_SlaveMSG.h"
#include <QHeaderView>

/****************  My_TableWidget_Dispaly ********************************/
///
/// \brief 初始化
/// \param parent
///
My_TableWidget_Dispaly::My_TableWidget_Dispaly(QWidget *parent): QTableWidget(parent)
{

}

///
/// \brief 添加title
/// \param title
///
void My_TableWidget_Dispaly::addTitle(const QStringList &title)
{
    int count  = title.count();
    this->setColumnCount(count);
    this->setHorizontalHeaderLabels(title);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽
}

///
/// \brief 添加一行信息
/// \param raw
/// \param data
///
void My_TableWidget_Dispaly::append_RawData(int raw,const QStringList &data)
{
    int num = 0;
    int raw_count = this->rowCount();
//    this->insertRow(raw_count);
    if(raw+1 > raw_count){
        this->setRowCount(raw+1);//num从0开始
    }

    foreach (QString str, data) {
        QTableWidgetItem *tableItem = new QTableWidgetItem(str);
        tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        this->setItem(raw,num++,tableItem);
    }
}

/****************  My_TableWidget_Dispaly end ********************************/


/****************  My_Table_SlaveMSG ********************************/
///
/// \brief 构造函数
/// \param parent
///
My_Table_SlaveMSG::My_Table_SlaveMSG(QWidget *parent) : My_TableWidget_Dispaly(parent)
{

}

///
/// \brief 初始化
///
void My_Table_SlaveMSG::init()
{
    QString str_title;
    QStringList str_title_List;
    str_title = "slave_index";
    str_title_List.append(str_title);
    str_title = "slave_name";
    str_title_List.append(str_title);
    str_title = "slave_Obits";
    str_title_List.append(str_title);
    str_title = "slave_Ibits";
    str_title_List.append(str_title);
    str_title = "slave_eep_man";
    str_title_List.append(str_title);
    str_title = "slave_eep_id";
    str_title_List.append(str_title);
    str_title = "slave_eep_rev";
    str_title_List.append(str_title);

    this->addTitle(str_title_List);
}

/********************* My_Table_SlaveMSG end **************************/

/********************* My_Table_SlaveItemMSG end **************************/
///
/// \brief 构造函数
/// \param parent
///
My_Table_SlaveItemMSG::My_Table_SlaveItemMSG(QWidget *parent): My_TableWidget_Dispaly(parent)
{

}

///
/// \brief 初始化
///
void My_Table_SlaveItemMSG::init()
{
    QString str_title;
    QStringList str_title_List;
    str_title = "slave";
    str_title_List.append(str_title);//0
    str_title = "sm_num";
    str_title_List.append(str_title);//1
    str_title = "addr_offset";
    str_title_List.append(str_title);//2
    str_title = "addr_bit";
    str_title_List.append(str_title);//3
    str_title = "obj_index";
    str_title_List.append(str_title);//4
    str_title = "obj_subindex";
    str_title_List.append(str_title);//5
    str_title = "bitLen";
    str_title_List.append(str_title);//6
    str_title = "dataType";
    str_title_List.append(str_title);//7
    str_title = "Item_name";
    str_title_List.append(str_title);//8

    this->addTitle(str_title_List);

    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);//对第7列单独设置根据内容设置宽度
    this->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);//对第7列单独设置根据内容设置宽度
    this->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);//对第7列单独设置根据内容设置宽度
    this->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);//对第8列单独设置根据内容设置宽度
//    this->setColumnWidth(7, 45);//设置固定宽度
}

/********************* My_Table_SlaveItemMSG end **************************/

