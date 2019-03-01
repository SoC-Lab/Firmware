#ifndef __ECU_H
#define __ECU_H

#include "mbed.h"
#include "Packet.h"

typedef enum state_e 
{
    START = 1,
    REQUEST_TH_POS,
    RECEIVE_TH_POS,
    CALC_MOTOR_PAR,
    SEND_MOTOR_PAR,
    WAIT_FOR_MCU_ACK,
} state_t;

class ECU {
public:
    ECU(Serial* uart, double t_period_s, double timeout_s);
		~ECU();
		void start();
		void stop();
		
private:  
		void statemachine();

		Packet packet;
		RtosTimer* timer;
		Serial* uart;

    uint8_t ini_ok;
    state_t state;
    state_t m_state;
    double  t;
    uint8_t throttle_pos;
    uint8_t tx_data;
    uint8_t attempt;
    uint8_t motor_par;
		double t_period_s;
		double timeout_s;
};

#endif // __ECU_H
