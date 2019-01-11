#include "GcodeParser.h"

GcodeParser::GcodeParser(QObject *parent) : QObject(parent)
{
  m_Gcode_Q = new QQueue<Gcode_segment>();

  m_currentPoint = QVector3D(0,0,0);//初始化变量
  m_isMetric = true;
  m_inAbsoluteMode = true;
  m_inAbsoluteIJKMode = false;
  m_lastGcodeCommand = -1;
  m_commandNumber = 0;
}

/**
* Add a command to be processed.
*/
int GcodeParser::addCommand(QString command)
{
    QString stripped = GcodePreprocessorUtils::removeComment(command);
    QStringList args = GcodePreprocessorUtils::splitCommand(stripped);
    return this->addCommand(args);
}

/**
* Add a command which has already been broken up into its arguments.
*/
int GcodeParser::addCommand(const QStringList &args)
{
    if (args.isEmpty()) {
        return NULL;
    }
    return processCommand(args);//处理命令
}

int GcodeParser::processCommand(const QStringList &args)
{
    static int xx = 0;
    QList<float> gCodes;
//    PointSegment *ps = NULL;

    // Handle F code
    double speed = GcodePreprocessorUtils::parseCoord(args, 'F');
//    if (!qIsNaN(speed)) this->m_lastSpeed = this->m_isMetric ? speed : speed * 25.4;

    // Handle S code
    double spindleSpeed = GcodePreprocessorUtils::parseCoord(args, 'S');
//    if (!qIsNaN(spindleSpeed)) this->m_lastSpindleSpeed = spindleSpeed;

    // Handle P code
    double dwell = GcodePreprocessorUtils::parseCoord(args, 'P');
//    if (!qIsNaN(dwell)) this->m_points.last()->setDwell(dwell);

    // handle G codes.
    gCodes = GcodePreprocessorUtils::parseCodes(args, 'G');

    // If there was no command, add the implicit one to the party.
//    if (gCodes.isEmpty() && m_lastGcodeCommand != -1) {//注释行
//        gCodes.append(m_lastGcodeCommand);
////        qDebug() << "ssd"<<xx++<<args;
//    }
  //NOTE:M代码必须要是新行才能识别
    if(gCodes.isEmpty()){//说明不是Gcode
        //解析Mcode
        QList<float> mCodes;
        mCodes = GcodePreprocessorUtils::parseCodes(args,'M');
        if(mCodes.isEmpty()){
            mCodes.append(Gcode_segment::Invalid_Mcode);//如果既不是Gcode也不是Mcode,则赋值为M-2表示无效代码
        }
        foreach (float code, mCodes) {
    //        ps = handleGCode(code, args);
            handleMCode(code);
//            qDebug() << code;
        }
    }
    else{
        //m_Gcode_Q->clear();
        foreach (float code, gCodes) {
    //        ps = handleGCode(code, args);
            handleGCode(code, args);
        }
    }


//    return ps;
    return 0;
}

int GcodeParser::handleMCode(float code){
    int Mcode_temp = Gcode_segment::No_Mcode;
    if(code == 100.0f) Mcode_temp = Gcode_segment::MotorTool_On;
    else if(code == 101.0f)  Mcode_temp = Gcode_segment::MotorTool_Off;
    Gcode_segment segment;
    segment.line = m_Gcode_Q->size();
    segment.data_xyz = this->m_currentPoint;
    segment.Mcode = Mcode_temp;
    m_Gcode_Q->enqueue(segment);
//    qDebug() << "handleMCode:"<<Mcode_temp;

    return 0;
}


int GcodeParser::handleGCode(float code, const QStringList &args)
{
//    PointSegment *ps = NULL;
    this->m_inAbsoluteMode = true;//设置为绝对坐标
    QVector3D nextPoint = GcodePreprocessorUtils::updatePointWithCommand(args, this->m_currentPoint, this->m_inAbsoluteMode);
//    qDebug() << qIsNaN(x)<< qIsNaN(y)<< qIsNaN(z);
//    qDebug() << nextPoint;
    this->m_currentPoint = nextPoint;
    Gcode_segment segment;
    segment.line = m_Gcode_Q->size();
    segment.data_xyz = nextPoint;
    segment.Mcode = Gcode_segment::No_Mcode;
    m_Gcode_Q->enqueue(segment);

//    if (code == 0.0f) ps = addLinearPointSegment(nextPoint, true);
//    else if (code == 1.0f) ps = addLinearPointSegment(nextPoint, false);
//    else if (code == 38.2f) ps = addLinearPointSegment(nextPoint, false);
//    else if (code == 2.0f) ps = addArcPointSegment(nextPoint, true, args);
//    else if (code == 3.0f) ps = addArcPointSegment(nextPoint, false, args);
//    else if (code == 17.0f) this->m_currentPlane = PointSegment::XY;
//    else if (code == 18.0f) this->m_currentPlane = PointSegment::ZX;
//    else if (code == 19.0f) this->m_currentPlane = PointSegment::YZ;
//    else if (code == 20.0f) this->m_isMetric = false;
//    else if (code == 21.0f) this->m_isMetric = true;
//    else if (code == 90.0f) this->m_inAbsoluteMode = true;
//    else if (code == 90.1f) this->m_inAbsoluteIJKMode = true;
//    else if (code == 91.0f) this->m_inAbsoluteMode = false;
//    else if (code == 91.1f) this->m_inAbsoluteIJKMode = false;

//    if (code == 0.0f || code == 1.0f || code == 2.0f || code == 3.0f || code == 38.2f) this->m_lastGcodeCommand = code;

//    return ps;
    return 0;
}

int GcodeParser::addLinearPointSegment(const QVector3D &nextPoint, bool fastTraverse)
{
//    PointSegment *ps = new PointSegment(&nextPoint, m_commandNumber++);

//    bool zOnly = false;

//    // Check for z-only
//    if ((this->m_currentPoint.x() == nextPoint.x()) &&
//            (this->m_currentPoint.y() == nextPoint.y()) &&
//            (this->m_currentPoint.z() != nextPoint.z())) {
//        zOnly = true;
//    }

//    ps->setIsMetric(this->m_isMetric);
//    ps->setIsZMovement(zOnly);
//    ps->setIsFastTraverse(fastTraverse);
//    ps->setIsAbsolute(this->m_inAbsoluteMode);
//    ps->setSpeed(fastTraverse ? this->m_traverseSpeed : this->m_lastSpeed);
//    ps->setSpindleSpeed(this->m_lastSpindleSpeed);
//    this->m_points.append(ps);

//    // Save off the endpoint.
//    this->m_currentPoint = nextPoint;

//    return ps;

    return 0;
}

QQueue<Gcode_segment>* GcodeParser::getGodeQueue(){
    return m_Gcode_Q;
}

void GcodeParser::clearQueue(){
    m_Gcode_Q->clear();
}

void GcodeParser::setCurrentPoint(const QVector3D point)
{
    m_currentPoint = point;
}
