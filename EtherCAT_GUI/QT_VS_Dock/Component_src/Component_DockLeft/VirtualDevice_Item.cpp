#include "VirtualDevice_Item.h"
#include <QDebug>

#include <QIcon>
#include <QApplication>
#include <QStyle>

///
/// \brief 构造函数
///
VirtualDevice_item::VirtualDevice_item():QListWidgetItem()
{
   this->setIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon));
}

///
/// \brief 析构函数
///
VirtualDevice_item::~VirtualDevice_item()
{
//   qDebug() << "~VirtualDevice_item";
}

///
/// \brief 获取Virtual slave对象
/// \return
///
Ethercat_Slave VirtualDevice_item::get_virtualSlave() const
{
   return m_virtualSlave;
}

///
/// \brief 设定Virtual slave对象
/// \param slave
///
void VirtualDevice_item::set_virtualSlave(Ethercat_Slave &slave)
{
    m_virtualSlave = slave;
}

///
/// \brief 获取关联的文件
/// \return
///
QString VirtualDevice_item::get_fileName() const
{
    return m_fileName;
}

///
/// \brief 设定关联的文件
/// \param name
///
void VirtualDevice_item::set_fileName(QString name)
{
    m_fileName = name;
}
