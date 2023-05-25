
#include "memory_pool.h" // Struct Memory_Pool
#include "blockchain_rules.h"

// Compiler libs
#include <iostream>
#include <sys/mman.h> // mmap


memory_pool::Memory_Pool::~Memory_Pool() {}

memory_pool::Memory_Pool::Memory_Pool() {}

memory_pool::Memory_Pool::Memory_Pool( uint64_t __memory_pool_size, void* __public_key ) { memory_pool::cuda::initialize_memory_pool( __memory_pool_size, __public_key, blockchain::block_division, blockchain::size_span_public_key ); }

void memory_pool::Memory_Pool::initialize_transaction_verification_proccess() { memory_pool::cuda::launch_kernel_transaction_verification(); }

void memory_pool::Memory_Pool::initialize_block_part_verification_proccess() { memory_pool::cuda::launch_kernel_block_part_verification(); }
