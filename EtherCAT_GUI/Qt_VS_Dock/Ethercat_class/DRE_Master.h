#ifndef DRE_MASTER_H
#define DRE_MASTER_H

#include <QString>
#include <QStringList>
#include <QList>

#define USE_CHECK_THREAD 1

#define Master_iid "DRE.My_Interface_Master"//定义ID

///
/// \brief The Ethercat_SlaveMSG_Item class
///
class Ethercat_SlaveMSG_Item
{
public:
    Ethercat_SlaveMSG_Item() {}
public:
    int m_slave_num;//隶属的从站编号
    int m_sm_num;//隶属的sm邮箱编号(识别输入输出)
    int m_addr_offset;//地址偏移量
    int m_addr_bit;//地址的位(对bool型变量有用)

    int m_obj_index;//Item的对象在xml中的编号
    int m_obj_index_sub;//Item的对象在xml中的子编号
    int m_item_bitLen; //Item的对象内存占据的大小(以bit为单位)
    int m_date_type;//Item的对象的数据类型
    QString m_name;//Item的对象在xml中的名称

    const QString dataType_Tostring();
    const QStringList dump_data(bool isDebug = false);
protected:
    typedef enum
    {
       DRE_ECT_BOOLEAN         = 0x0001,
       DRE_ECT_INTEGER8        = 0x0002,
       DRE_ECT_INTEGER16       = 0x0003,
       DRE_ECT_INTEGER32       = 0x0004,
       DRE_ECT_UNSIGNED8       = 0x0005,
       DRE_ECT_UNSIGNED16      = 0x0006,
       DRE_ECT_UNSIGNED32      = 0x0007,
       DRE_ECT_REAL32          = 0x0008,
       DRE_ECT_VISIBLE_STRING  = 0x0009,
       DRE_ECT_OCTET_STRING    = 0x000A,
       DRE_ECT_UNICODE_STRING  = 0x000B,
       DRE_ECT_TIME_OF_DAY     = 0x000C,
       DRE_ECT_TIME_DIFFERENCE = 0x000D,
       DRE_ECT_DOMAIN          = 0x000F,
       DRE_ECT_INTEGER24       = 0x0010,
       DRE_ECT_REAL64          = 0x0011,
       DRE_ECT_INTEGER64       = 0x0015,
       DRE_ECT_UNSIGNED24      = 0x0016,
       DRE_ECT_UNSIGNED64      = 0x001B,
       DRE_ECT_BIT1            = 0x0030,
       DRE_ECT_BIT2            = 0x0031,
       DRE_ECT_BIT3            = 0x0032,
       DRE_ECT_BIT4            = 0x0033,
       DRE_ECT_BIT5            = 0x0034,
       DRE_ECT_BIT6            = 0x0035,
       DRE_ECT_BIT7            = 0x0036,
       DRE_ECT_BIT8            = 0x0037
    } DRE_datatype;

};

///
/// \brief The Ethercat_Slave class
///
class Ethercat_Slave
{
public:
    Ethercat_Slave() {}

    QList<Ethercat_SlaveMSG_Item> input_list;//主站的input相当于从站的output
    QList<Ethercat_SlaveMSG_Item> output_list;//主站的output相当于从站的input

    int m_slave_index;
    QString m_name;
    uint32_t  m_Obits;
    uint32_t  m_Ibits;
    /** Manufacturer from EEprom */
    uint32_t           m_eep_man;
    /** ID from EEprom */
    uint32_t           m_eep_id;
    /** revision from EEprom */
    uint32_t          m_eep_rev;

    const QStringList dump_data(bool isDebug = false);
private:

};

class DRE_Master
{
public:
//    DRE_Master(){}
public:
    enum{
      ERROR_NONE = 0,
      ERROR_NO_ADAPTER = -1,
      ERROR_NO_SLAVE = -2
    }Error_state_enum;

    enum
    {
       /** No valid state. */
       STATE_NONE           = 0x00,
       /** Init state*/
       STATE_INIT           = 0x01,
       /** Pre-operational. */
       STATE_PRE_OP         = 0x02,
       /** Boot state*/
       STATE_BOOT           = 0x03,
       /** Safe-operational. */
       STATE_SAFE_OP        = 0x04,
       /** Operational */
       STATE_OPERATIONAL    = 0x08,
       /** Error or ACK error */
       STATE_ACK            = 0x10,
       STATE_ERROR          = 0x10
    } state_enum;

    typedef enum{
        Adapter_name,
        Adapter_desc
    }adapter_type_choose;

    /* Adapter */
    virtual bool Find_adapter() = 0;
    virtual QStringList get_AdapterName() = 0;
    virtual QStringList get_AdapterDescription() = 0;

    virtual void set_CurrentAdapter(QString &adapterName,QString &adapterDesc) = 0;
    virtual QList<Ethercat_Slave>* get_SlaveListPtr() = 0;
    virtual int get_PLC_Period() = 0;
    virtual void set_PLC_Period(int period_ms) = 0;
    virtual const QString get_CurrentAdapter(int type) = 0;
//    QString m_adapterNameSelect;
//    QString m_adapterDescSelect;

//    QList<Ethercat_Slave> slaves_list;
    /* Adapter End */

    /* Eterhcat system */
    virtual int Master_init(const QString &ifname) = 0;
    virtual int Master_init(const QString &ifname1, const QString &ifname2) = 0;
    virtual void Master_close(bool isTurnToInit = false) = 0;
    virtual int Master_config(bool isAutoScan) = 0;
    virtual bool Master_configDC() = 0;
    virtual int Master_stop() = 0;
    virtual int Master_run() = 0;

    virtual int Master_scan(const QString &ifname) = 0;
    virtual int Master_scan() = 0;
    virtual int Master_processData_send() = 0;
    virtual int Master_processData_receive() = 0;
    virtual int Master_processData_receive(int timeout) = 0;
    virtual bool Master_process_loop(bool isCheck=false, int check_state=STATE_NONE, int check_timeOut_us=0) = 0;
    virtual bool Master_process_loop() = 0;

    virtual int Master_readState_process() = 0;
    virtual int Master_getSlaveCount() = 0;
    virtual int Master_get_expectedWKC() = 0;
    virtual int Master_writeState(int slave_num, int state) = 0;
    virtual int Master_readState(int slave_num) = 0;
    virtual int Master_CheckState(int slave_num, int state, int check_timeOut_us) = 0;
    virtual int Master_ChangeState(int slave_num, int state_inquire) = 0;

    static QString Master_stateToString(int state);

    /* Eterhcat system end */

    /* Others */
    virtual char *Master_getAddressBase() = 0;

    #if USE_CHECK_THREAD
    virtual void Master_InitCheckThread() = 0;
    #endif
    virtual void Master_isCheckThread(bool isCheck) = 0;
    /* Others end */
};

#endif // DRE_MASTER_H
