#include "my_EthercatMaster.h"

#include "ethercat.h"//注意不要用在有UI的cpp文件中
#if USE_CHECK_THREAD
#include "master_check.h"
#endif
#include <stdio.h>
#include <QDebug>

ec_ODlistt ODlist;
ec_OElistt OElist;

#define IOMAP_SIZE 4096

///
/// \brief My_EthercatMaster::My_EthercatMaster
/// \param parent
///
My_EthercatMaster::My_EthercatMaster(QObject *parent) : QObject(parent)
{
    m_IOmap = (char*)malloc(sizeof(char)*IOMAP_SIZE);
    memset(m_IOmap,0,sizeof(char)*IOMAP_SIZE);

    m_adapterNameSelect = "";
    m_adapterDescSelect = "";

    m_slaves_list = new QList<Ethercat_Slave>;
    m_PLC_Period = 10;//默认10ms运行
}


///
/// \brief My_EthercatMaster::~My_EthercatMaster
///
My_EthercatMaster::~My_EthercatMaster()
{
    if(Master_getSlaveCount()>0){
      Master_close(true);
    }
}

///
/// \brief 查找网络适配器
/// \return
///
bool My_EthercatMaster::Find_adapter()
{
    ec_adaptert * adapter = NULL;
    adapter = ec_find_adapters();

    if(adapter==NULL){
        return false;
    }
    else{
        _adapterDescriptionList.clear();
        _adapterNameList.clear();
        while (adapter != NULL)
        {
    //     printf ("Description : %s, Device to use for wpcap: %s\n", adapter->desc,adapter->name);
            _adapterDescriptionList.append(adapter->desc);
            _adapterNameList.append(adapter->name);
            adapter = adapter->next;
        }

        ec_free_adapters(adapter);

        return true;
    }

}

///
/// \brief 获取网络适配器的名称
/// \return
///
QStringList My_EthercatMaster::get_AdapterName(){
    return _adapterNameList;
}

///
/// \brief 获取网络适配器的描述
/// \return
///
QStringList My_EthercatMaster::get_AdapterDescription(){
    return _adapterDescriptionList;
}


void My_EthercatMaster::set_CurrentAdapter(QString &adapterName,QString &adapterDesc){
   m_adapterNameSelect = adapterName;
   m_adapterDescSelect = adapterDesc;
}

QList<Ethercat_Slave>* My_EthercatMaster::get_SlaveListPtr(){
   return m_slaves_list;
}

int My_EthercatMaster::get_PLC_Period(){
   return m_PLC_Period;
}

void My_EthercatMaster::set_PLC_Period(int period_ms){
   m_PLC_Period = period_ms;
}

const QString My_EthercatMaster::get_CurrentAdapter(int type){
    switch(type){
    case Adapter_name:
        return m_adapterNameSelect;
        break;
    case Adapter_desc:
        return m_adapterDescSelect;
        break;
    default:
        break;
    }

   return QString("");
}


///
/// \brief EtherCAT初始化
/// \param ifname
/// \return
///
int My_EthercatMaster::Master_init(const QString & ifname){
//    qDebug() << ifname;
    //NOTE:连续进行转化，会因为临时变量销毁出错
//    const char *name = ifname.toStdString().c_str();//会乱码！！
    std::string name_str = ifname.toStdString();
    const char *name = name_str.c_str();
    return ec_init(name);
}

///
/// \brief 双网口的初始化(用于DC)
/// \param ifname1
/// \param ifname2
/// \return
///
int My_EthercatMaster::Master_init(const QString & ifname1,const QString & ifname2){
    std::string name1_str = ifname1.toStdString();
    std::string name2_str = ifname2.toStdString();
    const char *name1_const = name1_str.c_str();
    const char *name2_const = name2_str.c_str();
    char* name2 = new char[EC_MAXLEN_ADAPTERNAME];//足够长
    strcpy(name2,name2_const);
    return ec_init_redundant(name1_const,name2);
}

///
/// \brief 关闭EtherCAT
/// \param isTurnToInit
///
void My_EthercatMaster::Master_close(bool isTurnToInit){
//    ec_slave[0].state = EC_STATE_INIT;
//    /* request INIT state for all slaves */
//    ec_writestate(0);
    if(isTurnToInit){
        Master_writeState(0,STATE_INIT);//0表示all
        int chk = 40;
        do{
            Master_writeState(0,STATE_INIT);
            ec_statecheck(0, STATE_INIT, EC_TIMEOUTRET*6);
        }while(chk-- && Master_readState(0) != STATE_INIT);

    }
    ec_close();
    ec_slavecount = 0;//设置slave_num=0
}

///
/// \brief EtherCAT配置(地址)
/// \param isAutoScan
/// \return
///
int My_EthercatMaster::Master_config(bool isAutoScan){
    return ec_config(isAutoScan,m_IOmap);
}

///
/// \brief EtherCAT配置(DC功能)
/// \return
///
bool My_EthercatMaster::Master_configDC(){
    return ec_configdc();
}

///
/// \brief 停止EtherCAT(设置状态到初始init)
/// \return
///
int My_EthercatMaster::Master_stop(){
    //NOTE:Ethercat有一个状态机，如果init状态，无法直接到达operational状态
    //切换顺序,初始化->预运行->安全运行->运行
    #if USE_CHECK_THREAD
    Check_inOP = 0;//for check thread
    #endif
    return Master_writeState(0,STATE_INIT);//all to init
}

int My_EthercatMaster::Master_run(){
//    OSAL_THREAD_HANDLE thread1;
//    osal_thread_create(&thread1, 128000, &ecatcheck, (void*) &ctime);
//    int chk = 40;
//    if(Master_readState(0) == STATE_INIT){
//        do{
//            Master_writeState(0,STATE_PRE_OP);
//            ec_statecheck(0, STATE_PRE_OP, EC_TIMEOUTRET*6);
//        }while(chk-- && Master_readState(0) != STATE_PRE_OP);
//    }
//    chk = 40;
//    if(Master_readState(0) == STATE_PRE_OP){
//        do{
//            Master_writeState(0,STATE_SAFE_OP);
//            ec_statecheck(0, STATE_SAFE_OP, EC_TIMEOUTRET*6);
//        }while(chk-- && Master_readState(0) != STATE_SAFE_OP);
//    }

////    qDebug() << Master_stateToString(Master_readState(0));
//    chk = 40;
//     Master_writeState(0,STATE_OPERATIONAL);
//    do
//    {
//    ec_send_processdata();
//    ec_receive_processdata(EC_TIMEOUTRET);
//    ec_statecheck(0, STATE_OPERATIONAL, 50000);
//    }while (chk-- && (ec_slave[0].state != STATE_OPERATIONAL));
//    //return Master_writeState(0,STATE_OPERATIONAL);//all to operation
//     #if USE_CHECK_THREAD
//     Check_inOP = 1;//for checkThread
//     #endif
//     return ec_statecheck(0, STATE_OPERATIONAL, 50000);
    int ret = Master_ChangeState(0,STATE_OPERATIONAL);
    #if USE_CHECK_THREAD
    Check_inOP = 1;//for checkThread
    #endif
    return ret;

}

///
/// \brief My_EthercatMaster::Master_scan
/// \return
///
int My_EthercatMaster::Master_scan()
{
    return Master_scan(m_adapterNameSelect);
}

///
/// \brief 扫描从站(Todo)
/// \param ifname
/// \return
///
int My_EthercatMaster::Master_scan(const QString &ifname){
    int cnt, i, j, nSM;
    uint16 ssigen;
    int expectedWKC;
//    bool printSDO = false;
//    bool printMAP = true;

    //printf("Starting slaveinfo\n");
    m_slaves_list->clear();
    memset(m_IOmap,0,sizeof(char)*IOMAP_SIZE);

    /* initialise SOEM, bind socket to ifname */
    if(Master_init(ifname)){
        if(Master_config(false) >0){
            Master_configDC();
            while(EcatError){
                //注释下面一句话，运行出错??
                //printf("%s", ec_elist2string());//打印错误信息知道无错误
                qDebug("%s", ec_elist2string());
            }
            //printf("%d slaves found and configured.\n",ec_slavecount);
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            //printf("Calculated workcounter %d\n", expectedWKC);

            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 3);

            if (ec_slave[0].state != EC_STATE_SAFE_OP )
            {
                qDebug("Not all slaves reached safe operational state.");
//               printf("Not all slaves reached safe operational state.\n");
               Master_readState_process();
               for(i = 1; i<=ec_slavecount ; i++)
               {
                  if(ec_slave[i].state != EC_STATE_SAFE_OP)
                  {
                      qDebug("Slave %d State=%2x StatusCode=%4x : %s\n",
                             i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
//                     printf("Slave %d State=%2x StatusCode=%4x : %s\n",
//                        i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                  }
               }
            }
            //release版本下,会出现无法进入到safeOP,所以检查之后再次请求一次
            //NOTE:如果之前没有正常关闭EtherCAT,下次运行的时候会出错，从而无法进入到安全运行模式
            Master_ChangeState(0,STATE_SAFE_OP);


//            qDebug() << "address_inputs:"<<ec_slave[1].inputs;
//            qDebug() << "address_outputs:"<<ec_slave[1].outputs;//outputs和m_IOmap地址一致
//            //NOTE: C++用void*打印数组的地址
//            qDebug() << "address_ioMap" << (void *)m_IOmap;
            #if USE_CHECK_THREAD
            Check_expectedWKC = Master_get_expectedWKC();
            #endif
            Master_readState_process();
            for( cnt = 1 ; cnt <= ec_slavecount ; cnt++)
            {
               Ethercat_Slave slave;
               slave.m_slave_index = cnt;
               slave.m_name = QString(QLatin1String(ec_slave[cnt].name));
               slave.m_Obits = ec_slave[cnt].Obits;
               slave.m_Ibits = ec_slave[cnt].Ibits;
               slave.m_eep_man = ec_slave[cnt].eep_man;
               slave.m_eep_id = ec_slave[cnt].eep_id;
               slave.m_eep_rev = ec_slave[cnt].eep_rev;

               m_slaves_list->append(slave);
#if 0
               printf("\nSlave:%d\n Name:%s\n Output size: %dbits\n Input size: %dbits\n State: %d\n Delay: %d[ns]\n Has DC: %d\n",
                     cnt, ec_slave[cnt].name, ec_slave[cnt].Obits, ec_slave[cnt].Ibits,
                     ec_slave[cnt].state, ec_slave[cnt].pdelay, ec_slave[cnt].hasdc);
               if (ec_slave[cnt].hasdc) printf(" DCParentport:%d\n", ec_slave[cnt].parentport);
               printf(" Activeports:%d.%d.%d.%d\n", (ec_slave[cnt].activeports & 0x01) > 0 ,
                                            (ec_slave[cnt].activeports & 0x02) > 0 ,
                                            (ec_slave[cnt].activeports & 0x04) > 0 ,
                                            (ec_slave[cnt].activeports & 0x08) > 0 );
               printf(" Configured address: %4.4x\n", ec_slave[cnt].configadr);
               printf(" Man: %8.8x ID: %8.8x Rev: %8.8x\n", (int)ec_slave[cnt].eep_man, (int)ec_slave[cnt].eep_id, (int)ec_slave[cnt].eep_rev);
               for(nSM = 0 ; nSM < EC_MAXSM ; nSM++)
               {
                  if(ec_slave[cnt].SM[nSM].StartAddr > 0)
                     printf(" SM%1d A:%4.4x L:%4d F:%8.8x Type:%d\n",nSM, ec_slave[cnt].SM[nSM].StartAddr, ec_slave[cnt].SM[nSM].SMlength,
                            (int)ec_slave[cnt].SM[nSM].SMflags, ec_slave[cnt].SMtype[nSM]);
               }
               for(j = 0 ; j < ec_slave[cnt].FMMUunused ; j++)
               {
                  printf(" FMMU%1d Ls:%8.8x Ll:%4d Lsb:%d Leb:%d Ps:%4.4x Psb:%d Ty:%2.2x Act:%2.2x\n", j,
                          (int)ec_slave[cnt].FMMU[j].LogStart, ec_slave[cnt].FMMU[j].LogLength, ec_slave[cnt].FMMU[j].LogStartbit,
                          ec_slave[cnt].FMMU[j].LogEndbit, ec_slave[cnt].FMMU[j].PhysStart, ec_slave[cnt].FMMU[j].PhysStartBit,
                          ec_slave[cnt].FMMU[j].FMMUtype, ec_slave[cnt].FMMU[j].FMMUactive);
               }
               printf(" FMMUfunc 0:%d 1:%d 2:%d 3:%d\n",
                        ec_slave[cnt].FMMU0func, ec_slave[cnt].FMMU1func, ec_slave[cnt].FMMU2func, ec_slave[cnt].FMMU3func);
               printf(" MBX length wr: %d rd: %d MBX protocols : %2.2x\n", ec_slave[cnt].mbx_l, ec_slave[cnt].mbx_rl, ec_slave[cnt].mbx_proto);
               ssigen = ec_siifind(cnt, ECT_SII_GENERAL);
               /* SII general section */
               if (ssigen)
               {
                  ec_slave[cnt].CoEdetails = ec_siigetbyte(cnt, ssigen + 0x07);
                  ec_slave[cnt].FoEdetails = ec_siigetbyte(cnt, ssigen + 0x08);
                  ec_slave[cnt].EoEdetails = ec_siigetbyte(cnt, ssigen + 0x09);
                  ec_slave[cnt].SoEdetails = ec_siigetbyte(cnt, ssigen + 0x0a);
                  if((ec_siigetbyte(cnt, ssigen + 0x0d) & 0x02) > 0)
                  {
                     ec_slave[cnt].blockLRW = 1;
                     ec_slave[0].blockLRW++;
                  }
                  ec_slave[cnt].Ebuscurrent = ec_siigetbyte(cnt, ssigen + 0x0e);
                  ec_slave[cnt].Ebuscurrent += ec_siigetbyte(cnt, ssigen + 0x0f) << 8;
                  ec_slave[0].Ebuscurrent += ec_slave[cnt].Ebuscurrent;
               }
               printf(" CoE details: %2.2x FoE details: %2.2x EoE details: %2.2x SoE details: %2.2x\n",
                       ec_slave[cnt].CoEdetails, ec_slave[cnt].FoEdetails, ec_slave[cnt].EoEdetails, ec_slave[cnt].SoEdetails);
               printf(" Ebus current: %d[mA]\n only LRD/LWR:%d\n",
                       ec_slave[cnt].Ebuscurrent, ec_slave[cnt].blockLRW);

               if ((ec_slave[cnt].mbx_proto & ECT_MBXPROT_COE) && printSDO){
                       si_sdo(cnt);
               }
               if(printMAP)
               {
                   if (ec_slave[cnt].mbx_proto & ECT_MBXPROT_COE){
                       si_map_sdo(cnt);//定义的变量的map输出
                   }
                   else{
                       //si_map_sii(cnt);
                   }
               }
#endif
               if(ec_slave[cnt].mbx_proto & ECT_MBXPROT_COE){
                   Master_siMap_sdo(cnt);
               }
            }//for
            //Master_close();
            //Master_writeState(0,STATE_INIT);//0表示all
            return ERROR_NONE;
        }
        else{
            Master_close();
            return ERROR_NO_SLAVE;
        }
    }
    else{
        return ERROR_NO_ADAPTER;
    }
}

///
/// \brief EtherCAT写
/// \return
///
int My_EthercatMaster::Master_processData_send(){
    return ec_send_processdata();
}

///
/// \brief EtherCAT读进程
/// \return
///
int My_EthercatMaster::Master_processData_receive(){
    return Master_processData_receive(EC_TIMEOUTRET);
}

///
/// \brief EtherCAT读进程
/// \param timeout
/// \return
///
int My_EthercatMaster::Master_processData_receive(int timeout){
//    volatile int wkc;
//    wkc = ec_receive_processdata(EC_TIMEOUTRET);

//    if(wkc >= expectedWKC){
//    }
     int wkc = ec_receive_processdata(timeout);
     #if USE_CHECK_THREAD
     Check_wkc = wkc;//for check Thread
     #endif
    return wkc;
}

///
/// \brief My_EthercatMaster::Master_process_loop
/// \return
///
bool My_EthercatMaster::Master_process_loop(){
    return Master_process_loop(true,STATE_OPERATIONAL,50000);
}

///
/// \brief My_EthercatMaster::Master_process_loop
/// \param check_state
/// \param check_timeOut 单位us
/// \return
///
bool My_EthercatMaster::Master_process_loop(bool isCheck, int check_state,int check_timeOut_us){
   int wkc = 0;
   Master_processData_send();
   wkc = Master_processData_receive();
   if(isCheck){
       ec_statecheck(0, check_state, check_timeOut_us);//check需要添加，否则运行态会变成安全运行态
   }
   if(wkc >= Master_get_expectedWKC()){
       return true;
   }
   else{
       return false;
   }
}

///
/// \brief EtherCAT读数据
/// \return
///
int My_EthercatMaster::Master_readState_process(){
    return ec_readstate();
}

///
/// \brief 获取扫描的从站的个数
/// \return
///
int My_EthercatMaster::Master_getSlaveCount(){
    return ec_slavecount;
}

///
/// \brief EtherCAT读取状态
/// \param slave_num
/// \param state
/// \return
///
int My_EthercatMaster::Master_writeState(int slave_num,int state){
    ec_slave[slave_num].state = state;
    return ec_writestate(slave_num);
}

///
/// \brief EtherCAT读取状态
/// \param slave_num
/// \return
///
int My_EthercatMaster::Master_readState(int slave_num){
    Master_readState_process();
    return ec_slave[slave_num].state;
}

int My_EthercatMaster::Master_ChangeState(int slave_num,int state_inquire){//根据状态机改变状态
    int chk = 40;
    int current_state;
//    int loop_num = 0;
    current_state= Master_readState(slave_num);//查询当前的状态
//    if(current_state == state_inquire){
//        return ec_statecheck(slave_num, state_inquire, 50000);
//    }
//    ec_group[0].docheckstate = TRUE;

    if(current_state >= state_inquire){//逆向改变，直接请求即可
        Master_writeState(slave_num,state_inquire);
//        ec_statecheck(slave_num, state_inquire, EC_TIMEOUTSTATE);
        return ec_statecheck(slave_num, state_inquire, EC_TIMEOUTSTATE * 3);
    }
    else{
        do{
            chk = 40;
            if(Master_readState(slave_num) == STATE_INIT){
                do{
                    Master_writeState(slave_num,STATE_PRE_OP);
//                    ec_send_processdata();
//                    ec_receive_processdata(EC_TIMEOUTRET);//这两行要加上？？
                    ec_statecheck(slave_num, STATE_PRE_OP, EC_TIMEOUTRET);
                }while(chk-- && Master_readState(slave_num) != STATE_PRE_OP);
            }
            if(state_inquire == STATE_PRE_OP) break;

            chk = 40;
            if(Master_readState(slave_num) == STATE_PRE_OP){
                do{
                    Master_writeState(slave_num,STATE_SAFE_OP);
//                    ec_send_processdata();
//                    ec_receive_processdata(EC_TIMEOUTRET);//这两行要加上？？
                    ec_statecheck(slave_num, STATE_SAFE_OP, EC_TIMEOUTRET);
                }while(chk-- && Master_readState(slave_num) != STATE_SAFE_OP);
            }
            if(state_inquire == STATE_SAFE_OP) break;

            //Note:safeOp->Op如果没有检查线程，无法转换为运行态(因为在运行态中必须要有数据循环，失去了数据循环就会变成safeOp)
            chk = 40;
            if(Master_readState(slave_num) == STATE_SAFE_OP){
                do
                {
                    Master_writeState(slave_num,STATE_OPERATIONAL);
                    ec_send_processdata();//运行态的时候需要接收数据，否则不会响应状态转换
//                    ec_receive_processdata(EC_TIMEOUTRET);//这两行要加上？？
                    ec_statecheck(slave_num, STATE_OPERATIONAL, 50000);
                }while (chk-- && (Master_readState(slave_num) != STATE_OPERATIONAL));
             }
//             if(state_inquire == STATE_OPERATIONAL) break;

        }while(0);
    }//else

    return ec_statecheck(slave_num, state_inquire, EC_TIMEOUTRET*3);
}

int My_EthercatMaster::Master_CheckState(int slave_num,int state,int check_timeOut_us ){
    return ec_statecheck(slave_num, state, check_timeOut_us);
}

///
/// \brief 获取目标WKC
/// \return
///
int My_EthercatMaster::Master_get_expectedWKC(){
    int expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
    return expectedWKC;
}

/** CoE SDO write, blocking. Single subindex or Complete Access.
 *
 * A "normal" download request is issued, unless we have
 * small data, then a "expedited" transfer is used. If the parameter is larger than
 * the mailbox size then the download is segmented. The function will split the
 * parameter data in segments and send them to the slave one by one.
 *
 * @param[in]  Slave      = Slave number
 * @param[in]  Index      = Index to write
 * @param[in]  SubIndex   = Subindex to write, must be 0 or 1 if CA is used.
 * @param[in]  CA         = FALSE = single subindex. TRUE = Complete Access, all subindexes written.
 * @param[in]  psize      = Size in bytes of parameter buffer.
 * @param[out] p          = Pointer to parameter buffer
 * @param[in]  Timeout    = Timeout in us, standard is EC_TIMEOUTRXM
 * @return Workcounter from last slave response
 * @see ecx_SDOwrite
 */
int My_EthercatMaster::Master_SDOWrite(uint16_t Slave, uint16_t Index, uint8_t SubIndex,
                    bool CA, int psize, void *p, int Timeout){

   return ec_SDOwrite(Slave,Index,SubIndex,CA,psize,p,Timeout);
}

/** CoE SDO read, blocking. Single subindex or Complete Access.
 *
 * Only a "normal" upload request is issued. If the requested parameter is <= 4bytes
 * then a "expedited" response is returned, otherwise a "normal" response. If a "normal"
 * response is larger than the mailbox size then the response is segmented. The function
 * will combine all segments and copy them to the parameter buffer.
 *
 * @param[in]  slave      = Slave number
 * @param[in]  index      = Index to read
 * @param[in]  subindex   = Subindex to read, must be 0 or 1 if CA is used.
 * @param[in]  CA         = FALSE = single subindex. TRUE = Complete Access, all subindexes read.
 * @param[in,out] psize   = Size in bytes of parameter buffer, returns bytes read from SDO.
 * @param[out] p          = Pointer to parameter buffer
 * @param[in]  timeout    = Timeout in us, standard is EC_TIMEOUTRXM
 * @return Workcounter from last slave response
 * @see ecx_SDOread
 */
int My_EthercatMaster::Master_SDORead(uint16_t slave, uint16_t index, uint8_t subindex,
                   bool CA, int *psize, void *p, int timeout){
     return ec_SDOread(slave,index,subindex,CA,psize,p,timeout);
}

#if USE_CHECK_THREAD
///
/// \brief My_EthercatMaster::Master_InitCheckThread
///
void My_EthercatMaster::Master_InitCheckThread(){
   My_EcInit_check();
}

///
/// \brief My_EthercatMaster::Master_isCheckThread
/// \param isCheck
///
void My_EthercatMaster::Master_isCheckThread(bool isCheck){
   IsThreadCheck = isCheck? 1:0;
}
#endif

///
/// \brief 获取IO_map基地址(其地址与Outputs地址相同)
/// \return
///
char* My_EthercatMaster::Master_getAddressBase(){
    return m_IOmap;
}

///
/// \brief My_EthercatMaster::Master_usSleep(静态函数)
/// \param usec
///
void My_EthercatMaster::Master_usSleep(uint32_t usec){
    osal_usleep(usec);
}



/**************************  Private  ******************************************/

int My_EthercatMaster::Master_siMap_sdo(int slave)
{
    int wkc, rdl;
    int retVal = 0;
    uint8 nSM, iSM, tSM;
    int Tsize, outputs_bo, inputs_bo;
    uint8 SMt_bug_add;

//    printf("PDO mapping according to CoE :\n");
    SMt_bug_add = 0;
    outputs_bo = 0;
    inputs_bo = 0;
    rdl = sizeof(nSM); nSM = 0;
    /* read SyncManager Communication Type object count */
    wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, 0x00, FALSE, &rdl, &nSM, EC_TIMEOUTRXM);
    /* positive result from slave ? */
    if ((wkc > 0) && (nSM > 2))
    {
        /* make nSM equal to number of defined SM */
        nSM--;
        /* limit to maximum number of SM defined, if true the slave can't be configured */
        if (nSM > EC_MAXSM)
            nSM = EC_MAXSM;
        /* iterate for every SM type defined */
        for (iSM = 2 ; iSM <= nSM ; iSM++)
        {
            rdl = sizeof(tSM); tSM = 0;
            /* read SyncManager Communication Type */
            wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, iSM + 1, FALSE, &rdl, &tSM, EC_TIMEOUTRXM);
            if (wkc > 0)
            {
                if((iSM == 2) && (tSM == 2)) // SM2 has type 2 == mailbox out, this is a bug in the slave!
                {
                    SMt_bug_add = 1; // try to correct, this works if the types are 0 1 2 3 and should be 1 2 3 4
                    printf("Activated SM type workaround, possible incorrect mapping.\n");
                }
                if(tSM)
                    tSM += SMt_bug_add; // only add if SMt > 0

                if (tSM == 3) // outputs
                {
                    /* read the assign RXPDO */
                    //printf("  SM%1d outputs\n     addr b   index: sub bitl data_type    name\n", iSM);//ism=2
                    //Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].outputs - (uint8 *)&m_IOmap[0]), outputs_bo );
                    Tsize = Master_si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].outputs - (uint8 *)&m_IOmap[0]), outputs_bo );
                    outputs_bo += Tsize;
                }
                if (tSM == 4) // inputs
                {
                    /* read the assign TXPDO */
                    //printf("  SM%1d inputs\n     addr b   index: sub bitl data_type    name\n", iSM);//ism=3
                    //Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].inputs - (uint8 *)&m_IOmap[0]), inputs_bo );
                    Tsize = Master_si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].inputs - (uint8 *)&m_IOmap[0]), inputs_bo );
                    inputs_bo += Tsize;
                }
            }
        }
    }

    /* found some I/O bits ? */
    if ((outputs_bo > 0) || (inputs_bo > 0))
        retVal = 1;
    return retVal;
}

/** Read PDO assign structure */
int My_EthercatMaster::Master_si_PDOassign(uint16_t slave, uint16_t PDOassign, int mapoffset, int bitoffset)
{
    uint16 idxloop, nidx, subidxloop, rdat, idx, subidx;
    uint8 subcnt;
    int wkc, bsize = 0, rdl;
    int32 rdat2;
    uint8 bitlen, obj_subidx;
    uint16 obj_idx;
    int abs_offset, abs_bit;

    uint16_t sm_index = PDOassign - ECT_SDO_PDOASSIGN;
    Ethercat_Slave slave_select = m_slaves_list->at(slave-1);

    rdl = sizeof(rdat); rdat = 0;
    /* read PDO assign subindex 0 ( = number of PDO's) */
    wkc = ec_SDOread(slave, PDOassign, 0x00, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
    rdat = etohs(rdat);
    /* positive result from slave ? */
    if ((wkc > 0) && (rdat > 0))
    {
        /* number of available sub indexes */
        nidx = rdat;
        bsize = 0;
        /* read all PDO's */
        for (idxloop = 1; idxloop <= nidx; idxloop++)
        {
            rdl = sizeof(rdat); rdat = 0;
            /* read PDO assign */
            wkc = ec_SDOread(slave, PDOassign, (uint8)idxloop, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
            /* result is index of PDO */
            idx = etohl(rdat);
            if (idx > 0)
            {
                rdl = sizeof(subcnt); subcnt = 0;
                /* read number of subindexes of PDO */
                wkc = ec_SDOread(slave,idx, 0x00, FALSE, &rdl, &subcnt, EC_TIMEOUTRXM);
                subidx = subcnt;
                /* for each subindex */
                for (subidxloop = 1; subidxloop <= subidx; subidxloop++)
                {
                    rdl = sizeof(rdat2); rdat2 = 0;
                    /* read SDO that is mapped in PDO */
                    wkc = ec_SDOread(slave, idx, (uint8)subidxloop, FALSE, &rdl, &rdat2, EC_TIMEOUTRXM);
                    rdat2 = etohl(rdat2);
                    /* extract bitlength of SDO */
                    bitlen = LO_BYTE(rdat2);
                    bsize += bitlen;
                    obj_idx = (uint16)(rdat2 >> 16);
                    obj_subidx = (uint8)((rdat2 >> 8) & 0x000000ff);
                    abs_offset = mapoffset + (bitoffset / 8);
                    abs_bit = bitoffset % 8;
                    ODlist.Slave = slave;
                    ODlist.Index[0] = obj_idx;
                    OElist.Entries = 0;
                    wkc = 0;
                    /* read object entry from dictionary if not a filler (0x0000:0x00) */
                    if(obj_idx || obj_subidx)
                        wkc = ec_readOEsingle(0, obj_subidx, &ODlist, &OElist);
                    //printf("  [0x%4.4X.%1d] 0x%4.4X:0x%2.2X 0x%2.2X", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
                    Ethercat_SlaveMSG_Item itemSlaveMSG;
                    itemSlaveMSG.m_slave_num = slave;
                    itemSlaveMSG.m_sm_num = sm_index;
                    itemSlaveMSG.m_addr_offset = abs_offset;
                    itemSlaveMSG.m_addr_bit = abs_bit;
                    itemSlaveMSG.m_obj_index = obj_idx;
                    itemSlaveMSG.m_obj_index_sub = obj_subidx;
                    itemSlaveMSG.m_item_bitLen = bitlen;

                    if((wkc > 0) && OElist.Entries)
                    {
                        //printf(" %-12s %s\n", dtype2string(OElist.DataType[obj_subidx]), OElist.Name[obj_subidx]);
                        itemSlaveMSG.m_date_type = OElist.DataType[obj_subidx];
                        itemSlaveMSG.m_name = QString(QLatin1String(OElist.Name[obj_subidx]));

                        if(sm_index == 2){//outputs
                            slave_select.output_list.append(itemSlaveMSG);
                            m_slaves_list->replace(slave-1,slave_select);
                            //NOTE:不能连着进行转换，否则出错
                            //slaves_list.at(0).output_list.append(itemSlaveMSG);
                        }
                        else if (sm_index == 3){// inputs
                            slave_select.input_list.append(itemSlaveMSG);
                            m_slaves_list->replace(slave-1,slave_select);
                            //slaves_list.at(0).input_list.append(itemSlaveMSG);
                        }
                    }
                    else{
                       //printf("\n");

                    }
                    bitoffset += bitlen;
                };
            };
        };
    };
    /* return total found bitlength (PDO) */
    return bsize;
}

char* My_EthercatMaster::dtype2string(uint16 dtype)
{
    switch(dtype)
    {
        case ECT_BOOLEAN:
            sprintf(hstr, "BOOLEAN");
            break;
        case ECT_INTEGER8:
            sprintf(hstr, "INTEGER8");
            break;
        case ECT_INTEGER16:
            sprintf(hstr, "INTEGER16");
            break;
        case ECT_INTEGER32:
            sprintf(hstr, "INTEGER32");
            break;
        case ECT_INTEGER24:
            sprintf(hstr, "INTEGER24");
            break;
        case ECT_INTEGER64:
            sprintf(hstr, "INTEGER64");
            break;
        case ECT_UNSIGNED8:
            sprintf(hstr, "UNSIGNED8");
            break;
        case ECT_UNSIGNED16:
            sprintf(hstr, "UNSIGNED16");
            break;
        case ECT_UNSIGNED32:
            sprintf(hstr, "UNSIGNED32");
            break;
        case ECT_UNSIGNED24:
            sprintf(hstr, "UNSIGNED24");
            break;
        case ECT_UNSIGNED64:
            sprintf(hstr, "UNSIGNED64");
            break;
        case ECT_REAL32:
            sprintf(hstr, "REAL32");
            break;
        case ECT_REAL64:
            sprintf(hstr, "REAL64");
            break;
        case ECT_BIT1:
            sprintf(hstr, "BIT1");
            break;
        case ECT_BIT2:
            sprintf(hstr, "BIT2");
            break;
        case ECT_BIT3:
            sprintf(hstr, "BIT3");
            break;
        case ECT_BIT4:
            sprintf(hstr, "BIT4");
            break;
        case ECT_BIT5:
            sprintf(hstr, "BIT5");
            break;
        case ECT_BIT6:
            sprintf(hstr, "BIT6");
            break;
        case ECT_BIT7:
            sprintf(hstr, "BIT7");
            break;
        case ECT_BIT8:
            sprintf(hstr, "BIT8");
            break;
        case ECT_VISIBLE_STRING:
            sprintf(hstr, "VISIBLE_STRING");
            break;
        case ECT_OCTET_STRING:
            sprintf(hstr, "OCTET_STRING");
            break;
        default:
            sprintf(hstr, "Type 0x%4.4X", dtype);
    }
    return hstr;
}

char* My_EthercatMaster::SDO2string(uint16_t slave, uint16_t index, uint8_t subidx, uint16_t dtype)
{
   int l = sizeof(usdo) - 1, i;
   uint8 *u8;
   int8 *i8;
   uint16 *u16;
   int16 *i16;
   uint32 *u32;
   int32 *i32;
   uint64 *u64;
   int64 *i64;
   float *sr;
   double *dr;
   char es[32];

   memset(&usdo, 0, 128);
   ec_SDOread(slave, index, subidx, FALSE, &l, &usdo, EC_TIMEOUTRXM);
   if (EcatError)
   {
      return ec_elist2string();
   }
   else
   {
      switch(dtype)
      {
         case ECT_BOOLEAN:
            u8 = (uint8*) &usdo[0];
            if (*u8) sprintf(hstr, "TRUE");
             else sprintf(hstr, "FALSE");
            break;
         case ECT_INTEGER8:
            i8 = (int8*) &usdo[0];
            sprintf(hstr, "0x%2.2x %d", *i8, *i8);
            break;
         case ECT_INTEGER16:
            i16 = (int16*) &usdo[0];
            sprintf(hstr, "0x%4.4x %d", *i16, *i16);
            break;
         case ECT_INTEGER32:
         case ECT_INTEGER24:
            i32 = (int32*) &usdo[0];
            sprintf(hstr, "0x%8.8x %d", *i32, *i32);
            break;
         case ECT_INTEGER64:
            i64 = (int64*) &usdo[0];
//            sprintf(hstr, "0x%16.16"PRIx64" %"PRId64, *i64, *i64);
            break;
         case ECT_UNSIGNED8:
            u8 = (uint8*) &usdo[0];
            sprintf(hstr, "0x%2.2x %u", *u8, *u8);
            break;
         case ECT_UNSIGNED16:
            u16 = (uint16*) &usdo[0];
            sprintf(hstr, "0x%4.4x %u", *u16, *u16);
            break;
         case ECT_UNSIGNED32:
         case ECT_UNSIGNED24:
            u32 = (uint32*) &usdo[0];
            sprintf(hstr, "0x%8.8x %u", *u32, *u32);
            break;
         case ECT_UNSIGNED64:
            u64 = (uint64*) &usdo[0];
//            sprintf(hstr, "0x%16.16"PRIx64" %"PRIu64, *u64, *u64);
            break;
         case ECT_REAL32:
            sr = (float*) &usdo[0];
            sprintf(hstr, "%f", *sr);
            break;
         case ECT_REAL64:
            dr = (double*) &usdo[0];
            sprintf(hstr, "%f", *dr);
            break;
         case ECT_BIT1:
         case ECT_BIT2:
         case ECT_BIT3:
         case ECT_BIT4:
         case ECT_BIT5:
         case ECT_BIT6:
         case ECT_BIT7:
         case ECT_BIT8:
            u8 = (uint8*) &usdo[0];
            sprintf(hstr, "0x%x", *u8);
            break;
         case ECT_VISIBLE_STRING:
            strcpy(hstr, usdo);
            break;
         case ECT_OCTET_STRING:
            hstr[0] = 0x00;
            for (i = 0 ; i < l ; i++)
            {
               sprintf(es, "0x%2.2x ", usdo[i]);
               strcat( hstr, es);
            }
            break;
         default:
            sprintf(hstr, "Unknown type");
      }
      return hstr;
   }
}

/** Read PDO assign structure */
int My_EthercatMaster::si_PDOassign(uint16_t slave, uint16_t PDOassign, int mapoffset, int bitoffset)
{
    uint16 idxloop, nidx, subidxloop, rdat, idx, subidx;
    uint8 subcnt;
    int wkc, bsize = 0, rdl;
    int32 rdat2;
    uint8 bitlen, obj_subidx;
    uint16 obj_idx;
    int abs_offset, abs_bit;

    rdl = sizeof(rdat); rdat = 0;
    /* read PDO assign subindex 0 ( = number of PDO's) */
    wkc = ec_SDOread(slave, PDOassign, 0x00, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
    rdat = etohs(rdat);
    /* positive result from slave ? */
    if ((wkc > 0) && (rdat > 0))
    {
        /* number of available sub indexes */
        nidx = rdat;
        bsize = 0;
        /* read all PDO's */
        for (idxloop = 1; idxloop <= nidx; idxloop++)
        {
            rdl = sizeof(rdat); rdat = 0;
            /* read PDO assign */
            wkc = ec_SDOread(slave, PDOassign, (uint8)idxloop, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
            /* result is index of PDO */
            idx = etohl(rdat);
            if (idx > 0)
            {
                rdl = sizeof(subcnt); subcnt = 0;
                /* read number of subindexes of PDO */
                wkc = ec_SDOread(slave,idx, 0x00, FALSE, &rdl, &subcnt, EC_TIMEOUTRXM);
                subidx = subcnt;
                /* for each subindex */
                for (subidxloop = 1; subidxloop <= subidx; subidxloop++)
                {
                    rdl = sizeof(rdat2); rdat2 = 0;
                    /* read SDO that is mapped in PDO */
                    wkc = ec_SDOread(slave, idx, (uint8)subidxloop, FALSE, &rdl, &rdat2, EC_TIMEOUTRXM);
                    rdat2 = etohl(rdat2);
                    /* extract bitlength of SDO */
                    bitlen = LO_BYTE(rdat2);
                    bsize += bitlen;
                    obj_idx = (uint16)(rdat2 >> 16);
                    obj_subidx = (uint8)((rdat2 >> 8) & 0x000000ff);
                    abs_offset = mapoffset + (bitoffset / 8);
                    abs_bit = bitoffset % 8;
                    ODlist.Slave = slave;
                    ODlist.Index[0] = obj_idx;
                    OElist.Entries = 0;
                    wkc = 0;
                    /* read object entry from dictionary if not a filler (0x0000:0x00) */
                    if(obj_idx || obj_subidx)
                        wkc = ec_readOEsingle(0, obj_subidx, &ODlist, &OElist);
                    printf("  [0x%4.4X.%1d] 0x%4.4X:0x%2.2X 0x%2.2X", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
                    if((wkc > 0) && OElist.Entries)
                    {
                        printf(" %-12s %s\n", dtype2string(OElist.DataType[obj_subidx]), OElist.Name[obj_subidx]);
                    }
                    else
                        printf("\n");
                    bitoffset += bitlen;
                };
            };
        };
    };
    /* return total found bitlength (PDO) */
    return bsize;
}


int My_EthercatMaster::si_map_sdo(int slave)
{
    int wkc, rdl;
    int retVal = 0;
    uint8 nSM, iSM, tSM;
    int Tsize, outputs_bo, inputs_bo;
    uint8 SMt_bug_add;

    printf("PDO mapping according to CoE :\n");
    SMt_bug_add = 0;
    outputs_bo = 0;
    inputs_bo = 0;
    rdl = sizeof(nSM); nSM = 0;
    /* read SyncManager Communication Type object count */
    wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, 0x00, FALSE, &rdl, &nSM, EC_TIMEOUTRXM);
    /* positive result from slave ? */
    if ((wkc > 0) && (nSM > 2))
    {
        /* make nSM equal to number of defined SM */
        nSM--;
        /* limit to maximum number of SM defined, if true the slave can't be configured */
        if (nSM > EC_MAXSM)
            nSM = EC_MAXSM;
        /* iterate for every SM type defined */
        for (iSM = 2 ; iSM <= nSM ; iSM++)
        {
            rdl = sizeof(tSM); tSM = 0;
            /* read SyncManager Communication Type */
            wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, iSM + 1, FALSE, &rdl, &tSM, EC_TIMEOUTRXM);
            if (wkc > 0)
            {
                if((iSM == 2) && (tSM == 2)) // SM2 has type 2 == mailbox out, this is a bug in the slave!
                {
                    SMt_bug_add = 1; // try to correct, this works if the types are 0 1 2 3 and should be 1 2 3 4
                    printf("Activated SM type workaround, possible incorrect mapping.\n");
                }
                if(tSM)
                    tSM += SMt_bug_add; // only add if SMt > 0

                if (tSM == 3) // outputs
                {
                    /* read the assign RXPDO */
                    printf("  SM%1d outputs\n     addr b   index: sub bitl data_type    name\n", iSM);//ism=2
                    //Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].outputs - (uint8 *)&m_IOmap[0]), outputs_bo );
                    Tsize = Master_si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].outputs - (uint8 *)&m_IOmap[0]), outputs_bo );
                    outputs_bo += Tsize;
                }
                if (tSM == 4) // inputs
                {
                    /* read the assign TXPDO */
                    printf("  SM%1d inputs\n     addr b   index: sub bitl data_type    name\n", iSM);//ism=3
                    //Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].inputs - (uint8 *)&m_IOmap[0]), inputs_bo );
                    Tsize = Master_si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].inputs - (uint8 *)&m_IOmap[0]), inputs_bo );
                    inputs_bo += Tsize;
                }
            }
        }
    }

    /* found some I/O bits ? */
    if ((outputs_bo > 0) || (inputs_bo > 0))
        retVal = 1;
    return retVal;
}

int My_EthercatMaster::si_siiPDO(uint16_t slave, uint8_t t, int mapoffset, int bitoffset)
{
    uint16 a , w, c, e, er, Size;
    uint8 eectl;
    uint16 obj_idx;
    uint8 obj_subidx;
    uint8 obj_name;
    uint8 obj_datatype;
    uint8 bitlen;
    int totalsize;
    ec_eepromPDOt eepPDO;
    ec_eepromPDOt *PDO;
    int abs_offset, abs_bit;
    char str_name[EC_MAXNAME + 1];

    eectl = ec_slave[slave].eep_pdi;
    Size = 0;
    totalsize = 0;
    PDO = &eepPDO;
    PDO->nPDO = 0;
    PDO->Length = 0;
    PDO->Index[1] = 0;
    for (c = 0 ; c < EC_MAXSM ; c++) PDO->SMbitsize[c] = 0;
    if (t > 1)
        t = 1;
    PDO->Startpos = ec_siifind(slave, ECT_SII_PDO + t);
    if (PDO->Startpos > 0)
    {
        a = PDO->Startpos;
        w = ec_siigetbyte(slave, a++);
        w += (ec_siigetbyte(slave, a++) << 8);
        PDO->Length = w;
        c = 1;
        /* traverse through all PDOs */
        do
        {
            PDO->nPDO++;
            PDO->Index[PDO->nPDO] = ec_siigetbyte(slave, a++);
            PDO->Index[PDO->nPDO] += (ec_siigetbyte(slave, a++) << 8);
            PDO->BitSize[PDO->nPDO] = 0;
            c++;
            /* number of entries in PDO */
            e = ec_siigetbyte(slave, a++);
            PDO->SyncM[PDO->nPDO] = ec_siigetbyte(slave, a++);
            a++;
            obj_name = ec_siigetbyte(slave, a++);
            a += 2;
            c += 2;
            if (PDO->SyncM[PDO->nPDO] < EC_MAXSM) /* active and in range SM? */
            {
                str_name[0] = 0;
                if(obj_name)
                  ec_siistring(str_name, slave, obj_name);
                if (t)
                  printf("  SM%1d RXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
                else
                  printf("  SM%1d TXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
                printf("     addr b   index: sub bitl data_type    name\n");
                /* read all entries defined in PDO */
                for (er = 1; er <= e; er++)
                {
                    c += 4;
                    obj_idx = ec_siigetbyte(slave, a++);
                    obj_idx += (ec_siigetbyte(slave, a++) << 8);
                    obj_subidx = ec_siigetbyte(slave, a++);
                    obj_name = ec_siigetbyte(slave, a++);
                    obj_datatype = ec_siigetbyte(slave, a++);
                    bitlen = ec_siigetbyte(slave, a++);
                    abs_offset = mapoffset + (bitoffset / 8);
                    abs_bit = bitoffset % 8;

                    PDO->BitSize[PDO->nPDO] += bitlen;
                    a += 2;

                    str_name[0] = 0;
                    if(obj_name)
                      ec_siistring(str_name, slave, obj_name);

                    printf("  [0x%4.4X.%1d] 0x%4.4X:0x%2.2X 0x%2.2X", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
                    printf(" %-12s %s\n", dtype2string(obj_datatype), str_name);
                    bitoffset += bitlen;
                    totalsize += bitlen;
                }
                PDO->SMbitsize[ PDO->SyncM[PDO->nPDO] ] += PDO->BitSize[PDO->nPDO];
                Size += PDO->BitSize[PDO->nPDO];
                c++;
            }
            else /* PDO deactivated because SM is 0xff or > EC_MAXSM */
            {
                c += 4 * e;
                a += 8 * e;
                c++;
            }
            if (PDO->nPDO >= (EC_MAXEEPDO - 1)) c = PDO->Length; /* limit number of PDO entries in buffer */
        }
        while (c < PDO->Length);
    }
    if (eectl) ec_eeprom2pdi(slave); /* if eeprom control was previously pdi then restore */
    return totalsize;
}


int My_EthercatMaster::si_map_sii(int slave)
{
    int retVal = 0;
    int Tsize, outputs_bo, inputs_bo;

    printf("PDO mapping according to SII :\n");

    outputs_bo = 0;
    inputs_bo = 0;
    /* read the assign RXPDOs */
    Tsize = si_siiPDO(slave, 1, (int)(ec_slave[slave].outputs - (uint8*)&m_IOmap), outputs_bo );
    outputs_bo += Tsize;
    /* read the assign TXPDOs */
    Tsize = si_siiPDO(slave, 0, (int)(ec_slave[slave].inputs - (uint8*)&m_IOmap), inputs_bo );
    inputs_bo += Tsize;
    /* found some I/O bits ? */
    if ((outputs_bo > 0) || (inputs_bo > 0))
        retVal = 1;
    return retVal;
}

void My_EthercatMaster::si_sdo(int cnt)
{
    int i, j;

    ODlist.Entries = 0;
    memset(&ODlist, 0, sizeof(ODlist));
    if( ec_readODlist(cnt, &ODlist))
    {
        printf(" CoE Object Description found, %d entries.\n",ODlist.Entries);
        for( i = 0 ; i < ODlist.Entries ; i++)
        {
            ec_readODdescription(i, &ODlist);
            while(EcatError) printf("%s", ec_elist2string());
            printf(" Index: %4.4x Datatype: %4.4x Objectcode: %2.2x Name: %s\n",
                ODlist.Index[i], ODlist.DataType[i], ODlist.ObjectCode[i], ODlist.Name[i]);
            memset(&OElist, 0, sizeof(OElist));
            ec_readOE(i, &ODlist, &OElist);
            while(EcatError) printf("%s", ec_elist2string());
            for( j = 0 ; j < ODlist.MaxSub[i]+1 ; j++)
            {
                if ((OElist.DataType[j] > 0) && (OElist.BitLength[j] > 0))
                {
                    printf("  Sub: %2.2x Datatype: %4.4x Bitlength: %4.4x Obj.access: %4.4x Name: %s\n",
                        j, OElist.DataType[j], OElist.BitLength[j], OElist.ObjAccess[j], OElist.Name[j]);
                    if ((OElist.ObjAccess[j] & 0x0007))
                    {
                        printf("          Value :%s\n", SDO2string(cnt, ODlist.Index[i], j, OElist.DataType[j]));
                    }
                }
            }
        }
    }
    else
    {
        while(EcatError) printf("%s", ec_elist2string());
    }
}

#if 0

/******************** Ethercat_SlaveMSG_Item **************************************/

const QString Ethercat_SlaveMSG_Item::dataType_Tostring()
{
    QString str = "";
    uint16_t dtype = m_date_type;

    switch(dtype)
    {
        case ECT_BOOLEAN:
            str = "BOOLEAN";
            break;
        case ECT_INTEGER8:
            str = "INTEGER8";
            break;
        case ECT_INTEGER16:
            str = "INTEGER16";
            break;
        case ECT_INTEGER32:
            str = "INTEGER32";
            break;
        case ECT_INTEGER24:
            str = "INTEGER24";
            break;
        case ECT_INTEGER64:
            str = "INTEGER64";
            break;
        case ECT_UNSIGNED8:
            str = "UNSIGNED8";
            break;
        case ECT_UNSIGNED16:
            str = "UNSIGNED16";
            break;
        case ECT_UNSIGNED32:
            str = "UNSIGNED32";
            break;
        case ECT_UNSIGNED24:
            str = "UNSIGNED24";
            break;
        case ECT_UNSIGNED64:
            str = "UNSIGNED64";
            break;
        case ECT_REAL32:
            str = "REAL32";
            break;
        case ECT_REAL64:
            str = "REAL64";
            break;
        case ECT_BIT1:
            str = "BIT1";
            break;
        case ECT_BIT2:
            str = "BIT2";
            break;
        case ECT_BIT3:
            str = "BIT3";
            break;
        case ECT_BIT4:
            str = "BIT4";
            break;
        case ECT_BIT5:
            str = "BIT5";
            break;
        case ECT_BIT6:
            str = "BIT6";
            break;
        case ECT_BIT7:
            str = "BIT7";
            break;
        case ECT_BIT8:
            str = "BIT8";
            break;
        case ECT_VISIBLE_STRING:
            str = "VISIBLE_STRING";
            break;
        case ECT_OCTET_STRING:
            str = "OCTET_STRING";
            break;
        default:
            str = "User Type 0x"+QString::number(dtype,16);
    }
    return str;
}

const QStringList Ethercat_SlaveMSG_Item::dump_data(bool isDebug)
{
    QStringList str_list;
    QString str;
    QString str_debug;

    str_debug = isDebug == true? "slave_num=" : "";
    str = QString::number(m_slave_num);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "sm_num=" : "";
    str = QString::number(m_sm_num);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "addr_offset=" : "";
    str = "0x"+QString::number(m_addr_offset,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "addr_bit=" : "";
    str = "0x"+QString::number(m_addr_bit,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "obj_index=" : "";
    str = "0x"+QString::number(m_obj_index,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "obj_index_sub_num=" : "";
    str = "0x"+QString::number(m_obj_index_sub,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "item_bitLen=" : "";
    str = "0x"+QString::number(m_item_bitLen,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "dataType=" : "";
    str = dataType_Tostring();
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "name=" : "";
    str = m_name;
    str_list.append(str_debug+str);

    return str_list;
}

/***********************  Ethercat_SlaveMSG_Item END  *********************************/

/******************** Ethercat_Slave **************************************/
const QStringList Ethercat_Slave::dump_data(bool isDebug)
{
    QStringList str_list;
    QString str;
    QString str_debug;

    str_debug = isDebug == true? "slave_index=" : "";
    str = QString::number(m_slave_index);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "name=" : "";
    str = m_name;
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_Obits=" : "";
    str = "0x"+QString::number(m_Obits,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_Ibits=" : "";
    str = "0x"+QString::number(m_Ibits,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_eep_man=" : "";
    str = "0x"+QString::number(m_eep_man,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_eep_id=" : "";
    str = "0x"+QString::number(m_eep_id,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_eep_rev=" : "";
    str = "0x"+QString::number(m_eep_rev,16);
    str_list.append(str_debug+str);

    return str_list;
}
/***********************  Ethercat_Slave END  *********************************/
#endif


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(User_plugins, UserApp_plugin)
#endif // QT_VERSION < 0x050000
