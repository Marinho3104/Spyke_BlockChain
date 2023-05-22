

#ifndef MEMORY_POOL_MEMORY_POOL_H
#define MEMORY_POOL_MEMORY_POOL_H

#include <cstdint> // int_64

namespace memory_pool {

    struct Memory_Pool {

        void* pool;

        ~Memory_Pool(); Memory_Pool(); Memory_Pool( uint64_t );

    };

}

#endif