#include "mainformview.h"

///
/// \brief 用于测试的对象初始化
///
void MainFormView::Init_TestCores(){
    mm_time = new QTimer();
    connect(mm_time,SIGNAL(timeout()),this,SLOT(mm_timerOut()));
}



/************  Slots *****************/

///
/// \brief 扫描从站
///
void MainFormView::mDeviceTree_EcScan_clicked(){
//  mm_time->start(50);//1000ms
//  timer_num = 0;
//  m_scanWait_processBar->show();
////  Set_StatusMessage(tr("Waiting for Scanning!"),3000);
//  m_status_label->setText(tr("Scanning..."));
    Master_scan();
}

///
/// \brief MainFormView::AddTree_Master_clicked
///
void MainFormView::AddTree_Master_clicked(){
    mDeviceTree->Add_LeftTree_Master();
}

void MainFormView::AddTree_Slave_clicked(){
    mDeviceTree->Add_LeftTree_Slaves();
}

void MainFormView::AddTree_SlaveItem_clicked(){
    mDeviceTree->Add_LeftTree_SlaveItems();
}

void MainFormView::AddTree_SlaveMessageInput_clicked(){
    mDeviceTree->Add_LeftTree_SlaveMessage(1,1,MARK_SLAVE_ITEM_INPUT-MARK_SLAVE_ITEM);
}

void MainFormView::AddTree_SlaveMessageOutput_clicked(){
    mDeviceTree->Add_LeftTree_SlaveMessage(1,1,MARK_SLAVE_ITEM_OUTPUT-MARK_SLAVE_ITEM);
}
/************  Slots end ***************/
