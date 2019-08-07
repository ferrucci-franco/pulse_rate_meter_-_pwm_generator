/* Includes ------------------------------------------------------------------*/
#include "pulses_input_and_pwm_output.h"

/* Private includes ----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
extern uint16_t mb_holding_register_buffer[MB_REG_HOLDING_NREGS];

extern TIM_HandleTypeDef htim2; // PWM 1
extern TIM_HandleTypeDef htim1; // PWM 2

/* Private user code ---------------------------------------------------------*/
uint16_t PSC_REG[] = {7999, 3199, 1599, 799, 319, 159, 79, 31}; // for CNT_CLK = 80MHz
uint16_t ARR_REG[] = {  99,   99,   99,  99,  99,  99, 99, 99};

TIM_HandleTypeDef *htim_pwm1 = &htim2;
TIM_HandleTypeDef *htim_pwm2 = &htim1;


MB_PWM_errorCode PWM_compute_PSC_and_ARR(MB_PWM_freq freq_code, uint16_t *PSC_ptr, uint16_t *ARR_ptr)
{
    MB_PWM_errorCode errorCode = PWM_noError;
    
    switch(freq_code)
	{
        case PWM_FREQ_100HZ:
			*PSC_ptr = PSC_REG[0];
			*ARR_ptr = ARR_REG[0];
            break;
    	case PWM_FREQ_250HZ:
			*PSC_ptr = PSC_REG[1];
			*ARR_ptr = ARR_REG[1];
            break;
		case PWM_FREQ_500HZ:
			*PSC_ptr = PSC_REG[2];
			*ARR_ptr = ARR_REG[2];
            break;
		case PWM_FREQ_1000HZ:
			*PSC_ptr = PSC_REG[3];
			*ARR_ptr = ARR_REG[3];
            break;
		case PWM_FREQ_2500HZ:
			*PSC_ptr = PSC_REG[4];
			*ARR_ptr = ARR_REG[4];
            break;
		case PWM_FREQ_5000HZ:
			*PSC_ptr = PSC_REG[5];
			*ARR_ptr = ARR_REG[5];
            break;
		case PWM_FREQ_10KHZ:
			*PSC_ptr = PSC_REG[6];
			*ARR_ptr = ARR_REG[6];
            break;
		case PWM_FREQ_25KHZ:
			*PSC_ptr = PSC_REG[7];
			*ARR_ptr = ARR_REG[7];
            break;
        default:
			*PSC_ptr = 0;
			*ARR_ptr = 0;
            errorCode = PWM_Error;
            break;
    }
    return errorCode;
}

MB_PWM_errorCode MB_PWM_Update()
{

    MB_PWM_errorCode errorCode = PWM_noError;
    
    /** Stops PWM timers if necessary ------------------------------------------*/  
    /* Check enable/disable Modbus status register */
    if (mb_holding_register_buffer[PWM1_STATUS] == PWM_DISABLE)
    {
        /* Stops pwm */
        HAL_TIM_PWM_Stop(htim_pwm1, TIM_CHANNEL_1);
    }

    if (mb_holding_register_buffer[PWM2_STATUS] == PWM_DISABLE)
    {
        /* Stops pwm */
        HAL_TIM_PWM_Stop(htim_pwm2, TIM_CHANNEL_1);  
    }

    /** Process PWM 1 -------------------------------------------------------------*/
    /* Update PWM timer */
    uint16_t PSC_1, ARR_1, CCR_1;
    errorCode = PWM_compute_PSC_and_ARR((MB_PWM_freq)mb_holding_register_buffer[PWM1_FREQ_CODE], &PSC_1, &ARR_1);
    if (PWM_Error == errorCode)
    {
        /* Invalid frequency code */
        errorCode = PWM_Error;        
    }
    else
    {
        /* Update timer registers */
        __HAL_TIM_SET_PRESCALER(htim_pwm1, PSC_1);
        __HAL_TIM_SET_AUTORELOAD(htim_pwm1, ARR_1);
    }

    /* Check if duty cycle is within range */
    CCR_1 = mb_holding_register_buffer[PWM1_DUTY_CYCLE];
    if (CCR_1 > 100)
    {
        /* Invalid duty cycle */
        errorCode = PWM_Error;
    }
    else
    {
        /* Update timer register */
        __HAL_TIM_SET_COMPARE(htim_pwm1, TIM_CHANNEL_1, CCR_1);
    }
    
    /** Process PWM 2 -------------------------------------------------------------*/
    /* Update PWM timer */
    uint16_t PSC_2, ARR_2, CCR_2;
    errorCode = PWM_compute_PSC_and_ARR((MB_PWM_freq)mb_holding_register_buffer[PWM2_FREQ_CODE], &PSC_2, &ARR_2);
    if (PWM_Error == errorCode)
    {
        /* Invalid frequency code */
        errorCode = PWM_Error;
    }
    else
    {
        /* Update timer registers */
        __HAL_TIM_SET_PRESCALER(htim_pwm2, PSC_2*2+1); // since TIM1 freq. is twice of TIM2
        __HAL_TIM_SET_AUTORELOAD(htim_pwm2, ARR_2);
    }

    /* Check if duty cycle is within range */
    CCR_2 = mb_holding_register_buffer[PWM2_DUTY_CYCLE];
    if (CCR_2 > 100)
    {
        /* Invalid duty cycle */
        errorCode = PWM_Error;
    }
    else
    {
        /* Update timer register */
        __HAL_TIM_SET_COMPARE(htim_pwm2, TIM_CHANNEL_1, CCR_2);
    }
    
    /** Starts PWM timers if necessary ------------------------------------------*/  
    /* Check enable/disable Modbus status register */
    if (mb_holding_register_buffer[PWM1_STATUS] != PWM_DISABLE)
    {
        /* Starts PWM (in case it hasn't already been started)*/
        HAL_TIM_PWM_Start(htim_pwm1, TIM_CHANNEL_1);
    }

    if (mb_holding_register_buffer[PWM2_STATUS] != PWM_DISABLE)
    {
        /* Starts PWM (in case it hasn't already been started)*/
        HAL_TIM_PWM_Start(htim_pwm2, TIM_CHANNEL_1);
    }
    return errorCode;
}

