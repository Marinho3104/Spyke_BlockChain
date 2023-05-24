

#ifndef MEMORY_POOL_KERNEL_COMMUNICATION_CUH
#define MEMORY_POOL_KERNEL_COMMUNICATION_CUH

#include <cstdint>

namespace p2p {

    struct Connection;

}

namespace memory_pool::cuda {

    extern "C++" {

        // Sends a transaction data to a thread block to be handle and it is all correct stores it in memory pool
        // If space is available
        void handle_transaction( void* );

        // Sends data from all transaction that have not been broudcasted yet
        void transactions_to_broudcast( p2p::Connection**, uint16_t );

    }

}

#endif