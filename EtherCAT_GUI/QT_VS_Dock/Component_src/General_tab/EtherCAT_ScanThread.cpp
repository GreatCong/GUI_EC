#include "EtherCAT_ScanThread.h"

///
/// \brief 构造函数
/// \param parent
///
EtherCAT_ScanThread::EtherCAT_ScanThread(QObject *parent):QThread(parent),Component_Private()
{
    this->Init_Object();
}

///
/// \brief 析构函数
///
EtherCAT_ScanThread::~EtherCAT_ScanThread()
{
    this->quit();
}

///
/// \brief 初始化
/// \return
///
bool EtherCAT_ScanThread::Init_Cores()
{
   this->m_Master_scanResult = 0;

   return true;
}

///
/// \brief 销毁应用
/// \return
///
bool EtherCAT_ScanThread::Destroy_Cores()
{
  return true;
}

///
/// \brief 初始化对象
/// \return
///
bool EtherCAT_ScanThread::Init_Object()
{
    this->set_componentWidgetPtr(nullptr);

    return true;
}

///
/// \brief 初始化信号槽
/// \return
///
bool EtherCAT_ScanThread::Init_Slots()
{
    return true;
}

///
/// \brief 线程执行入口
///
void EtherCAT_ScanThread::run()
{
    int state = 0;

    if(this->ComponentP_get_MasterPtr() == nullptr){
        return;
    }

    state = this->ComponentP_get_MasterPtr()->Master_scan();

    this->Master_set_scanResult(state);
}
