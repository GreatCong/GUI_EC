#ifndef FORM_GENERALTAB_H
#define FORM_GENERALTAB_H

#include <QWidget>
#include <QTableWidget>
#include <QListWidgetItem>

#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

//#include "my_EthercatMaster.h"
#include "DRE_Master_Loader.h"

namespace Ui {
class Form_GeneralTab;
}

///
/// \brief 自定义List
///
class My_ListViewItem : public QListWidgetItem
{
public:
    explicit My_ListViewItem(QListWidget *view = nullptr);
    explicit My_ListViewItem(const QString &text ,QListWidget *view = nullptr);
public:
   int current_index;
};

///
/// \brief EtherCAT配置的UI界面
///
class Form_GeneralTab : public QWidget
{
    Q_OBJECT

public:
    explicit Form_GeneralTab(QWidget *parent = 0);
    ~Form_GeneralTab();
public:
    typedef enum{
       Master_findAdapter_b,
       Master_stateInit_b,
       Master_statePreOp_b,
       Master_stateSafeOp_b,
       Master_stateOp_b
    }buttons_choose;

    typedef enum{
        Master_adapterDesc_e,
        Master_adapterName_e,

        Master_InquireState_e,
        Master_ActualState_e
    }lineEdits_choose;

    typedef enum{
        Master_periodPLC_c,
    }ComboBox_choose;

    typedef enum{
        Master_Aadapter_L,
    }ListWidget_choose;

    QTableWidget *table;
    //DRE_Master *master;
    const QString getMaster_adapterName();
    const QString getMaster_adapterDesc();
    void setMaster_adapterDesc(const QString &str);
    void setMaster_adapterName(const QString &str);

    QPushButton *get_ButtonPtr(buttons_choose choose);
    QLineEdit *get_LineEditPtr(lineEdits_choose choose);
    QComboBox *get_ComboBoxPtr(ComboBox_choose choose);
    QListWidget *get_ListWidgetPtr(ListWidget_choose choose);
    void Master_AdapterFind_Handle(QStringList adapterList);
private slots:
    void on_pushButton_AdapterFind_clicked();

    void on_pushButton_State_Init_clicked();

    void on_pushButton_State_PreOp_clicked();

    void on_pushButton_State_SafeOp_clicked();

    void on_pushButton_State_Op_clicked();

    void on_comboBox_PLCPeriod_currentIndexChanged(int index);

private:
    Ui::Form_GeneralTab *ui;
    void Init_cores();
    void Destroy_cores();
signals:
    void sig_pushButton_clicked(int button_id);
    void sig_ComboBox_changed(int comboBox_id,int index);

};

#endif // FORM_GENERALTAB_H
