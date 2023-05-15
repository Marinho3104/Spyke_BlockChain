

#ifndef WALLET_WALLET_H
#define WALLET_WALLET_H

#include "wallet_definitions.h" // Wallet Definitions

// Compiler libs

#include <cstddef> // size_t

namespace wallet {

    /* Wallet Connections Information */
    struct Wallet_Connections_Information {



    };

    /* Wallet Information */
    struct Wallet_Information {

        // Private Key 
        unsigned char private_key[ WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH ];

        // Public Key
        unsigned char public_key[ WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH ];

        // Public Key Type
        unsigned char public_key_type;

        Wallet_Information() = default; Wallet_Information( void*, void*, unsigned char );

        // Prints all wallet information
        void print();

    };

    /* Represents a Wallet, with all basic functions */
    struct Wallet {

        // Holds all Wallet Keys information related
        Wallet_Information wallet_information;

        // Holds all Wallet Nodes Connections information related
        // Wallet_Connections_Information wallet_connections_information;

        // Others

        // When a new signature is make, the signature is stored here
        unsigned char signature[ WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH ]; 

        ~Wallet(); Wallet();

        // Performs a signature with given data, stores in signature variable
        void sign( void*, size_t );

        // Performs a verfication about a signature with wallet information
        bool verify( void*, void*, size_t );


        /* Run otions */
        void run();

        // Save wallet data into file
        bool save( char* );

        /* Static Functions */

        // Returns a new Wallet Struct with default informaiton and a new Key Pair
        static Wallet* get_new_wallet( unsigned char );

        // Returns a Wallet Struct with specify file data
        static Wallet* by_file( char* );

    };

}

#endif