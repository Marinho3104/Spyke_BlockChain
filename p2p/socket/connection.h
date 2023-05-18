

#ifndef P2P_SOCKET_CONNECTION_H
#define P2P_SOCKET_CONNECTION_H


// Compiler libs

#include <cstddef> // size_t

namespace p2p {

    struct Connection {

        // End Point version IPv4 IPv6
        unsigned char version;

        // Connection End Point
        void* end_point;

        // Connection socket descriptor
        int socket_descriptor;

        // Connection hint
        void* hint;


        // Connection Data
        void* data; size_t data_size;


        Connection( unsigned char, void* );

        // Tries to establish a connection with this end point
        bool connect();

        // Tries to set a server up with this end point
        bool server( int );

        // Returns the length of end point data and version
        unsigned char get_end_point_version_length();

        // Prints current socket descriptor value and end point information
        void print();

    };

}

#endif