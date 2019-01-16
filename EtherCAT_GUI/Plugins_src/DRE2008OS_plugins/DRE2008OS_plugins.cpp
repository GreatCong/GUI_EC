#include "DRE2008OS_plugins.h"
#include <QGridlayout>
#include <QPushButton>

//#include "my_queue.h"

DRE2008OS_plugin::DRE2008OS_plugin(QObject *parent) :
    QObject(parent),EtherCAT_UserApp()
{
    m_userWidget = new Form_plot();
    m_userCallback = new DRE2008_OS_Callback();

    set_UIWidgetPtr(m_userWidget);
    set_CallbackPtr(m_userCallback);
    set_MessageObj(nullptr);
}

DRE2008OS_plugin::~DRE2008OS_plugin(){
    //    delete m_userWidget;
}

int DRE2008OS_plugin::Check_SampleParameter(int OS_ch,int AD_ch,int sampleRate)
{
    int ret = 0;

    switch(OS_ch){
    case DRE2008_OS_Callback::OS_CH1:
        if(AD_ch > DRE2008_OS_Callback::AD_CH1){
            QMessageBox::warning(get_UIWidgetPtr(),tr("Warning"),tr("AD Channel is invalid!"));
            ret = -1;
        }
        break;
    case DRE2008_OS_Callback::OS_CH1_CH2:
        if(AD_ch > DRE2008_OS_Callback::AD_CH2){
            QMessageBox::warning(get_UIWidgetPtr(),tr("Warning"),tr("AD Channel is invalid!"));
            ret = -1;
        }
        else{
            if(sampleRate > 20){
                QMessageBox::warning(get_UIWidgetPtr(),tr("Warning"),tr("SampleRate is invalid!"));
                ret = -2;
            }
        }
        break;
    case DRE2008_OS_Callback::OS_CH1_CH4:
        if(AD_ch > DRE2008_OS_Callback::AD_CH4){
            QMessageBox::warning(get_UIWidgetPtr(),tr("Warning"),tr("AD Channel is invalid!"));
            ret = -1;
        }
        else{
            if(sampleRate > 10){
                QMessageBox::warning(get_UIWidgetPtr(),tr("Warning"),tr("SampleRate is invalid!"));
                ret = -2;
            }
        }
        break;
    case DRE2008_OS_Callback::OS_CH1_CH8:
//        if(m_userCallback->m_AD_Channel > DRE2008_OS_Callback::AD_CH8){
//            QMessageBox::warning(get_UIWidgetPtr(),tr("Warning"),tr("AD Channel is invalid!"));
//        }
        if(sampleRate > 5){
            QMessageBox::warning(get_UIWidgetPtr(),tr("Warning"),tr("SampleRate is invalid!"));
            ret = -2;
        }
        break;
    case DRE2008_OS_Callback::OS_NONE:
        break;
    default:
        break;

    }

    return ret;
}

void DRE2008OS_plugin::Init_Cores()
{
    m_plotDisplay_Num = 1024;
    m_isMaster_Run = false;

    m_timePlot = new QTimer();
    connect(m_timePlot,SIGNAL(timeout()),this,SLOT(user_timeout_handle()));
    connect(m_userWidget->get_pushButtonPtr(Form_plot::Measure_start_b),SIGNAL(clicked(bool)),this,SLOT(plot_pushButton_PlotStart_clicked()));
    connect(m_userWidget->get_pushButtonPtr(Form_plot::Measure_stop_b),SIGNAL(clicked(bool)),this,SLOT(plot_pushButton_PlotStop_clicked()));
    connect(m_userWidget->get_comboBoxptr(Form_plot::Measure_ADchannel_c),SIGNAL(currentIndexChanged(int)),SLOT(plot_combobox_ADchannel_currentIndexChanged(int)));
    connect(m_userWidget->get_comboBoxptr(Form_plot::Measure_OSchannel_c),SIGNAL(currentIndexChanged(int)),SLOT(plot_combobox_OSchannel_currentIndexChanged(int)));
    connect(m_userWidget->get_DialPtr(Form_plot::Measure_SampleRate_d),SIGNAL(valueChanged(int)),SLOT(plot_dial_SampleRate_valueChanged(int)));
    connect(m_userWidget->get_DialPtr(Form_plot::Measure_DisplayNum_d),SIGNAL(valueChanged(int)),SLOT(plot_dial_DisplayNum_valueChanged(int)));

    connect(m_userCallback,SIGNAL(Master_RunStateChanged(bool)),this,SLOT(callback_Master_RunStateChanged(bool)));
//    qDebug() << "m_OS_Channel" << m_userCallback->m_OS_Channel;
//    qDebug() << "m_SamplingRate" << m_userCallback->m_SamplingRate;
//    qDebug() << "m_AD_Channel" << m_userCallback->m_AD_Channel;
}

void DRE2008OS_plugin::Destroy_Cores()
{
    m_timePlot->stop();
}

void DRE2008OS_plugin::plot_pushButton_PlotStart_clicked()
{

    m_userCallback->Set_OverSampling_run(true);
    m_timePlot->start(5);//刷新时间不能太快，会崩溃
    m_userWidget->get_pushButtonPtr(Form_plot::Measure_start_b)->setEnabled(false);
    m_userWidget->get_pushButtonPtr(Form_plot::Measure_stop_b)->setEnabled(true);

    m_userCallback->Set_OverSampling_Reset(false);//超采样初始化
    m_userCallback->m_measure_Q->clear();

}

void DRE2008OS_plugin::plot_pushButton_PlotStop_clicked()
{
    m_userCallback->Set_OverSampling_run(false);
    m_timePlot->stop();
    m_userWidget->get_pushButtonPtr(Form_plot::Measure_start_b)->setEnabled(true);
    m_userWidget->get_pushButtonPtr(Form_plot::Measure_stop_b)->setEnabled(false);

    m_userCallback->Set_OverSampling_Reset(false);//超采样初始化
    m_userCallback->m_measure_Q->clear();

}

/******************* SLOTs *********************************/
void DRE2008OS_plugin::user_timeout_handle(){
    int display_num = 1024*4;
    static QVector<double> m_x(display_num),m_y(display_num);

    m_userWidget->get_LineEditPtr(Form_plot::Measure_ErrState_e)->setText(m_userCallback->ErrorState_ToString());//显示Error

    int q_size = m_userCallback->m_measure_Q->size();
    if(q_size > display_num){
        for(int i=0;i<display_num;i++){
          m_x[i] = i;
          m_y[i] = m_userCallback->m_measure_Q->dequeue();
        }
    }
    else{
        return ;
    }

    m_userWidget->get_customPlot()->graph(0)->setData(m_x, m_y);
    m_userWidget->get_customPlot()->xAxis->setRange(-10, m_plotDisplay_Num);
    m_userWidget->get_customPlot()->graph(0)->rescaleValueAxis(true);
//    m_userWidget->get_customPlot()->graph(0)->rescaleValueAxis();
//    m_userWidget->get_customPlot()->yAxis->setRange(-20, 700);
    m_userWidget->get_customPlot()->replot();
}

void DRE2008OS_plugin::plot_dial_SampleRate_valueChanged(int value)
{ 

    if(Check_SampleParameter(m_userCallback->m_OS_Channel,m_userCallback->m_AD_Channel,value) < 0){//恢复上一次的值
        m_userWidget->get_DialPtr(Form_plot::Measure_SampleRate_d)->setValue(m_userCallback->m_SamplingRate);
    }
    else{
        m_userCallback->m_SamplingRate = value;
        m_userWidget->get_LineEditPtr(Form_plot::Measure_SampleRate_e)->setText(QString::number(m_userCallback->m_SamplingRate));
    }

}

void DRE2008OS_plugin::plot_dial_DisplayNum_valueChanged(int value)
{

    m_plotDisplay_Num = value;
    m_userWidget->get_LineEditPtr(Form_plot::Measure_DisplayNum_e)->setText(QString::number(m_plotDisplay_Num));

}

void DRE2008OS_plugin::plot_combobox_ADchannel_currentIndexChanged(int index)
{
   //参数从0开始,index也是从0开始
//    qDebug() << index;
    if(Check_SampleParameter(m_userCallback->m_OS_Channel,index,m_userCallback->m_SamplingRate) < 0){//恢复上一次的值
         m_userWidget->get_comboBoxptr(Form_plot::Measure_ADchannel_c)->setCurrentIndex(m_userCallback->m_AD_Channel);
    }
    else{
         m_userCallback->m_AD_Channel = index;
    }
}

void DRE2008OS_plugin::plot_combobox_OSchannel_currentIndexChanged(int index)
{
   //参数从0开始
    if(Check_SampleParameter(index,m_userCallback->m_AD_Channel,m_userCallback->m_SamplingRate) < 0){//恢复上一次的值
        m_userWidget->get_comboBoxptr(Form_plot::Measure_OSchannel_c)->setCurrentIndex(m_userCallback->m_OS_Channel);
    }
    else{
        m_userCallback->m_OS_Channel = index;
    }
}

void DRE2008OS_plugin::callback_Master_RunStateChanged(bool isRun)
{
//    m_is_EtherCAT_Run = isRun;
    if(isRun){
        m_userWidget->get_pushButtonPtr(Form_plot::Measure_start_b)->setEnabled(true);
        m_userWidget->get_pushButtonPtr(Form_plot::Measure_stop_b)->setEnabled(true);
    }
    else{
        m_userWidget->get_pushButtonPtr(Form_plot::Measure_start_b)->setEnabled(false);
        m_userWidget->get_pushButtonPtr(Form_plot::Measure_stop_b)->setEnabled(false);
    }
}
/******************* SLOTs  end ***********************/


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DRE2008OS_plugins, DRE2008OS_plugin)
#endif // QT_VERSION < 0x050000
