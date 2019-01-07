#include "mainformview.h"

///
/// \brief 用于测试的对象初始化
///
void MainFormView::Init_TestCores(){
    mm_time = new QTimer();
    connect(mm_time,SIGNAL(timeout()),this,SLOT(mm_timerOut()));

    Init_Plots();
}

#include "qcustomplot.h"
void MainFormView::Init_Plots(){
    QVector<double> m_x(101),m_y(101);
    for(int i=0;i<101;i++){
        m_x[i] = i/50.0-1;
        m_y[i] = sin(m_x[i]*3.14);
    }
    // 初始化
    QCustomPlot *m_CustomPlot = new QCustomPlot;
    // CustomPlot的基础功能设置
    m_CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iMultiSelect  | QCP::iSelectOther | QCP::iSelectItems);
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
    m_CustomPlot->axisRect()->setupFullAxesBox();
    // 清空CustomPlot中的图形
    m_CustomPlot->clearGraphs();
    // 在CustomPlot中添加图形
    m_CustomPlot->addGraph();
    // 设置图形中的数据m_x和m_y是两个QVector容器
    m_CustomPlot->graph(0)->setData(m_x, m_y);
    // 这个是设置图形显示为合适范围（感觉设置的只是Y轴）
    m_CustomPlot->graph(0)->rescaleValueAxis(true);
    // 设置X轴的显示范围（这里是4条轴，x是下面那条，x2是上面那条，Y是先左后右）
    m_CustomPlot->xAxis->setRange(m_x.at(0) - 1, m_x.at(m_x.size() - 1) + 1 );
    // 刷新m_CustomPlot中数据
    m_CustomPlot->replot();

    mtabWeidgetItem_Messure->layout()->addWidget(m_CustomPlot);
}

/************  Slots *****************/

///
/// \brief 扫描从站
///
void MainFormView::mDeviceTree_EcScan_clicked(){
//  mm_time->start(50);//1000ms
//  timer_num = 0;
//  m_scanWait_processBar->show();
////  Set_StatusMessage(tr("Waiting for Scanning!"),3000);
//  m_status_label->setText(tr("Scanning..."));
    Master_scan();
}

///
/// \brief MainFormView::AddTree_Master_clicked
///
void MainFormView::AddTree_Master_clicked(){
    mDeviceTree->Add_LeftTree_Master();
}

void MainFormView::AddTree_Slave_clicked(){
    mDeviceTree->Add_LeftTree_Slaves();
}

void MainFormView::AddTree_SlaveItem_clicked(){
    mDeviceTree->Add_LeftTree_SlaveItems();
}

void MainFormView::AddTree_SlaveMessageInput_clicked(){
    mDeviceTree->Add_LeftTree_SlaveMessage(1,1,MARK_SLAVE_ITEM_INPUT-MARK_SLAVE_ITEM);
}

void MainFormView::AddTree_SlaveMessageOutput_clicked(){
    mDeviceTree->Add_LeftTree_SlaveMessage(1,1,MARK_SLAVE_ITEM_OUTPUT-MARK_SLAVE_ITEM);
}
/************  Slots end ***************/
