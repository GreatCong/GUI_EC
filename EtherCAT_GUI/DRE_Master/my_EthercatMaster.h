#ifndef MY_ETHERCATMASTER_H
#define MY_ETHERCATMASTER_H

//#include <QGenericPlugin>
#include <QObject>
#include "DRE_Master.h"

Q_DECLARE_INTERFACE(DRE_Master, Master_iid) //定义接口

///
/// \brief The My_EthercatMaster class
///
class My_EthercatMaster : public QObject,public DRE_Master
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID Master_iid FILE "DRE_Master.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(DRE_Master)

public:
    explicit My_EthercatMaster(QObject *parent = nullptr);
    ~My_EthercatMaster();

public:

    /* Adapter */
    bool Find_adapter();
    QStringList get_AdapterName();
    QStringList get_AdapterDescription();
    /* Adapter End */

    void set_CurrentAdapter(QString &adapterName,QString &adapterDesc);
    QList<Ethercat_Slave>* get_SlaveListPtr();
    int get_PLC_Period();
    void set_PLC_Period(int period_ms);
    const QString get_CurrentAdapter(int type);

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

    #if USE_CHECK_THREAD
    void Master_InitCheckThread();
    #endif
    void Master_isCheckThread(bool isCheck);

    void Master_usSleep(uint32_t usec);
    /* Others end */
protected:
    QString m_adapterNameSelect;
    QString m_adapterDescSelect;

    QList<Ethercat_Slave> *m_slaves_list;
    int m_PLC_Period;
private:
    QStringList _adapterNameList;
    QStringList _adapterDescriptionList;

    char *m_IOmap;


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
