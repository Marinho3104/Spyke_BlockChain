

#include "connection.h" // Connection headers

// Compiler libs
#include <iostream>


p2p::Connection::Connection( unsigned char __version, void* __end_point ) 
    : version( __version ), end_point( __end_point ), socket_descriptor( -1 ), hint( 0 ) {}

// Tries to establish a connection with this end point
bool p2p::Connection::connect() {}

// Tries to set a server up with this end point
bool p2p::Connection::server() {}
