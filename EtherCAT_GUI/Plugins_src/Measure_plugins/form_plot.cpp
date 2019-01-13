#include "form_plot.h"
#include "ui_form_plot.h"

#include <QTime>

Form_plot::Form_plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_plot)
{
    ui->setupUi(this);

    Init_Plots();

    m_timePlot = new QTimer();
    connect(m_timePlot,SIGNAL(timeout()),this,SLOT(user_timeout_handle()));
}

Form_plot::~Form_plot()
{
    m_timePlot->stop();

    delete ui;
}

void Form_plot::Init_Plots(){
    QVector<double> m_x(101),m_y(101);
    for(int i=0;i<101;i++){
        m_x[i] = i/50.0-1;
        m_y[i] = sin(m_x[i]*3.14);
    }
    // 初始化
//    QCustomPlot *m_CustomPlot = new QCustomPlot;
    // CustomPlot的基础功能设置
    get_customPlot()->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iMultiSelect  | QCP::iSelectOther | QCP::iSelectItems);
    // 基础功能共有以下几种，大体意思是：
    // 1、轴可拖动     2、通过鼠标滚轮改变轴显示范围 3、用户可以选择多个对象,设定的修饰符（不是特别明白）
    // 4、图形是可选的  5、轴是可选的 6、图例是可选的。。。
    /*enum Interaction { iRangeDrag         = 0x001 ///< <tt>0x001</tt> Axis ranges are draggable (see \ref QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeDragAxes)
                         ,iRangeZoom        = 0x002 ///< <tt>0x002</tt> Axis ranges are zoomable with the mouse wheel (see \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeZoomAxes)
                         ,iMultiSelect      = 0x004 ///< <tt>0x004</tt> The user can select multiple objects by holding the modifier set by \ref QCustomPlot::setMultiSelectModifier while clicking
                         ,iSelectPlottables = 0x008 ///< <tt>0x008</tt> Plottables are selectable (e.g. graphs, curves, bars,... see QCPAbstractPlottable)
                         ,iSelectAxes       = 0x010 ///< <tt>0x010</tt> Axes are selectable (or parts of them, see QCPAxis::setSelectableParts)
                         ,iSelectLegend     = 0x020 ///< <tt>0x020</tt> Legends are selectable (or their child items, see QCPLegend::setSelectableParts)
                         ,iSelectItems      = 0x040 ///< <tt>0x040</tt> Items are selectable (Rectangles, Arrows, Textitems, etc. see \ref QCPAbstractItem)
                         ,iSelectOther      = 0x080 ///< <tt>0x080</tt> All other objects are selectable (e.g. your own derived layerables, the plot title,...)
                     };*/
    // 设置矩形边框
    get_customPlot()->axisRect()->setupFullAxesBox();
    // 清空CustomPlot中的图形
    get_customPlot()->clearGraphs();
    // 在CustomPlot中添加图形
    get_customPlot()->addGraph();
    // 设置图形中的数据m_x和m_y是两个QVector容器
    get_customPlot()->graph(0)->setData(m_x, m_y);
    // 这个是设置图形显示为合适范围（感觉设置的只是Y轴）
    get_customPlot()->graph(0)->rescaleValueAxis(true);
    // 设置X轴的显示范围（这里是4条轴，x是下面那条，x2是上面那条，Y是先左后右）
    get_customPlot()->xAxis->setRange(m_x.at(0) - 1, m_x.at(m_x.size() - 1) + 1 );
    // 刷新m_CustomPlot中数据
    get_customPlot()->replot();
}

QCustomPlot *Form_plot::get_customPlot(){
    return ui->widget_customPlot;
}



void Form_plot::on_pushButton_PlotStart_clicked()
{

    m_timePlot->start(10);
}

void Form_plot::on_pushButton_PlotStop_clicked()
{
   m_timePlot->stop();
}

void Form_plot::user_timeout_handle(){
    QVector<double> m_x(101),m_y(101);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    for(int i=0;i<101;i++){
        m_x[i] = (i/50.0-1);
        m_y[i] = sin(m_x[i]*3.14)*(qrand()%10);
    }
    get_customPlot()->graph(0)->setData(m_x, m_y);
    get_customPlot()->graph(0)->rescaleValueAxis(true);
    get_customPlot()->replot();
}
