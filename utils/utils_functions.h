

#ifndef UTILS_UTILS_FUNCTIONS_H
#define UTILS_UTILS_FUNCTIONS_H

// Compiler libs

#include <cstddef> // size_t

namespace utils {

    // Returns all data inside a file, in given path
    void* get_file_data( char*, size_t& );

}

#endif