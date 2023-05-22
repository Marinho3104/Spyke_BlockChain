

#ifndef WALLET_WALLET_H
#define WALLET_WALLET_H

#include "wallet_definitions.h" // Wallet Definitions
#include "transaction.h" // Struct Transaction

// Compiler libs
#include <cstddef> // size_t
#include <cstdint> // int_32

// Forward
namespace p2p {

    struct Connection;
    struct Packet;

}

namespace wallet {

    /* Wallet Connections Information */
    struct Wallet_Connections_Information {

        p2p::Connection* communication_connections; uint16_t communication_connections_count;

        ~Wallet_Connections_Information(); Wallet_Connections_Information(); Wallet_Connections_Information( void* );

        void print();

    };

    /* Wallet Information */
    struct Wallet_Information {

        // Private Key 
        unsigned char private_key[ WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH ];

        // Public Key
        unsigned char public_key[ WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH ];

        // Public Key Type
        unsigned char public_key_type;

        Wallet_Information() = default; Wallet_Information( void* ); Wallet_Information( void*, void*, unsigned char );

        // Prints all wallet information
        void print();

    };

    /* Represents a Wallet, with all basic functions */
    struct Wallet {

        // Holds all Wallet Keys information related
        Wallet_Information wallet_information;

        // Holds all Wallet Nodes Connections information related
        Wallet_Connections_Information wallet_connections_information;

        // Others

        // When a new signature is make, the signature is stored here
        unsigned char signature[ WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH ]; 

        types::Transaction transaction_ready;

        ~Wallet(); Wallet();

        // Performs a signature with given data, stores in signature variable
        void sign( void*, size_t );

        // Performs a verfication about a signature with wallet information
        bool verify( void*, void*, size_t );

        void print();

        // Creates and sign a transaction
        void creates_and_signs_transaction();

        // Sends data to a available node
        void send_to_node( p2p::Packet* ); 

        /* Run otions */
        void run();

        /* Static Functions */

        // Returns a Wallet Struct with specify file data
        static Wallet* by_file( char* );

    };

}

#endif