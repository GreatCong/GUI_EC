#include "master_check.h"
#include "ethercat.h"

#define EC_TIMEOUTMON 500

OSAL_THREAD_HANDLE Thread_check = 0;
boolean needlf;

uint8 currentgroup = 0;

/* extern */
char IsThreadCheck = 0;
char Check_inOP = 0;
volatile int Check_wkc;
int Check_expectedWKC;
/* extern end */

//DWORD WINAPI ecatcheck( LPVOID lpParam )
OSAL_THREAD_FUNC ecatcheck(void *lpParam)
{
    int slave;

    while(1)
    {
        if(IsThreadCheck){
            if( Check_inOP && ((Check_wkc < Check_expectedWKC) || ec_group[currentgroup].docheckstate))
            {
                if (needlf)
                {
                   needlf = FALSE;
                   printf("\n");
                }
                /* one ore more slaves are not responding */
                ec_group[currentgroup].docheckstate = FALSE;
                ec_readstate();
                for (slave = 1; slave <= ec_slavecount; slave++)
                {
                   if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
                   {
                      ec_group[currentgroup].docheckstate = TRUE;
                      if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                      {
                         printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                         ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                         ec_writestate(slave);
                      }
                      else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                      {
                         printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                         ec_slave[slave].state = EC_STATE_OPERATIONAL;
                         ec_writestate(slave);
                      }
                      else if(ec_slave[slave].state > EC_STATE_NONE)
                      {
                         if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                         {
                            ec_slave[slave].islost = FALSE;
                            printf("MESSAGE : slave %d reconfigured\n",slave);
                         }
                      }
                      else if(!ec_slave[slave].islost)
                      {
                         /* re-check state */
                         ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                         if (ec_slave[slave].state == EC_STATE_NONE)
                         {
                            ec_slave[slave].islost = TRUE;
                            printf("ERROR : slave %d lost\n",slave);
                         }
                      }
                   }
                   if (ec_slave[slave].islost)
                   {
                      if(ec_slave[slave].state == EC_STATE_NONE)
                      {
                         if (ec_recover_slave(slave, EC_TIMEOUTMON))
                         {
                            ec_slave[slave].islost = FALSE;
                            printf("MESSAGE : slave %d recovered\n",slave);
                         }
                      }
                      else
                      {
                         ec_slave[slave].islost = FALSE;
                         printf("MESSAGE : slave %d found\n",slave);
                      }
                   }
                }
//                if(!ec_group[currentgroup].docheckstate)
//                   printf("OK : all slaves resumed OPERATIONAL.\n");
            }
            osal_usleep(20000);//10ms
        }

    }

    return 0;
}

void My_EcInit_check(){
    if(Thread_check==0){
        osal_thread_create(&Thread_check, 128000, &ecatcheck, (void*) &ctime);
    }
}
