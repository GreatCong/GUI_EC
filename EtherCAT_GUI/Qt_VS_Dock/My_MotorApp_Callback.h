#ifndef MY_MOTORAPP_CALLBACK_H
#define MY_MOTORAPP_CALLBACK_H

#include "EtherCAT_UserApp.h"
#include <QQueue>
#include <QThread>

#include "GcodeParser.h"

//#define ARM_PEINT_DEBUG 1
#define Printf_MSG qDebug

#define AXIS_N 3 //3轴控制

#define DEFAULT_X_STEPS_PER_MM 			89.29//86.48f
#define DEFAULT_Y_STEPS_PER_MM 			88.90//89.29f
#define DEFAULT_Z_STEPS_PER_MM 			89.33//90.87f//86.48f

#define STEP_BIT(n) 					(1 << n) 				//位mask
#define STEP_BIT_SetTrue(x,mask) 	(x |= mask)				//该位设置为真
#define STEP_BIT_SetFalse(x,mask)	(x &= ~mask)			//该位设置为假
#define STEP_BIT_Toggle(x,mask) 		(x ^= mask)				//位切换
#define STEP_BIT_IsTrue(x,mask) 		((x & mask) != 0)		//该位是否真
#define STEP_BIT_IsFalse(x,mask) 	((x & mask) == 0)		//该位是否假

typedef struct{
  float arm[AXIS_N];
  uint8_t transfer_state;
}ARM_Motion_s;

typedef struct
{
  uint32_t Axis_steps[AXIS_N];
  uint32_t step_event_count;//最大步长
  uint16_t direction_bits;
  int Mcode;
}Stepper_block;

typedef Stepper_block* Stepper_block_t;

typedef struct
{
  uint32_t Axis_steps[AXIS_N];
  uint32_t counter[AXIS_N];
  uint32_t step_count;
  uint16_t step_outbits;//支持16轴
  uint16_t dir_outbits;

  uint16_t exec_block_index;
  Stepper_block *exec_block;
}Stepper_control;

class My_MotorApp_Callback : public QObject,public Ethercat_Callback
{
    Q_OBJECT
public:
    explicit My_MotorApp_Callback(QObject *parent = nullptr);
    ~My_MotorApp_Callback();
    virtual void Master_AppLoop_callback();
    virtual void Master_AppStart_callback();
    virtual void Master_AppStop_callback();
    virtual int Master_setAdressBase(char* address);
    virtual void Master_ReleaseAddress();

    virtual void Gcode_setAddress(QQueue<Gcode_segment> *q_addr);
    virtual void Gcode_ReleaseAddress();

    const int16_t* output_ptr;
    uint16_t* input_ptr;
    uint32_t* input_MotorStep_ptr;
    int loop_count[AXIS_N];
    QQueue<Stepper_block*> *m_Stepper_block_Q;//定义控制队列
    bool m_sys_reset;
    int32_t m_sys_position[AXIS_N];
    float m_Step_perMM[AXIS_N];

    ARM_Motion_s m_ARM_Motion_test;
    bool m_RenewST_init;//自动运行的激励标志位
    bool m_RenewST_ready;//自动运行的激励标志位
    bool m_McodeFlag;//有Mcode的特殊情况，设置激励标志

    int Planner_BufferLine(float *target, int userData);

    typedef enum{
        AXIS_X,
        AXIS_Y,
        AXIS_Z
    }step_axis_3;

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

    void Arm_motion_reset();
private:
    char *m_Master_addressBase;
    Stepper_control m_Stepper_control;

    void Motor_Reset();
    ARM_Motion_s calculate_forward(const float *cartesian_theta);
    ARM_Motion_s calculate_arm(const float *cartesian);
//    int Planner_BufferLine(float *target, int userData);
    QQueue<Gcode_segment> *m_GcodeSegment_Q;
    bool isRun;
public slots:
    void GcodeSendThread_run();
signals:
    void Gcode_lineChange(int line);
    void Gcode_PositionChange(QVector3D pos);//发送正解信息
    void Gcode_ThetaChange(QVector3D theta);//发送反解信息
    void Master_QuitSignal(bool isQuit);
};

#endif // MY_MOTORAPP_CALLBACK_H
