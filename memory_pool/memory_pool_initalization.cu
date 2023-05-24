

#include "memory_pool_initialization.cuh"
#include "utils_functions_cuda.cuh"

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>

namespace memory_pool::cuda {

    ::cuda::std::binary_semaphore* memory_pool_sems;
    ::cuda::std::binary_semaphore* memory_pool_data_broudcast_sems;
    void* memory_pool;

    uint64_t* ready_transactions_count;
    uint64_t* memory_pool_transaction_capacity;

    unsigned char *public_key_type;

    bool *public_key_type_enable;

}

void memory_pool::cuda::initialize_memory_pool( uint64_t __memory_pool_size ) {

    cudaMallocManaged( &memory_pool_transaction_capacity, sizeof( uint64_t ) ); utils::cuda::check_cuda_error();
    cudaMallocManaged( &ready_transactions_count, sizeof( uint64_t ) ); utils::cuda::check_cuda_error();

    cudaMallocManaged( &public_key_type, sizeof( unsigned char ) ); utils::cuda::check_cuda_error();
    cudaMallocManaged( &public_key_type_enable, sizeof( bool ) ); utils::cuda::check_cuda_error();

    *memory_pool_transaction_capacity = __memory_pool_size; *public_key_type_enable = 0;

    // Allocate memory for transaction data be stored 
    cudaMallocManaged( &memory_pool, __memory_pool_size * TRANSACTION_PROPAGATION_LENGTH ); utils::cuda::check_cuda_error();

    // Allocated memory for memory pool semaphores
    cudaMallocManaged( &memory_pool_sems, sizeof( ::cuda::std::binary_semaphore ) * __memory_pool_size ); utils::cuda::check_cuda_error();
    cudaMallocManaged( &memory_pool_data_broudcast_sems, sizeof( ::cuda::std::binary_semaphore ) * __memory_pool_size ); utils::cuda::check_cuda_error();

    for ( uint64_t _ = 0; _ < __memory_pool_size; _++ ) { new ( memory_pool_sems + _ ) ::cuda::std::binary_semaphore( 1 ); new ( memory_pool_data_broudcast_sems + _ ) ::cuda::std::binary_semaphore( 0 ); }

}

