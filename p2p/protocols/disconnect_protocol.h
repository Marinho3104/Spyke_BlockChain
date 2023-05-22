

#ifndef P2P_PROTOCOLS_DISCONNECT_PROTOCOL_H
#define P2P_PROTOCOLS_DISCONNECT_PROTOCOL_H

namespace p2p {

    // Forward
    struct Packet;

    struct Disconnect_Protocol {

        /* Static */

        static Packet* get_packet();

    };

}

#endif