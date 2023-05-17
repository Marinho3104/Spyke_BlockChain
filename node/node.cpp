
#include "node_definitions.h" // Node Definitions
#include "utils_functions.h" // get_file_data()
#include "node.h" // Node headers

// Compiler libs
#include <string.h> // memcpy
#include <cstddef> // size_t
#include <iostream>

// Node Information

node::Node_Connections_Information::Node_Connections_Information() 
    : server_connection( 0 ), initial_connections( 0 ), initial_connections_count( 0 ) {}

node::Node_Connections_Information::Node_Connections_Information( void* __data ) : initial_connections( 0 ) {

    if ( *( ( bool* ) __data + 1 ) ) {

        server_connection = 
            ( p2p::Connection* ) malloc( sizeof( p2p::Connection ) );

        new ( server_connection ) 
            p2p::Connection( *( unsigned char* ) ( __data + 1 ), __data + 2 );

        __data = __data + server_connection->get_end_point_version_length();

    } __data = __data + 1;

    initial_connections_count = 
        *( ( uint32_t* ) __data );

    __data = __data + NODE_NODE_CONNECTIONS_INFORMATION_INITIAL_CONNECTIONS_COUNT_LENGTH;

    if ( initial_connections_count ) 

        initial_connections =
            ( p2p::Connection* ) malloc( sizeof( p2p::Connection ) * initial_connections_count );

    for ( uint32_t _ = 0; _ < initial_connections_count; _++ ) {

        new ( initial_connections + _ ) 
            p2p::Connection( *( unsigned char* ) __data, __data + 1 );

        __data = __data + initial_connections[ _ ].get_end_point_version_length();

    }

}

// Node Information

node::Node_Information::Node_Information() 
    : max_server_open_connections( 0 ), max_ordinary_connections( 0 ), max_stable_connections( 0 ), memory_pool_size( 0 ) {}

node::Node_Information::Node_Information( void* __data ) {

    memcpy(
        this,
        __data,
        sizeof( Node_Information )
    );

}


// Node

node::Node::Node() {}


node::Node* node::Node::get_new_node() {

    node::Node* _node = 
        ( node::Node* ) malloc( sizeof( node::Node ) );

    // Default Node settings 
    *_node = 
        node::Node();

    // Default Node Information settings
    _node->node_information = 
        node::Node_Information();

    // Default Node Connections Information settings
    _node->connections_information = 
        node::Node_Connections_Information();

    return _node;

}

node::Node* node::Node::by_file( char* __path ) {

    size_t _data_length = 0;

    void* _data = 
        utils::get_file_data(
            __path ? __path : ( char* ) NODE_NODE_DEFINITIONS_DEFAULT_SAVE_PATH,
            _data_length
        ), *_data_copy = _data;

    node::Node* _node = 
        ( node::Node* ) malloc( sizeof( node::Node ) );

    *_node =
        node::Node();

    // Set node connections info
    _node->connections_information =
        node::Node_Connections_Information( _data );

    // Set node info
    _node->node_information = 
        node::Node_Information( _data ); 

    free( _data_copy );

    return _node;

}
