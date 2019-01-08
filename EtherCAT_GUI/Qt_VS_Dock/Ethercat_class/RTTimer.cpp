#include "RTTimer.h"

Testtimest testtime;

int TimeRun(int id, int outtime, void (CALLBACK* lpfnTimer)(/*HWND, UINT, UINT_PTR, DWORD*/))//定时器启动实现 采用了可以多次重复创建并且删除的安全方式
{
    if (0 > id || 0 >= outtime)
    {
        return -1;
    }
    testtime.ptime = testtime.ptimehead;//为方便多次删除后继续用，每次先获取首节点
    if (0 != testtime.ptime)//如果头节点不为空，判断当前新增定时器ID是否与之前增加ID重复
    {
        while (id != testtime.ptime->id && 0 != testtime.ptime->next)
            testtime.ptime = testtime.ptime->next;
        if (id == testtime.ptime->id)//如果启动的定时器Id与之前的重复 则不启动
        {
            return -1;
        }
    }
    if (0 == testtime.ptime)
    {
        testtime.ptime = new timest;
        testtime.ptime->next = 0;
        testtime.ptime->prev = 0;
        testtime.ptimehead = testtime.ptime;
    }
    else
    {
        while (0 != testtime.ptime->next)
            testtime.ptime = testtime.ptime->next;
        testtime.ptime->next = new timest;
        testtime.ptime->next->next = 0;
        testtime.ptime->next->prev = testtime.ptime;
    }
    while (0 != testtime.ptime->next)
        testtime.ptime = testtime.ptime->next;
    testtime.ptime->id = id;//id
    testtime.ptime->beginRun = 1;//启动状态
    testtime.ptime->outtime = outtime;//超时时间
    testtime.ptime->lpfnTimer = lpfnTimer;//回调函数
    HANDLE handle = CreateThread(NULL, 0, TimerRunThread, testtime.ptime, 0, NULL);
    CloseHandle(handle);//为了C程序也能运行采用了API线程，并且句柄没实际用处就直接释放掉
    return 0;
}


DWORD WINAPI TimerRunThread(LPVOID _this)//定时器线程函数实现
{
    ptimest Runject = (/*decltype(_this)*/ptimest)_this;//本来想用获取类型方式来直接获取传入的类型，后来想到了在线程函数中类型是void*所以只能手动写入传入的参数类型
    while (1 == Runject->beginRun)//定时器状态为1执行，0时退出线程函数，删除定时器节点
    {
        UsSleep(Runject->outtime);
        if (0 != Runject->lpfnTimer)//如果回调函数不为空调用回调函数
        {
            Runject->lpfnTimer();
        }
        else
        {
            TestTime(Runject->id);//根据ID号进行调用普通定时器启动函数
        }
    }
    return 0;
}


void TestTime(int id)//测试 定时器调用函数实现
{
    int a = 0;
    if (1 == id)
    {
        a = 1;
    }
    if (2 == id)
    {
        a = 2;
    }
    if (3 == id)
    {
        a = 3;
    }
}
void TestKillTimer(int id)//定时器销毁函数实现
{
    ptimest deletime = testtime.ptimehead;
    ptimest next, prev;
    if (id == deletime->id)//首节点
    {
        next = testtime.ptimehead->next;
        deletime->beginRun = 0;
        UsSleep(deletime->outtime * 2);//为了可以退出定时器线程并且安全删除定时器节点采用了2倍超时时间
        delete deletime;
        deletime = 0;
        testtime.ptimehead = next;
        if (0 != next)
        {
            next->prev = 0;
        }
        return;
    }
    while (id != deletime->id && 0 != deletime)
        deletime = deletime->next;
    if (0 == deletime)//没有找到Id
    {
        return;
    }
    prev = deletime->prev;
    next = deletime->next;
    deletime->beginRun = 0;
    UsSleep(deletime->outtime * 2);
    delete deletime;
    deletime = 0;
    prev->next = next;
    if (0 != next)
    {
        next->prev = prev;
    }
}

int UsSleep(int us)//微秒延时函数实现
{
    LARGE_INTEGER fre;
    if (QueryPerformanceFrequency(&fre))
    {
        LARGE_INTEGER run, priv, curr, res;
        run.QuadPart = fre.QuadPart * us / 1000000;
        QueryPerformanceCounter(&priv);
        do
        {
            QueryPerformanceCounter(&curr);
        } while (curr.QuadPart - priv.QuadPart < run.QuadPart);
        curr.QuadPart -= priv.QuadPart;
        int nres = (curr.QuadPart * 1000000 / fre.QuadPart);
        return nres;
    }
    return -1;//
}
