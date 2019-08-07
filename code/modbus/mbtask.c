/* Includes ------------------------------------------------------------------*/
#include "mbtask.h"

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* ModBUS registers */
uint16_t mb_input_register_buffer[MB_REG_INPUT_NREGS];
uint16_t mb_holding_register_buffer[MB_REG_HOLDING_NREGS] = {0,0,0,0,0,0};
uint16_t mb_holding_register_buffer_prev[MB_REG_HOLDING_NREGS] = {0,0,0,0,0,0};


void ModbusRTUInitAndEnable(void const * argument)
{
	eMBErrorCode eStatus = eMBInit(MB_RTU, MB_MY_SLAVE_ADDRESS, MB_DONT_CARE, huart2.Init.BaudRate, huart2.Init.Parity);
	eStatus = eMBEnable();
}

void ModbusRTUTask(void const * argument)
{ 
	eMBErrorCode eStatus = eMBInit(MB_RTU, MB_MY_SLAVE_ADDRESS, MB_DONT_CARE, huart2.Init.BaudRate, huart2.Init.Parity);
	eStatus = eMBEnable();
    while(1)
    {
        eMBPoll();           
    }
}

/* eMBRegInputCB 
   Callback function used if the value of a Input Register is required by the protocol stack.*/
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode eStatus = MB_ENOERR;
    USHORT iRegIndex;

    if((usAddress >= MB_REG_INPUT_START) && (usAddress + usNRegs <= MB_REG_INPUT_START + MB_REG_INPUT_NREGS))
    {
        iRegIndex = usAddress - MB_REG_INPUT_START;
        while(usNRegs > 0)
        {
            *pucRegBuffer++ = (uint8_t)(mb_input_register_buffer[iRegIndex] >> 8 );	    // high byte
            *pucRegBuffer++ = (uint8_t)(mb_input_register_buffer[iRegIndex] & 0xFF );   // low byte
            iRegIndex++;
            usNRegs--;
        }
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    }
    else
    {
        HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
        eStatus = MB_ENOREG;			
    }
    return eStatus;
}

/* eMBRegHoldingCB
   Callback function used if a Holding Register value is read or written by the protocol stack */
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode eStatus = MB_ENOERR;
    USHORT iRegIndex;

    if((usAddress >= MB_REG_HOLDING_START) && (usAddress + usNRegs <= MB_REG_HOLDING_START + MB_REG_HOLDING_NREGS))
    {
        iRegIndex = usAddress - MB_REG_HOLDING_START;
        if (eMode == MB_REG_READ) // master reads registers
        {
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = (unsigned char)(mb_holding_register_buffer[iRegIndex] >> 8 );
                *pucRegBuffer++ = (unsigned char)(mb_holding_register_buffer[iRegIndex] & 0x00FF );
                iRegIndex++;
                usNRegs--;
            }
            HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
        }
        else if (eMode == MB_REG_WRITE)	// master writes registers
        {
            /* Copy buffer to chech for changer afterward*/
            for (int i = 0; i < MB_REG_HOLDING_NREGS; i++)
            {
                mb_holding_register_buffer_prev[i] = mb_holding_register_buffer[i];
            }

            while( usNRegs > 0 )
            {
                mb_holding_register_buffer[iRegIndex] = (uint16_t)(*pucRegBuffer++ << 8);
                mb_holding_register_buffer[iRegIndex] += (uint16_t)(*pucRegBuffer++ & 0x00FF);
                iRegIndex++;
                usNRegs--;
            }
            HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
        }
        else
        {
            HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
            eStatus = MB_ENOREG;	
        }
    }
    else
    {
            HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
            eStatus = MB_ENOREG;			
    }
    return eStatus;
}

/* eMBRegCoilsCB
   Callback function used if a Coil Register value is read or written by the protocol stack.*/
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
	return MB_ENOREG;
}

/* eMBRegDiscreteCB
   Callback function used if a Input Discrete Register value is read by the protocol stack. */
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
	return MB_ENOREG;
}

BOOL Modbus_RegHolding_have_changed()
{
    BOOL have_changed = FALSE;

    for(int i = 0; i < MB_REG_HOLDING_NREGS; i++)
    {
        if(mb_holding_register_buffer_prev[i] != mb_holding_register_buffer[i])
        {
            have_changed  = TRUE;
            /* Make previous reg. equal to current reg. to avoid re-entrance  */
            mb_holding_register_buffer_prev[i] = mb_holding_register_buffer[i];
        }
    }
    return have_changed;
}


