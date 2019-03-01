#ifndef __ENGINE_H
#define __ENGINE_H

#include "mbed.h"
#include "Packet.h"

typedef enum state_e 
{
	IDLE = 1,
	WAIT_FOR_ECU_DATA,
	SEND_ACK,
	SEND_ERROR,
} state_t;

class Engine {
public:
	Engine(Serial* uart, DigitalOut* engine, double t_period_s, double timeout_s);
	~Engine();
	void start();
	void stop();
		
private:  
	void statemachine();

	Packet packet;
	RtosTimer* timer;
	Serial* uart;
	DigitalOut* engine;

	uint8_t ini_ok;
	state_t state;
	state_t m_state;
	double  t;
	uint8_t tx_data;
	uint8_t motor_param;
	double t_period_s;
	double timeout_s;
};

#endif // __ENGINE_H