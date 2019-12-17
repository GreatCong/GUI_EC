#include "CodeEditor_form.h"
#include "ui_codeeditor_form.h"

#include <QFont>
#include "src/myhighlighter.h"
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>

CodeEditor_form::CodeEditor_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEditor_form)
{
    ui->setupUi(this);

    this->Init_cores();
}

CodeEditor_form::~CodeEditor_form()
{
    this->Destroy_cores();

    delete ui;
}

void CodeEditor_form::Init_cores()
{
    this->resize(800,600);
    this->setWindowTitle("Code_Editor");

    m_code_editor = new CodeEditor();
    m_code_editor->setMode(EDIT);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(m_code_editor);
    ui->widget_editor->setLayout(layout);
//    ui->widget_editor->layout()->addWidget(m_code_editor);

    //设置语法高亮
    MyHighLighter *highlighter = new MyHighLighter(m_code_editor->document());

    Q_UNUSED(highlighter);

    //设置编辑器的字体
    QFont code_font = m_code_editor->font();
    code_font.setPointSize(12);
    m_code_editor->setFont(code_font);

    m_code_process = new QProcess(this);
    connect(m_code_process , SIGNAL(readyReadStandardOutput()) , this , SLOT(on_readoutput()));
    connect(m_code_process , SIGNAL(readyReadStandardError()) , this , SLOT(on_readerror()));
    connect(m_code_process,SIGNAL(finished(int)),this,SLOT(on_readResult(int)));

    isMode_run = false;
    m_fileOpen_path = "./";//初始为当前目录

    get_lineEdit_ptr(LineEdit_parameter)->setPlaceholderText("default dll is tmp.dll");
}

void CodeEditor_form::Destroy_cores()
{
    if(m_code_process){
      m_code_process->close();
      m_code_process->waitForFinished();
    }
}

void CodeEditor_form::set_path(const QString path)
{
    m_path = path;
}

QString CodeEditor_form::get_path()
{
    return m_path;
}

QLineEdit *CodeEditor_form::get_lineEdit_ptr(CodeEditor_form::LineEdit_e obj)
{
    switch(obj){
    case LineEdit_parameter:
        return ui->lineEdit_parameter;
        break;
    default:
        qDebug() << "QLineEdit is invalid!";
        break;
    }

    return nullptr;
}

QPlainTextEdit *CodeEditor_form::get_PlainTextEdit_ptr(CodeEditor_form::PlainTextEdit_p obj)
{
    switch(obj){
    case PlainTextEdit_message:
        return ui->plainTextEdit_message;
        break;
    default:
        qDebug() << "PlainTextEdit is invalid!";
        break;
    }

    return nullptr;
}

void CodeEditor_form::sendMSG(const QString &str)
{
    //emit send_BottomMSG(str);
    this->get_PlainTextEdit_ptr(PlainTextEdit_message)->appendPlainText(str);
}

void CodeEditor_form::compile_handler(COMPILE_MODE mode){
    bool isOK = false;
    QMessageBox * mBox = new QMessageBox(this);
    mBox->setWindowTitle(QString::fromLocal8Bit("提示"));
    if(mode){
        mBox->setText(QString::fromLocal8Bit("是否保存后再运行"));
    }
    else{
        mBox->setText(QString::fromLocal8Bit("是否保存后再编译"));
    }
    mBox->setStandardButtons(QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);
    mBox->setButtonText(QMessageBox::StandardButton::Ok, QString::fromLocal8Bit("确定"));
    mBox->setButtonText(QMessageBox::StandardButton::Cancel, QString::fromLocal8Bit("取消"));
    mBox->exec();
    QMessageBox::StandardButton ret = mBox->standardButton(mBox->clickedButton());
    switch (ret)
    {
        case QMessageBox::NoButton:
        //        qDebug() << QString::fromLocal8Bit("没有选择").toStdString().c_str();
            break;
        case QMessageBox::Ok:
            isOK = true;
        //        qDebug() << QString::fromLocal8Bit("确定").toStdString().c_str();
            break;
        case QMessageBox::Cancel:
        //        qDebug() << QString::fromLocal8Bit("取消").toStdString().c_str();
            break;
        default:
            break;
    }
    if (mBox != nullptr)
    {
        delete mBox;
        mBox = nullptr;
    }

    if(!isOK) return;
    if(m_code_editor->toPlainText().isEmpty()) return;

    QFile file(m_path+"/../tmp/tmp.c");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(m_code_editor->toPlainText().toLocal8Bit());
    file.close();

    QDir temDir(m_path+"/../");
    QString path = temDir.absolutePath();
    QDir compile_Dir(m_path+"/../compile_tool");
    QString compile_path = compile_Dir.absolutePath();
//    qDebug() << path;
    QString dll_param;

    switch(mode){
       case MODE_COMPILE_APP:
        if(get_lineEdit_ptr(LineEdit_parameter)->text().isEmpty()){
            m_code_process->start("cmd.exe",QStringList() << "/c" << ("cd "+path+"/tmp")<<("&"+compile_path+"/tcc.exe") << (path+"/tmp/tmp.c") << "-o" << "tmp.exe");//"/c"表示只是运行一次
        }
        else {
            dll_param = get_lineEdit_ptr(LineEdit_parameter)->text();//添加了dll参数
            m_code_process->start("cmd.exe",QStringList() << "/c" << ("cd "+path+"/tmp")<<("&"+compile_path+"/tcc.exe") << (path+"/tmp/tmp.c") << dll_param << "-o" << "tmp.exe");//"/c"表示只是运行一次
        }

        isMode_run = false;
         break;
       case MODE_COMPILE_DLL:
//        m_code_process->start("cmd.exe",QStringList() << "/c" << (path+"/tcc.exe") << "-shared" << (path+"/tmp/tmp.c") );
        m_code_process->start("cmd.exe",QStringList() << "/c" << ("cd "+path+"/tmp")<<("&"+compile_path+"/tcc.exe") << "-shared" << (path+"/tmp/tmp.c") );
        isMode_run = false;
         break;
       case MODE_COMPILE_SCRIPT:
        m_code_process->start("cmd.exe",QStringList() << "/c" << (compile_path+"/tcc.exe") << "-run" << (path+"/tmp/tmp.c"));
        isMode_run = true;
         break;
       default:
        return;
         break;
    }


    m_code_process->waitForStarted();
}

void CodeEditor_form::loadFile(const QString &str){
    QFile file;
    file.setFileName(str);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {        
        m_code_editor->clear();
        QByteArray t ;
        while(!file.atEnd())
        {
            t += file.readLine();
        }
        m_code_editor->appendPlainText(QString::fromLocal8Bit(t));//QString::fromLocal8Bit(t)不会出现中文乱码的情况
        file.close();

        QFileInfo fileInfo(str);
        sendMSG("Open file ["+fileInfo.fileName() +" ] OK!");
    }
}

/******************* slots begin ******************/
void CodeEditor_form::on_readoutput()
{
    QString str;

     if(isMode_run){
         //ui->plainTextEdi_bottom->appendPlainText("[info]:"+QString::fromLocal8Bit(process->readAllStandardOutput()));
         str = QString::fromLocal8Bit(m_code_process->readAllStandardOutput());
         sendMSG(str);
     }
}

void CodeEditor_form::on_readerror()
{
    QString str;
    str = QString::fromLocal8Bit(m_code_process->readAllStandardError());
    QMessageBox::information(this,"info","error occurs!");
    sendMSG(str);
}

void CodeEditor_form::on_readResult(int exitCode)
{
    QString str;

    if(exitCode == 0){
    //qDebug() << "[info1]:"<<QString::fromLocal8Bit(process->readAllStandardOutput());
    if(isMode_run){
        //ui->plainTextEdi_bottom->appendPlainText("[info1]:"+QString::fromLocal8Bit(process->readAllStandardOutput()));
        str = QString::fromLocal8Bit(m_code_process->readAllStandardOutput());
        sendMSG(str);
    }
    else{
        if(m_code_process->readAllStandardOutput().isEmpty()){
//            ui->plainTextEdi_bottom->appendPlainText("[info1]: Compile OK\r\n");
            str = "Compile OK!";
            QMessageBox::information(this,"info","Compile OK!");
            sendMSG(str);
        }
    }
}
}


void CodeEditor_form::on_pushButton_code_compileApp_clicked()
{
   compile_handler(MODE_COMPILE_APP);
}

void CodeEditor_form::on_pushButton_code_compileDll_clicked(){
    compile_handler(MODE_COMPILE_DLL);
}

void CodeEditor_form::on_pushButton_saveDll_clicked()
{
    QString path = m_path+"/../tmp/";
    if(QFile::exists(path+"tmp.dll")){
        QString str_tmp = QFileDialog::getSaveFileName(this, QString("save path"), m_fileOpen_path,QString("file(*.dll)"));
        if(str_tmp.isEmpty()){
            return;
        }

        QFileInfo fi = QFileInfo(str_tmp);
//        file_name = fi.fileName();
        m_fileOpen_path = fi.absolutePath();

        if(QFile::exists(str_tmp)){//如果存在目标文件，则删除
            QFile::remove(str_tmp);
        }

        if(QFile::copy(path+"tmp.dll",str_tmp)){
            QMessageBox::information(this,"info","Save dll OK!");
        }
        else{
            QMessageBox::warning(this,"Warning","Save dll error!");
        }
    }
    else{
        QMessageBox::warning(this,"Warning","Please compile first!");
    }
}

void CodeEditor_form::on_pushButton_code_run_clicked()
{
    QDir temDir(m_path+"/../");
    QString path = temDir.absolutePath();

    m_code_process->start("cmd.exe",QStringList() << "/c" << (path+"/tmp/tmp.exe"));
    isMode_run = true;
    m_code_process->waitForStarted();
}

void CodeEditor_form::on_pushButton_code_save_clicked()
{
    QString e = m_code_editor->toPlainText();
    QFile file;
    QString str_tmp = QFileDialog::getSaveFileName(this, QString("save path"), m_fileOpen_path,QString("file(*.c)"));
    if(str_tmp.isEmpty()){
        return;
    }

    QFileInfo fi = QFileInfo(str_tmp);
//        file_name = fi.fileName();
    m_fileOpen_path = fi.absolutePath();

    file.setFileName(str_tmp);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(e.toLocal8Bit());
    file.close();
}


void CodeEditor_form::on_pushButton_code_open_clicked()
{
    QString str_tmp = QFileDialog::getOpenFileName(this, QString("choose files"), m_fileOpen_path,QString("file(*.c)"));

    if(str_tmp.isEmpty()){
        return;
    }

    QFileInfo fi = QFileInfo(str_tmp);
//        file_name = fi.fileName();
    m_fileOpen_path = fi.absolutePath();

    loadFile(str_tmp);
}

void CodeEditor_form::on_pushButton_code_test_clicked()
{
   compile_handler(MODE_COMPILE_SCRIPT);
}

void CodeEditor_form::on_pushButton_load_template_clicked()
{
    QDir temDir(m_path+"/../compile_tool");
    QString path = temDir.absolutePath();

    loadFile(path+"/template/template.c");
}

void CodeEditor_form::on_pushButton_messageClear_clicked()
{
    this->get_PlainTextEdit_ptr(PlainTextEdit_message)->clear();
}

/******************* slots end ******************/



