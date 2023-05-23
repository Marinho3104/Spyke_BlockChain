

#include "kernel_transaction_verification.cuh"
#include "memory_pool_initialization.cuh"
#include "transaction_definitions.h"

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <stdio.h>

namespace memory_pool::cuda {

    cudaStream_t transaction_verification_stream;

    void* transaction_verification_data;

    ::cuda::std::binary_semaphore* transaction_verification_semaphores;
    ::cuda::std::binary_semaphore* transaction_verification_thread_ready;

}

__global__ void memory_pool::cuda::kernel_transaction_verification( void* __data, ::cuda::std::binary_semaphore* __semaphore, ::cuda::std::binary_semaphore* __semaphore_ready, void* __memory_pool, ::cuda::std::binary_semaphore* __memory_pool_sems, uint64_t* __ready_transactions, uint64_t* __memory_pool_max_transactions_capacity ) {

    int _global_id = 
        blockIdx.x * blockDim.x + threadIdx.x;

    while( 1 ) {

        printf("To be Acquired %d\n", _global_id );

        __semaphore[ _global_id ].acquire();

        printf("Acquired %d\n", _global_id );

        // Make all needed confirmations TODO

        // If all confirmations succed
        
        kernel_store_transaction_data( __data + _global_id * TRANSACTION_PROPAGATION_LENGTH, __memory_pool, __memory_pool_sems, __ready_transactions, __memory_pool_max_transactions_capacity );

        __semaphore_ready[ _global_id ].release();

    }

}

__device__ void memory_pool::cuda::kernel_store_transaction_data( void* __transaction_data, void* __memory_pool, ::cuda::std::binary_semaphore* __memory_pool_sems, uint64_t* __ready_transactions, uint64_t* __memory_pool_max_transactions_capacity ) {

    for (
        uint64_t _ = 0; _ < *__memory_pool_max_transactions_capacity; _++
    ) {

        if ( __memory_pool_sems[ _ ].try_acquire() ) {

            memcpy(
                __memory_pool + _ * TRANSACTION_PROPAGATION_LENGTH,
                __transaction_data,
                TRANSACTION_PROPAGATION_LENGTH
            );
            
            break;

        }

    }

}






