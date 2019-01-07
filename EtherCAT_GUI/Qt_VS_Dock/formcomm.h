#ifndef FORMCOMM_H
#define FORMCOMM_H

#include <QWidget>

namespace Ui {
class FormComm;
}

class FormComm : public QWidget
{
    Q_OBJECT

public:
    explicit FormComm(QWidget *parent = 0);
    ~FormComm();

private:
    Ui::FormComm *ui;
};

#endif // FORMCOMM_H
