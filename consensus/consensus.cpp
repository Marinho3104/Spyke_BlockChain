
#include "propagation_protocol_definitions.h"
#include "propagation_protocol.h"
#include "utils_functions.h"

#include "transaction_definitions.h"
#include "consensus.h"
#include "SHA512.h"
#include "packet.h"
#include "wallet.h"
#include "miner.h"
#include "node.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

namespace consensus {

    unsigned char current_block_hash[ CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT + 1 ] = { 0 };

}

bool consensus::is_miners_turn( void* __public_key ){

    unsigned char _test_miner_pk[] = 
        { 9, 120, 221, 84, 253, 121, 43, 250, 126, 252, 29, 33, 78, 124, 213, 231, 187, 80, 74, 55, 26, 176, 50, 207, 252, 122, 232, 119, 27, 50, 188, 10 };

    if (
        ! memcmp(
            _test_miner_pk,
            __public_key,
            WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH
        )
    ) return 1;

    return 0;

}

void* consensus::get_block_part( wallet::Wallet* __miner_wallet, uint32_t* __transactions_count ) {

    void* _transaction_data =   
        memory_pool::cuda::get_transaction_data_from_memory_pool( __transactions_count );

    void* _block_part_data = 
        malloc( types::Block_Part::get_representation_length( *__transactions_count ) );

    memset( _block_part_data, 0, types::Block_Part::get_representation_length( *__transactions_count ) );

    memcpy(
        _block_part_data,
        current_block_hash,
        CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT
    );

    SHA512 sha512;

    memcpy(
        current_block_hash,
        sha512.hash( ( unsigned char* ) utils::convert_bytes_hex(current_block_hash, 64) ),
        CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT
    );

    memcpy(
        _block_part_data + CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT,
        current_block_hash,
        CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT
    );

    memcpy(
        _block_part_data + CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT + TYPES_BLOCK_PART_PREVIOUS_HASH_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH,
        __miner_wallet->wallet_information.public_key,
        WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH
    );

    memcpy(
        _block_part_data + CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT + TYPES_BLOCK_PART_PREVIOUS_HASH_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH,
        __transactions_count,
        TYPES_BLOCK_PART_TRANSACTION_COUNT_LENGTH
    );

    if ( *__transactions_count ) 

        memcpy(
            _block_part_data + CONSENSUS_CONSENSUS_CURRENT_BLOCK_HASH_LENGHT + TYPES_BLOCK_PART_PREVIOUS_HASH_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH + TYPES_BLOCK_PART_TRANSACTION_COUNT_LENGTH,
            _transaction_data,
            *__transactions_count * (TRANSACTION_LENGTH)
        );

    return _block_part_data;

}

void consensus::miner_selected( miner::Miner* __miner ) {

    std::cout << "My turn to shine :)" << std::endl;

    uint32_t _transactions_count = 0;

    void* _block_part_generated_data = 
        get_block_part( __miner->wallet, &_transactions_count );

    // Save in file
    char* _file_path = 
        ( char* ) malloc( sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 129 );

    void* _hash_hex;

    memcpy(
        _file_path,
        MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY,
        sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1
    );

    _file_path[ sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) + 128 ] = 0; 

    _hash_hex = 
        utils::convert_bytes_hex( _block_part_generated_data, 64 );

    memcpy(
        _file_path + sizeof( MEMORY_POOL_KERNEL_BLOCK_PART_VERIFICATION_STORE_DIRECTORY ) - 1,
        _hash_hex,
        128
    ); free( _hash_hex );

    utils::write_file_data(
        _block_part_generated_data,
        types::Block_Part::get_representation_length( _transactions_count ),
        _file_path
    ); free( _file_path );

    p2p::Propagation_Protocol* _propagation_protocol = 
        ( p2p::Propagation_Protocol* ) malloc( sizeof( p2p::Propagation_Protocol ) );

    new ( _propagation_protocol ) 
        p2p::Propagation_Protocol(
            P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_BLOCK_PART,
            types::Block_Part::get_representation_length( _transactions_count ),
            _block_part_generated_data
        );

    p2p::Packet* _packet =  
        _propagation_protocol->get_packet();

    // for ( int _ = 0; _ < __miner->node->node_information.max_ordinary_connections; _++ ) 

    //     if ( __miner->node->ordinary_connections[ _ ] ) __miner->node->ordinary_connections[ _ ]->send_packet( _packet );

    if ( __miner->node->ordinary_connections[ 0 ] ) __miner->node->ordinary_connections[ 0 ]->send_packet( _packet );


}

void consensus::consensus_main_process( miner::Miner* __miner ) {

    sleep( 10 );

    std::cout << "starting " << std::endl;

    while ( 1 ) {

        if ( is_miners_turn( __miner->wallet->wallet_information.public_key ) ) miner_selected( __miner );

        else std::cout << "Not my turn" << std::endl;

        // sleep( 5 );

    }

}
