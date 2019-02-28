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
DigitalOut led1(LED1, 1);
Thread thread;
Ticker taskclass_ticker;

void led0_thread() {
    while (true) {
			led1 = !led1;

			wait(1);
    }
}

void taskclass()
{
	//led3 = !led3; 
}

int main (void)
{
	taskclass_ticker.attach(&taskclass, 1);
	
	thread.start(led0_thread);
    
  while(1)
  {

  }
}

