#ifndef MASTER_CHECK_H
#define MASTER_CHECK_H

#ifdef __cplusplus
extern "C"
{
#endif

void My_EcInit_check();

extern char IsThreadCheck;
extern char Check_inOP;
extern volatile int Check_wkc;
int Check_expectedWKC;

#ifdef __cplusplus
}
#endif

#endif // MASTER_CHECK_H
