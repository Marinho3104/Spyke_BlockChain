
#include "transaction_definitions.h" // Transaction Definitions
#include "blockchain_utils.h" // get_public_key_type()
#include "utils_functions.h" // get_file_data()
#include "connection.h" // Struct Connection
#include "wallet.h" // Struct Wallet
#include "ed25519.h" // Ed25519 

#include "propagation_protocol_definitions.h" // Propagation Protocol definitions
#include "propagation_protocol.h" // Struct Propagation Protocol

// Compiler libs

#include <iostream>
#include <string.h> // memcpy
#include <unistd.h>

/* Wallet Connections Information */

wallet::Wallet_Connections_Information::~Wallet_Connections_Information() {}

wallet::Wallet_Connections_Information::Wallet_Connections_Information() {}

wallet::Wallet_Connections_Information::Wallet_Connections_Information( void* __data ) : communication_connections( 0 ) {

    communication_connections_count = 
        *( ( uint16_t* ) __data );

    __data = __data + WALLET_WALLET_DEFINITIONS_WALLET_CONNECTIONS_INFORMATION_COMMUNICATION_CONNECTIONS_COUNT_LENGTH;

    if ( communication_connections_count ) 

        communication_connections =
            ( p2p::Connection* ) malloc( sizeof( p2p::Connection ) * communication_connections_count );

    for ( uint32_t _ = 0; _ < communication_connections_count; _++ ) {

        new ( communication_connections + _ ) 
            p2p::Connection( *( unsigned char* ) __data, __data + 1 );

        __data = __data + communication_connections[ _ ].get_end_point_version_length();

    }

}

void wallet::Wallet_Connections_Information::print() {

    std::cout << "\n\t\t --> Wallet Connections Information <-- \n" << std::endl;

    std::cout << "\tCommunication Nodes [ " << communication_connections_count << " ]\n" << std::endl;

    if ( communication_connections_count )

        for ( int _ = 0; _ < communication_connections_count; _++ ) 

            { std::cout << "\t\t[ " << _ << " ] "; communication_connections[ _ ].print(); std::cout << std::endl; }

    else 

        std::cout << "\t\t--- --- --- " << std::endl;

    std::cout << std::endl;

}


/* Wallet Information */

wallet::Wallet_Information::Wallet_Information( void* __private_key,  void* __public_key, unsigned char __pk_type ) : public_key_type( __pk_type ) {

    // Copy Private Key
    memcpy(
        private_key,
        __private_key,
        WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH
    );
    
    // Copy Public Key
    memcpy(
        public_key,
        __public_key,
        WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH
    );

}

wallet::Wallet_Information::Wallet_Information( void* __data ) {

    // Copy Private Key
    memcpy(
        private_key,
        __data,
        WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH
    );
    
    // Copy Public Key
    memcpy(
        public_key,
        __data + WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH,
        WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH
    );

    public_key_type = 
        blockchain::get_public_key_type( public_key );

}

void wallet::Wallet_Information::print() {

    std::cout << "\n\t\t --> Wallet Information <-- \n" << std::endl;

    std::cout << "\tPrivate Key: " << std::endl;
    std::cout << "\t\t{ " << ( int ) *private_key;

    for ( int _ = 1; _ < WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH; _++ )

        std::cout << ", " << ( int ) private_key[ _ ];

    std::cout << " }\n" << std::endl;

    std::cout << "\tPublic Key: " << std::endl;
    std::cout << "\t\t{ " << ( int ) *public_key;

    for ( int _ = 1; _ < WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH; _++ )

        std::cout << ", " << ( int ) public_key[ _ ];

    std::cout << " }\n" << std::endl;

    std::cout << "\tPublic Key Type: " << ( int ) public_key_type << "\n" << std::endl;

}


/* Wallet */

wallet::Wallet::~Wallet() {}

wallet::Wallet::Wallet() { memset( signature, 0, WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH ); }

void wallet::Wallet::sign( void* __data_to_sign, size_t __data_size ) {

    ed25519_sign(
        signature,
        ( unsigned char* ) __data_to_sign,
        __data_size,
        wallet_information.public_key,
        wallet_information.private_key
    );

}

bool wallet::Wallet::verify( void* __signature, void* __data, size_t __data_size ) {

    return 
        ed25519_verify(
            __signature ? ( unsigned char* ) __signature : signature,
            ( unsigned char* ) __data,
            __data_size,
            wallet_information.public_key
        );

}

void wallet::Wallet::print() {

    

}

void wallet::Wallet::creates_and_signs_transaction() {

    memcpy( transaction_ready.from, wallet_information.public_key, WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH );

    memset( transaction_ready.to, 1, 32 );

    transaction_ready.amount = 10;
    transaction_ready.fee = 10;
    transaction_ready.nonce = 10;

    sign(
        ( void* ) &transaction_ready + WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH,
        TRANSACTION_SIGN_DATA_LENGTH
    );

    memcpy(
        transaction_ready.signature,
        signature,
        WALLET_WALLET_DEFINITIONS_ED25519_SIGNATURE_LENGTH
    );

}

void wallet::Wallet::send_to_node( p2p::Packet* __packet ) {

    for ( int _ = 0; _ < wallet_connections_information.communication_connections_count; _++ ) {

        if ( ! wallet_connections_information.communication_connections[ _ ].connect() ) continue;

        wallet_connections_information.communication_connections[ _ ].send_packet( __packet );

        wallet_connections_information.communication_connections[ _ ].disconnect();

        break;

    }



    std::cout << "out" << std::endl;

} 

void wallet::Wallet::run() {

    while( 1 ) {

        system( "clear" );

        wallet_information.print();
        wallet_connections_information.print();

        std::cout << "0: Create and sign a transaction" << std::endl;
        std::cout << "1: Send Transaction" << std::endl;

        std::cout << "\n --> ";
        std::string _; std::cin >> _;

        switch ( *_.c_str() )
        {
        case '0': creates_and_signs_transaction(); break;
        case '1': 

            {

                p2p::Propagation_Protocol* _propagation_protocol = 
                    ( p2p::Propagation_Protocol* ) malloc( sizeof( p2p::Propagation_Protocol ) );

                new ( _propagation_protocol ) p2p::Propagation_Protocol(
                    P2P_PROTOCOLS_PROPAGATION_PROTOCOL_DEFINITIONS_PROPAGATION_TYPE_TRANSACTION,
                    TRANSACTION_PROPAGATION_LENGTH,
                    &transaction_ready
                );

                send_to_node( _propagation_protocol->get_packet() );

            }

        default: break;
        }

    }

}


wallet::Wallet* wallet::Wallet::by_file( char* __settings_path ) {

    wallet::Wallet* _wallet = 
        ( wallet::Wallet* ) malloc( sizeof( wallet::Wallet ) );

    // Initialize other variables
    *_wallet = 
        wallet::Wallet();

    size_t _data_size = 0;
    void* _settings_data = 
        utils::get_file_data(
            __settings_path ? __settings_path : ( char* ) WALLET_WALLET_DEFINITIONS_DEFAULT_SETTINGS_PATH,
            _data_size
        );

    // Set Key Pair and type
    _wallet->wallet_information = 
        wallet::Wallet_Information(
            _settings_data
        );

    // Set Node Communication
    _wallet->wallet_connections_information = 
        wallet::Wallet_Connections_Information(
            _settings_data + WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH + WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH 
        );


    // free( _key_pair_data );

    return _wallet;

}
