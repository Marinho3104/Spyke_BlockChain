

#ifndef MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_CUH
#define MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_CUH

#include <cstdint>

// Nvcc libs
#include <cuda/semaphore>

// Blocks used in kernel call
#define MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCKS 1
// Block Threads used in kernel call
#define MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_THREADS 1

// All threads running block part verification at same time
#define MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS \
    MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCKS * MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_THREADS

// Pointer to thread blocks pointer addresses
#define MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_DATA \
    MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS * sizeof( void* )

// Semaphores Memory used by all thread blocks times blocks
#define MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_ALL_THREADS_SEMAPHORES_SIZE \ 
    MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS * sizeof( ::cuda::std::binary_semaphore )


namespace p2p {

    struct Connection;

}

namespace memory_pool::cuda {

    extern cudaStream_t block_part_stream;

    extern void** block_part_verification_data;

    extern ::cuda::std::binary_semaphore* block_part_verification_semaphores;

    extern ::cuda::std::binary_semaphore* block_part_verification_thread_ready;

    // Kernel to verify a block part
    __global__ void kernel_block_part_verification( void**, ::cuda::std::binary_semaphore* );

}

#endif