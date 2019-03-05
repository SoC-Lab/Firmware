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
#include "Engine.h"

#ifdef TARGET_NUCLEO_F072RB 
	#define UART_TX   PA_9
	#define UART_RX   PA_10
	#define USER_LED LED1
	#define ENGINE_PP PC_0
	#define THROTTLE_PP BUTTON1
#elif TARGET_ZEDBOARD 
	#define UART_TX STDIO_UART_TX
	#define UART_RX STDIO_UART_RX
	#define USER_LED LED0
	#define ENGINE_PP GPIO1
	#define THROTTLE_PP GPIO16
#endif

/*******************************************************************/
Serial uart(UART_TX, UART_RX, 9600);
DigitalOut engine_pp(ENGINE_PP, 1);
Engine engine(&uart, &engine_pp, 2e-3);
DigitalOut led(USER_LED);

int main (void)
{
	engine.start();
	
  while(1)
  {
		led = !led;
		wait(1.0);
  }
}

