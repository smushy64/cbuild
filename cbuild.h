#if !defined(CB_HEADER)
#define CB_HEADER
/**
 * @file cbuild.h
 *
 * @brief C-Build System.
 *
 * @details
 *
 *    Single header library for writing a build system in C.
 *    Include to get API, include again (ONCE) with CB_IMPLEMENTATION
 *    defined to define implementation.
 *   
 *    Options can be defined before first include.
 *   
 *    Options:
 *   
 *    - define CB_THREAD_COUNT with number between 2 and 16 to set how many
 *      threads cbuild is allowed to initialize.
 *   
 *    - define CB_ENABLE_ASSERTIONS to get debug assertions in implementation and
 *      access to CB_ASSERT() macro. The CB_ASSERT() macro is otherwise defined
 *      to be CB_UNUSED( ... )
 *
 *    - define CB_DISABLE_TYPEDEF to not define integer and float types.
 *
 *    - define CBPFN( x ) to set function prefixes.
 *
 *    - defined CBPT( x ) to set type prefixes.
 * 
 * @author Alicia Amarilla (smushyaa@gmail.com)
 *
 * @date May 28, 2024
 *
 * @copyright MIT License.
*/
// IWYU pragma: begin_exports
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
// IWYU pragma: end_exports

// NOTE(alicia): Macro Constants

#if !defined(CBPFN)
    /// @brief Generate identifier with prefix.
    /// @param x (identifier) Identifier to prefix.
    /// @return x
    #define CBPFN(x) x
#endif

#if !defined(CBPT)
    /// @brief Generate identifier with prefix.
    /// @param x (identifier) Identifier to prefix.
    /// @return x
    #define CBPT(x) x
#endif

/// @brief Macro for marking a dynamic array.
/// @details
/// Just pastes @c T *
/// @param T (type) Type of array.
#define CB_DARRAY( T ) T *

/// @brief C-Build major version.
#define CB_VERSION_MAJOR 0
/// @brief C-Build minor version.
#define CB_VERSION_MINOR 2
/// @brief C-Build patch version.
#define CB_VERSION_PATCH 0

/// @brief C-Build combined version.
#define CB_VERSION \
    (CB_VERSION_MAKE( CB_VERSION_MAJOR, CB_VERSION_MINOR, CB_VERSION_PATCH ))

/// @brief C-Build version string.
#define CB_VERSION_STRING \
    CB_STRINGIFY_VALUE( CB_VERSION_MAJOR ) "." \
    CB_STRINGIFY_VALUE( CB_VERSION_MINOR ) "." \
    CB_STRINGIFY_VALUE( CB_VERSION_PATCH )

/// @brief Unknown compiler.
#define CB_COMPILER_UNKNOWN 0
    /// @brief Unknown compiler name.
    #define CB_COMPILER_UNKNOWN_NAME    "Unknown"
    /// @brief Unknown compiler C compiler command.
    #define CB_COMPILER_UNKNOWN_C_CMD   "cc"
    /// @brief Unknown compiler C++ compiler command.
    #define CB_COMPILER_UNKNOWN_CPP_CMD "c++"
/// @brief GCC compiler.
#define CB_COMPILER_GCC     1
    /// @brief GCC compiler name.
    #define CB_COMPILER_GCC_NAME        "GCC"
    /// @brief GCC compiler C compiler command.
    #define CB_COMPILER_GCC_C_CMD       "gcc"
    /// @brief GCC compiler C++ compiler command.
    #define CB_COMPILER_GCC_CPP_CMD     "g++"
/// @brief Clang compiler.
#define CB_COMPILER_CLANG   2
    /// @brief Clang compiler name.
    #define CB_COMPILER_CLANG_NAME      "clang"
    /// @brief Clang compiler C compiler command.
    #define CB_COMPILER_CLANG_C_CMD     "clang"
    /// @brief Clang compiler C++ compiler command.
    #define CB_COMPILER_CLANG_CPP_CMD   "clang++"
/// @brief Microsoft Visual C++ compiler.
#define CB_COMPILER_MSVC    3
    /// @brief MSVC compiler name.
    #define CB_COMPILER_MSVC_NAME       "Microsoft Visual C++"
    /// @brief MSVC compiler C compiler command.
    #define CB_COMPILER_MSVC_C_CMD      "cl"
    /// @brief MSVC compiler C++ compiler command.
    #define CB_COMPILER_MSVC_CPP_CMD    "cl"

#if !defined(CB_COMPILER_COUNT)
    /// @brief Number of compiler enum variants.
    #define CB_COMPILER_COUNT 4
#endif
// CB_COMPILER_CURRENT
// CB_COMPILER_CURRENT_NAME
// CB_COMPILER_CURRENT_C_CMD
// CB_COMPILER_CURRENT_CPP_CMD
// CB_COMPILER_IS_GNU_COMPATIBLE (1 or undefined)

/// @brief Unknown platform.
#define CB_PLATFORM_UNKNOWN   0
    /// @brief Unknown platform name.
    #define CB_PLATFORM_UNKNOWN_NAME       "Unknown"
/// @brief GNU/Linux platform.
#define CB_PLATFORM_GNU_LINUX 1
    /// @brief GNU/Linux platform name.
    #define CB_PLATFORM_GNU_LINUX_NAME     "GNU/Linux"
/// @brief Windows platform.
#define CB_PLATFORM_WINDOWS   2
    /// @brief Windows platform name.
    #define CB_PLATFORM_WINDOWS_NAME       "Windows"
    /// @brief Windows (MinGW) platform name.
    #define CB_PLATFORM_WINDOWS_MINGW_NAME "Windows (MinGW)"
/// @brief MacOS platform.
#define CB_PLATFORM_MACOS     3
    /// @brief MacOS platform name.
    #define CB_PLATFORM_MACOS_NAME         "MacOS"

#if !defined(CB_PLATFORM_COUNT)
    /// @brief Number of platform enum variants.
    #define CB_PLATFORM_COUNT 4
#endif
// CB_PLATFORM_CURRENT
// CB_PLATFORM_CURRENT_NAME
// CB_PLATFORM_IS_POSIX (1 or undefined)
// CB_PLATFORM_IS_MINGW (1 or undefined, always undefined on non-windows)

/// @brief Unknown architecture.
#define CB_ARCH_UNKNOWN 0
    /// @brief Unknown architecture with unknown word size.
    #define CB_ARCH_UNKNOWN_UNKNOWN_NAME "Unknown-Unknown"
    /// @brief Unknown architecture with 32-bit word size.
    #define CB_ARCH_UNKNOWN_32_NAME      "Unknown-32"
    /// @brief Unknown architecture with 64-bit word size.
    #define CB_ARCH_UNKNOWN_64_NAME      "Unknown-64"
/// @brief x86 architecture.
#define CB_ARCH_X86     1
    /// @brief x86 architecture with unknown word size.
    #define CB_ARCH_X86_UNKNOWN_NAME     "x86-Unknown"
    /// @brief x86 architecture with 32-bit word size.
    #define CB_ARCH_X86_32_NAME          "x86"
    /// @brief x86 architecture with 64-bit word size.
    #define CB_ARCH_X86_64_NAME          "x86-64"
/// @brief ARM architecture.
#define CB_ARCH_ARM     2
    /// @brief ARM architecture with unknown word size.
    #define CB_ARCH_ARM_UNKNOWN_NAME     "ARM-Unknown"
    /// @brief ARM architecture with 32-bit word size.
    #define CB_ARCH_ARM_32_NAME          "ARM"
    /// @brief ARM architecture with 64-bit word size.
    #define CB_ARCH_ARM_64_NAME          "ARM64"

#if !defined(CB_ARCH_COUNT)
    /// @brief Number of architecture enum variants.
    #define CB_ARCH_COUNT 3
#endif
// CB_ARCH_CURRENT
// CB_ARCH_CURRENT_NAME
// CB_ARCH_WORD_SIZE (realistically, 64 or 32, defaults to 32)
// CB_ARCH_IS_64BIT  (1 or undefined)

#if !defined(CB_COMPILER_CURRENT)
    #if defined(__clang__)
        /// @brief Current compiler is clang.
        #define CB_COMPILER_CURRENT CB_COMPILER_CLANG
    #elif defined(__GNUC__)
        /// @brief Current compiler is GCC.
        #define CB_COMPILER_CURRENT CB_COMPILER_GCC
    #elif defined(_MSC_VER)
        /// @brief Current compiler is MSVC.
        #define CB_COMPILER_CURRENT CB_COMPILER_MSVC
    #else
        /// @brief Current compiler is unknown.
        #define CB_COMPILER_CURRENT CB_COMPILER_UNKNOWN
    #endif
#endif

#if !defined(CB_COMPILER_IS_GNU_COMPATIBLE)
    #if CB_COMPILER_CURRENT == CB_COMPILER_GCC || \
        CB_COMPILER_CURRENT == CB_COMPILER_CLANG

        /// @brief Current compiler is compatible with GNU extensions.
        #define CB_COMPILER_IS_GNU_COMPATIBLE 1
    #endif
#endif

#if !defined(CB_PLATFORM_CURRENT)
    #if defined(_WIN32)
        /// @brief Current platform is Windows.
        #define CB_PLATFORM_CURRENT CB_PLATFORM_WINDOWS
    #elif defined(__gnu_linux__)
        /// @brief Current platform is GNU/Linux.
        #define CB_PLATFORM_CURRENT CB_PLATFORM_GNU_LINUX
    #elif defined(__APPLE__)
        /// @brief Current platform is MacOS.
        #define CB_PLATFORM_CURRENT CB_PLATFORM_MACOS
    #else
        /// @brief Current platform is unknown.
        #define CB_PLATFORM_CURRENT CB_PLATFORM_UNKNOWN
    #endif
#endif

#if !defined(CB_PLATFORM_IS_POSIX)
    #if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX || \
        CB_PLATFORM_CURRENT == CB_PLATFORM_MACOS
        
        /// @brief Current platform is POSIX compliant.
        #define CB_PLATFORM_IS_POSIX 1

    #elif CB_PLATFORM_CURRENT == CB_PLATFORM_UNKNOWN && CB_COMPILER_IS_GNU_COMPATIBLE

        #if __has_include(<unistd.h>)
            /// @brief Current platform is POSIX compliant.
            #define CB_PLATFORM_IS_POSIX 1
        #endif

    #endif
#endif

#if !defined(CB_PLATFORM_IS_MINGW)
    #if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS && \
        (defined(__MINGW32__) || defined(__MINGW64__))
    
        /// @brief Platform is Windows with MinGW.
        #define CB_PLATFORM_IS_MINGW 1
    #endif
#endif

#if !defined(CB_ARCH_CURRENT)
    #if defined(__aarch64__) || defined(_M_ARM64)

        /// @brief Current architecture is ARM.
        #define CB_ARCH_CURRENT CB_ARCH_ARM

        #if !defined(CB_ARCH_WORD_SIZE)
            /// @brief Current word size is 64-bit.
            #define CB_ARCH_WORD_SIZE 64
        #endif

    #elif defined(__arm__) || defined(_M_ARM)

        /// @brief Current architecture is ARM.
        #define CB_ARCH_CURRENT CB_ARCH_ARM

        #if !defined(CB_ARCH_WORD_SIZE)
            /// @brief Current word size is 32-bit.
            #define CB_ARCH_WORD_SIZE 32
        #endif
    
    #elif defined(__X86__) || defined(__x86_64__)

        /// @brief Current architecture is x86.
        #define CB_ARCH_CURRENT CB_ARCH_X86

        #if !defined(CB_ARCH_WORD_SIZE)
            /// @brief Current word size is 64-bit.
            #define CB_ARCH_WORD_SIZE 64
        #endif

    #elif defined(__i386__) || defined(_M_IX86)

        /// @brief Current architecture is x86.
        #define CB_ARCH_CURRENT CB_ARCH_X86

        #if !defined(CB_ARCH_WORD_SIZE)
            /// @brief Current word size is 32-bit.
            #define CB_ARCH_WORD_SIZE 32
        #endif

    #else

        /// @brief Current architecture is unknown.
        #define CB_ARCH_CURRENT CB_ARCH_UNKNOWN

        #if !defined(CB_ARCH_WORD_SIZE)

            #if UINT64_MAX == UINTPTR_MAX
                /// @brief Current word size is 64-bit.
                #define CB_ARCH_WORD_SIZE 64
            #else
                /// @brief Current word size is 32-bit.
                #define CB_ARCH_WORD_SIZE 32
            #endif

        #endif
    #endif
#endif /* !defined(CB_ARCH_CURRENT) */

#if !defined(CB_ARCH_IS_64BIT)
    #if CB_ARCH_WORD_SIZE == 64
        /// @brief Current architecture is 64-bit.
        #define CB_ARCH_IS_64BIT 1
    #endif /* CB_ARCH_WORD_SIZE == 64 */
#endif /* !defined(CB_ARCH_IS_64BIT) */

#if !defined(CB_COMPILER_CURRENT_NAME)
    #if CB_COMPILER_CURRENT == CB_COMPILER_GCC
        /// @brief Name of current compiler.
        #define CB_COMPILER_CURRENT_NAME CB_COMPILER_GCC_NAME
    #elif CB_COMPILER_CURRENT == CB_COMPILER_CLANG
        /// @brief Name of current compiler.
        #define CB_COMPILER_CURRENT_NAME CB_COMPILER_CLANG_NAME
    #elif CB_COMPILER_CURRENT == CB_COMPILER_MSVC
        /// @brief Name of current compiler.
        #define CB_COMPILER_CURRENT_NAME CB_COMPILER_MSVC_NAME
    #else
        /// @brief Name of current compiler.
        #define CB_COMPILER_CURRENT_NAME CB_COMPILER_UNKNOWN_NAME
    #endif
#endif

#if !defined(CB_COMPILER_CURRENT_C_CMD)
    #if CB_COMPILER_CURRENT == CB_COMPILER_CLANG
        /// @brief C compile command of current compiler.
        #define CB_COMPILER_CURRENT_C_CMD   CB_COMPILER_CLANG_C_CMD
    #elif CB_COMPILER_CURRENT == CB_COMPILER_GCC
        /// @brief C compile command of current compiler.
        #define CB_COMPILER_CURRENT_C_CMD   CB_COMPILER_GCC_C_CMD
    #elif CB_COMPILER_CURRENT == CB_COMPILER_MSVC
        /// @brief C compile command of current compiler.
        #define CB_COMPILER_CURRENT_C_CMD   CB_COMPILER_MSVC_C_CMD
    #else
        /// @brief C compile command of current compiler.
        #define CB_COMPILER_CURRENT_C_CMD   CB_COMPILER_UNKNOWN_C_CMD
    #endif
#endif

#if !defined(CB_COMPILER_CURRENT_CPP_CMD)
    #if CB_COMPILER_CURRENT == CB_COMPILER_CLANG
        /// @brief C++ compile command of current compiler.
        #define CB_COMPILER_CURRENT_CPP_CMD CB_COMPILER_CLANG_CPP_CMD
    #elif CB_COMPILER_CURRENT == CB_COMPILER_GCC
        /// @brief C++ compile command of current compiler.
        #define CB_COMPILER_CURRENT_CPP_CMD CB_COMPILER_GCC_CPP_CMD
    #elif CB_COMPILER_CURRENT == CB_COMPILER_MSVC
        /// @brief C++ compile command of current compiler.
        #define CB_COMPILER_CURRENT_CPP_CMD CB_COMPILER_MSVC_CPP_CMD
    #else
        /// @brief C++ compile command of current compiler.
        #define CB_COMPILER_CURRENT_CPP_CMD CB_COMPILER_UNKNOWN_CPP_CMD
    #endif
#endif

#if !defined(CB_PLATFORM_CURRENT_NAME)
    #if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
        #if CB_PLATFORM_IS_MINGW
            /// @brief Name of current platform.
            #define CB_PLATFORM_CURRENT_NAME CB_PLATFORM_WINDOWS_MINGW_NAME
        #else
            /// @brief Name of current platform.
            #define CB_PLATFORM_CURRENT_NAME CB_PLATFORM_WINDOWS_NAME
        #endif
    #elif CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
        /// @brief Name of current platform.
        #define CB_PLATFORM_CURRENT_NAME CB_PLATFORM_GNU_LINUX_NAME
    #elif CB_PLATFORM_CURRENT == CB_PLATFORM_MACOS
        /// @brief Name of current platform.
        #define CB_PLATFORM_CURRENT_NAME CB_PLATFORM_MACOS_NAME
    #else
        /// @brief Name of current platform.
        #define CB_PLATFORM_CURRENT_NAME CB_PLATFORM_UNKNOWN_NAME
    #endif
#endif

#if !defined(CB_ARCH_CURRENT_NAME)
    #if CB_ARCH_CURRENT == CB_ARCH_X86
        #if CB_ARCH_WORD_SIZE == 64
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_X86_64_NAME
        #elif CB_ARCH_WORD_SIZE == 32
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_X86_32_NAME
        #else
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_X86_UNKNOWN_NAME
        #endif
    #elif CB_ARCH_CURRENT == CB_ARCH_ARM
        #if CB_ARCH_WORD_SIZE == 64
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_ARM_64_NAME
        #elif CB_ARCH_WORD_SIZE == 32
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_ARM_32_NAME
        #else
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_ARM_UNKNOWN_NAME
        #endif
    #else
        #if CB_ARCH_WORD_SIZE == 64
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_UNKNOWN_64_NAME
        #elif CB_ARCH_WORD_SIZE == 32
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_UNKNOWN_32_NAME
        #else
            /// @brief Name of current architecture.
            #define CB_ARCH_CURRENT_NAME CB_ARCH_UNKNOWN_UNKNOWN_NAME
        #endif
    #endif
#endif

#if !defined(CB_PATH_CAPACITY)
    #if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
        /// @brief Maximum path length expected.
        #define CB_PATH_CAPACITY (8192)
        // NOTE(alicia): this is to allow wide paths to be the same max length as narrow paths
    #else
        /// @brief Maximum path length expected.
        #define CB_PATH_CAPACITY (4096)
    #endif
#endif

/// @brief Number of local buffers per thread.
#define CB_LOCAL_BUFFER_COUNT (4)
/// @brief Capacity of local buffers in bytes.
#define CB_LOCAL_BUFFER_CAPACITY CB_PATH_CAPACITY

/// @brief Maximum number of MT jobs.
#define CB_MAX_JOBS (32)

/// @brief Minimum number of threads allowed.
#define CB_THREAD_COUNT_MIN (1)
/// @brief Maximum number of threads allowed.
#define CB_THREAD_COUNT_MAX (16)

#if !defined(CB_THREAD_COUNT)
    /// @brief Number of threads to be spawned for job system.
    #define CB_THREAD_COUNT (8)
#else
    #if CB_THREAD_COUNT > CB_THREAD_COUNT_MAX
        /// @brief Number of threads to be spawned for job system.
        #define CB_THREAD_COUNT CB_THREAD_COUNT_MAX
    #elif CB_THREAD_COUNT < CB_THREAD_COUNT_MIN
        /// @brief Number of threads to be spawned for job system.
        #define CB_THREAD_COUNT CB_THREAD_COUNT_MIN
    #endif
#endif

#if defined(__cplusplus)
    #define restrict __restrict

    /// @brief Define struct.
    #define CB_STRUCT(x) x
#else
    /// @brief Define struct.
    #define CB_STRUCT(x) (x)
#endif


// NOTE(alicia): Types

#if !defined(CB_DISABLE_TYPEDEF)
    /// @brief Unsigned 8-bit integer.
    typedef uint8_t   u8;
    /// @brief Unsigned 16-bit integer.
    typedef uint16_t  u16;
    /// @brief Unsigned 32-bit integer.
    typedef uint32_t  u32;
    /// @brief Unsigned 64-bit integer.
    typedef uint64_t  u64;
    /// @brief Unsigned pointer sized integer.
    typedef uintptr_t usize;

    /// @brief 8-bit integer.
    typedef int8_t   i8;
    /// @brief 16-bit integer.
    typedef int16_t  i16;
    /// @brief 32-bit integer.
    typedef int32_t  i32;
    /// @brief 64-bit integer.
    typedef int64_t  i64;
    /// @brief Pointer sized integer.
    typedef intptr_t isize;

    /// @brief IEEE-754 single precision float.
    typedef float  f32;
    /// @brief IEEE-754 double precision float.
    typedef double f64;
#endif

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    /// @brief Volatile 32-bit signed for atomic operations.
    typedef volatile long      atom;
    /// @brief Volatile 64-bit signed for atomic operations.
    typedef volatile long long atom64;
#else
    /// @brief Volatile 32-bit signed for atomic operations.
    typedef volatile int32_t atom;
    /// @brief Volatile 64-bit signed for atomic operations.
    typedef volatile int64_t atom64;
#endif

/// @brief String is dynamically allocated on the heap,
/// UTF-8 encoded and null-terminated.
typedef char CBPT(DString);
/// @brief String slice. Not necessarily null-terminated.
/// @note When printing, use "%.*s" format specifier and
/// provide length followed by pointer in args.
typedef struct {
    /// @brief ASCII length of string.
    size_t len;
    /// @brief Pointer to start of string buffer.
    const char* cc;
} CBPT(String);
/// @brief Logger levels.
typedef enum {
    /// @brief Info level.
    /// @details
    /// Most permissive, all logger messages allowed.
    CB_LOG_LEVEL_INFO,
    /// @brief Warning level.
    /// @details
    /// Warning, Error and Fatal messages allowed.
    CB_LOG_LEVEL_WARNING,
    /// @brief Error level.
    /// @details
    /// Error and Fatal messages allowed.
    CB_LOG_LEVEL_ERROR,
    /// @brief Fatal level.
    /// @details
    /// Most restrictive level, only fatal messages allowed.
    CB_LOG_LEVEL_FATAL,
} CBPT(LogLevel);
/// @brief Types of file seek.
typedef enum {
    /// @brief Seek from current position.
    CB_SEEK_TYPE_CURRENT,
    /// @brief Seek from start of file.
    CB_SEEK_TYPE_BEGIN,
    /// @brief Seek from end of file.
    CB_SEEK_TYPE_END,
} CBPT(SeekType);
/// @brief Flags for opening file.
typedef enum {
    /// @brief Open file for reading.
    CB_FILE_OPEN_READ     = (1 << 0),
    /// @brief Open file for writing.
    CB_FILE_OPEN_WRITE    = (1 << 1),
    /// @brief Create file. File must not exist if using this flag.
    CB_FILE_OPEN_CREATE   = (1 << 2),
    /// @brief Open and truncate file. File can only be opened for writing.
    CB_FILE_OPEN_TRUNCATE = (1 << 3),
    /// @brief Open and set position to end of file. File must be opened for writing.
    CB_FILE_OPEN_APPEND   = (1 << 4),
} CBPT(FileOpenFlag);

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    /// @brief Cross-platform file descriptor.
    typedef intptr_t CBPT(FileID);
    /// @brief Cross-platform process ID.
    typedef intptr_t CBPT(ProcessID);
#else
    /// @brief Cross-platform file descriptor.
    typedef int   CBPT(FileID);
    /// @brief Cross-platform process ID.
    typedef pid_t CBPT(ProcessID);
#endif
/// @brief Opaque mutex handle.
typedef struct {
    /// @brief Opaque handle.
    void* handle;
} Mutex;
/// @brief Opaque semaphore handle.
typedef struct {
    /// @brief Opaque handle.
    void* handle;
} Semaphore;
/// @brief Cross-platform pipe for reading.
typedef CBPT(FileID) CBPT(PipeRead);
/// @brief Cross-platform pipe for writing.
typedef CBPT(FileID) CBPT(PipeWrite);
/// @brief Cross-platform generic pipe (reading or writing).
typedef CBPT(FileID) CBPT(Pipe);
/// @brief Command line arguments for creating a process.
typedef struct {
    /// @brief Number of arguments.
    size_t        count;
    /// @brief Array of arguments. Last item in array is a null-pointer.
    const char** args;
} CBPT(Command);
/// @brief Command line argument builder.
typedef struct {
    /// @brief Character buffer.
    CB_DARRAY(char) buf;
    /// @brief Array of strings.
    CB_DARRAY(const char*) args;
} CBPT(CommandBuilder);
/// @brief Result of walking a directory.
typedef struct {
    /// @brief Number of paths found in directory.
    size_t    count;
    /// @brief Dynamic array of paths found in directory.
    CB_DARRAY(CBPT(String)) paths;
    /// @brief String buffer.
    CBPT(DString)* buf;
} WalkDirectory;
/// @brief Hang thread on wait.
#define MT_WAIT_INFINITE (UINT32_MAX)
/// @brief Function prototype for job system.
/// @param[in] params (optional) Pointer to additional parameters.
typedef void JobFN( void* params );
/// @brief Filter prototype for string split functions.
/// @param     index  Index of the string being filtered.
/// @param     str    String to be filtered.
/// @param[in] params (optional) Pointer to additional parameters.
/// @return Filtered string.
typedef CBPT(String) CBPT(StringSplitDelimFilterFN)(
    size_t index, CBPT(String) str, void* params );
/// @brief Filter prototype for dynamic array.
/// @param     index       Index of item being tested.
/// @param     item_stride Size of items in array.
/// @param[in] item        Pointer to item being tested.
/// @param[in] params      (optional) Pointer to additional parameters.
/// @return
///     - @c True  : Item passed filter, will be kept in.
///     - @c False : Item failed filter, will be filtered out.
typedef bool CBPT(DarrayFilterFN)(
    size_t index, size_t item_stride, const void* item, void* params );

// NOTE(alicia): Macro Functions

/// @brief Mark any variables/parameters as unused.
/// @param ... Variables to be marked as unused.
#define CB_UNUSED( ... ) _INTERNAL_CB_UNUSED_FUNCTION( 0, __VA_ARGS__ )

/// @brief Create a new string from string literal.
/// @param literal (string literal) Literal to make string out of.
/// @return New @c string.
#define CB_STRING( literal ) \
    CBPFN(string_new)( sizeof(literal) - 1, literal )

/// @brief Create a new dynamic string from string literal.
/// @param literal (string literal) Literal to make string from.
/// @return New dynamic string.
#define CB_DSTRING( literal ) \
    CBPFN(dstring_new)( sizeof(literal) - 1, literal )

#define _INTERNAL_CB_INSERT_PANIC() exit(-1)

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    #define _INTERNAL_CB_INSERT_UNREACHABLE()    __assume(0)
    #define _INTERNAL_CB_INSERT_CRASH()          __debugbreak()

    /// @brief Insert a debugger break statement.
    #define CB_DEBUG_BREAK()                     __debugbreak()

    /// @brief Mark function as no return.
    #define CB_NO_RETURN                         __declspec( noreturn )

    /// @brief Cross-compiler macro for declaring a thread local variable.
    #define CB_TLS( type )\
        __declspec( thread ) type
#else
    #define _INTERNAL_CB_INSERT_UNREACHABLE()    __builtin_unreachable()
    #define _INTERNAL_CB_INSERT_CRASH()          __builtin_trap()

    /// @brief Insert a debugger break statement.
    #define CB_DEBUG_BREAK()                     __builtin_debugtrap()

    /// @brief Mark function as no return.
    #define CB_NO_RETURN                         _Noreturn

    /// @brief Cross-compiler macro for declaring a thread local variable.
    #define CB_TLS( type )\
        _Thread_local type
#endif

#if defined(CB_ENABLE_ASSERTIONS)
    /// @brief Check if condition is true. Panic if it's not. (Noreturn on fail)
    /// @param condition (boolean expression) Condition to check.
    /// @param ...       (format and format arguments) Message to log upon failed condition.
    #define CB_ASSERT( condition, ... ) do {\
        if( !(condition) ) {\
            fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): assertion '"#condition"' failed! message: ", thread_id(), __LINE__, __FUNCTION__ );\
            fprintf( stderr, __VA_ARGS__ );\
            fprintf( stderr, "\033[1;00m\n" );\
            fflush( stderr );\
            fence();\
            _INTERNAL_CB_INSERT_PANIC();\
        }\
    } while(0)
#else
    /// @brief Check if condition is true. Panic if it's not. (Noreturn on fail)
    /// @param ... (anything) Assertion is disabled so any arguments go to CB_UNUSED() macro.
    #define CB_ASSERT( ... ) CB_UNUSED( __VA_ARGS__ )
#endif

/// @brief Insert a panic with message. (Noreturn)
/// @param ... (format and format arguments) Message to log.
#define CB_PANIC( ... ) do {\
    fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): panic! message: ", thread_id(), __LINE__, __FUNCTION__ );\
    fprintf( stderr, __VA_ARGS__ );\
    fprintf( stderr, "\033[1;00m\n" );\
    fflush( stderr );\
    fence();\
    _INTERNAL_CB_INSERT_PANIC();\
} while(0)

/// @brief Assert something that should always be checked. (Noreturn on fail)
/// @details
/// Crashes program rather than inserting exit(-1) on fail.
/// @param condition (boolean expression) Condition to check.
/// @param ...       (format and format arguments) Message to log upon failed condition.
#define CB_EXPECT_CRASH( condition, ... ) do {\
    if( !(condition) ) {\
        fprintf( stderr, "\033[1;35m[F:%02u] " __FILE__ ":%i:%s(): expected '" #condition "'! message: ", thread_id(), __LINE__, __FUNCTION__ );\
        fprintf( stderr, __VA_ARGS__ );\
        fprintf( stderr, "\033[1;00m\n" );\
        fflush( stderr );\
        fence();\
        _INTERNAL_CB_INSERT_CRASH();\
    }\
} while(0)

/// @brief Assert something that should always be checked. (Noreturn on fail)
/// @param condition (boolean expression) Condition to check.
/// @param ...       (format and format arguments) Message to log upon failed condition.
#define CB_EXPECT( condition, ... ) do {\
    if( !(condition) ) {\
        fprintf( stderr, "\033[1;35m[F:%02u] " __FILE__ ":%i:%s(): expected '" #condition "'! message: ", thread_id(), __LINE__, __FUNCTION__ );\
        fprintf( stderr, __VA_ARGS__ );\
        fprintf( stderr, "\033[1;00m\n" );\
        fflush( stderr );\
        fence();\
        _INTERNAL_CB_INSERT_PANIC();\
    }\
} while(0)

/// @brief Mark control path as unimplemented. (Noreturn)
#define CB_UNIMPLEMENTED() do {\
    fprintf( stderr, "\033[1;35m[F:%02u] " __FILE__ ":%i:%s(): unimplemented path!\033[1;00m\n", thread_id(), __LINE__, __FUNCTION__ );\
    fflush( stderr );\
    fence();\
    _INTERNAL_CB_INSERT_PANIC();\
} while(0)

/// @brief Mark control path as unreachable (hopefully). (Noreturn)
#define CB_UNREACHABLE() do {\
    fprintf( stderr, "\033[1;35m[F:%02u] " __FILE__ ":%i:%s(): reached unreachable path!\033[1;00m\n", thread_id(), __LINE__, __FUNCTION__ );\
    fflush( stderr );\
    fence();\
    _INTERNAL_CB_INSERT_UNREACHABLE();\
    _INTERNAL_CB_INSERT_PANIC();\
} while(0)
/*           ^^^^^ just in case compiler warns */

/// @brief Create version integer the same way C-Build defines version integers.
/// @param major (unsigned short) Major version.
/// @param minor (unsigned char)  Minor version.
/// @param patch (unsigned char)  Patch version.
/// @return (unsigned int) C-Build version integer.
#define CB_VERSION_MAKE( major, minor, patch ) \
    ((unsigned int)((major) << 16u | (minor) << 8u | (patch)))

/// @brief Read major version from C-Build style version integer.
/// @param version (unsigned int) Version integer.
/// @return (unsigned short) Major version.
#define CB_VERSION_READ_MAJOR( version ) \
    ((unsigned short)(((version) & 0xFFFF0000u) >> 16u))
/// @brief Read minor version from C-Build style version integer.
/// @param version (unsigned int) Version integer.
/// @return (unsigned char) Minor version.
#define CB_VERSION_READ_MINOR( version ) \
    ((unsigned char)(((version) & 0x0000FF00u) >> 8u))
/// @brief Read patch version from C-Build style version integer.
/// @param version (unsigned int) Version integer.
/// @return (unsigned char) Patch version.
#define CB_VERSION_READ_PATCH( version ) \
    ((unsigned char)((version) & 0x000000FFu))

/// @brief Stringify.
/// @param x (Any) C identifier to stringify.
/// @return (const char* literal) String.
#define CB_STRINGIFY( x )       #x
/// @brief Stringify expanded macro.
/// @param x (Any) Macro to expand, then stringify.
/// @return (const char* literal) String.
#define CB_STRINGIFY_VALUE( x ) CB_STRINGIFY( x )

/// @brief Log an info level message to stdout.
/// @param ... (format and format arguments) Message to log.
#define CB_INFO( ... )  logger( CB_LOG_LEVEL_INFO, __VA_ARGS__ )
/// @brief Log a warning level message to stdout.
/// @param ... (format and format arguments) Message to log.
#define CB_WARN( ... )  logger( CB_LOG_LEVEL_WARNING, __VA_ARGS__ )
/// @brief Log an error level message to stderr.
/// @param ... (format and format arguments) Message to log.
#define CB_ERROR( ... ) logger( CB_LOG_LEVEL_ERROR, __VA_ARGS__ )
/// @brief Log a fatal level message to stderr.
/// @param ... (format and format arguments) Message to log.
#define CB_FATAL( ... ) logger( CB_LOG_LEVEL_FATAL, __VA_ARGS__ )

/// @brief Calculate length of static array.
/// @param array (any[]) Static array to calculate length of.
/// @return (size_t) Length of @c array.
#define CB_ARRAY_LEN( array ) (sizeof(array) / sizeof((array)[0]))
/// @brief Convert kilobytes to bytes.
/// @param kb (size_t) Kilobytes.
/// @return (size_t) @c kb as bytes.
#define CB_KILOBYTES( kb ) (kb * 1000ULL)
/// @brief Convert megabytes to bytes.
/// @param mb (size_t) Megabytes.
/// @return (size_t) @c mb as bytes.
#define CB_MEGABYTES( mb ) (CB_KILOBYTES(mb) * 1000ULL)
/// @brief Convert gigabytes to bytes.
/// @param gb (size_t) Gigabytes.
/// @return (size_t) @c gb as bytes.
#define CB_GIGABYTES( gb ) (CB_MEGABYTES(gb) * 1000ULL)
/// @brief Convert Terabytes to bytes.
/// @param tb (size_t) Terabytes.
/// @return (size_t) @c tb as bytes.
#define CB_TERABYTES( tb ) (CB_GIGABYTES(tb) * 1000ULL)
/// @brief Convert kibibytes to bytes.
/// @param kb (size_t) Kibibytes.
/// @return (size_t) @c kb as bytes.
#define CB_KIBIBYTES( kb ) (kb * 1024ULL)
/// @brief Convert mebibytes to bytes.
/// @param mb (size_t) Mebibytes.
/// @return (size_t) @c mb as bytes.
#define CB_MEBIBYTES( mb ) (CB_KIBIBYTES(mb) * 1024ULL)
/// @brief Convert gibibytes to bytes.
/// @param gb (size_t) Gibibytes.
/// @return (size_t) @c gb as bytes.
#define CB_GIBIBYTES( gb ) (CB_MEBIBYTES(gb) * 1024ULL)
/// @brief Convert tebibytes to bytes.
/// @param tb (size_t) Tebibytes.
/// @return (size_t) @c tb as bytes.
#define CB_TEBIBYTES( tb ) (CB_GIBIBYTES(tb) * 1024ULL)

/// @brief Function for initializing cbuild. Must be called from main()
/// @param logger_level Level to set logger to.
#define CB_INIT( logger_level )\
    _init_( logger_level, argv[0], __FILE__, argc, (const char**)argv )

// NOTE(alicia): Functions

/// @brief Rebuild cbuild.
/// @param[in] cbuild_source_file_name Filename of source file (__FILE__).
/// @param[in] cbuild_executable_name  Name of cbuild executable (argv[0]).
/// @param     reload                  Reloads cbuild after rebuilding. Only supported on POSIX platforms.
CB_NO_RETURN
void CBPFN(rebuild_self)(
    const char* cbuild_source_file_name,
    const char* cbuild_executable_name, bool reload );

/// @brief Allocate memory on the heap. Always returns zeroed memory.
/// @warning
/// Do not free with free(), use heap_free() instead!
/// Do not reallocate with realloc(), use heap_realloc() instead!
/// @param size Size in bytes of memory to allocate.
/// @return
///     - Pointer : Allocation succeeded, pointer to start of buffer.
///     - NULL    : Allocation failed.
void* CBPFN(heap_alloc)( size_t size );
/// @brief Reallocate memory on the heap. Always returns zeroed memory.
/// @warning
/// Do not free with free(), use heap_free() instead!
/// Do not reallocate with realloc(), use heap_realloc() instead!
/// @param[in] memory   Pointer to block of memory to reallocate.
/// @param     old_size Size in bytes of memory to reallocate.
/// @param     new_size New size of memory to reallocate. Must be > @c old_size.
/// @return
///     - Pointer : Allocation succeeded, pointer to start of buffer.
///     - NULL    : Allocation failed.
void* CBPFN(heap_realloc)( void* memory, size_t old_size, size_t new_size );
/// @brief Free memory allocated on the heap.
/// @warning
/// Only free pointers from heap_alloc() or heap_realloc()!
/// Do not use with pointer from malloc()!
/// @param[in] memory Pointer to block of memory to free.
/// @param     size   Size in bytes of block of memory.
void  CBPFN(heap_free)( void* memory, size_t size );
/// @brief Query how many bytes are currently allocated.
/// @return Bytes allocated.
size_t CBPFN(heap_query_usage)(void);
/// @brief Query how many bytes have been allocated thus far.
/// @return Bytes allocated in total.
size_t CBPFN(heap_query_total_usage)(void);
/// @brief Copy value across block of memory.
/// @param[in] memory     Block to modify, must be >= @c value_size in size.
/// @param     value_size Size of value to stamp.
/// @param[in] value      Pointer to value to stamp.
/// @param     size       Total size of @c memory in bytes.
void CBPFN(memory_stamp)( void* memory, size_t value_size, const void* value, size_t size );
/// @brief Set bytes in block of memory to a value.
/// @param[in] memory Block to modify.
/// @param     value  Value to set bytes to.
/// @param     size   Size of @c memory in bytes.
void CBPFN(memory_set)( void* memory, int8_t value, size_t size );
/// @brief Set bytes in block of memory to zero.
/// @param[in] memory Block to modify.
/// @param     size   Size of @c memory in bytes.
void CBPFN(memory_zero)( void* memory, size_t size );
/// @brief Copy bytes from one block of memory to another.
/// @details
/// @c dst and @c src cannot be overlapping.
/// Use memory_move() instead for overlapping memory.
/// @param[in] dst  Pointer to destination.
/// @param[in] src  Pointer to source.
/// @param     size Size of @c dst and @c src.
void CBPFN(memory_copy)( void* restrict dst, const void* restrict src, size_t size );
/// @brief Copy bytes from one block of memory to another.
/// @details
/// @c dst and @c src can be overlapping.
/// @param[in] dst  Pointer to destination.
/// @param[in] src  Pointer to source.
/// @param     size Size of @c dst and @c src.
void CBPFN(memory_move)( void* dst, const void* src, size_t size );
/// @brief Compare bytes from two blocks of memory for equality.
/// @param[in] a, b Pointer to blocks of memory to compare.
/// @param     size Size of @c dst and @c src.
/// @return
///     - @c True  : @c a and @c b are equal.
///     - @c False : @c a and @c b are not equal.
bool CBPFN(memory_cmp)( const void* a, const void* b, size_t size );

/// @brief Check if character is in set.
/// @param c   Character to check for.
/// @param set Set of characters to check against.
/// @return
///     - @c True  : @c c is in @c set.
///     - @c False : @c c is not in @c set.
bool CBPFN(char_in_set)( char c, CBPT(String) set );

/// @brief Calculate UTF-8 length of null-terminated string.
/// @note
/// Does not check if @c string is a valid UTF-8 string!
/// @param[in] string Pointer to character buffer.
/// @return UTF-8 length of @c string.
size_t CBPFN(strlen_utf8)( const char* string );

/// @brief Create a new string from length and buffer.
/// @param     length Length of string buffer.
/// @param[in] buf    Pointer to start of buffer.
/// @return New string.
CBPT(String) CBPFN(string_new)( size_t len, const char* buf );
/// @brief Create an empty string.
/// @return Empty @c string.
CBPT(String) CBPFN(string_empty)();
/// @brief Create a new string from null-terminated string.
/// @param s Null-terminated to make string out of.
/// @return New @c string.
CBPT(String) CBPFN(string_from_cstr)( const char* cstr );
/// @brief Create a new string from dynamic string.
/// @param d (const DString*) Dynamic string to make string out of.
/// @return New @c string.
CBPT(String) CBPFN(string_from_dstring)( CBPT(DString)* dynamic );
/// @brief Check if string is empty.
/// @param str String to check.
/// @return
///     - @c True  : @c str is empty.
///     - @c False : @c str is not empty.
bool CBPFN(string_is_empty)( CBPT(String) str );
/// @brief Check if string is null-terminated.
/// @param str String to check.
/// @return
///     - @c True  : @c str is null-terminated.
///     - @c False : @c str is not null-terminated.
bool CBPFN(string_is_null_terminated)( CBPT(String) str );
/// @brief Compare two strings for equality.
/// First compares string lengths, then compares contents.
/// @param a, b Strings to compare.
/// @return
///     - @c True  : Strings @c a and @c b are equal.
///     - @c False : Strings @c a and @c b are not equal.
bool CBPFN(string_cmp)( CBPT(String) a, CBPT(String) b );
/// @brief Search for character in string.
/// @param      str           String to search in.
/// @param      c             ASCII character to search for.
/// @param[out] opt_out_index (optional) Pointer to store index of @c c if it was found.
/// @return
///     - @c True  : @c c was found in @c string.
///     - @c False : @c c was not found in @c string.
bool CBPFN(string_find)( CBPT(String) str, char c, size_t* opt_out_index );
/// @brief Search for character in string from end of string.
/// @param      str           String to search in.
/// @param      c             ASCII character to search for.
/// @param[out] opt_out_index (optional) Pointer to store index of @c c if it was found.
/// @return
///     - @c True  : @c c was found in @c string.
///     - @c False : @c c was not found in @c string.
bool CBPFN(string_find_rev)( CBPT(String) str, char c, size_t* opt_out_index );
/// @brief Search for a set of characters in string.
/// @param      str           String to search in.
/// @param      set           Set of ASCII characters to search for.
/// @param[out] opt_out_index (optional) Pointer to store index of first character in set found.
/// @return
///     - @c True  : Character in @c set was found in @c string.
///     - @c False : Character in @c set was not found in @c string.
bool CBPFN(string_find_set)( CBPT(String) str, CBPT(String) set, size_t* opt_out_index );
/// @brief Search for a set of characters in string from end of string.
/// @param      str           String to search in.
/// @param      set           Set of ASCII characters to search for.
/// @param[out] opt_out_index (optional) Pointer to store index of first character in set found.
/// @return
///     - @c True  : Character in @c set was found in @c string.
///     - @c False : Character in @c set was not found in @c string.
bool CBPFN(string_find_set_rev)( CBPT(String) str, CBPT(String) set, size_t* opt_out_index );
/// @brief Search for substring in string.
/// @param      str           String to search in.
/// @param      substr        String to search for.
/// @param[out] opt_out_index (optional) Pointer to store index of @c substr if it was found.
/// @return
///     - @c True  : @c substr was found in @c string.
///     - @c False : @c substr was not found in @c string.
bool CBPFN(string_find_string)( CBPT(String) str, CBPT(String) substr, size_t* opt_out_index );
/// @brief Search for substring in string from end of string.
/// @param      str           String to search in.
/// @param      substr        String to search for.
/// @param[out] opt_out_index (optional) Pointer to store index of @c substr if it was found.
/// @return
///     - @c True  : @c substr was found in @c string.
///     - @c False : @c substr was not found in @c string.
bool CBPFN(string_find_string_rev)( CBPT(String) str, CBPT(String) substr, size_t* opt_out_index );
/// @brief Get pointer to first character in string.
/// @param str String to get character from.
/// @return
///     - @c Pointer : @c str is not empty.
///     - @c NULL    : @c str is empty.
char* CBPFN(string_first)( CBPT(String) str );
/// @brief Get pointer to last character in string.
/// @param str String to get character from.
/// @return
///     - @c Pointer : @c str is not empty.
///     - @c NULL    : @c str is empty.
char* CBPFN(string_last)( CBPT(String) str );
/// @brief Advance string forward by one character.
/// @param str String to advance.
/// @return
///     - @c string       : String one character forward.
///     - @c empty string : String has no more characters.
CBPT(String) CBPFN(string_advance)( CBPT(String) str );
/// @brief Advance string forward by given number of characters.
/// @param str    String to advance.
/// @param stride Number of characters to advance by.
/// @return
///     - @c string       : String @c stride number of characters forward.
///     - @c empty string : String has no more characters or stride >= @c str.len.
CBPT(String) CBPFN(string_advance_by)( CBPT(String) str, size_t stride );
/// @brief Shorten string length to maximum.
/// @details
/// Does nothing if max > str.len.
/// Returns empty string if max == 0.
/// @param str String to truncate.
/// @param max Maximum length to truncate to.
/// @return Truncated string.
CBPT(String) CBPFN(string_truncate)( CBPT(String) str, size_t max );
/// @brief Subtract from string length.
/// @details
/// Does nothing if amount == 0. 
/// Returns empty string if amount >= str.len.
/// @param str    String to trim.
/// @param amount Number of characters to trim from the end.
/// @return Trimmed string.
CBPT(String) CBPFN(string_trim)( CBPT(String) str, size_t amount );
/// @brief Remove leading whitespace characters from string.
/// @param str String to remove from.
/// @return @c str without leading whitespace.
CBPT(String) CBPFN(string_trim_leading_whitespace)( CBPT(String) str );
/// @brief Remove trailing whitespace characters from string.
/// @param str String to remove from.
/// @return @c str without trailing whitespace.
CBPT(String) CBPFN(string_trim_trailing_whitespace)( CBPT(String) str );
/// @brief Remove leading and trailing whitespace characters from string.
/// @param str String to remove from.
/// @return @c str without leading or trailing whitespace.
CBPT(String) CBPFN(string_trim_surrounding_whitespace)( CBPT(String) str );
/// @brief Split string in two from given index.
/// @warning
/// CB_EXPECT() 's that @c at is <= @c str.len!
/// @param      src           String to split.
/// @param      at            Index to split at.
/// @param      keep_split    True to keep index in right side, false to remove it.
/// @param[out] opt_out_left  (optional) Pointer to write left side to.
/// @param[out] opt_out_right (optional) Pointer to write right side to.
void CBPFN(string_split_at)(
    CBPT(String) src, size_t at, bool keep_split,
    CBPT(String)* opt_out_left, CBPT(String)* opt_out_right );
/// @brief Split string at first instance of character.
/// @param      src           String to split.
/// @param      c             Character to split at.
/// @param      keep_split    True to keep character in right side, false to remove it.
/// @param[out] opt_out_left  (optional) Pointer to write left side to.
/// @param[out] opt_out_right (optional) Pointer to write right side to.
/// @return
///     - @c True  : Character @c c was found in @c src and string was split.
///     - @c False : Character @c c was not found in @c src.
bool CBPFN(string_split_char)(
    CBPT(String) src, char c, bool keep_split,
    CBPT(String)* opt_out_left, CBPT(String)* opt_out_right );
/// @brief Split string by delimiter. Allocates dynamic array.
/// @param src        String to split.
/// @param delim      Delimiter to split by.
/// @param keep_delim True if delimiter should be kept in splits.
/// @return
///     - @c Dynamic array of strings : Dynamic array was allocated with splits.
///     - @c NULL : Failed to allocate dynamic array of splits.
CBPT(String)* CBPFN(string_split_delim)( CBPT(String) src, CBPT(String) delim, bool keep_delim );
/// @brief Split string by delimiter. Allocates dynamic array.
/// @param src        String to split.
/// @param delim      Delimiter character to split by.
/// @param keep_delim True if delimiter should be kept in splits.
/// @return
///     - @c Dynamic array of strings : Dynamic array was allocated with splits.
///     - @c NULL : Failed to allocate dynamic array of splits.
CBPT(String)* CBPFN(string_split_delim_char)( CBPT(String) src, char delim, bool keep_delim );
/// @brief Split string by delimiter. Allocates dynamic array.
/// @param     src        String to split.
/// @param     delim      Delimiter to split by.
/// @param     keep_delim True if delimiter should be kept in splits.
/// @param[in] filter     Function to further process splits.
/// @param[in] params     (optional) Parameters for filtering function.
/// @return
///     - @c Dynamic array of strings : Dynamic array was allocated with splits.
///     - @c NULL : Failed to allocate dynamic array of splits.
CBPT(String)* CBPFN(string_split_delim_ex)(
    CBPT(String) src, CBPT(String) delim, bool keep_delim,
    CBPT(StringSplitDelimFilterFN)* filter, void* params );
/// @brief Split string by delimiter. Allocates dynamic array.
/// @param     src        String to split.
/// @param     delim      Delimiter character to split by.
/// @param     keep_delim True if delimiter should be kept in splits.
/// @param[in] filter     Function to further process splits.
/// @param[in] params     (optional) Parameters for filtering function.
/// @return
///     - @c Dynamic array of strings : Dynamic array was allocated with splits.
///     - @c NULL : Failed to allocate dynamic array of splits.
CBPT(String)* CBPFN(string_split_delim_char_ex)(
    CBPT(String) src, char delim, bool keep_delim,
    CBPT(StringSplitDelimFilterFN)* filter, void* params );
/// @brief Calculate UTF-8 length of string.
/// @note
/// Does not check if @c string is a valid UTF-8 string!
/// @param str String to get UTF-8 length of.
/// @return UTF-8 length of @c str.
size_t CBPFN(string_len_utf8)( CBPT(String) str );

/// @brief Allocate an empty dynamic string with given capacity.
/// @param cap Capacity to allocate.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_empty)( size_t cap );
/// @brief Create new dynamic string from string buffer.
/// @details
/// Allocates appropriately sized dynamic string and
/// copies characters from @c str into it.
/// Dynamic strings are always null-terminated.
/// @param     len Length of string buffer.
/// @param[in] str Pointer to start of string buffer.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_new)( size_t len, const char* str );
/// @brief Create a new dynamic string from null-terminated string.
/// @details
/// Allocates appropriately sized dynamic string and
/// copies characters from @c str into it.
/// Dynamic strings are always null-terminated.
/// @param[in] str (const char*) Pointer to null-terminated string.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_from_cstr)( const char* str );
/// @brief Create a new dynamic string from string.
/// @details
/// Allocates appropriately sized dynamic string and
/// copies characters from @c str into it.
/// Dynamic strings are always null-terminated.
/// @param str (string) String.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_from_string)( CBPT(String) str );
/// @brief Create a formatted dynamic string.
/// @param[in] format Format string literal.
/// @param     va     Variadic list of formatting arguments.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_fmt_va)( const char* format, va_list va );
/// @brief Create a formatted dynamic string.
/// @param[in] format Format string literal.
/// @param     ...    Format arguments.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_fmt)( const char* format, ... );
/// @brief Attempt to reallocate dynamic string buffer.
/// @param[in] str    Dynamic string to reallocate.
/// @param     amount Additional capacity to reallocate. Gets added to existing capacity.
/// @return 
///     - @c Dynamic String : Buffer was successfully reallocated.
///     - @c NULL : Failed to reallocate dynamic string.
CBPT(DString)* CBPFN(dstring_grow)( CBPT(DString)* str, size_t amount );
/// @brief Create a new dynamic string from existing dynamic string.
/// @param[in] src Dynamic string to clone.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_clone)( const CBPT(DString)* src );
/// @brief Create dynamic string by concatenating two strings.
/// @param lhs, rhs Strings to concatenate.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_concat)( CBPT(String) lhs, CBPT(String) rhs );
/// @brief Create dynamic string by concatenating two null-terminated strings.
/// @param[in] lhs, rhs (const char*) Strings to concatenate.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_concat_cstr)( const char* lhs, const char* rhs );
/// @brief Create dynamic string by concatenating multiple strings.
/// @param     count         Number of strings to concatenate.
/// @param[in] strings       Pointer to strings to concatenate.
/// @param     opt_separator (optional) String to use to separate @c strings.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_concat_multi)(
    size_t count, const CBPT(String)* strings, CBPT(String) opt_separator );
/// @brief Create dynamic string by concatenating multiple null-terminated strings.
/// @param     count         Number of strings to concatenate.
/// @param[in] strings       Pointer to strings to concatenate.
/// @param[in] opt_separator (optional) String to use to separate @c strings.
/// @return
///     - @c Dynamic String : Buffer was successfully allocated.
///     - @c NULL : Failed to allocate dynamic string.
CBPT(DString)* CBPFN(dstring_concat_multi_cstr)(
    size_t count, const char** strings, const char* opt_separator );
/// @brief Append string to end of dynamic string.
/// @param[in] str    Dynamic string to append to.
/// @param     append String to append.
/// @return
///     - @c Dynamic String : Append was successful.
///     - @c NULL : Failed to reallocate dynamic string.
CBPT(DString)* CBPFN(dstring_append)( CBPT(DString)* str, CBPT(String) append );
/// @brief Append null-terminated string to end of dynamic string.
/// @param[in] str    Dynamic string to append to.
/// @param[in] append Null-terminated string to append.
/// @return
///     - @c Dynamic String : Append was successful.
///     - @c NULL : Failed to reallocate dynamic string.
#define dstring_append_cstr( str, append )\
    dstring_append( str, string_from_cstr( append ) )
/// @brief Append string literal to end of dynamic string.
/// @param[in] str     (DString*)       Dynamic string to append to.
/// @param     literal (string litearl) String literal to append.
/// @return
///     - @c Dynamic String : Append was successful.
///     - @c NULL : Failed to reallocate dynamic string.
#define dstring_append_text( str, literal )\
    dstring_append( str, CB_STRING( literal ) )
/// @brief Prepend string to start of dynamic string.
/// @param[in] str     Dynamic string to prepend to.
/// @param     prepend String to append.
/// @return
///     - @c Dynamic String : Prepend was successful.
///     - @c NULL : Failed to reallocate dynamic string.
CBPT(DString)* CBPFN(dstring_prepend)( CBPT(DString)* str, CBPT(String) prepend );
/// @brief Prepend null-terminated string to end of dynamic string.
/// @param[in] str     Dynamic string to prepend to.
/// @param[in] prepend Null-terminated string to prepend.
/// @return
///     - @c Dynamic String : Prepend was successful.
///     - @c NULL : Failed to reallocate dynamic string.
#define dstring_prepend_cstr( str, prepend )\
    dstring_prepend( str, string_from_cstr( prepend ) )
/// @brief Prepend string literal to end of dynamic string.
/// @param[in] str     (DString*)       Dynamic string to prepend to.
/// @param     literal (string litearl) String literal to prepend.
/// @return
///     - @c Dynamic String : Prepend was successful.
///     - @c NULL : Failed to reallocate dynamic string.
#define dstring_prepend_text( str, literal )\
    dstring_prepend( str, CB_STRING( literal ) )
/// @brief Insert string inside dynamic string.
/// @param[in] str    Dynamic string to insert in.
/// @param     insert String to insert.
/// @param     at     Index to insert at. Must be <= @c str.len.
/// @return
///     - @c Dynamic String : Insert was successful.
///     - @c NULL : Failed to reallocate dynamic string.
CBPT(DString)* CBPFN(dstring_insert)( CBPT(DString)* str, CBPT(String) insert, size_t at );
/// @brief Insert null-terminated string inside dynamic string.
/// @param[in] str    Dynamic string to insert in.
/// @param[in] insert String to insert.
/// @param     at     Index to insert at. Must be <= @c str.len.
/// @return
///     - @c Dynamic String : Insert was successful.
///     - @c NULL : Failed to reallocate dynamic string.
#define dstring_insert_cstr( str, insert, at )\
    dstring_insert( str, string_from_cstr( insert ), at )
/// @brief Insert string literal inside dynamic string.
/// @param[in] str     Dynamic string to insert in.
/// @param     literal (string literal) String to insert.
/// @param     at      Index to insert at. Must be <= @c str.len.
/// @return
///     - @c Dynamic String : Insert was successful.
///     - @c NULL : Failed to reallocate dynamic string.
#define dstring_insert_text( str, literal, at )\
    dstring_insert( str, CB_STRING( literal ), at )
/// @brief Append character to end of dynamic string.
/// @param[in] str Dynamic string to push into.
/// @param     c   Character to push.
/// @return
///     - @c Dynamic String : Push was successful.
///     - @c NULL : Failed to reallocate dynamic string.
CBPT(DString)* CBPFN(dstring_push)( CBPT(DString)* str, char c );
/// @brief Emplace character inside of dynamic string.
/// @param[in] str Dynamic string to emplace into.
/// @param     c   Character to emplace.
/// @param     at  Index to emplace at.
/// @return
///     - @c Dynamic String : Push was successful.
///     - @c NULL : Failed to reallocate dynamic string.
CBPT(DString)* CBPFN(dstring_emplace)( CBPT(DString)* str, char c, size_t at );
/// @brief Pop last character from dynamic string, if available.
/// @param[in]  str       Dynamic string to pop character from.
/// @param[out] opt_out_c (optional) Pointer to write popped character to.
/// @return
///     - @c True  : @c str has a character to pop.
///     - @c False : @c str is empty.
bool CBPFN(dstring_pop)( CBPT(DString)* str, char* opt_out_c );
/// @brief Remove character from dynamic string.
/// @param[in] str   Dynamic string to remove character from.
/// @param     index Index of character to remove.
/// @return
///     - @c True  : @c index was in bounds of @c str and was removed.
///     - @c False : @c index was out of bounds.
bool CBPFN(dstring_remove)( CBPT(DString)* str, size_t index );
/// @brief Remove range of characters from dynamic string.
/// @details
/// With assertions on, asserts that from < to.
/// @param[in] str            Dynamic string to remove characters from.
/// @param     from_inclusive Start of range, inclusive.
/// @param     to_exclusive   End of range, exclusive. (can be == length).
/// @return
///     - @c True  : Range was in bounds of @c str and was removed.
///     - @c False : Range was out of bounds.
bool CBPFN(dstring_remove_range)( CBPT(DString)* str, size_t from_inclusive, size_t to_exclusive );
/// @brief Shorten dynamic string length to maximum.
/// @details
/// Does nothing if max > str.len.
/// Returns empty string if max == 0.
/// @param[in] str Dynamic string to truncate.
/// @param     max Maximum length to truncate to.
void CBPFN(dstring_truncate)( CBPT(DString)* str, size_t max );
/// @brief Subtract from dynamic string length.
/// @details
/// Does nothing if amount == 0. 
/// Returns empty string if amount >= str.len.
/// @param[in] str    Dynamic string to trim.
/// @param     amount Number of characters to trim from the end.
void CBPFN(dstring_trim)( CBPT(DString)* str, size_t amount );
/// @brief Set length of dynamic string to zero and zero out memory.
/// @note This does not free the string!
/// @param[in] str Dynamic string to clear.
void CBPFN(dstring_clear)( CBPT(DString)* str );
/// @brief Calculate remaining capacity in dynamic string.
/// @param[in] str Dynamic string.
/// @return Remaining capacity. Does not include null byte.
size_t CBPFN(dstring_remaining)( const CBPT(DString)* str );
/// @brief Get length of dynamic string.
/// @param[in] str Dynamic string.
/// @return Length not including null byte.
size_t CBPFN(dstring_len)( const CBPT(DString)* str );
/// @brief Get capacity of dynamic string.
/// @param[in] str Dynamic string.
/// @return Total capacity, includes null byte.
size_t CBPFN(dstring_cap)( const CBPT(DString)* str );
/// @brief Get total heap memory usage of dynamic string.
/// @param[in] str Dynamic string.
/// @return Total memory usage.
size_t CBPFN(dstring_total_size)( const CBPT(DString)* str );
/// @brief Check if dynamic string is empty.
/// @param[in] str Dynamic string.
/// @return
///     - @c True  : Length is zero.
///     - @c False : Length is not zero.
bool CBPFN(dstring_is_empty)( const CBPT(DString)* str );
/// @brief Check if dynamic string is full.
/// @param[in] str Dynamic string.
/// @return
///     - @c True  : Length equals capacity - 1.
///     - @c False : Dynamic string has remaining capacity.
bool CBPFN(dstring_is_full)( const CBPT(DString)* str );
/// @brief Set length of dstring.
/// @details
/// Panics if @c len exceeds string capacity.
///
/// This function also sets null-terminator.
/// @param[in] str Dynamic string.
/// @param     len Length to set.
void CBPFN(dstring_set_len)( CBPT(DString)* str, size_t len );
/// @brief Get mutable pointer to start of dynamic string buffer.
/// @details
/// Used internally.
/// Returns NULL if @c str is NULL.
/// @param[in] str Dynamic string.
/// @return Start of dynamic string.
void* CBPFN(dstring_head)( CBPT(DString)* str );
/// @brief Get pointer to start of dynamic string buffer.
/// @details
/// Used internally.
/// Returns NULL if @c str is NULL.
/// @param str Dynamic string.
/// @return Start of dynamic string.
const void* CBPFN(dstring_head_const)( const CBPT(DString)* str );
/// @brief Free a dynamic string.
/// @details
/// Use this function instead of heap_free() as string pointer
/// is not the pointer allocated from system.
/// @param[in] str Dynamic string.
void CBPFN(dstring_free)( CBPT(DString)* str );

/// @brief Allocate an empty dynamic array.
/// @param stride Size of items in dynamic array.
/// @param cap    Number of items dynamic array should be able to hold.
/// @return
///     - @c Pointer : Pointer to start of dynamic array.
///     - @c NULL    : Failed to allocate dynamic array.
void* darray_empty( size_t stride, size_t cap );
/// @brief Create dynamic array from existing array.
/// @param     stride Size of items in array.
/// @param     len    Length of array.
/// @param[in] buf    Pointer to start of array.
/// @return
///     - @c Pointer : Pointer to start of dynamic array.
///     - @c NULL    : Failed to allocate dynamic array.
void* darray_from_array( size_t stride, size_t len, const void* buf );
/// @brief Calculate memory requirement for dynamic array.
/// @details
/// Used to create dynamic array with fixed memory size.
/// @param stride Size of items in dynamic array.
/// @param cap    Number of items dynamic array should be able to hold.
/// @return Required size of dynamic array buffer.
size_t darray_static_memory_requirement( size_t stride, size_t cap );
/// @brief Create a dynamic array from static memory. (non reallocating)
/// @note
/// Returned pointer is not the same as @c buf!
/// Do not free this array with darray_free(),
/// use result of darray_head() or @c buf to free.
/// @param     stride Size of items in dynamic array.
/// @param     cap    Number of items dynamic array can hold.
/// @param[in] buf    Start of buffer. Must be able to hold result from darray_static_memory_requirement().
/// @return Start of dynamic array buffer.
void* darray_static( size_t stride, size_t cap, void* buf );
/// @brief Create a dynamic array from item literals.
/// @param type (type) Type of items.
/// @param ...  (any of type @c type) Items to fill array with.
/// @return
///     - @c Pointer : Pointer to start of dynamic array.
///     - @c NULL    : Failed to allocate dynamic array.
#define darray_literal( type, ... )\
    darray_from_array( sizeof(type),\
        sizeof((type[]){ __VA_ARGS__ }) / sizeof(type), (type[]){ __VA_ARGS__ } )
/// @brief Concatenate two arrays into one darray.
/// @note
/// Arrays provided should hold the same type of data.
/// @param     stride  Size of items in @c lhs and @c rhs.
/// @param     lhs_len Length of @c lhs.
/// @param[in] lhs     Pointer to start of left hand array.
/// @param     rhs_len Length of @c rhs.
/// @param[in] rhs     Pointer to start of right hand array.
/// @return
///     - @c Pointer : Pointer to start of dynamic array.
///     - @c NULL    : Failed to allocate dynamic array.
void* darray_join( size_t stride,
    size_t lhs_len, const void* lhs, size_t rhs_len, const void* rhs );
/// @brief Create dynamic array from filtered array.
/// @param     stride        Size of items in @c src.
/// @param     len           Length of @c src.
/// @param[in] src           Pointer to start of array to be filtered.
/// @param[in] filter        Pointer to filter function.
/// @param[in] filter_params (optional) Parameters to filter function.
/// @return
///     - @c Pointer : Pointer to start of dynamic array.
///     - @c NULL    : Failed to allocate dynamic array.
void* darray_from_filter(
    size_t stride, size_t len, const void* src,
    CBPT(DarrayFilterFN)* filter, void* filter_params );
/// @brief Grow dynamic array by @c amount number of items.
/// @param[in] darray Dynamic array to grow.
/// @param     amount Number of items to grow by.
/// @return
///     - @c Pointer : Reallocated dynamic array capable of holding cap + @c amount.
///     - @c NULL    : Failed to reallocate dynamic array.
void* darray_grow( void* darray, size_t amount );
/// @brief Create new dynamic array from existing dynamic array.
/// @param[in] darray Dynamic array to clone.
/// @return
///     - @c Pointer : Pointer to start of dynamic array.
///     - @c NULL    : Failed to allocate dynamic array.
void* darray_clone( const void* darray );
/// @brief Set dynamic array length to zero and zero out existing items.
/// @param[in] darray Dynamic array to clear.
void darray_clear( void* darray );
/// @brief Set length of dynamic array.
/// @details
/// Fallible:
/// - If @c len is > darray.cap, reallocates dynamic array.
///
/// Guaranteed to succeed:
/// - If @c len is > darray.len and < darray.cap, sets new length and zeroes new items.
/// - If @c len < darray.len, same as calling darray_truncate().
/// @param[in] darray Dynamic array.
/// @param     len    New length.
/// @return
///     - @c Pointer : Pointer to dynamic array.
///     - @c NULL    : Failed to reallocate dynamic array.
void* darray_set_len( void* darray, size_t len );
/// @brief Set maximum length of dynamic array.
/// @details
/// If @c max >= darray.len, returns without modifying array.
/// Otherwise, truncates length and zeroes out max to length.
/// @param[in] darray Dynamic array to modify.
/// @param     max    Maximum length of dynamic array.
void darray_truncate( void* darray, size_t max );
/// @brief Subtract from dynamic array length.
/// @details
/// If @c amount is >= darray.len, same as clearing darray.
/// @param[in] darray Dynamic array to modify.
/// @param     amount Number of items to substract from length.
void darray_trim( void* darray, size_t amount );
/// @brief Attempt to push new item to end of dynamic array.
/// @param[in] darray Dynamic array.
/// @param[in] item   Pointer to item to push.
/// @return
///     - @c True  : Had capacity to push new item.
///     - @c False : Dynamic array is full.
bool darray_try_push( void* darray, const void* item );
/// @brief Attempt to emplace new item inside of dynamic array.
/// @param[in] darray Dynamic array.
/// @param[in] item   Pointer to item to emplace.
/// @param     at     Index to emplace at.
/// @return
///     - @c True  : Had capacity to emplace new item.
///     - @c False : Dynamic array is full.
bool darray_try_emplace( void* darray, const void* item, size_t at );
/// @brief Attempt to append array to end of dynamic array.
/// @param[in] darray Dynamic array.
/// @param     count  Number of items in @c items.
/// @param[in] items  Pointer to start of array to append.
/// @return
///     - @c True  : Had capacity to append array.
///     - @c False : Dynamic array is full.
bool darray_try_append( void* darray, size_t count, const void* items );
/// @brief Attempt to insert array inside of dynamic array.
/// @param[in] darray Dynamic array.
/// @param     count  Number of items in @c items.
/// @param[in] items  Pointer to start of array to insert.
/// @param     at     Index to insert at.
/// @return
///     - @c True  : Had capacity to insert array.
///     - @c False : Dynamic array is full.
bool darray_try_insert( void* darray, size_t count, const void* items, size_t at );
/// @brief Pop last item from dynamic array.
/// @param[in]  darray       Dynamic array to pop from.
/// @param[out] opt_out_item (optional) Pointer to write last item to.
/// @return
///     - @c True  : Popped last item.
///     - @c False : Dynamic array is empty.
bool darray_pop( void* darray, void* opt_out_item );
/// @brief Push new item to end of dynamic array.
/// @param[in] darray Dynamic array.
/// @param[in] item   Pointer to item to push.
/// @return
///     - @c Pointer : Pushed successfully.
///     - @c NULL    : Failed to reallocate dynamic array.
void* darray_push( void* darray, const void* item );
/// @brief Emplace new item inside of dynamic array.
/// @param[in] darray Dynamic array.
/// @param[in] item   Pointer to item to emplace.
/// @param     at     Index to emplace at.
/// @return
///     - @c Pointer : Emplaced successfully.
///     - @c NULL    : Failed to reallocate dynamic array.
void* darray_emplace( void* darray, const void* item, size_t at );
/// @brief Append array to end of dynamic array.
/// @param[in] darray Dynamic array.
/// @param     count  Number of items in @c items.
/// @param[in] items  Pointer to start of array to append.
/// @return
///     - @c Pointer : Appended successfully.
///     - @c NULL    : Failed to reallocate dynamic array.
void* darray_append( void* darray, size_t count, const void* items );
/// @brief Insert array inside of dynamic array.
/// @param[in] darray Dynamic array.
/// @param     count  Number of items in @c items.
/// @param[in] items  Pointer to start of array to insert.
/// @param     at     Index to insert at.
/// @return
///     - @c Pointer : Inserted successfully.
///     - @c NULL    : Failed to reallocate dynamic array.
void* darray_insert( void* darray, size_t count, const void* items, size_t at );
/// @brief Remove item from dynamic array.
/// @param[in] darray Dynamic array.
/// @param     index  Index of item to remove.
/// @return
///     - @c True  : Removed item.
///     - @c False : Dynamic array was empty.
bool darray_remove( void* darray, size_t index );
/// @brief Remove range of items from dynamic array.
/// @param[in] darray Dynamic array.
/// @param     from_inclusive Start of range, inclusive.
/// @param     to_exclusive   End of range, exclusive. (can be == length).
/// @return
///     - @c True  : Range was in bounds of @c darray and was removed.
///     - @c False : Range was out of bounds.
bool darray_remove_range( void* darray, size_t from_inclusive, size_t to_exclusive );
/// @brief Calculate remaining capacity in dynamic array.
/// @param[in] darray Dynamic array.
/// @return Remaining capacity.
size_t darray_remaining( const void* darray );
/// @brief Get length of dynamic array.
/// @param[in] darray Dynamic array.
/// @return Length of dynamic array.
size_t darray_len( const void* darray );
/// @brief Get capacity of dynamic array.
/// @param[in] darray Dynamic array.
/// @return Capacity of dynamic array.
size_t darray_cap( const void* darray );
/// @brief Get size of items in dynamic array.
/// @param[in] darray Dynamic array.
/// @return Size of items in bytes.
size_t darray_stride( const void* darray );
/// @brief Get total heap memory usage of dynamic array.
/// @param[in] darray Dynamic array.
/// @return Total memory usage.
size_t darray_total_size( const void* darray );
/// @brief Check if dynamic array is empty.
/// @param[in] darray Dynamic array.
/// @return
///     - @c True  : Dynamic array is empty.
///     - @c False : Dynamic array has items in it.
bool darray_is_empty( const void* darray );
/// @brief Check if dynamic array is full.
/// @param[in] darray Dynamic array.
/// @return
///     - @c True  : Dynamic array is full.
///     - @c False : Dynamic array has capacity available.
bool darray_is_full( const void* darray );
/// @brief Get mutable pointer to start of dynamic array buffer.
/// @details
/// Used internally.
/// Returns NULL if @c darray is NULL.
/// @param[in] darray Dynamic array.
/// @return Start of dynamic array buffer.
void* darray_head( void* darray );
/// @brief Get pointer to start of dynamic array buffer.
/// @details
/// Used internally.
/// Returns NULL if @c darray is NULL.
/// @param[in] darray Dynamic array.
/// @return Start of dynamic array buffer.
const void* darray_head_const( const void* darray );
/// @brief Free a dynamic array.
/// @details
/// Use this function instead of heap_free().
/// @param[in] darray Dynamic array.
void darray_free( void* darray );

/// @brief Get read-only string containing current working directory.
/// @return Read-only string containing current working directory.
CBPT(String) path_cwd(void);
/// @brief Get read-only string containing home directory.
/// @return Read-only string containing home directory.
CBPT(String) path_home(void);
/// @brief Check if path is absolute.
/// @param[in] path Path to check.
/// @return
///     - @c True  : @c path is an absolute path.
///     - @c False : @c path is a relative path.
bool path_is_absolute( const char* path );
/// @brief Check if path points to a file/directory.
/// @details
/// Untested: if it returns false for files that require elevated permissions.
/// @param[in] path Path to check.
/// @return
///     - @c True  : @c path points to real file/directory.
///     - @c False : @c path does not point to file/directory.
bool path_exists( const char* path );
/// @brief Check if path points to directory.
/// @param[in] path Path to check.
/// @return
///     - @c True  : @c path points to existing directory.
///     - @c False : @c path points to something else.
///     - @c False : @c path does not point to existing directory.
bool path_is_directory( const char* path );
/// @brief Count number of path chunks in path.
/// @param path Path to count.
/// @return Number of chunks.
size_t path_chunk_count( CBPT(String) path );
/// @brief Split path into chunks.
/// @param path Path to split.
/// @return
///     - Dynamic array of chunks. Free with darray_free()
///     - NULL : Failed to allocate chunks array.
CBPT(String)* path_chunk_split( CBPT(String) path );
/// @brief Check if path matches glob pattern.
/// @details
/// This is a very simple glob pattern.
/// It only supports:
///
/// - * : One or more any characters.
/// - ? : One of any character.
/// @param path Path to check.
/// @param glob Glob pattern.
/// @return
///     - @c True  : @c path matches @c glob.
///     - @c False : @c path does not match @c glob.
bool path_matches_glob( CBPT(String) path, CBPT(String) glob );
/// @brief Walk a directory, collecting all files/directories.
/// @details
/// If @c out_result is empty, allocates new buffers for it.
/// Otherwise, appends results to end of @c out_result.
/// @param[in]      dir          Path to directory to walk.
/// @param          recursive    If function should keep searching in subdirectories.
/// @param          include_dirs If function should include directory names in final list.
/// @param[in, out] out_result   Result of directory walk.
/// @return
///     - @c True  : Successfully walked directory and wrote results to @c out_result.
///     - @c False : Failed to open @c dir.
///     - @c False : Failed to allocate results.
bool path_walk_dir(
    const char* dir, bool recursive,
    bool include_dirs, WalkDirectory* out_result );
/// @brief Create list of filtered paths from path_walk_dir() result.
/// @param[in] wd   path_walk_dir() result to filter.
/// @param     glob Glob pattern to check for.
/// @return
///     - Dynamic array of paths that match pattern.
///     - NULL : Failed to allocate result.
CB_DARRAY(CBPT(String)) path_walk_glob_filter( const WalkDirectory* wd, CBPT(String) glob );
/// @brief Free result of path_walk_dir().
/// @param[in] wd Walk directory result to free.
void path_walk_free( WalkDirectory* wd );

/// @brief Create a null file descriptor.
/// @return Null file descriptor.
#define file_null() (0)
/// @brief Open a file.
/// @param[in]  path     Path of file to open. Length must be <= 4096.
/// @param      flags    Flags to open with.
/// @param[out] out_file File descriptor if successful.
/// @return
///     - @c True  : File was opened successfully.
///     - @c False : Failed to open file. Check log for more details.
bool fd_open( const char* path, CBPT(FileOpenFlag) flags, CBPT(FileID)* out_file );
/// @brief Close a file descriptor.
/// @param[in] file Pointer to file descriptor to close.
void fd_close( CBPT(FileID)* file );
/// @brief Write to file.
/// @param[in]  file               File descriptor to write to.
/// @param      size               Size of buffer to write.
/// @param[in]  buf                Pointer to buffer to write.
/// @param[out] opt_out_write_size (optional) Pointer to write number of bytes written.
/// @return
///     - @c True  : Successfully wrote to file.
///     - @c False : Failed to write file.
bool fd_write( CBPT(FileID)* file, size_t size, const void* buf, size_t* opt_out_write_size );
/// @brief Write formatted string to file.
/// @param[in] file   File descriptor to write to.
/// @param[in] format Format string literal.
/// @param     va     Variadic list of format arguments.
/// @return
///     - @c True  : Successfully wrote to file.
///     - @c False : Failed to write file.
bool fd_write_fmt_va( CBPT(FileID)* file, const char* format, va_list va );
/// @brief Write formatted string to file.
/// @param[in] file   File descriptor to write to.
/// @param[in] format Format string literal.
/// @param     ...    Format arguments.
/// @return
///     - @c True  : Successfully wrote to file.
///     - @c False : Failed to write file.
bool fd_write_fmt( CBPT(FileID)* file, const char* format, ... );
/// @brief Read bytes from file.
/// @param[in]  file              File descriptor to read from.
/// @param      size              Size of read buffer.
/// @param[out] buf               Pointer to buffer.
/// @param[out] opt_out_read_size (optional) Pointer to write number of bytes read.
/// @return
///     - @c True  : Successfully read from file.
///     - @c False : Failed to read file.
bool fd_read( CBPT(FileID)* file, size_t size, void* buf, size_t* opt_out_read_size );
/// @brief Set file size to current position.
/// @param[in] file File descriptor.
/// @return
///     - @c True  : Successfully set file size.
///     - @c False : Failed to set file size.
bool fd_truncate( CBPT(FileID)* file );
/// @brief Query file size.
/// @param[in] file File descriptor.
/// @return Size of @c file.
size_t fd_query_size( CBPT(FileID)* file );
/// @brief Seek to file position.
/// @param[in] file File descriptor.
/// @param     type Type of seek.
/// @param     seek Bytes to seek. Can be negative to seek in reverse.
void fd_seek( CBPT(FileID)* file, CBPT(SeekType) type, intptr_t seek );
/// @brief Query current seek position.
/// @param[in] file File descriptor.
/// @return Bytes from start of file.
size_t fd_query_position( CBPT(FileID)* file );
/// @brief Query creation time of file at given path.
/// @param[in] path Null-terminated path to file. Length must be <= 4096.
/// @return Creation time in POSIX time.
time_t file_query_time_create( const char* path );
/// @brief Query last time modified of file at given path.
/// @param[in] path Null-terminated path to file. Length must be <= 4096.
/// @return Last time modified in POSIX time.
time_t file_query_time_modify( const char* path );
/// @brief Move file from one path to another.
/// @param[in] dst Null-terminated destination path. Length must be <= 4096.
/// @param[in] src Null-terminated source path. Length must be <= 4096.
/// @return
///     - @c True  : Successfully moved file.
///     - @c False : Failed to move file.
bool file_move( const char* dst, const char* src );
/// @brief Copy file from one path to another.
/// @param[in] dst Null-terminated destination path. Length must be <= 4096.
/// @param[in] src Null-terminated source path. Length must be <= 4096.
/// @return
///     - @c True  : Successfully copied file.
///     - @c False : Failed to copie file.
bool file_copy( const char* dst, const char* src );
/// @brief Remove file from system.
/// @param[in] path Null-terminated path. Length must be <= 4096.
/// @return
///     - @c True  : Successfully removed file.
///     - @c False : Failed to remove file.
bool file_remove( const char* path );
/// @brief Create directory.
/// @param[in] path Path to create directory at.
/// @return
///     - @c True  : Directory created successfully or it already exists.
///     - @c False : Failed to create directory.
bool dir_create( const char* path );
/// @brief Remove directory.
/// @param[in] path      Path to directory to remove.
/// @param     recursive If directory contains items.
/// @return
///     - @c True  : Directory removed successfully.
///     - @c False : Failed to remove directory.
///     - @c False : Attempted to remove directory that is not empty without @c recursive.
bool dir_remove( const char* path, bool recursive );

/// @brief Atomically add to atomic integer.
/// @param[in, out] atomic Pointer to atomic to add to.
/// @param          val    Value to add to @c atomic.
/// @return Value of @c atomic before add.
atom   atomic_add( atom* atomic, atom val );
/// @brief Atomically add to atomic integer.
/// @param[in, out] atom Pointer to atomic to add to.
/// @param          val    Value to add to @c atom.
/// @return Value of @c atom before add.
atom64 atomic_add64( atom64* atom, atom64 val );
/// @brief Compare atomic to value and if they match, exchange with new value.
/// @param[in, out] atomic Pointer to atomic to modify.
/// @param          comp   Value to compare agains @c atomic.
/// @param          exch   Value to exchange with @c atomic if comparison succeeds.
/// @return Value of @c atom before exchange.
atom   atomic_compare_swap( atom* atomic, atom comp, atom exch );
/// @brief Compare atomic to value and if they match, exchange with new value.
/// @param[in, out] atom Pointer to atomic to modify.
/// @param          comp Value to compare agains @c atom.
/// @param          exch Value to exchange with @c atom if comparison succeeds.
/// @return Value of @c atom before exchange.
atom64 atomic_compare_swap64( atom64* atom, atom64 comp, atom64 exch );
/// @brief Insert a full memory barrier.
/// @details
/// Inserts a compiler and instruction memory barrier to ensure memory ordering.
void fence(void);

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    /// @brief Create an unintialized mutex.
    /// @return Unintialized mutex.
    #define mutex_null() {0}
#else
    /// @brief Create an unintialized mutex.
    /// @return Unintialized mutex.
    #define mutex_null() (Mutex){ .handle=NULL }
#endif
/// @brief Create a mutex.
/// @param[out] out_mutex Pointer to write new mutex to.
/// @return
///     - @c True  : Created mutex successfully.
///     - @c False : Failed to create mutex (Windows only).
bool mutex_create( Mutex* out_mutex );
/// @brief Check if mutex was initialized.
/// @param[in] mutex Pointer to mutex to check.
/// @return
///     - @c True : Mutex was initialized.
///     - @c False : Mutex has not yet been initialized (Windows only).
bool mutex_is_valid( const Mutex* mutex );
/// @brief Lock mutex.
/// @details
/// Waits indefinitely for mutex lock.
/// @param[in] mutex Mutex to lock.
void mutex_lock( Mutex* mutex );
/// @brief Lock mutex, wait for only a limited time.
/// @param[in] mutex Mutex to lock.
/// @param     ms    Maximum milliseconds to wait for lock.
/// @return
///     - @c True  : Locked mutex before timeout.
///     - @c False : Timed out.
bool mutex_lock_timed( Mutex* mutex, uint32_t ms );
/// @brief Unlock locked mutex. Does nothing if mutex is already unlocked.
/// @param[in] mutex Mutex to unlock.
void mutex_unlock( Mutex* mutex );
/// @brief Destroy mutex.
/// @param[in] mutex Mutex to destroy.
void mutex_destroy( Mutex* mutex );

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    /// @brief Create an uninitialized semaphore.
    /// @return Unintialized semaphore.
    #define semaphore_null() {0}
#else
    /// @brief Create an uninitialized semaphore.
    /// @return Unintialized semaphore.
    #define semaphore_null() (Semaphore){ .handle=NULL }
#endif
/// @brief Create a semaphore.
/// @param[out] out_semaphore Pointer to write semaphore to.
/// @return
///     - @c True  : Created semaphore successfully.
///     - @c False : Failed to create semaphore.
bool semaphore_create( Semaphore* out_semaphore );
/// @brief Check if semaphore has already been initialized.
/// @param[in] semaphore Semaphore to check.
/// @return
///     - @c True  : Semaphore is initialized.
///     - @c False : Semaphore has not been initialized.
bool semaphore_is_valid( const Semaphore* semaphore );
/// @brief Wait indefinitely for semaphore to be signaled.
/// @param[in] semaphore Semaphore to wait for.
void semaphore_wait( Semaphore* semaphore );
/// @brief Wait for semaphore to be signaled.
/// @param[in] semaphore Semaphore to wait for.
/// @param     ms        Maximum milliseconds to wait for signal.
/// @return
///     - @c True  : Semaphore was signaled in time.
///     - @c False : Timed out.
bool semaphore_wait_timed( Semaphore* semaphore, uint32_t ms );
/// @brief Signal a semaphore.
/// @param[in] semaphore Semaphore to signal.
void semaphore_signal( Semaphore* semaphore );
/// @brief Destroy a semaphore.
/// @param[in] semaphore Semaphore to destroy.
void semaphore_destroy( Semaphore* semaphore );
/// @brief Sleep the current thread for given milliseconds.
/// @param ms Milliseconds to sleep for.
void thread_sleep( uint32_t ms );

/// @brief Enqueue a new job.
/// @param[in] job    Pointer to job function to enqueue.
/// @param[in] params (optional) Parameters for job function.
/// @return
///     - @c True  : Was able to enqueue job.
///     - @c False : Job queue is full, use job_enqueue_timed() instead to wait for empty spot.
bool job_enqueue( JobFN* job, void* params );
/// @brief Wait for job queue to enqueue.
/// @param[in] job    Pointer to job function to enqueue.
/// @param[in] params (optional) Parameters for job function.
/// @param     ms     Maximum milliseconds to wait, use MT_WAIT_INFINITE to wait indefinitely.
/// @return
///     - @c True  : Was able to enqueue in time.
///     - @c False : Timed out.
bool job_enqueue_timed( JobFN* job, void* params, uint32_t ms );
/// @brief Wait for next job to complete.
/// @param ms Milliseconds to wait for, use MT_WAIT_INFINITE to wait indefinitely.
/// @return
///      - @c True  : Next job completed in time or queue was empty.
///      - @c False : Timed out.
bool job_wait_next( uint32_t ms );
/// @brief Wait for all jobs to complete.
/// @param ms Milliseconds to wait for, use MT_WAIT_INFINITE to wait indefinitely.
/// @return
///      - @c True  : Jobs completed in time or queue was empty.
///      - @c False : Timed out.
bool job_wait_all( uint32_t ms );

/// @brief Get the current thread's monotonic id.
/// @return Current thread id. (0 is main thread)
uint32_t thread_id(void);

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    /// @brief Create empty command.
    /// @return Empty command.
    #define command_null() {0}
#else
    /// @brief Create empty command.
    /// @return Empty command.
    #define command_null() (CBPT(Command)){ .args=0, .count=0 }
#endif
/// @brief Create new command.
/// @param ... Null-terminated strings of arguments. First argument must be path to process to execute.
/// @return New command.
#define command_new( ... )\
    (CBPT(Command)){ .args=(const char*[]){ __VA_ARGS__, 0 },\
        .count=sizeof((const char*[]){ __VA_ARGS__ }) / sizeof(const char*) }
/// @brief Convert command to dynamic string.
/// @param[in] command Command to convert.
/// @return
///     - Dynamic string, free with dstring_free().
///     - NULL : Failed to allocate result.
CBPT(DString)* command_flatten_dstring( const CBPT(Command)* command );

/// @brief Create command builder.
/// @param[in]  exe         Name of executable.
/// @param[out] out_builder Pointer to write result to.
/// @return
///     - true  : Allocated new command builder.
///     - false : Failed to allocate new command builder.
bool command_builder_new( const char* exe, CBPT(CommandBuilder)* out_builder );
/// @brief Clear command builder.
/// @param[in] builder Builder to clear.
void command_builder_clear( CBPT(CommandBuilder)* builder );
/// @brief Push argument to end of command builder.
/// @param[in] builder Builder to push to.
/// @param[in] arg     Argument to push.
/// @return
///     - true  : Pushed argument successfully.
///     - false : Failed to reallocate command builder.
bool command_builder_push( CBPT(CommandBuilder)* builder, const char* arg );
bool __internal_command_builder_append( CBPT(CommandBuilder)* builder, const char* first, ... );
/// @brief Append arguments to end of command builder.
/// @param[in] builder Builder to append to.
/// @param     args... Arguments to append.
/// @return
///     - true  : Appended arguments successfully.
///     - false : Failed to reallocate command builder.
#define command_builder_append( builder, ... )\
    __internal_command_builder_append( builder, __VA_ARGS__, NULL )
/// @brief Append list of arguments to end of command builder.
/// @param[in] builder Builder to append to.
/// @param     count   Number of arguments to append.
/// @param[in] args    Pointer to list of arguments to append.
/// @return
///     - true  : Appended arguments successfully.
///     - false : Failed to reallocate command builder.
bool command_builder_append_list(
    CBPT(CommandBuilder)* builder, size_t count, const char** args );
/// @brief Convert builder to command.
/// @param[in] builder Builder to convert.
/// @return Command.
CBPT(Command) command_builder_cmd( CBPT(CommandBuilder)* builder );
/// @brief Free command builder.
/// @param[in] builder Builder to free.
void command_builder_free( CBPT(CommandBuilder)* builder );

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    /// @brief Create null pipe.
    /// @return Null pipe.
    #define pipe_null() (0)
#else
    /// @brief Create null pipe.
    /// @return Null pipe.
    #define pipe_null() (-1)
#endif
/// @brief Open read and write pipes.
/// @param[out] out_read  Pointer to write read end of pipe to.
/// @param[out] out_write Pointer to write write end of pipe to.
void pipe_open( CBPT(PipeRead)* out_read, CBPT(PipeWrite)* out_write );
/// @brief Read from read end of pipe.
/// @param[in]  read_pipe         Pointer to read end of pipe.
/// @param      size              Size of buffer to read to.
/// @param[in]  buf               Buffer to write read result to.
/// @param[out] opt_out_read_size (optional) Pointer to write number of bytes read.
/// @return
///     - @c True  : Successfully read from pipe.
///     - @c False : Failed to read from pipe.
#define pipe_read( read_pipe, size, buf, opt_out_read_size )\
    fd_read( read_pipe, size, buf, opt_out_read_size )
/// @brief Write to write end of pipe.
/// @param[in]  write_pipe         Pointer to write end of pipe.
/// @param      size               Size of buffer to write.
/// @param[in]  buf                Buffer to write to pipe.
/// @param[out] opt_out_write_size (optional) Pointer to write number of bytes written.
/// @return
///     - @c True  : Successfully wrote to pipe.
///     - @c False : Failed to write to pipe.
#define pipe_write( write_pipe, size, buf, opt_out_write_size )\
    fd_write( write_pipe, size, buf, opt_out_write_size )
/// @brief Close pipe.
/// @param pipe Pipe to close (CBPT(PipeRead) or CBPT(PipeWrite)).
void pipe_close( CBPT(Pipe) pipe );

/// @brief Create null process ID.
/// @return Null process ID.
#define pid_null() (0)
/// @brief Check if process is in path.
/// @param[in] process_name Name of process to check for.
/// @return
///     - @c True  : Process is in path.
///     - @c False : Process is not in path.
bool process_in_path( const char* process_name );
/// @brief Execute a process command.
/// @param cmd Command to execute.
/// @param redirect_void If output should just be redirected to void.
/// @param[in] opt_stdin  (optional) Pointer to read stdin pipe.
/// @param[in] opt_stdout (optional) Pointer to write stdout pipe.
/// @param[in] opt_stderr (optional) Pointer to write stderr pipe.
/// @param[in] opt_cwd    (optional) Change current working directory for command.
/// @return ID of process executed.
/// Process ID must be discarded using process_discard(),
/// process_wait() or successful process_wait_timed().
CBPT(ProcessID) process_exec(
    CBPT(Command) cmd, bool redirect_void, CBPT(PipeRead)* opt_stdin,
    CBPT(PipeWrite)* opt_stdout, CBPT(PipeWrite)* opt_stderr, const char* opt_cwd );
/// @brief Wait indefinitely for process to complete.
/// @details
/// This function discards @c pid.
/// @param pid ID of process to wait for.
/// @return
///     - >= 0 : Process exited normally, return code from process.
///     - <  0 : Process did not exit normally.
int process_wait( CBPT(ProcessID) pid );
/// @brief Wait for process to complete.
/// @details
/// If return code is >= 0 that means process exited normally.
/// If return code is < 0 that means process did not exit normally.
/// If process completes in time, this function discards @c pid.
/// @param      pid         ID of process to wait for.
/// @param[out] opt_out_res (optional) Pointer to write return code to.
/// @param      ms          Max milliseconds to wait for.
/// @return
///     - @c True  : Process completed in time.
///     - @c False : Timed out.
bool process_wait_timed( CBPT(ProcessID) pid, int* opt_out_res, uint32_t ms );
/// @brief Discard process ID.
/// @details
/// While not necessary on POSIX, pid should always
/// be discard with process_wait(), proces_wait_timed() or this function.
/// @param pid Process ID to discard.
void process_discard( CBPT(ProcessID) pid );

/// @brief Get current time in milliseconds.
/// @return Time in milliseconds.
double timer_milliseconds(void);
/// @brief Get current time in seconds.
/// @return Time in seconds.
double timer_seconds(void);

/// @brief Get pointer to next local byte buffer.
/// @details
/// cbuild allocates CB_LOCAL_BUFFER_COUNT * CB_THREAD_COUNT
/// number of buffers of size CB_LOCAL_BUFFER_CAPACITY
/// when either CB_INIT() or this function is first called.
/// Local buffers should not be freed.
/// Local buffers should be used immediately to prevent
/// other functions from overwriting the currently obtained
/// local buffer.
/// @return Pointer to next local byte buffer.
uint8_t* local_byte_buffer(void);
/// @brief Write formatted string to local buffer.
/// @details
/// cbuild allocates CB_LOCAL_BUFFER_COUNT * CB_THREAD_COUNT
/// number of buffers of size CB_LOCAL_BUFFER_CAPACITY
/// when either CB_INIT() or this function is first called.
/// Local buffers should not be freed.
/// Local buffers should be used immediately to prevent
/// other functions from overwriting the currently obtained
/// local buffer.
/// @param[in] format Format string literal.
/// @param     va     Variadic list of format arguments.
/// @return Pointer to start of formatted local buffer.
char* local_fmt_va( const char* format, va_list va );
/// @brief Write formatted string to local buffer.
/// @details
/// cbuild allocates CB_LOCAL_BUFFER_COUNT * CB_THREAD_COUNT
/// number of buffers of size CB_LOCAL_BUFFER_CAPACITY
/// when either CB_INIT() or this function is first called.
/// Local buffers should not be freed.
/// Local buffers should be used immediately to prevent
/// other functions from overwriting the currently obtained
/// local buffer.
/// @param[in] format Format string literal.
/// @param     ...    Format arguments.
/// @return Pointer to start of formatted local buffer.
char* local_fmt( const char* format, ... );

/// @brief Set logging level.
/// @warning
/// This function is not MT-safe so only
/// call it before using jobs system.
/// @param level Level to set logger to.
void logger_set_level( CBPT(LogLevel) level );
/// @brief Get current logging level.
/// @return Current logging level.
CBPT(LogLevel) logger_get_level(void);
/// @brief Write formatted logging message.
/// @param     level  Logger level of message.
/// @param[in] format Format string literal.
/// @param     va     Variadic list of format arguments.
void logger_va( CBPT(LogLevel) level, const char* format, va_list va );
/// @brief Write formatted logging message.
/// @param     level  Logger level of message.
/// @param[in] format Format string literal.
/// @param     ...    Format arguments.
void logger( CBPT(LogLevel) level, const char* format, ... );

/// @brief Query name of compiler used to compile this instance of cbuild.
/// @details
/// This name corresponds to the command used to compiler cbuild.
/// @return String containing name of compiler.
CBPT(String) cbuild_query_compiler(void);
/// @brief Query command line arguments used in this instance of cbuild.
/// @return Pointer to command.
const CBPT(Command)* cbuild_query_command_line(void);

void _init_(
    CBPT(LogLevel) logger_level,
    const char* executable_name,
    const char* source_name,
    int argc, const char** argv );

static inline void _INTERNAL_CB_UNUSED_FUNCTION( int _, ... ) {(void)_;}

#endif /* header guard */

#if defined(CB_IMPLEMENTATION)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    #include <process.h>
#else
    void posix_process_replace( CBPT(Command) cmd );
#endif

struct GlobalBuffers {
    atom obtained; // atomic boolean

    CBPT(String) cwd;
    CBPT(String) home;

    CBPT(PipeRead)  void_read;
    CBPT(PipeWrite) void_write;
};
struct LocalBuffers {
    char buffers[CB_THREAD_COUNT + 1]
        [CB_LOCAL_BUFFER_COUNT][CB_LOCAL_BUFFER_CAPACITY];
    atom indices[CB_THREAD_COUNT + 1];
};
struct DynamicString {
    size_t len, cap;
    char  buf[];
};
struct DynamicArray {
    size_t len, cap, stride;
    uint8_t buf[];
};

struct JobEntry {
    JobFN* proc;
    void*  params;
};
struct JobQueue {
    Semaphore wakeup;
    atom front, back;
    atom pending, len;
    struct JobEntry entries[CB_MAX_JOBS];
};

volatile struct JobQueue* global_queue = NULL;

atom   global_is_mt              = false; // boolean
atom64 global_memory_usage       = 0;
atom64 global_total_memory_usage = 0;

atom            global_thread_id_source = 1; // 0 is main thread
CB_TLS( uint32_t ) global_thread_id        = 0;

static Mutex       global_logger_mutex = mutex_null();
static CBPT(LogLevel) global_logger_level = CB_LOG_LEVEL_INFO;

static CBPT(Command) global_command_line;

volatile struct LocalBuffers* global_local_buffers = NULL;
#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    volatile struct GlobalBuffers global_buffers = {0};
#else
    volatile struct GlobalBuffers global_buffers =
        (struct GlobalBuffers){ .obtained=false };
#endif

void  thread_create( JobFN* func, void* params );
char* internal_cwd(void);
char* internal_home(void);
static bool path_walk_dir_internal(
    CBPT(DString)** path, bool recursive, bool include_dirs,
    size_t* out_count, CBPT(DString)** out_buffer );

static bool job_dequeue( struct JobQueue* queue, struct JobEntry* out_entry ) {
    if( !queue->len ) {
        return false;
    }
    atomic_add( &queue->len, -1 );

    fence();
    atom front = atomic_add( &queue->front, 1 );
    fence();

    *out_entry = queue->entries[ (front + 1) % CB_MAX_JOBS ];
    return true;
}
void job_queue_proc( void* params ) {
    struct JobQueue* queue = params;
    fence();

    for( ;; ) {
        struct JobEntry entry;
        CBPFN(memory_zero)( &entry, sizeof(entry) );

        semaphore_wait( &queue->wakeup );
        fence();

        if( job_dequeue( queue, &entry ) ) {
            entry.proc( entry.params );
            fence();
            atomic_add( &queue->pending, -1 );
        }
    }
}
static void initialize_job_queue(void) {
    CB_INFO(
        "creating job queue with %u entries and %u threads . . .",
        CB_MAX_JOBS, CB_THREAD_COUNT );

    CB_EXPECT( mutex_create( &global_logger_mutex ), "failed to create logger mutex!" );
    atomic_add( &global_is_mt, 1 );

    size_t queue_size       = sizeof(*global_queue);
    struct JobQueue* queue = CBPFN(heap_alloc)( queue_size );

    CB_EXPECT( queue, "failed to allocate job queue!" );

    CB_EXPECT(
        semaphore_create( &queue->wakeup ),
        "failed to create job queue semaphore!" );
    queue->front = -1;

    fence();

    for( size_t i = 0; i < CB_THREAD_COUNT; ++i ) {
        thread_create( job_queue_proc, queue );
    }

    fence();
    global_queue = queue;
}
static volatile struct JobQueue* get_job_queue(void) {
    if( !global_queue ) {
        initialize_job_queue();
    }
    return global_queue;
}
static volatile struct LocalBuffers* get_local_buffers(void) {
    if( !global_local_buffers ) {
        global_local_buffers = CBPFN(heap_alloc)( sizeof(*global_local_buffers) );
        CB_EXPECT( global_local_buffers,
            "failed to allocate local buffers! size: %zu",
            sizeof(*global_local_buffers) );
    }
    return global_local_buffers;
}
static volatile char* get_next_local_buffer( uint32_t id ) {
    volatile struct LocalBuffers* b = get_local_buffers();

    uint32_t index = 0;
    if( global_is_mt ) {
        atom atomic = atomic_add( &b->indices[id], 1 );
        index = (*(uint32_t*)&atomic) % CB_LOCAL_BUFFER_COUNT;
    } else {
        atom atomic     = b->indices[id];
        b->indices[id] += 1;
        index = (*(uint32_t*)&atomic) % CB_LOCAL_BUFFER_COUNT;
    }

    volatile char* result = b->buffers[id][index];
    CBPFN(memory_zero)( (void*)result, CB_LOCAL_BUFFER_CAPACITY );
    return result;
}
static volatile struct GlobalBuffers* get_global_buffers(void) {
    if( !global_buffers.obtained ) {
        atomic_add( &global_buffers.obtained, 1 );
        
        char* cwd  = internal_cwd();
        char* home = internal_home();
        global_buffers.cwd  = string_from_cstr( cwd );
        global_buffers.home = string_from_cstr( home );

        pipe_open(
            (CBPT(PipeRead)*)&global_buffers.void_read,
            (CBPT(PipeWrite)*)&global_buffers.void_write );
    }
    return &global_buffers;
}
static bool validate_file_flags( CBPT(FileOpenFlag) flags ) {
    if( !( flags & CB_FILE_OPEN_READ | CB_FILE_OPEN_WRITE ) ) {
        CB_ERROR( "FD flags must have READ and/or WRITE set!" );
        return false;
    }
    if( flags & CB_FILE_OPEN_TRUNCATE ) {
        if( flags & CB_FILE_OPEN_APPEND ) {
            CB_ERROR( "FD flag APPEND and TRUNCATE cannot be set at the same time!" );
            return false;
        }
        if( flags & CB_FILE_OPEN_READ ) {
            CB_ERROR( "FD flag TRUNCATE and READ cannot be set at the same time!" );
            return false;
        }
    }

    return true;
}
static bool dir_remove_internal( const char* path );

#if !defined(CBUILD_COMPILER_NAME)
    #define CBUILD_COMPILER_NAME CB_COMPILER_CURRENT_C_CMD
#endif

#if !defined(CBUILD_COMPILER_OUTPUT_FLAG)
    #if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
        #define CBUILD_COMPILER_OUTPUT_FLAG "-Fe:"
    #else
        #define CBUILD_COMPILER_OUTPUT_FLAG "-o"
    #endif
#endif

#if !defined(CBUILD_POSIX_FLAGS) && CB_PLATFORM_IS_POSIX
    #define CBUILD_POSIX_FLAGS "-lpthread"
#endif

CBPT(String) cbuild_query_compiler(void) {
    return CB_STRING( CBUILD_COMPILER_NAME );
}
const CBPT(Command)* cbuild_query_command_line(void) {
    return &global_command_line;
}

void _platform_init_(void);
void _init_(
    CBPT(LogLevel) logger_level,
    const char* executable_name,
    const char* source_name,
    int argc, const char** argv
) {
    _platform_init_();
    logger_set_level( logger_level );

    const char** heap_args = (const char**)darray_from_array(
        sizeof(const char*), argc, argv
    ); {
        CB_EXPECT( heap_args, "failed to allocate arguments!" );
        const char* nul = 0;
        heap_args = darray_push( heap_args, &nul );
    }

    global_command_line.count = argc;
    global_command_line.args  = heap_args;

    (void)get_local_buffers();
    (void)get_global_buffers();

    CB_EXPECT( path_exists( __FILE__ ),
        "cbuild MUST be run from its source code directory!" );
    CB_EXPECT( path_exists( source_name ),
        "cbuild MUST be run from its source code directory!" );

    bool rebuild = false;
    if( path_exists( executable_name ) ) {
        time_t executable_modify = file_query_time_modify( executable_name );
        time_t source_modify     = file_query_time_modify( source_name );
        time_t header_modify     = file_query_time_modify( __FILE__ );

        double diff_source = difftime( executable_modify, source_modify );
        double diff_header = difftime( executable_modify, header_modify );

        rebuild = (diff_source < 0.0) || (diff_header < 0.0);
    } else {
        rebuild = true;
    }

    if( !rebuild ) {
        const char* old_name = local_fmt( "%s.old", executable_name );
        if( path_exists( old_name ) ) {
            file_remove( old_name );
        }
        return;
    }
    // rebuild

    CB_INFO( "changes detected in cbuild source, rebuilding . . ." );
    CBPFN(rebuild_self)( source_name, executable_name, true );
}
CB_NO_RETURN void CBPFN(rebuild_self)(
    const char* cbuild_source_file_name,
    const char* cbuild_executable_name,
    bool reload
) {

    double start = timer_milliseconds();

    #if !defined(CBUILD_ADDITIONAL_FLAGS)
        #define CBUILD_ADDITIONAL_FLAGS 0
    #endif
    #if !defined(CBUILD_POSIX_FLAGS)
        #define CBUILD_POSIX_FLAGS 0
    #endif

    const char* posix_flags[] = { CBUILD_POSIX_FLAGS };
    const char* additional[]  = { CBUILD_ADDITIONAL_FLAGS };
    size_t arg_count = 0;
    const char* args[6 + CB_ARRAY_LEN( additional ) + CB_ARRAY_LEN( posix_flags )];
    CBPFN(memory_zero)( (void*)args, sizeof(args) );

    args[arg_count++] = CBUILD_COMPILER_NAME;
    args[arg_count++] = cbuild_source_file_name;
    args[arg_count++] = CBUILD_COMPILER_OUTPUT_FLAG;
    args[arg_count++] = cbuild_executable_name;

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    args[arg_count++] = "-nologo";
#endif

    for( int i = 0; i < CB_ARRAY_LEN( additional ); ++i ) {
        const char* a = additional[i];
        if( a ) {
            args[arg_count++] = additional[i];
        }
    }
    for( int i = 0; i < CB_ARRAY_LEN( posix_flags ); ++i ) {
        const char* a = posix_flags[i];
        if( a ) {
            args[arg_count++] = posix_flags[i];
        }
    }

    CBPT(Command) rebuild_cmd;
    rebuild_cmd.count = arg_count;
    rebuild_cmd.args  = args;

    CB_INFO( "rebuilding with command:" );

    const char* old_name = local_fmt( "%s.old", cbuild_executable_name );
    if( path_exists( old_name ) ) {
        CB_EXPECT(
            file_remove( old_name ),
            "could not remove old executable!" );
    }

    CB_EXPECT(
        file_move( old_name, cbuild_executable_name ),
        "could not rename executable!" );

    fence();

    CBPT(ProcessID) pid = process_exec( rebuild_cmd, false, 0, 0, 0, 0 );
    int res = process_wait( pid );
    if( res != 0 ) {
        CB_FATAL( "failed to rebuild!" );
        file_move( cbuild_executable_name, old_name );

        exit(127);
    }

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    /* attempt to remove annoying .obj file generated by msvc */ {
        CBPT(String) exe = string_from_cstr( cbuild_executable_name );
        char* local = (char*)local_byte_buffer();
        CBPFN(memory_copy)( local, exe.cc, exe.len );
        size_t dot = 0;
        if( CBPFN(string_find_rev)( exe, '.', &dot ) ) {
            local[++dot] = 'o';
            local[++dot] = 'b';
            local[++dot] = 'j';
            if( path_exists( local ) ) {
                file_remove( local );
            }
        }
    }
#endif

    double end = timer_milliseconds();
    CB_INFO( "rebuilt in %fms", end - start );

    if( !reload ) {
        exit(0);
    }

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    printf(
        "\033[1;33m"
        "[W:00] cbuild: "
        "windows does not support automatically reloading cbuild, "
        "please run it again."
        "\033[1;00m\n" );
    exit(0);
#else

    posix_process_replace( global_command_line );
    // execvp( global_command_line.args[0], (char* const*)(global_command_line.args) );
    // process_exec( global_command_line, false, 0, 0, 0, 0 );
    exit(0);
#endif
}

size_t CBPFN(heap_query_usage)(void) {
    return global_memory_usage;
}
size_t CBPFN(heap_query_total_usage)(void) {
    return global_total_memory_usage;
}
void CBPFN(memory_set)( void* memory, int8_t value, size_t size ) {
    memset( memory, value, size );
}
void CBPFN(memory_zero)( void* memory, size_t size ) {
    CBPFN(memory_set)( memory, 0, size );
}
void CBPFN(memory_stamp)( void* memory, size_t value_size, const void* value, size_t size ) {
    uint8_t*   dst = (uint8_t*)memory;
    size_t rem = size;
    while( rem ) {
        if( value_size > rem ) {
            break;
        }

        CBPFN(memory_copy)( dst, value, value_size );
        dst += value_size;
    }
}
void CBPFN(memory_copy)( void* restrict dst, const void* restrict src, size_t size ) {
    memcpy( dst, src, size );
}
void CBPFN(memory_move)( void* dst, const void* src, size_t size ) {
    memmove( dst, src, size );
}
bool CBPFN(memory_cmp)( const void* a, const void* b, size_t size ) {
    return memcmp( a, b, size ) == 0;
}

bool CBPFN(char_in_set)( char c, CBPT(String) set ) {
    for( size_t i = 0; i < set.len; ++i ) {
        if( c == set.cc[i] ) {
            return true;
        }
    }
    return false;
}

size_t CBPFN(strlen_utf8)( const char* str ) {
    return CBPFN(string_len_utf8)( string_from_cstr( str ) );
}

CBPT(String) CBPFN(string_new)( size_t len, const char* buf ) {
    CBPT(String) result;
    result.len = len;
    result.buf = (char*)buf;
    return result;
}
CBPT(String) CBPFN(string_empty)() {
    return CBPFN(string_new)( 0, 0 );
}
CBPT(String) CBPFN(string_from_cstr)( const char* cstr ) {
    return CBPFN(string_new)( cstr ? strlen(cstr) : 0, cstr );
}
CBPT(String) CBPFN(string_from_dstring)( CBPT(DString)* dynamic ) {
    return CBPFN(string_new)( dynamic ? CBPFN(dstring_len)(dynamic) : 0, dynamic );
}
bool CBPFN(string_is_empty)( CBPT(String) str ) {
    return !(str.cc && str.len);
}
bool CBPFN(string_is_null_terminated)( CBPT(String) str ) {
    if( !str.cc[str.len] ) {
        return true;
    }
    if( str.len && !str.cc[str.len - 1] ) {
        return true;
    }
    return false;
}
bool CBPFN(string_cmp)( CBPT(String) a, CBPT(String) b ) {
    if( a.len != b.len ) {
        return false;
    }
    return CBPFN(memory_cmp)( a.cc, b.cc, a.len );
}
bool CBPFN(string_find)( CBPT(String) str, char c, size_t* opt_out_index ) {
    const char* ptr = memchr( str.cc, c, str.len );
    if( !ptr ) {
        return false;
    }
    if( opt_out_index ) {
        *opt_out_index = ptr - str.cc;
    }
    return true;
}
bool CBPFN(string_find_rev)( CBPT(String) str, char c, size_t* opt_out_index ) {
    for( size_t i = str.len; i-- > 0; ) {
        if( str.cc[i] == c ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
bool CBPFN(string_find_set)( CBPT(String) str, CBPT(String) set, size_t* opt_out_index ) {
    for( size_t i = 0; i < set.len; ++i ) {
        if( CBPFN(string_find)( str, set.cc[i], opt_out_index ) ) {
            return true;
        }
    }
    return false;
}
bool CBPFN(string_find_set_rev)( CBPT(String) str, CBPT(String) set, size_t* opt_out_index ) {
    for( size_t i = str.len; i-- > 0; ) {
        if( CBPFN(char_in_set)( str.cc[i], set ) ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
bool CBPFN(string_find_string)( CBPT(String) str, CBPT(String) substr, size_t* opt_out_index ) {
    CBPT(String) rem = str;
    if( rem.len < substr.len ) {
        return false;
    }

    while( rem.len ) {
        size_t start = 0;
        if( CBPFN(string_find)( rem, substr.cc[0], &start ) ) {
            rem = CBPFN(string_advance_by)( rem, start );
            if( rem.len < substr.len ) {
                return false;
            }

            if( CBPFN(string_cmp)( CBPFN(string_truncate)(rem, substr.len), substr ) ) {
                if( opt_out_index ) {
                    *opt_out_index = rem.cc - str.cc;
                }
                return true;
            }
            rem = CBPFN(string_advance)( rem );
        } else {
            break;
        }
    }
    return false;
}
bool CBPFN(string_find_string_rev)( CBPT(String) str, CBPT(String) substr, size_t* opt_out_index ) {
    if( str.len < substr.len ) {
        return false;
    }
    for( size_t i = str.len; i-- > 0; ) {
        if( str.cc[i] != substr.cc[i] ) {
            continue;
        }
        if( str.len - i < substr.len ) {
            break;
        }
        CBPT(String) part = CBPFN(string_new)( substr.len, str.cc + i );

        if( CBPFN(string_cmp)( part, substr ) ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
char* CBPFN(string_first)( CBPT(String) str ) {
    if( CBPFN(string_is_empty)( str ) ) {
        return NULL;
    }
    return (char*)str.cc;
}
char* CBPFN(string_last)( CBPT(String) str ) {
    if( CBPFN(string_is_empty)( str ) ) {
        return NULL;
    }
    return (char*)( str.cc + (str.len - 1) );
}
CBPT(String) CBPFN(string_advance)( CBPT(String) str ) {
    if( CBPFN(string_is_empty)( str ) ) {
        return str;
    }
    return CBPFN(string_new)( str.len - 1, str.cc + 1 );
}
CBPT(String) CBPFN(string_advance_by)( CBPT(String) str, size_t stride ) {
    if( CBPFN(string_is_empty)( str ) ) {
        return str;
    }
    if( str.len <= stride ) {
        return CBPFN(string_new)( 0, str.cc + (str.len - 1) );
    }
    return CBPFN(string_new)( str.len - stride, str.cc + stride );
}
CBPT(String) CBPFN(string_truncate)( CBPT(String) str, size_t max ) {
    return CBPFN(string_new)( max > str.len ? str.len : max, str.cc );
}
CBPT(String) CBPFN(string_trim)( CBPT(String) str, size_t amount ) {
    if( amount >= str.len ) {
        return CBPFN(string_new)( 0, str.cc );
    }
    return CBPFN(string_new)( str.len - amount, str.cc );
}
CBPT(String) CBPFN(string_trim_leading_whitespace)( CBPT(String) str ) {
    CBPT(String) res = str;
    while( res.len ) {
        if( isspace( *res.cc ) ) {
            res = CBPFN(string_advance)( res );
        } else {
            break;
        }
    }
    return res;
}
CBPT(String) CBPFN(string_trim_trailing_whitespace)( CBPT(String) str ) {
    CBPT(String) res = str;
    while( res.len ) {
        if( isspace( *CBPFN(string_last)( res ) ) ) {
            res.len--;
        } else {
            break;
        }
    }
    return res;
}
CBPT(String) CBPFN(string_trim_surrounding_whitespace)( CBPT(String) str ) {
    return CBPFN(string_trim_leading_whitespace)( CBPFN(string_trim_trailing_whitespace)( str ) );
}
void CBPFN(string_split_at)(
    CBPT(String) src, size_t at, bool keep_split, CBPT(String)* opt_out_left, CBPT(String)* opt_out_right
) {
    CB_EXPECT( at <= src.len,
        "index provided is outside string bounds! at: %zu", at );

    if( opt_out_left ) {
        *opt_out_left = CBPFN(string_truncate)( src, at );
    }
    if( opt_out_right ) {
        *opt_out_right = CBPFN(string_advance_by)( src, at + (keep_split ? 0 : 1) );
    }
}
bool CBPFN(string_split_char)(
    CBPT(String) src, char c, bool keep_split, CBPT(String)* opt_out_left, CBPT(String)* opt_out_right
) {
    size_t at = 0;
    if( !CBPFN(string_find)( src, c, &at ) ) {
        return false;
    }
    CBPFN(string_split_at)( src, at, keep_split, opt_out_left, opt_out_right );
    return true;
}
CBPT(String)* CBPFN(string_split_delim)( CBPT(String) src, CBPT(String) delim, bool keep_delim ) {
    size_t  count  = 0;
    CBPT(String) substr = src;
    while( substr.len ) {
        size_t pos = 0;
        if( CBPFN(string_find_string)( substr, delim, &pos ) ) {
            count++;
            substr = CBPFN(string_advance_by)( substr, pos + delim.len );
        } else {
            count++;
            break;
        }
    }

    CBPT(String)* res = (CBPT(String)*)darray_empty( sizeof(CBPT(String)), count );
    CB_EXPECT( res, "failed to allocate string buffer!" );

    if( count == 1 ) {
        return (CBPT(String)*)darray_push( res, &src );
    }

    substr = src;
    if( keep_delim ) {
        while( substr.len ) {
            size_t pos = 0;
            if( CBPFN(string_find_string)( substr, delim, &pos ) ) {
                CBPT(String) chunk = substr;
                chunk.len = pos + delim.len;

                CB_EXPECT( darray_try_push( res, &chunk ),
                    "misallocated result!" );

                substr = CBPFN(string_advance_by)( substr, chunk.len );
            } else {
                CB_EXPECT( darray_try_push( res, &substr ),
                    "misallocated result!" );
                break;
            }
        }
    } else {
        while( substr.len ) {
            size_t pos = 0;
            if( CBPFN(string_find_string)( substr, delim, &pos ) ) {
                CBPT(String) chunk = substr;
                chunk.len    = pos;

                CB_EXPECT( darray_try_push( res, &chunk ),
                    "misallocated result!" );

                substr = CBPFN(string_advance_by)( substr, chunk.len + delim.len );
            } else {
                CB_EXPECT( darray_try_push( res, &substr ),
                    "misallocated result!" );
                break;
            }
        }
    }

    return res;
}
CBPT(String)* CBPFN(string_split_delim_char)(
    CBPT(String) src, char delim, bool keep_delim
) {
    char delim_[1] = { delim };
    return CBPFN(string_split_delim)( src, CBPFN(string_new)( 1, delim_ ), keep_delim );
}
CBPT(String)* CBPFN(string_split_delim_char_ex)(
    CBPT(String) src,
    char delim,
    bool keep_delim,
    CBPT(StringSplitDelimFilterFN)* filter,
    void* params
) {
    char delim_[1] = { delim };
    return CBPFN(string_split_delim_ex)(
        src, CBPFN(string_new)( 1, delim_ ), keep_delim, filter, params );
}
CBPT(String)* CBPFN(string_split_delim_ex)(
    CBPT(String) src, CBPT(String) delim, bool keep_delim,
    CBPT(StringSplitDelimFilterFN)* filter, void* params
) {
    CB_EXPECT( filter, "no filter function provided!" );

    size_t  count  = 0;
    CBPT(String) substr = src;
    while( substr.len ) {
        size_t pos = 0;
        if( CBPFN(string_find_string)( substr, delim, &pos ) ) {
            count++;
            substr = CBPFN(string_advance_by)( substr, pos + delim.len );
        } else {
            count++;
            break;
        }
    }

    CBPT(String)* res = (CBPT(String)*)darray_empty( sizeof(CBPT(String)), count );
    CB_EXPECT( res, "failed to allocate string buffer!" );

    if( count == 1 ) {
        CBPT(String) filtered = filter( 0, src, params );
        if( filtered.len ) {
            CB_EXPECT( darray_try_push( res, &filtered ),
                "misallocated result!" );
        }
        return res;
    }

    size_t index = 0;
    substr = src;
    if( keep_delim ) {
        while( substr.len ) {
            size_t pos = 0;
            if( CBPFN(string_find_string)( substr, delim, &pos ) ) {
                CBPT(String) chunk = substr;
                chunk.len    = pos + delim.len;

                chunk = filter( index, chunk, params );

                if( chunk.len ) {
                    CB_EXPECT( darray_try_push( res, &chunk ),
                        "misallocated result!" );
                    index++;
                }

                substr = CBPFN(string_advance_by)( substr, chunk.len );
            } else {
                CBPT(String) filtered = filter( index++, substr, params );

                if( filtered.len ) {
                    CB_EXPECT( darray_try_push( res, &filtered ),
                        "misallocated result!" );
                }
                break;
            }
        }
    } else {
        while( substr.len ) {
            size_t pos = 0;
            if( CBPFN(string_find_string)( substr, delim, &pos ) ) {
                CBPT(String) chunk = substr;
                chunk.len    = pos;

                chunk = filter( index, chunk, params );

                if( chunk.len ) {
                    CB_EXPECT( darray_try_push( res, &chunk ),
                        "misallocated result!" );
                    index++;
                }

                substr = CBPFN(string_advance_by)( substr, chunk.len + delim.len );
            } else {
                CBPT(String) filtered = filter( index++, substr, params );

                if( filtered.len ) {
                    CB_EXPECT( darray_try_push( res, &filtered ),
                        "misallocated result!" );
                }
                break;
            }
        }
    }

    return res;

}

size_t CBPFN(string_len_utf8)( CBPT(String) str ) {
    const unsigned char* ucc = (const unsigned char*)str.cc;
    size_t res = 0;
    for( size_t i = 0; i < str.len; ++i ) {
        if( (ucc[i] & 0xC0) != 0x80 ) {
            res++;
        }
    }
    return res;
}

CBPT(DString)* CBPFN(dstring_empty)( size_t cap ) {
    size_t capacity = cap ? cap : 1;
    struct DynamicString* res =
        (struct DynamicString*)CBPFN(heap_alloc)( sizeof(*res) + capacity );
    res->cap = capacity;
    return res->buf;
}
CBPT(DString)* CBPFN(dstring_new)( size_t len, const char* str ) {
    struct DynamicString* res = 
        (struct DynamicString*)CBPFN(dstring_head)( CBPFN(dstring_empty)( len + 1 ) );
    if( !res ) {
        return NULL;
    }
    CBPFN(memory_copy)( res->buf, str, len );
    res->len = len;
    return res->buf;
}
CBPT(DString)* CBPFN(dstring_from_cstr)( const char* cstr ) {
    return CBPFN(dstring_new)( cstr ? strlen(cstr) : 0, cstr );
}
CBPT(DString)* CBPFN(dstring_from_string)( CBPT(String) str ) {
    return CBPFN(dstring_new)( str.len, str.cc );
}
CBPT(DString)* CBPFN(dstring_fmt_va)( const char* format, va_list va ) {
    va_list va2;
    va_copy( va2, va );

    int msg_len = vsnprintf( 0, 0, format, va2 );
    va_end( va2 );

    struct DynamicString* res = CBPFN(dstring_head)( CBPFN(dstring_empty)( msg_len + 8 ) );
    if( !res ) {
        return NULL;
    }

    vsnprintf( res->buf, res->cap, format, va );
    res->len = msg_len;

    return res->buf;
}
CBPT(DString)* CBPFN(dstring_fmt)( const char* format, ... ) {
    va_list va;
    va_start( va, format );
    CBPT(DString)* res = CBPFN(dstring_fmt_va)( format, va );
    va_end( va );
    return res;
}
CBPT(DString)* CBPFN(dstring_grow)( CBPT(DString)* str, size_t amount ) {
    struct DynamicString* res = CBPFN(dstring_head)( str );
    size_t old_size = sizeof(struct DynamicString) + res->cap;
    size_t new_size = old_size + amount;

    res = CBPFN(heap_realloc)( res, old_size, new_size );
    if( !res ) {
        return NULL;
    }

    res->cap += amount;
    return res->buf;
}
CBPT(DString)* CBPFN(dstring_clone)( const CBPT(DString)* src ) {
    return CBPFN(dstring_from_string)( string_from_dstring( src ) );
}
CBPT(DString)* CBPFN(dstring_concat)( CBPT(String) lhs, CBPT(String) rhs ) {
    size_t len        = lhs.len + rhs.len;
    size_t total_size = len + 8;
    struct DynamicString* res = CBPFN(dstring_head)( CBPFN(dstring_empty)( total_size ) );

    CBPFN(memory_copy)( res->buf, lhs.cc, lhs.len );
    CBPFN(memory_copy)( res->buf + lhs.len, rhs.cc, rhs.len );

    res->len = len;

    return res->buf;
}
CBPT(DString)* CBPFN(dstring_concat_cstr)( const char* lhs, const char* rhs ) {
    return CBPFN(dstring_concat)(
        CBPFN(string_from_cstr)( lhs ), CBPFN(string_from_cstr)( rhs ) );
}
CBPT(DString)* CBPFN(dstring_concat_multi)(
    size_t count, const CBPT(String)* strings, CBPT(String) opt_separator
) {
    CB_EXPECT( count, "did not provide any strings!" );
    size_t total_size = (count - 1) * opt_separator.len;
    for( size_t i = 0; i < count; ++i ) {
        total_size += strings[i].len;
    }

    CBPT(DString)* res = CBPFN(dstring_empty)( total_size + 1 );

    if( opt_separator.len ) {
        for( size_t i = 0; i < count; ++i ) {
            res = CBPFN(dstring_append)( res, strings[i] );
            if( i + 1 != count ) {
                res = CBPFN(dstring_append)( res, opt_separator );
            }
        }
    } else {
        for( size_t i = 0; i < count; ++i ) {
            res = CBPFN(dstring_append)( res, strings[i] );
        }
    }

    return res;
}
CBPT(DString)* CBPFN(dstring_concat_multi_cstr)(
    size_t count, const char** strings, const char* opt_separator
) {
    CB_EXPECT( count, "did not provide any strings!" );
    size_t seplen     = opt_separator ? strlen( opt_separator ) : 0;
    size_t total_size = (count - 1) * seplen;
    for( size_t i = 0; i < count; ++i ) {
        const char* current = strings[i];
        if( !current ) {
            continue;
        }
        total_size += strlen( current );
    }

    CBPT(DString)* res = CBPFN(dstring_empty)( total_size + 1 );

    if( opt_separator && seplen ) {
        CBPT(String) sep = CBPFN(string_new)( seplen, opt_separator );
        for( size_t i = 0; i < count; ++i ) {
            const char* current = strings[i];
            if( !current ) {
                continue;
            }

            res = CBPFN(dstring_append_cstr)( res, current );
            if( i + 1 != count ) {
                res = CBPFN(dstring_append)( res, sep );
            }
        }
    } else {
        for( size_t i = 0; i < count; ++i ) {
            const char* current = strings[i];
            if( !current ) {
                continue;
            }
            res = CBPFN(dstring_append_cstr)( res, current );
        }
    }

    return res;
}
CBPT(DString)* CBPFN(dstring_append)( CBPT(DString)* str, CBPT(String) append ) {
    struct DynamicString* res = CBPFN(dstring_head)( str );

    if( res->len + append.len + 1 > res->cap ) {
        res = CBPFN(dstring_head)( CBPFN(dstring_grow)( res->buf, append.len + 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    CBPFN(memory_copy)( res->buf + res->len, append.cc, append.len );
    res->len += append.len;
    res->buf[res->len] = 0;
    return res->buf;
}
CBPT(DString)* CBPFN(dstring_prepend)( CBPT(DString)* str, CBPT(String) prepend ) {
    struct DynamicString* res = CBPFN(dstring_head)( str );

    if( res->len + prepend.len + 1 > res->cap ) {
        res = CBPFN(dstring_head)( CBPFN(dstring_grow)( res->buf, prepend.len + 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    CBPFN(memory_move)( res->buf + prepend.len, res->buf, res->len + 1 );
    CBPFN(memory_copy)( res->buf, prepend.cc, prepend.len );

    res->len += prepend.len;
    return res->buf;
}
CBPT(DString)* CBPFN(dstring_insert)( CBPT(DString)* str, CBPT(String) insert, size_t at ) {
    if( at == 0 ) {
        return CBPFN(dstring_prepend)( str, insert );
    }
    struct DynamicString* res = CBPFN(dstring_head)( str );
    if( res->len && res->len - 1 == at ) {
        return CBPFN(dstring_append)( str, insert );
    }
    if( at >= res->len ) {
        CB_WARN(
            "dstring_insert: attempted to insert past dstring bounds! "
            "len: %zu index: %zu", res->len, at );
        return NULL;
    }

    if( res->len + insert.len + 1 > res->cap ) {
        res = CBPFN(dstring_head)( CBPFN(dstring_grow)( res->buf, insert.len + 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    CBPFN(memory_move)( res->buf + at + insert.len, res->buf + at, (res->len + 1) - at );
    CBPFN(memory_copy)( res->buf + at, insert.cc, insert.len );
    res->len += insert.len;

    return res->buf;
}
CBPT(DString)* CBPFN(dstring_push)( CBPT(DString)* str, char c ) {
    struct DynamicString* res = CBPFN(dstring_head)( str );
    if( res->len + 2 >= res->cap ) {
        res = CBPFN(dstring_head)( CBPFN(dstring_grow)( str, 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    res->buf[res->len++] = c;
    res->buf[res->len]   = 0;
    return res->buf;
}
CBPT(DString)* CBPFN(dstring_emplace)( CBPT(DString)* str, char c, size_t at ) {
    return CBPFN(dstring_insert)( str, CBPFN(string_new)( 1, &c ), at );
}
bool CBPFN(dstring_pop)( CBPT(DString)* str, char* opt_out_c ) {
    struct DynamicString* head = CBPFN(dstring_head)( str );
    if( !head->len ) {
        return false;
    }
    char c = head->buf[--head->len];
    head->buf[head->len] = 0;

    if( opt_out_c ) {
        *opt_out_c = c;
    }
    return true;
}
bool CBPFN(dstring_remove)( CBPT(DString)* str, size_t index ) {
    struct DynamicString* head = CBPFN(dstring_head)( str );
    if( !head->len || index > head->len ) {
        CB_WARN(
            "dstring_remove: attempted to remove past dstring bounds! "
            "len: %zu index: %zu", head->len, index );
        return false;
    }

    CBPFN(memory_move)( head->buf + index, head->buf + index + 1, (head->len + 1) - index );
    head->len--;

    return true;
}
bool CBPFN(dstring_remove_range)( CBPT(DString)* str, size_t from_inclusive, size_t to_exclusive ) {
    CB_ASSERT( from_inclusive < to_exclusive,
        "dstring_remove_range: invalid range provided! (%zu, %zu]",
        from_inclusive, to_exclusive );
    struct DynamicString* head = CBPFN(dstring_head)( str );
    if( !head->len || from_inclusive >= head->len || to_exclusive > head->len ) {
        CB_WARN(
            "dstring_remove_range: attempted to remove past dstring bounds! "
            "len: %zu range: (%zu, %zu]", head->len, from_inclusive, to_exclusive );
        return false;
    }

    size_t span = to_exclusive - from_inclusive;

    CBPFN(memory_move)(
        head->buf + from_inclusive,
        head->buf + to_exclusive,
        (head->len + 1) - span );
    head->len -= span;

    return true;
}
void CBPFN(dstring_truncate)( CBPT(DString)* str, size_t max ) {
    struct DynamicString* head = CBPFN(dstring_head)( str );
    if( max >= head->len ) {
        return;
    }
    CBPFN(memory_zero)( head->buf + max, head->len - max );
    head->len = max;
}
void CBPFN(dstring_trim)( CBPT(DString)* str, size_t amount ) {
    size_t len = CBPFN(dstring_len)( str );
    CBPFN(dstring_truncate)( str, amount > len ? 0 : len - amount );
}
void CBPFN(dstring_clear)( CBPT(DString)* str ) {
    struct DynamicString* head = CBPFN(dstring_head)( str );

    CBPFN(memory_zero)( head->buf, head->len );
    head->len = 0;
}
size_t CBPFN(dstring_remaining)( const CBPT(DString)* str ) {
    // -1 to not include null-terminator
    return (CBPFN(dstring_cap)( str ) - 1)  - CBPFN(dstring_len)( str );
}
size_t CBPFN(dstring_len)( const CBPT(DString)* str ) {
    return ((struct DynamicString*)str - 1)->len;
}
size_t CBPFN(dstring_cap)( const CBPT(DString)* str ) {
    return ((struct DynamicString*)str - 1)->cap;
}
size_t CBPFN(dstring_total_size)( const CBPT(DString)* str ) {
    return CBPFN(dstring_cap)( str ) + sizeof(struct DynamicString);
}
bool CBPFN(dstring_is_empty)( const CBPT(DString)* str ) {
    return CBPFN(dstring_len)( str ) == 0;
}
bool CBPFN(dstring_is_full)( const CBPT(DString)* str ) {
    return CBPFN(dstring_len)( str ) == CBPFN(dstring_cap)( str );
}
void CBPFN(dstring_set_len)( CBPT(DString)* str, size_t len ) {
    struct DynamicString* head = CBPFN(dstring_head)( str );
    CB_EXPECT( len < head->cap, "length exceeds string capacity!" );
    head->len            = len;
    head->buf[head->len] = 0;
}
void* CBPFN(dstring_head)( CBPT(DString)* str ) {
    if( !str ) {
        return NULL;
    }
    return str - sizeof(struct DynamicString);
}
const void* CBPFN(dstring_head_const)( const CBPT(DString)* str ) {
    if( !str ) {
        return NULL;
    }
    return str - sizeof(struct DynamicString);
}
void CBPFN(dstring_free)( CBPT(DString)* str ) {
    struct DynamicString* head = CBPFN(dstring_head)( str );
    if( !head ) {
        return;
    }
    size_t total_size = head->cap + sizeof(*head);
    CBPFN(heap_free)( head, total_size );
}

CBPT(String) path_cwd(void) {
    volatile struct GlobalBuffers* gb = get_global_buffers();
    return gb->cwd;
}
CBPT(String) path_home(void) {
    volatile struct GlobalBuffers* gb = get_global_buffers();
    return gb->home;
}
size_t path_chunk_count( CBPT(String) path ) {
    CBPT(String) subpath = path;
    size_t  count   = 0;

    while( subpath.len ) {
        size_t sep = 0;
        if( CBPFN(string_find)( subpath, '/', &sep ) ) {
            CBPT(String) chunk = subpath;
            chunk.len    = sep;

            subpath = CBPFN(string_advance_by)( subpath, sep + 1 );
            count++;
        } else {
            count++;
            break;
        }
    }

    return count;
}
CBPT(String)* path_chunk_split( CBPT(String) path ) {
    size_t   cap = path_chunk_count( path );
    CBPT(String)* res = darray_empty( sizeof(*res), cap ? cap : 1 );
    if( !res ) {
        return NULL;
    }

    CBPT(String) subpath = path;
    while( subpath.len ) {
        size_t sep = 0;
        if( CBPFN(string_find)( subpath, '/', &sep ) ) {
            CBPT(String) chunk = subpath;
            chunk.len    = sep;

            CB_EXPECT( darray_try_push( res, &chunk ), "push should have worked!" );

            subpath = CBPFN(string_advance_by)( subpath, sep + 1 );
        } else {
            CB_EXPECT( darray_try_push( res, &subpath ), "push should have worked!" );
            break;
        }
    }

    return res;
}
bool path_matches_glob( CBPT(String) path, CBPT(String) glob ) {
    if( glob.len == 1 && glob.cc[0] == '*' ) {
        return true;
    }

    while( path.len && *glob.cc != '*' ) {
        if( *glob.cc != *path.cc && *glob.cc != '?' ) {
            return false;
        }
        glob = CBPFN(string_advance)( glob );
        path = CBPFN(string_advance)( path );
    }

    CBPT(String) mp, cp;
    while( path.len ) {
        if( *glob.cc == '*' ) {
            glob = CBPFN(string_advance)( glob );
            if( !glob.len ) {
                return true;
            }

            mp = glob;
            cp = CBPFN(string_advance)( path );
        } else if( *glob.cc == *path.cc || *glob.cc == '?' ) {
            glob = CBPFN(string_advance)( glob );
            path = CBPFN(string_advance)( path );
        } else {
            glob = mp;
            cp   = CBPFN(string_advance)( cp );
            path = cp;
        }
    }

    while( glob.len && *glob.cc == '*' ) {
        glob = CBPFN(string_advance)( glob );
    }
    return glob.len ? false : true;

}
static bool path_walk_glob_filter_filter(
    size_t index, size_t stride, const void* item, void* params
) {
    CB_UNUSED(index, stride);

    CBPT(String) glob = *(CBPT(String)*)params;
    CBPT(String) path = *(CBPT(String)*)item;

    return path_matches_glob( path, glob );
}
CBPT(String)* path_walk_glob_filter( const WalkDirectory* wd, CBPT(String) glob ) {
    CB_ASSERT( wd && wd->paths, "walk result is null!" );

    CBPT(String)* res = darray_from_filter(
        sizeof(CBPT(String)), wd->count, wd->paths,
        path_walk_glob_filter_filter, &glob );
    return res;
}
bool path_walk_dir(
    const char* dir, bool recursive,
    bool include_dirs, WalkDirectory* out_result
) {
    CB_ASSERT( dir, "no path provided!" );
    CB_ASSERT( out_result, "no walk dir result provided!" );

    CBPT(DString)* path = CBPFN(dstring_from_cstr)( dir );
    if( !path ) {
        return false;
    }

    CBPT(DString)* buffer = CBPFN(dstring_empty)( 255 );
    if( !buffer ) {
        CBPFN(dstring_free)( path );
        CB_ERROR( "path_walk_dir: failed to allocate buffer!" );
        return false;
    }

    size_t count = 0;
    bool result = path_walk_dir_internal(
        &path, recursive, include_dirs, &count, &buffer );
    CBPFN(dstring_free)( path );

    if( !result ) {
        CBPFN(dstring_free)( buffer );
        return false;
    }

    if( !count ) {
        CBPFN(dstring_free)( buffer );
        return true;
    }

    size_t total = out_result->count + count;
    CBPT(String)* paths = darray_empty( sizeof(CBPT(String)), total );
    if( !paths ) {
        CBPFN(dstring_free)( buffer );
        return false;
    }

    if( out_result->buf ) {
        CBPT(DString)* concat =
            CBPFN(dstring_append)( out_result->buf, string_from_dstring( buffer ) );
        CBPFN(dstring_free)( buffer );
        if( !concat ) {
            return false;
        }
    } else {
        out_result->buf = buffer;
    }

    out_result->count = total;

    CBPT(String) rem = string_from_dstring( out_result->buf );
    while( rem.len ) {
        size_t nul = 0;
        if( CBPFN(string_find)( rem, 0, &nul ) ) {
            CBPT(String) current = rem;
            current.len    = nul;

            CB_EXPECT( darray_try_push( paths, &current ), "miscalculated path count!" );

            rem = CBPFN(string_advance_by)( rem, nul + 1 );
        } else {
            CB_EXPECT( darray_try_push( paths, &rem ), "miscalculated path count!" );
            break;
        }
    }

    if( out_result->paths ) {
        darray_free( out_result->paths );
    }

    out_result->paths = paths;
    return true;
}
void path_walk_free( WalkDirectory* wd ) {
    if( wd ) {
        if( wd->paths ) {
            darray_free( wd->paths );
        }
        if( wd->buf ) {
            CBPFN(dstring_free)( wd->buf );
        }

        CBPFN(memory_zero)( wd, sizeof(*wd) );
    }
}
bool dir_remove( const char* path, bool recursive ) {
    if( !recursive ) {
        return dir_remove_internal( path );
    }

    WalkDirectory wd;
    CBPFN(memory_zero)( &wd, sizeof(wd) );

    if( !path_walk_dir( path, true, false, &wd ) ) {
        CB_ERROR( "dir_remove: failed to walk directory '%s'!", path );
        return false;
    }

    for( size_t i = 0; i < wd.count; ++i ) {
        if( !file_remove( wd.paths[i].cc ) ) {
            CB_ERROR( "dir_remove: failed to remove file '%s'!", wd.paths[i].cc );

            path_walk_free( &wd );
            return false;
        }
    }

    path_walk_free( &wd );

    if( !path_walk_dir( path, true, true, &wd ) ) {
        CB_ERROR( "dir_remove: failed to walk directory '%s'!", path );
        return false;
    }

    for( size_t i = 0; i < wd.count; ++i ) {
        if( !dir_remove_internal( wd.paths[i].cc ) ) {
            CB_ERROR( "dir_remove: failed to remove dir '%s'!", wd.paths[i].cc );

            path_walk_free( &wd );
            return false;
        }
    }

    path_walk_free( &wd );
    return dir_remove_internal( path );
}

bool fd_write_fmt_va( CBPT(FileID)* file, const char* format, va_list va ) {
    char* formatted = local_fmt_va( format, va );
    return fd_write( file, strlen( formatted ), formatted, 0 );
}
bool fd_write_fmt( CBPT(FileID)* file, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    bool res = fd_write_fmt_va( file, format, va );
    va_end( va );
    return res;
}

void* darray_empty( size_t stride, size_t cap ) {
    struct DynamicArray* res = CBPFN(heap_alloc)( sizeof(*res) + (stride * cap) );
    if( !res ) {
        return NULL;
    }
    res->stride = stride;
    res->cap    = cap;
    return res->buf;
}
void* darray_from_array( size_t stride, size_t len, const void* buf ) {
    struct DynamicArray* res = darray_head( darray_empty( stride, len + 2 ) );
    if( !res ) {
        return NULL;
    }

    CBPFN(memory_copy)( res->buf, buf, len * stride );
    res->len = len;

    return res->buf;
}
size_t darray_static_memory_requirement( size_t stride, size_t cap ) {
    return (stride * cap) + sizeof(struct DynamicArray);
}
void* darray_static( size_t stride, size_t cap, void* buf ) {
    struct DynamicArray* res = buf;
    res->stride = stride;
    res->cap    = cap;
    res->len    = 0;
    return res->buf;
}
void* darray_join( size_t stride,
    size_t lhs_len, const void* lhs, size_t rhs_len, const void* rhs
) {
    struct DynamicArray* res =
        darray_head( darray_empty( stride, lhs_len + rhs_len + 2 ) );
    if( !res ) {
        return NULL;
    }

    CBPFN(memory_copy)( res->buf, lhs, stride * lhs_len );
    CBPFN(memory_copy)( res->buf + (stride * lhs_len), rhs, rhs_len );
    res->len = lhs_len + rhs_len;

    return res;
}
void* darray_from_filter(
    size_t stride, size_t len, const void* src,
    CBPT(DarrayFilterFN)* filter, void* filter_params
) {
    const uint8_t* src_bytes = src;

    void* res = darray_empty( stride, 10 );
    if( !res ) {
        return NULL;
    }

    for( size_t i = 0; i < len; ++i ) {
        const uint8_t* item = src_bytes + (stride * i);
        if( filter( i, stride, item, filter_params ) ) {
            void* _new = darray_push( res, item );
            if( !_new ) {
                darray_free( res );
                return NULL;
            }
        }
    }

    return res;
}

void* darray_grow( void* darray, size_t amount ) {
    struct DynamicArray* res = darray_head( darray );
    size_t old_size = (res->stride * res->cap) + sizeof(*res);
    size_t new_size = (res->stride * amount) + old_size;

    res = CBPFN(heap_realloc)( res, old_size, new_size );
    if( !res ) {
        return NULL;
    }

    res->cap += amount;
    return res->buf;
}
void* darray_clone( const void* darray ) {
    return darray_from_array(
        darray_stride( darray ), darray_len( darray ), darray );
}
void darray_clear( void* darray ) {
    struct DynamicArray* head = darray_head( darray );
    CBPFN(memory_zero)( head->buf, head->len * head->stride );
    head->len = 0;
}
void* darray_set_len( void* darray, size_t len ) {
    if( len > darray_cap( darray ) ) {
        size_t diff = len - darray_cap( darray );

        struct DynamicArray* head = darray_head( darray_grow( darray, diff ) );
        if( !head ) {
            return NULL;
        }

        head->len = len;
        return head->buf;
    } else if( len > darray_len( darray ) ) {
        struct DynamicArray* head = darray_head( darray );
        size_t diff = head->len - len;
        CBPFN(memory_zero)( head->buf + len, head->stride * diff );
        head->len = diff;

        return head->buf;
    } else {
        darray_truncate( darray, len );
        return darray;
    }
}
void darray_truncate( void* darray, size_t max ) {
    struct DynamicArray* head = darray_head( darray );
    if( max >= head->len ) {
        return;
    }
    size_t diff = head->len - max;
    CBPFN(memory_zero)( head->buf + max, head->stride * diff );
    head->len = max;
}
void darray_trim( void* darray, size_t amount ) {
    struct DynamicArray* head = darray_head( darray );
    darray_truncate( head->buf, amount > head->len ? 0 : head->len - amount );
}
bool darray_try_push( void* darray, const void* item ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len == head->cap ) {
        return false;
    }

    CBPFN(memory_copy)( head->buf + (head->stride * head->len), item, head->stride );
    head->len++;

    return true;
}
bool darray_try_emplace( void* darray, const void* item, size_t at ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len == head->cap ) {
        return false;
    }
    if( at >= head->len ) {
        CB_WARN(
            "darray_emplace: attempted to emplace past darray bounds! "
            "len: %zu index: %zu", head->len, at );
        return false;
    }

    CBPFN(memory_move)(
        head->buf + ( head->stride * ( at + 1 ) ),
        head->buf + ( head->stride * at ),
        head->stride * ( ( head->len + 1 ) - at ) );
    CBPFN(memory_copy)( head->buf + ( head->stride * at ), item, head->stride );
    head->len++;

    return true;
}
bool darray_try_insert( void* darray, size_t count, const void* items, size_t at ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len + count > head->cap ) {
        return false;
    }
    if( at >= head->len ) {
        CB_WARN(
            "darray_insert: attempted to insert past darray bounds! "
            "len: %zu index: %zu", head->len, at );
        return false;
    }

    CBPFN(memory_move)(
        head->buf + ( head->stride * ( at + count ) ),
        head->buf + ( head->stride * at ),
        head->stride * ( ( head->len + 1 ) - at ) );
    CBPFN(memory_copy)( head->buf + ( head->stride * at ), items, head->stride * count );
    head->len += count;

    return true;
}
bool darray_try_append( void* darray, size_t count, const void* items ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len + count > head->cap ) {
        return false;
    }

    CBPFN(memory_copy)(
        head->buf + (head->stride * head->len),
        items, head->stride * count );
    head->len += count;

    return true;
}
bool darray_pop( void* darray, void* opt_out_item ) {
    struct DynamicArray* head = darray_head( darray );
    if( !head->len ) {
        return false;
    }

    head->len--;
    if( opt_out_item ) {
        CBPFN(memory_copy)(
            opt_out_item, head->buf + (head->stride * head->len), head->stride );
    }

    CBPFN(memory_zero)( head->buf + (head->stride * head->len), head->stride );
    return true;
}
void* darray_push( void* darray, const void* item ) {
    void* res = darray;
    if( darray_try_push( res, item ) ) {
        return res;
    }

    res = darray_grow( res, 5 );
    if( !res ) {
        return NULL;
    }

    darray_try_push( res, item );
    return res;
}
void* darray_emplace( void* darray, const void* item, size_t at ) {
    if( at >= darray_len( darray ) ) {
        CB_WARN(
            "darray_emplace: attempted to emplace past darray bounds! "
            "len: %zu index: %zu", darray_len( darray ), at );
        return NULL;
    }

    void* res = darray;
    if( darray_try_emplace( res, item, at ) ) {
        return res;
    }

    res = darray_grow( res, 5 );
    if( !res ) {
        return NULL;
    }

    darray_try_emplace( res, item, at );
    return res;
}
void* darray_insert( void* darray, size_t count, const void* items, size_t at ) {
    struct DynamicArray* res = darray_head( darray );

    if( at >= res->len ) {
        CB_WARN(
            "darray_insert: attempted to insert past darray bounds! "
            "len: %zu index: %zu", res->len, at );
        return false;
    }

    if( darray_try_insert( res->buf, count, items, at ) ) {
        return res->buf;
    }

    res = darray_head( darray_grow( res->buf, (count - (res->cap - res->len)) + 5 ) );
    if( !res ) {
        return NULL;
    }

    darray_try_insert( res->buf, count, items, at );
    return res->buf;
}
void* darray_append( void* darray, size_t count, const void* items ) {
    struct DynamicArray* res = darray_head( darray );
    if( darray_try_append( res->buf, count, items ) ) {
        return res;
    }

    res = darray_head( darray_grow( res->buf, (count - (res->cap - res->len)) + 5 ) );
    if( !res ) {
        return NULL;
    }

    darray_try_append( res->buf, count, items );
    return res->buf;
}
bool darray_remove( void* darray, size_t index ) {

    struct DynamicArray* head = darray_head( darray );
    if( !head->len || index > head->len ) {
        CB_WARN(
            "darray_remove: attempted to remove past array bounds! "
            "len: %zu index: %zu", head->len, index );
        return false;
    }
    if( index == (head->len - 1) ) {
        return darray_pop( darray, 0 );
    }

    void* item_to_remove = head->buf + (head->stride * index);
    void* item_next      = (uint8_t*)item_to_remove + head->stride;
    size_t move_size      = (head->buf + (head->stride * head->cap)) - (uint8_t*)item_next;

    CBPFN(memory_move)( item_to_remove, item_next, move_size );
    head->len--;
    CBPFN(memory_zero)( head->buf + (head->stride * head->len), head->stride );

    return true;
}
bool darray_remove_range( void* darray, size_t from_inclusive, size_t to_exclusive ) {
    CB_ASSERT( from_inclusive < to_exclusive,
        "darray_remove_range: invalid range provided! (%zu, %zu]",
        from_inclusive, to_exclusive );
    struct DynamicArray* head = darray_head( darray );
    if( !head->len || from_inclusive >= head->len || to_exclusive > head->len ) {
        CB_WARN(
            "darray_remove_range: attempted to remove past array bounds! "
            "len: %zu range: (%zu, %zu]", head->len, from_inclusive, to_exclusive );
        return false;
    }

    // TODO(alicia): reimplement this properly
    for( size_t i = from_inclusive; i < to_exclusive; ++i ) {
        darray_remove( darray, from_inclusive );
    }

    return true;
}
size_t darray_remaining( const void* darray ) {
    return darray_cap( darray ) - darray_len( darray );
}
size_t darray_len( const void* darray ) {
    return ((struct DynamicArray*)darray - 1)->len;
}
size_t darray_cap( const void* darray ) {
    return ((struct DynamicArray*)darray - 1)->cap;
}
size_t darray_stride( const void* darray ) {
    return ((struct DynamicArray*)darray - 1)->stride;
}
size_t darray_total_size( const void* darray ) {
    return
        sizeof(struct DynamicArray) +
        (darray_cap( darray ) * darray_stride( darray ));
}
bool darray_is_empty( const void* darray ) {
    return darray_len( darray ) == 0;
}
bool darray_is_full( const void* darray ) {
    return darray_len( darray ) == darray_cap( darray );
}
void* darray_head( void* darray ) {
    if( !darray ) {
        return NULL;
    }
    return (struct DynamicArray*)darray - 1;
}
const void* darray_head_const( const void* darray ) {
    if( !darray ) {
        return NULL;
    }
    return (const struct DynamicArray*)darray - 1;
}
void darray_free( void* darray ) {
    if( darray ) {
        struct DynamicArray* head = darray_head( darray );
        size_t total_size = (head->cap * head->stride) + sizeof(*head);
        CBPFN(heap_free)( head, total_size );
    }
}

bool job_enqueue( JobFN* job, void* params ) {
    volatile struct JobQueue* queue = get_job_queue();

    if( queue->pending >= CB_MAX_JOBS ) {
        CB_WARN(
            "attempted to enqueue job while queue is full!" );
        return false;
    }

    struct JobEntry entry;
    entry.proc   = job;
    entry.params = params;

    fence();

    atom back = atomic_add( &queue->back, 1 );
    queue->entries[back % CB_MAX_JOBS] = entry;

    fence();

    atomic_add( &queue->len, 1 );
    atomic_add( &queue->pending, 1 );

    semaphore_signal( (Semaphore*)&queue->wakeup );
    return true;
}
bool job_enqueue_timed( JobFN* job, void* params, uint32_t ms ) {
    volatile struct JobQueue* queue = get_job_queue();

    while( queue->pending >= CB_MAX_JOBS ) {
        if( !job_wait_next( ms ) ) {
            return false;
        }
    }

    CB_EXPECT( job_enqueue( job, params ),
        "enqueue unexpectedly failed!" );
    return true;
}
bool job_wait_next( uint32_t ms ) {
    volatile struct JobQueue* queue = get_job_queue();

    uint32_t current = queue->pending;
    if( !current ) {
        return true;
    }

    if( ms == MT_WAIT_INFINITE ) {
        while( queue->pending >= current ) {
            thread_sleep( 1 );
        }
        return true;
    } else for( uint32_t i = 0; i < ms; ++i ) {
        if( queue->pending < current ) {
            return true;
        }
        thread_sleep( 1 );
    }

    return false;
}
bool job_wait_all( uint32_t ms ) {
    volatile struct JobQueue* queue = get_job_queue();

    if( ms == MT_WAIT_INFINITE ) {
        while( queue->pending ) {
            thread_sleep(1);
        }
        return true;
    } else for( uint32_t i = 0; i < ms; ++i ) {
        if( !queue->pending ) {
            return true;
        }
        thread_sleep( 1 );
    }

    return false;
}

uint32_t thread_id(void) {
    return global_thread_id;
}
bool command_builder_new( const char* exe, CBPT(CommandBuilder)* out_builder ) {
    out_builder->buf = darray_empty( sizeof(char), 128 );
    if( !out_builder->buf ) {
        return false;
    }
    out_builder->args = darray_empty( sizeof(const char*), 5 );
    if( !out_builder->args ) {
        darray_free( out_builder->buf );
        return false;
    }
    command_builder_push( out_builder, exe );
    return true;
}
void command_builder_clear( CBPT(CommandBuilder)* builder ) {
    darray_clear( builder->buf );
    darray_clear( builder->args );
}
bool command_builder_push( CBPT(CommandBuilder)* builder, const char* arg ) {
    size_t arg_len = strlen( arg );
    if( darray_remaining( builder->buf ) < arg_len + 1 ) {
        CB_DARRAY(char) new_buf = darray_grow( builder->buf, darray_cap( builder->buf ) );
        if( !new_buf ) {
            return false;
        }
        builder->buf = new_buf;
    }
    if( darray_remaining( builder->args ) < 1 ) {
        CB_DARRAY(const char*) new_buf =
            darray_grow( builder->args, darray_cap( builder->args ) );
        if( !new_buf ) {
            return false;
        }
        builder->args = new_buf;
    }
    const char* new_arg = builder->buf + darray_len( builder->buf );
    darray_try_append( builder->buf, arg_len + 1, arg );

    darray_pop( builder->args, 0 );
    darray_try_push( builder->args, &new_arg );
    const char* null = 0;
    darray_try_push( builder->args, &null );

    return true;
}
bool __internal_command_builder_append(
    CBPT(CommandBuilder)* builder, const char* first, ...
) {
    if( !command_builder_push( builder, first ) ) {
        return false;
    }
    va_list va;
    va_start( va, first );

    for( ;; ) {
        const char* next = va_arg( va, const char* );
        if( !next ) {
            break;
        }

        if( !command_builder_push( builder, next ) ) {
            va_end(va);
            return false;
        }
    }

    va_end(va);
    return true;
}
bool command_builder_append_list(
    CBPT(CommandBuilder)* builder, size_t count, const char** args
) {
    for( size_t i = 0; i < count; ++i ) {
        if( !command_builder_push( builder, args[i])) {
            return false;
        }
    }
    return true;
}
CBPT(Command) command_builder_cmd( CBPT(CommandBuilder)* builder ) {
    return (CBPT(Command)){
        .count = darray_len(builder->args) - 1,
        .args  = builder->args,
    };
}
void command_builder_free( CBPT(CommandBuilder)* builder ) {
    darray_free( builder->buf );
    darray_free( builder->args );
    CBPFN(memory_zero)( builder, sizeof(*builder) );
}
CBPT(DString)* command_flatten_dstring( const CBPT(Command)* command ) {
    size_t total_len = 1;
    for( size_t i = 0; i < command->count; ++i ) {
        total_len += strlen( command->args[i] ) + 3; // to account for potential "" and space
    }
    CBPT(DString)* res = CBPFN(dstring_empty)( total_len );
    if( !res ) {
        return NULL;
    }

    for( size_t i = 0; i < command->count; ++i ) {
        const char* current = command->args[i];
        if( !current ) {
            continue;
        }

        size_t current_len = strlen( current );
        if( !current_len ) {
            continue;
        }

        bool contains_space = false;
        CBPT(String) arg = CBPFN(string_new)( current_len, current );
        if( CBPFN(string_find)( arg, ' ', 0 ) ) {
            CB_EXPECT( CBPFN(dstring_push)( res, '"' ) == res, "miscalculated total_len!" );
            contains_space = true;
        }

        CB_EXPECT( CBPFN(dstring_append)( res, arg ) == res, "miscalculated total_len!" );

        if( contains_space ) {
            CB_EXPECT( CBPFN(dstring_push)( res, '"' ) == res, "miscalculated total_len!" );
        }

        if( i + 1 != command->count ) {
            CB_EXPECT( CBPFN(dstring_push)( res, ' ' ) == res, "miscalculated total_len!" );
        }
    }

    return res;
}

uint8_t* local_byte_buffer() {
    fence();
    return (uint8_t*)get_next_local_buffer( thread_id() );
}
char* local_fmt_va( const char* format, va_list va ) {
    char* buf = (char*)local_byte_buffer();
    vsnprintf( buf, CB_LOCAL_BUFFER_CAPACITY - 1, format, va ); // -1 to ensure null-terminated
    return buf;
}
char* local_fmt( const char* format, ... ) {
    va_list va;
    va_start( va, format );
    char* res = local_fmt_va( format, va );
    va_end( va );
    return res;
}

static bool logger_check_level( CBPT(LogLevel) level ) {
    return level >= global_logger_level;
}

void logger_set_level( CBPT(LogLevel) level ) {
    global_logger_level = level;
}
CBPT(LogLevel) logger_get_level(void) {
    return global_logger_level;
}
void logger_va( CBPT(LogLevel) level, const char* format, va_list va ) {
    if( !logger_check_level( level ) ) {
        return;
    }

    static const char local_level_letters[] = {
        'I', // CB_LOG_LEVEL_INFO
        'W', // CB_LOG_LEVEL_WARNING
        'E', // CB_LOG_LEVEL_ERROR
        'F', // CB_LOG_LEVEL_FATAL
    };

    static const char* local_level_colors[] = {
        "",           // CB_LOG_LEVEL_INFO
        "\033[1;33m", // CB_LOG_LEVEL_WARNING
        "\033[1;31m", // CB_LOG_LEVEL_ERROR
        "\033[1;35m", // CB_LOG_LEVEL_FATAL
    };

    FILE* const level_output[] = {
        stdout, // CB_LOG_LEVEL_INFO
        stdout, // CB_LOG_LEVEL_WARNING
        stderr, // CB_LOG_LEVEL_ERROR
        stderr, // CB_LOG_LEVEL_FATAL
    };

    if( global_is_mt ) {
        mutex_lock( &global_logger_mutex );
    }

    FILE* output = level_output[level];

    fprintf( output, "%s[%c:%02u] cbuild: ",
        local_level_colors[level], local_level_letters[level], thread_id() );
    vfprintf( output, format, va );
    fprintf( output, "\033[1;00m\n" );

    fflush( output );

    if( global_is_mt ) {
        mutex_unlock( &global_logger_mutex );
    }
}
void logger( CBPT(LogLevel) level, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    logger_va( level, format, va );
    va_end( va );
}

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <limits.h>

#if !defined(PATH_MAX)
    #define PATH_MAX 260
#endif

struct Win32ThreadParams {
    JobFN* proc;
    void*  params;
    uint32_t    id;
};

#define CBUILD_LOCAL_WIDE_CAPACITY (CB_LOCAL_BUFFER_CAPACITY / 2)

static struct Win32ThreadParams global_win32_thread_params[CB_THREAD_COUNT];

static HANDLE global_win32_process_heap = NULL;

void _platform_init_(void) {
    SetConsoleCP( CP_UTF8 );
    SetConsoleOutputCP( CP_UTF8 );
}
static HANDLE get_process_heap(void) {
    if( !global_win32_process_heap ) {
        global_win32_process_heap = GetProcessHeap();
        CB_EXPECT( global_win32_process_heap, "failed to get process heap!" );
    }
    return global_win32_process_heap;
}
static time_t win32_filetime_to_posix( FILETIME ft ) {
    #define WIN32_TICKS_PER_SECOND (10000000)
    #define WIN32_TO_POSIX_DIFF    (11644473600ULL)

    ULARGE_INTEGER uli;
    uli.LowPart  = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    time_t res = (time_t)(
        (uli.QuadPart / WIN32_TICKS_PER_SECOND) - WIN32_TO_POSIX_DIFF );

    #undef WIN32_TICKS_PER_SECOND
    #undef WIN32_TO_POSIX_DIFF
    
    return res;
}
static wchar_t* win32_local_utf8_to_wide( CBPT(String) utf8 ) {
    CB_ASSERT( utf8.cc, "null pointer!" );
    CB_ASSERT(
        utf8.len < CBUILD_LOCAL_WIDE_CAPACITY,
        "attempted to convert a utf8 string longer than %u!",
        CBUILD_LOCAL_WIDE_CAPACITY );

    wchar_t* buf = (wchar_t*)local_byte_buffer();
    wchar_t* dst = buf;

    const char* src = utf8.cc;
    size_t rem       = utf8.len;

    while( rem ) {
        size_t max_convert = rem;
        if( max_convert >= INT32_MAX ) {
            max_convert = INT32_MAX;
        }
        // TODO(alicia): utf8 bounds checking!

        int written = MultiByteToWideChar(
            CP_UTF8, 0, src, max_convert, dst, max_convert );
        CB_EXPECT( written, "failed to convert utf8 string!" );

        src += max_convert;
        dst += written;
        rem -= max_convert;
    }

    return buf;
}
static CBPT(String) win32_local_wide_to_utf8( size_t len, wchar_t* wide ) {
    CB_ASSERT( wide, "null pointer!" );
    CB_ASSERT(
        len < CBUILD_LOCAL_WIDE_CAPACITY,
        "attempted to convert a wide string longer than %u!",
        CBUILD_LOCAL_WIDE_CAPACITY );

    char* buf = (char*)local_byte_buffer();
    char* dst = buf;

    wchar_t* src = wide;
    size_t rem    = len;
    while( rem ) {
        size_t max_convert = rem;
        if( max_convert >= INT32_MAX ) {
            max_convert = INT32_MAX;
        }
        // bounds checking
        const char* last_codepoint = (const char*)(src + (max_convert - 1));
        if( last_codepoint[0] & 0b10000000 ) {
            max_convert--;
            CB_ASSERT( max_convert, "invalid wide string!" );
        }
        
        int written = WideCharToMultiByte(
            CP_UTF8, 0, src, max_convert, dst, max_convert, 0, 0 );
        CB_EXPECT( written, "failed to convert wide string!" );

        src += max_convert;
        dst += written;
        rem -= max_convert;
    }

    return CBPFN(string_new)( dst - buf, buf );
}
static CBPT(String) win32_local_error_message( DWORD error_code ) {
    char* buf = (char*)local_byte_buffer();

    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code,
        0, buf, CB_LOCAL_BUFFER_CAPACITY, 0 );

    return string_from_cstr( buf );
}
static wchar_t* win32_local_path_canon( CBPT(String) path ) {
    #define PATH_RELATIVE 0
    #define PATH_HOME     1
    #define PATH_ABSOLUTE 2

    int path_type = PATH_RELATIVE;

    if( path.len >= sizeof("A:")  ) {
        if( isalpha( path.cc[0] ) && path.cc[1] == ':' ) {
            path_type = PATH_ABSOLUTE;
        }
    } else if( path.cc[0] == '~' ) {
        path_type = PATH_HOME;
    }

    CBPT(String) subpath = path;

    size_t min    = sizeof("\\\\?\\A:");
    size_t offset = sizeof("\\\\?\\") - 1;

    const char* home       = NULL;
    const char* home_drive = NULL;
    size_t home_drive_len  = 0;
    size_t home_len        = 0;
    size_t pwd_len         = 0;

    switch( path_type ) {
        case PATH_RELATIVE:  {
            pwd_len   = GetCurrentDirectoryW( 0, 0 );
        } break;
        case PATH_ABSOLUTE: break;
        case PATH_HOME: {
            home_drive = getenv( "HOMEDRIVE" );
            home       = getenv( "HOMEPATH" );

            home_drive_len = strlen( home_drive );
            home_len       = strlen( home );
        } break;
        default: break;
    }

    wchar_t* buf = (wchar_t*)local_byte_buffer();
    CBPFN(memory_copy)( buf, L"\\\\?\\", sizeof(L"\\\\?\\") - sizeof(wchar_t) );
    switch( path_type ) {
        case PATH_RELATIVE:  {
            offset += GetCurrentDirectoryW(
                CBUILD_LOCAL_WIDE_CAPACITY - offset, buf + offset );
        } break;
        case PATH_ABSOLUTE: break;
        case PATH_HOME: {
            MultiByteToWideChar(
                CP_UTF8, 0, home_drive, home_drive_len,
                buf + offset, CBUILD_LOCAL_WIDE_CAPACITY - offset );
            offset += home_drive_len;
            MultiByteToWideChar(
                CP_UTF8, 0, home, home_len,
                buf + offset, CBUILD_LOCAL_WIDE_CAPACITY - offset );
            offset += home_len;

            subpath = CBPFN(string_advance)( subpath );
        } break;
        default: break;
    }

    size_t last_chunk_len = 0;
    while( subpath.len ) {
        size_t sep = 0;
        if( CBPFN(string_find)( subpath, '/', &sep ) ) {
            if( !sep ) {
                subpath = CBPFN(string_advance)( subpath );
                continue;
            }

            CBPT(String) chunk = subpath;
            chunk.len    = sep;

            if( chunk.len < 3 ) {
                if( CBPFN(string_cmp)( chunk, CB_STRING( "." ) ) ) {
                    subpath = CBPFN(string_advance_by)( subpath, chunk.len + 1 );
                    continue;
                }
                if( CBPFN(string_cmp)( chunk, CB_STRING( ".." ) ) ) {
                    for( size_t i = offset; i-- > 0; ) {
                        wchar_t c = buf[i];
                        if( c == '\\' ) {
                            offset = i;
                            break;
                        }
                    }
                    if( offset < min ) {
                        offset = min;
                    }
                    buf[offset] = 0;
                    subpath = CBPFN(string_advance_by)( subpath, chunk.len + 1 );
                    continue;
                }
            }

            if( buf[offset - 1] != '\\' ) {
                buf[offset++] = '\\';
            }
            if( offset + chunk.len >= CBUILD_LOCAL_WIDE_CAPACITY ) {
                break;
            }
            MultiByteToWideChar(
                CP_UTF8, 0, chunk.cc, chunk.len,
                buf + offset, CBUILD_LOCAL_WIDE_CAPACITY - offset );
            offset += chunk.len;
            last_chunk_len = chunk.len;

            subpath = CBPFN(string_advance_by)( subpath, chunk.len + 1 );
        } else {
            if( CBPFN(string_cmp)( subpath, CB_STRING( "." ) ) ) {
                break;
            }
            if( CBPFN(string_cmp)( subpath, CB_STRING( ".." ) ) ) {
                for( size_t i = offset; i-- > 0; ) {
                    wchar_t c = buf[i];
                    if( c == '\\' ) {
                        offset = i;
                        break;
                    }
                }
                if( offset < min ) {
                    offset = min;
                }
                buf[offset] = 0;
                break;
            }

            if( buf[offset - 1] != '\\' ) {
                buf[offset++] = '\\';
            }
            if( offset + subpath.len >= CBUILD_LOCAL_WIDE_CAPACITY ) {
                break;
            }
            MultiByteToWideChar(
                CP_UTF8, 0, subpath.cc, subpath.len,
                buf + offset, CBUILD_LOCAL_WIDE_CAPACITY - offset );
            offset += subpath.len;
            break;
        }
    }
    buf[offset] = 0;

    #undef PATH_RELATIVE
    #undef PATH_HOME    
    #undef PATH_ABSOLUTE
    return buf;
}

void* CBPFN(heap_alloc)( size_t size ) {
    void* res = HeapAlloc( get_process_heap(), HEAP_ZERO_MEMORY, size );
    if( !res ) {
        return NULL;
    }
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, size );
        atomic_add64( &global_total_memory_usage, size );
    } else {
        global_memory_usage       += size;
        global_total_memory_usage += size;
    }
    return res;
}
void* CBPFN(heap_realloc)( void* memory, size_t old_size, size_t new_size ) {
    CB_ASSERT( new_size >= old_size, "attempted to reallocate to smaller buffer!" );
    void* res = HeapReAlloc( get_process_heap(), HEAP_ZERO_MEMORY, memory, new_size );
    if( !res ) {
        return NULL;
    }
    size_t diff = new_size - old_size;
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, diff );
        atomic_add64( &global_total_memory_usage, diff );
    } else {
        global_memory_usage       += diff;
        global_total_memory_usage += diff;
    }
    return res;
}
void CBPFN(heap_free)( void* memory, size_t size ) {
    if( !memory ) {
        CB_WARN( "attempted to free null pointer!" );
        return;
    }
    HeapFree( get_process_heap(), 0, memory );
    atom64 neg = size;
    neg = -neg;
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, neg );
    } else {
        global_memory_usage += neg;
    }
}
bool path_is_absolute( const char* path ) {
    CB_ASSERT( path, "null path!" );
    if( !path[0] || !path[1] ) {
        return false;
    }
    return
        isalpha( path[0] ) &&
        path[1] == ':';
}
static DWORD path_attributes( CBPT(String) path ) {
    wchar_t* wpath = win32_local_path_canon( path );
    return GetFileAttributesW( wpath );
}
bool path_is_directory( const char* path ) {
    CBPT(String) spath = string_from_cstr( path );
    DWORD attr   = path_attributes( spath );

    if( attr == INVALID_FILE_ATTRIBUTES ) {
        return false;
    } else {
        return attr & FILE_ATTRIBUTE_DIRECTORY;
    }
}
bool path_exists( const char* path ) {
    CB_ASSERT( path, "null path!" );
    CBPT(String) spath = string_from_cstr( path );
    return path_attributes( spath ) != INVALID_FILE_ATTRIBUTES;
}
static bool path_walk_dir_internal_long(
    CBPT(DString)** path, bool recursive, bool include_dirs,
    size_t* out_count, CBPT(DString)** out_buffer
) {
    size_t original_len = CBPFN(dstring_len)( *path );
    CBPT(DString)* _new = CBPFN(dstring_append)( *path, CB_STRING( "/*" ) );
    if( !_new ) {
        return false;
    }
    *path = _new;

    wchar_t* wpath = win32_local_path_canon( string_from_dstring( *path ) );

    WIN32_FIND_DATAW fd;
    CBPFN(memory_zero)( &fd, sizeof( fd ) );
    HANDLE find_file = FindFirstFileW( wpath, &fd );
    CBPFN(dstring_truncate)( *path, original_len );

    if( find_file == INVALID_HANDLE_VALUE ) {
        return false;
    }

    do {
        if(
            wcscmp( fd.cFileName, L"." )    == 0 ||
            wcscmp( fd.cFileName, L".." )   == 0 ||
            wcscmp( fd.cFileName, L".git" ) == 0
        ) {
            continue;
        }

        _new = CBPFN(dstring_push)( *path, '/' );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *path = _new;

        size_t name_len = wcslen( fd.cFileName );
        CBPT(String) npath   = win32_local_wide_to_utf8( name_len, fd.cFileName );

        _new = CBPFN(dstring_append)( *path, npath );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *path = _new;

        if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            if( include_dirs ) {
                _new = CBPFN(dstring_append)(
                    *out_buffer, CBPFN(string_new)( CBPFN(dstring_len)( *path ) + 1 , *path ) );
                if( !_new ) {
                    FindClose( find_file );
                    return false;
                }
                *out_buffer = _new;
                *out_count += 1;
            }

            if( recursive ) {
                if( !path_walk_dir_internal_long(
                    path, recursive, include_dirs, out_count, out_buffer
                ) ) {
                    FindClose( find_file );
                    return false;
                }
            }

            CBPFN(dstring_truncate)( *path, original_len );
            continue;
        }

        _new = CBPFN(dstring_append)(
            *out_buffer, CBPFN(string_new)( CBPFN(dstring_len)( *path ) + 1, *path ) );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *out_buffer = _new;
        *out_count += 1;

        CBPFN(dstring_truncate)( *path, original_len );
    } while( FindNextFileW( find_file, &fd ) != FALSE );

    FindClose( find_file );
    return true;
}
static bool path_walk_dir_internal(
    CBPT(DString)** path, bool recursive, bool include_dirs,
    size_t* out_count, CBPT(DString)** out_buffer
) {
    return path_walk_dir_internal_long(
        path, recursive, include_dirs, out_count, out_buffer );
}

static DWORD fd_open_dwaccess( CBPT(FileOpenFlag) flags ) {
    DWORD res = 0;
    if( flags & CB_FILE_OPEN_READ ) {
        res |= GENERIC_READ;
    }
    if( flags & CB_FILE_OPEN_WRITE ) {
        res |= GENERIC_WRITE;
    }
    return res;
}
static DWORD fd_open_dwcreationdisposition( CBPT(FileOpenFlag) flags ) {
    DWORD res = OPEN_EXISTING;
    if( flags & CB_FILE_OPEN_CREATE ) {
        res = CREATE_ALWAYS;
    } else if( flags & CB_FILE_OPEN_TRUNCATE ) {
        res = TRUNCATE_EXISTING;
    }
    return res;
}
static bool fd_open_long( CBPT(String) path, CBPT(FileOpenFlag) flags, CBPT(FileID)* out_file ) {
    DWORD dwDesiredAccess       = fd_open_dwaccess( flags );
    DWORD dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD dwCreationDisposition = fd_open_dwcreationdisposition( flags );
    DWORD dwFlagsAndAttributes  = 0;

    wchar_t* wide = win32_local_path_canon( path );

    HANDLE handle = CreateFileW(
        wide, dwDesiredAccess, dwShareMode, 0,
        dwCreationDisposition, dwFlagsAndAttributes, 0 );
    if( handle == INVALID_HANDLE_VALUE ) {
        DWORD error_code = GetLastError();
        CBPT(String) msg = win32_local_error_message( error_code );

        CB_ERROR(
            "failed to open '%S'! reason: (0x%X) %s", wide, error_code, msg.cc );
        return false;
    }

    *out_file = (size_t)handle;
    return true;
}

bool fd_open( const char* path, CBPT(FileOpenFlag) flags, CBPT(FileID)* out_file ) {
    if( !validate_file_flags( flags ) ) {
        return false;
    }
    CBPT(String) path_str = string_from_cstr( path );
    return fd_open_long( path_str, flags, out_file );
}
void fd_close( CBPT(FileID)* file ) {
    CloseHandle( (HANDLE)*file );
    *file = 0;
}

static bool fd_write_32(
    CBPT(FileID)* file, DWORD size, const void* buf, DWORD* opt_out_write_size
) {
    HANDLE hFile = (HANDLE)*file;
    bool is_console = GetFileType( hFile ) == FILE_TYPE_CHAR;

    DWORD out_size = 0;
    BOOL res = FALSE;
    if( is_console ) {
        // NOTE(alicia): actually UTF-8 encoded
        // because of _platform_init_ SetConsoleOutputCP to CP_UTF8
        res = WriteConsoleA( hFile, buf, size, &out_size, 0 );
    } else {
        res = WriteFile( hFile, buf, size, &out_size, 0 );
    }
    if( opt_out_write_size ) {
        *opt_out_write_size = out_size;
    }
    return res;
}
#if CB_ARCH_IS_64BIT
static bool fd_write_64(
    CBPT(FileID)* file, size_t size, const void* buf, size_t* opt_out_write_size
) {
    DWORD size0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD size1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    size_t write_total = 0;
    DWORD write_size  = 0;
    bool res = fd_write_32( file, size0, buf, &write_size );
    write_total = write_size;

    if( write_size != size0 ) {
        if( opt_out_write_size ) {
            *opt_out_write_size = write_total;
        }
        return res;
    }
    if( !res ) {
        return false;
    }

    if( size1 ) {
        res = fd_write_32( file, size1, (const uint8_t*)buf + size0, &write_size );
        if( !res ) {
            return false;
        }
        write_total += write_size;
    }

    if( opt_out_write_size ) {
        *opt_out_write_size = write_total;
    }
    return true;
}
#endif

static bool fd_read_32(
    CBPT(FileID)* file, DWORD size, void* buf, DWORD* opt_out_read_size
) {
    DWORD out_size = 0;
    BOOL res = ReadFile( (HANDLE)*file, buf, size, &out_size, 0 );
    if( opt_out_read_size ) {
        *opt_out_read_size = out_size;
    }
    return res;
}
#if CB_ARCH_IS_64BIT
static bool fd_read_64(
    CBPT(FileID)* file, size_t size, void* buf, size_t* opt_out_read_size
) {
    DWORD size0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD size1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    size_t read_total = 0;
    DWORD read_size  = 0;
    bool res = fd_read_32( file, size0, buf, &read_size );
    read_total = read_size;

    if( read_size != size0 ) {
        if( opt_out_read_size ) {
            *opt_out_read_size = read_total;
        }
        return res;
    }
    if( !res ) {
        return false;
    }

    if( size1 ) {
        res = fd_read_32( file, size1, (uint8_t*)buf + size0, &read_size );
        if( !res ) {
            return false;
        }
        read_total += read_size;
    }

    if( opt_out_read_size ) {
        *opt_out_read_size = read_total;
    }
    return true;
}
#endif

bool fd_write( CBPT(FileID)* file, size_t size, const void* buf, size_t* opt_out_write_size ) {
#if CB_ARCH_IS_64BIT
    return fd_write_64( file, size, buf, opt_out_write_size );
#else
    return fd_write_32( file, size, buf, (DWORD*)opt_out_write_size );
#endif
}
bool fd_read( CBPT(FileID)* file, size_t size, void* buf, size_t* opt_out_read_size ) {
#if CB_ARCH_IS_64BIT
    return fd_read_64( file, size, buf, opt_out_read_size );
#else
    return fd_read_32( file, size, buf, (DWORD*)opt_out_read_size );
#endif
}
bool fd_truncate( CBPT(FileID)* file ) {
    return SetEndOfFile( (HANDLE)*file ) != FALSE;
}
size_t fd_query_size( CBPT(FileID)* file ) {
#if CB_ARCH_IS_64BIT
    LARGE_INTEGER li;
    CB_EXPECT( GetFileSizeEx( (HANDLE)*file, &li ), "failed to get file size!" );
    return li.QuadPart;
#else
    DWORD res = GetFileSize( (HANDLE)*file, 0 );
    return res;
#endif
}
void fd_seek( CBPT(FileID)* file, CBPT(SeekType) type, intptr_t seek ) {
    DWORD dwMoveMethod = 0;
    switch( type ) {
        case CB_SEEK_TYPE_CURRENT: {
            dwMoveMethod = FILE_CURRENT;
        } break;
        case CB_SEEK_TYPE_BEGIN: {
            dwMoveMethod = FILE_BEGIN;
        } break;
        case CB_SEEK_TYPE_END: {
            dwMoveMethod = FILE_END;
        } break;
    }

#if CB_ARCH_IS_64BIT 
    LARGE_INTEGER li;
    li.QuadPart = seek;

    CB_EXPECT(
        SetFilePointerEx( (HANDLE)*file, li, 0, dwMoveMethod ) != FALSE,
        "failed to seek file!" );
#else
    SetFilePointer( (HANDLE)*file, seek, 0, dwMoveMethod );
#endif
}
size_t fd_query_position( CBPT(FileID)* file ) {
    DWORD dwMoveMethod = FILE_CURRENT;

#if CB_ARCH_IS_64BIT 
    LARGE_INTEGER li;
    li.QuadPart = 0;

    LARGE_INTEGER res;
    CB_EXPECT(
        SetFilePointerEx( (HANDLE)*file, li, &res, dwMoveMethod ) != FALSE,
        "failed to query file position!" );

    return res.QuadPart;
#else
    DWORD res = SetFilePointer( (HANDLE)*file, 0, 0, dwMoveMethod );
    return res;
#endif

}
static void file_query_time_long(
    CBPT(String) path, FILETIME* out_create, FILETIME* out_modify
) {
    DWORD dwDesiredAccess       = 0;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes  = 0;
    DWORD dwShareMode           =
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

    wchar_t* wpath = win32_local_path_canon( path );

    HANDLE handle = CreateFileW(
        wpath, dwDesiredAccess, dwShareMode,
        NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL );

    CB_EXPECT( handle != INVALID_HANDLE_VALUE, "failed to open file!" );
    CB_EXPECT(
        GetFileTime( handle, out_create, 0, out_modify ),
        "failed to get file time!" );

    CloseHandle( handle );
}
time_t file_query_time_create( const char* path ) {
    FILETIME create;
    CBPFN(memory_zero)( &create, sizeof(create) );

    size_t path_len = strlen( path );
    file_query_time_long( CBPFN(string_new)( path_len, path ), &create, 0 );

    return win32_filetime_to_posix( create );
}
time_t file_query_time_modify( const char* path ) {
    FILETIME modify;
    CBPFN(memory_zero)( &modify, sizeof(modify) );

    size_t path_len = strlen( path );
    file_query_time_long( CBPFN(string_new)( path_len, path ), 0, &modify );
    return win32_filetime_to_posix( modify );
}

static bool file_move_long( CBPT(String) dst, CBPT(String) src ) {
    wchar_t* dst_wide = win32_local_path_canon( dst );
    wchar_t* src_wide = win32_local_path_canon( src );

    bool res = MoveFileW( src_wide, dst_wide );
    return res == TRUE;
}

bool file_move( const char* dst, const char* src ) {
    CB_ASSERT( dst && src, "null path provided!" );
    size_t dst_len = strlen( dst );
    size_t src_len = strlen( src );

    return
        file_move_long( CBPFN(string_new)( dst_len, dst ), CBPFN(string_new)( src_len, src ) );
}

static bool file_copy_long( CBPT(String) dst, CBPT(String) src ) {
    wchar_t* dst_wide = win32_local_path_canon( dst );
    wchar_t* src_wide = win32_local_path_canon( src );

    bool res = CopyFileW( src_wide, dst_wide, FALSE );
    return res == TRUE;
}

bool file_copy( const char* dst, const char* src ) {
    CB_ASSERT( dst && src, "null path provided!" );
    size_t dst_len = strlen( dst );
    size_t src_len = strlen( src );

    return
        file_copy_long( CBPFN(string_new)( dst_len, dst ), CBPFN(string_new)( src_len, src ) );
}
static bool file_remove_long( CBPT(String) path ) {
    wchar_t* path_wide = win32_local_path_canon( path );
    return DeleteFileW( path_wide ) != FALSE;
}
bool file_remove( const char* path ) {
    size_t path_len = strlen( path );
    return file_remove_long( CBPFN(string_new)( path_len, path ) );
}
static bool dir_create_long( CBPT(String) path ) {
    wchar_t* wpath = win32_local_path_canon( path );
    if( CreateDirectoryW( wpath, 0 ) ) {
        return true;
    } else {
        DWORD res = GetLastError();
        return res == ERROR_ALREADY_EXISTS;
    }
}
bool dir_create( const char* path ) {
    size_t len = strlen( path );
    return dir_create_long( CBPFN(string_new)( len, path ) );
}
static bool dir_remove_internal_long( CBPT(String) path ) {
    wchar_t* wpath = win32_local_path_canon( path );
    return RemoveDirectoryW( wpath );
}
static bool dir_remove_internal( const char* path ) {
    size_t len = strlen( path );
    return dir_remove_internal_long( CBPFN(string_new)( len, path ) );
}

atom atomic_add( atom* atomic, atom val ) {
    return _InterlockedExchangeAdd( atomic, val );
}
atom64 atomic_add64( atom64* atom, atom64 val ) {
    return _InterlockedExchangeAdd64( atom, val );
}
atom atomic_compare_swap( atom* atomic, atom comp, atom exch ) {
    return _InterlockedCompareExchange( atomic, exch, comp );
}
atom64 atomic_compare_swap64( atom64* atom, atom64 comp, atom64 exch ) {
    return _InterlockedCompareExchange64( atom, exch, comp );
}

void fence(void) {
    MemoryBarrier();
}

double timer_milliseconds(void) {
    return timer_seconds() * 1000.0;
}
double timer_seconds(void) {
    LARGE_INTEGER qpf, qpc;
    QueryPerformanceFrequency( &qpf );
    QueryPerformanceCounter( &qpc );

    return (double)qpc.QuadPart / (double)qpf.QuadPart;
}

bool mutex_create( Mutex* out_mutex ) {
    HANDLE handle = CreateMutexA( 0, 0, 0 );
    if( !handle ) {
        return false;
    }
    out_mutex->handle = (void*)handle;
    return true;
}
bool mutex_is_valid( const Mutex* mutex ) {
    return mutex->handle != NULL;
}
void mutex_lock( Mutex* mutex ) {
    WaitForSingleObject( mutex->handle, INFINITE );
}
bool mutex_lock_timed( Mutex* mutex, uint32_t ms ) {
    if( ms == INFINITE ) {
        mutex_lock( mutex );
        return true;
    }
    DWORD  result = WaitForSingleObject( mutex->handle, ms );
    return result != WAIT_TIMEOUT;
}
void mutex_unlock( Mutex* mutex ) {
    ReleaseMutex( mutex->handle );
}
void mutex_destroy( Mutex* mutex ) {
    CloseHandle( mutex->handle );
    CBPFN(memory_zero)( mutex, sizeof(*mutex) );
}

bool semaphore_create( Semaphore* out_semaphore ) {
    HANDLE handle = CreateSemaphoreA( NULL, 0, INT32_MAX, 0 );
    if( !handle ) {
        return false;
    }
    out_semaphore->handle = handle;
    return true;
}
bool semaphore_is_valid( const Semaphore* semaphore ) {
    return semaphore->handle != NULL;
}
void semaphore_wait( Semaphore* semaphore ) {
    WaitForSingleObject( semaphore->handle, INFINITE );
}
bool semaphore_wait_timed( Semaphore* semaphore, uint32_t ms ) {
    if( ms == INFINITE ) {
        semaphore_wait( semaphore );
        return true;
    }
    DWORD  result = WaitForSingleObject( semaphore->handle, ms );
    return result != WAIT_TIMEOUT;
}
void semaphore_signal( Semaphore* semaphore ) {
    ReleaseSemaphore( semaphore->handle, 1, NULL );
}
void semaphore_destroy( Semaphore* semaphore ) {
    CloseHandle( semaphore->handle );
    CBPFN(memory_zero)( semaphore, sizeof(*semaphore) );
}

void thread_sleep( uint32_t ms ) {
    Sleep( ms );
}

void pipe_open( CBPT(PipeRead)* out_read, CBPT(PipeWrite)* out_write ) {
    HANDLE read = 0, write = 0;
    SECURITY_ATTRIBUTES sa;
    CBPFN(memory_zero)( &sa, sizeof(sa) );

    sa.nLength        = sizeof(sa);
    sa.bInheritHandle = TRUE;

    CB_EXPECT( CreatePipe( &read, &write, &sa, 0 ), "failed to create pipes!" );

    *out_read  = (intptr_t)read;
    *out_write = (intptr_t)write;
}
void pipe_close( CBPT(Pipe) pipe ) {
    HANDLE handle = (HANDLE)pipe;
    CloseHandle( handle );
}
bool process_in_path( const char* process_name ) {
    char* cmd = local_fmt( "where.exe %s /Q", process_name );
    return system( cmd ) == 0;
}
CBPT(ProcessID) process_exec(
    CBPT(Command) cmd, bool redirect_void, CBPT(PipeRead)* opt_stdin,
    CBPT(PipeWrite)* opt_stdout, CBPT(PipeWrite)* opt_stderr, const char* opt_cwd
) {
    STARTUPINFOW        startup;
    PROCESS_INFORMATION info;

    CBPFN(memory_zero)( &startup, sizeof(startup) );
    CBPFN(memory_zero)( &info,    sizeof(info) );

    startup.cb         = sizeof(startup);
    startup.hStdInput  = GetStdHandle( STD_INPUT_HANDLE );
    startup.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    startup.hStdError  = GetStdHandle( STD_ERROR_HANDLE );

    BOOL bInheritHandle = FALSE;
    if( redirect_void ) {
        bInheritHandle = TRUE;
        volatile struct GlobalBuffers* gb = get_global_buffers();
        startup.hStdInput  = (HANDLE)gb->void_read;
        startup.hStdOutput = (HANDLE)gb->void_write;
        startup.hStdError  = (HANDLE)gb->void_write;
    } else {
        if( opt_stdin ) {
            startup.hStdInput = (HANDLE)*opt_stdin;
            bInheritHandle    = TRUE;
        }
        if( opt_stdout ) {
            startup.hStdOutput = (HANDLE)*opt_stdout;
            bInheritHandle     = TRUE;
        }
        if( opt_stderr ) {
            startup.hStdError = (HANDLE)*opt_stderr;
            bInheritHandle    = TRUE;
        }
        if( bInheritHandle ) {
            startup.dwFlags |= STARTF_USESTDHANDLES;
        }
    }

    DWORD flags = 0;

    size_t cmd_line_utf8_len = 0;
    for( size_t i = 0; i < cmd.count; ++i ) {
        cmd_line_utf8_len += strlen( cmd.args[i] );
    }

    size_t wide_cmd_line_cap  = cmd_line_utf8_len + 8 + cmd.count;
    size_t wide_cmd_line_size = sizeof(wchar_t) * wide_cmd_line_cap;
    size_t wide_cmd_line_len  = 0;
    wchar_t* cmd_line = HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, wide_cmd_line_size );

    CB_EXPECT( cmd_line, "failed to allocate command line wide buffer!" );

    for( size_t i = 0; i < cmd.count; ++i ) {
        CBPT(String) current = string_from_cstr( cmd.args[i] );
        if( !current.cc || !current.len ) {
            continue;
        }

        bool contains_space = false;
        if( CBPFN(string_find)( current, ' ', 0 ) ) {
            cmd_line[wide_cmd_line_len++] = L'"';
            contains_space = true;
        }

        int len = MultiByteToWideChar(
            CP_UTF8, 0, current.cc, current.len, 
            cmd_line + wide_cmd_line_len, wide_cmd_line_cap - wide_cmd_line_len );
        wide_cmd_line_len += len;

        if( contains_space ) {
            cmd_line[wide_cmd_line_len++] = L'"';
        }

        if( i + 1 != cmd.count ) {
            cmd_line[wide_cmd_line_len++] = L' ';
        }
    }

    wchar_t* wide_cwd = NULL;
    if( opt_cwd ) {
        wide_cwd = win32_local_path_canon( string_from_cstr( opt_cwd ) );
        CB_INFO( "cd '%S'", wide_cwd );
    }

    CB_INFO( "%S", cmd_line );
    BOOL res = CreateProcessW(
        NULL, cmd_line, NULL, NULL, bInheritHandle,
        flags, NULL, wide_cwd, &startup, &info );
    HeapFree( GetProcessHeap(), 0, cmd_line );

    CB_EXPECT( res, "failed to launch process '%s'!", cmd.args[0] );

    CloseHandle( info.hThread );
    return (intptr_t)info.hProcess;
}
int process_wait( CBPT(ProcessID) pid ) {
    DWORD res = WaitForSingleObject( (HANDLE)pid, INFINITE );
    switch( res ) {
        case WAIT_OBJECT_0: break;
        default: {
            CBPT(String) reason = win32_local_error_message( GetLastError() );
            CB_PANIC( "failed to wait for pid! reason: %s", reason.cc );
            return -1;
        } break;
    }

    DWORD exit_code = 0;
    CB_EXPECT( GetExitCodeProcess(
        (HANDLE)pid, &exit_code ), "failed to get exit code!" );

    CloseHandle( (HANDLE)pid );
    return exit_code;
}
bool process_wait_timed( CBPT(ProcessID) pid, int* opt_out_res, uint32_t ms ) {
    bool success = true;
    DWORD res   = WaitForSingleObject( (HANDLE)pid, ms );
    switch( res ) {
        case WAIT_OBJECT_0: break;
        case WAIT_TIMEOUT: {
            success = false;
        } break;
        default: {
            CBPT(String) reason = win32_local_error_message( GetLastError() );
            CB_PANIC( "failed to wait for pid! reason: %s", reason.cc );
            return -1;
        } break;
    }

    DWORD exit_code = 0;
    CB_EXPECT( GetExitCodeProcess(
        (HANDLE)pid, &exit_code ), "failed to get exit code!" );

    if( success ) {
        CloseHandle( (HANDLE)pid );
    }

    *opt_out_res = exit_code;
    return success;
}
void process_discard( CBPT(ProcessID) pid ) {
    CloseHandle( (HANDLE)pid );
}

unsigned int win32_thread_proc( void* params ) {
    struct Win32ThreadParams* p = params;
    global_thread_id = p->id;
    
    fence();
    p->proc( p->params );
    fence();

    return 0;
}

void thread_create( JobFN* func, void* params ) {
    CB_EXPECT(
        global_thread_id_source < (CB_THREAD_COUNT + 1),
        "exceeded maximum number of threads!" );

    uint32_t id = atomic_add( &global_thread_id_source, 1 );

    struct Win32ThreadParams* p = global_win32_thread_params + (id - 1);
    p->id     = id;
    p->params = params;
    p->proc   = func;

    fence();

    HANDLE h = (HANDLE)_beginthreadex( 0, 0, win32_thread_proc, p, 0, 0 );
    CB_EXPECT( h != NULL, "failed to create thread!" );
}

char* internal_cwd(void) {
    DWORD len = GetCurrentDirectoryA( 0, 0 );
    char* buf = CBPFN(heap_alloc)( len );

    CB_EXPECT( buf, "failed to allocate working directory buffer!" );

    GetCurrentDirectoryA( len, buf );

    for( size_t i = 0; i < len; ++i ) {
        if( buf[i] == '\\' ) {
            buf[i] = '/';
        }
    }

    return buf;
}
char* internal_home(void) {
    const char* drive = getenv( "HOMEDRIVE" );
    CB_EXPECT( drive, "failed to get home directory drive!" );
    const char* home  = getenv( "HOMEPATH" );
    CB_EXPECT( home, "failed to get home path!" );

    CBPT(DString)* buf = CBPFN(dstring_concat_cstr)( drive, home );
    CB_EXPECT( buf, "failed to allocate home directory buffer!" );

    size_t len = CBPFN(dstring_len)( buf );
    for( size_t i = 0; i < len; ++i ) {
        if( buf[i] == '\\' ) {
            buf[i] = '/';
        }
    }

    return buf;
}

#else /* WINDOWS end */
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>

volatile atom global_semaphore_val = 0;

struct PosixMutex {
    atom value;
#if CB_ARCH_IS_64BIT
    uint32_t  __padding;
#endif
};

struct PosixThreadParams {
    JobFN* proc;
    void*  params;
    uint32_t    id;
};
static struct PosixThreadParams global_posix_thread_params[CB_THREAD_COUNT];

void _platform_init_(void) {
    return;
}

static struct timespec ms_to_timespec( uint32_t ms ) {
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    return ts;
}
static double timespec_to_ms( struct timespec* ts ) {
    return ((double)ts->tv_sec * 1000.0) + ((double)ts->tv_nsec / 1000000.0);
}
static const char* generate_semaphore_name(void) {
    atom val = atomic_add( &global_semaphore_val, 1 );
    return (const char*)local_fmt( "cbuild_sem%u", val );
}

void* CBPFN(heap_alloc)( size_t size ) {
    void* res = malloc( size );
    if( !res ) {
        return NULL;
    }
    memset( res, 0, size );
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, size );
        atomic_add64( &global_total_memory_usage, size );
    } else {
        global_memory_usage       += size;
        global_total_memory_usage += size;
    }
    return res;
}
void* CBPFN(heap_realloc)( void* memory, size_t old_size, size_t new_size ) {
    CB_ASSERT( new_size >= old_size, "attempted to reallocate to smaller buffer!" );
    void* res = realloc( memory, new_size );
    if( !res ) {
        return NULL;
    }
    size_t diff = new_size - old_size;
    memset( res + old_size, 0, diff );
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, diff );
        atomic_add64( &global_total_memory_usage, diff );
    } else {
        global_memory_usage       += diff;
        global_total_memory_usage += diff;
    }
    return res;
}
void CBPFN(heap_free)( void* memory, size_t size ) {
    if( !memory ) {
        CB_WARN( "attempted to free null pointer!" );
        return;
    }
    free( memory );
    atom64 neg = size;
    neg = -neg;
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, neg );
    } else {
        global_memory_usage += neg;
    }
}
bool path_is_absolute( const char* path ) {
    return *path == '/';
}
bool path_exists( const char* path ) {
    return access( path, F_OK ) == 0;
}
bool path_is_directory( const char* path ) {
    struct stat s;
    int res = stat( path, &s );
    if( res == -1 ) {
        return false;
    }

    return S_ISDIR( s.st_mode );
}
static bool path_walk_dir_internal(
    CBPT(DString)** path, bool recursive, bool include_dirs,
    size_t* out_count, CBPT(DString)** out_buffer
) {
    struct dirent* entry;
    DIR* dir = opendir( *path );
    if( !dir ) {
        return false;
    }

    size_t original_len = CBPFN(dstring_len)( *path );
    long  pos = 0;

    while( (entry = readdir( dir )) ) {
        if(
            strcmp( entry->d_name, "." )    == 0 ||
            strcmp( entry->d_name, ".." )   == 0 ||
            strcmp( entry->d_name, ".git" ) == 0
        ) {
            continue;
        }
        CBPT(DString)* _new = CBPFN(dstring_push)( *path, '/' );
        if( !_new ) {
            closedir( dir );
            return false;
        }
        *path = _new;

        size_t name_len = strlen( entry->d_name );
        _new = CBPFN(dstring_append)( *path, CBPFN(string_new)( name_len, entry->d_name ) );
        if( !_new ) {
            closedir( dir );
            return false;
        }
        *path = _new;

        struct stat st;
        stat( *path, &st );

        if( S_ISDIR( st.st_mode ) ) {
            if( include_dirs ) {
                _new = CBPFN(dstring_append)(
                    *out_buffer, CBPFN(string_new)( CBPFN(dstring_len)( *path ) + 1, *path ) );
                if( !_new ) {
                    closedir( dir );
                    return false;
                }
                *out_buffer = _new;

                *out_count += 1;
            }

            if( recursive ) {
                pos = telldir( dir );
                closedir( dir );

                if( !path_walk_dir_internal(
                    path, recursive, include_dirs,
                    out_count, out_buffer
                ) ) {
                    return false;
                }

                CBPFN(dstring_truncate)( *path, original_len );
                dir = opendir( *path );

                seekdir( dir, pos );
            }

            CBPFN(dstring_truncate)( *path, original_len );
            continue;
        }

        _new = CBPFN(dstring_append)(
            *out_buffer, CBPFN(string_new)( CBPFN(dstring_len)( *path ) + 1, *path ) );
        if( !_new ) {
            closedir( dir );
            return false;
        }
        *out_buffer = _new;

        *out_count += 1;

        CBPFN(dstring_truncate)( *path, original_len );
    }

    closedir( dir );
    return true;
}

bool fd_open( const char* path, CBPT(FileOpenFlag) flags, CBPT(FileID)* out_file ) {
    if( !validate_file_flags( flags ) ) {
        return false;
    }
    int oflag = 0;
    if( (flags & (CB_FILE_OPEN_READ | CB_FILE_OPEN_WRITE)) == CB_FILE_OPEN_READ ) {
        oflag |= O_RDONLY;
    } else if( (flags & (CB_FILE_OPEN_READ | CB_FILE_OPEN_WRITE)) == CB_FILE_OPEN_WRITE ) {
        oflag |= O_WRONLY;
    } else { // read + write
        oflag |= O_RDWR;
    }
    if( flags & CB_FILE_OPEN_TRUNCATE ) {
        oflag |= O_TRUNC;
    }
    if( flags & CB_FILE_OPEN_CREATE ) {
        oflag |= O_CREAT;
    }
    if( flags & CB_FILE_OPEN_APPEND ) {
        oflag |= O_APPEND;
    }

    mode_t mode = S_IRUSR | S_IWUSR;

    int fd = open( path, oflag, mode );
    if( fd < 0 ) {
        CB_ERROR( "fd_open: failed to open '%s'!", path );
        return false;
    }

    *out_file = fd;
    return true;
}
void fd_close( CBPT(FileID)* file ) {
    close( *file );
    *file = 0;
}
bool fd_write( CBPT(FileID)* file, size_t size, const void* buf, size_t* opt_out_write_size ) {
    intptr_t write_size = write( *file, buf, size );
    if( write_size < 0 ) {
        return false;
    }

    if( opt_out_write_size ) {
        *opt_out_write_size = write_size;
    }
    return true;
}
bool fd_read( CBPT(FileID)* file, size_t size, void* buf, size_t* opt_out_read_size ) {
    intptr_t read_size = read( *file, buf, size );
    if( read_size < 0 ) {
        return false;
    }

    if( opt_out_read_size ) {
        *opt_out_read_size = read_size;
    }

    return true;
}
bool fd_truncate( CBPT(FileID)* file ) {
    size_t pos = fd_query_position( file ); // fd_query_position handles lseek fail
    return ftruncate( *file, pos ) == 0;
}
size_t fd_query_size( CBPT(FileID)* file ) {
    off_t pos = lseek( *file, 0, SEEK_CUR );
    CB_EXPECT( pos >= 0, "failed to query file descriptor size!" );
    off_t res = lseek( *file, 0, SEEK_END );
    CB_EXPECT( res >= 0, "failed to query file descriptor size!" );
    CB_EXPECT( lseek( *file, pos, SEEK_SET ) >= 0,
        "failed to query file descriptor size!" );

    return res;
}
void fd_seek( CBPT(FileID)* file, CBPT(SeekType) type, intptr_t seek ) {
    static const int local_whence[] = {
        SEEK_CUR, // CB_SEEK_TYPE_CURRENT
        SEEK_SET, // CB_SEEK_TYPE_BEGIN
        SEEK_END, // CB_SEEK_TYPE_END
    };
    CB_EXPECT( lseek( *file, seek, local_whence[type] ) >= 0, "failed to seek!" );
}
size_t fd_query_position( CBPT(FileID)* file ) {
    off_t pos = lseek( *file, 0, SEEK_CUR );
    CB_EXPECT( pos >= 0, "failed to get current file position!" );
    return pos;
}
time_t file_query_time_create( const char* path ) {
    struct stat st;
    CB_EXPECT( stat( path, &st ) == 0,
        "failed to query create time for '%s'!", path );
    return st.st_ctime;
}
time_t file_query_time_modify( const char* path ) {
    struct stat st;
    CB_EXPECT( stat( path, &st ) == 0,
        "failed to query modify time for '%s'!", path );
    return st.st_mtime;
}
bool file_move( const char* dst, const char* src ) {
    return rename( src, dst ) == 0;
}
bool file_copy( const char* dst, const char* src ) {
    CBPT(FileID) src_file, dst_file;
    if( !fd_open( src, CB_FILE_OPEN_READ, &src_file ) ) {
        return false;
    }
    if( path_exists( dst ) ) {
        if( !fd_open( dst, CB_FILE_OPEN_WRITE | CB_FILE_OPEN_TRUNCATE, &dst_file ) ) {
            fd_close( &src_file );
            return false;
        }
    } else {
        if( !fd_open( dst, CB_FILE_OPEN_WRITE | CB_FILE_OPEN_CREATE, &dst_file ) ) {
            fd_close( &src_file );
            return false;
        }
    }

    char* buf = (char*)local_byte_buffer();
    size_t rem = fd_query_size( &src_file );

    while( rem ) {
        size_t max_write = rem;
        if( rem > CB_LOCAL_BUFFER_CAPACITY ) {
            max_write = CB_LOCAL_BUFFER_CAPACITY;
        }

        if( !fd_read( &src_file, max_write, buf, 0 ) ) {
            fd_close( &src_file );
            fd_close( &dst_file );
            return false;
        }
        if( !fd_write( &dst_file, max_write, buf, 0 ) ) {
            fd_close( &src_file );
            fd_close( &dst_file );
            return false;
        }

        rem -= max_write;
    }

    fd_close( &src_file );
    fd_close( &dst_file );
    return true;
}
bool file_remove( const char* path ) {
    int res = remove( path );
    return res == 0;
}
bool dir_create( const char* path ) {
    int res = mkdir( path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    if( res == 0 ) {
        return true;
    }
    switch( errno ) {
        case EEXIST: return true;
        default:     return false;
    }
}
static bool dir_remove_internal( const char* path ) {
    int res = rmdir( path );
    return res == 0;
}

atom atomic_add( atom* atomic, atom val ) {
    return __sync_fetch_and_add( atomic, val );
}
atom64 atomic_add64( atom64* atom, atom64 val ) {
    return __sync_fetch_and_add( atom, val );
}
atom atomic_compare_swap( atom* atomic, atom comp, atom exch ) {
    return __sync_val_compare_and_swap( atomic, comp, exch );
}
atom64 atomic_compare_swap64( atom64* atom, atom64 comp, atom64 exch ) {
    return __sync_val_compare_and_swap( atom, comp, exch );
}
void fence(void) {
#if CB_ARCH_CURRENT == CB_ARCH_X86 && CB_ARCH_IS_64BIT
    __asm__ volatile ("mfence":::"memory");
#elif CB_ARCH_CURRENT == CB_ARCH_ARM
    __asm__ volatile ("dmb":::"memory");
#else
    __asm__ volatile ("":::"memory");
#endif
}

bool mutex_create( Mutex* out_mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)out_mutex;
    mtx->value = 0;
    return true;
}
bool mutex_is_valid( const Mutex* mutex ) {
    CB_UNUSED( mutex );
    return true;
}
void mutex_lock( Mutex* mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)mutex;
    while( atomic_compare_swap( &mtx->value, 0, mtx->value + 1 ) != 0 ) {
        thread_sleep(1);
    }
}
bool mutex_lock_timed( Mutex* mutex, uint32_t ms ) {
    if( ms == MT_WAIT_INFINITE ) {
        mutex_lock( mutex );
        return true;
    }

    struct PosixMutex* mtx = (struct PosixMutex*)mutex;

    for( uint32_t i = 0; i < ms; ++i ) {
        if( atomic_compare_swap( &mtx->value, 0, mtx->value + 1 ) != 0 ) {
            thread_sleep(1);
            continue;
        } else {
            return true;
        }
    }
    return false;
}
void mutex_unlock( Mutex* mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)mutex;
    atomic_compare_swap( &mtx->value, mtx->value, mtx->value - 1 );
}
void mutex_destroy( Mutex* mutex ) {
    mutex->handle = 0;
}

bool semaphore_create( Semaphore* out_semaphore ) {
    sem_t* s = sem_open( generate_semaphore_name(), O_CREAT, 0644, 0 );
    if( !s ) {
        const char* reason = "unknown";

        // reason for not handling case is commented below.
        // technically all of these errors should lead
        // to program being aborted but that's left up
        // to the programmer to decide in this case.
        switch( errno ) {
            case ENFILE:
            case EMFILE: {
                reason = "too many open file descriptors.";
            } break;
            case ENOMEM: {
                reason = "out of memory.";
            } break;
            case EACCES: // this function does not open existing semaphores.
            case EEXIST: // this function should not be capable of creating
                         // semaphore with existing name.
            case EINVAL: // value is never greater than SEM_VALUE_MAX
            case ENAMETOOLONG: // name can never be too long
            case ENOENT: // same as EACCES
            default: break;
        }

        CB_ERROR( "failed to create semaphore! reason: %s", reason );
        return false;
    }
    out_semaphore->handle = s;
    return true;
}
bool semaphore_is_valid( const Semaphore* semaphore ) {
    return semaphore->handle != NULL;
}
void semaphore_wait( Semaphore* semaphore ) {
    CB_EXPECT( sem_wait( semaphore->handle ) == 0, "failed to wait on semaphore!" );
}
bool semaphore_wait_timed( Semaphore* semaphore, uint32_t ms ) {
    if( ms == MT_WAIT_INFINITE ) {
        semaphore_wait( semaphore );
        return true;
    }

    struct timespec ts = ms_to_timespec( ms );
    int res = sem_timedwait( semaphore->handle, &ts );

    if( res == ETIMEDOUT ) {
        // only error that is actually expected,
        // anything else should abort the program
        return false;
    } else {
        CB_EXPECT( res == 0, "failed to wait on semaphore!" );
        return true;
    }
}
void semaphore_signal( Semaphore* semaphore ) {
    CB_EXPECT( sem_post( semaphore->handle ) == 0, "failed to post semaphore!" );
}
void semaphore_destroy( Semaphore* semaphore ) {
    sem_close( semaphore->handle );
    *semaphore = semaphore_null();
}

void pipe_open( CBPT(PipeRead)* out_read, CBPT(PipeWrite)* out_write ) {
    int fd[2];
    CB_EXPECT( pipe( fd ) != -1, "failed to create pipes!" );
    *out_read  = fd[0];
    *out_write = fd[1];
}
void pipe_close( CBPT(Pipe) pipe ) {
    close( pipe );
}

bool process_in_path( const char* process_name ) {
    CBPT(Command) cmd = command_new( "which", process_name );

    CBPT(ProcessID) pid = process_exec( cmd, true, 0, 0, 0, 0 );
    int res = process_wait( pid );

    return res == 0;
}
void posix_process_replace( CBPT(Command) cmd ) {
    execvp( cmd.args[0], (char* const*)cmd.args );
}
CBPT(ProcessID) process_exec(
    CBPT(Command) cmd, bool redirect_void, CBPT(PipeRead)* opt_stdin,
    CBPT(PipeWrite)* opt_stdout, CBPT(PipeWrite)* opt_stderr, const char* opt_cwd
) {
    CBPT(PipeRead)   stdin_;
    CBPT(PipeWrite) stdout_;
    CBPT(PipeWrite) stderr_;

    if( redirect_void ) {
        volatile struct GlobalBuffers* gb = get_global_buffers();
        stdin_  = gb->void_read;
        stdout_ = gb->void_write;
        stderr_ = gb->void_write;
    } else {
        stdin_  = opt_stdin  ? *opt_stdin  : STDIN_FILENO;
        stdout_ = opt_stdout ? *opt_stdout : STDOUT_FILENO;
        stderr_ = opt_stderr ? *opt_stderr : STDERR_FILENO;
    }

    pid_t pid = fork();
    CB_EXPECT( pid >= 0, "failed to fork!" );

    if( pid ) { // thread that ran process_exec
        if( opt_cwd ) {
            CB_INFO( "cd '%s'", opt_cwd );
        }
        CBPT(DString)* flat = command_flatten_dstring( &cmd );
        if( flat ) {
            CB_INFO( "%s", flat );
            CBPFN(dstring_free)( flat );
        }
        return pid;
    }

    // thread where process will run

    CB_EXPECT_CRASH( dup2( stdin_ , STDIN_FILENO  ) >= 0, "failed to setup stdin!" );
    CB_EXPECT_CRASH( dup2( stdout_, STDOUT_FILENO ) >= 0, "failed to setup stdout!" );
    CB_EXPECT_CRASH( dup2( stderr_, STDERR_FILENO ) >= 0, "failed to setup stderr!" );

    if( opt_cwd ) {
        chdir( opt_cwd );
    }

    CB_EXPECT_CRASH( execvp(
        cmd.args[0], (char* const*)(cmd.args) // cmd.args always has a null string at the end
    ) >= 0, "failed to execute command!" );
    exit(0);
}
int process_wait( CBPT(ProcessID) pid ) {
    int wstatus = 0;
    CB_EXPECT( waitpid( pid, &wstatus, 0 ) == pid, "failed to wait for process!" );

    if( WIFEXITED( wstatus ) ) {
        int status = WEXITSTATUS( wstatus );
        return status;
    }

    return -1;
}
bool process_wait_timed( CBPT(ProcessID) pid, int* opt_out_res, uint32_t ms ) {
    if( ms == MT_WAIT_INFINITE ) {
        int res = process_wait( pid );
        if( opt_out_res ) {
            *opt_out_res = res;
        }
        return true;
    }

    for( uint32_t i = 0; i < ms; ++i ) {
        int wstatus = 0;
        pid_t v = waitpid( pid, &wstatus, WNOHANG );
        CB_EXPECT( v != -1, "failed to wait for process!" );

        if( v == 0 ) {
            thread_sleep(1);
            continue;
        }

        if( opt_out_res ) {
            if( WIFEXITED( wstatus ) ) {
                *opt_out_res = WEXITSTATUS( wstatus );
            } else {
                *opt_out_res = -1;
            }
        }

        return true;
    }

    return false;
}
void process_discard( CBPT(ProcessID) pid ) {
    CB_UNUSED(pid); // this is not necessary on POSIX
}

double timer_milliseconds(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    fence();
    return timespec_to_ms( &ts );
}
double timer_seconds(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    fence();
    return timespec_to_ms( &ts ) / 1000.0;
}

void thread_sleep( uint32_t ms ) {
    struct timespec ts = ms_to_timespec( ms );
    struct timespec rem;
    CBPFN(memory_zero)( &rem, sizeof(rem) );

    while( clock_nanosleep( CLOCK_REALTIME, 0, &ts, &rem ) ) {
        ts = rem;
    }
}

void* posix_thread_proc( void* params ) {
    struct PosixThreadParams* p = params;
    global_thread_id = p->id;

    fence();
    p->proc( p->params );
    fence();

    return 0;
}

void thread_create( JobFN* func, void* params ) {
    CB_EXPECT(
        global_thread_id_source < (CB_THREAD_COUNT + 1),
        "exceeded maximum number of threads! max: %u", CB_THREAD_COUNT );

    uint32_t id = atomic_add( &global_thread_id_source, 1 );

    struct PosixThreadParams* p = global_posix_thread_params + (id - 1);
    p->id     = id;
    p->params = params;
    p->proc   = func;

    fence();

    pthread_t thread;
    int res = pthread_create( &thread, NULL, posix_thread_proc, p );
    CB_EXPECT( res == 0, "failed to create thread!" );
}

char* internal_cwd(void) {
    char* buf = CBPFN(heap_alloc)( PATH_MAX );
    CB_EXPECT( buf, "failed to allocate working directory buffer!" );

    char* res = getcwd( buf, PATH_MAX );
    CB_EXPECT( res, "failed to get working directory!" );

    return res;
}
char* internal_home(void) {
    char* home = getenv( "HOME" );
    CB_EXPECT( home, "failed to get home directory!" );

    return home;
}

#endif /* POSIX end */

#endif /* CB_IMPLEMENTATION end */
