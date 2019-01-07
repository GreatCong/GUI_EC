#include "mainformview.h"

//用于解析G代码
#define PROGRESSMINLINES 10000 //G代码文件的最大行数
#define PROGRESSSTEP     1000



///
/// \brief 初始化中心内容框
///
void MainFormView::Init_FrameCenter_Content()
{
//    this->user_formComm = new FormComm();//定义父类的时候会调用show
//    ui->frame_center_main->layout()->addWidget(user_formComm);

    mTabWedget_center = new QTabWidget();
    QGridLayout *user_layout = new QGridLayout();
    mtabWeidgetItem_General = new QWidget();
    user_layout = new QGridLayout();
    mtabWeidgetItem_General->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_General,tr("General"));
//    QVBoxLayout *h_layout = new QVBoxLayout();
//    QFrame *xx1 = new QFrame();
//    QTableWidget *xx2 = new QTableWidget();
//    xx2->setColumnCount(2);
//    xx2->setHorizontalHeaderLabels(QStringList()<<tr("Index")<<tr("Content"));
//    xx2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽
//    h_layout->addWidget(xx1,1);
//    h_layout->addWidget(xx2,1);
//    mtabWeidgetItem_General->setLayout(h_layout);

    user_form_generalTab = new Form_GeneralTab();
    mtabWeidgetItem_General->layout()->addWidget(user_form_generalTab);


    mtabWeidgetItem_Messure = new QWidget();
    user_layout = new QGridLayout();
    mtabWeidgetItem_Messure->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_Messure,tr("Messure"));

    mtabWeidgetItem_Control = new QWidget();
    user_layout = new QGridLayout();
    mtabWeidgetItem_Control->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_Control,tr("Control"));

    QPushButton *button_openGcode = new QPushButton(tr("Open Gcode"));
    connect(button_openGcode,SIGNAL(clicked(bool)),this,SLOT(Control_OpenGcode_clicked()));
    QPushButton *button_reloadGcode = new QPushButton(tr("Reload Gcode"));
    connect(button_reloadGcode,SIGNAL(clicked(bool)),this,SLOT(Control_ReloadGcode_clicked()));
    QPushButton *button_sendGcode = new QPushButton(tr("Send Gcode"));
    connect(button_sendGcode,SIGNAL(clicked(bool)),this,SLOT(Control_SendGcode_clicked()));

    m_lineEdit_GcodePath = new QLineEdit();
    m_lineEdit_GcodePath->setPlaceholderText(tr("Gcode Path"));
    m_lineEdit_XPos= new QLineEdit(tr("280.799"));
    m_lineEdit_XPos->setPlaceholderText(tr("X轴"));
    m_lineEdit_YPos= new QLineEdit(tr("0"));
    m_lineEdit_YPos->setPlaceholderText(tr("Y轴"));
    m_lineEdit_ZPos= new QLineEdit(tr("155"));
    m_lineEdit_ZPos->setPlaceholderText(tr("Z轴"));
    m_lineEdit_Pos_Step= new QLineEdit(tr("1"));
    m_lineEdit_Pos_Step->setPlaceholderText(tr("步长"));
    m_lineEdit_Pos_Speed= new QLineEdit(tr("1000"));
    m_lineEdit_Pos_Speed->setPlaceholderText(tr("速度"));

    m_table_Gcode = new QTableWidget();
    m_table_Gcode->setColumnCount(3);
    m_table_Gcode->setHorizontalHeaderLabels(QStringList()<<tr("#")<<tr("Gcode")<<tr("State"));
    m_table_Gcode->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置等宽
    m_table_Gcode->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);//对第1列单独设置根据内容设置宽度
    m_table_Gcode->verticalHeader()->setVisible(false); //设置垂直头不可见

    QHBoxLayout *h_layout_pos = new QHBoxLayout();
    h_layout_pos->addWidget(button_openGcode);
    h_layout_pos->addWidget(button_reloadGcode);
    user_layout->addLayout(h_layout_pos,0,0);
    user_layout->addWidget(m_lineEdit_GcodePath);
    user_layout->addWidget(m_table_Gcode);
    user_layout->addWidget(button_sendGcode);
    h_layout_pos = new QHBoxLayout();
    h_layout_pos->addWidget(new QLabel(tr("X轴")));
    h_layout_pos->addWidget(m_lineEdit_XPos);
    h_layout_pos->addWidget(new QLabel(tr("Y轴")));
    h_layout_pos->addWidget(m_lineEdit_YPos);
    h_layout_pos->addWidget(new QLabel(tr("Z轴")));
    h_layout_pos->addWidget(m_lineEdit_ZPos);
    user_layout->addLayout(h_layout_pos,4,0);
    h_layout_pos = new QHBoxLayout();
    h_layout_pos->addWidget(new QLabel(tr("步长")));
    h_layout_pos->addWidget(m_lineEdit_Pos_Step);
    h_layout_pos->addWidget(new QLabel(tr("速度")));
    h_layout_pos->addWidget(m_lineEdit_Pos_Speed);
    user_layout->addLayout(h_layout_pos,5,0);

//    mtabWeidgetItem_Control->layout()->addWidget(button_openGcode);
//    mtabWeidgetItem_Control->layout()->addWidget(button_reloadGcode);
//    mtabWeidgetItem_Control->layout()->addWidget(m_lineEdit_GcodePath);
//    mtabWeidgetItem_Control->layout()->addWidget(m_table_Gcode);
//    mtabWeidgetItem_Control->layout()->addWidget(button_sendGcode);
//    mtabWeidgetItem_Control->layout()->addWidget(m_lineEdit_XPos);
//    mtabWeidgetItem_Control->layout()->addWidget(m_lineEdit_YPos);
//    mtabWeidgetItem_Control->layout()->addWidget(m_lineEdit_ZPos);

    mtabWeidgetItem_UserApps = new QWidget();
    user_layout = new QGridLayout();
    mtabWeidgetItem_UserApps->setLayout(user_layout);
    mTabWedget_center->addTab(mtabWeidgetItem_UserApps,tr("User Application"));

    getCenterWidget()->layout()->addWidget(mTabWedget_center);

//    QTableView *xx = new QTableView();
//    xx_model = new GCodeTableModel();
////    xx_model->setHorizontalHeaderLabels(QStringList()<<tr("项目"));
//    xx->setModel(xx_model);
//    mtabWeidgetItem_UserApps->layout()->addWidget(xx);
//    GCodeItem xx_item;
//    xx_item.command = "sss";
//    xx_item.state =  GCodeItem::InQueue;
//    xx_item.response = "OK";
//    xx_item.line = 1;
//    xx_model->data().append(xx_item);
//    GCodeItem xx_item1;
//    xx_item1.command = "sss";
//    xx_item1.state =  GCodeItem::InQueue;
//    xx_item1.response = "OK";
//    xx_item1.line = 2;
//    xx_model->data().append(xx_item1);
//     xx_item1.line = 3;
//     xx_model->data().append(xx_item1);
//     xx->selectRow(1);
//     xx_model->insertRow(xx_model->rowCount());


    //添加用户自定义的Application
    mTabWedget_center->hide();

    //添加slave信息显示的widget
    m_widget_slaveMSG = new QWidget();
    QVBoxLayout *v_layout = new QVBoxLayout();
    m_tableView_slaveMSG = new My_Table_SlaveMSG();
    m_tableView_slaveItemMSG = new My_Table_SlaveItemMSG();
    v_layout->addWidget(m_tableView_slaveMSG,1);
    v_layout->addWidget(m_tableView_slaveItemMSG,3);
    m_widget_slaveMSG->setLayout(v_layout);

    getCenterWidget()->layout()->addWidget(m_widget_slaveMSG);
    m_widget_slaveMSG->hide();//暂时不显示

    m_tableView_slaveMSG->init();
    m_tableView_slaveItemMSG->init();
}

int MainFormView::Gcode_load(QString &fileName){
    if(!fileName.isEmpty()){
        //m_pluginDir = dir;
  //        qDebug() << m_pluginDir;
//        qDebug() << dir;
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, this->windowTitle(), tr("Can't open file:\n") + fileName);
            return -1;
        }




//        // Set filename
//        m_programFileName = fileName;

        // Prepare text stream
        QTextStream textStream(&file);

        // Read lines
        QList<QString> data;
        while (!textStream.atEnd()) data.append(textStream.readLine());

//        qDebug() << data;

        //读取信息后，进行G代码的解析
        // Prepare parser
//        GcodeParser gp;
//        GcodeParser *gp_t = new GcodeParser();

        QProgressDialog progress(tr("Opening file..."), tr("Abort"), 0, data.count(), this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setFixedSize(progress.sizeHint());
        if (data.count() > PROGRESSMINLINES) {//如果行数较大，就进行拆分
            progress.show();
            progress.setStyleSheet("QProgressBar {text-align: center; qproperty-format: \"\"}");
        }

        QString command;
        QString stripped;
        QString trimmed;
        QList<QString> args;

//        GCodeItem item;
        gp_t->clearQueue();
        m_table_Gcode->clearContents();
        int line_num = 0;
        QTableWidgetItem *tableItem;
        bool isCommentLine = false;
        while (!data.isEmpty())
        {
            command = data.takeFirst();

            // Trim command
            trimmed = command.trimmed();//移除字符串两端的空白字符

            if (!trimmed.isEmpty()) {
                // Split command
                stripped = GcodePreprocessorUtils::removeComment(command);
                if(stripped.isEmpty()){
                    isCommentLine = true;
                }
                else{
                    isCommentLine = false;
                }
//                qDebug() << stripped;
                args = GcodePreprocessorUtils::splitCommand(stripped);

//                gp.addCommand(args);//里面包含了handle
                gp_t->addCommand(args);//处理命令
//                m_GcodeSegment_Q = gp_t->getGodeQueue();
                if(isCommentLine){//空的表示是注释行
                    Gcode_segment segment;//插入M代码，让行对应
                    segment.line = gp_t->getGodeQueue()->size();
                    segment.data_xyz = QVector3D(0,0,0);//注释的M代码位置无效
                    segment.Mcode = Gcode_segment::COMMENT_CODE;
                    gp_t->getGodeQueue()->enqueue(segment);

                }
//                while(!gp_t->getGodeQueue()->empty())
//                qDebug() << gp_t->getGodeQueue()->dequeue().data_xyz;
                  //添加到table中
                m_table_Gcode->setRowCount(1+line_num);
                tableItem = new QTableWidgetItem(QString::number(line_num));
                tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_table_Gcode->setItem(line_num,0,tableItem);

                tableItem = new QTableWidgetItem(trimmed);
                tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_table_Gcode->setItem(line_num,1,tableItem);

                tableItem = new QTableWidgetItem(tr("Ready"));
                tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                m_table_Gcode->setItem(line_num++,2,tableItem);

//                item.command = trimmed;
//                item.state = GCodeItem::InQueue;
//                item.line = gp.getCommandNumber();
//                item.args = args;

//                m_programModel.data().append(item);
            }

            if (progress.isVisible() && (data.count() % PROGRESSSTEP == 0)) {
                progress.setValue(progress.maximum() - data.count());
                qApp->processEvents();
                if (progress.wasCanceled()) break;
            }
        }
        progress.close();

        m_GcodeSegment_Q = gp_t->getGodeQueue();//传递指针
        //加一行，表示end
        m_table_Gcode->setRowCount(1+line_num);
        tableItem = new QTableWidgetItem(QString::number(line_num));
        tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_table_Gcode->setItem(line_num,0,tableItem);

        tableItem = new QTableWidgetItem(tr("End"));
        tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_table_Gcode->setItem(line_num,1,tableItem);

        tableItem = new QTableWidgetItem(tr("Ready"));
        tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        m_table_Gcode->setItem(line_num++,2,tableItem);

        Gcode_segment segment;//插入M代码，让行对应
        segment.line = gp_t->getGodeQueue()->size();
        segment.data_xyz = QVector3D(0,0,0);//M代码位置无效
        segment.Mcode = Gcode_segment::EndParse_CODE;
        gp_t->getGodeQueue()->enqueue(segment);


////实现滚动效果
//m_table_Gcode->selectRow(250);
//m_table_Gcode->scrollTo(m_table_Gcode->model()->index(250,1));

//        // Load lines
//        loadFile(data);
//        while(!gp_t->getGodeQueue()->empty())
//        qDebug() << "xx:"<<gp_t->getGodeQueue()->dequeue().data_xyz;
//        if(gp_t->getGodeQueue()->empty()){
//            qDebug() << "mainview_center Q empty!";
//        }
//        else{
//            qDebug() << "mainview_center Q No empty!";
//        }
    }

    return 0;
}

/************  Slots *******************/
void MainFormView::Control_ReloadGcode_clicked(){
    if(user_form_generalTab->master->Master_getSlaveCount()>0){
       Master_stop();//防止界面卡死
       StatusMessage_change(tr("Stop Master..."),3000);
    }
    int ret = Gcode_load(m_GcodePath_full);
    if(ret ==0){
        m_bottomText->appendPlainText(tr("Reload Gcode file OK!"));
    }
}

void MainFormView::Control_OpenGcode_clicked(){
    if(user_form_generalTab->master->Master_getSlaveCount()>0){
       Master_stop();//防止界面卡死
       StatusMessage_change(tr("Stop Master..."),3000);
    }
    StatusMessage_change(tr("Stop Master..."),3000);
    QString fileName = "";
    fileName  = QFileDialog::getOpenFileName(this, tr("Open Gcode"), m_GcodePath,
                               tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt);;All files (*.*)"));//如果没有选择路径就会为空
    if(!fileName.isEmpty()){
        m_GcodePath_full = fileName;
        //分离path和fileName
        QFileInfo fileInfo = QFileInfo(fileName);
//        qDebug() << fileInfo.fileName() <<fileInfo.absolutePath();
        m_GcodePath = fileInfo.absolutePath();

        m_lineEdit_GcodePath->setText(fileName);
        m_bottomText->appendPlainText(tr("OPen Gcode file OK!"));
    }

    Gcode_load(m_GcodePath_full);
}

void MainFormView::Control_SendGcode_clicked(){

}

void MainFormView::MotorCallback_GcodeLineChange(int line){
//    qDebug() << line;
    //实现滚动效果
    m_table_Gcode->selectRow(line);
    m_table_Gcode->scrollTo(m_table_Gcode->model()->index(line,QTableView::EnsureVisible));
    int current_row = m_table_Gcode->currentRow();
    QTableWidgetItem *tableItem = new QTableWidgetItem(tr("OK"));
    tableItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_table_Gcode->setItem(current_row,2,tableItem);

}

/************  Slots end ***************/
