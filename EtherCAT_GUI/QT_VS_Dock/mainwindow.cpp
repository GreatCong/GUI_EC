#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTreeWidget>
#include <QTextEdit>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>

///
/// \brief 构造函数
/// \param parent
///
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init_Cores();
    _path_setting = "./config.ini";
    _view_main->Load_setting(_path_setting);
    _view_main->Init_Components();

    if(_view_main->get_MasterPtr() == nullptr){//主站没有加载
        ui->actionEthercat_run->setEnabled(false);
        ui->actionEthercat_stop->setEnabled(false);
        ui->actionEthercat_exit->setEnabled(false);
        ui->actionEthercat_scan->setEnabled(false);
        ui->actionEthercat_output->setEnabled(false);
    }

    ui->action_GenrateCodeIsAbsolute->setChecked(true);
}

///
/// \brief 析构函数
///
MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief 初始化
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
    connect(_view_main,SIGNAL(User_Windows_CriticalError(int)),this,SLOT(View_UserWindows_CriticalError(int)));
    connect(_view_main,SIGNAL(MasterRunMode_change(bool)),this,SLOT(change_masterRunMode(bool)));

    ui->actionEthercat_run->setEnabled(false);
    ui->actionEthercat_stop->setEnabled(false);
    ui->actionEthercat_exit->setEnabled(false);
    ui->actionEthercat_scan->setEnabled(true);
    ui->actionEthercat_output->setEnabled(false);

    ui->action_ViewDeviceTreeWindow->setChecked(true);
    ui->action_ViewPluginsWindow->setChecked(true);
    ui->action_ViewMessageWindow->setChecked(true);

    _view_main->get_arrayActionMenuBar()->push_back(ui->actionEthercat_run);
    _view_main->get_arrayActionMenuBar()->push_back(ui->actionEthercat_stop);
    _view_main->get_arrayActionMenuBar()->push_back(ui->actionEthercat_exit);
    _view_main->get_arrayActionMenuBar()->push_back(ui->actionEthercat_scan);
    _view_main->get_arrayActionMenuBar()->push_back(ui->actionEthercat_output);

}

///
/// \brief 生成代码是否是绝对地址
/// \return
///
bool MainWindow::get_GenetateCode_isAbsolute()
{
    return ui->action_GenrateCodeIsAbsolute->isChecked();
}

///
/// \brief 加载皮肤文件
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
//    QPixmap pixmap(":/image1/Resource/Icons/VS.png");

//    QIcon vs_icon;
//    vs_icon.addPixmap(pixmap.copy(0, 0, 24, 24), QIcon::Normal, QIcon::Off);
//    vs_icon.addPixmap(pixmap.copy(0, 24, 24, 24), QIcon::Normal, QIcon::On);
//    vs_icon.addPixmap(pixmap.copy(24, 0, 24, 24), QIcon::Active, QIcon::On);
//    vs_icon.addPixmap(pixmap.copy(24, 24, 24, 24), QIcon::Active, QIcon::Off);

//    this->setWindowIcon(vs_icon);
    this->setWindowTitle(tr("Ethercat Master"));
}

///
/// \brief 初始化底部状态栏信息
///
void MainWindow::Init_StatusBar_Content(){
    statusBar()->addPermanentWidget(_view_main->get_ProcessBarPtr(MainFormView::ProcessBar_ScanWait_p));//放在view,不能加入父类而出错
    statusBar()->addWidget(_view_main->get_LabelPtr(MainFormView::Label_status_l));

}

///
/// \brief 主站的scan按钮处理
/// \param isUIonly
///
void MainWindow::masterMode_scan_handle(bool isUIonly){
    static int run_num;
    if(run_num > 0){ //避免这个信号执行多次
       return;
    }

    int ret = 0;
    if(!isUIonly){
        run_num ++;
        ret = _view_main->Master_scan();
    }
//      if(ret==0){
//          ui->actionEthercat_run->setEnabled(true);
//          ui->actionEthercat_stop->setEnabled(true);
//          ui->actionEthercat_exit->setEnabled(true);
//          ui->actionEthercat_scan->setEnabled(false);
//      }
      run_num = 0;
}

///
/// \brief 主站的运行按钮处理
/// \param isUIonly
///
void MainWindow::masterMode_run_handle(bool isUIonly){
    int ret = 0;
    if(!isUIonly){
        ret = _view_main->Master_run();
    }
      if(ret==0){
          ui->actionEthercat_run->setEnabled(false);
          ui->actionEthercat_stop->setEnabled(true);
          ui->actionEthercat_exit->setEnabled(true);
      }
}

///
/// \brief 主站的停止按钮处理
/// \param isUIonly
///
void MainWindow::masterMode_stop_handle(bool isUIonly){
    int ret = 0;
    if(!isUIonly){
       ret =_view_main->Master_stop();
    }
       if(ret==0){
           ui->actionEthercat_run->setEnabled(true);
           ui->actionEthercat_stop->setEnabled(false);
           ui->actionEthercat_exit->setEnabled(true);
           ui->actionEthercat_output->setEnabled(true);
       }
}

///
/// \brief 主站的退出按钮处理
/// \param isUIonly
///
void MainWindow::masterMode_exit_handle(bool isUIonly){
    int ret = 0;
    if(!isUIonly){
        ret = _view_main->Master_exit();
    }
     if(ret==0){
         ui->actionEthercat_run->setEnabled(false);
         ui->actionEthercat_stop->setEnabled(false);
         ui->actionEthercat_exit->setEnabled(false);
         ui->actionEthercat_scan->setEnabled(true);
         ui->actionEthercat_output->setEnabled(true);
     }
}

/********************** SLOTS ******************************/
///
/// \brief 设置状态栏信息
/// \param message
///
void MainWindow::change_statusBarMessage(QString message,int interval){
    statusBar()->showMessage(message,interval);//3s后关闭
}

///
/// \brief 设置master为run模式
/// \param isRun
///
void MainWindow::change_masterRunMode(bool isRun)
{
    if(isRun){
        masterMode_run_handle(true);
    }
    else{
        masterMode_stop_handle(true);
    }
}

///
/// \brief 向底部状态栏中添加widget
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
    masterMode_scan_handle();
}

///
/// \brief Ethecat Run Action
///
void MainWindow::on_actionEthercat_run_triggered()
{
   masterMode_run_handle();
}

///
/// \brief Ethecat stop Action
///
void MainWindow::on_actionEthercat_stop_triggered()
{
    masterMode_stop_handle();
}

///
/// \brief MainWindow::on_actionEthercat_setting_triggered
///
void MainWindow::on_actionEthercat_setting_triggered()
{
    _view_main->Master_setting();
}

///
/// \brief MainWindow::on_actionEthercat_exit_triggered
///
void MainWindow::on_actionEthercat_exit_triggered()
{
    masterMode_exit_handle();
}

///
/// \brief 生成从站的头文件
///
void MainWindow::on_actionEthercat_output_triggered()
{
    QString dir = "";
    dir = QFileDialog::getExistingDirectory(this, "请选择输出路径...", "./");//如果没有选择路径就会为空
    if(!dir.isEmpty()){
        _view_main->Generate_UserCode(dir,this->get_GenetateCode_isAbsolute());
//          qDebug() << dir;
    }

}

///
/// \brief MainWindow::on_actionAbout_triggered
///
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"About",MASTER_VERSION_CODE);
}

///
/// \brief MainWindow::View_UserWindows_CriticalError
/// \param error
///
void MainWindow::View_UserWindows_CriticalError(int error)
{
    Q_UNUSED(error);
}

///
/// \brief 视图显示--左边的dock
/// \param checked
///
void MainWindow::on_action_ViewDeviceTreeWindow_triggered(bool checked)
{
    if(checked){
        _view_main->get_FramePtr(MainFormView::Frame_left_f)->show();
    }
    else{
        _view_main->get_FramePtr(MainFormView::Frame_left_f)->hide();
    }

}

///
/// \brief 视图显示--右边的dock
/// \param checked
///
void MainWindow::on_action_ViewPluginsWindow_triggered(bool checked)
{
    if(checked){
        _view_main->get_FramePtr(MainFormView::Frame_right_f)->show();
    }
    else{
        _view_main->get_FramePtr(MainFormView::Frame_right_f)->hide();
    }
}

///
/// \brief 视图显示--底部的dock
/// \param checked
///
void MainWindow::on_action_ViewMessageWindow_triggered(bool checked)
{
    if(checked){
        _view_main->get_FramePtr(MainFormView::Frame_bottom_f)->show();
    }
    else{
        _view_main->get_FramePtr(MainFormView::Frame_bottom_f)->hide();
    }
}


///
/// \brief 打开用户手册
///
void MainWindow::on_actionUserManual_triggered()//用户手册
{
    //使用默认浏览器打开网页
//    QString URL = "www.baidu.com";
//    QDesktopServices::openUrl(QUrl("file:///C://Users", QUrl::TolerantMode));//打开文件夹
    QDir tempFilePath("./User-Manual.html");
    QString usrl_absPath = tempFilePath.absolutePath();
    if(QFile::exists(usrl_absPath)){
        QString url_str = "file:///"+usrl_absPath;//本地文件的URL必须要绝对地址
        QDesktopServices::openUrl(QUrl(url_str.toLatin1()));
    }
    else{
        QMessageBox::warning(this,"warning","User's Manual Document not exists,please check the path!");
    }

}

/********************** SLOTS END ******************************/

/********************** Evenet ******************************/
///
/// \brief 主窗口关闭事件
/// \param event
///
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
            _view_main->Destroy_Cores();
            event->accept();  //接受退出信号，程序退出
        }
}
/********************** Evenet END ******************************/
