#ifndef RTTIMER_H
#define RTTIMER_H

//#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int UsSleep(int us);//返回实际的微秒延时时间

//参数一 定时器ID号，参数二 定时时间(微秒)，参数三 回调函数(不需要设置NULL或者不写)
int TimeRun(int id, int outtime, void (CALLBACK* lpfnTimer)(/*HWND, UINT, UINT_PTR, DWORD*/) = NULL);//定时器启动函数

                                                                                                     //参数一 定时器ID号
void TestKillTimer(int id);//定时器销毁函数 (可以不使用)


                        //第一个参数 定时器ID号(ID号用于多个定时器之间的判断)
void TestTime(int id);//测试用的普通定时器调用函数 想启动的函数写在这个函数内

DWORD WINAPI TimerRunThread(LPVOID _this);//定时器线程函数

typedef struct timest//定时器数据保存结构体
{
    int id;//ID号
    int outtime;//超时时间
    volatile int beginRun;//运行状态
    void (CALLBACK* lpfnTimer)(/*HWND, UINT, UINT_PTR, DWORD*/);//回调函数
    timest *prev, *next;//双向链表指针
}*ptimest;

typedef struct Testtimest//定时器执行结构体
{
    ptimest ptime, ptimehead;//定时器运行节点以及头节点
    Testtimest()
    {
        ptime = 0;
        ptimehead = 0;
    }
    virtual ~Testtimest()//析构函数内部封装了定时器对象退出后的检查及释放
    {
        while (0 != ptimehead)
        {
            ptimest deletime;
            deletime = ptimehead->next;
            if (1 == ptimehead->beginRun)//如果定时线程还在执行 ，先退出线程再删除
            {
                ptimehead->beginRun = 0;
                UsSleep(ptimehead->outtime * 2);//删除线程节点前要留出当前定时器的2倍睡眠时间
            }
            delete ptimehead;
            ptimehead = deletime;
        }
    }
};


#endif // RTTIMER_H
