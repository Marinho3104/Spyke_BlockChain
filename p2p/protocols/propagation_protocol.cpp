
#include "propagation_protocol_definitions.h" // Propagation definitions
#include "propagation_protocol.h" // Struct Propagation Protocol
#include "transaction_definitions.h" // Transaction Definitions
#include "transaction.h" // Struct Transaction
#include "protocols_ids.h" // Protocols ids
#include "packet.h" // Struct packet

// Compiler libs
#include <iostream>
#include <string.h> // memcpy

p2p::Propagation_Protocol::~Propagation_Protocol() {}

p2p::Propagation_Protocol::Propagation_Protocol( unsigned char __propagation_type, uint64_t __size, void* __data ) 
    : propagation_type( __propagation_type ), size( __size ), data( __data ) {}

void p2p::Propagation_Protocol::handle() {

    switch ( propagation_type )
    {
    case P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_TRANSACTION: memory_pool::cuda::handle_transaction( data ); break;
    default: break;
    }

}

uint64_t p2p::Propagation_Protocol::get_representation_length() 
    { return P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_LENGTH + size; }

void* p2p::Propagation_Protocol::get_representation() {

    void* _representation =     
        malloc( get_representation_length() );

    memcpy(
        _representation,
        &propagation_type,
        P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_LENGTH
    );

    memcpy(
        _representation + P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_LENGTH,
        data,
        size
    );

    return _representation;

}

p2p::Packet* p2p::Propagation_Protocol::get_packet() {

    p2p::Packet* _packet = 
        ( p2p::Packet* ) malloc( sizeof( p2p::Packet ) );

    new ( _packet ) p2p::Packet(
        P2P_PROTOCOLS_PROTOCOLS_IDS_PROPAGATION_PROTOCOL,
        get_representation_length(),
        get_representation()
    );

    return _packet;

}


p2p::Propagation_Protocol* p2p::Propagation_Protocol::get_propagation_protocol( p2p::Packet* __packet ) {

    p2p::Propagation_Protocol* _propagation_protocol = 
        ( p2p::Propagation_Protocol* ) malloc( sizeof( p2p::Propagation_Protocol ) );

    new ( _propagation_protocol ) p2p::Propagation_Protocol(
        *( unsigned char* ) __packet->data,
        __packet->size - P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_LENGTH,
        __packet->data + 1
    );

    return _propagation_protocol;

} 

