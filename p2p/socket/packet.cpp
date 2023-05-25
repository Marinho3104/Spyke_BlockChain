

#include "packet_definitions.h" // Packet definitions
#include "packet.h" // Struct Packet

// Compiler Libs
#include <iostream>
#include <string.h> // memcpy

p2p::Packet::~Packet() { free( data ); } 

p2p::Packet::Packet( unsigned char __protocol_id, uint64_t __size, void* __data ) : protocol_id( __protocol_id ), size( __size ), data( __data ) {}

uint64_t p2p::Packet::get_representation_length() { return P2P_SOCKET_PACKET_DEFINITIONS_PROTOCOL_ID_LENGTH + P2P_SOCKET_PACKET_DEFINITIONS_SIZE_LENGTH + size; }

void* p2p::Packet::get_representation() {

    void* _representation = 
        malloc( get_representation_length() );

    memcpy(
        _representation,
        &protocol_id,
        P2P_SOCKET_PACKET_DEFINITIONS_PROTOCOL_ID_LENGTH
    );

    memcpy(
        _representation + P2P_SOCKET_PACKET_DEFINITIONS_PROTOCOL_ID_LENGTH,
        &size,
        P2P_SOCKET_PACKET_DEFINITIONS_SIZE_LENGTH
    );

    memcpy(
        _representation + P2P_SOCKET_PACKET_DEFINITIONS_PROTOCOL_ID_LENGTH + P2P_SOCKET_PACKET_DEFINITIONS_SIZE_LENGTH,
        data,
        size
    );

    return _representation;

}

p2p::Packet* p2p::Packet::get_packet( unsigned char* __headers, void* __body) {

    p2p::Packet* _packet =  
        ( p2p::Packet* ) malloc( sizeof( p2p::Packet ) );

    new ( _packet ) p2p::Packet(
        *__headers,
        *( uint64_t* ) ( __headers + 1 ),
        __body
    );

    return _packet;

}

