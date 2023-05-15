
#include "blockchain_utils.h" // Blockchain utils Headers
#include "blockchain_rules.h" // Blockchain rules


// Compiler libs
#include <cstdint> // int_32
#include <limits> // Get types limits

unsigned char blockchain::get_public_key_type( void* __public_key ) {
   
    uint32_t* _pk_value =   
        ( uint32_t* ) __public_key;

    unsigned char _type = 0;

    for (; _type < blockchain::block_division - 1; _type++ )

        if ( *_pk_value < ( _type + 1 ) * blockchain::size_span_public_key ) break;

    return _type;
    
}


