/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MBTASK_H
#define _MBTASK_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

#include "mb.h"
#include "mbport.h"

/* Defines -------------------------------------------------------------------*/
#define MB_MY_SLAVE_ADDRESS 	4

#define PULSE_NUMBER_OF_INPUTS (13)

#define MB_REG_INPUT_START 		(700 + 1)
#define MB_REG_INPUT_NREGS 		(PULSE_NUMBER_OF_INPUTS * 2 + 1)
#define MB_REG_INPUT_FRAME 		(MB_REG_INPUT_NREGS - 1)    // Address of 'frame counter' register


#define MB_REG_HOLDING_START 	(600 + 1)
#define MB_REG_HOLDING_NREGS 	(6)

#define MB_PWM_PULL_UP_STATUS 			MB_REG_HOLDING_START
#define MB_PWM_PULL_UP_FREQ_CODE		(MB_PWM_PULL_UP_STATUS+1)
#define MB_PWM_PULL_UP_DUTY_CYCLE_CODE	(MB_PWM_PULL_UP_FREQ_CODE+1)

#define MB_PWM_OPEN_COLLECTOR_STATUS 			(MB_PWM_PULL_UP_DUTY_CYCLE_CODE+1)
#define MB_PWM_OPEN_COLLECTOR_FREQ_CODE			(MB_PWM_OPEN_COLLECTOR_STATUS+1)
#define MB_PWM_OPEN_COLLECTOR_DUTY_CYCLE_CODE	(MB_PWM_OPEN_COLLECTOR_FREQ_CODE+1)

#define MB_DONT_CARE    0       // It doesn't care since the UART is initialized before calling eMBInit

/* Exported variables --------------------------------------------------------*/
extern uint16_t mb_input_register_buffer[MB_REG_INPUT_NREGS];		    // the master can only read this buffer
extern uint16_t mb_holding_register_buffer[MB_REG_HOLDING_NREGS];       // the master can read/write this buffer
extern uint16_t mb_holding_register_buffer_prev[MB_REG_HOLDING_NREGS];  // used to check for changes in holding registers

/* Exported functions prototypes ---------------------------------------------*/
void ModbusRTUInitAndEnable(void const * argument);
void ModbusRTUTask(void const * argument);
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode );
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );
BOOL Modbus_RegHolding_have_changed();


#endif /* __MBTASK_H */

