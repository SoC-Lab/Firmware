#include "Packet.h"

Packet::Packet() 
{
    
}

Packet::~Packet()
{
	
}

uint8_t Packet::build_control_packet(uint8_t dest_address, uint8_t source_address, uint8_t command)
{
    return  ((CONTROL_ID     & 0x03) << PACKET_ID_BIT_POS) | 
            ((dest_address   & 0x03) << DEST_ADDR_BIT_POS) | 
            ((source_address & 0x03) << SOURCE_ADDR_BIT_POS) | 
            ((command        & 0x03) << COMMAND_BIT_POS);
}

uint8_t Packet::build_data_packet(uint8_t dest_address, uint8_t data)
{
    return  ((dest_address & 0x03) << PACKET_ID_BIT_POS) | 
            ((data         & 0x3F) << DATA_BIT_POS);
}

uint8_t Packet::validate_data_packet(uint8_t source_address, uint8_t data_packet)
{
    if((data_packet & PACKET_ID_MASK) == ((source_address & 0x03) << PACKET_ID_BIT_POS)) return 1;
    else                                                                                 return 0;
}

uint8_t Packet::validate_control_packet(uint8_t dest_address, uint8_t source_address, uint8_t command, uint8_t control_packet)
{
    if((control_packet & PACKET_ID_MASK)   == (CONTROL_ID << PACKET_ID_BIT_POS) &&
       (control_packet & DEST_ADDR_MASK)   == ((dest_address & 0x03) << DEST_ADDR_BIT_POS) &&
       (control_packet & SOURCE_ADDR_MASK) == ((source_address & 0x03) << SOURCE_ADDR_BIT_POS) &&
       (control_packet & COMMAND_MASK)     == ((command & 0x03) << COMMAND_BIT_POS)) return 1;
    else                                                                             return 0;
}
