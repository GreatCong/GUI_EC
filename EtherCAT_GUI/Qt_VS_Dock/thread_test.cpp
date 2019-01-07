#include "thread_test.h"
#include <QDebug>
#include <QThread>

thread_test::thread_test(QObject *parent) : QObject(parent)
{

}

void thread_test::debug(){
    qDebug() << "OK";
    qDebug()<<"from thread slot:" <<QThread::currentThreadId();
}
