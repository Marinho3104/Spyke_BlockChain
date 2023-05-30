

#include "kernel_transaction_verification.cuh" // Transaction verification variables
#include "utils_functions_cuda.cuh"
#include "kernel_block_part_verification.cuh"
#include "kernel_communication.cuh" // Kernel communication functions
#include "transaction_definitions.h" // Transaction definitions
#include "memory_pool_initialization.cuh"
#include "utils_functions.h"
#include "block_part.h"
#include "connection.h" // Struct Connection
#include "propagation_protocol_definitions.h"
#include "propagation_protocol.h" // Struct Propagation Protocol
#include "packet.h" // Struct Packet

// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>

void memory_pool::cuda::handle_block_part( void* __block_part_data, uint64_t __block_part_data_size ) {

    char* _file_path = 
        ( char* ) malloc( sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 129 );

    _file_path[ sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 128 ] = 0; 

    memcpy(
        _file_path,
        MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY,
        sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1
    );

    void* _hash_hex = 
        utils::convert_bytes_hex( __block_part_data, 64 );

    memcpy(
        _file_path + sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1,
        _hash_hex,
        128
    ); free( _hash_hex );

    // Checks if there is already the file with given block part hash data
    if ( ! access( _file_path, F_OK ) ) { std::cout << "Block part already in file" << std::endl; return; }

    printf("Block part received in CPU cuda\n");

    for (
        int _ = 0;
        _ < MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS;
        _++
    ) {

        // Thread was available
        if ( block_part_verification_thread_ready[ _ ].try_acquire() ) {

            cudaMallocManaged( block_part_verification_data + _, __block_part_data_size + 8 ); utils::cuda::check_cuda_error();
    
            memcpy(
                block_part_verification_data[ _ ],
                &__block_part_data_size,
                8
            );

            memcpy(
                block_part_verification_data[ _ ] + 8,
                __block_part_data,
                __block_part_data_size
            );

            block_part_verification_semaphores[ _ ].release();

        }
    
    }

}

void memory_pool::cuda::handle_transaction( void* __transaction_data ) {

    for (
        int _ = 0;
        _ < MEMORY_POOL_KERNEL_TRANSACTION_VERIFICATION_BLOCKS_TIMES_BLOCK_THREADS;
        _++
    ) {

        // Thread was available
        if ( transaction_verification_thread_ready[ _ ].try_acquire() ) {

            // Copies the transaction data into device memory
            memcpy(
                transaction_verification_data + _ * (TRANSACTION_PROPAGATION_LENGTH),
                __transaction_data,
                TRANSACTION_PROPAGATION_LENGTH
            );

            transaction_verification_semaphores[ _ ].release(); break;

        }

    }

}

void memory_pool::cuda::transactions_to_broudcast( p2p::Connection** __connections, uint16_t __connections_count ) {

    p2p::Propagation_Protocol* _propagation_protocol = 
        ( p2p::Propagation_Protocol* ) malloc( sizeof( p2p::Propagation_Protocol ) );

    p2p::Packet* _packet;

    for (
        int _ = 0;
        _ < *memory_pool_transaction_capacity;
        _++
    ) {
        
        // Thread was available
        if ( memory_pool_data_broudcast_sems[ _ ].try_acquire() ) {
            
            new ( _propagation_protocol ) 
                p2p::Propagation_Protocol(
                    P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_TRANSACTION,
                    TRANSACTION_PROPAGATION_LENGTH,
                    memory_pool + _ * (TRANSACTION_PROPAGATION_LENGTH)
                );

            _packet = 
                _propagation_protocol->get_packet();

            for ( uint16_t _ = 0; _ < __connections_count; _++ )

                if ( __connections[ _ ]  ) __connections[ _ ]->send_packet( _packet );

        }

    }

}

void* memory_pool::cuda::get_transaction_data_from_memory_pool( uint32_t* __transactions_count ) {

    uint64_t _ready_transactions = *ready_transactions_count;

    *__transactions_count = _ready_transactions;

    if ( ! *__transactions_count ) return 0;

    void* _data = 
        malloc( _ready_transactions * (TRANSACTION_LENGTH) ), *_rtr = _data;

    std::cout << "Malloc pointer: " << _data << std::endl;
    std::cout << "Ready transactions: " << *ready_transactions_count << std::endl;

    for ( uint64_t _ = 0; _ready_transactions && _ < *memory_pool_transaction_capacity; _++ )

        // Have data
        if ( ! memory_pool_sems[ _ ].try_acquire() ) {

            memset(
                _data,
                0,
                TRANSACTION_LENGTH
            );

            memcpy(
                _data,
                memory_pool + _ * (TRANSACTION_PROPAGATION_LENGTH),
                TRANSACTION_PROPAGATION_LENGTH
            );
            
            _data = _data + TRANSACTION_LENGTH;

            memory_pool_sems[ _ ].release();

            (*ready_transactions_count)--; 
            
            _ready_transactions--;

        }

        // No data
        else memory_pool_sems[ _ ].release();

    return _rtr;

}

void memory_pool::cuda::store_broudcast_block_part_data( p2p::Connection** __connections, uint16_t __connections_count ) {

    p2p::Propagation_Protocol* _propagation_protocol = 
        ( p2p::Propagation_Protocol* ) malloc( sizeof( p2p::Propagation_Protocol ) );

    p2p::Packet* _packet = 0;

    char* _file_path = 
        ( char* ) malloc( sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 129 );

    void* _hash_hex;

    memcpy(
        _file_path,
        MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY,
        sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1
    );

    _file_path[ sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 128 ] = 0; 

    for (
        uint32_t _ = 0;
        _ < MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_BLOCK_PART_STORE_FOR_FILE_MEMORY;
        _++
    ) {

        if ( ! block_part_memory_pool_semaphores[ _ ].try_acquire() ) {

            new ( _propagation_protocol ) 
                p2p::Propagation_Protocol(
                    P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_BLOCK_PART,
                    *( uint64_t* ) block_part_memory_pool[ _ ],
                    block_part_memory_pool[ _ ] + 8
                );
            
            _packet = _propagation_protocol->get_packet();

            for ( uint16_t _ = 0; _ < __connections_count; _++ ) if ( __connections[ _ ]  ) __connections[ _ ]->send_packet( _packet );

            _hash_hex = 
                utils::convert_bytes_hex( block_part_memory_pool[ _ ] + 8, 64 );

            memcpy(
                _file_path + sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1,
                _hash_hex,
                128
            ); free( _hash_hex );
  
            utils::write_file_data(
                block_part_memory_pool[ _ ] + 8,
                *( uint64_t* ) block_part_memory_pool[ _ ],
                _file_path
            );

            block_part_memory_pool_semaphores[ _ ].release();

        }

        else block_part_memory_pool_semaphores[ _ ].release();

    }

    free( _file_path );

    _propagation_protocol->~Propagation_Protocol(); free( _propagation_protocol );

    free( _packet );

}


