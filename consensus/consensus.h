

/* Responsible for keep the miner blocks updated and when is miner turn to mine a new block mine it as well */

#ifndef CONSENSUS_CONSENSUS_H
#define CONSENSUS_CONSENSUS_H

// Path to block part store directoryu
#define MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY "./data/"

#include "consensus_definitions.h"
#include "block_part.h"

#include <cstdint>

// Forward
namespace miner {

    struct Miner;

}

namespace wallet {

    struct Wallet;

}

namespace memory_pool::cuda {

    extern "C++" {

        void* get_transaction_data_from_memory_pool( uint32_t* );

    }

}

namespace consensus {

    extern unsigned char current_block_hash[ CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT + 1 ];

    // Returns 1 if is miner turn to mine a block
    bool is_miners_turn( void* );

    // Returns a generated block with all transactions in memory pool and sign by miner 
    void* get_block_part( wallet::Wallet*, uint32_t* );

    // Create sign and broudcast new block
    void miner_selected( miner::Miner* );

    // Its the main proccess for the consensus proccesses
    // Keeps all functions running as it is supose to 
    void consensus_main_process( miner::Miner* );


}


#endif