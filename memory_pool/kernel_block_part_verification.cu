

#include "kernel_block_part_verification.cuh"


// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>

namespace memory_pool::cuda {

    void** block_part_verification_data;

    ::cuda::std::binary_semaphore* block_part_verification_semaphores;
    ::cuda::std::binary_semaphore* block_part_verification_thread_ready;

    cudaStream_t block_part_stream;

}

__global__ void memory_pool::cuda::kernel_block_part_verification( void** __data, ::cuda::std::binary_semaphore* __sems, ::cuda::std::binary_semaphore* __ready_thread, void** __memory_pool_data, ::cuda::std::binary_semaphore* __memory_pool_sems, ::cuda::std::binary_semaphore* __memory_pool_broudcast_sems ) {

    int _global_id = 
        blockIdx.x * blockDim.x + threadIdx.x;

    while( 1 ) {

        __sems[ _global_id ].acquire();

        // Makes all verifications

        // Add to memory_pool 
        kernel_block_part_store_block_part( __data[ _global_id ], __memory_pool_data, __memory_pool_sems, __memory_pool_broudcast_sems );

        __ready_thread[ _global_id ].release();

    }

}

__device__ void memory_pool::cuda::kernel_block_part_store_block_part( void* __block_part_data, void** __memory_pool, ::cuda::std::binary_semaphore* __memory_pool_sems, ::cuda::std::binary_semaphore* __memory_pool_broudcast_sems ) {

    for (
        int _ = 0; 
        _ < MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_PART_STORE_FOR_FILE_MEMORY;
        _++
    ) {

        if ( __memory_pool_sems[ _ ].try_acquire() ) {

            memcpy(
                __memory_pool + _,
                &__block_part_data,
                sizeof( void* )
            );


            __memory_pool_broudcast_sems[ _ ].release();

            printf("Block part stored\n"); break;

        }

    }

}
