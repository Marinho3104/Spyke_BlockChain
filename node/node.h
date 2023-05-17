

#ifndef NODE_NODE_H
#define NODE_NODE_H

#include "connection.h"  // Struct Connection


// Compiler libs
#include <cstdint> // int_32

namespace node {

    /* Holds connections information about Node */
    struct Node_Connections_Information {

        // Server connection information
        p2p::Connection* server_connection;

        // Initial connection ( connections made when the node is initialized )
        p2p::Connection* initial_connections; uint16_t initial_connections_count;

        Node_Connections_Information(); Node_Connections_Information( void* );

    };

    /* Holds Node information */
    struct Node_Information {

        // Max Number of open connections the server can handle at same time 
        uint16_t max_server_open_connections;

        // Max number of ordinary connections at same time
        uint16_t max_ordinary_connections; 

        // Max number of stable connections at same time
        uint16_t max_stable_connections;

        // Number of bytes the memory pool is allocated with 
        uint64_t memory_pool_size;

        Node_Information(); Node_Information( void* );

    };

    /* Represents a node structure with all needed variables and functions */
    struct Node {

        // Connections informations
        Node_Connections_Information connections_information;

        // Node Information
        Node_Information node_information;

        Node();


        /* Static */

        // Returns a new node with default settings
        static Node* get_new_node();

        // Returns a node with information from given file
        static Node* by_file( char* );

    };

}

#endif