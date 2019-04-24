#ifndef CNC_MOTION_H
#define CNC_MOTION_H

#include "ARM_Motion.h"
#include <QQueue>

typedef struct ARM_SendState
{
    ARM_Struct *position_send;
    ARM_Struct *theta_send;
    bool sendPositionOK;
    bool sendThetaOK;
    int n_axis;
    ARM_SendState(int n) {
        n_axis = n;
        position_send = new ARM_Struct(n_axis);
        theta_send = new ARM_Struct(n_axis);
        sendPositionOK = false;
        sendThetaOK = false;

    }
    ARM_SendState() {
        ARM_SendState(3);//默认是三轴
    }
}ARM_SendState;

class CNC_Motion : public ARM_Motion
{
public:
    CNC_Motion();
public:
    typedef enum{
        AXIS_X,
        AXIS_Y,
        AXIS_Z,
        AXIS_N
    }AXIS_Enum;

    int32_t m_sys_position[AXIS_N];
    QQueue<Stepper_block*> *m_Stepper_block_Q;//定义控制队列
    Stepper_control *m_Stepper_control;
    bool m_RenewST_init;//自动运行的激励标志位
    bool m_RenewST_ready;//自动运行的激励标志位
    bool m_McodeFlag;//有Mcode的特殊情况，设置激励标志

    virtual uint8_t calculate_arm(float *cartesian_theta, const float *cartesian);
    virtual uint8_t calculate_forward(float *cartesian, const float *cartesian_theta);
    float get_StepsPerUnit(int n);
    int get_Axis_count();
    void Arm_motion_reset();
    ARM_Struct get_PositionInit();
    int planner_BufferLine(const float *target, int userData, ARM_SendState *send_signal);
    void loopRun();
    void Motor_Reset();

    void set_OutputPtr(const int16_t* output_ptr){this->output_ptr = output_ptr;}
    void set_IutputPtr(uint16_t* input_ptr){this->input_ptr = input_ptr;}
    void set_Iutput_MotorStepPtr(uint32_t* input_MotorStep_ptr){this->input_MotorStep_ptr = input_MotorStep_ptr;}
    const int16_t* get_OutputPtr(){return this->output_ptr;}
    uint16_t* get_IutputPtr(){return this->input_ptr;}
    uint32_t* get_Iutput_MotorStepPtr(){return this->input_MotorStep_ptr;}
    void release_IO_ptr(){
        output_ptr = NULL;
        input_ptr = NULL;
        input_MotorStep_ptr = NULL;
    }
private:
    const int16_t* output_ptr;
    uint16_t* input_ptr;
    uint32_t* input_MotorStep_ptr;
    typedef enum{
        AXIS_1,
        AXIS_2,
        AXIS_3,
        AXIS_4,
        AXIS_5,
    }step_axis_5;

    typedef enum{
        motor1_step,
        motor2_step,
        motor3_step,
        motor4_step,
        motor5_step
    }step32_enum;

    typedef enum{
        step_setting,
        step_fre,
        motor_dir
    }step16_enum;

    typedef enum{
        step_enable,
        step_AutoRun_start,
        step_AutoRun_stop
    }step_setting_enum;

    typedef enum{
        motor_MSG,
        error_MSG,
        AD_CH1,
        AD_CH2
    }output_MSG_enum;

    typedef enum{
        Planner_OK,
        Planner_EmptyBlock
    }planner_enum;

    ARM_Struct *m_PositionInit;//笛卡尔坐标系为(0,0,0)的角度

};

#endif // CNC_Motion_H
