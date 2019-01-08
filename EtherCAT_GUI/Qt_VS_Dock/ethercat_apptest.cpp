#include "mainformview.h"

#include "RTTimer.h"

typedef struct{
   int Timer_id;
   bool isRun;
   int period_us;
}RTtimer_User_S;

RTtimer_User_S RTtimer_User = {
    1,false,2000
};

DWORD_PTR user;

void CALLBACK TimerCallBack()//回调函数
{
//    if(ret == Error_TimeOut){
//        MY_DEBUG_PRINT("OverTime!\r\n");
//    }
    MainFormView *t = reinterpret_cast<MainFormView*>(user);//强制转换

    static volatile int wkc;
    int expectedWKC = t->get_UserGeneralTab()->master->Master_get_expectedWKC();
     //t->m_timer_advanced->is_enableCallBack = false;//防止多次并发
     int state = t->get_UserGeneralTab()->master->Master_readState(0);
     //qDebug() << user_form_generalTab->master->Master_stateToString(state);
     if(state == My_EthercatMaster::STATE_OPERATIONAL){
         if(wkc >= expectedWKC){//第一次运行会出现这个问题
              t->m_motorApp_callback.Master_AppLoop_callback();
         }
         else{
             qWarning() << "wkc<expectedWKC";
         }
     }
     else{
         qDebug() << t->get_UserGeneralTab()->master->Master_stateToString(state);
         t->get_UserGeneralTab()->master->Master_writeState(0,My_EthercatMaster::STATE_OPERATIONAL);
         t->get_UserGeneralTab()->master->Master_CheckState(0,My_EthercatMaster::STATE_OPERATIONAL,1000);
     }
     //user_form_generalTab->master->Master_process_loop(My_EthercatMaster::STATE_OPERATIONAL,1000);//设置5ms的检查延时
     t->get_UserGeneralTab()->master->Master_processData_send();
     wkc= t->get_UserGeneralTab()->master->Master_processData_receive(5000);//短了wkc<expectedWKC
     //t->m_timer_advanced->is_enableCallBack = true;

//    static int i;
//    qDebug() << i++;
}

/********************* Slots  *******************************/
void MainFormView::mTimerAdvanced_timeout()
{
   static volatile int wkc;
   int expectedWKC = user_form_generalTab->master->Master_get_expectedWKC();
    m_timer_advanced->is_enableCallBack = false;//防止多次并发
    int state = user_form_generalTab->master->Master_readState(0);
    //qDebug() << user_form_generalTab->master->Master_stateToString(state);
    if(state == My_EthercatMaster::STATE_OPERATIONAL){
        if(wkc >= expectedWKC){//第一次运行会出现这个问题
             m_motorApp_callback.Master_AppLoop_callback();
        }
        else{
            qWarning() << "wkc<expectedWKC";
        }
    }
    else{
        qDebug() << user_form_generalTab->master->Master_stateToString(state);
        user_form_generalTab->master->Master_writeState(0,My_EthercatMaster::STATE_OPERATIONAL);
        user_form_generalTab->master->Master_CheckState(0,My_EthercatMaster::STATE_OPERATIONAL,1000);
    }
    //user_form_generalTab->master->Master_process_loop(My_EthercatMaster::STATE_OPERATIONAL,1000);//设置5ms的检查延时
    user_form_generalTab->master->Master_processData_send();
    wkc= user_form_generalTab->master->Master_processData_receive(5000);//短了wkc<expectedWKC
    m_timer_advanced->is_enableCallBack = true;
}
/********************* Slots end  *******************************/

/******************** Ethercat API(提供给mainwindow的槽使用) ******************************/
///
/// \brief MainFormView::Master_scan
///
int MainFormView::Master_scan(){
    int state = 0;
    delete mDeviceTree;
    Init_FrameLeft_Content();//重新初始化设备树

    mDeviceTree->Add_LeftTree_Master();
    if(user_form_generalTab->master->m_adapterNameSelect.isEmpty()){
        mTabWedget_center->show();
        m_widget_slaveMSG->hide();
        return -1;
    }
    ProcessBar_startEvent();

    MMTimer_RT::Delay_MSec(300);//需要无阻塞的延时 500ms

    state = user_form_generalTab->master->Master_scan();

    if(state == My_EthercatMaster::ERROR_NO_SLAVE){
        ProcessBar_stopEvent();//不出现processBar
        QMessageBox::information(this,tr("Information"),tr("No Slave Found!"));
        return -2;
    }
    else if(state == My_EthercatMaster::ERROR_NONE){
//        m_tableView_slaveMSG->clearContents();
//        m_tableView_slaveItemMSG->clearContents();
//        int slave_count = 0;
//        int slaveItem_count = 0;
//        mDeviceTree->Add_LeftTree_Master();
        foreach (Ethercat_Slave slave, user_form_generalTab->master->slaves_list) {
            //qDebug() << slave.dump_data(true);
            //TextList_append(m_bottomText,slave.dump_data(true));
            //m_tableView_slaveMSG->append_RawData(slave_count++,slave.dump_data());
            mDeviceTree->Add_LeftTree_Slaves(slave.m_slave_index,slave.m_name);
            foreach (Ethercat_SlaveMSG_Item input, slave.input_list) {
                //qDebug() << input.dump_data(true);
//                TextList_append(m_bottomText,input.dump_data(true));
                 //m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,input.dump_data());
                mDeviceTree->Add_LeftTree_SlaveMessage(1,slave.m_slave_index,MARK_SLAVE_ITEM_INPUT - MARK_SLAVE_ITEM,input.m_name);
            }
            foreach (Ethercat_SlaveMSG_Item output, slave.output_list) {
               // qDebug() << output.dump_data(true);
//                TextList_append(m_bottomText,output.dump_data(true));
                //m_tableView_slaveItemMSG->append_RawData(slaveItem_count++,output.dump_data());
                mDeviceTree->Add_LeftTree_SlaveMessage(1,slave.m_slave_index,MARK_SLAVE_ITEM_OUTPUT - MARK_SLAVE_ITEM,output.m_name);
            }

        }
    }

    return 0;
}

///
/// \brief MainFormView::Master_run
///
int MainFormView::Master_run(){

    if(user_form_generalTab->master->Master_getSlaveCount()>0){
        //设置10ms，有时候还会超时，并非严格的10ms周期
//        m_timer_advanced->setInterval(10);//10ms
//        m_timer_advanced->start();
//        TimeRun(Timer_id, 10000, TimerCallBack); //开启周期数据通信
//        user = (DWORD_PTR)this;

        m_motorApp_callback.Gcode_setAddress(m_GcodeSegment_Q);
//        if(m_GcodeSegment_Q->empty()){
//            qDebug() << "Master_run Q empty!";
//        }
//        else{
//            qDebug() << "Master_run Q No empty!";
//        }
        m_status_label->setText(tr("Run Mode"));

        user_form_generalTab->master->Master_run();
        m_motorApp_callback.Master_setAdressBase(user_form_generalTab->master->Master_getAddressBase());//设定地址
        m_motorApp_callback.Master_AppStart_callback();//开始运行回调

        //最好放在后面，否则会崩溃
        if(!RTtimer_User.isRun){
            TimeRun(RTtimer_User.Timer_id, RTtimer_User.period_us, TimerCallBack); //开启周期数据通信
            user = (DWORD_PTR)this;
            RTtimer_User.isRun = true;
        }

    }
    else{
        QMessageBox::warning(this,tr("Ethercat warning"),tr("No Slaves!Please scan slaves first"));
        return -1;
    }

    this->setFocus();//如果用键盘测试的话，需要设置焦点
    return 0;
}

///
/// \brief MainFormView::Master_stop
///
int MainFormView::Master_stop(){

    if(user_form_generalTab->master->Master_getSlaveCount()>0){
        //m_timer_advanced->stop();
        if(RTtimer_User.isRun){
           TestKillTimer(RTtimer_User.Timer_id);
           RTtimer_User.isRun = false;
        }

        user_form_generalTab->master->Master_stop();
        m_motorApp_callback.Master_AppStop_callback();//结束运行回调
        GcodeSendThread->quit();
        GcodeSendThread->wait();
        m_motorApp_callback.Gcode_ReleaseAddress();
//        m_motorApp_callback.Master_AppStop_callback();//结束运行回调
        m_status_label->setText(tr("Ready"));
    }
    else{
         QMessageBox::warning(this,tr("Ethercat warning"),tr("No Slaves!Please scan slaves first"));
         return -1;
    }

    return 0;
}

int MainFormView::Master_setting(){
  this->setFocus();//如果用键盘测试的话，需要设置焦点

   return 0;
}

int MainFormView::Master_exit(){
  if(user_form_generalTab->master->Master_getSlaveCount()>0){
    Master_stop();
    user_form_generalTab->master->Master_close(true);
  }

  return 0;
}

/******************** Ethercat API end ******************************/

void MainFormView::EthercatApp_init(){
    #ifdef _MSC_VER //windows
    m_timer_advanced = new MMTimer_RTwin(); //时间较短时会柱塞UI
    #endif
    //    m_timer_advanced = new QTimer();
    //    m_timer_advanced->setTimerType(Qt::PreciseTimer);//设定高精度定时器
    connect(m_timer_advanced,SIGNAL(timeout()),this,SLOT(mTimerAdvanced_timeout()));

    //设置master check thread,可以防止运行状态时，突然切换到安全运行状态
    user_form_generalTab->master->Master_InitCheckThread();
    user_form_generalTab->master->Master_isCheckThread(true);

    m_GcodeSegment_Q = nullptr;
    gp_t = new GcodeParser();
//    m_GcodeSegment_Q = new QQueue<Gcode_segment>();
    GcodeSendThread = new QThread();
    m_motorApp_callback.moveToThread(GcodeSendThread);
    connect(GcodeSendThread,SIGNAL(started()),&m_motorApp_callback,SLOT(GcodeSendThread_run()));//Gcode发送线程
    connect(&m_motorApp_callback,SIGNAL(Gcode_lineChange(int)),this,SLOT(MotorCallback_GcodeLineChange(int)));//实现Gcode滚动效果
//    thread = new QThread();
//    user_form_generalTab->moveToThread(thread);
//    connect(thread,SIGNAL(started()),user_form_generalTab,SLOT(Master_scanSlot()));
}

void MainFormView::EthercatApp_destroy(){
    m_timer_advanced->stop();
    m_motorApp_callback.deleteLater();
    GcodeSendThread->quit();
    GcodeSendThread->wait();
    if(user_form_generalTab->master->Master_getSlaveCount()>0){
       user_form_generalTab->master->Master_close(true);
    }
}

void MainFormView::keyPressEvent(QKeyEvent *event){
    if(m_motorApp_callback.input_ptr == NULL){
        return;
    }

    QVector3D coor_temp;

    switch(event->key()){
    case Qt::Key_Space:
        memset(m_motorApp_callback.loop_count,0,sizeof(m_motorApp_callback.loop_count));//stop
        m_motorApp_callback.m_Stepper_block_Q->clear();
        m_motorApp_callback.m_sys_reset = true;
        break;
    case Qt::Key_Control:
        m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_X] = m_lineEdit_XPos->text().toFloat();//300;
        m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Y] = m_lineEdit_YPos->text().toFloat();//200;
        m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Z] = m_lineEdit_ZPos->text().toFloat();//100;
        m_motorApp_callback.Planner_BufferLine(m_motorApp_callback.m_ARM_Motion_test.arm,Gcode_segment::No_Mcode);
        m_motorApp_callback.m_sys_reset = false;
//        m_motorApp_callback.start();
        break;
     case Qt::Key_R:
            m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_X] = 280.80;
            m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Y] = 0;
            m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Z] = 155;
//            m_motorApp_callback.start();
            m_motorApp_callback.Planner_BufferLine(m_motorApp_callback.m_ARM_Motion_test.arm,Gcode_segment::No_Mcode);
            m_motorApp_callback.m_sys_reset = false;

        break;
     case Qt::Key_S:
//        if(m_motorApp_callback.m_sys_reset){
//            while(!m_GcodeSegment_Q->empty()){
//                QVector3D data = m_GcodeSegment_Q->dequeue().data_xyz;
//                data_last = data;
//                qDebug() << data;
//                m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_X] = data.x();
//                m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Y] = data.y();
//                m_motorApp_callback.m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Z] = data.z();
//                m_motorApp_callback.Planner_BufferLine(m_motorApp_callback.m_ARM_Motion_test.arm,0);
//            }
//            m_motorApp_callback.m_sys_reset = false;
//            m_motorApp_callback.start();
//            m_motorApp_callback.loop_count[0] = 1;
//        }
            GcodeSendThread->start();
        break;
   case Qt::Key_Q:
        m_lineEdit_XPos->setText(tr("280.799"));
        m_lineEdit_YPos->setText(tr("0"));
        m_lineEdit_ZPos->setText(tr("155"));
        break;
    case Qt::Key_A:
        m_motorApp_callback.m_RenewST_ready = true;
        break;
    case Qt::Key_Left://左
        coor_temp.setY(m_lineEdit_YPos->text().toFloat()-m_lineEdit_Pos_Step->text().toInt());
        m_lineEdit_YPos->setText(QString("%1").arg(coor_temp.y()));
        break;
    case Qt::Key_Right://右
        coor_temp.setY(m_lineEdit_YPos->text().toFloat()+m_lineEdit_Pos_Step->text().toInt());
        m_lineEdit_YPos->setText(QString("%1").arg(coor_temp.y()));
        break;
    case Qt::Key_Up://前
        coor_temp.setX(m_lineEdit_XPos->text().toFloat()+m_lineEdit_Pos_Step->text().toInt());
        m_lineEdit_XPos->setText(QString("%1").arg(coor_temp.x()));
        break;
    case Qt::Key_Down://后
        coor_temp.setX(m_lineEdit_XPos->text().toFloat()-m_lineEdit_Pos_Step->text().toInt());
        m_lineEdit_XPos->setText(QString("%1").arg(coor_temp.x()));
        break;
    case Qt::Key_Z://上
        coor_temp.setZ(m_lineEdit_ZPos->text().toFloat()+m_lineEdit_Pos_Step->text().toInt());
        m_lineEdit_ZPos->setText(QString("%1").arg(coor_temp.z()));
        break;
    case Qt::Key_X://下
        coor_temp.setZ(m_lineEdit_ZPos->text().toFloat()-m_lineEdit_Pos_Step->text().toInt());
        m_lineEdit_ZPos->setText(QString("%1").arg(coor_temp.z()));
        break;
    default:
        qDebug() << event->key();
        break;
    }
}
