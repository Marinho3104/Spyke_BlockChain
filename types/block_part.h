

#ifndef TYPES_BLOCK_PART_H
#define TYPES_BLOCK_PART_H

#include "block_part_definitions.h" // Block Part definitions
#include "wallet_definitions.h" // Ed25519 info

// Compiler libs

#include <cstdint> // uint_32 uint_64

namespace types {

    struct Block_Part {

        /* Header */

        // This and previous block hash
        unsigned char previous_hash[ TYPES_BLOCK_PART_PREVIOUS_HASH_LENGTH ], hash[ TYPES_BLOCK_PART_HASH_LENGTH ];

        // Miner signature
        unsigned char signature[ WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH ];

        // Miners public key
        unsigned char miner_public_key[ WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH ];

        // Transactions count inside this block part
        uint32_t transactions_count;


        /* Body */

        // Transactions data
        void* transactions_data;

        // Returns the pointer to transaction data
        void* get_transaction_data();

        // Returns the newest transaction with given public key
        void* get_transaction_data( void* );

        /* Static */

        // 
        static uint64_t get_representation_length( uint32_t );

    } __attribute__((packed));

}

#endif