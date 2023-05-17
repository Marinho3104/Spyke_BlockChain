

#ifndef P2P_SOCKET_END_POINT_IPV4_H
#define P2P_SOCKET_END_POINT_IPV4_H


// Compiler libs

#include <cstdint> // uint_18 uint_64

namespace p2p {

    /* Holds a iPv4 end point */
    struct End_Point_IPv4 {

        uint32_t address;
        uint16_t port;

        End_Point_IPv4( uint32_t, uint16_t );

        // Prints the string representation of this end point
        void print();

        /* Static */

        // Returns this end point data length
        static unsigned char get_data_size();

    };

}

#endif