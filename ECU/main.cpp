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

/*******************************************************************/
void uart_tx_handler();
void uart_rx_handler();

DigitalOut led0(LED0, 1);
DigitalOut led1(LED1, 1);
DigitalOut led2(LED2, 1);
DigitalOut led3(LED3, 1);
Serial uart(STDIO_UART_TX, STDIO_UART_RX);
Thread thread;
Ticker taskclass_ticker;

void led0_thread() {
    while (true) {
			led0 = !led0;
			
			if(uart.readable())
			{
				if(uart.getc() == 0xAA) 
					{ 
						led2 = !led2;
					}
			}
			uart.putc(0xAA);
			wait(1);
    }
}

void taskclass()
{
	//led3 = !led3; 
}

int main (void)
{
	uart.attach(&uart_tx_handler, Serial::TxIrq);
	uart.attach(&uart_rx_handler, Serial::RxIrq);
	
	taskclass_ticker.attach(&taskclass, 1);
	
	thread.start(led0_thread);
    
  while(1)
  {

  }
}

void uart_tx_handler(void)
{
	
}

void uart_rx_handler(void)
{
	
}


extern "C" {
	void taskclass1(void);
	
	void taskclass1(void)
	{
		led1 = !led1; 
	}

	void UART0_Handler ( void )
	{
			led3 = !led3;
			NVIC_ClearPendingIRQ(UART0_IRQn);
	}
}

