

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

    extern "C++" {

        void initialize_memory_pool( uint64_t );

    }

}

#endif