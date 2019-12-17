#include "mainformview.h"

#include <QMap>
#include <QMessageBox>
#include <QDebug>

#ifdef Q_OS_WIN
#include "MMTimer_RTwin/RTTimer.h"
#endif

///
/// \brief PLC的定时器结构体
//
typedef struct{
   int Timer_id;
   bool isRun;
//   int period_us;
}RTtimer_User_S;

RTtimer_User_S RTtimer_User = {
    1,false
};



#ifdef Q_OS_WIN

DWORD_PTR user;//Windows下的强制指针传递

///
/// \brief PLC回调函数
///
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
             t->get_userAppPtr()->get_CallbackPtr()->Master_AppLoop_callback();
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
     t->get_genetalTabPtr()->Master_UI_Loop(state);
     //user_form_generalTab->master->Master_process_loop(My_EthercatMaster::STATE_OPERATIONAL,1000);//设置5ms的检查延时
     t->get_MasterPtr()->Master_processData_send();
     wkc= t->get_MasterPtr()->Master_processData_receive(5000);//短了wkc<expectedWKC
     //t->m_timer_advanced->is_enableCallBack = true;

//    static int i;
//    qDebug() << i++;
}

#else
///
/// \brief 非windows系统下的PLC回调
///
void MainFormView::Master_TimerAdvanced_timeout(){
    //    if(ret == Error_TimeOut){
    //        MY_DEBUG_PRINT("OverTime!\r\n");
    //    }
        MainFormView *t = this;//强制转换

        static volatile int wkc;
        int expectedWKC = t->get_MasterPtr()->Master_get_expectedWKC();
         //t->m_timer_advanced->is_enableCallBack = false;//防止多次并发
         int state = t->get_MasterPtr()->Master_readState(0);
         //qDebug() << user_form_generalTab->master->Master_stateToString(state);
         if(state == DRE_Master::STATE_OPERATIONAL){
             if(wkc >= expectedWKC){//第一次运行会出现这个问题
                 t->get_userAppPtr()->get_CallbackPtr()->Master_AppLoop_callback();
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
         t->get_genetalTabPtr()->Master_UI_Loop(state);
         //user_form_generalTab->master->Master_process_loop(My_EthercatMaster::STATE_OPERATIONAL,1000);//设置5ms的检查延时
         t->get_MasterPtr()->Master_processData_send();
         wkc= t->get_MasterPtr()->Master_processData_receive(5000);//短了wkc<expectedWKC
         //t->m_timer_advanced->is_enableCallBack = true;

    //    static int i;
    //    qDebug() << i++;
}
#endif

/******************** Ethercat API(提供给mainwindow的槽使用) ******************************/
///
/// \brief 主站扫描
///
int MainFormView::Master_scan(){
    if(this->get_userAppPtr() == nullptr){
        QMessageBox::critical(this,tr("Information"),tr("No Plugin is loaded!"));
        return Master_Err_InvalidPlugin;
    }
    else{
        if(this->get_userAppPtr()->get_CallbackPtr() == nullptr){
            QMessageBox::critical(this,tr("Information"),tr("No Callback in the Plugin"));
            return Master_Err_InvalidCallback;
        }
    }

    if(m_master->get_CurrentAdapter(DRE_Master::Adapter_name).isEmpty()){
        mTabWedget_center->show();
        m_widget_slaveMSG->hide();
        mTabWedget_center->setCurrentIndex(ComponentWidgets::Tab_General);
        QMessageBox::critical(this,tr("Information"),tr("No Adapter is found!"));
        return Master_Err_InvalidAdapter;
    }

    //对组件循环遍历
    foreach (Component_Private* component, *(this->m_ComponentList)) {
        component->ComponentP_master_ScanStart();//组件的扫描前的回调
    }

    this->ProcessBar_startEvent();

    this->m_Master_ScanThread.ComponentP_set_MasterPtr(this->m_master);//设置master的指针
    this->m_Master_ScanThread.start();//开启扫描

    return Master_Err_NONE;
}

///
/// \brief 主站运行
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
        this->get_userAppPtr()->get_CallbackPtr()->Master_setAdressBase(m_master->Master_getAddressBase());
//        get_MasterPtr()->Master_processData_receive(5000);//添加下，保证在start回调的时候能够接收到robot_id的值
        this->get_userAppPtr()->get_CallbackPtr()->Master_AppStart_callback();

        this->get_genetalTabPtr()->Master_UI_RUn();
        masterMode_change(true);


        //最好放在后面，否则会崩溃
        if(!RTtimer_User.isRun){
            #ifdef Q_OS_WIN
            TimeRun(RTtimer_User.Timer_id, m_master->get_PLC_Period()*1000, TimerCallBack); //开启周期数据通信
            user = (DWORD_PTR)this;
            #else
            m_timer_advanced->setInterval(m_master->get_PLC_Period());
            m_timer_advanced->start();
            #endif
            RTtimer_User.isRun = true;
        }

    }
    else{
        QMessageBox::warning(this,tr("Ethercat warning"),tr("No Slaves!Please scan slaves first"));
        return Master_Err_NoSlave;
    }

//    this->setFocus();//如果用键盘测试的话，需要设置焦点
    this->get_FramePtr(Frame_right_f)->setEnabled(false);//EtherCAT销毁后才能选择加载插件
    return Master_Err_NONE;
}

///
/// \brief 主站停止
///
int MainFormView::Master_stop(){

    if(m_master->Master_getSlaveCount()>0){
        //m_timer_advanced->stop();
        if(RTtimer_User.isRun){
           #ifdef Q_OS_WIN
           TestKillTimer(RTtimer_User.Timer_id);
           #else
           m_timer_advanced->stop();
           #endif
           RTtimer_User.isRun = false;
        }

        m_master->Master_stop();
        this->get_userAppPtr()->get_CallbackPtr()->Master_AppStop_callback();
        m_status_label->setText(tr("Ready"));

        //设置状态
        this->get_genetalTabPtr()->Master_UI_Stop();
        masterMode_change(false);
    }
    else{
         QMessageBox::warning(this,tr("Ethercat warning"),tr("No Slaves!Please scan slaves first"));
         return Master_Err_NoSlave;
    }

    this->get_FramePtr(Frame_right_f)->setEnabled(false);//EtherCAT销毁后才能选择加载插件

    return Master_Err_NONE;
}

///
/// \brief 主站配置按钮响应
/// \return
///
int MainFormView::Master_setting(){
//  this->setFocus();//如果用键盘测试的话，需要设置焦点

   return 0;
}

///
/// \brief 主站退出
/// \return
///
int MainFormView::Master_exit(){
  if(m_master->Master_getSlaveCount()>0){
    Master_stop();
    m_master->Master_close(true);
  }

  this->get_FramePtr(Frame_right_f)->setEnabled(true);//EtherCAT销毁后才能选择加载插件

  return Master_Err_NONE;
}

/******************** Ethercat API end ******************************/

///
/// \brief EtherCAT应用初始化
///
void MainFormView::EthercatApp_init(){
//    #ifdef _MSC_VER //windows
//    m_timer_advanced = new MMTimer_RTwin(); //时间较短时会柱塞UI
//    #endif
//    //    m_timer_advanced = new QTimer();
//    //    m_timer_advanced->setTimerType(Qt::PreciseTimer);//设定高精度定时器
//    connect(m_timer_advanced,SIGNAL(timeout()),this,SLOT(mTimerAdvanced_timeout()));

      #ifndef Q_OS_WIN
        m_timer_advanced = new QTimer();
        m_timer_advanced->setTimerType(Qt::PreciseTimer);//设定高精度定时器
        connect(m_timer_advanced,SIGNAL(timeout()),this,SLOT(Master_TimerAdvanced_timeout()));

        this->System_BottomMessage("System is not windows,Timer is set to AdvancedTimer!");
      #endif

    this->Master_attach(this->get_genetalTabPtr()->ComponentP_get_MasterPtr());
    this->System_BottomMessage("System is not windows,EtherCAT Allication requires root user!");
    if(m_master == nullptr){
        emit User_Windows_CriticalError(Sys_Err_InvalidMaster);
        System_BottomMessage(tr("Invalid master!"));
    }
    else{
        //设置master check thread,可以防止运行状态时，突然切换到安全运行状态
        m_master->Master_InitCheckThread();
        m_master->Master_isCheckThread(true);

        connect(&this->m_Master_ScanThread,SIGNAL(started()),this,SLOT(System_MasterScanThread_started()));
        connect(&this->m_Master_ScanThread,SIGNAL(finished()),this,SLOT(System_MasterScanThread_finished()));
    }
}

///
/// \brief EtherCAT应用关闭
///
void MainFormView::EthercatApp_destroy(){
//    m_timer_advanced->stop();
//    m_motorApp_callback.deleteLater();
//    GcodeSendThread->quit();
//    GcodeSendThread->wait();

    if(RTtimer_User.isRun){
       #ifdef Q_OS_WIN
       TestKillTimer(RTtimer_User.Timer_id);
       #else
       m_timer_advanced->stop();
       #endif
       RTtimer_User.isRun = false;
    }

    if(m_master->Master_getSlaveCount()>0){
       this->Master_stop();//先stop，再close会安全些
       m_master->Master_close(true);
    }

    this->Master_dettach();
}

/**************** SLOT begin  *********************/

///
/// \brief EtherCAT扫描线程开启前事件
///
void MainFormView::System_MasterScanThread_started(){
//    qDebug() << "System_MasterScanThread_started";
    this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_run)->setEnabled(false);
    this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_stop)->setEnabled(false);
    this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_exit)->setEnabled(false);
    this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_scan)->setEnabled(true);

    this->get_FramePtr(Frame_right_f)->setEnabled(false);
}

///
/// \brief EtherCAT扫描线程关闭后事件
///
void MainFormView::System_MasterScanThread_finished(){
//    qDebug() << "System_MasterScanThread_finished";

    int state = 0;
    state = this->m_Master_ScanThread.Master_get_scanResult();

    do{
        if(state == DRE_Master::ERROR_NO_SLAVE){
            ProcessBar_stopEvent();//不出现processBar
            QMessageBox::information(this,tr("Information"),tr("No Slave Found!"));
            state =  Master_Err_NoSlave;
            break;
        }
        else if(state == DRE_Master::ERROR_NONE){
    //        m_tableView_slaveMSG->clearContents();
    //        m_tableView_slaveItemMSG->clearContents();
    //        int slave_count = 0;
    //        int slaveItem_count = 0;
    //        mDeviceTree->Add_LeftTree_Master();
    //        qDebug() << m_master->Master_getSlaveCount();
            this->get_userAppPtr()->get_CallbackPtr()->Master_setSlaveCount(this->get_MasterPtr()->Master_getSlaveCount());

            //对组件循环遍历
            foreach (Component_Private* component, *(this->m_ComponentList)) {
                component->ComponentP_master_ScanFinish();//组件的开始后的回调
            }

            //如果使能了扫描过程读取数据
            if(this->get_userAppPtr()->get_CallbackPtr()->Master_getAppScan_allowRead()){
                //m_master->Master_run();//先切换到run模式，然后再切换到safe_op模式
    //             m_master->Master_ChangeState(0,DRE_Master::STATE_OPERATIONAL);//先切换到run模式，然后再切换到safe_op模式
                //NOTE:一次性对所有进行状态切换，有时候会不稳定
                for(int slave_index=1;slave_index<=m_master->Master_getSlaveCount();slave_index++){
                    m_master->Master_ChangeState(slave_index,DRE_Master::STATE_OPERATIONAL);//先切换到run模式，然后再切换到safe_op模式
                }
                 this->get_userAppPtr()->get_CallbackPtr()->Master_setAdressBase(m_master->Master_getAddressBase());//safe_op模式下无法读取值
                get_MasterPtr()->Master_processData_receive(this->get_userAppPtr()->get_CallbackPtr()->Master_getAppScan_ReadTimeout());//添加下，保证在start回调的时候能够接收到robot_id的值
            }
            this->get_userAppPtr()->get_CallbackPtr()->Master_AppScan_callback();//scan的回调
            if(this->get_userAppPtr()->get_CallbackPtr()->Master_getAppScan_allowRead()){
                m_master->Master_ChangeState(0,DRE_Master::STATE_SAFE_OP);
            }

            //设置EtherCAT状态显示
            this->get_genetalTabPtr()->Master_UI_Scan();
        }

        this->get_FramePtr(Frame_right_f)->setEnabled(false);//EtherCAT销毁后才能选择加载插件
    }while(0);

    if(state == Master_Err_NONE){
        this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_run)->setEnabled(true);
        this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_stop)->setEnabled(true);
        this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_exit)->setEnabled(true);
        this->get_arrayActionMenuBar()->at(MenuBar_actionEthercat_scan)->setEnabled(false);
    }
    else{
        this->get_FramePtr(Frame_right_f)->setEnabled(true);
    }

    this->timer_num = 56;//设置进度条为56%

    state = Master_Err_NONE;
}
/****************** SLOT end *******************/
