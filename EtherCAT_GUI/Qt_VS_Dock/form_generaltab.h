#ifndef FORM_GENERALTAB_H
#define FORM_GENERALTAB_H

#include <QWidget>
#include <QTableWidget>
#include <QListWidgetItem>

#include <QPushButton>
#include <QLineEdit>

//#include "my_EthercatMaster.h"
#include "DRE_Master_Loader.h"

namespace Ui {
class Form_GeneralTab;
}

class Form_GeneralTab : public QWidget
{
    Q_OBJECT

public:
    explicit Form_GeneralTab(QWidget *parent = 0);
    ~Form_GeneralTab();
public:
    typedef enum{
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

    QTableWidget *table;
    DRE_Master *master;
    const QString getMaster_adapterName();
    const QString getMaster_adapterDesc();
    void setMaster_adapterDesc(const QString &str);
    void setMaster_adapterName(const QString &str);

    QPushButton *get_ButtonPtr(buttons_choose choose);
    QLineEdit *get_LineEditPtr(lineEdits_choose choose);
private slots:
    void on_pushButton_AdapterFind_clicked();

    void on_listWidget_AdapterList_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_State_Init_clicked();

    void on_pushButton_State_PreOp_clicked();

    void on_pushButton_State_SafeOp_clicked();

    void on_pushButton_State_Op_clicked();

private:
    Ui::Form_GeneralTab *ui;//外部无法访问??
    void Init_cores();

    DRE_Master_Loader *m_master_Loader;
    void Load_master();
};

#endif // FORM_GENERALTAB_H
