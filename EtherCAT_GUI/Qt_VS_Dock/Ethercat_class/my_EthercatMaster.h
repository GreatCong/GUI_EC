#ifndef MY_ETHERCATMASTER_H
#define MY_ETHERCATMASTER_H

#include <QObject>

#define USE_CHECK_THREAD 1

#include "EtherCAT_UserApp.h"

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
private:

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

///
/// \brief The My_EthercatMaster class
///
class My_EthercatMaster : public QObject
{
    Q_OBJECT
public:
    explicit My_EthercatMaster(QObject *parent = nullptr);
    ~My_EthercatMaster();
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

    /* Adapter */
    bool Find_adapter();
    QStringList get_AdapterName();
    QStringList get_AdapterDescription();

    QString m_adapterNameSelect;
    QString m_adapterDescSelect;

    QList<Ethercat_Slave> slaves_list;
    /* Adapter End */

    /* calback */
    void Master_set_UserCallBack(Ethercat_Callback *callback);
    Ethercat_Callback *Master_get_UserCallBack();
    /* calback End */

    /* Eterhcat system */
    int Master_init(const QString &ifname);
    int Master_init(const QString &ifname1, const QString &ifname2);
    void Master_close(bool isTurnToInit = false);
    int Master_config(bool isAutoScan);
    bool Master_configDC();
    int Master_stop();
    int Master_run();

    int Master_scan(const QString &ifname);
    int Master_scan();
    int Master_processData_send();
    int Master_processData_receive();
    int Master_processData_receive(int timeout);
    bool Master_process_loop(bool isCheck=false, int check_state=STATE_NONE, int check_timeOut_us=0);
    bool Master_process_loop();

    int Master_readState_process();
    int Master_getSlaveCount();
    int Master_get_expectedWKC();
    int Master_writeState(int slave_num, int state);
    int Master_readState(int slave_num);
    int Master_CheckState(int slave_num, int state, int check_timeOut_us);
    int Master_ChangeState(int slave_num, int state_inquire);

    /* Eterhcat system end */

    /* Others */
    char *Master_getAddressBase();
    static QString My_EthercatMaster::Master_stateToString(int state);


    static void My_EthercatMaster:: Master_usSleep(uint32_t usec);

    #if USE_CHECK_THREAD
    void Master_InitCheckThread();
    #endif
    void Master_isCheckThread(bool isCheck);
    /* Others end */

private:
    QStringList _adapterNameList;
    QStringList _adapterDescriptionList;

    char *m_IOmap;

    Ethercat_Callback *m_callback;


    char usdo[128];
    char hstr[1024];

    void si_sdo(int cnt);
    int si_map_sii(int slave);
    int si_siiPDO(uint16_t slave, uint8_t t, int mapoffset, int bitoffset);
    int si_map_sdo(int slave);
    int si_PDOassign(uint16_t slave, uint16_t PDOassign, int mapoffset, int bitoffset);
    char *SDO2string(uint16_t slave, uint16_t index, uint8_t subidx, uint16_t dtype);
    char *dtype2string(uint16_t dtype);

    int Master_si_PDOassign(uint16_t slave, uint16_t PDOassign, int mapoffset, int bitoffset);
    int Master_siMap_sdo(int slave);
signals:

public slots:
};

#endif // MY_ETHERCATMASTER_H
