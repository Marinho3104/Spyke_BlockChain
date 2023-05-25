

#ifndef MEMORY_POOL_KERNEL_INITIALIZATIONS_CUH
#define MEMORY_POOL_KERNEL_INITIALIZATIONS_CUH

namespace memory_pool::cuda { 

    extern "C++" {

        // Initialize threads block for transaction verification
        void launch_kernel_transaction_verification();

        // Initialize threads block for block part verification
        void launch_kernel_block_part_verification();

    }

}

#endif