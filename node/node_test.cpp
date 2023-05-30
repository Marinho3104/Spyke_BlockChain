
#ifdef NODE_TEST

#include <iostream>
#include "block_part.h"
#include "transaction.h"
#include "utils_functions.h"

#include "node.h"
#include <string.h>

int main() {

    types::Transaction _transaction;

    unsigned char _pk[] = 
        { 193, 23, 25, 60, 16, 143, 237, 26, 119, 37, 88, 134, 81, 163, 24, 107, 53, 191, 111, 189, 3, 116, 55, 37, 101, 178, 27, 48, 5, 214, 27, 218 };

    memcpy(
        _transaction.from,
        _pk,
        32
    );

    _transaction.from_balance = 1000;

    void* _block_part_data = 
        malloc( types::Block_Part::get_representation_length( 1 ) );

    memset(
        _block_part_data,
        0, 64
    );

    memset(
        _block_part_data + 64,
        0, 64
    );

    memset(
        _block_part_data + 64 + 63,
        1, 1
    );

    types::Block_Part* _block_part = ( types::Block_Part* ) _block_part_data;

    _block_part->transactions_count = 1;

    memcpy(
        &_block_part->transactions_data,
        &_transaction,
        168
    );

    utils::write_file_data(
        _block_part_data,
        types::Block_Part::get_representation_length( 1 ),
        ( char* ) utils::convert_bytes_hex( _block_part->previous_hash, 64 )
    );


    // node::Node* _node = 
    //     node::Node::by_file( 0 );

    // _node->run_interface();

    // free( _node );    

}

#endif