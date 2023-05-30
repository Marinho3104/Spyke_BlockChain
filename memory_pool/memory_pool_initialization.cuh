

#ifndef MEMORY_POOL_INITIALIZATION_CUH
#define MEMORY_POOL_INITIALIZATION_CUH

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <cstdint>
#include <stdio.h>

#include "transaction_definitions.h"

namespace memory_pool::cuda {

    // Memory pool
    extern void* memory_pool;

    // Semaphores for memory pool
    extern ::cuda::std::binary_semaphore* memory_pool_sems;

    // Semaphores for memory pool ( signal if data have been broudcast already )
    extern ::cuda::std::binary_semaphore* memory_pool_data_broudcast_sems;

    // Current ready transactions
    extern uint64_t* ready_transactions_count;

    // Memory pool transactions capacity
    extern uint64_t* memory_pool_transaction_capacity;

    // Public key type
    extern unsigned char *public_key_type;

    // Is public key type enable
    extern bool *public_key_type_enable;

    // Block division
    extern unsigned char* block_division;

    // Block part span
    extern uint32_t* size_span_block_division;


    /* Block Part */

    extern void** block_part_memory_pool;

    extern ::cuda::std::binary_semaphore* block_part_memory_pool_semaphores;
    extern ::cuda::std::binary_semaphore* block_part_memory_pool_broudcast_semaphores;

    extern void* current_block_part_hash;

    extern "C++" {

        void initialize_memory_pool( uint64_t, void*, unsigned char, uint32_t );

        void update_current_hash( void* );

    }

}

#endif