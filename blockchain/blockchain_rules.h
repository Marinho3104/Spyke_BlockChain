

#ifndef BLOCKCHAIN_BLOCKCHAIN_RULES_H
#define BLOCKCHAIN_BLOCKCHAIN_RULES_H

// Compiler libs
#include <cstdint> // int_32
#include <limits> // Get types limits

namespace blockchain {

    /* Variables that are saved in blockchain context */

    // Block Division ( How many block parts per block )
    extern unsigned char block_division;

    /* Extra calculations to improve velocity */

    // Size span for each public key type ( Size for each type )
    extern uint32_t size_span_public_key;

    /* Constant Variables */ 
    
    // Bytes for public key type ( How much bytes it consider to get the public key type )
    const unsigned char bytes_public_key_type = 4; 

    // Max int32_t value
    const uint32_t max_int32_t_value = 
        std::numeric_limits < uint32_t >::max();

    // Update blockchain variables with blockchain context data
    void update_blockchain_variables();

    // Update extra variables 
    void update_extra_variables();

}

#endif