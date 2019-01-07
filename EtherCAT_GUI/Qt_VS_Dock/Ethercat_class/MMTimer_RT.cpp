#include "MMTimer_RT.h"
#include <QTime>
#include <QApplication>

MMTimer_RT::MMTimer_RT(QObject *parent) : QObject(parent)
{

}

/*********************** static *********************************/
void MMTimer_RT::Delay_MSec(int msec){
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )

    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MMTimer_RT::Delay_MSec_Suspend(int msec){
    QTime _Timer = QTime::currentTime();

    QTime _NowTimer;
    do{
       _NowTimer=QTime::currentTime();
    }while (_Timer.msecsTo(_NowTimer)<=msec);
}
/********************** static end ******************************/
