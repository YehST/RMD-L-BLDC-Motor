
#ifndef RMD_L_RS485_H
#define RMD_L_RS485_H

#ifdef __cplusplus
extern "C" {
#endif

/* user type structure----------------------- */
typedef struct {
    int8_t Cur_KP;
    int8_t Cur_KI;
    int8_t Vel_KP;
    int8_t Vel_KI;
    int8_t Pos_KP;
    int8_t Pos_KI;
} Controller;

typedef struct {
    int8_t temperature;
    float current;
    int32_t accel;  
    float speed;
    float angle;
    float A_Phase_cur;
    float B_Phase_cur;
    float C_Phase_cur;
    uint32_t ZeroShift;

    float voltage;
    uint16_t ErrorState;
    uint8_t Brake;
    /* ErrorState------------
        0x0002 堵轉
        0x0004 low voltage
        0x0008 Over voltage
        0x0010 Over Phase current
        0x0040 Power large than limit
        0x0080 wrong write parameters
        0x0100 Over speed
        0x1000 temperature too high
        0x2000 Error Encoder abjust
    ----------------------*/
} Motor_state;

typedef struct {
    uint8_t ID;
    // Controller
    Controller PID;
    // Motor State
    Motor_state state;
} RMD_Motordef;


#define RMD_CMD_Read_PIDArg 0x30
#define RMD_CMD_Write_PIDRAM 0x31
#define RMD_CMD_Write_PIDROM 0x32
#define RMD_CMD_Read_ACC 0x42
#define RMD_CMD_Write_ACC 0x43
#define RMD_CMD_Read_MultiEncoder 0x60
#define RMD_CMD_Read_RawMultiEncoder 0x61
#define RMD_CMD_Read_MultiEncoder_ZeroShift 0x62
#define RMD_CMD_SetZero_MultiEncoderVal 0x63
#define RMD_CMD_SetZero_CurrentPos 0x64
#define RMD_CMD_Read_SingleEncoder 0x90
#define RMD_CMD_Read_MultiAngle 0x92
#define RMD_CMD_Read_SingleAngle 0x94
#define RMD_CMD_Read_State1 0x9A
#define RMD_CMD_Read_State2 0x9C
#define RMD_CMD_Read_State3 0x9D

#define RMD_CMD_CloseMotor 0x80
#define RMD_CMD_StopMotor 0x81

#define RMD_CMD_Torque 0xA1
#define RMD_CMD_Speed 0xA2
#define RMD_CMD_ABSAngle 0xA4
#define RMD_CMD_SingleAngle 0xA6
#define RMD_CMD_AddAngle 0xA8

#define RMD_CMD_GET_SystemMode 0x70
#define RMD_CMD_GET_MotorPower 0x71
#define RMD_CMD_SystemReset 0x76
#define RMD_CMD_Brake_Release 0x77
#define RMD_CMD_Brake_Lock 0x78

#define RMD_CMD_GET_SystemTime 0xB1
#define RMD_CMD_GET_SystemVersion 0xB2
#define RMD_CMD_CommuProtect 0xB3
#define RMD_CMD_Set_BaudRate 0xB4
#define RMD_CMD_GET_MotorType 0xB5
#define RMD_CMD_Function 0x20
#define RMD_CMD_MultiMotorControl 0xCD
#define RMD_CMD_Set_485ID 0x79

/* user type define----------------------- */

uint16_t crc16table(uint8_t *ptr, uint16_t len);
void RS_Init_Handle(UART_HandleTypeDef *huart, GPIO_TypeDef *EN_GPIO, uint16_t EN_GPIO_PIN, TIM_HandleTypeDef* timer);
void sendCMD(RMD_Motordef* Motor, uint8_t CMD, uint8_t* Data);
void ReceiveData(RMD_Motordef* Motor, uint8_t CMD);
void RS_Read_PID(RMD_Motordef* Motor);
void RS_Write_PID_RAM(RMD_Motordef* Motor, int8_t Cur_P, int8_t Cur_I, int8_t Vel_P, int8_t Vel_I, int8_t Pos_P, int8_t Pos_I);
void RS_Write_PID_ROM(RMD_Motordef* Motor, int8_t Cur_P, int8_t Cur_I, int8_t Vel_P, int8_t Vel_I, int8_t Pos_P, int8_t Pos_I);
void RS_Read_ACC(RMD_Motordef* Motor);
void RS_Write_ACC(RMD_Motordef* Motor, uint32_t Accelation);
void RS_Read_MultiEncoder(RMD_Motordef* Motor);
void RS_Read_RawMultiEncoder(RMD_Motordef* Motor);
void RS_Read_MultiEncoder_ZeroShift(RMD_Motordef* Motor);
void RS_SetZero_MultiEncoderVal(RMD_Motordef* Motor, uint32_t EncoderZeroShift);
void RS_SetZero_CurPosition(RMD_Motordef* Motor);
void RS_ReadState1(RMD_Motordef* Motor);
void RS_ReadState2(RMD_Motordef* Motor);
void RS_ReadState3(RMD_Motordef* Motor);
void RS_TorqueControl(RMD_Motordef* Motor, int16_t iqControl);
void RS_speedControl(RMD_Motordef* Motor, int32_t speedControl);
void RS_ABSangleControl(RMD_Motordef* Motor, int32_t ref_angle, int16_t maxSpeed);
void RS_SingleAngleControl(RMD_Motordef* Motor, int16_t ref_angle, int16_t maxSpeed);



void RS_CloseBLDC(RMD_Motordef* Motor);
void RS_StopBLDC(RMD_Motordef* Motor);
void RS_SystemReset(RMD_Motordef* Motor);
void delay_us(uint16_t us);

#ifdef __cplusplus
}
#endif

#endif
