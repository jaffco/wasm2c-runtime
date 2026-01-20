#ifndef WASM_RT_COMPAT_H
#define WASM_RT_COMPAT_H

// Compatibility header to fix issues with the runtime without modifying it
// This header should be included before the runtime headers

// Fix for mmap-related functions on non-Windows systems
#ifndef _WIN32
#include <sys/mman.h>
#include <unistd.h>
#endif

#endif // WASM_RT_COMPAT_H