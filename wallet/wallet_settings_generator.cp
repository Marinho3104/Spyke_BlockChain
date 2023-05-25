
#include <iostream>
#include <string.h>
#include "blockchain_rules.h"

#include "ed25519.h" // Ed25519 
#include "blockchain_utils.h"

char* file_generation_path = 0;

char* default_output_file_path = "./wallet_settings";
char* key_pair_file_path = "./wallet_key_pair";
char* keys_pair_file_path = 0;
bool key_pair_pk_type_generation = 0;
unsigned char key_pairs_pk_type = 0;


// Get and write into a output file the end point information
void get_write_end_point( FILE* _output_file ) {

    // Get end point version
    int _version;

    std::cout << "End point version [ 4 / 6 ]: "; scanf( "%d", &_version );

    if ( _version != 4 && _version != 6 ) { std::cout << "End point version should only have 4 or 6" << std::endl; exit( 1 ); }

    // Get address
    char _address[ _version == 4 ? 16 : 40 ] = { 0 };

    std::cout << "End point address " << ( ( _version == 4 ) ? "( xxx.xxx.xxx.xxx in decimal ) " : "(xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx in hexadecimal ) " ) 
        << ": ";
    
    if ( _version == 4 ) scanf( "%15s", _address );

    else scanf( "%39s", _address );

    // Get port
    int _port;

    std::cout << "End point port: ";
    scanf( "%d", &_port );

    // Write version
    fwrite(
        &_version,
        1, 1,
        _output_file
    );

    // Write address
    const char* _delimiter = ( _version == 4 ) ? "." : ":";
    char* _current_token = strtok( _address, _delimiter );

    // For 4 bytes address IPv4
    if ( _version == 4 ) {

        // Address int form
        int _address_converted = 0;

        while( _current_token ) {

            _address_converted |= atoi( _current_token );

            _current_token = strtok( 0, _delimiter );

            if ( _current_token ) _address_converted <<= 8;

        }

        fwrite(
            &_address_converted,
            4, 1, _output_file
        );

    }

    // For 16 bytes address IPv6
    else { std::cout << "Ipv6 not implemented yet" << std::endl; exit( 1 ); }

    // Write port
    fwrite(
        &_port,
        2, 1,
        _output_file
    );

}

// Manual generation
void manual_generation() {

    FILE* _output_file = 
        fopen( default_output_file_path, "wb" );


    FILE* _key_pair_file = 
        fopen( key_pair_file_path, "wb" );

    if ( ! _output_file || ! _key_pair_file ) { std::cout << "Error trying to create the output file" << std::endl; exit( 1 ); }

    // Key Pairs and Seed
    unsigned char 
        _private_key[ 64 ], 
            _public_key[ 32 ],
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

        std::cout << ( int ) _pk_type << std::endl;
        std::cout << ( int ) key_pairs_pk_type << std::endl;

    } while( _pk_type != key_pairs_pk_type && key_pair_pk_type_generation );

    fwrite(
        _private_key, 64, 1,
        _output_file
    );

    fwrite(
        _public_key, 32, 1,
        _output_file
    );

    fwrite(
        _private_key, 64, 1,
        _key_pair_file
    );

    fwrite(
        _public_key, 32, 1,
        _key_pair_file
    );

    fclose( _key_pair_file );

    // Node to communication
    unsigned short _nodes_communication_count;

    std::cout << "Node communication count: "; scanf( "%hu", &_nodes_communication_count );

    fwrite(
        &_nodes_communication_count,
        2, 1, _output_file
    );


    for ( unsigned short _ = 0; _ < _nodes_communication_count; _++ ) {
        
        std::cout << "\n\tConnection " << _ + 1 << ":\n" << std::endl; get_write_end_point( _output_file ); 

    }

    fclose( _output_file );

}

// Generation with file
void file_generation() {

    std::cout << file_generation_path << std::endl;

}

// Print available flags
void print_help_information() {

    std::cout << "Available flags: \n" << std::endl;

    std::cout << "  -f:\tSpecify a file for settings generation" << std::endl; 
    std::cout << "  -o:\tSpecify the output file for settings generation ( When not specified, it uses the default one )" << std::endl;
    std::cout << "  -k:\tSpecify a file to use the keys from instead creating new ones" << std::endl;
    std::cout << "  -t:\tSpecify a type of public key in keys generation" << std::endl;

}

// Check flags
void check_flags( int __argc, char** __argv ) {

    // Check for flags
    for ( int _ = 0; _ < __argc; _++ ) {

        if (
            ! strcmp(
                __argv[ _ ],
                "-h"
            )
        ) { print_help_information(); exit( 1 ); }

        if (
            ! strcmp(
                __argv[ _ ],
                "-f"
            )
        ) {

            if ( _ == __argc - 1 ) { std::cout << "Flag: '-f' needs a the file path!" << std::endl; exit( 1 ); }

            file_generation_path = __argv[ ++_ ];

        }

        if (
            ! strcmp(
                __argv[ _ ],
                "-o"
            )
        ) {

            if ( _ == __argc - 1 ) { std::cout << "Flag: '-o' needs a the output file path!" << std::endl; exit( 1 ); }

            default_output_file_path = __argv[ ++_ ];

        }

        if (
            ! strcmp(
                __argv[ _ ],
                "-k"
            )
        ) {

            if ( _ == __argc - 1 ) { std::cout << "Flag: '-k' needs a the key pairs file path!" << std::endl; exit( 1 ); }

            keys_pair_file_path = __argv[ ++_ ];

        }

        if (
            ! strcmp(
                __argv[ _ ],
                "-t"
            )
        ) {

            key_pair_pk_type_generation = 1;

            key_pairs_pk_type = atoi( __argv[ ++_ ] );

        }

    }

}

// Start point
int main( int argc, char** argv ) {

    blockchain::update_blockchain_variables();

    // Check flags
    check_flags( argc, argv );

    // File generation
    if ( file_generation_path ) file_generation();

    // Manual generation
    else manual_generation();

}


