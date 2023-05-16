

#include "socket_functions.h" // socket functions headers
#include "connection_definitions.h" // Connection types
#include "end_point_ipv4.h" // Struct End_Point_IPv4

// Compiler libs
#include <sys/socket.h> // Socket related
#include <sys/types.h> // Socket related
#include <arpa/inet.h> // Socket related
#include <iostream>
#include <unistd.h> // Linux lib
#include <netdb.h> // Socket related


bool p2p::connect_to( Connection* __connection ) {

    switch ( __connection->version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: return connect_to_IPv4( __connection ); break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6: 
    default: throw "Unknow Connection Version"; break;
    }

    return 0;

}

void p2p::disconnect_from( Connection* __connection ) { close( __connection->socket_descriptor ); }

bool p2p::connect_to_IPv4( Connection* __connection ) {

    // Makes a copy of the end point to avoid the need of casting every time we need it
    End_Point_IPv4* _end_point = 
        ( End_Point_IPv4* ) __connection->end_point;

    // Create the socket
    __connection->socket_descriptor = socket( AF_INET, SOCK_STREAM, 0 );

    if ( __connection->socket_descriptor < 0 ) return 0;

    // Allocates memory for hint
    sockaddr_in* _hint = 
        ( sockaddr_in* ) malloc( sizeof( sockaddr_in ) );

    // Set the socket information into hint
    _hint->sin_family = AF_INET;
    _hint->sin_port = htons( _end_point->port );
    _hint->sin_addr.s_addr = htonl( _end_point->address );

    // Connect to socket 
    if (
        connect(
            __connection->socket_descriptor,
            ( sockaddr* ) _hint,
            sizeof( *_hint )
        ) < 0
    ) return 0;

    return 1;

}

bool p2p::set_server( Connection* __connection, int __max_connections ) {

    switch ( __connection->version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: return set_server_IPv4( __connection, __max_connections ); break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6: 
    default: throw "Unknow Connection Version"; break;
    }

    return 0;

}

bool p2p::set_server_IPv4( Connection* __connection, int __max_connections ) {

    // Makes a copy of the end point to avoid the need of casting every time we need it
    End_Point_IPv4* _end_point = 
        ( End_Point_IPv4* ) __connection->end_point;

    __connection->socket_descriptor = 
        socket( AF_INET, SOCK_STREAM, 0 );

    if ( __connection->socket_descriptor < 0 ) return 0;

    // Just to dont have to wait for to free the address and port when it is retarted
    // After the testing we can remove it
    int _opt = 1;
    if (
        setsockopt(
            __connection->socket_descriptor,
            SOL_SOCKET,
            SO_REUSEADDR | SO_REUSEPORT,
            &_opt,
            sizeof( _opt )
        )
    ) return 0;

    // Allocates memory for hint
    sockaddr_in* _hint = 
        ( sockaddr_in* ) malloc( sizeof( sockaddr_in ) );

    // Set the socket information into hint
    _hint->sin_family = AF_INET;
    _hint->sin_port = htons( _end_point->port );
    _hint->sin_addr.s_addr = htonl( _end_point->address );

    // Bind the server
    if (
        bind(
            __connection->socket_descriptor,
            ( sockaddr* ) _hint,
            sizeof( *_hint )
        ) == -1
    ) return 0;

    // Listen 
    if (
        listen(
            __connection->socket_descriptor,
            __max_connections
        )
    ) return 0;

    return 1;

}