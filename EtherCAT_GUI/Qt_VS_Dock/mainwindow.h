#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTextEdit>

#include "mainformview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public:

private slots:
    void change_statusBarWidget(QWidget *widget, bool isAdd);
    void change_statusBarMessage(QString message, int interval);
    void on_actionEthercat_scan_triggered();

    void on_actionEthercat_run_triggered();

    void on_actionEthercat_stop_triggered();

    void on_actionEthercat_setting_triggered();

    void on_actionEthercat_exit_triggered();

    void on_actionAbout_triggered();

private:
    MainFormView *_view_main;
    QString _path_setting;

    void Init_Icons();
    void Init_StatusBar_Content();
    bool Load_UserStyle(QString name);
    void Init_Cores();

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
