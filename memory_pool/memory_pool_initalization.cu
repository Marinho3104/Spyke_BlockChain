

#include "kernel_block_part_verification.cuh"
#include "memory_pool_initialization.cuh"
#include "utils_functions_cuda.cuh"
#include "blockchain_utils.h"

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

    unsigned char* block_division;

    uint32_t* size_span_block_division;


    void** block_part_memory_pool;
    ::cuda::std::binary_semaphore* block_part_memory_pool_broudcast_semaphores;
    ::cuda::std::binary_semaphore* block_part_memory_pool_semaphores;

}

void memory_pool::cuda::initialize_memory_pool( uint64_t __memory_pool_size, void* __public_key, unsigned char __block_division, uint32_t __size_span_block_division ) {

    cudaMallocManaged( &memory_pool_transaction_capacity, sizeof( uint64_t ) ); utils::cuda::check_cuda_error();
    cudaMallocManaged( &ready_transactions_count, sizeof( uint64_t ) ); utils::cuda::check_cuda_error();

    cudaMallocManaged( &block_division, sizeof( unsigned char ) ); utils::cuda::check_cuda_error();
    cudaMallocManaged( &size_span_block_division, sizeof( uint64_t ) ); utils::cuda::check_cuda_error();

    cudaMallocManaged( &public_key_type, sizeof( unsigned char ) ); utils::cuda::check_cuda_error();
    cudaMallocManaged( &public_key_type_enable, sizeof( bool ) ); utils::cuda::check_cuda_error();

    *memory_pool_transaction_capacity = __memory_pool_size; *public_key_type_enable = __public_key;

    *block_division = __block_division; *size_span_block_division = __size_span_block_division;

    if ( *public_key_type_enable )

        *public_key_type = 
            blockchain::get_public_key_type( __public_key );

    // Allocate memory for transaction data be stored 
    cudaMallocManaged( &memory_pool, __memory_pool_size * TRANSACTION_PROPAGATION_LENGTH ); utils::cuda::check_cuda_error();

    // Allocated memory for memory pool semaphores
    cudaMallocManaged( &memory_pool_sems, sizeof( ::cuda::std::binary_semaphore ) * __memory_pool_size ); utils::cuda::check_cuda_error();
    cudaMallocManaged( &memory_pool_data_broudcast_sems, sizeof( ::cuda::std::binary_semaphore ) * __memory_pool_size ); utils::cuda::check_cuda_error();

    for ( uint64_t _ = 0; _ < __memory_pool_size; _++ ) { new ( memory_pool_sems + _ ) ::cuda::std::binary_semaphore( 1 ); new ( memory_pool_data_broudcast_sems + _ ) ::cuda::std::binary_semaphore( 0 ); }

    cudaMallocManaged( &block_part_memory_pool, MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_PART_STORE_FOR_FILE_MEMORY * sizeof( void* ) ); utils::cuda::check_cuda_error();

    for ( uint64_t _ = 0; _ < MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_PART_STORE_FOR_FILE_MEMORY; _++ ) block_part_memory_pool[ _ ] = 0;

    cudaMallocManaged( &block_part_memory_pool_semaphores, sizeof( ::cuda::std::binary_semaphore ) * MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_PART_STORE_FOR_FILE_MEMORY ); 
    cudaMallocManaged( &block_part_memory_pool_broudcast_semaphores, sizeof( ::cuda::std::binary_semaphore ) * MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_PART_STORE_FOR_FILE_MEMORY ); 
    utils::cuda::check_cuda_error();

    for ( uint64_t _ = 0; _ < MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_PART_STORE_FOR_FILE_MEMORY; _++ )

        { new ( block_part_memory_pool_semaphores + _ ) ::cuda::std::binary_semaphore( 1 ); new ( block_part_memory_pool_broudcast_semaphores + _ ) ::cuda::std::binary_semaphore( 0 ); }

        

}

