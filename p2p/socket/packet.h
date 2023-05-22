

#ifndef P2P_SOCKET_PACKET_H
#define P2P_SOCKET_PACKET_H

#include <cstdint> // int_64

namespace p2p {

    struct Packet {

        unsigned char protocol_id;
        uint64_t size;
        void* data;

        ~Packet(); Packet( unsigned char, uint64_t, void* );

        uint64_t get_representation_length();

        void* get_representation();

        /* Static */
        
        static Packet* get_packet( unsigned char*, void* );

    };

}

#endif