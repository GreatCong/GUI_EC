#ifndef VIRTUALDEVICE_ITEM_H
#define VIRTUALDEVICE_ITEM_H

#include <QListWidgetItem>
#include "DRE_Master.h"

class VirtualDevice_item : public QListWidgetItem
{
public:
    VirtualDevice_item();
    virtual ~VirtualDevice_item();

    Ethercat_Slave get_virtualSlave() const;
    void set_virtualSlave(Ethercat_Slave &slave);

    QString get_fileName() const;
    void set_fileName(QString name);
private:
    Ethercat_Slave m_virtualSlave;
    QString m_fileName;
};

#endif // VIRTUALDEVICE_ITEM_H
