/*
 * Copyright:
 * ----------------------------------------------------------------
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 *   (C) COPYRIGHT 2014, 2016 ARM Limited
 *       ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 * ----------------------------------------------------------------
 * File:     main.c
 * ----------------------------------------------------------------
 *
 */
 
/*
 * --------Included Headers--------
 */

#include "mbed.h"
#include "Throttle.h"

#ifdef TARGET_NUCLEO_F072RB 
	#define UART_TX PA_9
	#define UART_RX PA_10
	#define THROTTLE_PP BUTTON1
	#define USER_LED LED1
#elif TARGET_ZEDBOARD 
	#define UART_TX STDIO_UART_TX
	#define UART_RX STDIO_UART_RX
	#define THROTTLE_PP GPIO18
	#define USER_LED LED0
#endif

/*******************************************************************/
Serial uart(UART_TX, UART_RX, 9600);
DigitalIn i_throttle(THROTTLE_PP);
Throttle throttle(&uart, &i_throttle, 2e-3);
DigitalOut led(USER_LED);

int main (void)
{
	throttle.start();
	
	while(1) 
	{
		led = !led;
		wait(0.5);
	}
}

