#include <stdint.h>
#include <stddef.h>

// Assuming libDaisy is available in the build
// Include the necessary headers
#include "daisy.h"  // For daisy::Logger

// WASI errno values (minimal set)
#define __WASI_ESUCCESS 0
#define __WASI_EBADF 8
#define __WASI_ESPIPE 29

// WASI types (simplified)
typedef uint32_t __wasi_fd_t;
typedef uint32_t __wasi_errno_t;
typedef uint64_t __wasi_filesize_t;
typedef uint64_t __wasi_filedelta_t;
typedef uint32_t __wasi_whence_t;

#define __WASI_WHENCE_SET 0
#define __WASI_WHENCE_CUR 1
#define __WASI_WHENCE_END 2

// IOVec structure for scatter-gather I/O
typedef struct __wasi_ciovec_t {
    const void* buf;
    size_t buf_len;
} __wasi_ciovec_t;

extern "C" {

// fd_close: Close a file descriptor (stub - always success for stdout/stderr)
__wasi_errno_t w2c_wasi__snapshot__preview1_fd_close(__wasi_fd_t fd) {
    // For embedded, just return success
    return __WASI_ESUCCESS;
}

// fd_write: Write to a file descriptor (redirect stdout/stderr to libDaisy Logger)
__wasi_errno_t w2c_wasi__snapshot__preview1_fd_write(
    __wasi_fd_t fd,
    const __wasi_ciovec_t* iovs,
    size_t iovs_len,
    __wasi_filesize_t* nwritten
) {
    if (fd == 1 || fd == 2) {  // stdout or stderr
        size_t total_written = 0;
        for (size_t i = 0; i < iovs_len; ++i) {
            // Use libDaisy's Logger::Print for output
            // Note: This assumes Logger is initialized elsewhere
            // For simplicity, print as string (assuming ASCII)
            const char* str = (const char*)iovs[i].buf;
            size_t len = iovs[i].buf_len;
            // Find null terminator or use len
            size_t print_len = 0;
            while (print_len < len && str[print_len] != '\0') print_len++;
            if (print_len > 0) {
                // Use Print without newline for raw write
                daisy::Logger<>::Print("%.*s", (int)print_len, str);
            }
            total_written += len;
        }
        *nwritten = total_written;
        return __WASI_ESUCCESS;
    }
    return __WASI_EBADF;  // Bad file descriptor
}

// fd_seek: Seek in a file descriptor (stub - not supported in embedded)
__wasi_errno_t w2c_wasi__snapshot__preview1_fd_seek(
    __wasi_fd_t fd,
    __wasi_filedelta_t offset,
    __wasi_whence_t whence,
    __wasi_filesize_t* newoffset
) {
    // Seeking not supported for stdout/stderr
    return __WASI_ESPIPE;
}

}