#include "mainformview.h"

///
/// \brief 初始化左边的内容框
///
void MainFormView::Init_FrameLeft_Content()
{
//    Init_Left_DeviceTree();
    mDeviceTree = new My_TreeView_Device(this);
    get_FramePtr(Frame_left_f)->layout()->addWidget(mDeviceTree);

    Init_DeviceTreeItem_menu();//初始化右键菜单
    //连接信号槽
    connect(mDeviceTree,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(mDeviceTree_customContextMenuRequested(const QPoint &)));
    connect(mDeviceTree,SIGNAL(doubleClicked ( const QModelIndex)),this,SLOT(mDeviceTree_DoubleClicked(const QModelIndex)));
    connect(mDeviceTree,SIGNAL(expanded(const QModelIndex)),this,SLOT(mDeviceTree_expanded(const QModelIndex)));
}

///
/// \brief 初始化设备树的菜单栏
///
void MainFormView::Init_DeviceTreeItem_menu()
{
    //创建menu
    m_projectMenu = new QMenu(this);
    m_itemMenu = new QMenu(this);

    //Device mexu
    QAction* ac = nullptr;
    ac = new QAction(tr("Scan"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(mDeviceTree_EcScan_clicked()));//关联槽函数
    m_projectMenu->addAction(ac);

    m_projectMenu->addSeparator();//添加分割线

    QMenu* itemTestChildMenu = new QMenu(m_projectMenu);
    itemTestChildMenu->setTitle(tr("Test"));

    ac = new QAction(tr("Add Master"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(AddTree_Master_clicked()));//关联槽函数
    itemTestChildMenu->addAction(ac);

    ac = new QAction(tr("Add Slave"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(AddTree_Slave_clicked()));
    itemTestChildMenu->addAction(ac);

    ac = new QAction(tr("Add SlaveItem"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(AddTree_SlaveItem_clicked()));
    itemTestChildMenu->addAction(ac);

    ac = new QAction(tr("Add SlaveMessage_Input"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(AddTree_SlaveMessageInput_clicked()));
    itemTestChildMenu->addAction(ac);
    ac = new QAction(tr("Add SlaveMessage_Output"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(AddTree_SlaveMessageOutput_clicked()));
    itemTestChildMenu->addAction(ac);

    m_projectMenu->addAction(itemTestChildMenu->menuAction());

    ac = new QAction(tr("Change to Link"),this);
    m_itemMenu->addAction(ac);
    ac = new QAction(tr("Change to unLink"),this);
    m_itemMenu->addAction(ac);
    ac = new QAction(tr("OnLine Write"),this);
    m_itemMenu->addAction(ac);
    ac = new QAction(tr("OnLine Read"),this);
    m_itemMenu->addAction(ac);
}

/************  Slots *******************/
///
/// \brief Tree自定义菜单消息
/// \param pos
///
void MainFormView::mDeviceTree_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = mDeviceTree->indexAt(pos);
    QVariant var = index.data(ROLE_MARK);
    if(var.isValid())
    {
        if(MARK_DEVICE == var.toInt())
            m_projectMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
//        else if(MARK_SLAVE_MESSAGE == var.toInt())
        else if(mDeviceTree->isSlaveMessage(var.toInt()))
            m_itemMenu->exec(QCursor::pos());
    }
}

///
/// \brief Tree双击响应
/// \param index
///
void MainFormView::mDeviceTree_DoubleClicked(const QModelIndex index)
{
    QVariant var = index.data(ROLE_MARK);
    if(var.isValid())
    {
        if(mDeviceTree->isMaster(var.toInt())){//project才会弹出新窗口
            mTabWedget_center->show();
            m_widget_slaveMSG->hide();
        }
        else if(mDeviceTree->isSlave(var.toInt())){
            int slave_count = m_master->Master_getSlaveCount();
            if(slave_count>0){
                m_tableView_slaveMSG->clearContents();
                m_tableView_slaveItemMSG->clearContents();
                int slave_index = var.toInt() - MARK_SLAVE-1;
                int slaveItem_count = 0;
                Ethercat_Slave slave = m_master->slaves_list.at(slave_index);
                m_tableView_slaveMSG->append_RawData(slave_index,slave.dump_data());

                foreach (Ethercat_SlaveMSG_Item input, slave.input_list) {
                     m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,input.dump_data());
                }
                foreach (Ethercat_SlaveMSG_Item output, slave.output_list) {
                    m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,output.dump_data());
                }
            }

            mTabWedget_center->hide();
            m_widget_slaveMSG->show();
        }
    }
}

///
/// \brief 设备树Item展开
/// \param index
///
void MainFormView::mDeviceTree_expanded(const QModelIndex index){
    Q_UNUSED(index);
    //展开的时候进行内容自适应
    mDeviceTree->resizeColumnToContents(0);//ScrollBar是针对于整个column而言，而不是针对content而言
}


/************  Slots end ***************/
