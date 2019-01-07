#ifndef MMTIMER_RT_H
#define MMTIMER_RT_H

#include <QObject>

class MMTimer_RT : public QObject
{
    Q_OBJECT
public:
    explicit MMTimer_RT(QObject *parent = nullptr);
    virtual void setInterval(int interval_ms) = 0;
    virtual void start(int interval_ms) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    static void MMTimer_RT::Delay_MSec(int msec);
    static void MMTimer_RT::Delay_MSec_Suspend(int msec);
signals:
    void timeout();
public slots:
};

#endif // MMTIMER_RT_H
