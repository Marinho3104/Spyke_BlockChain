

#include "end_point_ipv4_definitions.h" // End Point IPv4 definitions
#include "end_point_ipv4.h" // End Point IPv4 headers

// Compiler libs
#include <iostream>


p2p::End_Point_IPv4::End_Point_IPv4( uint32_t __address, uint16_t __port ) : address( __address ), port( __port ) {}

void p2p::End_Point_IPv4::print() 
    { std::cout << ( address >> 24 ) << "." << ( address >> 16 & 0xf ) << "." << ( address >> 8 & 0xf ) << "." << ( address & 0xf ) << ":" << port;  }


unsigned char p2p::End_Point_IPv4::get_data_size() 
    { return P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH + P2P_SOCKET_END_POINT_IPV4_PORT_LENGTH; }

