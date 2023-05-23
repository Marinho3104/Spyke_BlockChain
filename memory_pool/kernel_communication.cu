

#include "kernel_transaction_verification.cuh" // Transaction verification variables
#include "kernel_communication.cuh" // Kernel communication functions
#include "transaction_definitions.h" // Transaction definitions

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>

void memory_pool::cuda::handle_transaction( void* __transaction_data ) {

    for (
        int _ = 0;
        _ < MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS;
        _++
    ) {

        // Thread was available
        if ( transaction_verification_thread_ready[ _ ].try_acquire() ) {

            // Copies the transaction data into device memory
            memcpy(
                transaction_verification_data + _ * TRANSACTION_PROPAGATION_LENGTH,
                __transaction_data,
                TRANSACTION_PROPAGATION_LENGTH
            );

            transaction_verification_semaphores[ _ ].release(); break;

        }

    }

}
