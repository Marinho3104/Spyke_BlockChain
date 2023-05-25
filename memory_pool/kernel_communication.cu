

#include "kernel_transaction_verification.cuh" // Transaction verification variables
#include "kernel_communication.cuh" // Kernel communication functions
#include "transaction_definitions.h" // Transaction definitions
#include "memory_pool_initialization.cuh"
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

void memory_pool::cuda::handle_block_part( void* __block_part_data ) {

    printf("Block part received in CPU cuda\n");

    types::Block_Part* _block_part = 
        ( types::Block_Part* ) __block_part_data;

    void* _data = 
        _block_part->get_transaction_data() + WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH;

    std::cout << "Transaction count: " << _block_part->transactions_count  << std::endl; 

    if ( ! _block_part->transactions_count ) return;

    std::cout << "Transaction amount: " << *( uint64_t* ) _data  << std::endl; 
    std::cout << "Transaction fee: " << *( uint64_t* ) ( _data + 8 )  << std::endl; 
    std::cout << "Transaction nonce: " << *( uint64_t* ) ( _data + 8 + 8 ) << std::endl; 

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

            (*ready_transactions_count)--; _ready_transactions--;

        }

        // No data
        else memory_pool_sems[ _ ].release();

    return _rtr;

}

