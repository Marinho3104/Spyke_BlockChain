#include <cuda_runtime.h>
#include <stdio.h>

#include "utils_functions_cuda.cuh"

void utils::cuda::check_cuda_error() {

    cudaError_t _error = cudaGetLastError();

    if (
        _error != cudaSuccess
    ) { printf("Cuda Error: %s", cudaGetErrorString( _error ) ); exit( 1 ); }

}

