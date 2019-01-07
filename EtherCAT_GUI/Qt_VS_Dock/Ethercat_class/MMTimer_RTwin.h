#ifndef MMTIMER_RTWIN_H
#define MMTIMER_RTWIN_H

#include "MMTimer_RT.h"
#include <qt_windows.h>
///
/// \brief windows qt下高精度定时器
///
class MMTimer_RTwin : public MMTimer_RT
{
    Q_OBJECT
public:
    explicit MMTimer_RTwin(QObject *parent = nullptr);
    explicit MMTimer_RTwin(int interval, QObject *parent = nullptr);
    ~MMTimer_RTwin();
    void setInterval(int interval_ms);
    void start(int interval_ms);
    void start();
    void stop();
    bool is_enableCallBack;
private:
    int m_interval;
    int m_id;

friend void WINAPI CALLBACK mmtimer_proc(uint, uint, DWORD_PTR, DWORD_PTR, DWORD_PTR);
signals:
//    void timeout();
public slots:

};

#endif // MMTimer_RTwin_H
