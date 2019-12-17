#include "Component_DockRight.h"

#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDateTime>
#include <QApplication>
#include <QDebug>
#include <QJsonArray>
#include <QHeaderView>
#include <QPluginLoader>
#include <QLabel>

#include "objectcontroller/objectcontroller.h"

#include <QMessageBox>

#include "Dock_Obj_Utils.h"
#include "EtherCAT_UserApp.h"

///
/// \brief 构造函数
/// \param parent
///
Component_DockRight::Component_DockRight(QObject *parent):QObject(parent),Component_Private()
{
//    this->Init_Object();
    this->set_AppName("Component_DockRight");

    m_component_Editor = nullptr;

    m_pluginList = nullptr;
    mTableWidget_property = nullptr;

    m_component_loader = nullptr;
    m_component_Editor = nullptr;

    m_DockWidget = nullptr;
    m_Plugin_Loader = nullptr;
}

///
/// \brief 析构函数
///
Component_DockRight::~Component_DockRight()
{
    if(this->get_componentWidgetPtr()){
        delete this->get_componentWidgetPtr();
        this->set_componentWidgetPtr(nullptr);
    }
}

///
/// \brief 初始化(构造后调用)
/// \return
///
bool Component_DockRight::Init_Cores()
{
    QGridLayout *userGridLayout= new QGridLayout();
    this->get_componentWidgetPtr()->setLayout(userGridLayout);

    QPushButton *button = new QPushButton(tr("插件管理"),this->get_componentWidgetPtr());
    connect(button,SIGNAL(clicked(bool)),this,SLOT(Plugins_Manager_clicked()));
    this->get_componentWidgetPtr()->layout()->addWidget(button);
    button = new QPushButton(tr("载入插件"),this->get_componentWidgetPtr());
    connect(button,SIGNAL(clicked(bool)),this,SLOT(Plugins_Add_clicked()));
    this->get_componentWidgetPtr()->layout()->addWidget(button);
    button = new QPushButton(tr("打开编辑器"),this->get_componentWidgetPtr());
    connect(button,SIGNAL(clicked(bool)),this,SLOT(Plugins_OpenEditor_clicked()));
    this->get_componentWidgetPtr()->layout()->addWidget(button);

    m_pluginList = new My_LisetView_Plugin(this->get_componentWidgetPtr());
    connect(m_pluginList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(mPlugins_item_DoubleClicked(QListWidgetItem*)));
    connect(m_pluginList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(mPlugins_item_Clicked(QListWidgetItem*)));
    this->get_componentWidgetPtr()->layout()->addWidget(m_pluginList);

    mTableWidget_property = new QTableWidget(this->get_componentWidgetPtr());
    mTableWidget_property->setColumnCount(2);
    mTableWidget_property->setHorizontalHeaderLabels(QStringList()<<tr("Index")<<tr("Content"));
    //mTableWidget_property->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽
    mTableWidget_property->horizontalHeader()->setStretchLastSection(true);
//    mTableWidget_property->insertRow(0);
//    mTableWidget_property->insertRow(1);
//    mTableWidget_property->insertRow(2);
//    mTableWidget_property->insertRow(3);
//    mTableWidget_property->setItem(0,0,new QTableWidgetItem(tr("Master1")));
//    mTableWidget_property->setItem(0,1,new QTableWidgetItem(tr("Slave1")));

    mTableWidget_property->verticalHeader()->setVisible(false); //设置垂直头不可见
//    mTableWidget_property->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

    this->get_componentWidgetPtr()->layout()->addWidget(mTableWidget_property);

    this->Init_Cores_Editor();
    this->Init_Slots();

    return true;
}

///
/// \brief 销毁(析构前调用)
/// \return
///
bool Component_DockRight::Destroy_Cores()
{
    if(m_component_Editor){
        m_component_Editor->Destroy_Cores();
        m_component_loader->Component_unload();

        delete m_component_loader;
    }

    if(this->get_userAppPtr()){
        this->get_userAppPtr()->Destroy_Cores();
        m_Plugin_Loader->unload();//卸载动态库

        delete m_Plugin_Loader;

        //释放new的对象
//        plugin_userApps->set_UIWidgetPtr(nullptr);//clearLayout会delete UI，需要手动ptr置空
        delete this->get_userAppPtr();

        this->set_userAppPtr(nullptr);
    }

    return true;
}

///
/// \brief 初始化内部对象
/// \return
///
bool Component_DockRight::Init_Object()
{
    m_DockWidget = new QWidget();
    this->set_componentWidgetPtr(m_DockWidget);

    if(this->ComponentP_get_ComponentListPtr()){
         this->ComponentP_get_ComponentListPtr()->append(this);//将自己注册到List中
    }
    else{
        qDebug() << "Component_DockLeft:ComponentP_get_ComponentListPtr=nullptr";
    }

    return true;
}

///
/// \brief 初始化槽
/// \return
///
bool Component_DockRight::Init_Slots()
{
    return true;
}

///
/// \brief 在获取配置信息后初始化
/// \return
///
bool Component_DockRight::Init_Cores_Editor()
{
    m_component_loader = new Component_Loader();
    m_component_loader->set_pluginDir(m_appPath);

    QString componentPlugin_fileName;
    QString componentPlugin_suffixName;

#ifdef Q_OS_WIN
   componentPlugin_fileName = "Component_Editor";
   componentPlugin_suffixName = ".dll";
   m_component_Editor = m_component_loader->Component_load(componentPlugin_fileName + componentPlugin_suffixName);
   if(m_component_Editor == nullptr){
       m_component_Editor = m_component_loader->Component_load(componentPlugin_fileName + "_d"+componentPlugin_suffixName);//方便调试库的加载
   }
#else//Q_OS_UNIX
   componentPlugin_fileName = "libComponent_Editor";
   componentPlugin_suffixName = ".so";
   m_component_Editor = m_component_loader->Component_load(componentPlugin_fileName + componentPlugin_suffixName);
   if(m_component_Editor == nullptr){
       m_component_Editor = m_component_loader->Component_load(componentPlugin_fileName + "_debug"+componentPlugin_suffixName);//方便调试库的加载
   }
#endif

   if(m_component_Editor == nullptr){
       this->System_BottomMessage(componentPlugin_fileName +
                                  componentPlugin_suffixName +
                                  " is not Found or Invalid!");
   }
   else{
       m_component_Editor->Init_Cores();
   }

    return true;
}

///
/// \brief 加载指定的插件
/// \param fileName
/// \return
///
bool Component_DockRight::LoadPlugins(const QString &fileName)
{
    QDir pluginsDir(this->get_ApppPath());

    if(this->get_userAppPtr()){//如果不为空
        this->get_userAppPtr()->Destroy_Cores();
        if(this->get_userAppPtr()->get_MessageObj()){//有消息响应
            disconnect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(MasterStop_Signal()),this,SLOT(Control_MasterStop_Signal()));
            disconnect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(StatusMessage_change(QString,int)),this,SLOT(Control_StatusMessage_change(QString,int)));
            disconnect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(BottomMessage_change(QString)),this,SLOT(Control_BottomMessage_change(QString)));
        }

        //NOTE:会delete UI
        Dock_Obj_Utils::clearLayout(this->ComponentP_get_ComponentWidgetsPtr()->m_tabWeidgetItem_UserApps->layout());//清除layout

        //NOTE:这里unload会调用plugin的析构
        this->m_Plugin_Loader->unload();//卸载动态库
        delete this->m_Plugin_Loader;

        //释放new的对象
        this->get_userAppPtr()->set_UIWidgetPtr(nullptr);//clearLayout会delete UI，需要手动ptr置空
        delete this->get_userAppPtr();

        this->set_userAppPtr(nullptr);
    }

    m_Plugin_Loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
    // 返回插件的根组件对象
    QObject *pPlugin = m_Plugin_Loader->instance();
    if (pPlugin != Q_NULLPTR) {
        QJsonObject json = m_Plugin_Loader->metaData().value("MetaData").toObject();
        QString plugin_type =  json.value("type").toVariant().toString();
        if(plugin_type.compare("Plugin")){//不是插件
//            qDebug() <<"OK";
            QMessageBox::information(this->get_componentWidgetPtr(),tr("Information"),tr("Plugin type is not \"Plugin\" !"));
            return false;
        }

       // 访问感兴趣的接口
       EtherCAT_UserApp *plugin_userApps_origin = qobject_cast<EtherCAT_UserApp *>(pPlugin);
       if (plugin_userApps_origin != Q_NULLPTR) {
           this->set_userAppPtr(plugin_userApps_origin->get_NewAppPtr());//new一个新的对象,可以delete
           this->get_userAppPtr()->Init_Object();//初始化相关的对象(其他功能实现的前提)

           this->get_userAppPtr()->set_appPath(this->get_ApppPath());//传递app的路径

           if(this->get_userAppPtr()->get_isUsePropertyWidget()){
               if(this->get_userAppPtr()->get_PropertyWidgetPtr()){
                   Dock_Obj_Utils::clearLayout(this->ComponentP_get_ComponentWidgetsPtr()->m_userPropertyWidget->layout());//清除layout
                   this->ComponentP_get_ComponentWidgetsPtr()->m_userPropertyWidget->layout()->addWidget(this->get_userAppPtr()->get_PropertyWidgetPtr());
               }

               this->ComponentP_get_ComponentWidgetsPtr()->m_controllerPropertyWidget->hide();
               this->ComponentP_get_ComponentWidgetsPtr()->m_userPropertyWidget->show();
           }
           else{
               this->ComponentP_get_ComponentWidgetsPtr()->m_controllerPropertyWidget->show();
               this->ComponentP_get_ComponentWidgetsPtr()->m_userPropertyWidget->hide();
           }

           //NOTE:为了让错误消息能够显示，先连接消息
           if(this->get_userAppPtr()->get_MessageObj()){//有消息响应就连接信号槽
               connect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(MasterStop_Signal()),this,SLOT(Control_MasterStop_Signal()));
               connect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(StatusMessage_change(QString,int)),this,SLOT(Control_StatusMessage_change(QString,int)));
               connect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(BottomMessage_change(QString)),this,SLOT(Control_BottomMessage_change(QString)));
               this->get_userAppPtr()->get_MessageObj()->setObjectName(this->get_userAppPtr()->get_AppName()+"_MSG");
               dynamic_cast<ObjectController*>(this->ComponentP_get_ComponentWidgetsPtr()->m_controllerPropertyWidget)->setObject(this->get_userAppPtr()->get_MessageObj());
           }
           else{
               dynamic_cast<ObjectController*>(this->ComponentP_get_ComponentWidgetsPtr()->m_controllerPropertyWidget)->setObject(nullptr);
           }

           bool ret = this->get_userAppPtr()->Init_Cores();
           if(ret == false){
               QMessageBox::warning(this->get_componentWidgetPtr(),"Warning","UserApp init error!");
               if(this->get_userAppPtr()->get_MessageObj()){//有消息响应
                   disconnect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(MasterStop_Signal()),this,SLOT(Control_MasterStop_Signal()));
                   disconnect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(StatusMessage_change(QString,int)),this,SLOT(Control_StatusMessage_change(QString,int)));
                   disconnect(this->get_userAppPtr()->get_MessageObj(),SIGNAL(BottomMessage_change(QString)),this,SLOT(Control_BottomMessage_change(QString)));
               }
               //NOTE:这里unload会调用plugin的析构
               this->m_Plugin_Loader->unload();//卸载动态库
               delete this->m_Plugin_Loader;

               delete this->get_userAppPtr();

               this->set_userAppPtr(nullptr);

               return false;
           }

           if(this->get_userAppPtr()->get_UIWidgetPtr() == nullptr){//如果没有UI,就定义一个默认的UI
               QLabel *plugin_default_UI = new QLabel(fileName+" UI");
               this->ComponentP_get_ComponentWidgetsPtr()->m_tabWeidgetItem_UserApps->layout()->addWidget(plugin_default_UI);
           }
           else{
//               qDebug() << mtabWeidgetItem_UserApps->width() << mtabWeidgetItem_UserApps->height();
                this->ComponentP_get_ComponentWidgetsPtr()->m_tabWeidgetItem_UserApps->layout()->addWidget(this->get_userAppPtr()->get_UIWidgetPtr());
           }

           //显示UI
           this->ComponentP_get_ComponentWidgetsPtr()->m_tabWedget_center->show();
           this->ComponentP_get_ComponentWidgetsPtr()->m_widget_slaveMSG->hide();
           dynamic_cast<QTabWidget*>(this->ComponentP_get_ComponentWidgetsPtr()->m_tabWedget_center)->setCurrentIndex(1);//显示自定义界面
       } else {
           //qWarning() << "qobject_cast falied";
           System_BottomMessage(tr("qobject_cast falied"));
//           Master_exit();
           return false;
       }
    }

    return true;
}

///
/// \brief 扫描插件
/// \param plugin_dir
/// \return
///
int Component_DockRight::ScanPlugins(const QString &plugin_dir)
{
    int plugin_num = 0;

    QDir pluginsDir(plugin_dir);

       // 查找目录中的所有插件
       foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
           QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
           // 返回插件的根组件对象
           QObject *pPlugin = loader.instance();
           if (pPlugin != Q_NULLPTR) {
               plugin_num ++;

//               // 获取元数据（名称、版本、依赖）
//               QJsonObject json = loader.metaData().value("MetaData").toObject();
//               qDebug() << "********** MetaData **********";
//               qDebug() << json.value("author").toVariant();
//               qDebug() << json.value("date").toVariant();
//               qDebug() << json.value("name").toVariant();
//               qDebug() << json.value("version").toVariant();
//               qDebug() << json.value("dependencies").toArray().toVariantList();

//               QListWidgetItem *item = new QListWidgetItem(fileName);
//               QJsonObject json = loader.metaData().value("MetaData").toObject();
//               qDebug() << json.value("Keys").toVariant();
//               qDebug() << json.value("dependencies").toArray().toVariantList();

               QJsonObject json = loader.metaData().value("MetaData").toObject();
               QString plugin_type =  json.value("type").toVariant().toString();
               My_LisetViewItem_Plugin *item = nullptr;
               if(!plugin_type.compare("Plugin")){//如果是插件
                   // 访问感兴趣的接口
                   EtherCAT_UserApp *plugin_userApps_origin = qobject_cast<EtherCAT_UserApp *>(pPlugin);
                   if (plugin_userApps_origin != Q_NULLPTR) {
                       item= new My_LisetViewItem_Plugin(plugin_userApps_origin->get_AppName());
                       item->plugin_jsonMSG = json;
                       item->setIcon(plugin_userApps_origin->get_AppIcon());
                       item->set_Item_fileName(fileName);
                       m_pluginList->addItem(item);
                   }
               }
               else if(!plugin_type.compare("Master")){ //如果是master
                   item= new My_LisetViewItem_Plugin("EtherCAT Master");
                   item->plugin_jsonMSG = json;
                   item->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
                   item->set_Item_fileName(fileName);
                   m_pluginList->addItem(item);
               }
               else if(!plugin_type.compare("Component")){//如果是组件库
                   Component *plugin_userComponent_origin = qobject_cast<Component *>(pPlugin);
                   if (plugin_userComponent_origin != Q_NULLPTR) {
                       item= new My_LisetViewItem_Plugin(plugin_userComponent_origin->get_AppName());
                       item->plugin_jsonMSG = json;
                       item->setIcon(plugin_userComponent_origin->get_AppIcon());
                       item->set_Item_fileName(fileName);
                       m_pluginList->addItem(item);
                   }
               }
               else{
                   qDebug() << "Other Plugin_type!";
               }

               //qDebug() << fileName;

           }
           //NOTE:这里会调用plugin的析构
           loader.unload();
           loader.deleteLater();

           this->set_userAppPtr(nullptr);
       }

       return plugin_num;
}

///
/// \brief 在底部状态栏显示信息
/// \param message
///
void Component_DockRight::System_BottomMessage(const QString &message)
{
    this->ComponentP_get_ComponentWidgetsPtr()->Set_BottomMessage(message);
}

/********************** slots begin **************************/

///
/// \brief 双击插件响应
/// \param item
///
void Component_DockRight::mPlugins_item_DoubleClicked(QListWidgetItem *item)
{
//   clearLayout(mtabWeidgetItem_UserApps->layout());
   My_LisetViewItem_Plugin *my_item = dynamic_cast<My_LisetViewItem_Plugin *>(item);
   LoadPlugins(my_item->get_Item_fileName());
   QString message = tr("Load ")+ item->text() + " (" +my_item->get_Item_fileName() + ")";
   System_BottomMessage(message);
//    qDebug() << item->text();
}

///
/// \brief 单击插件响应
/// \param item
///
void Component_DockRight::mPlugins_item_Clicked(QListWidgetItem *item)
{
//   mTableWidget_property->clear();//会把头去掉
    mTableWidget_property->clearContents();
  //父类到子类的强制转化
   My_LisetViewItem_Plugin *my_item = dynamic_cast<My_LisetViewItem_Plugin *>(item);
   // 获取元数据（名称、版本、依赖）
   QJsonObject json = my_item->plugin_jsonMSG;

   QString str;
   str = json.value("author").toVariant().toString();
   Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,0,tr("author"),str);
    str = json.value("date").toVariant().toString();
   Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,1,tr("date"),str);
    str = json.value("name").toVariant().toString();
   Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,2,tr("name"),str);
    str = json.value("version").toVariant().toString();
   Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,3,tr("version"),str);
   str = json.value("type").toVariant().toString();
   Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,4,tr("type"),str);
   str = json.value("description").toVariant().toString();
   Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,5,tr("description"),str);

   QVariantList dependence = json.value("dependencies").toArray().toVariantList();
   int dep_num = 1;
   int property_index = 5;//一共有5个属性

   if(!dependence.isEmpty()){
       if(dependence.size()==1){
           str = dependence.at(0).toString();
           Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,property_index+dep_num,tr("dependency"),str);
           dep_num++;
       }
       else{
           QString dep_tittle;
           foreach(QVariant dep,dependence){
              str = dep.toString();
              dep_tittle = tr("dependency_")+QString::number(dep_num);
              Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,property_index+dep_num,dep_tittle,str);
              dep_num++;
           }
       }

   }
   else{
       str = "";
       Dock_Obj_Utils::Add_TableProperty(mTableWidget_property,property_index+dep_num,tr("dependency"),str);
       dep_num++;
   }

   mTableWidget_property->setRowCount(property_index+dep_num);//重新设置下行数


   //  qDebug() << item->text();
}

///
/// \brief 插件管理点击
///
void Component_DockRight::Plugins_Manager_clicked()
{
  QString dir = "";
  dir = QFileDialog::getExistingDirectory(this->get_componentWidgetPtr(), "请选择插件路径...", m_appPath);//如果没有选择路径就会为空
  if(!dir.isEmpty()){
      m_appPath = dir;
//        qDebug() << m_pluginDir;
      foreach (Component_Private* component, *(this->ComponentP_get_ComponentListPtr())) {
          component->set_AppPath(m_appPath);
      }
      this->ComponentP_get_ComponentWidgetsPtr()->Set_AppPathChange(m_appPath);//发给主窗体
  }
}

///
/// \brief 扫描插件点击
///
void Component_DockRight::Plugins_Add_clicked()
{
//   m_pluginList->delete_Item_all();
    m_pluginList->clear();
   ScanPlugins(m_appPath);
}

///
/// \brief 打开代码编辑器点击
///
void Component_DockRight::Plugins_OpenEditor_clicked(){
    if(m_component_Editor){
        m_component_Editor->set_AppPath(this->get_ApppPath());
        m_component_Editor->get_componentWidgetPtr()->show();
    }
    else{
       QMessageBox::warning(this->get_componentWidgetPtr(),"Warning","Component_Editor is invalid!");
    }
}

///
/// \brief 代码编辑器的消息事件
/// \param str
///
void Component_DockRight::Plugins_codeEditor_MSG(const QString str){
    QString str_curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//获取当前的时间
    System_BottomMessage("Editor( "+str_curTime+" ): "+str);
}


///
/// \brief 用于插件的事件
/// \param message
/// \param interval
///
void Component_DockRight::Control_StatusMessage_change(QString message,int interval){
    this->ComponentP_get_ComponentWidgetsPtr()->Set_StatusMessage(message,interval);
}

///
/// \brief 用于插件的事件
/// \param message
///
void Component_DockRight::Control_BottomMessage_change(QString message){
    this->ComponentP_get_ComponentWidgetsPtr()->Set_BottomMessage(message);
}

///
/// \brief 用于插件的事件
///
void Component_DockRight::Control_MasterStop_Signal(){
    emit this->ComponentP_get_ComponentWidgetsPtr()->Set_MasterStop();
}

/************************ slots end **********************/
