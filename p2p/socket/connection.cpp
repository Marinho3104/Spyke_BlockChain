

#include "connection_definitions.h" // Connection definitions
#include "connection.h" // Connection headers
#include "end_point_ipv4.h" // struct End_Point_IPv4
#include "socket_functions.h" // Socket functions

// Compiler libs
#include <iostream>


p2p::Connection::Connection( unsigned char __version, void* __end_point ) 
    : version( __version ), end_point( __end_point ), socket_descriptor( -1 ), hint( 0 ) {}

bool p2p::Connection::connect() { return 0; }

bool p2p::Connection::server( int __max_connections) { return p2p::set_server( this, __max_connections ); }

unsigned char p2p::Connection::get_end_point_version_length() {

    switch ( version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: return P2P_SOCKET_CONNECTION_VERSION_LENGTH + p2p::End_Point_IPv4::get_data_size(); break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6: 
    default: throw "Unknow version"; break;
    }

}

void p2p::Connection::print() {

    std::cout << "Socket Value: " << socket_descriptor << " End Point: "; 

    switch ( version)
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: ( ( p2p::End_Point_IPv4* ) end_point )->print(); break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6:
    default: throw "Unknow Ip version"; break;
    }

}


