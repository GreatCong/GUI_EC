#ifndef DOCK_OBJ_UTILS_H
#define DOCK_OBJ_UTILS_H

#include <QPlainTextEdit>
#include <QTableWidget>
#include <QLayout>
#include <QString>
#include <QStringList>

///
/// \brief 工具类
///
class Dock_Obj_Utils
{
public:
    Dock_Obj_Utils();
    virtual ~Dock_Obj_Utils(){}
public:
    static void TextList_append(QPlainTextEdit *edit_widget, const QStringList &list);
    static bool Add_TableProperty(QTableWidget *table, int num, QString str1, QString str2);
    static bool AddTableContent(QTableWidget *table, int raw, int column, QString &str);
    static void clearLayout(QLayout *layout);
};

#endif // DOCK_OBJ_UTILS_H
