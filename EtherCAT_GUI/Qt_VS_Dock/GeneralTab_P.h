#ifndef GENERALTAB_P_H
#define GENERALTAB_P_H

#include "EtherCAT_UserApp.h"
//#include "my_EthercatMaster.h"
#include "form_generaltab.h"

class GeneralTab_P : public QObject,public EtherCAT_UserApp
{
    Q_OBJECT
public:
    explicit GeneralTab_P(QObject *parent = nullptr);
public:
    virtual void Init_Cores();
    virtual void Destroy_Cores();

    DRE_Master *get_MasterPtr();
    void Master_UI_RUn();
    void Master_UI_Stop();
    void Master_UI_Loop(int state);
    void Master_UI_Scan();
private:
    Form_GeneralTab *user_form_generalTab;
    QString m_settingPath;

    DRE_Master *m_master;
    void set_MasterPtr(DRE_Master *master);
protected:
    virtual int Load_setting(const QString &path);
    virtual int Save_setting(const QString &path);
signals:

public slots:
};

#endif // GENERALTAB_P_H
