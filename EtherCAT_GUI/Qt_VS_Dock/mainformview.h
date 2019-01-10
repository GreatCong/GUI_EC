#ifndef MAINFORMVIEW_H
#define MAINFORMVIEW_H

#include <QWidget>

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8") ///< 最好加上这句，否则乱码
#endif

#include <QStandardItemModel>
#include <QTreeView>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTabWidget>

#include "formcomm.h"
#include "formscripts.h"

#include "form_generaltab.h"
#include "my_treeview_device.h"
#include "my_UserAppWidge.h"
#include "my_lisetview_plugin.h"
#include "my_Table_SlaveMSG.h"

#include "ControlTab_P.h"

#ifdef _MSC_VER //windows
#include "MMTimer_RTwin.h"
#endif

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
private:
    /*   Left Frame  */
    My_TreeView_Device *mDeviceTree;
    QMenu* m_projectMenu;
    QMenu* m_itemMenu;
    /* Left Frame End */

    /*   Right Frame  */
    My_LisetView_Plugin *m_pluginList;
    QString m_pluginDir;
    /* Left Frame End */

    /*   Center Frame  */
    QTableWidget *mTableWidget_property;
    QTabWidget *mTabWedget_center;
    QWidget *mtabWeidgetItem_General;//普通
    QWidget *mtabWeidgetItem_Messure;//测量
    QWidget *mtabWeidgetItem_Control;//控制
    QWidget *mtabWeidgetItem_UserApps;//控制
    /* Left Frame End */

    /*   Bottom Frame  */
    QPlainTextEdit *m_bottomText;
    /* Left Frame End */

    /*   Ethercat  */
//    #ifdef _MSC_VER //windows
//     MMTimer_RTwin *m_timer_advanced;
//    #endif
    //    QTimer *m_timer_advanced;

    QWidget *m_widget_slaveMSG;
    My_Table_SlaveMSG *m_tableView_slaveMSG;
    My_Table_SlaveItemMSG *m_tableView_slaveItemMSG;

    /* Ethercat End */

    /*   others  */
    /* others End */

    /*   Temp  */
    QThread *thread;

    void Init_FrameLeft_Content(void);
    void Init_FrameRight_Content(void);
    void Init_FrameCenter_Content(void);
    void Init_FrameBottom_Content(void);    
    void Init_Cores(void);

    void Init_DeviceTreeItem_menu();

    int LoadPlugins();
    bool LoadPlugins(const QString &fileName);
    int ScanPlugins(const QString &plugin_dir);
    bool Add_TableProperty(QTableWidget *table, int num, QString &str1, QString &str2);
    bool AddTableContent(QTableWidget *table, int raw, int column, QString &str);

    void TextList_append(QPlainTextEdit *edit_widget, const QStringList &list);

    void ProcessBar_stopEvent();
    void ProcessBar_startEvent();
//    void keyPressEvent(QKeyEvent *event);

    void EthercatApp_init();
    void EthercatApp_destroy();

    /* 测试用的 */
    FormComm *user_formComm;
    FormScripts *user_form_scripts;
    Form_GeneralTab *user_form_generalTab;
    QTimer *mm_time;
    int timer_num;


    void Init_TestCores();
    void Init_Plots();
    /* 测试结束 */

//    int Gcode_load(QString &fileName);
public:
    /*   MainWindow  */
    QProgressBar *m_scanWait_processBar;
    QLabel *m_status_label;

    ControlTab_P *control_xx;

    typedef enum{
        Frame_left_f,
        Frame_right_f,
        Frame_center_f,
        Frame_bottom_f
    }Frame_choose;
    QFrame *get_FramePtr(Frame_choose choose);
    /* MainWindow End */

    /*   Temp  */

//    My_MotorApp_Callback m_motorApp_callback;//Ethercat应用回调
    void Set_StatusMessage(QString message,int interval);
    void Set_StatusWidget(QWidget *widget,bool isAdd);

    int Master_scan();
    int Master_run();
    int Master_stop();
    int Master_setting();
    int Master_exit();

    void clearLayout(QLayout *layout);

    int Load_setting(const QString &path);
    int Save_setting(const QString &path);

    Form_GeneralTab *get_UserGeneralTab(){
        return user_form_generalTab;
    }
private slots:
    void mDeviceTree_customContextMenuRequested(const QPoint &pos);
    void mDeviceTree_DoubleClicked(const QModelIndex);
    void mDeviceTree_expanded(const QModelIndex index);
    void AddTree_Master_clicked();
    void AddTree_Slave_clicked();
    void AddTree_SlaveItem_clicked();
    void AddTree_SlaveMessageInput_clicked();
    void AddTree_SlaveMessageOutput_clicked();
    void mDeviceTree_EcScan_clicked();
    void Plugins_Manager_clicked();
    void Plugins_Add_clicked();
    void BottonText_Clear_clicked();
    void mPlugins_item_DoubleClicked(QListWidgetItem *item);
    void mPlugins_item_Clicked(QListWidgetItem *item);

//    void mTimerAdvanced_timeout();


    /* 测试用的 */
    void mm_timerOut();
    /* 测试结束 */

    void Plugins_OpenEditor_clicked();

    void Control_StatusMessage_change(QString message,int interval);
    void Control_BottomMessage_change(QString message);
    void Control_MasterStop_Signal();
signals:
//    void explains(); //如果要自定义槽和信号, explains信号是必须的
    //NOTE:自定义信号，加入const变量无法识别???可能是没有向QT注册const signal
    void StatusMessage_change(QString message,int interval);//自定义信号
    void StatusWidget_change(QWidget *widget,bool isAdd);
};

#endif // MAINFORMVIEW_H
