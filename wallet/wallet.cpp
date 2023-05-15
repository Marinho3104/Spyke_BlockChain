
#include "blockchain_utils.h" // get_public_key_type()
#include "utils_functions.h" // get_file_data()
#include "wallet.h" // Struct Wallet
#include "ed25519.h" // Ed25519 


// Compiler libs

#include <iostream>
#include <string.h> // memcpy


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



void wallet::Wallet::run() {

    wallet_information.print();

    std::string _; std::cin >> _;

}

bool wallet::Wallet::save( char* __path ) {

    FILE* _save_file = 
        fopen( 
            __path ? __path : WALLET_WALLET_DEFINITIONS_DEFAULT_SAVE_PATH,
            "wb"    
        );

    if ( ! _save_file ) return 0;

    fwrite(
        this,
        sizeof( wallet::Wallet ),
        1,
        _save_file
    );

    fclose( _save_file );

    return 1;

}


wallet::Wallet* wallet::Wallet::get_new_wallet( unsigned char __target_type ) {

    // Key Pairs and Seed
    unsigned char 
        _private_key[ WALLET_WALLET_DEFINITIONS_ED25519_PRIVATE_KEY_LENGTH ], 
            _public_key[ WALLET_WALLET_DEFINITIONS_ED25519_PUBLIC_KEY_LENGTH ],
                _seed[ 32 ],
                    _pk_type;

    do {

        // Set seed
        ed25519_create_seed( _seed );

        // Create key pairs with seed
        ed25519_create_keypair(
            _public_key,
            _private_key,
            _seed
        );

        _pk_type = blockchain::get_public_key_type( _public_key );

    } while( _pk_type != __target_type );

    wallet::Wallet* _wallet = 
        ( wallet::Wallet* ) malloc( sizeof( wallet::Wallet ) );

    // Initialize other variables
    *_wallet = 
        wallet::Wallet();

    // Set Key Pair and type
    _wallet->wallet_information = 
        wallet::Wallet_Information(
            _private_key,
            _public_key,
            _pk_type
        );

    return _wallet;

}

wallet::Wallet* wallet::Wallet::by_file( char* __path ) {

    size_t _data_size = 0;

    void* _data = 
        utils::get_file_data(
            __path ? __path : ( char* ) WALLET_WALLET_DEFINITIONS_DEFAULT_SAVE_PATH,
            _data_size
        );

    wallet::Wallet* _wallet = 
        ( wallet::Wallet* ) malloc( sizeof( wallet::Wallet ) );

    memcpy(
        _wallet,
        _data,
        _data_size
    );

    return _wallet;

}
