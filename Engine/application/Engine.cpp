#include "Engine.h"

Engine::Engine(Serial* uart, DigitalOut* engine, DigitalOut* status, double t_period_s) : uart(uart), engine(engine), status(status), t_period_s(t_period_s) {
  ini_ok = 0;
	timer = new RtosTimer(callback(this, &Engine::statemachine), osTimerPeriodic);
}

Engine::~Engine()
{
	Engine::stop();
}

void Engine::start()
{
	timer->start(t_period_s * 1000.0);
}

void Engine::stop()
{
	timer->stop();
	delete timer;
	delete &packet;
}

void Engine::statemachine()
{
    /*** input process image ***/
    uint8_t rx_data;
		if(uart->readable()) { rx_data = uart->getc(); }
		else								 { rx_data = 0;            }
    const uint8_t error = 0;//get_error();

    /*** processing ***/
    do
    {
        if(!ini_ok)               { t = 0;              }
        else if(state != m_state) { t = 0;              }
        else                      { t = t + t_period_s; }

        m_state = state;

        if(!ini_ok) { 
            state = IDLE;
            m_state = state;    
            attempts = 0;
        }
        else if(state != IDLE && t > 0 && error != 0) {
            tx_data = packet.build_control_packet(ECU_ID, MCU_ID, ERROR_2);
            state = SEND_ERROR;
        }
        else if(state == IDLE && t > 0 && attempts < 2 && packet.validate_data_packet(MCU_ID, rx_data)) {
						attempts++;
        }
				else if(state == IDLE && t > 0 && packet.validate_data_packet(MCU_ID, rx_data)) {
						attempts = 0;
            tx_data = packet.build_control_packet(ECU_ID, MCU_ID, ACKNOWLEDGE);
            state = SEND_ACK;
        }
				else if(state == IDLE && t > 0 && packet.validate_control_packet(ECU_ID, MCU_ID, ACKNOWLEDGE, rx_data)) {
						state = BUS_BUSY;
        }
        else if(state == BUS_BUSY && t > 0)
        {
            attempts = 0;
            state = IDLE;
        }
        else if(state == WAIT_FOR_ECU_DATA && t > 0 && packet.validate_data_packet(MCU_ID, rx_data)) {
            tx_data = packet.build_control_packet(ECU_ID, MCU_ID, ACKNOWLEDGE);
            state = SEND_ACK;
        }
        else if(state == SEND_ACK && t > 0) {
            state = WAIT_FOR_ECU_DATA;
        }
        else if(state == SEND_ERROR && t > 0) {
            state = IDLE;
        }
        // else;
    } while(state != m_state);

    /*** output process image ***/
    if(!ini_ok);
    else if(state == IDLE) { 
        *status = 0;                        
    }
    else if(state == BUS_BUSY) { 
        *status = 1;                        
    }
    else if(state == WAIT_FOR_ECU_DATA) {
        *status = 1;
    }
    else if(state == SEND_ACK) { 
        *status = 0;
        *engine = rx_data & DATA_MASK; 
        uart->putc(tx_data);
    }
    else if(state == SEND_ERROR) { 
        uart->putc(tx_data);                               
    }
    // else;

    ini_ok = 1;
}
