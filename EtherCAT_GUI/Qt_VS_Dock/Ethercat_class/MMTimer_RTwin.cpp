#include "MMTimer_RTwin.h"
#include <QDebug>

#ifdef __MINGW32__ //w32api bug
#define TIME_KILL_SYNCHRONOUS 0x0100
#endif

MMTimer_RTwin::MMTimer_RTwin(QObject *parent) : MMTimer_RT(parent)
{
  is_enableCallBack = true;
}

MMTimer_RTwin::MMTimer_RTwin(int interval, QObject *parent):MMTimer_RT(parent),m_interval(interval),m_id(0)
{

}

MMTimer_RTwin::~MMTimer_RTwin()
{
    stop();
}

void MMTimer_RTwin::setInterval(int interval_ms)
{
    m_interval = interval_ms;
}

void MMTimer_RTwin::start()
{
    m_id = timeSetEvent(m_interval, 0, mmtimer_proc, (DWORD_PTR)this,
                    TIME_CALLBACK_FUNCTION | TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);//第二个参数：以毫秒指定延时的精度，越小精度越高
}

void MMTimer_RTwin::start(int interval_ms)
{
    m_interval = interval_ms;
    m_id = timeSetEvent(m_interval, 1, mmtimer_proc, (DWORD_PTR)this,
                    TIME_CALLBACK_FUNCTION | TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);
}

void MMTimer_RTwin::stop()
{
   if (m_id){
      timeKillEvent(m_id);
      m_id = 0;
   }
}

void WINAPI CALLBACK mmtimer_proc(uint timerId, uint, DWORD_PTR user, DWORD_PTR, DWORD_PTR)
{
    //static int xx;
    Q_UNUSED(timerId);

    MMTimer_RTwin *t = reinterpret_cast<MMTimer_RTwin*>(user);//强制转换
    if(t->is_enableCallBack){//多次并发，会造成UI阻塞
//        qDebug() << xx;
//        xx= 0;
        emit t->timeout();//貌似会触发3-5次
    }
//    else{//WARNING: Ethecat协议解析总是会超时间
//        qDebug() << "Timeout";
////        xx++;
//    }
}
