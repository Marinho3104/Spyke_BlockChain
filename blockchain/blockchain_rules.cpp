
#include "blockchain_rules.h" // Block Chain Rules Headers

// Compiler Libs
#include <iostream>

// Initialize Variables Once 
namespace blockchain {

    unsigned char block_division;

    uint32_t size_span_public_key;

}

void blockchain::update_blockchain_variables() {

    // Get the information from blokchain files TODO

    block_division = 2;

    update_extra_variables();

}

void blockchain::update_extra_variables() {

    // Set the block division span
    size_span_public_key = 
        max_int32_t_value / block_division;

}


