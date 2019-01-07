#include "mainformview.h"

/********* 自定义消息处理 **********/
///
/// \brief 设置状态栏的信息
/// \param message
///
void MainFormView::Set_StatusMessage(QString message, int interval)
{
    emit StatusMessage_change(message,interval);//发出自定义信号
}

///
/// \brief 设置状态栏的widget
/// \param widget
/// \param isAdd
///
void MainFormView::Set_StatusWidget(QWidget *widget, bool isAdd)
{
     emit StatusWidget_change(widget,isAdd);//发出自定义信号
}

/********* 自定义消息处理 END **********/
