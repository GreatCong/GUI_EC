#include "My_MotorApp_Callback.h"
#include <Qdebug>

My_MotorApp_Callback::My_MotorApp_Callback(QObject *parent): QObject(parent)
{
    this->Master_ReleaseAddress();//初始化指针为NULL
    memset(loop_count,0,sizeof(loop_count));
    memset(&m_Stepper_control,0,sizeof(m_Stepper_control));
    m_Stepper_block_Q = new QQueue<Stepper_block*>();
    m_sys_reset = false;
    memset(&m_sys_position,0,sizeof(m_sys_position));

    m_Step_perMM[AXIS_X] = DEFAULT_X_STEPS_PER_MM;
    m_Step_perMM[AXIS_Y] = DEFAULT_Y_STEPS_PER_MM;
    m_Step_perMM[AXIS_Z] = DEFAULT_Z_STEPS_PER_MM;

    Arm_motion_reset();
    isRun = true;
    m_RenewST_init = false;//自动运行的激励标志位
    m_RenewST_ready = false;//自动运行的激励标志位
    m_McodeFlag = false;
}

My_MotorApp_Callback::~My_MotorApp_Callback()
{
    this->Master_ReleaseAddress();
    memset(loop_count,0,sizeof(loop_count));
    memset(&m_Stepper_control,0,sizeof(m_Stepper_control));
    m_sys_reset = false;
    memset(&m_sys_position,0,sizeof(m_sys_position));
    isRun = false;
//    this->wait();
//    this->quit();//停止线程
    this->Gcode_ReleaseAddress();
}
//    static int xx;
void My_MotorApp_Callback::Master_AppLoop_callback()
{
    //控制电平输出

//    if(m_Stepper_control.step_count == 0){//需要获取新的block
//        if(m_Stepper_block_Q->empty()){
//            return ;
//        }
//        else{
//            m_Stepper_control.exec_block = m_Stepper_block_Q->dequeue();
//            m_Stepper_control.step_count = m_Stepper_control.exec_block->step_event_count;//一共运行所有轴的最大步长
//            m_Stepper_control.counter[AXIS_X] = m_Stepper_control.counter[AXIS_Y] = m_Stepper_control.counter[AXIS_Z] = (m_Stepper_control.exec_block->step_event_count >> 1);

//            m_Stepper_control.Axis_steps[AXIS_X] = m_Stepper_control.exec_block->Axis_steps[AXIS_X];
//            m_Stepper_control.Axis_steps[AXIS_Y] = m_Stepper_control.exec_block->Axis_steps[AXIS_Y];
//            m_Stepper_control.Axis_steps[AXIS_Z] = m_Stepper_control.exec_block->Axis_steps[AXIS_Z];
//            *(input_ptr+step_setting) = 1;
//            *(input_ptr+step_fre) = 5000;//设置频率为1000Hz
//        }
//    }

//    m_Stepper_control.step_outbits = 0;//复位Step bits
//    //direction_bit通过预运行函数设定
//    m_Stepper_control.dir_outbits = m_Stepper_control.exec_block->direction_bits;//传递dir bits

//    //插补运动
//    m_Stepper_control.counter[AXIS_X] += m_Stepper_control.Axis_steps[AXIS_X];
//    if(m_Stepper_control.counter[AXIS_X] > m_Stepper_control.exec_block->step_event_count){
//        //设置方向
//        m_Stepper_control.step_outbits |= (1<<AXIS_X);
//        m_Stepper_control.counter[AXIS_X] -= m_Stepper_control.exec_block->step_event_count;
//        //根据方向更新位置信息
//        if(m_Stepper_control.exec_block->direction_bits & (1<<AXIS_X)) {
//            m_sys_position[AXIS_X]--;
//        }
//        else {
//            m_sys_position[AXIS_X]++;
//        }
//    }

//    m_Stepper_control.counter[AXIS_Y] += m_Stepper_control.Axis_steps[AXIS_Y];
//    if(m_Stepper_control.counter[AXIS_Y] > m_Stepper_control.exec_block->step_event_count){
//        //设置方向
//        m_Stepper_control.step_outbits |= (1<<AXIS_Y);
//        m_Stepper_control.counter[AXIS_Y] -= m_Stepper_control.exec_block->step_event_count;
//        //根据方向更新位置信息
//        if(m_Stepper_control.exec_block->direction_bits & (1<<AXIS_Y)) {
//            m_sys_position[AXIS_Y]--;
//        }
//        else {
//            m_sys_position[AXIS_Y]++;
//        }
//    }

//    m_Stepper_control.counter[AXIS_Z] += m_Stepper_control.Axis_steps[AXIS_Z];
//    if(m_Stepper_control.counter[AXIS_Z] > m_Stepper_control.exec_block->step_event_count){
//        //设置方向
//        m_Stepper_control.step_outbits |= (1<<AXIS_Z);
//        m_Stepper_control.counter[AXIS_Z] -= m_Stepper_control.exec_block->step_event_count;
//        //根据方向更新位置信息
//        if(m_Stepper_control.exec_block->direction_bits & (1<<AXIS_Z)) {
//            m_sys_position[AXIS_Z]--;
//        }
//        else {
//            m_sys_position[AXIS_Z]++;
//        }
//    }

//    //设置方向
//    *(input_ptr+motor_dir) = m_Stepper_control.dir_outbits;//dir
////        *(input_ptr+6) = 0;//dir4

//     //设置Step
//    *(input_MotorStep_ptr+motor1_step) = (m_Stepper_control.step_outbits & (1<<AXIS_X))>>AXIS_X;//step1
//    *(input_MotorStep_ptr+motor2_step) = (m_Stepper_control.step_outbits & (1<<AXIS_Y))>>AXIS_Y;//step2
//    *(input_MotorStep_ptr+motor3_step) = (m_Stepper_control.step_outbits & (1<<AXIS_Z))>>AXIS_Z;//step3

//    m_Stepper_control.step_count--;
////    qDebug() << m_Stepper_control.step_count;

//    if(m_sys_reset){//如果用户进行了复位
//       m_Stepper_control.step_count=0;
//    }

//    if(m_Stepper_control.step_count == 0){
//        delete m_Stepper_control.exec_block;
//        m_Stepper_control.exec_block = NULL;//释放内存
//        Motor_Reset();//防止Ethercat将循环的数值传递到从站
//         *(input_ptr+step_setting) = 0;
//    }
//    qDebug() << "input"<<*input_ptr << *(input_ptr+1);
//    qDebug() << "output"<<*output_ptr << *(output_ptr+1);
//    qDebug() << xxx;
//    *(input_ptr+step_setting) = 1;
//    *(input_ptr+step_fre) = 1000;
//    if(xx==0){
//        STEP_BIT_SetTrue(*(input_ptr+step_setting),1<<step_AutoRun_start);
////        STEP_BIT_SetFalse(*(input_ptr+step_setting),1<<step_AutoRun_stop);
//        *(input_ptr+motor_dir) = 1;
//        *(input_MotorStep_ptr+motor1_step) = 1000;
//        *(input_MotorStep_ptr+motor2_step) = 3000;
//        *(input_MotorStep_ptr+motor3_step) = 2000;
////        qDebug() << "step1";
//        xx = 1;
//    }

//    if(xx==1 && (*(output_ptr+error_MSG)==1)){
//        xx =2;
////        STEP_BIT_SetFalse(*(input_ptr+step_setting),(1<<step_AutoRun_start));
////        qDebug() << "step2";
//    }
//    if(xx==2 && (*(output_ptr+motor_MSG)==0)){
////        *(input_MotorStep_ptr+motor1_step) = 0;
////        *(input_MotorStep_ptr+motor2_step) = 0;
////        *(input_MotorStep_ptr+motor3_step) = 0;
//        uint16_t mask = (1<<AXIS_X)|(1<<AXIS_Y)|(1<<AXIS_Z);
//        STEP_BIT_Toggle(*(input_ptr+motor_dir),mask);
//        STEP_BIT_SetTrue(*(input_ptr+step_setting),1<<step_AutoRun_stop);
//        STEP_BIT_SetTrue(*(input_ptr+step_setting),1<<step_AutoRun_start);
//        xx=1;
////        qDebug() << "step3";
//    }
//    *(input_MotorStep_ptr+motor1_step) = 0xaabbcc01;
//    *(input_MotorStep_ptr+motor2_step) = 0xaabbcc02;
//    *(input_MotorStep_ptr+motor3_step) = 0xaabbcc03;
//    *(input_MotorStep_ptr+motor4_step) = 0xaabbcc04;
//    *(input_MotorStep_ptr+motor5_step) = 0xaabbcc05;
//    if(xx<100){
//        *(input_ptr+motor1_dir) = 0;
//         *(input_ptr+motor2_dir) = 0;
//         *(input_ptr+motor3_dir) = 1;
//    }
//    else if(xx<200){
//        *(input_ptr+motor1_dir) = 1;
//         *(input_ptr+motor2_dir) = 0;
//         *(input_ptr+motor3_dir) = 0;
//    }
//    else{
//        xx = 0;
//    }
//    xx++;
//    *(input_ptr+motor1_step) = 1;
//    *(input_ptr+motor2_step) = 0;
//    *(input_ptr+motor3_step) = 1;

    m_Stepper_control.step_count = *(output_ptr+motor_MSG);
    if(m_sys_reset){//如果用户进行了复位
       m_Stepper_control.step_count=0;
       STEP_BIT_SetTrue(*(input_ptr+step_setting),1<<step_AutoRun_stop);//从站中stop判断在前
       Motor_Reset();
       *(input_MotorStep_ptr+AXIS_4) = 0;
    }

    //BUG:如果步数比较小,有时候无法自动完成激励
//    if(m_RenewST_init && (*(output_ptr+error_MSG)==2)){//给一个循环运动的激励
//        m_RenewST_ready = true;
//        m_RenewST_init = false;
//    }
    if(m_RenewST_init){//给一个循环运动的激励
        if((*(output_ptr+error_MSG)==2) | m_McodeFlag){
            m_RenewST_ready = true;
            m_RenewST_init = false;
            m_McodeFlag = false;
        }
    }

    if( m_Stepper_control.step_count== 0){//需要获取新的block
        if(m_Stepper_block_Q->empty()){
            return ;
        }
        else{
            m_Stepper_control.exec_block = m_Stepper_block_Q->dequeue();
//            m_Stepper_control.step_count = m_Stepper_control.exec_block->step_event_count;//一共运行所有轴的最大步长
//            m_Stepper_control.counter[AXIS_X] = m_Stepper_control.counter[AXIS_Y] = m_Stepper_control.counter[AXIS_Z] = (m_Stepper_control.exec_block->step_event_count >> 1);

            m_Stepper_control.Axis_steps[AXIS_X] = m_Stepper_control.exec_block->Axis_steps[AXIS_X];
            m_Stepper_control.Axis_steps[AXIS_Y] = m_Stepper_control.exec_block->Axis_steps[AXIS_Y];
            m_Stepper_control.Axis_steps[AXIS_Z] = m_Stepper_control.exec_block->Axis_steps[AXIS_Z];
            //设置变速
            if(m_Stepper_control.exec_block->step_event_count < 200){//步数太小的时候，速度设置小一些
                //NOTE:最小频率设置为180，再小会变大???
                *(input_ptr+step_fre) = 180;//设置频率为1000Hz
            }
            else{
                 *(input_ptr+step_fre) = 1000;//设置频率为1000Hz
            }


            m_Stepper_control.dir_outbits = m_Stepper_control.exec_block->direction_bits;//传递dir bits
            *(input_ptr+step_setting) = 1;

            STEP_BIT_SetTrue(*(input_ptr+step_setting),1<<step_AutoRun_start);//设置自动运行
            STEP_BIT_SetTrue(*(input_ptr+step_setting),1<<step_AutoRun_stop);//从站中stop判断在前
            *(input_ptr+motor_dir) = m_Stepper_control.dir_outbits;
            *(input_MotorStep_ptr+motor1_step) = m_Stepper_control.Axis_steps[AXIS_X];
            *(input_MotorStep_ptr+motor2_step) = m_Stepper_control.Axis_steps[AXIS_Y];
            *(input_MotorStep_ptr+motor3_step) = m_Stepper_control.Axis_steps[AXIS_Z];

            int32_t steps_loopNum[AXIS_N];
            int32_t steps_remain[AXIS_N];
            int32_t steps_limit = 3000;
            steps_loopNum[AXIS_X] = m_Stepper_control.Axis_steps[AXIS_X]/steps_limit;
            steps_remain[AXIS_X] = m_Stepper_control.Axis_steps[AXIS_X]%steps_limit;
            steps_loopNum[AXIS_Y] = m_Stepper_control.Axis_steps[AXIS_Y]/steps_limit;
            steps_remain[AXIS_Y] = m_Stepper_control.Axis_steps[AXIS_Y]%steps_limit;
            steps_loopNum[AXIS_Z] = m_Stepper_control.Axis_steps[AXIS_Z]/steps_limit;
            steps_remain[AXIS_Z] = m_Stepper_control.Axis_steps[AXIS_Z]%steps_limit;
//            qDebug() <<"steps_loopNum"<<steps_loopNum[AXIS_X] << steps_remain[AXIS_X];
            //根据方向记录位置信息
            while(steps_loopNum[AXIS_X]-->0){
                if(m_Stepper_control.dir_outbits & (1<<AXIS_X)) {
    //                m_sys_position[AXIS_X]-=m_Stepper_control.Axis_steps[AXIS_X];//直接相加减会出现有符号数和无符号数之间的运算
                    m_sys_position[AXIS_X] -= steps_limit;
                }
                else {
//                    m_sys_position[AXIS_X]+=m_Stepper_control.Axis_steps[AXIS_X];
                    m_sys_position[AXIS_X] += steps_limit;
                }
            }
            if(m_Stepper_control.dir_outbits & (1<<AXIS_X)) {//剩余的数字进行计算
//                m_sys_position[AXIS_X]-=m_Stepper_control.Axis_steps[AXIS_X];//直接相加减会出现有符号数和无符号数之间的运算
                m_sys_position[AXIS_X] -= steps_remain[AXIS_X];
            }
            else {
//                    m_sys_position[AXIS_X]+=m_Stepper_control.Axis_steps[AXIS_X];
                m_sys_position[AXIS_X] += steps_remain[AXIS_X];
            }

            while(steps_loopNum[AXIS_Y]-->0){
                if(m_Stepper_control.dir_outbits & (1<<AXIS_Y)) {
    //                m_sys_position[AXIS_Y]-=m_Stepper_control.Axis_steps[AXIS_Y];//直接相加减会出现有符号数和无符号数之间的运算
                    m_sys_position[AXIS_Y] -= steps_limit;
                }
                else {
//                    m_sys_position[AXIS_Y]+=m_Stepper_control.Axis_steps[AXIS_Y];
                    m_sys_position[AXIS_Y] += steps_limit;
                }
            }
            if(m_Stepper_control.dir_outbits & (1<<AXIS_Y)) {//剩余的数字进行计算
//                m_sys_position[AXIS_Y]-=m_Stepper_control.Axis_steps[AXIS_Y];//直接相加减会出现有符号数和无符号数之间的运算
                m_sys_position[AXIS_Y] -= steps_remain[AXIS_Y];
            }
            else {
//                    m_sys_position[AXIS_Y]+=m_Stepper_control.Axis_steps[AXIS_Y];
                m_sys_position[AXIS_Y] += steps_remain[AXIS_Y];
            }

            while(steps_loopNum[AXIS_Z]-->0){
                if(m_Stepper_control.dir_outbits & (1<<AXIS_Z)) {
    //                m_sys_position[AXIS_Z]-=m_Stepper_control.Axis_steps[AXIS_Z];//直接相加减会出现有符号数和无符号数之间的运算
                    m_sys_position[AXIS_Z] -= steps_limit;
                }
                else {
//                    m_sys_position[AXIS_Z]+=m_Stepper_control.Axis_steps[AXIS_Z];
                    m_sys_position[AXIS_Z] += steps_limit;
                }
            }
            if(m_Stepper_control.dir_outbits & (1<<AXIS_Z)) {//剩余的数字进行计算
//                m_sys_position[AXIS_Z]-=m_Stepper_control.Axis_steps[AXIS_Z];//直接相加减会出现有符号数和无符号数之间的运算
                m_sys_position[AXIS_Z] -= steps_remain[AXIS_Z];
            }
            else {
//                    m_sys_position[AXIS_Z]+=m_Stepper_control.Axis_steps[AXIS_Z];
                m_sys_position[AXIS_Z] += steps_remain[AXIS_Z];
            }

            m_RenewST_init = true;

            if(m_Stepper_control.exec_block->Mcode !=Gcode_segment::No_Mcode){
                switch(m_Stepper_control.exec_block->Mcode){
                case Gcode_segment::MotorTool_On:
                    *(input_MotorStep_ptr+AXIS_4) = 2000;
    //                STEP_BIT_SetFalse(*(input_ptr+step_setting),1<<step_AutoRun_start);//设置自动运行
                    break;
                case Gcode_segment::MotorTool_Off:
                    *(input_MotorStep_ptr+AXIS_4) = 0;
    //                STEP_BIT_SetFalse(*(input_ptr+step_setting),1<<step_AutoRun_start);//设置自动运行
                    break;
                case Gcode_segment::COMMENT_CODE:
                    break;
                case Gcode_segment::EndParse_CODE:
                    break;
                default:
                    qDebug() << "Mcode:"<<m_Stepper_control.exec_block->Mcode;
                    break;
                }

                m_McodeFlag = true;
//                STEP_BIT_SetFalse(*(input_ptr+step_setting),1<<step_AutoRun_stop);//从站中stop判断在前
                STEP_BIT_SetFalse(*(input_ptr+step_setting),1<<step_AutoRun_start);//设置自动运行
            }


            delete m_Stepper_control.exec_block;
            m_Stepper_control.exec_block = NULL;//释放内存
        }
    }
    else{
        Motor_Reset();
     *(input_ptr+step_setting) = 1;//只是使能，去掉自动运行的标志
    }



//    qDebug() << m_Stepper_control.step_count << QString::number(*(input_ptr+step_setting),2);
//    qDebug() << "Dir:"<<*(output_ptr+motor_dir);
//    qDebug() <<"Step_count:"<<*(output_ptr+motor_MSG)<<"isAutoRun"<<*(output_ptr+error_MSG);
//    qDebug() << *(output_ptr);

}

void My_MotorApp_Callback::Master_AppStart_callback()
{
    output_ptr = (int16_t*)(m_Master_addressBase+0x1a);
    input_ptr = (uint16_t*)(m_Master_addressBase+0x00);
    input_MotorStep_ptr = (uint32_t*)(m_Master_addressBase+0x06);
    //设定一个初始值,否则Ethercat会将循环的数值传递到从站
   Motor_Reset();
   *(input_MotorStep_ptr+AXIS_4) = 0;
//   xx=0;
   isRun = true;
//   if(m_GcodeSegment_Q->empty()){
//       qDebug() << "My_MotorApp_Callback Q empty!";
//   }
//   else{
//       qDebug() << "My_MotorApp_Callback Q No empty!";
//   }
}

void My_MotorApp_Callback::Master_AppStop_callback()
{
//  Motor_Reset();//加上会报错
  this->Master_ReleaseAddress();//释放指针
  isRun = false;
//  this->wait();
//  this->quit();//停止线程
  m_Stepper_block_Q->clear();
  m_Stepper_control.step_count = 0;
//  memset(&m_sys_position,0,sizeof(m_sys_position));
  Arm_motion_reset();//不能全部置0
}

int My_MotorApp_Callback::Master_setAdressBase(char *address)
{
    m_Master_addressBase = address;

    return 0;
}

void My_MotorApp_Callback::Master_ReleaseAddress()
{
    m_Master_addressBase = NULL;
    output_ptr = NULL;
    input_ptr = NULL;
    input_MotorStep_ptr = NULL;
}

void My_MotorApp_Callback::Gcode_setAddress(QQueue<Gcode_segment> *q_addr)
{
    m_GcodeSegment_Q = q_addr;
}

void My_MotorApp_Callback::Gcode_ReleaseAddress()
{
    m_GcodeSegment_Q = nullptr;
}

void My_MotorApp_Callback::Motor_Reset(){
    //设置方向
    *(input_ptr+motor_dir) = 0;//dir

     //设置Step
    *(input_MotorStep_ptr+motor1_step) = 0;//step1
    *(input_MotorStep_ptr+motor2_step) = 0;//step2
    *(input_MotorStep_ptr+motor3_step) = 0;//step3
}

int My_MotorApp_Callback::Planner_BufferLine(float * target,int userData){
    //Q_UNUSED(userData);//后面再添加

    float unit_vec[AXIS_N], delta_mm;
    int32_t target_steps[AXIS_N], position_steps[AXIS_N];

    Stepper_block* block_new = new Stepper_block();
    block_new->step_event_count = 0;

//    if(userData > Gcode_segment::No_Mcode){
//        for(int idx = 0;idx < AXIS_N;idx++){
//             block_new->Axis_steps[idx] = 0;
//        }
//        block_new->direction_bits = 0;
//        block_new->step_event_count = 0;
//        block_new->Mcode = userData;
//        m_Stepper_block_Q->enqueue(block_new);
//        // qDebug() << userData;
//        return Planner_OK;

//    }

    memcpy(position_steps, m_sys_position, sizeof(m_sys_position));// sys_position (vector in steps.int32)
    //正解校验
//    float print_position[3];
//    for(int i=0;i<3;i++){
//        print_position[i] = position_steps[i]/m_Step_perMM[i];
//    }
//    calculate_forward(print_position);

    //将target转化为角度坐标(反解)
    ARM_Motion_s arm_motion_target;
    arm_motion_target =  calculate_arm(target);
    if(arm_motion_target.transfer_state > 0){ //不符合余弦定理,有潜在的隐患
      delete block_new;//避免内存泄漏
      return Planner_EmptyBlock;//暂时将其定为EMPTY_BLOCK,即不运动
    }
    //分别计算
    for(int idx = 0;idx < AXIS_N;idx++){

        target_steps[idx] = std::lround(arm_motion_target.arm[idx]*m_Step_perMM[idx]);
        //target_steps[idx] = lround(target[idx]*settings.steps_per_mm[idx]);

        block_new->Axis_steps[idx] = std::labs(target_steps[idx]-position_steps[idx]);
        block_new->step_event_count = std::max(block_new->step_event_count,block_new->Axis_steps[idx]);

        delta_mm = (target_steps[idx] - position_steps[idx])/m_Step_perMM[idx];

        unit_vec[idx] = delta_mm; // Store unit vector numerator

        // Set direction bits. Bit enabled always means direction is negative.
        if(delta_mm < 0.0) {
            block_new->direction_bits |= (1<< idx);//方向取反
        }

//        qDebug() << "Delta:"<<delta_mm;
    }//for

    block_new->Mcode = userData;//赋值Mcode

//    qDebug() << block_new->Axis_steps[0] << block_new->Axis_steps[1] << block_new->Axis_steps[2];
//    qDebug() << "Dir:"<<block_new->direction_bits;

    ///偏移量为0，则为空的执行块
    if(block_new->step_event_count == 0) {
        if(userData > Gcode_segment::No_Mcode){//有Mcode的代码不取消
             m_Stepper_block_Q->enqueue(block_new);
//             qDebug() << userData;
             return Planner_OK;
        }

        delete block_new;//避免内存泄漏
        return Planner_EmptyBlock;
    }



    //添加到block队列
    m_Stepper_block_Q->enqueue(block_new);

    return Planner_OK;
}

void My_MotorApp_Callback::Arm_motion_reset(){
    ARM_Motion_s arm_position_init;//笛卡尔坐标系为(0,0,0)的角度

    arm_position_init.arm[AXIS_X] = 90;
    arm_position_init.arm[AXIS_Y] = 0;
    arm_position_init.arm[AXIS_Z] = 0;

    #if ARM_PEINT_DEBUG
    //打印初始位置坐标，便于调试
    ARM_Motion_s arm_position_debug;
    arm_position_debug = calculate_forward(arm_position_init.arm);
    Printf_MSG("[Arm_PosInit=(%f,%f,%f)]\r\n",arm_position_debug.arm[AXIS_X],arm_position_debug.arm[AXIS_Y],arm_position_debug.arm[AXIS_Z]);
    #endif

    for(int idx = 0; idx < AXIS_N; idx++) { //只用了3轴
         m_sys_position[idx] = std::lround(arm_position_init.arm[idx]*m_Step_perMM[idx]);
  }
}

void My_MotorApp_Callback::GcodeSendThread_run(){
    while(isRun){
        if(m_Stepper_control.step_count == 0){//需要获取新的block
            if(m_Stepper_block_Q->empty() && m_RenewST_ready){
                 m_RenewST_ready = false;
                 if(m_GcodeSegment_Q){
                     if(!m_GcodeSegment_Q->empty()){
                         Gcode_segment segment = m_GcodeSegment_Q->dequeue();
                         QVector3D data = segment.data_xyz;
                         int Mcode = segment.Mcode;
//                         qDebug() << data;

                         emit Gcode_lineChange(segment.line);//发射Gcode行号改变的信号

                         m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_X] = data.x();
                         m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Y] = data.y();
                         m_ARM_Motion_test.arm[My_MotorApp_Callback::AXIS_Z] = data.z();
                         int ret = Planner_BufferLine(m_ARM_Motion_test.arm,Mcode);
                         if(ret==Planner_EmptyBlock){//空的执行块
                             m_RenewST_ready = true;//重新获取新的执行块
                         }
                         m_sys_reset = false;
                     }
                 }
                 else{
                     qDebug() << "m_GcodeSegment_Q==NULL!";
                 }

            }
         }
        QThread::msleep(5);
    }
//    Planner_BufferLine(m_ARM_Motion_test.arm,0);
//    m_sys_reset = false;
}


//原点的设置是三个步进电机的中心位置的交点
//#define BIG_ARM_LENGTH 135				//	@大臂长度	135mm
//#define SMALL_ARM_LENGTH 170			//	@小臂长度	170mm
//#define HEAD_OFFSET   50				//	@头偏移		50mm
//#define CENCER_OFFSET 47				//	@中心偏移	47mm

//#define ARM_HIGH_OFFSET 110 // @ 原点到水平的高度110mm

//#define ARM_OFFSET_X 100 //mm   		//	@X偏移	100mm
//#define ARM_OFFSET_Y -56 //mm			//	@Y偏移	-56mm
//#define ARM_OFFSET_Z -56 //mm			//	@Z偏移	-56mm
//#define ARM_RAD2DEG 57.2957795			//	@转角	57.2957795度

#define BIG_ARM_LENGTH 155				//	@大臂长度	155mm
#define SMALL_ARM_LENGTH 170			//	@小臂长度	170mm
#define HEAD_OFFSET   50.2				//	@头偏移	48.8mm
#define CENCER_OFFSET 62				//	@中心偏移	62mm

//#define ARM_HIGH_OFFSET 110 // @ 原点到水平的高度110mm

//#define ARM_OFFSET_X 100 //mm   		//	@X偏移	100mm
//#define ARM_OFFSET_Y -56 //mm			//	@Y偏移	-56mm
//#define ARM_OFFSET_Z -56 //mm			//	@Z偏移	-56mm
#define ARM_RAD2DEG 57.2957795			//	@转角	57.2957795度

float return_angle(float a, float b, float c) {
  // cosine rule for angle between c and a
  return std::acos((a * a + c * c - b * b) / (2 * a * c));
}

uint8_t check_angle(float a, float b, float c){
  float data;
    data = (a * a + c * c - b * b) - (2 * a * c);

    if(data > 0){
      return 1;
    }
    else{
      return 0;
    }
}

ARM_Motion_s My_MotorApp_Callback::calculate_forward(const float *cartesian_theta)
{
    float x1,y1;
    float x2,y2;

    ARM_Motion_s arm_motion_temp;

    y1=std::sin(cartesian_theta[AXIS_X]/ARM_RAD2DEG)*BIG_ARM_LENGTH-sin(cartesian_theta[AXIS_Y]/ARM_RAD2DEG)*SMALL_ARM_LENGTH;

    x1=std::cos(cartesian_theta[AXIS_X]/ARM_RAD2DEG)*BIG_ARM_LENGTH+cos(cartesian_theta[AXIS_Y]/ARM_RAD2DEG)*SMALL_ARM_LENGTH;
    x1=x1+HEAD_OFFSET+CENCER_OFFSET;

    x2 = x1 *std::cos(cartesian_theta[AXIS_Z]/ARM_RAD2DEG);
    y2 = x1 *std::sin(cartesian_theta[AXIS_Z]/ARM_RAD2DEG);

    arm_motion_temp.arm[AXIS_X]=x2;
    arm_motion_temp.arm[AXIS_Y]=y2;
    arm_motion_temp.arm[AXIS_Z]=y1;

    arm_motion_temp.transfer_state = 0;//no error

   #if ARM_PEINT_DEBUG
   Printf_MSG("[Arm_Pos=(%f,%f,%f)]\r\n",arm_motion_temp.arm[AXIS_X],arm_motion_temp.arm[AXIS_Y],arm_motion_temp.arm[AXIS_Z]);
   #endif

  return arm_motion_temp;
}

ARM_Motion_s My_MotorApp_Callback::calculate_arm(const float *cartesian)
{
    float TempXY,TempXYZ;
    float TempX,TempY,TempZ;

  ARM_Motion_s arm_motion_temp;

    TempXY=std::sqrt(cartesian[AXIS_X]*cartesian[AXIS_X]
                +cartesian[AXIS_Y]*cartesian[AXIS_Y]);
    TempXYZ=std::sqrt((TempXY-CENCER_OFFSET-HEAD_OFFSET)*(TempXY-CENCER_OFFSET-HEAD_OFFSET)
                +cartesian[AXIS_Z]*cartesian[AXIS_Z]);

    if(check_angle(BIG_ARM_LENGTH,SMALL_ARM_LENGTH,TempXYZ)){
      arm_motion_temp.transfer_state = 1;//error
        #if ARM_PEINT_DEBUG
        Printf_MSG("[Calculate_arm Error!]\r\n");//打印错误信息
        #endif
    }
    else{
        arm_motion_temp.transfer_state = 0;//no error
    }

    TempX = std::asin(cartesian[AXIS_Z]/(TempXYZ))+return_angle(BIG_ARM_LENGTH,SMALL_ARM_LENGTH,TempXYZ);
    TempY = return_angle(BIG_ARM_LENGTH,TempXYZ,SMALL_ARM_LENGTH);
    TempZ = std::atan(cartesian[AXIS_Y]/cartesian[AXIS_X]);


    arm_motion_temp.arm[AXIS_X] = TempX*ARM_RAD2DEG;//弧度转化为角度
    arm_motion_temp.arm[AXIS_Y] = 180-arm_motion_temp.arm[AXIS_X]-TempY*ARM_RAD2DEG;
    arm_motion_temp.arm[AXIS_Z] = TempZ*ARM_RAD2DEG;

    #if ARM_PEINT_DEBUG
    Printf_MSG("[Arm_Theta=(%f,%f,%f)]\r\n",arm_motion_temp.arm[AXIS_X],arm_motion_temp.arm[AXIS_Y],arm_motion_temp.arm[AXIS_Z]);
    #endif

    return arm_motion_temp;
}


