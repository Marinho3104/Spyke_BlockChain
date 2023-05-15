

#include "utils_functions.h"

// Compiler Libs 
#include <iostream>

void* utils::get_file_data( char* __path, size_t& __data_size ) {

    FILE* _file = 
        fopen(
            __path, 
            "rb"
        );

    fseek( _file, 0, SEEK_END );

    __data_size = ftell( _file );

    fseek( _file, 0, SEEK_SET );

    void* _file_data =   
        malloc( __data_size );

    fread(
        _file_data,
        __data_size,
        1, _file
    );

    fclose( _file );

    return _file_data;

}
