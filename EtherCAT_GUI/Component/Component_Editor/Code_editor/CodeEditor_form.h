#ifndef CODEEDITOR_FORM_H
#define CODEEDITOR_FORM_H

#include <QWidget>

#include "src/codeeditor.h"
#include <QProcess>
#include <QString>
#include <QLineEdit>
#include <QPlainTextEdit>

namespace Ui {
class CodeEditor_form;
}

class CodeEditor_form : public QWidget
{
    Q_OBJECT

public:
    explicit CodeEditor_form(QWidget *parent = 0);
    ~CodeEditor_form();
public:
    typedef enum{
        LineEdit_parameter
    }LineEdit_e;

    typedef enum{
        PlainTextEdit_message
    }PlainTextEdit_p;

    void Init_cores();
    void Destroy_cores();
    void set_path(const QString path);
    QString get_path();
    QLineEdit *get_lineEdit_ptr(LineEdit_e obj);
    QPlainTextEdit *get_PlainTextEdit_ptr(PlainTextEdit_p obj);
private:
   CodeEditor *m_code_editor;
   QProcess *m_code_process;
   bool isMode_run;
   QString m_path;
   QString m_fileOpen_path;

   void sendMSG(const QString &str);

   typedef enum{
       MODE_COMPILE_APP,
       MODE_COMPILE_DLL,
       MODE_COMPILE_SCRIPT
   }COMPILE_MODE;

private:
    Ui::CodeEditor_form *ui;

    void compile_handler(COMPILE_MODE mode);
    void loadFile(const QString &str);
private slots:
    void on_readoutput();
    void on_readerror();
    void on_readResult(int exitCode);

    void on_pushButton_code_run_clicked();

    void on_pushButton_code_save_clicked();

    void on_pushButton_code_open_clicked();

    void on_pushButton_code_test_clicked();

    void on_pushButton_load_template_clicked();

    void on_pushButton_code_compileApp_clicked();

    void on_pushButton_code_compileDll_clicked();

    void on_pushButton_saveDll_clicked();

    void on_pushButton_messageClear_clicked();

signals:
    void send_BottomMSG(const QString &str);
};

#endif // CODEEDITOR_FORM_H
