#ifndef MAINFORMVIEW_H
#define MAINFORMVIEW_H

#include <QWidget>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8") ///< 最好加上这句，否则乱码
#endif

#include <QStandardItemModel>
#include <QTreeView>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTabWidget>
#include <QScrollArea>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>

#include "DRE_Master.h"
#include "Component_DockLeft.h"
#include "Component_DockRight.h"
#include "Dock_Obj_Utils.h"
#include "Component_DockBottom.h"
#include "Component_EtherCAT.h"
#include "EtherCAT_UserApp.h"
#include "EtherCAT_ScanThread.h"
#include <QVector>

#define MASTER_VERSION_CODE "Version 1.2.6 beta"

namespace Ui {
class MainFormView;
}


class MainFormView : public QWidget
{
    Q_OBJECT

public:
    explicit MainFormView(QWidget *parent = 0);
    ~MainFormView();
private:
    Ui::MainFormView *ui;

public:
    typedef enum{
        Frame_left_f,
        Frame_right_f,
        Frame_center_f,
        Frame_bottom_f
    }Frame_choose;

    typedef enum{
        ProcessBar_ScanWait_p
    }ProcessBar_choose;

    typedef enum{
        Label_status_l
    }Label_choose;

    typedef enum{
        Sys_Err_NONE = 0,
        Sys_Err_InvalidMaster = -1
    }Sys_Error;

    typedef enum{
        MenuBar_actionEthercat_run,
        MenuBar_actionEthercat_stop,
        MenuBar_actionEthercat_exit,
        MenuBar_actionEthercat_scan,
        MenuBar_actionEthercat_output
    }Action_MenuBar_enum;

    DRE_Master *get_MasterPtr();
    Component_EtherCAT *get_genetalTabPtr();

    virtual void Init_Cores(void);
    virtual void Destroy_Cores(void);
    virtual bool Init_Components(void);

    QFrame *get_FramePtr(Frame_choose choose);
    QProgressBar *get_ProcessBarPtr(ProcessBar_choose choose);
    QLabel *get_LabelPtr(Label_choose choose);

    QVector<QAction*> *get_arrayActionMenuBar() { return m_arrayActionMenuBar; }
    void set_arrayActionMenuBar(QVector<QAction*> *action) {m_arrayActionMenuBar = action; }

    EtherCAT_UserApp *get_userAppPtr() { return this->m_ComponentWidgets->m_userAppPtr; }
    void set_userAppPtr(EtherCAT_UserApp *userApp) { this->m_ComponentWidgets->m_userAppPtr = userApp; }

    int Master_scan();
    int Master_run();
    int Master_stop();
    int Master_setting();
    int Master_exit();

    int Load_setting(const QString &path);
    int Save_setting(const QString &path);

    int Generate_UserCode(QString &path, bool isAbsolute);
private:
    void Init_FrameLeft_Content(void);
    void Init_FrameRight_Content(void);
    void Init_FrameCenter_Content(void);
    void Init_FrameBottom_Content(void);
    void Init_TestCores();

    void ProcessBar_stopEvent();
    void ProcessBar_startEvent();
    void masterMode_change(bool isRun);

    void EthercatApp_init();
    void EthercatApp_destroy();
    void Master_attach(DRE_Master *master);
    void Master_dettach();

    void Set_StatusMessage(QString message,int interval);
    void Set_StatusWidget(QWidget *widget,bool isAdd);
    void System_BottomMessage(const QString &message);

    void set_Plugin_Dirs(const QString &path);
    bool Init_Cores_afterSetting();
private:
    typedef enum{
        Master_Err_NONE,
        Master_Err_InvalidAdapter = -1,
        Master_Err_NoSlave = -2,
        Master_Err_InvalidPlugin = -3,
        Master_Err_InvalidCallback = -4,
        Master_Err_UserCallbackBan = -5
    }master_Error;

    ComponentWidgets *m_ComponentWidgets;//对外窗口的集合
    QTabWidget *mTabWedget_center;
    QScrollArea *mtabWeidgetItem_General;//普通
    QScrollArea *mtabWeidgetItem_UserApps;//自定义
    QWidget *m_widget_slaveMSG;
    QList<Component_Private *> *m_ComponentList;//存储每个组件的this指针（注意都是Component指针）
    QPlainTextEdit *m_MessageText;//输出框指针

    Component_EtherCAT *m_component_EtherCAT;
    Component_DockRight *m_dockObj_right;
    Component_DockBottom *m_dockObj_bottom;
    Component_DockLeft *m_dockObj_left;
    EtherCAT_ScanThread m_Master_ScanThread;

    QProgressBar *m_scanWait_processBar;
    QLabel *m_status_label;

    QVector<QAction*> *m_arrayActionMenuBar;

    DRE_Master *m_master;
    QString m_pluginDir;

    QTimer *mm_time;
    int timer_num;

    QTimer *m_timer_advanced;
private slots:
    /* 测试用的 */
    void mm_timerOut();
    /* 测试结束 */

    #ifndef Q_OS_WIN //如果不是windows系统
    void Master_TimerAdvanced_timeout();
    #endif

    void Control_StatusMessage_change(QString message,int interval);
    void Control_BottomMessage_change(QString message);
    void Control_MasterStop_Signal();

    void System_MasterScanThread_started();
    void System_MasterScanThread_finished();
    void ComponentWidgetsSigMaster_setState(int state);
    void ComponentWidgetsSigAppPath_change(QString &appPath);
signals:
    void StatusMessage_change(QString message,int interval);
    void StatusWidget_change(QWidget *widget,bool isAdd);
    void MasterRunMode_change(bool isRun);

    void User_Windows_CriticalError(int error);
};

#endif // MAINFORMVIEW_H
