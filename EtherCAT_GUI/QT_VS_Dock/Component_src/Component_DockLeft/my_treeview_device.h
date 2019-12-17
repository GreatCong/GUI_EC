#ifndef MY_TREEVIEW_DEVICE_H
#define MY_TREEVIEW_DEVICE_H

#include <QTreeView>
#include <QStandardItemModel>

#include "user_common.h"

///
/// \brief 设备树class
///
class My_TreeView_Device : public QTreeView
{
    Q_OBJECT
public:
    explicit My_TreeView_Device(QWidget *parent = nullptr);
    virtual ~My_TreeView_Device();
public:
    typedef enum{
        ROLE_MARK = Qt::UserRole + 1,
        ROLE_MARK_DEVICE,
        ROLE_MARK_MASTER,
        ROLE_MARK_SLAVE,
        ROLE_MARK_SLAVE_ITEM,
        ROLE_MARK_SLAVE_MESSAGE
    }Tree_Mark_Role;

    //对应ROLE_MARK
    typedef enum{
        MARK_DEVICE,//Device标记
        MARK_MASTER = 100,//Master标记,最多支持100个主站
        MARK_SLAVE = 200,//Slave标记,最多支持100个从站
        MARK_SLAVE_ITEM = 300, //从站信息标记
        MARK_SLAVE_ITEM_INPUT,//从站输入模块
        MARK_SLAVE_ITEM_OUTPUT=310,//从站输出模块
        MARK_SLAVE_MESSAGE = 400 //从站信息标记
    }Tree_Mark;
public:
    void Init_Left_DeviceTree();
    bool Add_LeftTree_Master();
    QStandardItem *getMasterFolder(int master_count = 1);
    bool Add_LeftTree_Slaves(int master_count = 1);
    bool Add_LeftTree_Slaves(int master_count, const QString &slave_name,int input_count=1,int output_count=1);
    QStandardItem *getSlaveFolder(int master_count = 1,int slave_count = 1);
    bool Add_LeftTree_SlaveItems(int master_count = 1, int slave_count = 1, int slaveItem = MARK_SLAVE_ITEM_INPUT, int item_count=1);
    QStandardItem *getSlaveItemFolder(int master_count=1,int slave_count=1,int slaveItem_count=1);
    bool Add_LeftTree_SlaveMessage(int master_count=1, int slave_count=1, int slaveItem_count=1);
    bool Add_LeftTree_SlaveMessage(int master_count, int slave_count, int slaveItem_count, const QString &message);
    QStandardItemModel* getTreeModel();
    QList<QStandardItem*> getRoots();

    bool isMaster(int mark);
    bool isSlave(int mark);
    bool isSlaveItem(int mark);
    bool isSlaveMessage(int mark);
private:
    bool isItemsMark(int mark, int mark_def, int interval);

    int s_master_count;
    int s_slave_count;
    int s_slaveMessage_count;
signals:

public slots:
};

#endif // MY_TREEVIEW_DEVICE_H
