
#include "kernel_transaction_verification.cuh" // Transaction verification functions
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
                ( transaction_verification_data, transaction_verification_semaphores, transaction_verification_thread_ready, memory_pool, memory_pool_sems, ready_transactions_count, memory_pool_transaction_capacity );

        utils::cuda::check_cuda_error();

    }


}