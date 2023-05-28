
#include <iostream>
#include "SHA512.h"
#include <string.h>

constexpr char hexadecimal_characters[ 16 ] = 
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void* convert_bytes_hex( void* __bytes, unsigned long long __size ) {

    void* _hexadecimal_value = 
        malloc( __size * 2 + 1 ), *_rtr = _hexadecimal_value;

    while( __size-- ) {

        *( ( char* ) _hexadecimal_value ) = hexadecimal_characters[ ( *( ( char* ) __bytes ) >> 4 ) & 0xf ];

        _hexadecimal_value = _hexadecimal_value + 1;

        *( ( char* ) _hexadecimal_value ) = hexadecimal_characters[ *( ( char* ) __bytes ) & 0xf ];

        __bytes = __bytes + 1; _hexadecimal_value = _hexadecimal_value + 1;

    }

    *(
        ( char* ) _hexadecimal_value
    ) = 0;

    return _rtr;

}

int main() {

    char _path[ 133 ] = { 0 };

    _path[ 0 ] = '.';
    _path[ 1 ] = '/';
    _path[ 2 ] = 'a';
    _path[ 3 ] = '/';

    SHA512 sha512;

    memcpy(
        _path + 4,
        convert_bytes_hex(
            sha512.hash( _path ), 64 
        ), 128
    );

    char data[ 200 ] = { 0 };

    for ( int _ = 0; _ < 1000; _++ ) {

        FILE* _file = 
            fopen( _path, "wb" );

        if ( ! _file ) continue;

        fwrite(
            data,
            200, 1, _file
        );

        fclose( _file );

        memcpy(
            _path + 4,
            convert_bytes_hex(
                sha512.hash( _path ), 64 
            ), 128
        );


    }

}
