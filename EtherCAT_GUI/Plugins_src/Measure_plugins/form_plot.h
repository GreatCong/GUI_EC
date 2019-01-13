#ifndef FORM_PLOT_H
#define FORM_PLOT_H

#include <QWidget>

#include <QTimer>
#include <qcustomplot.h>

namespace Ui {
class Form_plot;
}

class Form_plot : public QWidget
{
    Q_OBJECT

public:
    explicit Form_plot(QWidget *parent = 0);
    ~Form_plot();

    QCustomPlot *get_customPlot();
private slots:

private:
    Ui::Form_plot *ui;

    void Init_Plots();
};

#endif // FORM_PLOT_H
