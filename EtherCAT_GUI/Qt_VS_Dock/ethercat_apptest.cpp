#include "mainformview.h"

#include "RTTimer.h"

typedef struct{
   int Timer_id;
   bool isRun;
//   int period_us;
}RTtimer_User_S;

RTtimer_User_S RTtimer_User = {
    1,false
};

DWORD_PTR user;

void CALLBACK TimerCallBack()//回调函数
{
//    if(ret == Error_TimeOut){
//        MY_DEBUG_PRINT("OverTime!\r\n");
//    }
    MainFormView *t = reinterpret_cast<MainFormView*>(user);//强制转换

    static volatile int wkc;
    int expectedWKC = t->get_MasterPtr()->Master_get_expectedWKC();
     //t->m_timer_advanced->is_enableCallBack = false;//防止多次并发
     int state = t->get_MasterPtr()->Master_readState(0);
     //qDebug() << user_form_generalTab->master->Master_stateToString(state);
     if(state == DRE_Master::STATE_OPERATIONAL){
         if(wkc >= expectedWKC){//第一次运行会出现这个问题
             t->plugin_userApps->get_CallbackPtr()->Master_AppLoop_callback();
//              t->m_motorApp_callback.Master_AppLoop_callback();
         }
         else{
             qWarning() << "wkc<expectedWKC";
         }
     }
     else{
//         qDebug() << My_EthercatMaster::Master_stateToString(state);
//         t->general_xx->Master_UI_Loop(state);
         t->get_MasterPtr()->Master_writeState(0,DRE_Master::STATE_OPERATIONAL);
         t->get_MasterPtr()->Master_CheckState(0,DRE_Master::STATE_OPERATIONAL,1000);
     }
     //NOTE:这里的UI,会不会影响实时性，待测试
     t->general_xx->Master_UI_Loop(state);
     //user_form_generalTab->master->Master_process_loop(My_EthercatMaster::STATE_OPERATIONAL,1000);//设置5ms的检查延时
     t->get_MasterPtr()->Master_processData_send();
     wkc= t->get_MasterPtr()->Master_processData_receive(5000);//短了wkc<expectedWKC
     //t->m_timer_advanced->is_enableCallBack = true;

//    static int i;
//    qDebug() << i++;
}

/********************* Slots  *******************************/

/********************* Slots end  *******************************/

/******************** Ethercat API(提供给mainwindow的槽使用) ******************************/
///
/// \brief MainFormView::Master_scan
///
int MainFormView::Master_scan(){
    if(plugin_userApps == nullptr){
        QMessageBox::critical(this,tr("Information"),tr("No Plugin is loaded!"));
        return Master_Err_InvalidPlugin;
    }
    else{
        if(plugin_userApps->get_CallbackPtr() == nullptr){
            QMessageBox::critical(this,tr("Information"),tr("No Callback in the Plugin"));
            return Master_Err_InvalidCallback;
        }
    }

    if(m_master->get_CurrentAdapter(DRE_Master::Adapter_name).isEmpty()){
        mTabWedget_center->show();
        m_widget_slaveMSG->hide();
        mTabWedget_center->setCurrentIndex(Tab_General);
        QMessageBox::critical(this,tr("Information"),tr("No Adapter is found!"));
        return Master_Err_InvalidAdapter;
    }

    int state = 0;
    delete mDeviceTree;
    Init_FrameLeft_Content();//重新初始化设备树

    mDeviceTree->Add_LeftTree_Master();

    ProcessBar_startEvent();

    MMTimer_RT::Delay_MSec(300);//需要无阻塞的延时 500ms

    state = m_master->Master_scan();

    if(state == DRE_Master::ERROR_NO_SLAVE){
        ProcessBar_stopEvent();//不出现processBar
        QMessageBox::information(this,tr("Information"),tr("No Slave Found!"));
        return Master_Err_NoSlave;
    }
    else if(state == DRE_Master::ERROR_NONE){
//        m_tableView_slaveMSG->clearContents();
//        m_tableView_slaveItemMSG->clearContents();
//        int slave_count = 0;
//        int slaveItem_count = 0;
//        mDeviceTree->Add_LeftTree_Master();
//        qDebug() << m_master->Master_getSlaveCount();
        plugin_userApps->get_CallbackPtr()->Master_setSlaveCount(m_master->Master_getSlaveCount());
        QList<Master_Address_t> addr_list;
        Master_Address_t addr = {-1,-1};//默认赋一个初值
        addr_list.clear();

        int master_index = 1;//暂时只考虑1个主站的情况

        foreach (Ethercat_Slave slave, *(m_master->get_SlaveListPtr())) {
            //qDebug() << slave.dump_data(true);
            //TextList_append(m_bottomText,slave.dump_data(true));
            //m_tableView_slaveMSG->append_RawData(slave_count++,slave.dump_data());
            mDeviceTree->Add_LeftTree_Slaves(master_index,slave.m_name);
            addr.inputs_offset = -1;
            addr.outputs_offset = -1;

            if(slave.input_list.size() > 0){//传递input和output地址
                addr.inputs_offset = slave.input_list.at(0).m_addr_offset;
            }
            if(slave.output_list.size() > 0){//传递input和output地址
                addr.outputs_offset = slave.output_list.at(0).m_addr_offset;
            }
            addr_list.append(addr);


            foreach (Ethercat_SlaveMSG_Item input, slave.input_list) {
                //qDebug() << input.dump_data(true);
//                TextList_append(m_bottomText,input.dump_data(true));
                 //m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,input.dump_data());
                mDeviceTree->Add_LeftTree_SlaveMessage(master_index,slave.m_slave_index,MARK_SLAVE_ITEM_INPUT - MARK_SLAVE_ITEM,input.m_name);
            }
            foreach (Ethercat_SlaveMSG_Item output, slave.output_list) {
                //qDebug() << output.dump_data(true);
//                TextList_append(m_bottomText,output.dump_data(true));
                //m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,output.dump_data());
                mDeviceTree->Add_LeftTree_SlaveMessage(master_index,slave.m_slave_index,MARK_SLAVE_ITEM_OUTPUT - MARK_SLAVE_ITEM,output.m_name);
            }

        }

        plugin_userApps->get_CallbackPtr()->Master_setAddressList(addr_list);//设置addr_list到插件

        //设置EtherCAT状态显示
        general_xx->Master_UI_Scan();
    }

    m_pluginList->setEnabled(false);//EtherCAT销毁后才能选择加载插件

    return Master_Err_NONE;
}

///
/// \brief MainFormView::Master_run
///
int MainFormView::Master_run(){

    if(m_master->Master_getSlaveCount()>0){
        //设置10ms，有时候还会超时，并非严格的10ms周期
//        m_timer_advanced->setInterval(10);//10ms
//        m_timer_advanced->start();
//        TimeRun(Timer_id, 10000, TimerCallBack); //开启周期数据通信
//        user = (DWORD_PTR)this;

//        m_motorApp_callback.Gcode_setAddress(m_GcodeSegment_Q);
//        if(m_GcodeSegment_Q->empty()){
//            qDebug() << "Master_run Q empty!";
//        }
//        else{
//            qDebug() << "Master_run Q No empty!";
//        }
        m_status_label->setText(tr("Run Mode"));

        m_master->Master_run();
        plugin_userApps->get_CallbackPtr()->Master_setAdressBase(m_master->Master_getAddressBase());
        plugin_userApps->get_CallbackPtr()->Master_AppStart_callback();

        general_xx->Master_UI_RUn();


        //最好放在后面，否则会崩溃
        if(!RTtimer_User.isRun){
            TimeRun(RTtimer_User.Timer_id, m_master->get_PLC_Period()*1000, TimerCallBack); //开启周期数据通信
            user = (DWORD_PTR)this;
            RTtimer_User.isRun = true;
        }

    }
    else{
        QMessageBox::warning(this,tr("Ethercat warning"),tr("No Slaves!Please scan slaves first"));
        return Master_Err_NoSlave;
    }

//    this->setFocus();//如果用键盘测试的话，需要设置焦点
    m_pluginList->setEnabled(false);//EtherCAT销毁后才能选择加载插件
    return Master_Err_NONE;
}

///
/// \brief MainFormView::Master_stop
///
int MainFormView::Master_stop(){

    if(m_master->Master_getSlaveCount()>0){
        //m_timer_advanced->stop();
        if(RTtimer_User.isRun){
           TestKillTimer(RTtimer_User.Timer_id);
           RTtimer_User.isRun = false;
        }

        m_master->Master_stop();
        plugin_userApps->get_CallbackPtr()->Master_AppStop_callback();
        m_status_label->setText(tr("Ready"));

        //设置状态
        general_xx->Master_UI_Stop();
    }
    else{
         QMessageBox::warning(this,tr("Ethercat warning"),tr("No Slaves!Please scan slaves first"));
         return Master_Err_NoSlave;
    }

    m_pluginList->setEnabled(false);//EtherCAT销毁后才能选择加载插件

    return Master_Err_NONE;
}

int MainFormView::Master_setting(){
//  this->setFocus();//如果用键盘测试的话，需要设置焦点

   return 0;
}

int MainFormView::Master_exit(){
  if(m_master->Master_getSlaveCount()>0){
    Master_stop();
    m_master->Master_close(true);
  }

  m_pluginList->setEnabled(true);//EtherCAT销毁后才能选择加载插件

  return Master_Err_NONE;
}

/******************** Ethercat API end ******************************/

void MainFormView::EthercatApp_init(){
//    #ifdef _MSC_VER //windows
//    m_timer_advanced = new MMTimer_RTwin(); //时间较短时会柱塞UI
//    #endif
//    //    m_timer_advanced = new QTimer();
//    //    m_timer_advanced->setTimerType(Qt::PreciseTimer);//设定高精度定时器
//    connect(m_timer_advanced,SIGNAL(timeout()),this,SLOT(mTimerAdvanced_timeout()));

    this->Master_attach(general_xx->get_MasterPtr());
    if(m_master == nullptr){
        emit User_Windows_CriticalError(Sys_Err_InvalidMaster);
        m_bottomText->appendPlainText(tr("Invalid master!"));
    }
    else{
        //设置master check thread,可以防止运行状态时，突然切换到安全运行状态
        m_master->Master_InitCheckThread();
        m_master->Master_isCheckThread(true);
    }


}

void MainFormView::EthercatApp_destroy(){
//    m_timer_advanced->stop();
//    m_motorApp_callback.deleteLater();
//    GcodeSendThread->quit();
//    GcodeSendThread->wait();
    if(RTtimer_User.isRun){
       TestKillTimer(RTtimer_User.Timer_id);
       RTtimer_User.isRun = false;
    }

    if(m_master->Master_getSlaveCount()>0){
       m_master->Master_close(true);
    }

    this->Master_dettach();
}
