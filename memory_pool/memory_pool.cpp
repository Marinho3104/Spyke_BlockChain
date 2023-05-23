
#include "memory_pool.h" // Struct Memory_Pool

// Compiler libs
#include <iostream>
#include <sys/mman.h> // mmap


memory_pool::Memory_Pool::~Memory_Pool() {}

memory_pool::Memory_Pool::Memory_Pool() {}

memory_pool::Memory_Pool::Memory_Pool( uint64_t __memory_pool_size ) { memory_pool::cuda::initialize_memory_pool( __memory_pool_size ); }

void memory_pool::Memory_Pool::initialize_transaction_verification_proccess() { memory_pool::cuda::launch_kernel_transaction_verification(); }
