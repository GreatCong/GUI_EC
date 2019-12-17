#include "my_ListView_plugin.h"

#include <QApplication>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>

#include <QMouseEvent>
#include <QDrag>
#include <QDebug>

///
/// \brief 构造函数
/// \param parent
///
My_LisetView_Plugin::My_LisetView_Plugin(QWidget *parent):
    QListWidget(parent)
{
   m_dragItem = nullptr;
}

///
/// \brief 析构函数
///
My_LisetView_Plugin::~My_LisetView_Plugin(){

}

///
/// \brief 鼠标拖动事件
/// \param event
///
void My_LisetView_Plugin::mouseMoveEvent(QMouseEvent *event){
    if(!(event->buttons()&Qt::LeftButton))
        return;
    if((event->pos()-m_dragPosition).manhattanLength()<QApplication::startDragDistance())
        return;
    if(!m_dragItem)
        return;
    QDrag *drag=new QDrag(this);
    QMimeData *mimeData=new QMimeData;
    My_LisetViewItem_Plugin *my_item = dynamic_cast<My_LisetViewItem_Plugin *>(this->currentItem());
    mimeData->setText(my_item->get_Item_fileName());
    drag->setMimeData(mimeData);

    //设置拖动图标
    drag->setPixmap(this->currentItem()->icon().pixmap(50,50));//图标大小50*50

    drag->exec(Qt::CopyAction|Qt::MoveAction);
}

///
/// \brief 鼠标释放事件
/// \param event
///
void My_LisetView_Plugin::mousePressEvent(QMouseEvent *event){
    m_dragPosition=event->pos();
    m_dragItem=this->itemAt(event->pos());

    //保持父类继承下来的按键行为
    QListWidget::mousePressEvent(event);
}

///
/// \brief 删除单个列表项
///
void My_LisetView_Plugin::delete_Item()
{
    //获取列表项的指针
    QListWidgetItem *item = this->takeItem(this->currentRow());
    delete item;        //释放指针所指向的列表项
}

///
/// \brief 删除多个列表项
///
void My_LisetView_Plugin::delete_Item_all()
{
    int num = this->count();  //获取列表项的总数目
    this->setFocus(); //将光标设置到列表框上，若注释该语句，则删除时，要手动将焦点设置到列表框，即点击列表项
    for(int i=0;i<num;i++)
    {   //逐个获取列表项的指针，并删除
        QListWidgetItem *item = this->takeItem(this->currentRow());
        delete item;
    }
}

///
/// \brief 添加多个列表项
///
void My_LisetView_Plugin::add_Item_all()
{
//    QStringList FileNames = QFileDialog::getOpenFileNames(this,"打开",QDir::currentPath(),"所有文件(*.*);;文本文档(*.txt)");
//    //方法1  整体添加
////    ui->listWidget->addItems(FileNames);
//    //方法2  逐个添加
//    int index=0,count=0;
////    QListWidgetItem *item = new QListWidgetItem;  //…………注释1
//    count = FileNames.count();              //获取打开文件的总数目
////    for(index = 0;index<count;index++)    //这样会报错，无法先取出栈底元素  //注释2
//    for(index=count-1;index>=0;index--)     //QList<QString>的数据结构是栈，只能从栈顶取元素
//   {
//        QListWidgetItem *item = new QListWidgetItem;
//        item->setText(FileNames.takeAt(index)); //逐个设置列表项的文本
////        qDebug()<<FileNames.takeAt(index);        //…………注释3
//        this->addItem(item);      //加载列表项到列表框
//   }
}


/*********** Items  ********/
///
/// \brief 构造1
/// \param view
///
My_LisetViewItem_Plugin::My_LisetViewItem_Plugin(QListWidget *view) : QListWidgetItem(view)
{

}

///
/// \brief 构造2
/// \param text
/// \param view
///
My_LisetViewItem_Plugin::My_LisetViewItem_Plugin(const QString &text, QListWidget *view): QListWidgetItem(text,view)
{

}

///
/// \brief 析构函数
///
My_LisetViewItem_Plugin::~My_LisetViewItem_Plugin()
{

}

