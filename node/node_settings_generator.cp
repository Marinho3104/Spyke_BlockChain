// Node Settings Generator
// From a file
// From manual

#include <iostream>
#include <string.h>

char* file_generation_path = 0;

char* default_output_file_path = "./node_settings";

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
    // const char* _delimiter = ( _version == 4 ) ? "." : ":";

    // char* _token = 
    //     strtok( _address, _delimiter );
    // int _token_conversion;

    // while( _token ) {

    //     _token_conversion = atoi( _token );

    //     fwrite(
    //         &_token_conversion,
    //         1, 1, _output_file
    //     );

    //     _token = strtok( 0, _delimiter );

    // }

    // std::cout << "Version: " << _version << std::endl;
    // std::cout << "Address: " << _address << std::endl;
    // std::cout << "Port: " << _port << std::endl;

}

// Manual generation
void manual_generation() {

    FILE* _output_file = 
        fopen( default_output_file_path, "wb" );

    if ( ! _output_file ) { std::cout << "Error trying to create the output file" << std::endl; exit( 1 ); }

    std::cout << "\n\tNode Connections Settings\n" << std::endl;

    int _have_server_connection_end_point;

    std::cout << "Have server connection end point [ 0 / 1 ]: ";
    scanf( "%d", &_have_server_connection_end_point );

    // If have server connection, get connection end point
    if ( _have_server_connection_end_point ) {

        // Get server connection end point
        get_write_end_point( _output_file );

    }

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

    }

}

// Start point
int main( int argc, char** argv ) {

    // Check flags
    check_flags( argc, argv );

    // File generation
    if ( file_generation_path ) file_generation();

    // Manual generation
    else manual_generation();

}
