#include "ECU.h"

ECU::ECU(Serial* uart, double t_period_s, double timeout_s) : uart(uart), t_period_s(t_period_s), timeout_s(timeout_s) {
  ini_ok = 0;
	timer = new RtosTimer(callback(this, &ECU::statemachine), osTimerPeriodic);
}

ECU::~ECU()
{
	ECU::stop();
}

void ECU::start()
{
	timer->start(t_period_s * 1000.0);
}

void ECU::stop()
{
	timer->stop();
	delete timer;
	delete &packet;
}

void ECU::statemachine()
{
	/*** input process image ***/
	uint8_t rx_data;
	if(uart->readable()) { rx_data = uart->getc(); }
	else								 { rx_data = 0;            }
    

	/*** processing ***/
	do
	{
		if(!ini_ok)               { t = 0;              }
		else if(state != m_state) { t = 0;              }
		else                      { t = t + t_period_s; }

		m_state = state;

		if(!ini_ok) { 
			state = START;
			m_state = state;
			attempt = 0;   
			throttle_pos = 0;
			motor_par = 0;
			tx_data = 0;             
		}
		else if(state == START && t > 0) {
			attempt = 1; 
			tx_data = packet.build_control_packet(THS_ID, ECU_ID, REQUEST); 
			state = REQUEST_TH_POS; 
		}
		else if(state == REQUEST_TH_POS && t > 0) { 
			state = RECEIVE_TH_POS;  
		}
		else if(state == RECEIVE_TH_POS && t > 0 && packet.validate_data_packet(ECU_ID, rx_data)) { 
			attempt = 1; 
			throttle_pos = rx_data & DATA_MASK; 
			state = CALC_MOTOR_PAR; 
		}
		else if(state == RECEIVE_TH_POS && t > timeout_s && attempt < 4) {  
			attempt++; 
			tx_data = packet.build_control_packet(THS_ID, ECU_ID, REQUEST); 
			state = REQUEST_TH_POS;
		}
		else if(state == RECEIVE_TH_POS && t > timeout_s) {  
			state = START;
		}
		else if(state == CALC_MOTOR_PAR && t > 0) {
			motor_par = throttle_pos;
			tx_data = packet.build_data_packet(MCU_ID, motor_par); 
			state = SEND_MOTOR_PAR;
		}
		else if(state == SEND_MOTOR_PAR && t > 0) { 
			state = WAIT_FOR_MCU_ACK;  
		}
		else if(state == WAIT_FOR_MCU_ACK && t > 0 && packet.validate_control_packet(ECU_ID, MCU_ID, ACKNOWLEDGE, rx_data)) { 
			attempt = 1; 
			tx_data = packet.build_control_packet(THS_ID, ECU_ID, REQUEST); 
			state = REQUEST_TH_POS; 
		}
		else if(state == WAIT_FOR_MCU_ACK && t > timeout_s && attempt < 4) { 
			attempt++; 
			tx_data = packet.build_data_packet(MCU_ID, throttle_pos);              
			state = SEND_MOTOR_PAR;                    
		}
		else if(state == WAIT_FOR_MCU_ACK && t > timeout_s) {          
			state = START;                   
		}
		// else;
	} while(state != m_state);

	/*** output process image ***/
	if(!ini_ok);
	else if(state == REQUEST_TH_POS) { uart->putc(tx_data); }
	else if(state == SEND_MOTOR_PAR) { uart->putc(tx_data); }
	// else;

	ini_ok = 1;
}
