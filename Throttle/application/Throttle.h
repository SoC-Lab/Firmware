#ifndef __THROTTLE_H
#define __THROTTLE_H

#include "mbed.h"
#include "Packet.h"

typedef enum state_e 
{
    IDLE = 1,
		BUS_BUSY,
    WAIT_FOR_ECU_REQ,
    SEND_TH_POS,
    SEND_ERROR,
} state_t;

class Throttle {
public:
	Throttle(Serial* uart, DigitalIn* i_throttle, DigitalOut* status, double t_period_s);
		~Throttle();
		void start();
		void stop();
		
private:  
		void statemachine();

		Packet packet;
		RtosTimer* timer;
		Serial* uart;
		DigitalIn* i_throttle;
		DigitalOut* status;

		uint8_t attempts;
    uint8_t ini_ok;
    state_t state;
    state_t m_state;
    double  t;
    uint8_t tx_data;
		double t_period_s;
};

#endif // __THROTTLE_H
