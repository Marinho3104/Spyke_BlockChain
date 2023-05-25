

#ifndef NODE_NODE_H
#define NODE_NODE_H

#include "connection.h"  // Struct Connection
#include "memory_pool.h" // Struct Memory_Pool

// Compiler libs
#include <cstdint> // int_32
#include <semaphore.h> // sem_t

namespace memory_pool::cuda {

    extern "C++" {

        // Sends all data from all transaction that have not been broudcasted yet
        void transactions_to_broudcast( p2p::Connection**, uint16_t );

    }

}

namespace node {

    /* Holds connections information about Node */
    struct Node_Connections_Information {

        // Server connection information
        p2p::Connection* server_connection;

        // Initial connection ( connections made when the node is initialized )
        p2p::Connection* initial_connections; uint16_t initial_connections_count;

        Node_Connections_Information(); Node_Connections_Information( void*, size_t& );

        // Prints all information about params
        void print();

        // Get bytes occupied in file by this params
        void get_size();

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

        // Prints all information about params
        void print();

    };

    /* Represents a node structure with all needed variables and functions */
    struct Node {

        // Connections informations
        Node_Connections_Information connections_information;

        // Node Information
        Node_Information node_information;

        // Temporary Variables
        bool node_is_running, server_is_up;

        // Ordinary connections info
        p2p::Connection** ordinary_connections; uint16_t ordinary_connections_count; sem_t ordinary_connections_sem;

        // Stable connections info
        p2p::Connection** stable_connections; uint16_t stable_connections_count; sem_t stable_connections_sem;

        // Open descriptor files 
        fd_set open_descriptor_files; sem_t open_descriptor_files_sem;

        // Node memory pool
        memory_pool::Memory_Pool memory_pool;

        Node();

        // Initialize all temporary variables
        void initialize_temporary_variable( void* );

        // Initialize memory pool
        void initialize_memory_pool( void* );

        // Adds a new connection into ordinary array
        bool add_ordinary_connection( p2p::Connection* );

        // Remove a connection into ordinary array
        bool remove_ordinary_connection( p2p::Connection* );

        // Adds a new connection into stable array
        bool add_stable_connection( p2p::Connection* );

        // Removes a connection into stable array
        bool remove_stable_connection( p2p::Connection* );

        // Remove a connection 
        bool remove_connection( p2p::Connection*, unsigned char );

        // Ads a new file descriptor to open_descriptor_files
        void add_new_file_descriptor( int );

        // Remove a file desciptor from open_descriptor_files
        void remove_file_descriptor( int );

        // Broudcast new data into Stable connections
        void broudcast_data();

        // Print all info about this node params
        void print();

        // Run a user interface
        void run_interface(); 

        // Run start running the Node
        void run();

        // Monitor all open file descriptors
        void monitor_open_file_descriptors();

        // Accept a new connection request 
        void accept_new_connection_request();

        // Handle protocol communication
        void p2p_communication( p2p::Connection*, unsigned char );


        /* Static */

        // Returns a new node with default settings
        static Node* get_new_node();

        // Returns a node with information from given file
        static Node* by_file( char* );

        // Returns a node with information from given file
        static Node* by_file( char*, void* );

    };

}

#endif