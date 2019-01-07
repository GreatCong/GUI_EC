#include "formcomm.h"
#include "ui_formcomm.h"

FormComm::FormComm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormComm)
{
    ui->setupUi(this);
}

FormComm::~FormComm()
{
    delete ui;
}
