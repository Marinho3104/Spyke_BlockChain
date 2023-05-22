

#ifndef P2P_PROTOCOLS_PROPAGATION_PROTOCOL_H
#define P2P_PROTOCOLS_PROPAGATION_PROTOCOL_H

#include <cstdint> // int_64

namespace p2p {

    // Forward
    struct Packet;

    struct Propagation_Protocol {

        unsigned char propagation_type;
        uint64_t size;
        void* data;

        ~Propagation_Protocol(); Propagation_Protocol( unsigned char, uint64_t, void* );

        void handle();

        uint64_t get_representation_length();

        void* get_representation();

        Packet* get_packet();

        /* Static */

        static Propagation_Protocol* get_propagation_protocol( p2p::Packet* ); 



    };

}

#endif