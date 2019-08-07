#include "max7219.h"

extern SPI_HandleTypeDef hspi2;

/*	MAX7219_Init handles all the initialization
 * 	routines neccessary for the proper functioning
 * 	of the device. Call this function always
 * 	before you try to use any other functions
 * 	from this library.
 *
 * 	RETVAL: none
 * */
 // send data;
  //decode led7seg
 uchar const LEDcode[19] =
{
    0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b, //0..9
    0x77,0x1f,0x4e,0x3d,0x4f,0x47,0x67,0x3e,0xff       //a..f,P,U,all_on
};

#define LED_CODE_d          0x3D
#define LED_CODE_DASH       0x01
#define LED_CODE_OVERFLOW   0x40
#define LED_CODE_OFF        0x00
#define LED_NO_OP_ADDR      0x00
#define LED_NO_OP_DATA      0x00
#define LED_DIGIT_0_ADDR    0x01
#define LED_DIGIT_1_ADDR    0x02
#define LED_DIGIT_2_ADDR    0x03
#define LED_DIGIT_3_ADDR    0x04
#define LED_DIGIT_4_ADDR    0x05
#define LED_DIGIT_5_ADDR    0x06
#define LED_DIGIT_6_ADDR    0x07
#define LED_DIGIT_7_ADDR    0x08

void MAX7219_SendByte (uchar dat)
{
    HAL_SPI_Transmit(&hspi2, &dat, 1, 1);   
}
 
// send address and dat;
void MAX7219_SendAddrDatTwoBytes(uchar addr1st, uchar dat1st, uchar addr2nd, uchar dat2nd)
{
    /* Set CS low */   
    HAL_GPIO_WritePin(MAX7219_CS_GPIO_Port, MAX7219_CS_Pin, GPIO_PIN_RESET);
    /* First byte */
    MAX7219_SendByte (addr1st);
    MAX7219_SendByte (dat1st);
    /* Second byte */
    MAX7219_SendByte (addr2nd);
    MAX7219_SendByte (dat2nd);
    /* Set CS high*/
    HAL_GPIO_WritePin(MAX7219_CS_GPIO_Port, MAX7219_CS_Pin, GPIO_PIN_SET);
}


    /** Initialization */
void MAX7219_Init ()
{
    MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, 0x0c, 0x01); // normal operation
    MAX7219_SendAddrDatTwoBytes(0x0c, 0x01, LED_NO_OP_ADDR, LED_NO_OP_DATA); // normal operation    
                                                                                
    MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, 0x0a, 0x06); // intensity
    MAX7219_SendAddrDatTwoBytes(0x0a, 0x06, LED_NO_OP_ADDR, LED_NO_OP_DATA); // intensity
                                                                                
    MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, 0x0b, 0x07); // all digits on
    MAX7219_SendAddrDatTwoBytes(0x0b, 0x07, LED_NO_OP_ADDR, LED_NO_OP_DATA); // all digits on
                                                                                
    MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, 0x09, 0x00); // decoding (no decode for digits 7–0)
    MAX7219_SendAddrDatTwoBytes(0x09, 0x00, LED_NO_OP_ADDR, LED_NO_OP_DATA); // decoding (no decode for digits 7–0)
                                                                                
    MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, 0x0f, 0x00); // display test off
    MAX7219_SendAddrDatTwoBytes(0x0f, 0x00, LED_NO_OP_ADDR, LED_NO_OP_DATA); // display test off
}

/** Display 8-digit number */
void MAX7219_Display8digitsInt(uint8_t displayNumber, uint32_t value)
{
    uint8_t is_first_display = 0;    

    /* Check if it is first MAX8219 chip */
    if (displayNumber == 0)
        is_first_display = 1;
    else
        is_first_display = 0;

    /* Check overflow */
    if(value > 99999999)
    {
        if (is_first_display)
        {
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_7_ADDR, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_6_ADDR, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_5_ADDR, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_4_ADDR, LED_CODE_OVERFLOW);        
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_0_ADDR, LED_CODE_OVERFLOW);
        }
        else
        {
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_7_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_6_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_5_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_4_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);        
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_0_ADDR, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
    }
    else
    {
        uint32_t a = value;
        uint32_t temp;

        /* Process 8th position */
        temp = a/10000000;
        if (temp == 0 && value < 10000000)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_7_ADDR, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_7_ADDR, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_7_ADDR, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_7_ADDR, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 10000000;

        /* Process 7th position */
        temp = a/1000000;
        if (temp == 0 && value < 1000000)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_6_ADDR, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_6_ADDR, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_6_ADDR, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_6_ADDR, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 1000000;

        /* Process 6th position */
        temp = a/100000;
        if (temp == 0 && value < 100000)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_5_ADDR, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_5_ADDR, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_5_ADDR, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_5_ADDR, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 100000;

        /* Process 5th position */
        temp = a/10000;
        if (temp == 0 && value < 10000)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_4_ADDR, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_4_ADDR, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_4_ADDR, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_4_ADDR, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 10000;
        
        /* Process 'one thousand' position */
        temp = a/1000;
        if (temp == 0 && value < 1000)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 1000;

        /* Process 'one hundred' position */
        temp = a/100;
        if (temp == 0 && value < 100)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 100;

        /* Process 'ten' position */
        temp = a/10;
        if (temp == 0 && value < 10)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 10;

        /* Process 'one' position */
        if (is_first_display)
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_0_ADDR, LEDcode[(a)]);
        else
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_0_ADDR, LEDcode[(a)], LED_NO_OP_ADDR, LED_NO_OP_DATA);        
    }
}

/** Display 4-digit number */
void MAX7219_DisplayNibbleInt(uint8_t nibble, uint32_t value)
{

    uint8_t offset = 0;
    uint8_t is_first_display = 0;    
    
    switch(nibble)
    {
        case 00:    // lower nibble of a byte
            offset = 0x00;
            is_first_display = 0x01;
            break;
        case 01:    // upper nibble of a byte
            offset = LED_DIGIT_3_ADDR;
            is_first_display = 0x01;
            break;
        case 02:    // lower nibble of a byte
            offset = 0x00;
            is_first_display = 0x00;
            break;
        case 03:    // upper nibble of a byte
            offset = LED_DIGIT_3_ADDR;
            is_first_display = 0x00;
            break;
        default:    // shouldn't be here!
            return; // exit without writing digit
    }

    /* Check overflow */
    if(value > 9999)
    {
        if (is_first_display)
        {
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR+offset, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR+offset, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_0_ADDR+offset, LED_CODE_OVERFLOW);
        }
        else
        {
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR+offset, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR+offset, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_0_ADDR+offset, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
    }
    else
    {
        uint32_t a = value;
        uint32_t temp;
        
        /* Process 'one thousand' position */
        temp = a/1000;
        if (temp == 0)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 1000;

        /* Process 'one hundred' position */
        temp = a/100;
        if (temp == 0 && value < 100)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR+offset, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR+offset, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR+offset, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR+offset, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 100;

        /* Process 'ten' position */
        temp = a/10;
        if (temp == 0 && value < 10)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR+offset, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR+offset, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR+offset, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR+offset, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 10;

        /* Process 'one' position */
        if (is_first_display)
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_0_ADDR+offset, LEDcode[(a)]);
        else
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_0_ADDR+offset, LEDcode[(a)], LED_NO_OP_ADDR, LED_NO_OP_DATA);        
    }
}

/** Display duty-cycle */
void MAX7219_DisplayNibbleDutyCycle(uint8_t nibble, uint32_t value)
{

    uint8_t offset = 0;
    uint8_t is_first_display = 0;    
    
    switch(nibble)
    {
        case 00:    // lower nibble of a byte
            offset = 0x00;
            is_first_display = 0x01;
            break;
        case 01:    // upper nibble of a byte
            offset = LED_DIGIT_3_ADDR;
            is_first_display = 0x01;
            break;
        case 02:    // lower nibble of a byte
            offset = 0x00;
            is_first_display = 0x00;
            break;
        case 03:    // upper nibble of a byte
            offset = LED_DIGIT_3_ADDR;
            is_first_display = 0x00;
            break;
        default:    // shouldn't be here!
            return; // exit without writing digit
    }

    /* Write 'd' */
    if (is_first_display)
    {
        MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LED_CODE_d);
    }
    else
    {
        MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LED_CODE_d,LED_NO_OP_ADDR, LED_NO_OP_DATA);
    }    
    
    /* Write number */
    if(value > 100)  // overflow
    {
        if (is_first_display)
        {
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR+offset, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR+offset, LED_CODE_OVERFLOW);
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_0_ADDR+offset, LED_CODE_OVERFLOW);
        }
        else
        {
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR+offset, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR+offset, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_0_ADDR+offset, LED_CODE_OVERFLOW,LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
    }
    else
    {
        uint32_t a = value;
        uint32_t temp;
        
        /* Process 'one hundred' position */
        temp = a/100;
        if (temp == 0)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR+offset, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR+offset, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_2_ADDR+offset, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_2_ADDR+offset, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 100;

        /* Process 'ten' position */
        temp = a/10;
        if (temp == 0 && value < 10)
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR+offset, LED_CODE_OFF);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR+offset, LED_CODE_OFF, LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        else
        {
            if (is_first_display)
                MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_1_ADDR+offset, LEDcode[(temp)]);
            else
                MAX7219_SendAddrDatTwoBytes(LED_DIGIT_1_ADDR+offset, LEDcode[(temp)], LED_NO_OP_ADDR, LED_NO_OP_DATA);
        }
        a %= 10;

        /* Process 'one' position */
        if (is_first_display)
            MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_0_ADDR+offset, LEDcode[(a)]);
        else
            MAX7219_SendAddrDatTwoBytes(LED_DIGIT_0_ADDR+offset, LEDcode[(a)], LED_NO_OP_ADDR, LED_NO_OP_DATA);        
    }
}

/** Dispaly 4 dashes */
void MAX7219_DisplayNibbleDashes(uint8_t nibble)
{
    uint8_t offset = 0;
    uint8_t is_first_display = 0;    
    
    switch(nibble)
    {
        case 00:    // lower nibble of a byte
            offset = 0x00;
            is_first_display = 0x01;
            break;
        case 01:    // upper nibble of a byte
            offset = LED_DIGIT_3_ADDR;
            is_first_display = 0x01;
            break;
        case 02:    // lower nibble of a byte
            offset = 0x00;
            is_first_display = 0x00;
            break;
        case 03:    // upper nibble of a byte
            offset = LED_DIGIT_3_ADDR;
            is_first_display = 0x00;
            break;
        default:    // shouldn't be here!
            return; // exit without writing digit
    }

    /* Write 'd---' */
    if (is_first_display)
    {
        MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LED_CODE_DASH);
        MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LED_CODE_DASH);
        MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LED_CODE_DASH);
        MAX7219_SendAddrDatTwoBytes(LED_NO_OP_ADDR, LED_NO_OP_DATA, LED_DIGIT_3_ADDR+offset, LED_CODE_DASH);
    }
    else
    {
        MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LED_CODE_DASH,LED_NO_OP_ADDR, LED_NO_OP_DATA);
        MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LED_CODE_DASH,LED_NO_OP_ADDR, LED_NO_OP_DATA);
        MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LED_CODE_DASH,LED_NO_OP_ADDR, LED_NO_OP_DATA);
        MAX7219_SendAddrDatTwoBytes(LED_DIGIT_3_ADDR+offset, LED_CODE_DASH,LED_NO_OP_ADDR, LED_NO_OP_DATA);
    } 
}
