/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PULSES_INPUT_AND_PWM_OUTPUT_H
#define __PULSES_INPUT_AND_PWM_OUTPUT_H

/* Includes ------------------------------------------------------------------*/
#include "mbtask.h"
#include "stm32f4xx_hal.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/** PWM timers*/
#define PWM1_PIN_INIT_STATE	    GPIO_PIN_SET
#define PWM2_PIN_INIT_STATE     GPIO_PIN_SET

/* Mapping of Modbus holding register into PWM commands */
#define PWM1_STATUS         0
#define PWM1_FREQ_CODE      1
#define PWM1_DUTY_CYCLE     2
#define PWM2_STATUS         3
#define PWM2_FREQ_CODE      4
#define PWM2_DUTY_CYCLE     5

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    PWM_DISABLE = 0,
    PWM_ENABLE = 1
} MB_PWM_status;

typedef enum
{
    PWM_noError = 0,
    PWM_Error = 1
} MB_PWM_errorCode;

typedef enum
{
	PWM_FREQ_100HZ =  0,
	PWM_FREQ_250HZ =  1,
	PWM_FREQ_500HZ =  2,
	PWM_FREQ_1000HZ = 3,
	PWM_FREQ_2500HZ = 4,
	PWM_FREQ_5000HZ = 5,
	PWM_FREQ_10KHZ =  6,
	PWM_FREQ_25KHZ =  7
} MB_PWM_freq;

/* Exported functions prototypes ---------------------------------------------*/
MB_PWM_errorCode MB_PWM_Update();

#endif /* __PULSES_INPUT_AND_PWM_OUTPUT_H */
