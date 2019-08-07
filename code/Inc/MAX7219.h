#ifndef		__MAX7219_H
#define		__MAX7219_H

#include "stm32f4xx_hal.h"
#include "main.h"

#define MAX7219_DIN_Pin         PIN_ROW_13_Pin
#define MAX7219_DIN_GPIO_Port   PIN_ROW_13_GPIO_Port

#define MAX7219_CLK_Pin         PIN_ROW_15_Pin
#define MAX7219_CLK_GPIO_Port   PIN_ROW_15_GPIO_Port

#define MAX7219_CS_Pin          PIN_ROW_14_Pin
#define MAX7219_CS_GPIO_Port    PIN_ROW_14_GPIO_Port

#define uchar unsigned char
#define ushort unsigned short
#define ulong unsigned long
#define uint unsigned int

void MAX7219_SendAddrDat (uchar addr, uchar dat);
void MAX7219_SendByte (uchar dat);
void MAX7219_Init (void);
void MAX7219_Clear(void);
void MAX7219_DisplayInt (uint val);
void MAX7219_Display8digitsInt(uint8_t displayNumber, uint32_t value);
void MAX7219_SendAddrDatTwoBytes(uchar addr1st, uchar dat1st, uchar addr2nd, uchar dat2nd);
void MAX7219_DisplayNibbleInt(uint8_t nibble, uint32_t value);
void MAX7219_DisplayNibbleDutyCycle(uint8_t nibble, uint32_t value);
void MAX7219_DisplayNibbleDashes(uint8_t nibble);

#endif
