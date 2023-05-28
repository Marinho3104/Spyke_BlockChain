

#ifndef P2P_SOCKET_CONNECTION_H
#define P2P_SOCKET_CONNECTION_H


// Compiler libs

#include <semaphore.h> // sem_t
#include <cstddef> // size_t

namespace p2p {

    // Forward
    struct Packet;


    struct Connection {

        // End Point version IPv4 IPv6
        unsigned char version;

        // Connection End Point
        void* end_point;

        // Connection socket descriptor
        int socket_descriptor;

        // Connection hint
        void* hint;

        // Connection Status
        unsigned char status;

        // Connection Data
        void* data; size_t data_size;

        // Semaphore for send calls been execute sync
        sem_t send_sem;


        Connection(); Connection( unsigned char, void* ); Connection( unsigned char, void*, int, void*, unsigned char );

        // Tries to establish a connection with this end point
        bool connect();

        // Tries to set a server up with this end point
        bool server( int );

        // Disconnect ( close ) socket descriptor
        bool disconnect();
        
        // Send a packet to this connection
        bool send_packet( p2p::Packet* );

        // Receive a packet from this connection
        p2p::Packet* get_packet();

        // Returns the length of end point data and version
        unsigned char get_end_point_version_length();

        // Prints current socket descriptor value and end point information
        void print();

    };

}

#endif