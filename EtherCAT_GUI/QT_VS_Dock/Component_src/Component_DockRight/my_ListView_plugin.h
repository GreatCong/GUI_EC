#ifndef MY_LISETVIEW_PLUGIN_H
#define MY_LISETVIEW_PLUGIN_H

#include <QListWidget>
#include <QJsonObject>
#include <QPoint>
#include <QIcon>
#include <QListWidgetItem>

#include <QObject>

class My_LisetView_Plugin : public QListWidget
{
  Q_OBJECT
  public:
     My_LisetView_Plugin(QWidget *parent=nullptr);
     ~My_LisetView_Plugin();
    void delete_Item();
    void delete_Item_all();
    void add_Item_all();
  protected:
     void mouseMoveEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
  private:
     QPoint m_dragPosition; // 拖放起点
     QListWidgetItem *m_dragItem; // 被拖放的item
private slots:

};

/******* Items *****/
class My_LisetViewItem_Plugin : public QListWidgetItem
{
//   Q_OBJECT
public:
    My_LisetViewItem_Plugin(QListWidget *view = nullptr);
    My_LisetViewItem_Plugin(const QString &text ,QListWidget *view = nullptr);
    virtual ~My_LisetViewItem_Plugin();
public:
   QJsonObject plugin_jsonMSG;
   QString get_Item_fileName(){ return m_item_fileName;}
   void set_Item_fileName(const QString fileName){ m_item_fileName = fileName;}
protected:
   QString m_item_fileName;
//signals:

//public slots:
};

#endif // MY_LISETVIEW_PLUGIN_H
