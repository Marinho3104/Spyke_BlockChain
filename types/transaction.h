

#ifndef TYPES_TRANSACTION_H
#define TYPES_TRANSACTION_H

#include "wallet_definitions.h" // Ed25519 info

// Compiler libs

#include <cstdint> // int_64

namespace types {

    /* Represents a Transaction Structure */
    struct Transaction {

        // From signature
        unsigned char signature[ WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH ];

        // From and To public keys
        unsigned char from[ WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH ], to[ WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH ];

        // From amount and fee spent
        int64_t amount, fee;

        // Transaction nonce
        int64_t nonce;

        // From and To Balance after transaction accepted
        int64_t from_balance, to_balance;

    // Needs to be packed so it occupies exactly the size intended to be and to more easly convert when it is readed from a file
    } __attribute__((packed));

}

#endif