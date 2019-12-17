#ifndef COMPONENT_ETHERCAT_H
#define COMPONENT_ETHERCAT_H

#include "Component_Private.h"
#include "form_generaltab.h"

#include "EtherCAT_ScanThread.h"

///
/// \brief EtherCAT系统组件,提供网卡选择等功能
///
class Component_EtherCAT : public QObject,public Component_Private
{
    Q_OBJECT
    Q_INTERFACES(Component_Private)
public:
    Component_EtherCAT(QObject *parent = nullptr);
    virtual ~Component_EtherCAT();
public:
    virtual bool Init_Cores();
    virtual bool Destroy_Cores();
    virtual bool Init_Object();
    virtual bool Init_Slots();

    void Master_UI_RUn();
    void Master_UI_Stop();
    void Master_UI_Loop(int state);
    void Master_UI_Scan();
private:
    Form_GeneralTab *m_form_EtherCAT;
    QString m_settingPath;
    DRE_Master_Loader *m_master_Loader;

    bool Master_AttachMaster(QString path);
    bool Master_DettachMaster();
protected:
    virtual int Load_setting(const QString &path);
    virtual int Save_setting(const QString &path);
signals:

public slots:
    void WidgetSig_pushButton_clicked(int buttonID);
    void WidgetSig_ComboBox_changed(int comboBox_id,int index);
    void WidgetSig_AdapterList_itemDoubleClicked(QListWidgetItem *item);
};

#endif // COMPONENT_ETHERCAT_H
