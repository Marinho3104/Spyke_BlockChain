

#include "memory_pool_initialization.cuh"
#include "utils_functions_cuda.cuh"

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>

namespace memory_pool::cuda {

    ::cuda::std::binary_semaphore* memory_pool_sems;
    void* memory_pool;

    uint64_t* ready_transactions_count;
    uint64_t* memory_pool_transaction_capacity;

}

void memory_pool::cuda::initialize_memory_pool( uint64_t __memory_pool_size ) {

    cudaMalloc( &memory_pool_transaction_capacity, sizeof( uint64_t ) ); utils::cuda::check_cuda_error();
    cudaMalloc( &ready_transactions_count, sizeof( uint64_t ) ); utils::cuda::check_cuda_error();

    cudaMemcpy(
        memory_pool_transaction_capacity,
        &__memory_pool_size,
        sizeof( uint64_t ),
        cudaMemcpyHostToDevice
    ); utils::cuda::check_cuda_error();

    // Allocate memory for transaction data be stored 
    cudaMalloc( &memory_pool, __memory_pool_size * TRANSACTION_PROPAGATION_LENGTH ); utils::cuda::check_cuda_error();

    // Allocated memory for memory pool semaphores
    cudaMalloc( &memory_pool_sems, sizeof( ::cuda::std::binary_semaphore ) * __memory_pool_size ); utils::cuda::check_cuda_error();

    ::cuda::std::binary_semaphore* _bin_sems = 
        ( ::cuda::std::binary_semaphore* ) malloc( sizeof( ::cuda::std::binary_semaphore ) * __memory_pool_size );

    for ( uint64_t _ = 0; _ < __memory_pool_size; _++ ) new ( _bin_sems + _ ) ::cuda::std::binary_semaphore( 1 );

    cudaMemcpy(
        memory_pool_sems,
        _bin_sems,
        sizeof( ::cuda::std::binary_semaphore ) * __memory_pool_size,
        cudaMemcpyHostToDevice
    ); utils::cuda::check_cuda_error();

    free( _bin_sems );

}

