#ifndef ETHERCAT_SCANTHREAD_H
#define ETHERCAT_SCANTHREAD_H


#include <QThread>
#include "Component_Private.h"

class EtherCAT_ScanThread : public QThread,public Component_Private
{
    Q_OBJECT
    Q_INTERFACES(Component_Private)
public:
    EtherCAT_ScanThread(QObject *parent = nullptr);
    virtual ~EtherCAT_ScanThread();
public:
    virtual bool Init_Cores();
    virtual bool Destroy_Cores();
    virtual bool Init_Object();
    virtual bool Init_Slots();

    virtual void run();

    int Master_get_scanResult(){return m_Master_scanResult;}
    void Master_set_scanResult(int res){ m_Master_scanResult = res;}
private:
    int m_Master_scanResult;
private slots:

};

#endif // ETHERCAT_SCANTHREAD_H
