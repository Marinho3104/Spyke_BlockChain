
#include "disconnect_protocol.h" // Struct Disconnect
#include "protocols_ids.h" // Protocols ids
#include "packet.h" // Struct Packet
 
// Compiler libs
#include <iostream>


p2p::Packet* p2p::Disconnect_Protocol::get_packet() {

    p2p::Packet* _packet = 
        ( p2p::Packet* ) malloc( sizeof( p2p::Packet ) );

    new ( _packet ) p2p::Packet(
        P2P_PROTOCOLS_PROTOCOLS_IDS_DISCONNECT_PROTOCOL,
        0, 0
    );

    return _packet;

}
