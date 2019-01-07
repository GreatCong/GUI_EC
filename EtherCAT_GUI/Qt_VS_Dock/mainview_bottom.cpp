#include "mainformview.h"

///
/// \brief 初始化下面的内容框
///
void MainFormView::Init_FrameBottom_Content()
{
    QPushButton *button_clear = new QPushButton(tr("Clear"));
    getBottomWidget()->layout()->addWidget(button_clear);
    connect(button_clear,SIGNAL(clicked(bool)),this,SLOT(BottonText_Clear_clicked()));
    m_bottomText = new QPlainTextEdit(tr("Generate Code OK!"));
    getBottomWidget()->layout()->addWidget(m_bottomText);
}

/************  Slots *******************/
///
/// \brief 清除输出信息 点击
///
void MainFormView::BottonText_Clear_clicked()
{
   m_bottomText->clear();
}
/************  Slots end ***************/
