#include "Dock_Obj_Utils.h"

///
/// \brief 构造函数
///
Dock_Obj_Utils::Dock_Obj_Utils()
{

}


///
/// \brief 从list中输出到QPlainTextEdit
/// \param edit_widget
/// \param list
///
void Dock_Obj_Utils::TextList_append(QPlainTextEdit *edit_widget,const QStringList &list){
    foreach (QString str, list) {
        edit_widget->appendPlainText(str);
    }
}

///
/// \brief 向table中添加插件属性
/// \param table
/// \param num
/// \param str1
/// \param str2
/// \return
///
bool Dock_Obj_Utils::Add_TableProperty(QTableWidget *table,int num,QString str1,QString str2){
    int rawCount = table->rowCount();
    if(num+1 > rawCount){
        table->setRowCount(num+1);//num从0开始
    }

   AddTableContent(table,num,0,str1);
   AddTableContent(table,num,1,str2);
    return true;
}

///
/// \brief 向table中添加内容
/// \param table
/// \param raw
/// \param column
/// \param str
/// \return
///
bool Dock_Obj_Utils::AddTableContent(QTableWidget *table,int raw,int column, QString &str){
    QTableWidgetItem *tableItem = new QTableWidgetItem(str);
    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    table->setItem(raw,column,tableItem);

    return true;
}

///
/// \brief 清除layout的widget
/// \param layout
///
void Dock_Obj_Utils::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)) != 0){
        //删除widget
        if(item->widget()){
            delete item->widget();
            //item->widget()->deleteLater();
        }
        //删除子布局
        QLayout *childLayout = item->layout();
        if(childLayout){
            clearLayout(childLayout);
        }
        delete item;
    }
}
