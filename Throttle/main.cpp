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
#elif TARGET_ZEDBOARD 
	#define UART_TX STDIO_UART_TX
	#define UART_RX STDIO_UART_RX
#endif

/*******************************************************************/
Serial uart(UART_TX, UART_RX, 9600);
DigitalIn i_throttle(BUTTON1);
Throttle throttle(&uart, &i_throttle, 2e-3);

int main (void)
{
	throttle.start();
	
	while(1) 
	{

	}
}

