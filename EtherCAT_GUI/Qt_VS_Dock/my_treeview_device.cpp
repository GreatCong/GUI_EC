/*
* 设备树的定义
*/

#include "my_treeview_device.h"
#include <QDebug>

My_TreeView_Device::My_TreeView_Device(QWidget *parent) : QTreeView(parent)
{
  Init_Left_DeviceTree();

  s_master_count = 1;
  s_slave_count = 1;
  s_slaveMessage_count = 1;
}

///
/// \brief 初始化Ethecat设备树
///
void My_TreeView_Device::Init_Left_DeviceTree(){
//    mTreeView_doc = new QTreeView(this);
    this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);//设置自定义菜单栏
//    Init_DeviceTreeItem_menu();
//    connect(mTreeView_doc,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(mTreeView_doc_customContextMenuRequested(const QPoint &)));
//    connect(mTreeView_doc,SIGNAL(doubleClicked ( const QModelIndex)),this,SLOT(mTreeView_doc_DoubleClicked(const QModelIndex)));

    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList()<<tr("项目"));
    //添加设备树
    QStandardItem* root = new QStandardItem(QIcon(":/image1/Resource/Icons/Device.png"),tr("Device"));
    //QTreeView只负责显示渲染，数据都是Model来负责管理 setData相当于是做标记 setData(变量,角色)
    root->setData(MARK_DEVICE,ROLE_MARK);//首先它是项目中Device节点
    root->setData(MARK_DEVICE,ROLE_MARK_DEVICE);//其次它属于Device节点
    root->setData(
                   tr("Ethecat设备树，请先进行从站扫描！\n")
                   ,Qt::ToolTipRole
                   );//利用QT自带的Role,类似于Tooltips
    model->appendRow(root);

    this->setModel(model);//添加模型
    this->setAutoScroll(true);
    this->setEditTriggers(0);//设置不可编辑
}

///
/// \brief 添加主站到设备树
/// \return
///
bool My_TreeView_Device::Add_LeftTree_Master(){
//    static int s_master_count = 1;

    QStandardItemModel* masterFolder = getTreeModel();//master直接挂在root
    QStandardItem* item = new QStandardItem(QIcon(":/image1/Resource/Icons/Master.png")
                                            ,tr("Master%1").arg(s_master_count));
    item->setData(MARK_MASTER+s_master_count,ROLE_MARK);//标记为master
    item->setData(MARK_MASTER+s_master_count,ROLE_MARK_MASTER);//再次标定为master
    masterFolder->appendRow(item);
    ++s_master_count;

    return true;
}

///
/// \brief 获取主站在设备中的节点
/// \param master_count
/// \return
///
QStandardItem* My_TreeView_Device::getMasterFolder(int master_count)
{
    QList<QStandardItem*> roots = getRoots();
    for(auto i=roots.begin();i!=roots.end();++i){
        if((*i)->data(ROLE_MARK) == MARK_MASTER + master_count){
            return (*i);
        }
    }
    return nullptr;
}

///
/// \brief 添加从站到设备树中
/// \param master_count
/// \return
///
bool My_TreeView_Device::Add_LeftTree_Slaves(int master_count){
    return Add_LeftTree_Slaves(master_count,"");
}

bool My_TreeView_Device::Add_LeftTree_Slaves(int master_count,const QString &slave_name){
//    static int s_slave_count = 1;

    QStandardItem* masterFolder = getMasterFolder(master_count);//先查找master的节点
    if(masterFolder)
    {
        QStandardItem* item = new QStandardItem(QIcon(":/image1/Resource/Icons/Slave.png")
                                                ,tr("Slave%1").arg(s_slave_count));
        if(!slave_name.isEmpty()){//load slave_name
            QString str = item->text()+"(" + slave_name + ")";
            item->setText(str);
        }
        item->setData(MARK_SLAVE+s_slave_count,ROLE_MARK);//标记为master
        item->setData(MARK_SLAVE+s_slave_count,ROLE_MARK_MASTER);//再次标定为master
        item->setData(MARK_SLAVE+s_slave_count,ROLE_MARK_SLAVE);//再次标定为master
        masterFolder->appendRow(item);

        Add_LeftTree_SlaveItems(master_count,s_slave_count,MARK_SLAVE_ITEM_INPUT);//每次添加slave就添加一对输入输出
        Add_LeftTree_SlaveItems(master_count,s_slave_count,MARK_SLAVE_ITEM_OUTPUT);

        ++s_slave_count;

        return true;
    }

    return false;
}

///
/// \brief 获取从站在设备树中的节点
/// \param master_count
/// \param slave_count
/// \return
///
QStandardItem* My_TreeView_Device::getSlaveFolder(int master_count,int slave_count)
{
    QStandardItem* master = getMasterFolder(master_count);//获取主站的节点
    if(nullptr == master)
        return nullptr;
    for(int i=0;i < master->rowCount();++i)
    {
        QStandardItem* child = master->child(i);
        QVariant var = child->data(ROLE_MARK_SLAVE);
        if(!var.isValid())
            continue;//说明不是ROLE_MARK_FOLDER，有可能是一些项目，对应项目结构树那个xxx.pro就是一个非文件夹条目
        if(MARK_SLAVE+slave_count == var.value<int>())
            return child;
    }
    return nullptr;
}

///
/// \brief 添加从站类别到设备树中
/// \param master_count
/// \param slave_count
/// \param slaveItem
/// \return
///
bool My_TreeView_Device::Add_LeftTree_SlaveItems(int master_count, int slave_count, int slaveItem){//从站输入输出
//    static int s_slaveItem_count = 1;

//    QStandardItem* slaveFolder = getSlaveFolder(master_count,slave_count);//先查找slave的节点
//    if(slaveFolder)
//    {
//        QStandardItem* item = new QStandardItem(QIcon(":/image1/Resource/Icons/Arrow_Circle_Right_128px.png")
//                                                ,tr("SlaveItem%1").arg(s_slaveItem_count));
//        item->setData(MARK_SLAVE_ITEM+s_slaveItem_count,ROLE_MARK);//标记为slave item
//        item->setData(MARK_SLAVE_ITEM+s_slaveItem_count,ROLE_MARK_MASTER);//再次标定为master
//        item->setData(MARK_SLAVE_ITEM+s_slaveItem_count,ROLE_MARK_SLAVE);//再次标定为master
//        item->setData(MARK_SLAVE_ITEM+s_slaveItem_count,ROLE_MARK_SLAVE_ITEM);//再次标定为master
//        slaveFolder->appendRow(item);
//        ++s_slaveItem_count;

//        return true;
//    }

//    return false;

    QStandardItem* slaveFolder = getSlaveFolder(master_count,slave_count);//先查找slave的节点
    if(slaveFolder)
    {
        QStandardItem* item = new QStandardItem();
        switch(slaveItem){
        case MARK_SLAVE_ITEM_INPUT://输入
            item->setText(tr("Input"));
            item->setIcon(QIcon(":/image1/Resource/Icons/Input.png"));

            item->setData(MARK_SLAVE_ITEM_INPUT,ROLE_MARK);//标记为slave item
            item->setData(MARK_SLAVE_ITEM_INPUT,ROLE_MARK_MASTER);//再次标定为master
            item->setData(MARK_SLAVE_ITEM_INPUT,ROLE_MARK_SLAVE);//再次标定为master
            item->setData(MARK_SLAVE_ITEM_INPUT,ROLE_MARK_SLAVE_ITEM);//再次标定为master
            slaveFolder->appendRow(item);
            break;
        case MARK_SLAVE_ITEM_OUTPUT://输出
            item->setText(tr("Output"));
            item->setIcon(QIcon(":/image1/Resource/Icons/Output.png"));

            item->setData(MARK_SLAVE_ITEM_OUTPUT,ROLE_MARK);//标记为slave item
            item->setData(MARK_SLAVE_ITEM_OUTPUT,ROLE_MARK_MASTER);//再次标定为master
            item->setData(MARK_SLAVE_ITEM_OUTPUT,ROLE_MARK_SLAVE);//再次标定为master
            item->setData(MARK_SLAVE_ITEM_OUTPUT,ROLE_MARK_SLAVE_ITEM);//再次标定为master
            slaveFolder->appendRow(item);
            break;
        default:
            break;

        }

        return true;
    }

    return false;
}

///
/// \brief 获取从站类别在设备树中的节点
/// \param master_count
/// \param slave_count
/// \param slaveItem_count
/// \return
///
QStandardItem* My_TreeView_Device::getSlaveItemFolder(int master_count,int slave_count,int slaveItem_count){
    QStandardItem* slave = getSlaveFolder(master_count,slave_count);//获取从站的节点
    if(nullptr == slave)
        return nullptr;
    for(int i=0;i < slave->rowCount();++i)
    {
        QStandardItem* child = slave->child(i);
        QVariant var = child->data(ROLE_MARK_SLAVE_ITEM);
        if(!var.isValid())
            continue;//说明不是ROLE_MARK_FOLDER，有可能是一些项目，对应项目结构树那个xxx.pro就是一个非文件夹条目
        if(MARK_SLAVE_ITEM+slaveItem_count == var.value<int>())
            return child;
    }
    return nullptr;
}

///
/// \brief 添加从站信息到设备树中
/// \param master_count
/// \param slave_count
/// \param slaveItem_count
/// \return
///
bool My_TreeView_Device::Add_LeftTree_SlaveMessage(int master_count,int slave_count,int slaveItem_count){
//    static int s_slaveMessage_count = 1;

    if(Add_LeftTree_SlaveMessage(master_count,slave_count,slaveItem_count,tr("SlaveMessage%1").arg(s_slaveMessage_count))){
        ++s_slaveMessage_count;
        return true;
    }

    return false;
}

///
/// \brief 添加从站信息到设备树中
/// \param master_count
/// \param slave_count
/// \param slaveItem_count
/// \param message
/// \return
///
bool My_TreeView_Device::Add_LeftTree_SlaveMessage(int master_count,int slave_count,int slaveItem_count,const QString &message ){

    QStandardItem* slaveItemFolder = getSlaveItemFolder(master_count,slave_count,slaveItem_count);//先查找slaveItem的节点
    if(slaveItemFolder)
    {
        QStandardItem* item = new QStandardItem(message);
        item->setData(MARK_SLAVE_MESSAGE,ROLE_MARK);//标记为slave item
        item->setData(MARK_SLAVE_MESSAGE,ROLE_MARK_MASTER);//再次标定为master
        item->setData(MARK_SLAVE_MESSAGE,ROLE_MARK_SLAVE);//再次标定为master
        item->setData(MARK_SLAVE_MESSAGE,ROLE_MARK_SLAVE_ITEM);//再次标定为master
        item->setData(MARK_SLAVE_MESSAGE,ROLE_MARK_SLAVE_MESSAGE);//再次标定为master
        slaveItemFolder->appendRow(item);

        switch(slaveItem_count){
        case MARK_SLAVE_ITEM_INPUT-MARK_SLAVE_ITEM:
            item->setIcon(QIcon(":/image1/Resource/Icons/Arrow_left.png"));
            break;
        case MARK_SLAVE_ITEM_OUTPUT-MARK_SLAVE_ITEM:
            item->setIcon(QIcon(":/image1/Resource/Icons/Arrow_right.png"));
            break;
        default:
            break;
        }

        return true;
    }

    return false;
}


///
/// \brief 获取设备树的model
/// \return
///
QStandardItemModel* My_TreeView_Device::getTreeModel()
{
    return qobject_cast<QStandardItemModel*>(this->model());
}

///
/// \brief 获取设备树的根节点
/// \return
///
QList<QStandardItem*> My_TreeView_Device::getRoots()
{
    QList<QStandardItem*> roots;
    QStandardItemModel* model = getTreeModel();
    for(int i=0;i < model->rowCount();++i)
    {
        roots.append(model->item(i));
    }
    return roots;
}

bool My_TreeView_Device::isItemsMark(int mark,int mark_def,int interval){
    int n = mark - mark_def;
    if(n >= 0 && n < interval){
        return true;
    }
    else{
        return false;
    }
}

bool My_TreeView_Device::isMaster(int mark){
    return isItemsMark(mark,MARK_MASTER,100);
}

bool My_TreeView_Device::isSlave(int mark){
    return isItemsMark(mark,MARK_SLAVE,100);
}

bool My_TreeView_Device::isSlaveItem(int mark){
    return isItemsMark(mark,MARK_SLAVE_ITEM,100);
}

bool My_TreeView_Device::isSlaveMessage(int mark){
    return isItemsMark(mark,MARK_SLAVE_MESSAGE,100);
}

