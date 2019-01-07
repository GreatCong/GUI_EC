#ifndef FORM_GENERALTAB_H
#define FORM_GENERALTAB_H

#include <QWidget>
#include <QTableWidget>
#include <QListWidgetItem>

#include "my_EthercatMaster.h"

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
    QTableWidget *table;
    My_EthercatMaster *master;
    const QString getMaster_adapterName();
    const QString getMaster_adapterDesc();
    void setMaster_adapterDesc(const QString &str);
    void setMaster_adapterName(const QString &str);
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
};

#endif // FORM_GENERALTAB_H
