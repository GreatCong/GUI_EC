#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTreeWidget>
#include <QTextEdit>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init_Cores();
    _path_setting = "./config.ini";
    _view_main->Load_setting(_path_setting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::Init_Cores
///
void MainWindow::Init_Cores(){
    Load_UserStyle(":/Darkstyle/Resource/style/qdarkstyle/style.qss");
    _view_main = new MainFormView();
    this->setCentralWidget(_view_main);//new放在前面执行，防止后面指针错误

    Init_Icons();
    Init_StatusBar_Content();
//    _view_main->show();

    connect(_view_main,SIGNAL(StatusMessage_change(QString,int)),this,SLOT(change_statusBarMessage(QString,int)));
    connect(_view_main,SIGNAL(StatusWidget_change(QWidget*,bool)),this,SLOT(change_statusBarWidget(QWidget*,bool)));

    ui->actionEthercat_run->setEnabled(false);
    ui->actionEthercat_stop->setEnabled(false);
    ui->actionEthercat_exit->setEnabled(false);
}

///
/// \brief MainWindow::Load_UserStyle
/// \param name
/// \return
///
bool MainWindow::Load_UserStyle(QString name){
    QFile qss(name);
    if(!qss.open(QFile::ReadOnly)){
        return false;
    }

    this->setStyleSheet(qss.readAll());
    qss.close();

    return true;
}

///
/// \brief 初始化应用程序图标
///
void MainWindow::Init_Icons()
{
    QPixmap pixmap(":/image1/Resource/Icons/VS.png");

    QIcon vs_icon;
    vs_icon.addPixmap(pixmap.copy(0, 0, 24, 24), QIcon::Normal, QIcon::Off);
    vs_icon.addPixmap(pixmap.copy(0, 24, 24, 24), QIcon::Normal, QIcon::On);
    vs_icon.addPixmap(pixmap.copy(24, 0, 24, 24), QIcon::Active, QIcon::On);
    vs_icon.addPixmap(pixmap.copy(24, 24, 24, 24), QIcon::Active, QIcon::Off);

    this->setWindowIcon(vs_icon);
    this->setWindowTitle(tr("Ethercat Master"));
}

///
/// \brief MainWindow::Init_StatusBar_Content
///
void MainWindow::Init_StatusBar_Content(){

//    QProgressBar *xx = new QProgressBar(this);
//    QLabel *permanent = new QLabel(this);
//    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken);
//    permanent->setText(
//      tr("Ethecat Master"));
//    permanent->setTextFormat(Qt::RichText);
//    permanent->setOpenExternalLinks(true);
//    ui->statusBar->addPermanentWidget(xx);
//    xx->setMaximum(100);
//    xx->setValue(50);
//    ui->statusBar->addPermanentWidget(permanent);
//   statusBar()->addWidget(_view_main->m_scanWait_processBar);//放在view,不能加入父类而出错
    statusBar()->addPermanentWidget(_view_main->m_scanWait_processBar);//放在view,不能加入父类而出错
    statusBar()->addWidget(_view_main->m_status_label);

}

/********************** SLOTS ******************************/
///
/// \brief MainWindow::change_statusBarMessage
/// \param message
///
void MainWindow::change_statusBarMessage(QString message,int interval){
    statusBar()->showMessage(message,interval);//3s后关闭
}

///
/// \brief MainWindow::change_statusBarWidget
/// \param widget
/// \param isAdd
///
void MainWindow::change_statusBarWidget(QWidget *widget,bool isAdd){
    if(isAdd){
        statusBar()->addWidget(widget,1);
    }
    else{
        statusBar()->removeWidget(widget);
    }

}

///
/// \brief Ethecat Scan Action
///
void MainWindow::on_actionEthercat_scan_triggered()
{
    int ret = 0;
      ret = _view_main->Master_scan();
      if(ret==0){
          ui->actionEthercat_run->setEnabled(true);
          ui->actionEthercat_stop->setEnabled(true);
          ui->actionEthercat_exit->setEnabled(true);
          ui->actionEthercat_scan->setEnabled(false);
      }
}

///
/// \brief Ethecat Run Action
///
void MainWindow::on_actionEthercat_run_triggered()
{
    int ret = 0;
      ret = _view_main->Master_run();
      if(ret==0){
          ui->actionEthercat_run->setEnabled(false);
          ui->actionEthercat_stop->setEnabled(true);
          ui->actionEthercat_exit->setEnabled(true);
      }
}

///
/// \brief Ethecat stop Action
///
void MainWindow::on_actionEthercat_stop_triggered()
{
    int ret = 0;
       ret =_view_main->Master_stop();
       if(ret==0){
           ui->actionEthercat_run->setEnabled(true);
           ui->actionEthercat_stop->setEnabled(false);
           ui->actionEthercat_exit->setEnabled(true);
       }
}

void MainWindow::on_actionEthercat_setting_triggered()
{
    _view_main->Master_setting();
}

void MainWindow::on_actionEthercat_exit_triggered()
{
    int ret = 0;
       ret = _view_main->Master_exit();
     if(ret==0){
         ui->actionEthercat_run->setEnabled(false);
         ui->actionEthercat_stop->setEnabled(false);
         ui->actionEthercat_exit->setEnabled(false);
         ui->actionEthercat_scan->setEnabled(true);
     }
}
/********************** SLOTS END ******************************/

/********************** Evenet ******************************/
void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("退出程序"),
            QString(tr("警告：程序有一个任务正在运行中，是否结束操作退出?")),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::No) {
            event->ignore();  //忽略退出信号，程序继续运行
        }
        else if (button == QMessageBox::Yes) {
            _view_main->Save_setting(_path_setting);
            event->accept();  //接受退出信号，程序退出
        }
}
/********************** Evenet END ******************************/

