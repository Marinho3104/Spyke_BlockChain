
#include "memory_pool.h" // Struct Memory_Pool

// Compiler libs
#include <iostream>
#include <sys/mman.h> // mmap


memory_pool::Memory_Pool::~Memory_Pool() {}

memory_pool::Memory_Pool::Memory_Pool() : pool( 0 ) {}

memory_pool::Memory_Pool::Memory_Pool( uint64_t __memory_pool_size ) { pool = mmap( 0, __memory_pool_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0 ); }
