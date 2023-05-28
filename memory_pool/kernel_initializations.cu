
#include "kernel_transaction_verification.cuh" // Transaction verification functions
#include "kernel_block_part_verification.cuh" // Block part functions
#include "kernel_initializations.cuh" // Extern functions
#include "memory_pool_initialization.cuh"
#include "utils_functions_cuda.cuh"

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>



extern "C++" {

    void memory_pool::cuda::launch_kernel_transaction_verification() {

        // Initialize stream
        cudaStreamCreate( &transaction_verification_stream ); utils::cuda::check_cuda_error();

        cudaMallocManaged( &transaction_verification_semaphores, MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_ALL_THREADS_SEMAPHORES_SIZE ); utils::cuda::check_cuda_error();
        cudaMallocManaged( &transaction_verification_thread_ready, MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_ALL_THREADS_SEMAPHORES_SIZE ); utils::cuda::check_cuda_error();

        for ( int _ = 0; _ < MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS; _++ ) {

            new ( transaction_verification_semaphores + _ ) ::cuda::std::binary_semaphore( 0 );
            new ( transaction_verification_thread_ready + _ ) ::cuda::std::binary_semaphore( 1 );

        }

        cudaMallocManaged( &transaction_verification_data, MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_ALL_THREADS_TRANSACTION_SIZE ); utils::cuda::check_cuda_error();

        // Launch function
        kernel_transaction_verification 
            <<< MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS, MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCK_THREADS, 0, transaction_verification_stream >>> 
                ( transaction_verification_data, transaction_verification_semaphores, transaction_verification_thread_ready, memory_pool, memory_pool_sems, memory_pool_data_broudcast_sems, ready_transactions_count, memory_pool_transaction_capacity, public_key_type, public_key_type_enable, block_division, size_span_block_division );

        utils::cuda::check_cuda_error();

    }

    void memory_pool::cuda::launch_kernel_block_part_verification() {

        // Initialize stream
        cudaStreamCreate( &block_part_stream ); utils::cuda::check_cuda_error();

        cudaMallocManaged( &block_part_verification_semaphores, MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_ALL_THREADS_SEMAPHORES_SIZE ); utils::cuda::check_cuda_error();
        cudaMallocManaged( &block_part_verification_thread_ready, MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_ALL_THREADS_SEMAPHORES_SIZE ); utils::cuda::check_cuda_error();

        for ( int _ = 0; _ < MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_ALL_THREADS_SEMAPHORES_SIZE; _++ ) {

            new ( block_part_verification_semaphores + _ ) ::cuda::std::binary_semaphore( 0 );
            new ( block_part_verification_thread_ready + _ ) ::cuda::std::binary_semaphore( 1 );

        }

        cudaMallocManaged( &block_part_verification_data, MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_DATA ); utils::cuda::check_cuda_error();

        for ( int _ = 0; _ < MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_DATA; _++ ) block_part_verification_data[ _ ] = 0;

        kernel_block_part_verification
            <<< MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCKS, MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_THREADS, 0, block_part_stream >>>
                ( block_part_verification_data, block_part_verification_semaphores );

        utils::cuda::check_cuda_error();

    }


}