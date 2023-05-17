// Node Settings Generator
// From a file
// From manual

#include <iostream>
#include <string.h>

char* file_generation_path = 0;

// Manual generation
void manual_generation() {

    

}

// Generation with file
void file_generation() {

    std::cout << file_generation_path << std::endl;

}

// Print available flags
void print_help_information() {

    std::cout << "Available flags: \n" << std::endl;

    std::cout << "  -f:\tSpecify a file for settings generation" << std::endl; 

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

            file_generation_path = __argv[ _ + 1 ];

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
