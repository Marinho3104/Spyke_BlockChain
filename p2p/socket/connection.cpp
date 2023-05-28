

#include "connection_definitions.h" // Connection definitions
#include "connection.h" // Connection headers
#include "end_point_ipv4.h" // struct End_Point_IPv4
#include "socket_functions.h" // Socket functions

// Compiler libs
#include <iostream>

p2p::Connection::Connection() 
    : version( 0 ), end_point( 0 ), socket_descriptor( -1 ), hint( 0 ), status( P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED ), data( 0 ), data_size( 0 ) { sem_init( &send_sem, 0, 1 ); }

p2p::Connection::Connection( unsigned char __version, void* __end_point ) 
    : version( __version ), end_point( __end_point ), socket_descriptor( -1 ), hint( 0 ), status( P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED ), 
        data( 0 ), data_size( 0 ) { sem_init( &send_sem, 0, 1 ); }

p2p::Connection::Connection( unsigned char __version, void* __end_point, int __socket_descriptor, void* __hint, unsigned char __status ) 
    : version( __version ), end_point( __end_point ), socket_descriptor( __socket_descriptor ), hint( __hint ), status( __status ), data( 0 ), data_size( 0 ) { sem_init( &send_sem, 0, 1 ); }

bool p2p::Connection::connect() { return p2p::connect_to( this ); }

bool p2p::Connection::server( int __max_connections) { return p2p::set_server( this, __max_connections ); }

bool p2p::Connection::disconnect() { return p2p::disconnect_from( this ); }

bool p2p::Connection::send_packet( p2p::Packet* __packet ) { sem_wait( &send_sem ); bool _rtr = p2p::send_packet_to( this, __packet ); sem_post( &send_sem ); return _rtr; }

p2p::Packet* p2p::Connection::get_packet() { return p2p::get_packet_from( this ); }

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


