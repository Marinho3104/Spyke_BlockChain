
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

node::Node_Connections_Information::Node_Connections_Information( void* __data, size_t& __size ) : initial_connections( 0 ) {

    void* _backup = __data;

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

    __size = ( char* ) __data - ( char* ) _backup;

}

void node::Node_Connections_Information::print() {

    std::cout << "\t\t--> Node Connection Information <--\n" << std::endl;

    std::cout << "\tServer Connection: \n\n\t\t"; 
    
    server_connection->print(); std::cout << "\n" << std::endl;

    std::cout << "\tInitial Connections [ " << initial_connections_count << " ]:\n" << std::endl;

    for ( int _ = 0; _ < initial_connections_count; _++ ) {

        std::cout << "\t\t[ " << _ << " ]: "; initial_connections[ _ ].print(); std::cout << "\n" << std::endl;

    }

    std::cout << std::endl;

}

// Node Information

node::Node_Information::Node_Information() 
    : max_server_open_connections( 0 ), max_ordinary_connections( 0 ), max_stable_connections( 0 ), memory_pool_size( 0 ) {}

node::Node_Information::Node_Information( void* __data ) {

    max_server_open_connections = 
        *( uint16_t* ) ( __data );   
    __data = __data + NODE_NODE_INFORMATION_MAX_SERVER_OPEN_CONNECTIONS_LENGTH;

    max_ordinary_connections = 
        *( uint16_t* ) ( __data );
    __data = __data + NODE_NODE_INFORMATION_MAX_ORDINARY_CONNECTIONS_LENGTH;

    max_stable_connections = 
        *( uint16_t* ) ( __data );
    __data = __data + NODE_NODE_INFORMATION_MAX_STABLE_CONNECTIONS_LENGTH;

    memory_pool_size = 
        *( uint64_t* ) ( __data );

}

void node::Node_Information::print() {

    std::cout << "\t\t--> Node Information <--\n" << std::endl;

    std::cout << "\tMax server open connections: " << max_server_open_connections << std::endl;
    std::cout << "\tMax ordinary connections: " << max_ordinary_connections << std::endl;
    std::cout << "\tMax stable connections: " << max_stable_connections << std::endl;
    std::cout << "\tMemory pool size: " << memory_pool_size << std::endl;

    std::cout << "\n" << std::endl;

}

// Node

node::Node::Node() : node_is_running( 0 ), server_is_up( 0 ) {}

void node::Node::print() {

    std::cout << "\t\t\t--> Node Information <---\n" << std::endl; 

    connections_information.print(); node_information.print();

    std::cout << "\t\t--> Node Temporary Information <-- \n" << std::endl;

    std::cout << "\tNode is running: " << node_is_running << std::endl;
    std::cout << "\tServer is up: " << server_is_up << std::endl;

    std::cout << "\n" << std::endl;

}

void node::Node::run_interface() {

    while( 1 ) {

        system( "clear" );

        print();

        std::cout << "\t\t--> Interface <--\n" << std::endl;

        std::cout << "\t0: Run Node" << std::endl;

        std::cout << "\n\t --> ";

        std::string _; std::cin >> _; 

        switch ( *_.c_str() )
        {
        case '0': run(); break;
        default: break;
        }

    }

} 

void node::Node::run() {

    // Set the server up
    server_is_up = 
        connections_information.server_connection->server( node_information.max_server_open_connections );
    if ( ! server_is_up ) throw "Was not possible to set the server up";

    

    // Set node running
    node_is_running = 1;

}


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
        );

    size_t _length;

    node::Node* _node = 
        ( node::Node* ) malloc( sizeof( node::Node ) );

    *_node =
        node::Node();

    // Set node connections info
    _node->connections_information =
        node::Node_Connections_Information( _data, _length );

    // Set node info
    _node->node_information = 
        node::Node_Information( _data + _length ); 

    // free( _data );

    return _node;

}
