

#ifndef MEMORY_POOL_MEMORY_POOL_H
#define MEMORY_POOL_MEMORY_POOL_H

#include <cstdint> // int_64

namespace memory_pool::cuda {

    // Cuda Extern functions
    extern "C++" {

        void launch_kernel_transaction_verification();

        void initialize_memory_pool( uint64_t );

    } 

}

namespace memory_pool {

    struct Memory_Pool {

        ~Memory_Pool(); Memory_Pool(); Memory_Pool( uint64_t );

        void initialize_transaction_verification_proccess();

    };

}

#endif