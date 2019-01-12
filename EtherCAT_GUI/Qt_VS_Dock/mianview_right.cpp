#include "mainformview.h"


///
/// \brief 初始化右边的内容框
///
void MainFormView::Init_FrameRight_Content()
{
    QPushButton *button = new QPushButton(tr("插件管理"),this);
    connect(button,SIGNAL(clicked(bool)),this,SLOT(Plugins_Manager_clicked()));
    get_FramePtr(Frame_right_f)->layout()->addWidget(button);
    button = new QPushButton(tr("载入插件"),this);
    connect(button,SIGNAL(clicked(bool)),this,SLOT(Plugins_Add_clicked()));
    get_FramePtr(Frame_right_f)->layout()->addWidget(button);
    button = new QPushButton(tr("打开编辑器"),this);
    connect(button,SIGNAL(clicked(bool)),this,SLOT(Plugins_OpenEditor_clicked()));
    get_FramePtr(Frame_right_f)->layout()->addWidget(button);

    m_pluginList = new My_LisetView_Plugin(this);
    connect(m_pluginList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(mPlugins_item_DoubleClicked(QListWidgetItem*)));
    connect(m_pluginList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(mPlugins_item_Clicked(QListWidgetItem*)));
    get_FramePtr(Frame_right_f)->layout()->addWidget(m_pluginList);

    mTableWidget_property = new QTableWidget(this);
    mTableWidget_property->setColumnCount(2);
    mTableWidget_property->setHorizontalHeaderLabels(QStringList()<<tr("Index")<<tr("Content"));
    mTableWidget_property->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽
//    mTableWidget_property->horizontalHeader()->setStretchLastSection(true);
//    mTableWidget_property->insertRow(0);
//    mTableWidget_property->insertRow(1);
//    mTableWidget_property->insertRow(2);
//    mTableWidget_property->insertRow(3);
//    mTableWidget_property->setItem(0,0,new QTableWidgetItem(tr("Master1")));
//    mTableWidget_property->setItem(0,1,new QTableWidgetItem(tr("Slave1")));

    mTableWidget_property->verticalHeader()->setVisible(false); //设置垂直头不可见
//    mTableWidget_property->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

    get_FramePtr(Frame_right_f)->layout()->addWidget(mTableWidget_property);
}

/************  Slots *******************/


///
/// \brief 双击插件响应
/// \param item
///
void MainFormView::mPlugins_item_DoubleClicked(QListWidgetItem *item)
{
//   clearLayout(mtabWeidgetItem_UserApps->layout());
   LoadPlugins(item->text());
   m_bottomText->appendPlainText(tr("Load ")+item->text());
//    qDebug() << item->text();
}

///
/// \brief 单击插件响应
/// \param item
///
void MainFormView::mPlugins_item_Clicked(QListWidgetItem *item)
{
//   mTableWidget_property->clear();//会把头去掉
    mTableWidget_property->clearContents();
  //父类到子类的强制转化
   My_LisetViewItem_Plugin *my_item = dynamic_cast<My_LisetViewItem_Plugin *>(item);
   // 获取元数据（名称、版本、依赖）
   QJsonObject json = my_item->plugin_jsonMSG;

   QString str;
   str = json.value("author").toVariant().toString();
   Add_TableProperty(mTableWidget_property,0,tr("author"),str);
    str = json.value("date").toVariant().toString();
   Add_TableProperty(mTableWidget_property,1,tr("date"),str);
    str = json.value("name").toVariant().toString();
   Add_TableProperty(mTableWidget_property,2,tr("name"),str);
    str = json.value("version").toVariant().toString();
   Add_TableProperty(mTableWidget_property,3,tr("version"),str);

   QVariantList dependence = json.value("dependencies").toArray().toVariantList();
   if(!dependence.isEmpty()){
       if(dependence.size()==1){
           str = dependence.at(0).toString();
           Add_TableProperty(mTableWidget_property,4,tr("dependency"),str);
       }
       else{
           int dep_num = 1;
           QString dep_tittle;
           foreach(QVariant dep,dependence){
              str = dep.toString();
              dep_tittle = tr("dependency_")+QString::number(dep_num);
              Add_TableProperty(mTableWidget_property,3+dep_num,dep_tittle,str);
              dep_num++;
           }
       }

   }
   else{
       str = "";
       Add_TableProperty(mTableWidget_property,4,tr("dependency"),str);
   }


   //  qDebug() << item->text();
}

///
/// \brief 插件管理点击
///
void MainFormView::Plugins_Manager_clicked()
{
  QString dir = "";
  dir = QFileDialog::getExistingDirectory(this, "请选择插件路径...", "./");//如果没有选择路径就会为空
  if(!dir.isEmpty()){
      m_pluginDir = dir;
//        qDebug() << m_pluginDir;
  }
}

///
/// \brief 扫描插件点击
///
void MainFormView::Plugins_Add_clicked()
{
//   m_pluginList->delete_Item_all();
    m_pluginList->clear();
   ScanPlugins(m_pluginDir);
}
/************  Slots end ***************/
