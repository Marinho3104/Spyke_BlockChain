

#ifndef UTILS_UTILS_FUNCTIONS_H
#define UTILS_UTILS_FUNCTIONS_H

// Compiler libs

#include <cstddef> // size_t
#include <cstdint> // uint

namespace utils {

    // Hexadecimal Characters
    constexpr char hexadecimal_characters[ 16 ] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    // Converts a byte array into a hexadecimal byte array
    // Returns malloc() value with the hexadecimal
    void* convert_bytes_hex( void*, unsigned long long );

    // Converts a hexadecimal array into a byte array
    // Returns malloc() value with the byte array
    void* convert_hex_bytes( void*, unsigned long long, bool );

    // Returns all data inside a file, in given path
    void* get_file_data( char*, size_t& );

    // Write into file given data
    void write_file_data( void*, uint64_t, char* );

}

#endif