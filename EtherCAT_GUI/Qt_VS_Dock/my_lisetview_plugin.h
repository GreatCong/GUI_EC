#ifndef MY_LISETVIEW_PLUGIN_H
#define MY_LISETVIEW_PLUGIN_H

#include <QListWidget>
#include <QJsonObject>

class My_LisetView_Plugin : public QListWidget
{
    Q_OBJECT
public:
    explicit My_LisetView_Plugin(QWidget *parent = nullptr);
public:
    void delete_Item();
    void delete_Item_all();
    void add_Item_all();
signals:

public slots:
};

/******* Items *****/
class My_LisetViewItem_Plugin : public QListWidgetItem
{
//    Q_OBJECT
public:
    explicit My_LisetViewItem_Plugin(QListWidget *view = nullptr);
    explicit My_LisetViewItem_Plugin(const QString &text ,QListWidget *view = nullptr);
public:
   QJsonObject plugin_jsonMSG;
//signals:

//public slots:
};

#endif // MY_LISETVIEW_PLUGIN_H
