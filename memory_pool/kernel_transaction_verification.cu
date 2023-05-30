

#include "kernel_transaction_verification.cuh"
#include "kernel_block_part_verification.cuh"
#include "memory_pool_initialization.cuh"
#include "transaction_definitions.h"
#include "utils_functions.h"
#include "transaction.h"
#include "block_part.h"

#include <iostream>

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <stdio.h>

namespace memory_pool::cuda {

    cudaStream_t transaction_verification_stream;

    void* transaction_verification_data;
    void* transaction_verification_previous_data;

    ::cuda::std::binary_semaphore* transaction_verification_previous_data_semphores;

    ::cuda::std::binary_semaphore* transaction_verification_semaphores;
    ::cuda::std::binary_semaphore* transaction_verification_thread_ready;

}

void memory_pool::cuda::set_previous_data() {

    char* _file_path = 
        ( char* ) malloc( sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 128 );

    void* _hash_hex;

    memcpy(
        _file_path,
        MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY,
        sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1
    );

    _file_path[ sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 127 ] = 0; 

    void* _current_block_hash_search;

    while( 1 ) {

        _current_block_hash_search = current_block_part_hash;

        for ( int _ = 0; _ < MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS; _++ ) {
        
            if ( ! transaction_verification_thread_ready[ _ ].try_acquire() ) {

                while( 1 ) {

                    std::cout << "Search loop" << std::endl;

                    _hash_hex = 
                        utils::convert_bytes_hex( _current_block_hash_search, 64 );

                    std::cout << ( char* ) _hash_hex << std::endl;

                    memcpy(
                        _file_path + sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1,
                        _hash_hex,
                        128
                    ); free( _hash_hex );   

                    size_t _file_data_size;

                    void* _block_part_data = 
                        utils::get_file_data( _file_path, _file_data_size );

                    if ( ! _block_part_data ) { std::cout << "Block from file null " << std::endl; exit( 1 ); }

                    types::Block_Part* _block_part = 
                        ( types::Block_Part* ) _block_part_data;

                    _current_block_hash_search = _block_part->previous_hash;

                    void* _previous_data = 
                        _block_part->get_transaction_data(
                            transaction_verification_data + 64
                        );

                    if ( ! _previous_data ) { std::cout << "Transaction data null " << std::endl; continue; }

                    memcpy(
                        transaction_verification_previous_data + _ * TRANSACTION_LENGTH,
                        _previous_data,
                        TRANSACTION_LENGTH
                    );

                    transaction_verification_previous_data_semphores->release();

                    break;

                }

            }

            else transaction_verification_thread_ready[ _ ].release();
        
        }

        sleep( 2 );

    }

}

__global__ void memory_pool::cuda::kernel_transaction_verification( void* __data, ::cuda::std::binary_semaphore* __semaphore, ::cuda::std::binary_semaphore* __semaphore_ready, void* __previous_data, ::cuda::std::binary_semaphore* __previous_data_semaphores, void* __memory_pool, ::cuda::std::binary_semaphore* __memory_pool_sems, ::cuda::std::binary_semaphore* __memory_pool_broudcast_sems,  uint64_t* __ready_transactions, uint64_t* __memory_pool_max_transactions_capacity, unsigned char* __public_key_type, bool* __public_key_type_enable, unsigned char* __block_division, uint32_t* __size_span_public_key ) {

    int _global_id = 
        blockIdx.x * blockDim.x + threadIdx.x;

    while( 1 ) {

        // printf("To be Acquired %d\n", _global_id );

        __semaphore[ _global_id ].acquire();

        printf("Acquired %d\n", _global_id );

        printf("Waiting for previous data %d\n", _global_id );

        __previous_data_semaphores->acquire();

        printf("Acquired previous data %d\n", _global_id );

        // printf("Public key enable %d\n", *__public_key_type_enable );

        // Checks if transaction is already in pool
        if ( ! kernel_memory_pool_transaction_check( __data + _global_id * TRANSACTION_PROPAGATION_LENGTH, __memory_pool, __memory_pool_max_transactions_capacity ) ) {

            // Make all needed confirmations TODO

            types::Transaction* _previous_transaction = 
                ( types::Transaction* ) ( __previous_data + _global_id * TRANSACTION_LENGTH );

            types::Transaction* _transaction = 
                ( types::Transaction* ) ( __data + _global_id * TRANSACTION_PROPAGATION_LENGTH );

            if (
                _previous_transaction->from_balance >= _transaction->amount + _transaction->fee
            ) {

                // If all confirmations succeed

                // Checks if public key type is enable
                if ( ! *__public_key_type_enable || kernel_get_public_key_type( __data + _global_id * TRANSACTION_PROPAGATION_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH, __block_division, __size_span_public_key ) == *__public_key_type ) 
                
                    kernel_store_transaction_data( __data + _global_id * TRANSACTION_PROPAGATION_LENGTH, __memory_pool, __memory_pool_sems, __memory_pool_broudcast_sems, __ready_transactions, __memory_pool_max_transactions_capacity );

            }

        } // else printf( "Already in memory pool" );

        __semaphore_ready[ _global_id ].release();

    }

}

__device__ void memory_pool::cuda::kernel_store_transaction_data( void* __transaction_data, void* __memory_pool, ::cuda::std::binary_semaphore* __memory_pool_sems,  ::cuda::std::binary_semaphore* __memory_pool_broudcast_sems, uint64_t* __ready_transactions, uint64_t* __memory_pool_max_transactions_capacity ) {

    for (
        uint64_t _ = 0; _ < *__memory_pool_max_transactions_capacity; _++
    ) {

        if ( __memory_pool_sems[ _ ].try_acquire() ) {

            printf("Store in memory pool %d\n", _);

            memcpy(
                __memory_pool + _ * TRANSACTION_PROPAGATION_LENGTH,
                __transaction_data,
                TRANSACTION_PROPAGATION_LENGTH
            );

            __memory_pool_broudcast_sems[ _ ].release();

            atomicAdd( ( unsigned long long* ) __ready_transactions, 1 );
  
            break;

        }

    }

}

__device__ bool memory_pool::cuda::kernel_memory_pool_transaction_check( void* __transaction_data, void* __memory_pool, uint64_t* __memory_pool_max_transactions_capacity ) {

    bool _rtr;

    for (
        uint64_t _ = 0; _ < *__memory_pool_max_transactions_capacity; _++
    ) {

        _rtr = 1;

        for ( int __ = 0; __ < TRANSACTION_PROPAGATION_LENGTH; __++ )

            if ( *( unsigned char* ) ( __memory_pool + _ * TRANSACTION_PROPAGATION_LENGTH + __ ) != *( unsigned char* ) ( __transaction_data + __ ) ) { _rtr = 0; break; }

        if ( _rtr ) return _rtr;

    }

    return 0;

}

__device__ unsigned char memory_pool::cuda::kernel_get_public_key_type( void* __public_key, unsigned char* __block_division, uint32_t* __size_span_public_key ) {

    uint32_t* _pk_value =   
        ( uint32_t* ) __public_key;

    unsigned char _type = 0;

    for (; _type < *__block_division - 1; _type++ )

        if ( *_pk_value < ( _type + 1 ) * *__size_span_public_key ) break;

    return _type;

}




