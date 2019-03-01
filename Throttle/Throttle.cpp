#include "Throttle.h"

Throttle::Throttle(Serial* uart, DigitalIn* i_throttle, double t_period_s, double timeout_s) : uart(uart), i_throttle(i_throttle), t_period_s(t_period_s), timeout_s(timeout_s) {
  ini_ok = 0;
	timer = new RtosTimer(callback(this, &Throttle::statemachine), osTimerPeriodic);
}

Throttle::~Throttle()
{
	Throttle::stop();
}

void Throttle::start()
{
	timer->start(t_period_s * 1000.0);
}

void Throttle::stop()
{
	timer->stop();
	delete timer;
	delete &packet;
}

void Throttle::statemachine()
{
    /*** input process image ***/
    uint8_t rx_data;
		if(uart->readable()) { rx_data = uart->getc(); }
		else								 { rx_data = 0;            }
    const uint8_t throttle_pos = *i_throttle;
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
        }
        else if(state != IDLE && t > 0 && error != 0) {
            tx_data = packet.build_control_packet(ECU_ID, THS_ID, ERROR_1);
            state = SEND_ERROR;
        }
        else if(state == IDLE && t > 0 && packet.validate_control_packet(THS_ID, ECU_ID, REQUEST, rx_data)) {
            state = WAIT_FOR_ECU_REQ;
        }
        else if(state == WAIT_FOR_ECU_REQ && t > 0 && packet.validate_data_packet(THS_ID, rx_data)) {
            state = IDLE;
        }
        else if(state == WAIT_FOR_ECU_REQ && t > 0 && packet.validate_control_packet(THS_ID, ECU_ID, REQUEST, rx_data)) {
            tx_data = packet.build_data_packet(ECU_ID, throttle_pos);
            state = SEND_TH_POS;
        }
        else if(state == SEND_TH_POS && t > 0) {
            state = WAIT_FOR_ECU_REQ;
        }
        else if(state == SEND_ERROR && t > 0) {
            state = IDLE;
        }
        // else;
    } while(state != m_state);

    /*** output process image ***/
    if(!ini_ok);
    else if(state == SEND_TH_POS) { uart->putc(tx_data);  }
    else if(state == SEND_ERROR)  { uart->putc(tx_data);  }
    // else;

    ini_ok = 1;
}
