

#include "kernel_block_part_verification.cuh"


// Nvcc libs
#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>

namespace memory_pool::cuda {

    void** block_part_verification_data;

    ::cuda::std::binary_semaphore* block_part_verification_semaphores;

    cudaStream_t block_part_stream;

}

__global__ void memory_pool::cuda::kernel_block_part_verification( void** __data, ::cuda::std::binary_semaphore* __sems ) {

    int _global_id = 
        blockIdx.x * blockDim.x + threadIdx.x;

    printf("Kernel block part verification launched");

    __sems[ _global_id ].acquire();

    printf("Kernel block part acquired");

}
