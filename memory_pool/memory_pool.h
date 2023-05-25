

#ifndef MEMORY_POOL_MEMORY_POOL_H
#define MEMORY_POOL_MEMORY_POOL_H

#include <cstdint> // int_64

namespace memory_pool::cuda {

    // Cuda Extern functions
    extern "C++" {

        void launch_kernel_transaction_verification();

        void launch_kernel_block_part_verification();

        void initialize_memory_pool( uint64_t, void*, unsigned char, uint32_t );

    } 

}

namespace memory_pool {

    struct Memory_Pool {

        void* public_key;

        ~Memory_Pool(); Memory_Pool(); Memory_Pool( uint64_t, void* );

        void initialize_transaction_verification_proccess();

        void initialize_block_part_verification_proccess();

    };

}

#endif