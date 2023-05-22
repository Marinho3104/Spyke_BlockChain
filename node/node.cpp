
#include "socket_functions.h" // Socket functions related
#include "node_definitions.h" // Node Definitions
#include "utils_functions.h" // get_file_data()
#include "protocols_ids.h" // Protocols Ids
#include "packet.h" // Struct packet
#include "node.h" // Node headers

// Compiler libs
#include <string.h> // memcpy
#include <cstddef> // size_t
#include <thread> // thread
#include <iostream>
#include <unistd.h>

// P2p communication protocols
#include "propagation_protocol.h"
#include "disconnect_protocol.h"


// Node Information

node::Node_Connections_Information::Node_Connections_Information() 
    : server_connection( 0 ), initial_connections( 0 ), initial_connections_count( 0 ) {}

node::Node_Connections_Information::Node_Connections_Information( void* __data, size_t& __size ) : initial_connections( 0 ) {

    void* _backup = __data;

    if ( *( bool* ) __data ) {

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
    
    if ( server_connection ) server_connection->print(); else std::cout << "--- --- ---"; std::cout << "\n" << std::endl;

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

node::Node::Node() : node_is_running( 0 ), server_is_up( 0 ), ordinary_connections_count( 0 ), stable_connections_count( 0 ) {}

void node::Node::initialize_temporary_variable() {

    ordinary_connections = ( p2p::Connection** ) malloc( sizeof( p2p::Connection* ) * node_information.max_ordinary_connections ); 
    sem_init( &ordinary_connections_sem, 0, 1 );
    for ( int _ = 0; _ < node_information.max_ordinary_connections; _++ ) ordinary_connections[ _ ] = 0;

    stable_connections = ( p2p::Connection** ) malloc( sizeof( p2p::Connection* ) * node_information.max_stable_connections ); 
    sem_init( &stable_connections_sem, 0, 1 );
    for ( int _ = 0; _ < node_information.max_stable_connections; _++ ) stable_connections[ _ ] = 0;

    sem_init( &open_descriptor_files_sem, 0, 1 ); FD_ZERO( &open_descriptor_files );

    memory_pool = 
        memory_pool::Memory_Pool( node_information.memory_pool_size );

}

bool node::Node::add_ordinary_connection( p2p::Connection* __connection ) {

    sem_wait( &ordinary_connections_sem );

    bool _rtr = 0;

    if ( ordinary_connections_count != node_information.max_ordinary_connections ) {

        for ( int _ = 0; _ < node_information.max_ordinary_connections; _++ )

            if ( ! ordinary_connections[ _ ] ) { ordinary_connections[ _ ] = __connection; add_new_file_descriptor( ordinary_connections[ _ ]->socket_descriptor ); break; }

        _rtr = 1; ordinary_connections_count++;

    }
    
    sem_post( &ordinary_connections_sem );

    return _rtr;

}

bool node::Node::remove_ordinary_connection( p2p::Connection* __connection ) {

    sem_wait( &ordinary_connections_sem );
    
    bool _rtr = 0;

    for ( int _ = 0; _ < node_information.max_ordinary_connections; _++ )

        if ( ordinary_connections[ _ ] ) { ordinary_connections[ _ ] = 0; _rtr = 1; ordinary_connections_count--; break;  } 

    sem_post( &ordinary_connections_sem );

    return _rtr;

}

bool node::Node::add_stable_connection( p2p::Connection* __connection ) {

    sem_wait( &stable_connections_sem );

    bool _rtr = 0;

    if ( stable_connections_count != node_information.max_stable_connections ) {

        for ( int _ = 0; _ < node_information.max_stable_connections; _++ )

            if ( ! stable_connections[ _ ] ) { stable_connections[ _ ] = __connection; add_new_file_descriptor( ordinary_connections[ _ ]->socket_descriptor ); break; }

        _rtr = 1; stable_connections_count++;

    }

    sem_post( &stable_connections_sem );

    return _rtr;

}

bool node::Node::remove_stable_connection( p2p::Connection* __connection ) {

    sem_wait( &stable_connections_sem );

    bool _rtr = 0;

    for ( int _ = 0; _ < node_information.max_stable_connections; _++ )

        if ( stable_connections[ _ ] ) { stable_connections[ _ ] = 0; _rtr = 1; stable_connections_count--; break;  } 

    sem_post( &stable_connections_sem );

    return _rtr;

}

void node::Node::add_new_file_descriptor( int __file_descriptor ) {

    sem_wait( &open_descriptor_files_sem ); FD_SET( __file_descriptor, &open_descriptor_files ); sem_post( &open_descriptor_files_sem );

}

void node::Node::remove_file_descriptor( int __file_descriptor ) {

    sem_wait( &open_descriptor_files_sem ); FD_CLR( __file_descriptor, &open_descriptor_files ); sem_post( &open_descriptor_files_sem );

}

void node::Node::print() {

    std::cout << "\t\t\t--> Node Information <---\n" << std::endl; 

    connections_information.print(); node_information.print();

    std::cout << "\t\t--> Node Temporary Information <-- \n" << std::endl;

    std::cout << "\tNode is running: " << node_is_running << std::endl;
    std::cout << "\tServer is up: " << server_is_up << std::endl;

    std::cout << "\n\tOrdinary Connections [ " << ordinary_connections_count << " ]:\n" << std::endl;

    for ( int _ = 0; _ < node_information.max_ordinary_connections; _++ )

        if ( ordinary_connections[ _ ] ) 
            { std::cout << "\t\t[ " << _ << " ] "; ordinary_connections[ _ ]->print(); std::cout << std::endl;  }

    std::cout << "\n\tStable Connections [ " << stable_connections_count << " ]:" << std::endl;

    for ( int _ = 0; _< node_information.max_stable_connections; _++ )

        if ( stable_connections[ _ ] ) 
            { std::cout << "\t\t[ " << _ << " ] "; stable_connections[ _ ]->print(); std::cout << std::endl;  }

    std::cout << "\n" << std::endl;

}

void node::Node::run_interface() {

    while( 1 ) {

        system( "clear" );

        print();

        std::cout << "\t\t--> Interface <--\n" << std::endl;

        if ( ! node_is_running ) std::cout << "\t0: Run Node" << std::endl;

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

    if ( connections_information.server_connection ) {
     
        // Set the server up
        server_is_up = 
            connections_information.server_connection->server( node_information.max_server_open_connections );
        if ( ! server_is_up ) throw "Was not possible to set the server up";
        add_new_file_descriptor( connections_information.server_connection->socket_descriptor );

    }

    for ( int _ = 0; _ < connections_information.initial_connections_count; _++ ) {

        if ( ! connections_information.initial_connections[ _ ].connect() ) continue;

        // Add to ordinary connections array
        add_ordinary_connection( connections_information.initial_connections + _ );

        sleep( 10 );

        std::cout << "Sending" << std::endl;

        ( connections_information.initial_connections + _ )->send_packet( p2p::Disconnect_Protocol::get_packet() );

    }


    // Set node running
    node_is_running = 1;

    // Launch in a separate thread a thread to monitor file descriptors changes
    std::thread(
        &node::Node::monitor_open_file_descriptors,
        this
    ).detach();

}

void node::Node::monitor_open_file_descriptors() {

    while ( 1 ) {

        fd_set _copy = open_descriptor_files;

        if (
            select(
                FD_SETSIZE,
                &_copy,
                NULL,
                NULL,
                NULL
            ) < 0 
        ) break;

        // Server file descriptor 
        // Is a new connection request
        if ( FD_ISSET( connections_information.server_connection->socket_descriptor, &_copy ) ) accept_new_connection_request();

        // Check if receive data from ordinary_connections
        for ( unsigned short _ = 0; _ < node_information.max_ordinary_connections; _++ )

            if ( 
                *( ordinary_connections + _ ) && 
                FD_ISSET( ( *( ordinary_connections + _ ) )->socket_descriptor, &_copy )  
            ) p2p_communication( *( ordinary_connections + _ ), NODE_NODE_INFORMATION_ORDINARY_CONNECTION );

        // Check if receive data from stable_connections
        for ( unsigned short _ = 0; _ < node_information.max_stable_connections; _++ )

            if ( 
                *( stable_connections + _ ) && 
                FD_ISSET( ( *( stable_connections + _ ) )->socket_descriptor, &_copy )  
            ) p2p_communication( *( stable_connections + _ ), NODE_NODE_INFORMATION_STABLE_CONNECTION );

    }

}

void node::Node::accept_new_connection_request() {

    // Accepted and get connection
    p2p::Connection* _accepted_connection = 
        p2p::accept_new_connection_request( connections_information.server_connection->socket_descriptor );

    if ( ! add_ordinary_connection( _accepted_connection ) ) _accepted_connection->disconnect();

    add_new_file_descriptor( _accepted_connection->socket_descriptor );

}

void node::Node::p2p_communication( p2p::Connection* __connection, unsigned char __connection_type ) {

    p2p::Packet* _packet_received = __connection->get_packet();

    std::cout << "P2P communication protocol id: " << ( int ) _packet_received->protocol_id << std::endl;

    // Protocol handle by both stable and ordinary connections
    switch ( _packet_received->protocol_id )
    {
    case P2P_PROTOCOLS_PROTOCOLS_IDS_DISCONNECT_PROTOCOL: __connection->disconnect(); break;
    case P2P_PROTOCOLS_PROTOCOLS_IDS_PROPAGATION_PROTOCOL: 
    
        {

            p2p::Propagation_Protocol* _propagation_protocol = 
                p2p::Propagation_Protocol::get_propagation_protocol( _packet_received );

            _propagation_protocol->handle();

            break;
        
        }

    default: break;
    }

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

    _node->initialize_temporary_variable();

    // free( _data );

    return _node;

}
