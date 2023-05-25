

#include "transaction_definitions.h" // Transactions defintions
#include "block_part_definitions.h" // Block part definitions
#include "block_part.h" // Block Part Headers

// Compiler libs
#include <iostream>


void* types::Block_Part::get_transaction_data() { return &transactions_data; }

uint64_t types::Block_Part::get_representation_length( uint32_t __transactions_count ) {

    return TYPES_BLOCK_PART_PREVIOUS_HASH_LENGTH + TYPES_BLOCK_PART_HASH_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH + 
        WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH + TYPES_BLOCK_PART_TRANSACTION_COUNT_LENGTH + __transactions_count * (TRANSACTION_LENGTH);

}

