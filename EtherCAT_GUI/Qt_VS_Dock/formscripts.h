#ifndef FORMSCRIPTS_H
#define FORMSCRIPTS_H

#include <QWidget>

#include <QTableWidget>

namespace Ui {
class FormScripts;
}

class FormScripts : public QWidget
{
    Q_OBJECT

public:
    explicit FormScripts(QWidget *parent = 0);
    ~FormScripts();
public:
    QTableWidget *table;

private slots:

private:
    Ui::FormScripts *ui;//外部无法访问??
};

#endif // FORMSCRIPTS_H
