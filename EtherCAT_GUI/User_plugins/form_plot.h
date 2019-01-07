#ifndef FORM_PLOT_H
#define FORM_PLOT_H

#include <QWidget>

#include <QTimer>

namespace Ui {
class Form_plot;
}

class Form_plot : public QWidget
{
    Q_OBJECT

public:
    explicit Form_plot(QWidget *parent = 0);
    ~Form_plot();

private slots:
    void on_pushButton_clicked();

    void user_timeout_handle();
private:
    Ui::Form_plot *ui;

    int time_count;
    QTimer *time;
};

#endif // FORM_PLOT_H
