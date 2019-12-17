#ifndef MY_TABLE_SLAVEMSG_H
#define MY_TABLE_SLAVEMSG_H

#include <QTableWidget>

///
/// \brief The My_TableWidget_Dispaly class
///
class My_TableWidget_Dispaly : public QTableWidget
{
    Q_OBJECT
public:
    explicit My_TableWidget_Dispaly(QWidget *parent = nullptr);
    virtual ~My_TableWidget_Dispaly(){}
public:
    void addTitle(const QStringList &title);
    virtual void append_RawData(int raw, const QStringList &data);
    virtual void init()=0;
signals:

public slots:
};

///
/// \brief The My_Table_SlaveMSG class
///
class My_Table_SlaveMSG : public My_TableWidget_Dispaly
{
    Q_OBJECT
public:
    explicit My_Table_SlaveMSG(QWidget *parent = nullptr);
    virtual ~My_Table_SlaveMSG(){}
public:
    void init();
signals:

public slots:
};

///
/// \brief The My_Table_SlaveItemMSG class
///
class My_Table_SlaveItemMSG : public My_TableWidget_Dispaly
{
    Q_OBJECT
public:
    explicit My_Table_SlaveItemMSG(QWidget *parent = nullptr);
public:
    void init();
signals:

public slots:
};

#endif // MY_TABLE_SLAVEMSG_H
