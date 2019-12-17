#include "Component_DockLeft.h"

#include <QGridLayout>
#include <QDebug>
#include <QVBoxLayout>
#include <QDomComment>
#include <QFile>
#include <QMessageBox>
#include <QDomComment>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QFileDialog>

///
/// \brief 构造函数
/// \param parent
///
Component_DockLeft::Component_DockLeft(QObject *parent):QObject(parent)
{
//  qDebug() << "Component_DockLeft" << this->m_appName;
    this->set_AppName("Component_DockLeft");

    m_DeviceTree = nullptr;
    m_widgetObjectController = nullptr;
    m_tableView_slaveMSG = nullptr;
    m_tableView_slaveItemMSG = nullptr;
    m_userPropertyWidget = nullptr;

    m_projectMenu = nullptr;
    m_itemMenu = nullptr;

    m_DockWidget = nullptr;
    m_tabWidget = nullptr;
    m_tabItem_DeviceTree = nullptr;
    m_tabItem_virtualDevice = nullptr;

    m_ListWidget_virtualDevice_xml = nullptr;
    m_ListWidget_virtualDevice_select = nullptr;
}

///
/// \brief 析构函数
///
Component_DockLeft::~Component_DockLeft()
{
    if(this->get_componentWidgetPtr()){
        delete this->get_componentWidgetPtr();
        this->set_componentWidgetPtr(nullptr);
    }
}

///
/// \brief 初始化
/// \return
///
bool Component_DockLeft::Init_Cores()
{  
    QGridLayout *userGridLayout= new QGridLayout();
    this->get_componentWidgetPtr()->setLayout(userGridLayout);

    //初始化Tabwidget
    this->m_tabWidget  = new QTabWidget(this->get_componentWidgetPtr());
    this->m_tabWidget->setTabPosition(QTabWidget::South);
    this->get_componentWidgetPtr()->layout()->addWidget(this->m_tabWidget);

    //初始化设备树tab区域
    this->m_tabItem_DeviceTree = new QScrollArea(this->m_tabWidget);
    userGridLayout= new QGridLayout();
    this->m_tabItem_DeviceTree->setLayout(userGridLayout);
    this->m_tabItem_DeviceTree->layout()->setMargin(0);
    this->m_tabWidget->addTab(this->m_tabItem_DeviceTree,"DeviceTree");
    //初始化虚拟设备tab区域
    this->m_tabItem_virtualDevice = new QScrollArea(this->m_tabWidget);
    userGridLayout= new QGridLayout();
    this->m_tabItem_virtualDevice->setLayout(userGridLayout);
    this->m_tabItem_virtualDevice->layout()->setMargin(0);
    this->m_tabWidget->addTab(this->m_tabItem_virtualDevice,"VirtualDevice");

    /* 设备树tab区域的控件 */
    //设备树初始化
    m_DeviceTree = new My_TreeView_Device(this->m_tabItem_DeviceTree);
    this->m_tabItem_DeviceTree->layout()->addWidget(m_DeviceTree);
    //单个控件的属性框初始化
    m_widgetObjectController = new ObjectController(this->m_tabItem_DeviceTree);
    this->m_tabItem_DeviceTree->layout()->addWidget(m_widgetObjectController);

    //多个控件的属性框初始化
    m_userPropertyWidget = new QScrollArea(this->m_tabItem_DeviceTree);
    this->m_tabItem_DeviceTree->layout()->addWidget(m_userPropertyWidget);
    m_userPropertyWidget->hide();//暂时不显示
    userGridLayout= new QGridLayout();
    m_userPropertyWidget->setLayout(userGridLayout);
    m_userPropertyWidget->layout()->setMargin(0);

    this->Init_DeviceTreeItem_menu();//初始化右键菜单

    //初始化slave信息窗口的内容
    m_tableView_slaveMSG = new My_Table_SlaveMSG(this->ComponentP_get_ComponentWidgetsPtr()->m_widget_slaveMSG);
    m_tableView_slaveItemMSG = new My_Table_SlaveItemMSG(this->ComponentP_get_ComponentWidgetsPtr()->m_widget_slaveMSG);
    dynamic_cast<QVBoxLayout*>(this->ComponentP_get_ComponentWidgetsPtr()->m_widget_slaveMSG->layout())->addWidget(m_tableView_slaveMSG,1);
    dynamic_cast<QVBoxLayout*>(this->ComponentP_get_ComponentWidgetsPtr()->m_widget_slaveMSG->layout())->addWidget(m_tableView_slaveItemMSG,3);

    m_tableView_slaveMSG->init();
    m_tableView_slaveItemMSG->init();

    //向主窗口传递指针
    this->ComponentP_get_ComponentWidgetsPtr()->m_userPropertyWidget = m_userPropertyWidget;
    this->ComponentP_get_ComponentWidgetsPtr()->m_controllerPropertyWidget = m_widgetObjectController;

    /* 虚拟设备的tab区域 */
    QGroupBox *groupBox_virtualDevice = new QGroupBox(this->m_tabItem_virtualDevice);
    groupBox_virtualDevice->setTitle("虚拟设备列表");
    userGridLayout = new QGridLayout();
    groupBox_virtualDevice->setLayout(userGridLayout);
    this->m_tabItem_virtualDevice->layout()->addWidget(groupBox_virtualDevice);
    QGroupBox *groupBox_virtualDevice_select = new QGroupBox(this->m_tabItem_virtualDevice);
    groupBox_virtualDevice_select->setTitle("加载的虚拟设备");
    userGridLayout = new QGridLayout();
    groupBox_virtualDevice_select->setLayout(userGridLayout);
    this->m_tabItem_virtualDevice->layout()->addWidget(groupBox_virtualDevice_select);

    this->m_ListWidget_virtualDevice_xml = new QListWidget(this->m_tabItem_virtualDevice);
    groupBox_virtualDevice->layout()->addWidget(this->m_ListWidget_virtualDevice_xml);
    this->m_ListWidget_virtualDevice_select = new QListWidget(this->m_tabItem_virtualDevice);
    groupBox_virtualDevice_select->layout()->addWidget(this->m_ListWidget_virtualDevice_select);

    this->Init_Slots();

    int xml_count = this->VirtualDevice_LoadXMLs(QString("../VirtualDevice_XML"));//相对于App的路径
    if(xml_count > 0){
        this->setBottom_message("Find VirtualDevice "+ QString::number(xml_count));
    }

   return true;
}

///
/// \brief 销毁
/// \return
///
bool Component_DockLeft::Destroy_Cores()
{

    return true;
}

///
/// \brief 初始化对象
/// \return
///
bool Component_DockLeft::Init_Object()
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
/// \brief 初始化信号槽
/// \return
///
bool Component_DockLeft::Init_Slots()
{
    this->Init_DeviceTree_slots();

    connect(this->m_ListWidget_virtualDevice_select,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(virtualDeviceSelect_itemDoubleClicked(QListWidgetItem*)));
    connect(this->m_ListWidget_virtualDevice_xml,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(virtualDeviceXML_itemDoubleClicked(QListWidgetItem*)));

    return true;
}

///
/// \brief 组件的主站扫描开始前的回调
///
void Component_DockLeft::ComponentP_master_ScanStart()
{
    this->m_tabItem_DeviceTree->layout()->removeWidget(m_DeviceTree);
    this->m_tabItem_DeviceTree->layout()->removeWidget(m_widgetObjectController);//移除后重新添加，保证顺序
    this->m_tabItem_DeviceTree->layout()->removeWidget(m_userPropertyWidget);

    delete m_DeviceTree;

    m_DeviceTree = new My_TreeView_Device(this->m_tabItem_DeviceTree);
    this->m_tabItem_DeviceTree->layout()->addWidget(m_DeviceTree);
    this->m_tabItem_DeviceTree->layout()->addWidget(m_widgetObjectController);//移除后重新添加，保证顺序
    this->m_tabItem_DeviceTree->layout()->addWidget(m_userPropertyWidget);

    this->get_DeviceTreePtr()->Add_LeftTree_Master();
    this->Init_DeviceTree_slots();//重新连接信号槽
}

///
/// \brief 组件的主站扫结束后的回调
///
void Component_DockLeft::ComponentP_master_ScanFinish()
{
    int count_virtualDeviceReg = this->VirtualDevice_RegistMaster();//将虚拟设备加载到主站系统中
    if(count_virtualDeviceReg > 0){
        this->setBottom_message("Regist VirtualDevice num = "+ QString::number(count_virtualDeviceReg));
    }

    QList<Master_Address_t> addr_list;
    Master_Address_t addr = {"",-1,-1,-1,-1};//默认赋一个初值
    addr_list.clear();

    int master_index = 1;//暂时只考虑1个主站的情况
    int slaveInput_count = 0;
    int slaveOutput_count = 0;
    QMap<int,int> slaveInput_map;
    QMap<int,int> slaveOutput_map;
    int temp_ObjIndex;

    //查找是否存在多个输入输出group
    foreach (Ethercat_Slave slave, *(this->ComponentP_get_MasterPtr()->get_SlaveListPtr())) {
        //TODO:这里可能存在查找的性能问题，后面再去优化
        foreach (Ethercat_SlaveMSG_Item input, slave.input_list) {
            temp_ObjIndex = input.m_obj_index>>12;//获取6010中的6
            if(slaveInput_map[temp_ObjIndex]==0){
                slaveInput_map[temp_ObjIndex] = ++slaveInput_count;
            }
        }
        foreach (Ethercat_SlaveMSG_Item output, slave.output_list) {
            temp_ObjIndex = output.m_obj_index>>12;
            if(slaveOutput_map[temp_ObjIndex]==0){
                slaveOutput_map[temp_ObjIndex] = ++slaveOutput_count;
            }
        }

        this->get_DeviceTreePtr()->Add_LeftTree_Slaves(master_index,slave.m_name,slaveInput_count,slaveOutput_count);
        addr.inputs_offset = -1;
        addr.outputs_offset = -1;

        addr.slave_name = slave.m_name;
        addr.inputs_Bits = slave.m_Ibits;
        addr.outputs_Bits = slave.m_Obits;

        if(slave.input_list.size() > 0){//传递input和output地址
            addr.inputs_offset = slave.input_list.at(0).m_addr_offset;
        }
        if(slave.output_list.size() > 0){//传递input和output地址
            addr.outputs_offset = slave.output_list.at(0).m_addr_offset;
        }
        addr_list.append(addr);


        foreach (Ethercat_SlaveMSG_Item input, slave.input_list) {
            temp_ObjIndex = input.m_obj_index>>12;
            this->get_DeviceTreePtr()->Add_LeftTree_SlaveMessage(master_index,slave.m_slave_index,
                                                                 My_TreeView_Device::MARK_SLAVE_ITEM_INPUT+(slaveInput_map[temp_ObjIndex]-1) - My_TreeView_Device::MARK_SLAVE_ITEM,
                                                                 input.m_name);

//                qDebug() << QString::number(input.m_obj_index,16) << ","<<QString::number((input.m_obj_index>>12),16);
        }
        foreach (Ethercat_SlaveMSG_Item output, slave.output_list) {
            temp_ObjIndex = output.m_obj_index>>12;
            this->get_DeviceTreePtr()->Add_LeftTree_SlaveMessage(master_index,slave.m_slave_index,
                                                                 My_TreeView_Device::MARK_SLAVE_ITEM_OUTPUT+(slaveOutput_map[temp_ObjIndex]-1) - My_TreeView_Device::MARK_SLAVE_ITEM,
                                                                 output.m_name);
        }

    }

    this->get_userAppPtr()->get_CallbackPtr()->Master_setAddressList(addr_list);//设置addr_list到插件
    this->get_userAppPtr()->get_CallbackPtr()->Master_setVirtualSlaveCount(count_virtualDeviceReg);//设置虚拟从站的个数
}

///
/// \brief 设备树的地址
/// \return
///
My_TreeView_Device *Component_DockLeft::get_DeviceTreePtr()
{
    return m_DeviceTree;
}

///
/// \brief 设置设备树的地址
/// \param deviceTree
///
void Component_DockLeft::set_DeviceTreePtr(My_TreeView_Device *deviceTree)
{
    m_DeviceTree = deviceTree;
}

///
/// \brief 初始化设备树的菜单栏
///
void Component_DockLeft::Init_DeviceTreeItem_menu()
{
    //创建menu
    m_projectMenu = new QMenu(this->get_componentWidgetPtr());
    m_itemMenu = new QMenu(this->get_componentWidgetPtr());

    //Device mexu
    QAction* ac = nullptr;
    ac = new QAction(tr("Scan"),this->get_componentWidgetPtr());
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

///
/// \brief 从制定路径中加载XML并加入到虚拟设备列表中
/// \param filePath (相对路径即可)
/// \return
///
int Component_DockLeft::VirtualDevice_LoadXMLs(QString filePath)
{
    int xml_count = 0;

    VirtualDevice_item * item = nullptr;

    QDir dir(this->get_ApppPath());
    dir.cd(filePath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList file_list = dir.entryInfoList();
    foreach (QFileInfo fileInfo, file_list) {
//        qDebug() << fileInfo.fileName();
        if(fileInfo.suffix().compare("xml")){
            continue;
        }

        item = this->VirtualDevice_ParseFileXML(fileInfo.absoluteFilePath());

        if(item != nullptr){
            ++xml_count;
            item->set_fileName(fileInfo.fileName());
            this->m_ListWidget_virtualDevice_xml->addItem(item);
            this->setBottom_message("Load Virtual_Device \""+
                                                                   item->text() + "("+
                                                                   item->get_fileName()+")\"");
        }
    }
      return xml_count;
}

///
/// \brief 解析XML文件对象
/// \param fileName
/// \return
///
VirtualDevice_item *Component_DockLeft::VirtualDevice_ParseFileXML(QString fileName)
{
    VirtualDevice_item *ret_item = nullptr;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        this->setBottom_message("open file "+fileName+" failed, error:"+file.errorString());
        return ret_item;
    }

    /*解析Dom节点*/
    QDomDocument    document;
    QString         strError;
    int             errLin = 0, errCol = 0;
    if( !document.setContent(&file, false, &strError, &errLin, &errCol) ) {
        this->setBottom_message("parse file failed at line "+
                                                               QString::number(errLin) + ",column" +
                                                               QString::number(errCol)+","+strError);
       return ret_item;
    }

    if( document.isNull() ) {
        this->setBottom_message("document is null !");
       return ret_item;
    }

    QString str_tmp;
    bool isInput = false;
    QDomElement root = document.documentElement();
    if(root.tagName().compare("Virtual_Device")){//如果发现不是虚拟设备
        qDebug() << "No Virtual_Device";
        return ret_item;
    }

    Ethercat_Slave virtual_slave;
    ret_item = new VirtualDevice_item();

    virtual_slave.m_name = root.attribute("name");

    QDomNodeList list_io = root.childNodes();
    int count_io = list_io.count();//判断是INPUT还是OUTPUT
    //       qDebug() << "count_io" << count_io;
    for(int k = 0;k<count_io;k++){
       isInput = false;
       QDomElement Devices = list_io.item(k).toElement();
       str_tmp = Devices.tagName();
       if(str_tmp.compare("INPUT") == 0){
           isInput = true;
       }
       else if(str_tmp.compare("OUTPUT") == 0){
           isInput = false;
       }
       else if(str_tmp.compare("Message") == 0){
           virtual_slave.m_eep_id = Devices.attribute("ProductId").toInt();
           virtual_slave.m_eep_man = Devices.attribute("ProductCode").toInt();
           virtual_slave.m_eep_rev = Devices.attribute("RevisionNo").toInt();
           continue;
       }

       QDomNodeList list = Devices.childNodes();
       int count = list.count();
       for(int i=0;i<count;i++){
           QDomNode dom_node = list.item(i);
           QDomElement element = dom_node.toElement();

           Ethercat_SlaveMSG_Item item;
           item.m_addr_offset = 0;

           QDomNodeList child_list = element.childNodes();
           int child_count = child_list.count();
    //               qDebug() << child_count;
           for(int j=0; j<child_count;j++)
           {
               QDomNode child_dom_node = child_list.item(j);
               QDomElement child_element = child_dom_node.toElement();
               QString child_tag_name = child_element.tagName();
               if(child_tag_name.compare("Index") == 0){
                   item.m_obj_index = child_element.text().toInt();
               }
               else if(child_tag_name.compare("Name") == 0){
                   item.m_name = child_element.text();
               }
               else if(child_tag_name.compare("Type") == 0){
                   item.m_date_type = this->VirtualDevice_ParseTypeXML(child_element.text());
               }
               else if(child_tag_name.compare("BitSize") == 0){
                   item.m_item_bitLen = child_element.text().toInt();
               }
           }

           if(isInput){
               virtual_slave.input_list.append(item);
           }
           else{
               virtual_slave.output_list.append(item);
           }
       }
    }

    virtual_slave.m_Ibits = 0;
    foreach (Ethercat_SlaveMSG_Item item, virtual_slave.input_list) {
       virtual_slave.m_Ibits += item.m_item_bitLen;
    }

    virtual_slave.m_Obits = 0;
    foreach (Ethercat_SlaveMSG_Item item, virtual_slave.output_list) {
       virtual_slave.m_Obits += item.m_item_bitLen;
    }

    virtual_slave.m_slave_index = -1;

    file.close();

    ret_item->set_virtualSlave(virtual_slave);
    ret_item->setText(virtual_slave.m_name);

   //测试
//    qDebug() << virtual_slave.m_eep_id << virtual_slave.m_eep_man << virtual_slave.m_eep_rev;
//    qDebug() << virtual_slave.m_Ibits << virtual_slave.m_Obits << virtual_slave.m_slave_index;
//    qDebug() << "Input";
//    foreach (Ethercat_SlaveMSG_Item item, virtual_slave.input_list) {
//      qDebug() << item.m_obj_index << item.m_name << item.m_date_type << item.m_item_bitLen;
//    }
//    qDebug() << "Output";
//    foreach (Ethercat_SlaveMSG_Item item, virtual_slave.output_list) {
//      qDebug() << item.m_obj_index << item.m_name << item.m_date_type << item.m_item_bitLen;
//    }

    return ret_item;
}

///
/// \brief 输出信息
/// \param str
///
void Component_DockLeft::setBottom_message(QString str)
{
    this->ComponentP_get_MessageTextPtr()->appendPlainText("[ "+this->get_AppName()+" ]\t"+str);
}

///
/// \brief 将虚拟设备注册到主站系统中
/// \return 返回注册了几个虚拟设备
///
int Component_DockLeft::VirtualDevice_RegistMaster()
{
    int tmp_Ibits = 0;
    int tmp_Obits = 0;
    int tmp_InputOffset = 0;
    int tmp_OutputOffset = 0;
    int slaveIndex_VirtualDevice = 1;

    int slaveCount_EtherCAT = this->ComponentP_get_MasterPtr()->Master_getSlaveCount();

    if(slaveCount_EtherCAT == 0){//没有实从站,就返回
        return 0;
    }


    QList<Ethercat_Slave>* slaveListPtr = this->ComponentP_get_MasterPtr()->get_SlaveListPtr();

    int slaveSize_EtherCAT = slaveListPtr->at(slaveCount_EtherCAT-1).input_list.at(0).m_addr_offset +
                                  (slaveListPtr->at(slaveCount_EtherCAT-1).m_Ibits/2)/4;

    //遍历计算输入和输出的总偏移
    foreach (VirtualDevice_item *item_virtual, this->m_virutalDevice_selectList) {
        tmp_Ibits += item_virtual->get_virtualSlave().m_Ibits;
        tmp_Obits += item_virtual->get_virtualSlave().m_Obits;
    }

    tmp_Ibits = tmp_Ibits/8;
    tmp_Obits = tmp_Obits/8;
    tmp_InputOffset = slaveSize_EtherCAT;
    tmp_OutputOffset = slaveSize_EtherCAT;


    foreach (VirtualDevice_item *item_virtual, this->m_virutalDevice_selectList) {
        Ethercat_Slave virtual_slave = item_virtual->get_virtualSlave();
        QList<Ethercat_SlaveMSG_Item> input_list;//主站的input相当于从站的output
        QList<Ethercat_SlaveMSG_Item> output_list;//主站的output相当于从站的input
        //output的第一个偏移量为0
        foreach (Ethercat_SlaveMSG_Item output_item, virtual_slave.output_list) {
            Ethercat_SlaveMSG_Item item = output_item;
            item.m_addr_offset = tmp_OutputOffset;
            item.m_obj_index = 0x7010 + item.m_obj_index;//为了与EtherCAT格式相同
            item.m_slave_num = slaveIndex_VirtualDevice + slaveCount_EtherCAT;
            item.m_sm_num = 2;
            item.m_obj_index_sub = 0;
            item.m_addr_bit = 0;
            tmp_OutputOffset += output_item.m_item_bitLen/8;//暂时不考虑bool变量
            output_list.append(item);
        }

        foreach (Ethercat_SlaveMSG_Item input_item, virtual_slave.input_list) {
            Ethercat_SlaveMSG_Item item = input_item;
            item.m_addr_offset = tmp_InputOffset + tmp_Obits;//再加上一个input的偏移
            item.m_obj_index = 0x6010 + item.m_obj_index;//为了与EtherCAT格式相同
            item.m_slave_num = slaveIndex_VirtualDevice + slaveCount_EtherCAT;
            item.m_sm_num = 3;
            item.m_obj_index_sub = 0;
            item.m_addr_bit = 0;
            tmp_InputOffset += input_item.m_item_bitLen/8;//暂时不考虑bool变量
            input_list.append(item);
        }

        virtual_slave.output_list = output_list;
        virtual_slave.input_list = input_list;
        virtual_slave.m_slave_index = slaveIndex_VirtualDevice + slaveCount_EtherCAT;
        item_virtual->set_virtualSlave(virtual_slave);

        slaveIndex_VirtualDevice ++;

        this->ComponentP_get_MasterPtr()->get_SlaveListPtr()->append(virtual_slave);//注册到主站中
    }

//    //测试
//    foreach (VirtualDevice_item *item_virtual, this->m_virutalDevice_selectList) {
//        Ethercat_Slave virtual_slave = item_virtual->get_virtualSlave();
//        qDebug() << virtual_slave.m_name;
//        qDebug() << "OUTPUT";
//        foreach (Ethercat_SlaveMSG_Item output_item, virtual_slave.output_list) {
//            qDebug() << output_item.m_name <<  output_item.m_addr_offset;
//        }
//        qDebug() << "INPUT";
//        foreach (Ethercat_SlaveMSG_Item intput_item, virtual_slave.input_list) {
//            qDebug() << intput_item.m_name << intput_item.m_addr_offset;
//        }
//    }

    return (slaveIndex_VirtualDevice-1);
}

///
/// \brief 初始化设备树的信号槽
///
void Component_DockLeft::Init_DeviceTree_slots()
{
    //连接信号槽
    connect(m_DeviceTree,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(mDeviceTree_customContextMenuRequested(const QPoint &)));
    connect(m_DeviceTree,SIGNAL(doubleClicked ( const QModelIndex)),this,SLOT(mDeviceTree_DoubleClicked(const QModelIndex)));
    connect(m_DeviceTree,SIGNAL(expanded(const QModelIndex)),this,SLOT(mDeviceTree_expanded(const QModelIndex)));
}

///
/// \brief 解析XML文件中的数据类型
/// \param str_type
/// \return
///
int Component_DockLeft::VirtualDevice_ParseTypeXML(QString str_type)
{
    if(str_type.compare("UINT8") == 0){
        return DRE_ECT_UNSIGNED8;
    }
    else if(str_type.compare("UINT16") == 0){
        return DRE_ECT_UNSIGNED16;
    }
    else if(str_type.compare("UINT32") == 0){
        return DRE_ECT_UNSIGNED32;
    }
    else if(str_type.compare("INT8") == 0){
        return DRE_ECT_INTEGER8;
    }
    else if(str_type.compare("INT16") == 0){
        return DRE_ECT_INTEGER16;
    }
    else if(str_type.compare("INT32") == 0){
        return DRE_ECT_INTEGER32;
    }

   return DRE_ECT_UNSIGNED8;
}

/************  Slots *******************/
///
/// \brief Tree自定义菜单消息
/// \param pos
///
void Component_DockLeft::mDeviceTree_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = m_DeviceTree->indexAt(pos);
    QVariant var = index.data(My_TreeView_Device::ROLE_MARK);
    if(var.isValid())
    {
        if(My_TreeView_Device::MARK_DEVICE == var.toInt())
            m_projectMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
//        else if(MARK_SLAVE_MESSAGE == var.toInt())
        else if(m_DeviceTree->isSlaveMessage(var.toInt()))
            m_itemMenu->exec(QCursor::pos());
    }
}

///
/// \brief Tree双击响应
/// \param index
///
void Component_DockLeft::mDeviceTree_DoubleClicked(const QModelIndex index)
{
    QVariant var = index.data(My_TreeView_Device::ROLE_MARK);
    if(var.isValid())
    {
        if(m_DeviceTree->isMaster(var.toInt())){//project才会弹出新窗口
            this->ComponentP_get_ComponentWidgetsPtr()->m_tabWedget_center->show();
            this->ComponentP_get_ComponentWidgetsPtr()->m_widget_slaveMSG->hide();
        }
        else if(m_DeviceTree->isSlave(var.toInt())){

            int slave_count = this->ComponentP_get_MasterPtr()->Master_getSlaveCount();
            if(slave_count>0){
                m_tableView_slaveMSG->clearContents();
                m_tableView_slaveItemMSG->clearContents();
                int slave_index = var.toInt() - My_TreeView_Device::MARK_SLAVE-1;
                int slaveItem_count = 0;
                Ethercat_Slave slave = this->ComponentP_get_MasterPtr()->get_SlaveListPtr()->at(slave_index);
               // m_tableView_slaveMSG->append_RawData(slave_index,slave.dump_data());
                m_tableView_slaveMSG->append_RawData(0,slave.dump_data());

                foreach (Ethercat_SlaveMSG_Item input, slave.input_list) {
                     m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,input.dump_data());
                }

                foreach (Ethercat_SlaveMSG_Item output, slave.output_list) {
                    m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,output.dump_data());
                }
                m_tableView_slaveItemMSG->setRowCount(slaveItem_count);
            }

            this->ComponentP_get_ComponentWidgetsPtr()->m_tabWedget_center->hide();
            this->ComponentP_get_ComponentWidgetsPtr()->m_widget_slaveMSG->show();
        }
    }

}

///
/// \brief 设备树Item展开
/// \param index
///
void Component_DockLeft::mDeviceTree_expanded(const QModelIndex index){
    Q_UNUSED(index);
    //展开的时候进行内容自适应
    m_DeviceTree->resizeColumnToContents(0);//ScrollBar是针对于整个column而言，而不是针对content而言
}

///
/// \brief 根据XML文件添加虚拟设备
/// \param item
///
void Component_DockLeft::virtualDeviceXML_itemDoubleClicked(QListWidgetItem *item)
{
    VirtualDevice_item *item_select = new VirtualDevice_item();
    *item_select = *(dynamic_cast<VirtualDevice_item *>(item));//复制拷贝

    this->m_ListWidget_virtualDevice_select->addItem(item_select);//添加文件
    this->m_virutalDevice_selectList.append(item_select);
}

///
/// \brief 删除添加的虚拟设备
/// \param item
///
void Component_DockLeft::virtualDeviceSelect_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);

    int row = this->m_ListWidget_virtualDevice_select->currentRow();
    QListWidgetItem *item_select = this->m_ListWidget_virtualDevice_select->takeItem(row);//takeItem才能删除
    this->m_virutalDevice_selectList.removeAt(row);
    delete item_select;
}


/************  Slots end ***************/

/************下面是测试用的slot************/

///
/// \brief 右键开始扫描
///
void Component_DockLeft::mDeviceTree_EcScan_clicked(){
   // Master_scan();
    this->ComponentP_get_ComponentWidgetsPtr()->Set_MasterState(ComponentWidgets::Cmd_Master_scan);
}

///
/// \brief 右键添加Master
///
void Component_DockLeft::AddTree_Master_clicked(){
    m_DeviceTree->Add_LeftTree_Master();
}

///
/// \brief 右键添加Slave
///
void Component_DockLeft::AddTree_Slave_clicked(){
    m_DeviceTree->Add_LeftTree_Slaves();
}

///
/// \brief 右键添加Slave_item
///
void Component_DockLeft::AddTree_SlaveItem_clicked(){
    m_DeviceTree->Add_LeftTree_SlaveItems();
}

///
/// \brief 右键添加input
///
void Component_DockLeft::AddTree_SlaveMessageInput_clicked(){
    m_DeviceTree->Add_LeftTree_SlaveMessage(1,1,My_TreeView_Device::MARK_SLAVE_ITEM_INPUT-My_TreeView_Device::MARK_SLAVE_ITEM);
}

///
/// \brief 右键添加output
///
void Component_DockLeft::AddTree_SlaveMessageOutput_clicked(){
    m_DeviceTree->Add_LeftTree_SlaveMessage(1,1,My_TreeView_Device::MARK_SLAVE_ITEM_OUTPUT-My_TreeView_Device::MARK_SLAVE_ITEM);
}

/****************************************/


