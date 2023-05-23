

#ifndef MEMORY_POOL_KERNEL_COMMUNICATION_CUH
#define MEMORY_POOL_KERNEL_COMMUNICATION_CUH

namespace memory_pool::cuda {

    extern "C++" {

        // Sends a transaction data to a thread block to be handle and it is all correct stores it in memory pool
        // If space is available
        void handle_transaction( void* );


    }

}

#endif