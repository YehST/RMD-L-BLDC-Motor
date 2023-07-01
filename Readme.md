# RMD_L_RS485函式庫使用說明

## Abstract
- This library is based on the communication datasheet provided by Myactuator and is compatible with the firmware version 2023/02 of the RMD-L series brushless motors. The communication protocol used is RS-485.

- This library is developed based on the STM32 HAL library. If you have any specific hardware requirements or need to use a different standard library, please make the necessary changes accordingly.

## Instructions For Use
- Required peripherals and pins for using this library:
  - UART for RS-485 communication *1
  - Pin for RS-485 communication mode switching *1
  - Timer for RS-485 communication delay *1
  - In addition, MAX485 chip is used to assist data transmission.
  
  Based on the above settings, the initialization function should be called first in the main file. The function requires the UART handle, mode switch pin, and timer handle as arguments. It is crucial to configure the timer to count every microsecond to ensure sufficient and accurate communication delay. For detailed information, please refer to the user manual of the STM32 development board you are using or consult the configuration data separately.
    ```C
        RS_Init_Handle(&huart2, GPIOA, GPIO_PIN_8, &htim8);
    ```
    After completing the communication, it is necessary to create the motor structure before use:
    ```C
        RMD_Motordef Motor = {
    	    .ID = 1
	    };
    ```
    When using multiple motors:
    ```C
        RMD_Motordef Motor[2] = {
            {.ID = 1},
            {.ID = 2}
    	};
    ```
    After the creation is completed, you can use all the read and write control commands in the library by passing the corresponding motor and data as arguments:
    ```C
        RS_ABSangleControl(&Motor[0], angleCMD, 1800);
    ```
    The data obtained from the motor response will be directly stored in the motor structure provided as an argument. You can retrieve the data by calling the appropriate functions when needed.

- NOTICE:
  1. When using multiple motors, it is necessary to introduce delays or implement timing control using an RTOS (Real-Time Operating System) to ensure proper sequencing between the motors. Here are the key points to consider:
  2. If you encounter issues with data transmission, check whether the baud rate settings of the motors and the board are the same. If they are the same, you can increase the value of "RS485_delay_time_us" in the library.

## Example
- In this example, the communication pins are configured using STM32 CubeMX. Two UART interfaces are set up for communication with the motors and the PC, respectively. A timer is used for RS485 communication delay, and a GPIO pin is used for switching the MAX485 mode. The pin configuration is as follows:
  
  | STM32 PIN | Device PIN |
  | --- | --- |
  |PD8|PC RX|
  |PD9|PC TX|
  |PA2|MAX485 RO|
  |PA3|MAX485 DI|
  |PA8|MAX485 RE&DE|
  |5V|VCC|
  |GND|GND|
