

#ifndef MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_CUH
#define MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_CUH

#include "transaction_definitions.h" // Transactions Definitions

// Nvcc libs
#include <cuda/semaphore>

// Blocks used in kernel call
#define MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS 1
// Block Threads used in kernel call
#define MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCK_THREADS 1024

// All threads running transaction verification at same time
#define MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS \
    MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS * MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCK_THREADS

// Transactions Memory used by all thread blocks times blocks
#define MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_ALL_THREADS_TRANSACTION_SIZE \
    MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS * TRANSACTION_PROPAGATION_LENGTH

// Semaphores Memory used by all thread blocks times blocks
#define MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_ALL_THREADS_SEMAPHORES_SIZE \ 
    MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS * sizeof( ::cuda::std::binary_semaphore )

namespace memory_pool::cuda {

    // Variables
    extern cudaStream_t transaction_verification_stream;

    extern void* transaction_verification_data;

    extern void* transaction_verification_previous_data;

    extern ::cuda::std::binary_semaphore* transaction_verification_previous_data_semphores;

    extern ::cuda::std::binary_semaphore* transaction_verification_semaphores;

    extern ::cuda::std::binary_semaphore* transaction_verification_thread_ready;

    // Checks previous block parts to get the previous data of current data
    void set_previous_data();

    // Kernel transaction verification 
    __global__ void kernel_transaction_verification( void*, ::cuda::std::binary_semaphore*, ::cuda::std::binary_semaphore*, void*, ::cuda::std::binary_semaphore*,  void*, ::cuda::std::binary_semaphore*, ::cuda::std::binary_semaphore*, uint64_t*, uint64_t*, unsigned char*, bool*, unsigned char*, uint32_t* );

    // Store the transaction data into memory pool
    __device__ void kernel_store_transaction_data( void*, void*, ::cuda::std::binary_semaphore*, ::cuda::std::binary_semaphore*, uint64_t*, uint64_t* );

    // Checks if a transaction is in the memory pool already
    __device__ bool kernel_memory_pool_transaction_check( void*, void*, uint64_t* );

    // Returns public key type
    __device__ unsigned char kernel_get_public_key_type( void*, unsigned char*, uint32_t* );

}

#endif