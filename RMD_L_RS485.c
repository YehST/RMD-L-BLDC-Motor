
#include "main.h"
#include "RMD_L_RS485.h"

/* -------------------------- user define ----------------------------- */
/* User private variables -------------------------------------- */

UART_HandleTypeDef* RS_huart;
GPIO_TypeDef* RS_EN_GPIO;
uint16_t RS_EN_GPIO_PIN;
TIM_HandleTypeDef* RS_timer;

const uint8_t crctableH[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};
/* Table of CRC values for low每order byte */
const uint8_t crctableL[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};


/* User private defined -------------------------------------- */
uint16_t RS485_delay_time_us = 100;
#define RS485_TX_Mode HAL_GPIO_WritePin(RS_EN_GPIO, RS_EN_GPIO_PIN, GPIO_PIN_SET);\
delay_us(RS485_delay_time_us);
#define RS485_RX_Mode HAL_GPIO_WritePin(RS_EN_GPIO, RS_EN_GPIO_PIN, GPIO_PIN_RESET);\
delay_us(RS485_delay_time_us);


/* User private function prototypes -----------------------------------------------*/
void RS_Init_Handle(UART_HandleTypeDef *huart, GPIO_TypeDef *EN_GPIO, uint16_t EN_GPIO_PIN, TIM_HandleTypeDef* timer){
  RS_huart = huart;
  RS_EN_GPIO = EN_GPIO;
  RS_EN_GPIO_PIN = EN_GPIO_PIN;
  RS_timer = timer;
  HAL_TIM_Base_Start(RS_timer);
}

uint16_t crc16table(uint8_t *ptr, uint16_t len)
{
    uint8_t crcH = 0xff;
    uint8_t crcL = 0xff; 
    uint16_t index;
    while (len--) 
    {
        index = crcL ^ *ptr++; 
        crcL = crcH ^ crctableH[index];
        crcH = crctableL[index];
    }
    return (crcH << 8 | crcL);
}

void sendCMD(RMD_Motordef* Motor, uint8_t CMD, uint8_t* Data){
  uint8_t TxData[13];
  TxData[0] = 0x3E;
  if (Motor == 0 || CMD == RMD_CMD_SET_485ID){
    TxData[1] = 0xCD;
  }
  else
    TxData[1] = Motor->ID;
  TxData[2] = 0x08;
  TxData[3] = CMD;
  memcpy(&TxData[4], Data, 7);

  uint16_t checkcrc = crc16table(TxData, 11);
  TxData[11] = (uint8_t)checkcrc;
  TxData[12] = (uint8_t)(checkcrc>>8);
  // for(int i =0; i<sizeof(TxData); i++){
  //   printData("%x ", TxData[i]);
  // }
  // printData("\n");  
  RS485_TX_Mode;
  HAL_UART_Transmit(RS_huart, TxData, 13, 5);
  
}
void ReceiveData(RMD_Motordef* Motor, uint8_t CMD){
  uint8_t RxData[13];
  uint8_t Data[8];
  RS485_RX_Mode;
  // HAL_UART_Receive_DMA(RS_huart, RxData, 13);
  HAL_UART_Receive(RS_huart, RxData, 13, 5);
  
  // for(int i =0; i<sizeof(RxData); i++){
  //   printData("%x ", RxData[i]);
  // }
  // printData("\n");
  if(RxData[0]==0x3e && (RxData[1]==Motor->ID || CMD == RMD_CMD_MultiMotorControl || CMD == RMD_CMD_SET_485ID) && RxData[2] == 8){
    memcpy(Data, &RxData[3], 8);
    DecodeData(Motor, Data);
  }
  else 
    printData("Error receive occur in CMD: %x \n", CMD);
}
void ReceiveMultiData(RMD_Motordef* Motor, uint8_t CMD, int MotorTotalNum){
  uint8_t RxData[13*MotorTotalNum];
  uint8_t Data[8];
  RS485_RX_Mode;
  // HAL_UART_Receive_DMA(RS_huart, RxData, 13);
  for(int i =0; i<MotorTotalNum; i++){
    HAL_UART_Receive(RS_huart, &RxData[13*i], 13, 10);
    // delay_us(500);
  }
  for(int i =0; i<sizeof(RxData); i++){
    printData("%x ", RxData[i]);
  }
  printData("\n");
  // for(int i =0; i<sizeof(RxData); i++){
  //   printData("%x ", RxData[i]);
  // }
  // printData("\n");
  // if(RxData[0]==0x3e && (RxData[1]==Motor->ID || CMD == RMD_CMD_MultiMotorControl || CMD == RMD_CMD_SET_485ID) && RxData[2] == 8){
  //   memcpy(Data, &RxData[3], 8);
  //   DecodeData(Motor, Data);
  // }
  // else 
  //   printData("Error receive occur in CMD: %x \n", CMD);
}
void DecodeData(RMD_Motordef* Motor, uint8_t* Data){
  switch(Data[0]){
      case RMD_CMD_Read_PIDArg:
        Motor->PID.Cur_KP = (int8_t)Data[2];
        Motor->PID.Cur_KI = (int8_t)Data[3];
        Motor->PID.Vel_KP = (int8_t)Data[4];
        Motor->PID.Vel_KI = (int8_t)Data[5];
        Motor->PID.Pos_KP = (int8_t)Data[6];
        Motor->PID.Pos_KI = (int8_t)Data[7];
        break;
      case RMD_CMD_Write_PIDRAM:
        Motor->PID.Cur_KP = (int8_t)Data[2];
        Motor->PID.Cur_KI = (int8_t)Data[3];
        Motor->PID.Vel_KP = (int8_t)Data[4];
        Motor->PID.Vel_KI = (int8_t)Data[5];
        Motor->PID.Pos_KP = (int8_t)Data[6];
        Motor->PID.Pos_KI = (int8_t)Data[7];
        break;
      case RMD_CMD_Write_PIDROM:
        Motor->PID.Cur_KP = (int8_t)Data[2];
        Motor->PID.Cur_KI = (int8_t)Data[3];
        Motor->PID.Vel_KP = (int8_t)Data[4];
        Motor->PID.Vel_KI = (int8_t)Data[5];
        Motor->PID.Pos_KP = (int8_t)Data[6];
        Motor->PID.Pos_KI = (int8_t)Data[7];
        break;
      case RMD_CMD_Read_ACC:
        Motor->state.accel = (Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4];
        break;
      case RMD_CMD_Read_MultiEncoder:
        Motor->state.angle = ((Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4])/16384.*360.;
        break;
      case RMD_CMD_Read_RawMultiEncoder:
        Motor->state.RawEncoder = ((Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4]);
        break;
      case RMD_CMD_Read_MultiEncoder_ZeroShift:
        Motor->state.ZeroShift = (Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4];
        break;
      case RMD_CMD_SetZero_MultiEncoderVal:
        Motor->state.ZeroShift = (Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4];
        break;
      case RMD_CMD_SetZero_CurrentPos:
        Motor->state.ZeroShift = (Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4];
        break;
      case RMD_CMD_Read_SingleEncoder:
        Motor->state.angle = ((Data[3]<<8)|Data[2])/16384.*360.;
        Motor->state.RawEncoder = ((Data[5]<<8)|Data[4]);
        Motor->state.ZeroShift = (Data[7]<<8)|(Data[6]);
        break;
      case RMD_CMD_Read_MultiAngle:
        Motor->state.angle = (float)((Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4])/100.;
        break;
      case RMD_CMD_Read_SingleAngle:
        Motor->state.angle = (float)((Data[7]<<8)|(Data[6]))/100.;
        break;
      case RMD_CMD_Read_State1:
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.Brake = Data[3];
        Motor->state.voltage = (float)((Data[5]<<8)|Data[4])*0.1;
        Motor->state.ErrorState = (Data[7]<<8)|Data[6];
        break;
      case RMD_CMD_Read_State2:
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.current = (float)((Data[3]<<8)|Data[2])/100.;
        Motor->state.speed = (float)((Data[5]<<8)|Data[4]);
        Motor->state.angle = (float)((Data[7]<<8)|Data[6]);
        break;
      case RMD_CMD_Read_State3:
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.A_Phase_cur = (float)((Data[3]<<8)|Data[2])*0.01;
        Motor->state.B_Phase_cur = (float)((Data[5]<<8)|Data[4])*0.01;
        Motor->state.C_Phase_cur = (float)((Data[7]<<8)|Data[6])*0.01;
        break;
      case RMD_CMD_Torque:
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.current = (float)((Data[3]<<8)|Data[2])/100.;
        Motor->state.speed = (float)((Data[5]<<8)|Data[4]);
        Motor->state.angle = (float)((Data[7]<<8)|Data[6]);
        break;
      case RMD_CMD_Speed:
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.current = (float)((Data[3]<<8)|Data[2])/100.;
        Motor->state.speed = (float)((Data[5]<<8)|Data[4]);
        Motor->state.angle = (float)((Data[7]<<8)|Data[6]);
        break;
      case RMD_CMD_ABSAngle:
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.current = (float)((Data[3]<<8)|Data[2])/100.;
        Motor->state.speed = (float)((Data[5]<<8)|Data[4]);
        Motor->state.angle = (float)((Data[7]<<8)|Data[6]);
        break;
      case RMD_CMD_SingleAngle: // Weird
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.current = (float)((Data[3]<<8)|Data[2])/100.;
        Motor->state.speed = (float)((Data[5]<<8)|Data[4]);
        Motor->state.angle = (float)((Data[7]<<8)|Data[6])/16384.*360.;
        break;
      case RMD_CMD_AddAngle:
        Motor->state.temperature = (int8_t)Data[1];
        Motor->state.current = (float)((Data[3]<<8)|Data[2])/100.;
        Motor->state.speed = (float)((Data[5]<<8)|Data[4]);
        Motor->state.angle = (float)((Data[7]<<8)|Data[6]);
        break;
      case RMD_CMD_GET_SystemRunMode:
        Motor->RunMode = Data[7];
        break;
      case RMD_CMD_GET_MotorPower:
        Motor->state.MotorPower = (float)((Data[7]<<8)|Data[6])*0.1;
        break;
      case RMD_CMD_GET_SystemRunTime:
        Motor->SystemRunTime = ((Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4]);
        break;
      case RMD_CMD_GET_SystemVersion:
        Motor->Version = (int)((Data[7]<<24)|(Data[6]<<16)|(Data[5]<<8)|Data[4]);
        break;
      case RMD_CMD_GET_MotorType:
        for (int i = 0; i<7; i++)
        Motor->Type[i] = (char)Data[i];
        break;
      case RMD_CMD_SET_485ID:
        printData("ID:%x\n", (Data[7]<<8)||Data[6]);
        break;
      default:
        break;}
}
void RS_Read_PID(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_PIDArg, Data);
  ReceiveData(Motor, RMD_CMD_Read_PIDArg);
}
void RS_Write_PID_RAM(RMD_Motordef* Motor, int8_t Cur_P, int8_t Cur_I, int8_t Vel_P, int8_t Vel_I, int8_t Pos_P, int8_t Pos_I)
{
  uint8_t Data[7] = {0};
  Data[1] = (uint8_t)Cur_P;
  Data[2] = (uint8_t)Cur_I;
  Data[3] = (uint8_t)Vel_P;
  Data[4] = (uint8_t)Vel_I;
  Data[5] = (uint8_t)Pos_P;
  Data[6] = (uint8_t)Pos_I;
  sendCMD(Motor, RMD_CMD_Write_PIDRAM, Data);
  // ReceiveData(Motor, RMD_CMD_Write_PIDRAM);
}
void RS_Write_PID_ROM(RMD_Motordef* Motor, int8_t Cur_P, int8_t Cur_I, int8_t Vel_P, int8_t Vel_I, int8_t Pos_P, int8_t Pos_I)
{
  uint8_t Data[7] = {0};
  Data[1] = (uint8_t)Cur_P;
  Data[2] = (uint8_t)Cur_I;
  Data[3] = (uint8_t)Vel_P;
  Data[4] = (uint8_t)Vel_I;
  Data[5] = (uint8_t)Pos_P;
  Data[6] = (uint8_t)Pos_I;
  sendCMD(Motor, RMD_CMD_Write_PIDROM, Data);
  // ReceiveData(Motor, RMD_CMD_Write_PIDROM);
}
void RS_Read_ACC(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_ACC, Data);
  ReceiveData(Motor, RMD_CMD_Read_ACC);
}
void RS_Write_ACC(RMD_Motordef* Motor, uint32_t Accelation)
{
  uint8_t Data[7] = {0};
  Data[3] = (uint8_t)(Accelation);
  Data[4] = (uint8_t)(Accelation>>8);
  Data[5] = (uint8_t)(Accelation>>16);
  Data[6] = (uint8_t)(Accelation>>24);
  sendCMD(Motor, RMD_CMD_Write_ACC, Data);
  // ReceiveData(Motor, RMD_CMD_Write_ACC);
}
void RS_Read_MultiEncoder(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_MultiEncoder, Data);
  ReceiveData(Motor, RMD_CMD_Read_MultiEncoder);
}
void RS_Read_RawMultiEncoder(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_RawMultiEncoder, Data);
  ReceiveData(Motor, RMD_CMD_Read_RawMultiEncoder);
}
void RS_Read_MultiEncoder_ZeroShift(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_MultiEncoder_ZeroShift, Data);
  ReceiveData(Motor, RMD_CMD_Read_MultiEncoder_ZeroShift);
}
void RS_SetZero_MultiEncoderVal(RMD_Motordef* Motor, uint32_t EncoderZeroShift)
{
  uint8_t Data[7] = {0};
  Data[3] = (uint8_t)(EncoderZeroShift);
  Data[4] = (uint8_t)(EncoderZeroShift>>8);
  Data[5] = (uint8_t)(EncoderZeroShift>>16);
  Data[6] = (uint8_t)(EncoderZeroShift>>24);
  sendCMD(Motor, RMD_CMD_SetZero_MultiEncoderVal, Data);
  // ReceiveData(Motor, RMD_CMD_SetZero_CurrentPos);
  HAL_Delay(1);
  RS_SystemReset(Motor);
}
void RS_SetZero_CurPosition(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_SetZero_CurrentPos, Data);
  // ReceiveData(Motor, RMD_CMD_SetZero_CurrentPos);
  RS_SystemReset(Motor);
}
void RS_Read_SingleEncoder(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_SingleEncoder, Data);
  ReceiveData(Motor, RMD_CMD_Read_SingleEncoder);
}
void RS_Read_MultiAngle(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_MultiAngle, Data);
  ReceiveData(Motor, RMD_CMD_Read_MultiAngle);
}
void RS_Read_SingleAngle(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_SingleAngle, Data);
  ReceiveData(Motor, RMD_CMD_Read_SingleAngle);
}
void RS_ReadState1(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_State1, Data);
  ReceiveData(Motor, RMD_CMD_Read_State1);
}
void RS_ReadState2(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_State2, Data);
  ReceiveData(Motor, RMD_CMD_Read_State2);
}
void RS_ReadState3(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Read_State3, Data);
  ReceiveData(Motor, RMD_CMD_Read_State3);
}
void RS_CloseBLDC(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_CloseMotor, Data);
}
void RS_StopBLDC(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_StopMotor, Data);
}
void RS_TorqueControl(RMD_Motordef* Motor, int16_t iqControl)
{
  uint8_t Data[7] = {0};
  Data[3] = (uint8_t)(iqControl);
  Data[4] = (uint8_t)(iqControl>>8);
  sendCMD(Motor, RMD_CMD_Torque, Data);
  ReceiveData(Motor, RMD_CMD_Torque);
}
void RS_speedControl(RMD_Motordef* Motor, int32_t speedControl)
{
  uint8_t Data[7] = {0};
  Data[3] = (uint8_t)(speedControl);
  Data[4] = (uint8_t)(speedControl>>8);
  Data[5] = (uint8_t)(speedControl>>16);
  Data[6] = (uint8_t)(speedControl>>24);
  sendCMD(Motor, RMD_CMD_Speed, Data);
  ReceiveData(Motor, RMD_CMD_Speed);
}
void RS_ABSangleControl(RMD_Motordef* Motor, float ref_angle, int16_t maxSpeed)
{
  uint8_t Data[7] = {0};
  int32_t angleCMD = (int32_t)(ref_angle*100.)+0.5;
  Data[1] = (uint8_t)(maxSpeed);
  Data[2] = (uint8_t)(maxSpeed>>8);
  Data[3] = (uint8_t)(angleCMD);
  Data[4] = (uint8_t)(angleCMD>>8);
  Data[5] = (uint8_t)(angleCMD>>16);
  Data[6] = (uint8_t)(angleCMD>>24);
  sendCMD(Motor, RMD_CMD_ABSAngle, Data);
  ReceiveData(Motor, RMD_CMD_ABSAngle);
}
void RS_SingleAngleControl(RMD_Motordef* Motor, int16_t ref_angle, int16_t maxSpeed)
{
  uint8_t Data[7] = {0};
  Data[0] = (ref_angle/100.-Motor->state.angle>0)?1:0; // SpinDirection
  Data[1] = (uint8_t)(maxSpeed);
  Data[2] = (uint8_t)(maxSpeed>>8);
  Data[3] = (uint8_t)(ref_angle);
  Data[4] = (uint8_t)(ref_angle>>8);
  sendCMD(Motor, RMD_CMD_SingleAngle, Data);
  ReceiveData(Motor, RMD_CMD_SingleAngle);
}
void RS_AddangleControl(RMD_Motordef* Motor, int32_t ref_angle, int16_t maxSpeed)
{
  uint8_t Data[7] = {0};
  Data[1] = (uint8_t)(maxSpeed);
  Data[2] = (uint8_t)(maxSpeed>>8);
  Data[3] = (uint8_t)(ref_angle);
  Data[4] = (uint8_t)(ref_angle>>8);
  Data[5] = (uint8_t)(ref_angle>>16);
  Data[6] = (uint8_t)(ref_angle>>24);
  sendCMD(Motor, RMD_CMD_AddAngle, Data);
  ReceiveData(Motor, RMD_CMD_AddAngle);
}
void RS_GET_SystemRunMode(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_GET_SystemRunMode, Data);
  ReceiveData(Motor, RMD_CMD_GET_SystemRunMode);
}
void RS_GET_MotorPower(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_GET_MotorPower, Data);
  ReceiveData(Motor, RMD_CMD_GET_MotorPower);
}
void RS_SystemReset(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_SystemReset, Data);
}
void RS_Brake_Release(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Brake_Release, Data);
}
void RS_Brake_Lock(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_Brake_Lock, Data);
}
void RS_GET_SystemRunTime(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_GET_SystemRunTime, Data);
  ReceiveData(Motor, RMD_CMD_GET_SystemRunTime);
}
void RS_GET_SystemVersion(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_GET_SystemVersion, Data);
  ReceiveData(Motor, RMD_CMD_GET_SystemVersion);
}
void RS_SET_CommuProtectTime(RMD_Motordef* Motor, int32_t ProtectTime_MS)
{
  uint8_t Data[7] = {0};
  Data[3] = (uint8_t)(ProtectTime_MS);
  Data[4] = (uint8_t)(ProtectTime_MS>>8);
  Data[5] = (uint8_t)(ProtectTime_MS>>16);
  Data[6] = (uint8_t)(ProtectTime_MS>>24);
  sendCMD(Motor, RMD_CMD_SET_CommuProtectTime, Data);
}
void RS_SET_BaudRate(RMD_Motordef* Motor, RMDL_BaudRate Baudrate)
{
  /* Baud Rate ----------------------
    - 0x00: 115200
    - 0x01: 500k
    - 0x02: 1M
    - 0x03: 1.5M
    - 0x04: 2.5M
  -------------------------------- */
  uint8_t Data[7] = {0};
  Data[6] = (uint8_t)(Baudrate);
  sendCMD(Motor, RMD_CMD_SET_BaudRate, Data);
}
void RS_GET_MotorType(RMD_Motordef* Motor)
{
  uint8_t Data[7] = {0};
  sendCMD(Motor, RMD_CMD_GET_MotorType, Data);
  ReceiveData(Motor, RMD_CMD_GET_MotorType);
}
void RS_Function(RMD_Motordef* Motor, uint8_t index, uint32_t value)
{
  uint8_t Data[7] = {0};
  Data[0] = index;
  Data[3] = (uint8_t)(value);
  Data[4] = (uint8_t)(value>>8);
  Data[5] = (uint8_t)(value>>16);
  Data[6] = (uint8_t)(value>>24);
  sendCMD(Motor, RMD_CMD_Function, Data);
  ReceiveData(Motor, RMD_CMD_Function);
}
void RS_MultiMotorControl(RMD_Motordef* Motor, uint8_t CMD, int MotorTotalNum)
{
  uint8_t Data[7] = {0};
  sendCMD(0, CMD, Data);
  ReceiveMultiData(Motor, CMD, MotorTotalNum);
  // for (int i = 0; i<2; i++){
  //   ReceiveData(&Motor[i], CMD);
  // }
}
void RS_SET_485ID(RMD_Motordef* Motor, uint8_t RW_Flag, uint8_t ID, int MotorTotalNum)
{
  uint8_t Data[7] = {0};
  Data[1] = RW_Flag; // 1:Read, 0:Write
  Data[6] = ID;
  sendCMD(0, RMD_CMD_SET_485ID, Data);
  ReceiveMultiData(Motor, RMD_CMD_SET_485ID, MotorTotalNum);
  // for (int i = 0; i<2; i++){
  //   ReceiveData(&Motor[i], RMD_CMD_SET_485ID);
  // }
}
void delay_us(uint16_t us)
{
 __HAL_TIM_SET_COUNTER(RS_timer, 0); // set the counter value a 0
 while (__HAL_TIM_GET_COUNTER(RS_timer) < us); // wait for the counter to reach the us input in the parameter
}