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
 *    - define CB_DISABLE_TYPEDEFS to disable 
 *      defining u8/16/32/64/size, i8/16/32/64/size and f32/64
 *
 *    - define CB_STRIP_PREFIXES to strip prefixes from functions and types.
 *      This does not strip prefixes from macro constants or functions.
 *
 * @author Alicia Amarilla (smushyaa@gmail.com)
 *
 * @date May 28, 2024
 *
 * @copyright MIT License.
*/
// IWYU pragma: begin_exports
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE   700
#if !defined(_LARGEFILE64_SOURCE)
    #define _LARGEFILE64_SOURCE
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// IWYU pragma: end_exports

// NOTE(alicia): forward declarations.

extern void exit( int status );


#if !(defined(CB_ALLOC) && defined(CB_FREE))

    /// @brief Allocate new memory.
    /// @note
    /// CBuild expects this macro function to zero newly allocated memory.
    /// @param[in] opt_ptr      (optional) Old pointer, can be NULL.
    /// @param     opt_old_size (optional) Size if @c opt_ptr is provided.
    /// @param     new_size     Bytes to allocate.
    /// @return Pointer to new memory.
    #define CB_ALLOC( opt_ptr, opt_old_size, new_size ) \
        (void*)((char*)memset( \
            (char*)realloc( opt_ptr, new_size ) \
                + opt_old_size, 0, new_size - opt_old_size \
            ) - opt_old_size )

    /// @brief Free memory.
    /// @param[in] ptr  Pointer to memory to free.
    /// @param     size Size of memory to free.
    #define CB_FREE( ptr, size ) \
        free( ptr )

#endif

// NOTE(alicia): Macro Constants

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

/// @brief Architecture is little endian.
#define CB_ARCH_LITTLE_ENDIAN 1
/// @brief Architecture is big endian.
#define CB_ARCH_BIG_ENDIAN    2

#if !defined(CB_ARCH_COUNT)
    /// @brief Number of architecture enum variants.
    #define CB_ARCH_COUNT 3
#endif
// CB_ARCH_CURRENT
// CB_ARCH_CURRENT_NAME
// CB_ARCH_CURRENT_BYTE_ORDER
// CB_ARCH_WORD_SIZE          (realistically, 64 or 32, defaults to 32)
// CB_ARCH_IS_64BIT           (1 or undefined)
// CB_ARCH_IS_LITTLE_ENDIAN   (1 or undefined)

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

        #if !defined(CB_ARCH_CURRENT_BYTE_ORDER)
            #define CB_ARCH_CURRENT_BYTE_ORDER \
                CB_ARCH_LITTLE_ENDIAN
        #endif

    #elif defined(__arm__) || defined(_M_ARM)

        /// @brief Current architecture is ARM.
        #define CB_ARCH_CURRENT CB_ARCH_ARM

        #if !defined(CB_ARCH_WORD_SIZE)
            /// @brief Current word size is 32-bit.
            #define CB_ARCH_WORD_SIZE 32
        #endif
    
        #if !defined(CB_ARCH_CURRENT_BYTE_ORDER)
            #define CB_ARCH_CURRENT_BYTE_ORDER \
                CB_ARCH_LITTLE_ENDIAN
        #endif

    #elif defined(__X86__) || defined(__x86_64__)

        /// @brief Current architecture is x86.
        #define CB_ARCH_CURRENT CB_ARCH_X86

        #if !defined(CB_ARCH_WORD_SIZE)
            /// @brief Current word size is 64-bit.
            #define CB_ARCH_WORD_SIZE 64
        #endif

        #if !defined(CB_ARCH_CURRENT_BYTE_ORDER)
            #define CB_ARCH_CURRENT_BYTE_ORDER \
                CB_ARCH_LITTLE_ENDIAN
        #endif

    #elif defined(__i386__) || defined(_M_IX86)

        /// @brief Current architecture is x86.
        #define CB_ARCH_CURRENT CB_ARCH_X86

        #if !defined(CB_ARCH_WORD_SIZE)
            /// @brief Current word size is 32-bit.
            #define CB_ARCH_WORD_SIZE 32
        #endif

        #if !defined(CB_ARCH_CURRENT_BYTE_ORDER)
            #define CB_ARCH_CURRENT_BYTE_ORDER \
                CB_ARCH_LITTLE_ENDIAN
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

#if !defined(CB_ARCH_IS_LITTLE_ENDIAN) && CB_ARCH_CURRENT_BYTE_ORDER == CB_ARCH_LITTLE_ENDIAN
    #define CB_ARCH_IS_LITTLE_ENDIAN 1
#endif

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

#if !defined(CB_RECOMPILE_COMPILER)
    #if defined(__cplusplus)
        #define CB_RECOMPILE_COMPILER \
            CB_COMPILER_CURRENT_CPP_CMD
    #else
        #define CB_RECOMPILE_COMPILER \
            CB_COMPILER_CURRENT_C_CMD
    #endif
#endif

#if !defined(CB_LOCAL_BUFFER_COUNT) || CB_LOCAL_BUFFER_COUNT < 2
    #undef CB_LOCAL_BUFFER_COUNT

    /// @brief Number of local buffers per thread.
    #define CB_LOCAL_BUFFER_COUNT    (4)
#endif
/// @brief Capacity of local buffers in bytes.
#define CB_LOCAL_BUFFER_CAPACITY (CB_KIBIBYTES(4))

#if defined(__cplusplus)
    #define restrict __restrict
#endif

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    /// @brief Mark function as no return.
    #define CB_NO_RETURN __declspec( noreturn )
#else
    /// @brief Mark function as no return.
    #define CB_NO_RETURN _Noreturn
#endif

/// @brief Color code black.
#define CB_COLOR_BLACK   "\033[1;30m"
/// @brief Color code white.
#define CB_COLOR_WHITE   "\033[1;37m"
/// @brief Color code red.
#define CB_COLOR_RED     "\033[1;31m"
/// @brief Color code green.
#define CB_COLOR_GREEN   "\033[1;32m"
/// @brief Color code blue.
#define CB_COLOR_BLUE    "\033[1;34m"
/// @brief Color code magenta.
#define CB_COLOR_MAGENTA "\033[1;35m"
/// @brief Color code yellow.
#define CB_COLOR_YELLOW  "\033[1;33m"
/// @brief Color code cyan.
#define CB_COLOR_CYAN    "\033[1;36m"
/// @brief Color code to reset color.
#define CB_COLOR_RESET   "\033[1;00m"

/// @brief UTF-8 code point for replacement character.
#define CB_UNICODE_CP8_REPLACEMENT_CHARACTER \
    cb_cp8_from_code_units( 0xEF, 0xBF, 0xBD, 0 )
    
/// @brief UTF-16 code point for replacement character.
#define CB_UNICODE_CP16_REPLACEMENT_CHARACTER \
    cb_cp16_from_code_units( 0xFFFD, 0 )
    
/// @brief UTF-32 code point for replacement character.
#define CB_UNICODE_CP32_REPLACEMENT_CHARACTER \
    cb_cp32_from_code_units( 0xFFFD )

/// @brief Wait indefinitely for something.
#define CB_WAIT_INFINITE 0xFFFFFFFF

// NOTE(alicia): Macro Functions

#if defined(__cplusplus)
    /// @brief Define struct.
    #define CB_STRUCT(x) x
#else
    /// @brief Define struct.
    #define CB_STRUCT(x) (x)
#endif

/// @brief Mark any variables/parameters as unused.
/// @param ... Variables to be marked as unused.
#define CB_UNUSED( ... ) \
    _cb_internal_unused( 0, __VA_ARGS__ )

/// @brief Create a new dynamic string from string literal.
/// @param literal (string literal) Literal to make string from.
/// @return New dynamic string.
#define CB_DSTRING( literal ) \
    dstring_new( sizeof(literal) - 1, literal )

/// @brief Insert an abnormal exit.
#define CB_INSERT_ABNORMAL_EXIT() exit(-1)

#if CB_COMPILER_CURRENT == CB_COMPILER_MSVC
    /// @brief Insert unreachable statement.
    #define CB_INSERT_UNREACHABLE() __assume(0)

    /// @brief Insert a trap statement. (crash program)
    #define CB_INSERT_TRAP() __debugbreak()

    /// @brief Insert a debugger break statement.
    #define CB_DEBUG_BREAK() __debugbreak()
#else
    /// @brief Insert unreachable statement.
    #define CB_INSERT_UNREACHABLE() __builtin_unreachable()

    /// @brief Insert a trap statement. (crash program)
    #define CB_INSERT_TRAP() __builtin_trap()

    /// @brief Insert a debugger break statement.
    #define CB_DEBUG_BREAK() __builtin_debugtrap()
#endif

/// @brief Log an info level message to stdout.
/// @param ... (format and format arguments) Message to log.
#define CB_INFO( ... ) cb_write_log( CB_LOG_INFO, __VA_ARGS__ )
/// @brief Log a warning level message to stdout.
/// @param ... (format and format arguments) Message to log.
#define CB_WARN( ... ) cb_write_log( CB_LOG_WARN, __VA_ARGS__ )
/// @brief Log an error level message to stderr.
/// @param ... (format and format arguments) Message to log.
#define CB_ERROR( ... ) cb_write_log( CB_LOG_ERROR, __VA_ARGS__ )
/// @brief Log an attention level message to stdout.
/// @param ... (format and format arguments) Message to log.
#define CB_ATTEN( ... ) cb_write_log( CB_LOG_ATTENTION, __VA_ARGS__ )
/// @brief Log a fatal level message to stderr.
/// @param ... (format and format arguments) Message to log.
#define CB_FATAL( ... ) cb_write_log( CB_LOG_FATAL, __VA_ARGS__ )

/// @brief Check if condition is true. Panic if it's not. (Noreturn on fail)
/// @param condition (boolean expression) Condition to check.
/// @param ...       (format and format arguments) Message to log upon failed condition.
#define CB_ASSERT( condition, ... ) do { \
    if( !(condition) ) { \
        CB_FATAL( "ASSERT @ " __FILE__ ":" CB_STRINGIFY_VALUE(__LINE__) ": " \
            "condition (" #condition ") failed! " __VA_ARGS__ ); \
        CB_INSERT_TRAP(); \
    }\
} while(0)

/// @brief Insert a panic with message. (Noreturn)
/// @param ... (format and format arguments) Message to log.
#define CB_PANIC( ... ) do {\
    CB_FATAL( "PANIC @ " __FILE__ ":" CB_STRINGIFY_VALUE(__LINE__) ": " __VA_ARGS__ ); \
    CB_INSERT_TRAP(); \
} while(0)

/// @brief Mark control path as unimplemented. (Noreturn)
#define CB_UNIMPLEMENTED( ... ) do {\
    CB_FATAL( "UNIMPLEMENTED @ " __FILE__ ":" CB_STRINGIFY_VALUE(__LINE__) ": " __VA_ARGS__ ); \
    CB_INSERT_TRAP(); \
} while(0)

/// @brief Mark control path as unreachable (hopefully). (Noreturn)
#define CB_UNREACHABLE( ... ) do {\
    CB_FATAL( "UNREACHABLE @ " __FILE__ ":" CB_STRINGIFY_VALUE(__LINE__) ": " __VA_ARGS__ ); \
    CB_INSERT_UNREACHABLE(); \
    CB_INSERT_TRAP(); \
} while(0)
/*           ^^^^^ just in case compiler warns */

/// @brief Create version integer the same way C-Build defines version integers.
/// @param major (unsigned short) Major version.
/// @param minor (unsigned char)  Minor version.
/// @param patch (unsigned char)  Patch version.
/// @return (unsigned int) C-Build version integer.
#define CB_VERSION_CREATE( major, minor, patch ) \
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

/// @brief Calculate length of static array.
/// @param array (any[]) Static array to calculate length of.
/// @return (uintptr_t) Length of @c array.
#define CB_ARRAY_LEN( array ) (sizeof(array) / sizeof((array)[0]))
/// @brief Convert kilobytes to bytes.
/// @param kb (uintptr_t) Kilobytes.
/// @return (uintptr_t) @c kb as bytes.
#define CB_KILOBYTES( kb ) (kb * 1000ULL)
/// @brief Convert megabytes to bytes.
/// @param mb (uintptr_t) Megabytes.
/// @return (uintptr_t) @c mb as bytes.
#define CB_MEGABYTES( mb ) (CB_KILOBYTES(mb) * 1000ULL)
/// @brief Convert gigabytes to bytes.
/// @param gb (uintptr_t) Gigabytes.
/// @return (uintptr_t) @c gb as bytes.
#define CB_GIGABYTES( gb ) (CB_MEGABYTES(gb) * 1000ULL)
/// @brief Convert Terabytes to bytes.
/// @param tb (uintptr_t) Terabytes.
/// @return (uintptr_t) @c tb as bytes.
#define CB_TERABYTES( tb ) (CB_GIGABYTES(tb) * 1000ULL)
/// @brief Convert kibibytes to bytes.
/// @param kb (uintptr_t) Kibibytes.
/// @return (uintptr_t) @c kb as bytes.
#define CB_KIBIBYTES( kb ) (kb * 1024ULL)
/// @brief Convert mebibytes to bytes.
/// @param mb (uintptr_t) Mebibytes.
/// @return (uintptr_t) @c mb as bytes.
#define CB_MEBIBYTES( mb ) (CB_KIBIBYTES(mb) * 1024ULL)
/// @brief Convert gibibytes to bytes.
/// @param gb (uintptr_t) Gibibytes.
/// @return (uintptr_t) @c gb as bytes.
#define CB_GIBIBYTES( gb ) (CB_MEBIBYTES(gb) * 1024ULL)
/// @brief Convert tebibytes to bytes.
/// @param tb (uintptr_t) Tebibytes.
/// @return (uintptr_t) @c tb as bytes.
#define CB_TEBIBYTES( tb ) (CB_GIBIBYTES(tb) * 1024ULL)

/// @brief Function for initializing cbuild. Must be called from main()
/// @param log_level Level to set logger to.
#define CB_INITIALIZE( log_level )\
    cb_initialize( log_level, argv[0], __FILE__, argc, (const char**)argv )

/// @brief Push item to dynamic array.
/// @param darray Pointer to dynamic array (struct with cap, len and buf fields)
/// @param item   Item to push to end of dynamic array.
#define CB_PUSH( darray, item ) do { \
    if( !(darray)->buf ) { \
        *(void**)(&(darray)->buf) = CB_ALLOC( NULL, 0, sizeof( (darray)->buf[0] ) * 4 ); \
        (darray)->cap = 4; \
    } else if( ((darray)->len + 1) >= (darray)->cap ) { \
        *(void**)(&(darray)->buf) = CB_ALLOC( \
            (darray)->buf, \
            sizeof((darray)->buf[0]) * (darray)->cap, \
            sizeof((darray)->buf[0]) * ((darray)->cap * 2) ); \
        (darray)->cap *= 2; \
    } \
    (darray)->buf[(darray)->len++] = item; \
} while(0)

/// @brief Append items to dynamic array.
/// @param darray Pointer to dynamic array (struct with cap, len and buf fields)
/// @param count  Number of items to append.
/// @param items  Items to append.
#define CB_APPEND( darray, count, items ) do { \
    if( !(darray)->buf ) { \
        *(void**)(&(darray)->buf) = CB_ALLOC( \
            NULL, 0, sizeof( (darray)->buf[0] ) * (4 + count) ); \
        (darray)->cap = 4 + count; \
    } else if( ((darray)->cap - (darray)->len) < count ) { \
        *(void**)(&(darray)->buf) = CB_ALLOC( \
            (darray)->buf, \
            sizeof((darray)->buf[0]) * (darray)->cap, \
            sizeof((darray)->buf[0]) * ((darray)->cap + count) ); \
        (darray)->cap += count; \
    } \
    memcpy( (darray)->buf + (darray)->len, items, sizeof((darray)->buf[0]) * count ); \
    (darray)->len += count; \
} while(0)

/// @brief Append string literal to end of string builder.
/// @param[in] builder     (CB_StringBuilder) Pointer to string builder.
/// @param[in] str_literal (string literal)   String literal to append.
#define CB_STRING_APPEND( builder, str_literal ) \
    CB_APPEND( builder, (int)(sizeof(str_literal) - 1), str_literal )

/// @brief String slice and string builder format string.
#define CB_STRING_FMT           "%.*s"
/// @brief Expand string slice or string builder into arguments for C formatting function.
/// @param string (CB_StringSlice* or CB_StringBuilder*) String to expand.
#define CB_STRING_ARG( string ) (string)->len, (string)->buf

/// @brief Create slice-like type.
/// @param struct Identifier of slice-like struct.
/// @param _len   (int)  Length of slice.
/// @param _buf   (any*) Pointer to start of slice buffer.
/// @return New slice-like.
#define CB_SLICE( struct, _len, _buf ) \
    CB_STRUCT( struct ){ .len=_len, .buf=_buf }

/// @brief Advance slice-like type.
/// @param struct Identifier of slice-like struct.
/// @param slice  (any*) Pointer to slice to advance.
/// @param amount (int)  Amount to advance slice by.
/// @return New slice-like advanced by given amount.
#define CB_ADVANCE( struct, slice, amount ) \
    ( \
        (slice)->len < amount ? \
        ( CB_SLICE( struct, 0, (slice)->buf + (slice)->len ) ) : \
        ( CB_SLICE( struct, (slice)->len - amount, (slice)->buf + amount ) ) )

/// @brief Create string slice.
/// @param _len (int)         Length of string slice.
/// @param _buf (const char*) Pointer to start of string slice.
/// @return CB_StringSlice.
#define CB_STRING_SLICE( _len, _buf ) \
    CB_STRUCT( CB_StringSlice ) { \
        .len  = _len, \
        .cbuf = _buf \
    }

/// @brief Create string slice from string literal.
///
/// @note
/// Length does not include null terminator.
///
/// @param text (string literal) String literal.
/// @return CB_StringSlice.
#define CB_STRING( text ) \
    CB_STRING_SLICE( sizeof(text) - 1, text )

/// @brief Create string slice from null terminated C string.
///
/// @note
/// Length does not include null terminator.
///
/// @param[in] cstr (const char*) C string.
/// @return CB_StringSlice.
#define CB_STRING_FROM_CSTR( cstr ) \
    CB_STRING_SLICE( strlen(cstr), cstr )

/// @brief Create command.
/// @param ... Command and arguments.
/// @return CB_Command.
#define CB_COMMAND( ... ) \
    CB_STRUCT( CB_Command ) { \
        .len = sizeof( (const char*[]){ __VA_ARGS__ } ) / sizeof( const char* ), \
        .buf = (const char*[]){ __VA_ARGS__, NULL } \
    }

// NOTE(alicia): Types

#if !defined(CB_DISABLE_TYPEDEFS)
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

    /// @brief IEEE-754 single-precision float.
    typedef float  f32;
    /// @brief IEEE-754 double-precision float.
    typedef double f64;
#endif

/// @brief Posix time.
typedef intptr_t CB_Time;

/// @brief Log level.
typedef enum CB_LogLevel {
    /// @brief Print info, warning, error, attention and fatal log messages.
    CB_LOG_INFO  ,
    /// @brief Print warning, error, attention, and fatal log messages.
    CB_LOG_WARN  ,
    /// @brief Print error, attention and fatal log messages.
    CB_LOG_ERROR ,
    /// @brief Print attention and fatal log messages.
    CB_LOG_ATTENTION,
    /// @brief Only print fatal log messages.
    CB_LOG_FATAL ,
    /// @brief Don't print any log messages.
    CB_LOG_NONE  = CB_LOG_FATAL,
    /// @brief Print all log messages.
    CB_LOG_ALL   = CB_LOG_INFO,
} CB_LogLevel;

/// @brief File open flags.
typedef enum CB_FileOpenFlags {
    /// @brief Open file for reading.
    CB_FOPEN_READ     = (1 << 0),
    /// @brief Open file for writing.
    CB_FOPEN_WRITE    = (1 << 1),
    /// @brief Create file, fails if file already exists.
    CB_FOPEN_CREATE   = (1 << 2),
    /// @brief Delete file when opened, fails if file doesn't exist.
    CB_FOPEN_TRUNCATE = (1 << 3),
    /// @brief Move file offset to end of file, fails if file doesn't exist.
    CB_FOPEN_APPEND   = (1 << 4),

    /// @brief Open file for reading and writing.
    CB_FOPEN_READ_WRITE = (CB_FOPEN_READ | CB_FOPEN_WRITE),
} CB_FileOpenFlags;

/// @brief File seek modes.
typedef enum CB_FileSeek {
    /// @brief Seek from current offset.
    CB_FSEEK_CUR,
    /// @brief Seek from start of file.
    CB_FSEEK_SET,
    /// @brief Seek from end of file.
    CB_FSEEK_END,
} CB_FileSeek;

/// @brief File types.
typedef enum CB_FileType {
    /// @brief File not found.
    CB_FTYPE_NULL,
    /// @brief File is a regular file.
    CB_FTYPE_FILE,
    /// @brief File is a directory.
    CB_FTYPE_DIRECTORY,
} CB_FileType;

/// @brief File handle.
typedef struct CB_File {
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    void* _internal_handle;
#else
    int _internal_handle;
#endif
} CB_File;

/// @brief Create null file.
#define CB_FILE_NULL() \
    CB_STRUCT( CB_File ){ ._internal_handle=0 }
/// @brief Create null pipe.
#define CB_PIPE_NULL() \
    CB_FILE_NULL()

/// @brief File information.
typedef struct CB_FileInfo {
    /// @brief Type of file.
    CB_FileType type;
    /// @brief Size of file.
    uintptr_t size;
    /// @brief Time stamps.
    struct {
        /// @brief Last time file was modified.
        CB_Time modify;
        /// @brief When file was created.
        CB_Time create;
    } time;
} CB_FileInfo;

/// @brief Pipe handle.
typedef CB_File CB_Pipe;
/// @brief Pipe handle for writing.
typedef CB_Pipe CB_PipeWrite;
/// @brief Pipe handle for reading.
typedef CB_Pipe CB_PipeRead;

/// @brief Directory walk controls.
typedef enum CB_DirectoryWalkControl {
    /// @brief Continue walk.
    CB_DWC_CONTINUE,
    /// @brief Stop walk.
    CB_DWC_STOP,
    /// @brief Skip directory.
    CB_DWC_SKIP,
} CB_DirectoryWalkControl;

/// @brief Information for current item in directory walk.
typedef struct CB_DirectoryWalkInfo {
    /// @brief Full path to current item.
    const char* path;
    /// @brief Length of path of current item.
    int path_len;
    /// @brief Offset of current item's name in path.
    int path_name_offset;

    /// @brief Type of current item.
    CB_FileType type;
    /// @brief Directory level of current item.
    int level;

    /// @brief Size of current item.
    uintptr_t   file_size;
} CB_DirectoryWalkInfo;

/// @brief Directory walk callback prototype.
///
/// @details
/// This function will be called for every item encountered
/// in directory. If item is a directory, return CB_DWC_SKIP
/// to skip walking directory or CB_DWC_CONTINUE to enter
/// directory.
///
/// @param[in] info   Pointer to info for current item.
/// @param[in] params (optional) Additional parameters for callback.
/// @return Enum telling calling function what to do after this callback is called.
typedef CB_DirectoryWalkControl
CB_DirectoryWalkFN( const CB_DirectoryWalkInfo* info, void* params );

/// @brief String slice.
typedef struct CB_StringSlice {
    /// @brief Length of string slice.
    int len;
    /// @brief Union of pointers to start of string slice.
    union {
        /// @brief Constant pointer to start of string slice.
        const char* cbuf;
        /// @brief Pointer to start of string slice.
        char*       buf;
        /// @brief Pointer to start of string slice (as bytes).
        uint8_t*    bytes;
    };
} CB_StringSlice;

/// @brief List of string slices.
typedef struct CB_StringSliceList {
    /// @brief Number of string slices that list can hold.
    int cap;
    /// @brief Number of string slices in list.
    int len;
    /// @brief Pointer to start of string slice list.
    CB_StringSlice* buf;
} CB_StringSliceList;

/// @brief String builder.
/// @details
/// This is a structure meant to be used with CB_PUSH and CB_APPEND.
/// In order to use as a string, a null terminator must be pushed.
typedef struct CB_StringBuilder {
    /// @brief Number of characters string builder can hold.
    int cap;
    /// @brief Union of string builder components and string slice.
    union {
        /// @brief Length and pointer to start of string builder buffer.
        struct {
            /// @brief Number of characters currently in string builder.
            int   len;
            /// @brief Pointer to start of string buffer.
            char* buf;
        };
        /// @brief String builder as a string slice.
        CB_StringSlice slice;
    };
} CB_StringBuilder;

/// @brief Process command.
typedef struct CB_Command {
    /// @brief Number of arguments + command itself.
    int          len;
    /// @brief Command arguments. Must always end with a null pointer.
    const char** buf;
} CB_Command;

/// @brief Process command builder.
typedef struct CB_CommandBuilder {
    /// @brief Number of commands command builder can hold.
    int cap;
    union {
        struct {
            /// @brief Number of arguments + command itself.
            int len;
            /// @brief Command arguments. Must always end with a null pointer.
            const char** buf;
        };
        /// @brief Command.
        CB_Command cmd;
    };
} CB_CommandBuilder;

/// @brief Process environment.
typedef struct CB_EnvironmentBuilder {
    /// @brief Max number of environment key + value pairs.
    int cap;
    /// @brief Number of environment key + value pairs.
    int len;
    /// @brief Key + value pair names.
    const char** name;
    /// @brief Key + value pair values.
    const char** value;
} CB_EnvironmentBuilder;

/// @brief Process ID.
typedef struct CB_ProcessID {
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    void* _internal_handle;
#else
    int _internal_handle;
#endif
} CB_ProcessID;

/// @brief Create null process ID.
#define CB_PROCESS_ID_NULL() \
    CB_STRUCT( CB_ProcessID ){ ._internal_handle=0 }

/// @brief UTF-8 code point.
typedef union CB_UTFCodePoint8 {
    /// @brief Bytes array.
    uint8_t bytes[4];
    /// @brief Code units array.
    uint8_t code_units[4];
} CB_UTFCodePoint8;
/// @brief UTF-16 code point.
typedef union CB_UTFCodePoint16 {
    /// @brief Bytes array.
    uint8_t  bytes[4];
    /// @brief Code units array.
    uint16_t code_units[2];
} CB_UTFCodePoint16;
/// @brief UTF-32 code point.
typedef union CB_UTFCodePoint32 {
    /// @brief Unicode code point or 'rune'.
    uint32_t rune;
    /// @brief Bytes array.
    uint8_t  bytes[4];
    /// @brief Code units.
    uint32_t code_units[1];
} CB_UTFCodePoint32;
/// @brief Unicode validation result.
typedef enum CB_UnicodeValidationResult {
    /// @brief UTF-8 sequence is valid.
    CB_UNICODE_RESULT_OK,
    /// @brief UTF-8 sequence is shorter than expected.
    CB_UNICODE_RESULT_UNFINISHED,
    /// @brief UTF-8 sequence is invalid.
    CB_UNICODE_RESULT_INVALID,
} CB_UnicodeValidationResult;

// NOTE(alicia): Cross-Platform Functions

/// @brief Initialize cbuild run-time.
///
/// @details
/// Checks if cbuild needs to be rebuilt and
/// initializes run-time.
///
/// @param     log_level       Logging level.
/// @param[in] path_executable Path to current executable.
/// @param[in] path_source     Path to current source file.
/// @param     argc            Number of command-line arguments.
/// @param[in] argv            Command-line arguments.
void cb_initialize(
    CB_LogLevel  log_level,
    const char*  path_executable,
    const char*  path_source,
    int          argc,
    const char** argv );

/// @brief Rebuild cbuild.
/// @param[in] path_executable  Path to current executable.
/// @param[in] path_source      Path to current source file.
/// @param     argc             Number of command-line arguments.
/// @param[in] argv             Command-line arguments.
/// @param[in] opt_cmd_override (optional) Compilation command override.
/// @param     should_reload    If newly compiled cbuild should be loaded.
CB_NO_RETURN
void cb_rebuild(
    const char*  path_executable,
    const char*  path_source,
    int          argc,
    const char** argv,
    CB_Command*  opt_cmd_override,
    bool         should_reload  );

/// @brief Get local buffer.
///
/// @details
/// Up to CB_LOCAL_BUFFER_COUNT local buffers can be obtained at a time.
/// Each buffer is of size CB_LOCAL_BUFFER_CAPACITY.
///
/// @return Local buffer.
char* cb_local_buf(void);
/// @brief Format local buffer.
/// @param[in] fmt Format string.
/// @param[in] va  Variadic list of format arguments.
/// @return Formatted local buffer.
char* cb_local_buf_fmt_va( const char* fmt, va_list va );
/// @brief Format local buffer.
/// @param[in] fmt Format string.
/// @param     ... Format arguments.
/// @return Formatted local buffer.
char* cb_local_buf_fmt( const char* fmt, ... );

/// @brief Allocate a formatted string buffer.
/// @param[in] fmt Format string.
/// @param[in] va  Variadic list of format arguments.
/// @return Formatted string. Must be freed with CB_FREE(). Size of buffer is strlen() + 1.
char* cb_alloc_fmt_va( const char* fmt, va_list va );
/// @brief Allocate a formatted string buffer.
/// @param[in] fmt Format string.
/// @param     ... Format arguments.
/// @return Formatted string. Must be freed with CB_FREE(). Size of buffer is strlen() + 1.
char* cb_alloc_fmt( const char* fmt, ... );

/// @brief Compare two strings.
/// @param a, b Strings to compare.
/// @return
///     - -1 : @c a is shorter than @c b
///     -  0 : @c a and @c b are equal in length and contents
///     -  1 : @c a is longer than @c b
int cb_string_cmp( CB_StringSlice a, CB_StringSlice b );
/// @brief Search for character in string.
/// @param string String to search in.
/// @param c      Character to search for. (ASCII)
/// @return
///     - Negative         : Character could not be found.
///     - Zero or Positive : Index of character in string.
int cb_string_find( CB_StringSlice string, char c );
/// @brief Search for unicode character in string.
/// @param string String to search in.
/// @param c      Character to search for. (Unicode)
/// @return
///     - Negative         : Character could not be found.
///     - Zero or Positive : Byte index of character in string.
int cb_string_find_unicode( CB_StringSlice string, uint32_t c );
/// @brief Search for character in string from end of string.
/// @param string String to search in.
/// @param c      Character to search for.
/// @return
///     - Negative         : Character could not be found.
///     - Zero or Positive : Index of character in string.
int cb_string_find_rev( CB_StringSlice string, char c );
/// @brief Search for unicode character in string from end of string.
/// @param string String to search in.
/// @param c      Character to search for. (Unicode)
/// @return
///     - Negative         : Character could not be found.
///     - Zero or Positive : Byte index of character in string.
int cb_string_find_unicode_rev( CB_StringSlice string, uint32_t c );
/// @brief Search for any character in set in string.
/// @param string String to search in.
/// @param set    Set of ASCII characters to search for.
/// @return
///     - Negative         : Set could not be found.
///     - Zero or Positive : Index of character found in string.
int cb_string_find_set( CB_StringSlice string, CB_StringSlice set );
/// @brief Search for any character in set in string from end of string.
/// @param string String to search in.
/// @param set    Set of ASCII characters to search for.
/// @return
///     - Negative         : Set could not be found.
///     - Zero or Positive : Index of character found in string.
int cb_string_find_set_rev( CB_StringSlice string, CB_StringSlice set );
/// @brief Search for any character in set in string.
/// @param     string String to search in.
/// @param     len    Number of unicode characters in set.
/// @param[in] set    Set of unicode characters to search for.
/// @return
///     - Negative         : Set could not be found.
///     - Zero or Positive : Byte index of character found in string.
int cb_string_find_set_unicode( CB_StringSlice string, int len, uint32_t* set );
/// @brief Search for any character in set in string from end of string.
/// @param     string String to search in.
/// @param     len    Number of unicode characters in set.
/// @param[in] set    Set of unicode characters to search for.
/// @return
///     - Negative         : Set could not be found.
///     - Zero or Positive : Byte index of character found in string.
int cb_string_find_set_unicode_rev( CB_StringSlice string, int len, uint32_t* set );
/// @brief Search for a phrase in string.
/// @param string String to search in.
/// @param phrase Phrase to search for.
/// @return
///     - Negative         : Phrase could not be found.
///     - Zero or Positive : Index of start of phrase found in string.
int cb_string_find_phrase( CB_StringSlice string, CB_StringSlice phrase );
/// @brief Search for a phrase in string from end of string.
/// @param string String to search in.
/// @param phrase Phrase to search for.
/// @return
///     - Negative         : Phrase could not be found.
///     - Zero or Positive : Index of start of phrase found in string.
int cb_string_find_phrase_rev( CB_StringSlice string, CB_StringSlice phrase );
/// @brief Advance string by given number of bytes.
/// @param string String to advance.
/// @param amount Number of bytes to advance by.
/// @return String.
CB_StringSlice cb_string_advance( CB_StringSlice string, int amount );
/// @brief Truncate string to given maximum length.
/// @param string String to truncate.
/// @param max    Maximum number of characters to truncate to.
/// @return Truncated string.
CB_StringSlice cb_string_truncate( CB_StringSlice string, int max );
/// @brief Subtract from string length.
/// @param string String to trim.
/// @param amount Number of bytes to trim by.
/// @return Trimmed string.
CB_StringSlice cb_string_trim( CB_StringSlice string, int amount );
/// @brief Create string slice from string slice.
///
/// @note
/// This function does not check if given range is valid.
///
/// @param string   String to clip from.
/// @param from_inc Start of range to clip, inclusive.
/// @param to_exc   End of range to clip, exclusive.
/// @return Clipped string.
CB_StringSlice cb_string_clip( CB_StringSlice string, int from_inc, int to_exc );
/// @brief Trim leading whitespace from string.
/// @param string String to trim.
/// @return Trimmed string.
CB_StringSlice cb_string_trim_leading_whitespace( CB_StringSlice string );
/// @brief Trim trailing whitespace from string.
/// @param string String to trim.
/// @return Trimmed string.
CB_StringSlice cb_string_trim_trailing_whitespace( CB_StringSlice string );
/// @brief Trim surrounding whitespace from string.
/// @param string String to trim.
/// @return Trimmed string.
CB_StringSlice cb_string_trim_surrounding_whitespace( CB_StringSlice string );
/// @brief Split string at index.
/// @param      string         String to split.
/// @param      index          Index to split at.
/// @param      should_include If @c out_left should include character at @c index.
/// @param[out] out_left       Pointer to write left side of split.
/// @param[out] out_right      Pointer to write right side of split.
void cb_string_split(
    CB_StringSlice string, int index, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right );
/// @brief Split string by character.
/// @param      string         String to split.
/// @param      c              Character to split by. (ASCII)
/// @param      should_include If @c out_left should include character @c c.
/// @param[out] out_left       Pointer to write left side of split.
/// @param[out] out_right      Pointer to write right side of split.
/// @return
///     - @c true  : Character found and split strings written to @c out_left and @c out_right.
///     - @c false : Failed to find character.
bool cb_string_split_by_char(
    CB_StringSlice string, char c, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right );
/// @brief Split string by character.
/// @param      string         String to split.
/// @param      c              Character to split by. (Unicode)
/// @param      should_include If @c out_left should include character @c c.
/// @param[out] out_left       Pointer to write left side of split.
/// @param[out] out_right      Pointer to write right side of split.
/// @return
///     - @c true  : Character found and split strings written to @c out_left and @c out_right.
///     - @c false : Failed to find character.
bool cb_string_split_by_char_unicode(
    CB_StringSlice string, uint32_t c, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right );
/// @brief Split string by any character in set.
/// @param      string         String to split.
/// @param      set            Set of characters to split by. (ASCII)
/// @param      should_include If @c out_left should include found character.
/// @param[out] out_left       Pointer to write left side of split.
/// @param[out] out_right      Pointer to write right side of split.
/// @return
///     - @c true  : Character in set found and split
///                    strings written to @c out_left and @c out_right.
///     - @c false : Failed to find any character in set. @c string written to @c out_left.
bool cb_string_split_by_set(
    CB_StringSlice string, CB_StringSlice set, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right );
/// @brief Split string by any character in set.
/// @param      string         String to split.
/// @param      set_len        Number of characters in set.
/// @param[in]  set            Set of characters to split by. (Unicode)
/// @param      should_include If @c out_left should include found character.
/// @param[out] out_left       Pointer to write left side of split.
/// @param[out] out_right      Pointer to write right side of split.
/// @return
///     - @c true  : Character in set found and split
///                    strings written to @c out_left and @c out_right.
///     - @c false : Failed to find any character in set. @c string written to @c out_left.
bool cb_string_split_by_set_unicode(
    CB_StringSlice string, int set_len, uint32_t* set, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right );
/// @brief Split string by phrase.
/// @param      string         String to split.
/// @param      phrase         Phrase to split string by.
/// @param      should_include If @c out_left should include found phrase.
/// @param[out] out_left       Pointer to write left side of split.
/// @param[out] out_right      Pointer to write right side of split.
/// @return
///     - @c true  : Phrase found and split strings written to @c out_left and @c out_right.
///     - @c false : Failed to find phrase. @c string written to @c out_left.
bool cb_string_split_by_phrase(
    CB_StringSlice string, CB_StringSlice phrase, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right );
/// @brief Split string by character into a list of substrings.
/// @param      string         String to split.
/// @param      c              Character to split by. (ASCII)
/// @param      should_include If strings in list should include found character.
/// @param[out] out_list       List to append string slices to.
/// @return Number of string slices found.
int cb_string_split_by_char_list(
    CB_StringSlice string, char c, bool should_include, CB_StringSliceList* out_list );
/// @brief Split string by character into a list of substrings.
/// @param      string         String to split.
/// @param      c              Character to split by. (Unicode)
/// @param      should_include If strings in list should include found character.
/// @param[out] out_list       List to append string slices to.
/// @return Number of string slices found.
int cb_string_split_by_char_unicode_list(
    CB_StringSlice string, uint32_t c, bool should_include, CB_StringSliceList* out_list );
/// @brief Split string by any character in set into a list of substrings.
/// @param      string         String to split.
/// @param      set            Set of characters to search for. (ASCII)
/// @param      should_include If strings in list should include found character.
/// @param[out] out_list       List to append string slices to.
/// @return Number of string slices found.
int cb_string_split_by_set_list(
    CB_StringSlice string, CB_StringSlice set,
    bool should_include, CB_StringSliceList* out_list );
/// @brief Split string by any character in set into a list of substrings.
/// @param      string         String to split.
/// @param      set_len        Number of characters in set.
/// @param[in]  set            Set of characters to search for. (Unicode)
/// @param      should_include If strings in list should include found character.
/// @param[out] out_list       List to append string slices to.
/// @return Number of string slices found.
int cb_string_split_by_set_unicode_list(
    CB_StringSlice string, int set_len, uint32_t* set,
    bool should_include, CB_StringSliceList* out_list );
/// @brief Split string by phrase into a list of substrings.
/// @param      string         String to split.
/// @param      phrase         Phrase to split by.
/// @param      should_include If strings in list should include found phrase.
/// @param[out] out_list       List to append string slices to.
/// @return Number of string slices found.
int cb_string_split_by_phrase_list(
    CB_StringSlice string, CB_StringSlice phrase,
    bool should_include, CB_StringSliceList* out_list );

/// @brief Create null-terminated string from string.
/// @param string                String to create C string from.
/// @param replace_null          If function should search for nulls to replace.
/// @param replacement_character Character to replace nulls with.
/// @return New C string. Must be freed with CB_FREE. Buffer size is strlen + 1.
char* cb_cstr_from_string(
    CB_StringSlice string, bool replace_null, uint32_t replacement_character );
/// @brief Append string slice to string builder.
/// @param[in] builder String builder to append to.
/// @param     string  String to append.
void cb_string_builder_from_string(
    CB_StringBuilder* builder, CB_StringSlice string );

/// @brief Get next unicode character in string.
/// @param      string        UTF-8 String.
/// @param[out] out_character Pointer to write unicode character to.
/// @return String advanced to next character or empty if this was the last character.
CB_StringSlice cb_string_unicode_next( CB_StringSlice string, uint32_t* out_character );

/// @brief Calculate UTF-8 length of string.
/// @param     opt_len  (optional) Length of string in code units (bytes).
///                       If zero, @c str_utf8 is assumed to be null-terminated.
/// @param[in] str_utf8 Pointer to start of UTF-8 encoded string.
/// @return Number of unicode characters in string.
int cb_utf8_len( int opt_len, const char* str_utf8 );
/// @brief Get next Unicode character in string.
/// @param      opt_len       (optional) Length of string in code units (bytes).
///                             If zero, @c str_utf8 is assumed to be null-terminated.
/// @param[in]  str_utf8      Pointer to start of UTF-8 encoded string.
/// @param[out] out_character Pointer to write unicode character to.
/// @return Pointer to next character in string.
const char* cb_utf8_next(
    int opt_len, const char* str_utf8, uint32_t* out_character );
/// @brief Index into UTF-8 string.
/// @param      opt_len       (optional) Length of string in code units (bytes).
///                             If zero, @c str_utf8 is assumed to be null-terminated.
/// @param[in]  str_utf8      Pointer to start of UTF-8 encoded string.
/// @param      index         UTF-8 index.
/// @param[out] out_character Pointer to write unicode character to.
/// @return Pointer to where unicode character starts in string.
const char* cb_utf8_index(
    int opt_len, const char* str_utf8, int index, uint32_t* out_character );

/// @brief Construct UTF-8 code point from code units.
/// @param code_unit_0, code_unit_1, code_unit_2, code_unit_3 (cp8) Code units.
/// @return UTF-8 code point.
#define cb_cp8_from_code_units( code_unit_0, code_unit_1, code_unit_2, code_unit_3 ) \
    (CB_STRUCT(CB_UTFCodePoint8){ .code_units = { \
        (code_unit_0), (code_unit_1), (code_unit_2), (code_unit_3) }})

/// @brief Construct UTF-16 code point from code units.
/// @param code_unit_0, code_unit_1 (cp16) Code units.
/// @return UTF-16 code point.
#define cb_cp16_from_code_units( code_unit_0, code_unit_1 ) \
    (CB_STRUCT(CB_UTFCodePoint16){ .code_units = { \
        (code_unit_0), (code_unit_1) }})

/// @brief Construct UTF-32 code point from code units.
/// @param code_unit_0 (cp32) Code units.
/// @return UTF-32 code point.
#define cb_cp32_from_code_units( code_unit_0 ) \
    (CB_STRUCT(CB_UTFCodePoint32){ .code_units = { (code_unit_0) }})

/// @brief Construct UTF-8 code point from bytes.
/// @param code_unit_0, code_unit_1, code_unit_2, code_unit_3 (uint8_t) Code units.
/// @return UTF-8 code point.
#define cb_cp8_from_bytes(                          \
    code_unit_0, code_unit_1, code_unit_2, code_unit_3 ) \
    (CB_STRUCT(CB_UTFCodePoint8){ .bytes = {           \
        (code_unit_0), (code_unit_1), (code_unit_2), (code_unit_3) } })

#if !defined(CB_ARCH_IS_LITTLE_ENDIAN) || CB_ARCH_IS_LITTLE_ENDIAN == 0

/// @brief Construct UTF-16 code point from bytes.
/// @param code_unit_0_low, code_unit_0_high (uint8_t) First code unit, low and high byte.
/// @param code_unit_1_low, code_unit_1_high (uint8_t) Second code unit, low and high byte.
/// @return UTF-16 code point.
#define cb_cp16_from_bytes(                                           \
    code_unit_0_low, code_unit_0_high, code_unit_1_low, code_unit_1_high ) \
    (struct_literal(CB_UTFCodePoint16){ .bytes = {                            \
        (code_unit_0_high), (code_unit_0_low), (code_unit_1_high), (code_unit_1_low) }})

/// @brief Construct UTF-32 code point from bytes.
/// @param low_0, low_1   (uint8_t) Low bytes.
/// @param high_0, high_1 (uint8_t) High bytes.
/// @return UTF-32 code point.
#define cb_cp32_from_bytes( low_0, low_1, high_0, high_1 ) \
    (struct_literal(CB_UTFCodePoint32){ .bytes = {                 \
        (high_1), (high_0), (low_1), (low_0) } })

/// @brief Access byte from UTF-16 code point.
/// @param[in] ptr_cp16  (CB_UTFCodePoint16*) Pointer to code point to access.
/// @param     code_unit (int)             Index of code unit.
///                                          Either 0 or 1.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define cb_cp16_read_byte( ptr_cp16, code_unit, byte ) \
    (((uint8_t*)((ptr_cp16)->code_units + code_unit))[1 - (byte)])

/// @brief Access byte from UTF-32 code point.
/// @param[in] ptr_cp32  (CB_UTFCodePoint32*) Pointer to code point to access.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define cb_cp32_read_byte( ptr_cp32, byte ) \
    ((ptr_cp32)->bytes[3 - (byte)])

#else

/// @brief Construct UTF-16 code point from bytes.
/// @param code_unit_0_low, code_unit_0_high (uint8_t) First code unit, low and high byte.
/// @param code_unit_1_low, code_unit_1_high (uint8_t) Second code unit, low and high byte.
/// @return UTF-16 code point.
#define cb_cp16_from_bytes(                                           \
    code_unit_0_low, code_unit_0_high, code_unit_1_low, code_unit_1_high ) \
    (struct_literal(CB_UTFCodePoint16){ .bytes = {                            \
        (code_unit_0_low), (code_unit_0_high), (code_unit_1_low), (code_unit_1_high) }})

/// @brief Construct UTF-32 code point from bytes.
/// @param low_0, low_1   (uint8_t) Low bytes.
/// @param high_0, high_1 (uint8_t) High bytes.
/// @return UTF-32 code point.
#define cb_cp32_from_bytes( low_0, low_1, high_0, high_1 ) \
    (struct_literal(CB_UTFCodePoint32){ .bytes = {                 \
        (low_0), (low_1), (high_0), (high_1) } })

/// @brief Access byte from UTF-16 code point.
/// @param[in] ptr_cp16  (CB_UTFCodePoint16*) Pointer to code point to access.
/// @param     code_unit (int)             Index of code unit.
///                                          Either 0 or 1.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define cb_cp16_read_byte( ptr_cp16, code_unit, byte ) \
    (((uint8_t*)((ptr_cp16)->code_units + code_unit))[(byte)])

/// @brief Access byte from UTF-32 code point.
/// @param[in] ptr_cp32  (CB_UTFCodePoint32*) Pointer to code point to access.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define cb_cp32_read_byte( ptr_cp32, byte ) \
    ((ptr_cp32)->bytes[(byte)])

#endif /* Little Endian */

/// @brief Convert UTF-8 code point to unicode rune.
/// @param cp8 UTF-8 code point.
/// @return Unicode rune. (UTF-32 encoding)
uint32_t cb_unicode_from_cp8( CB_UTFCodePoint8 cp8 );
/// @brief Convert UTF-16 code point to unicode rune.
/// @param cp16 UTF-16 code point.
/// @return Unicode rune. (UTF-32 encoding)
uint32_t cb_unicode_from_cp16( CB_UTFCodePoint16 cp16 );
/// @brief Convert UTF-32 code point to unicode rune.
///
/// @note
/// This function technically doesn't need to exist as
/// cp32 is the same as uint32_t.
/// That being said, if code point is invalid,
/// this function will return replacement character.
///
/// @param cp8 UTF-32 code point.
/// @return Unicode rune. (UTF-32 encoding)
uint32_t cb_unicode_from_cp32( CB_UTFCodePoint32 cp32 );

/// @brief Count number of code units in UTF-8 code point.
///
/// @note
/// This function only checks the first code unit to see how
/// many code units this code point should contain.
/// To properly validate code point, use cb_utf8_validate().
///
/// @param cp8 UTF-8 code point.
/// @return
///     - @c 1-4 : Number of code units in code point.
///     - @c 0   : UTF-8 code point is invalid.
int cb_cp8_code_unit_count( CB_UTFCodePoint8 cp8 );
/// @brief Count number of code units in UTF-16 code point.
///
/// @note
/// This function only checks the first code unit to see how
/// many code units this code point should contain.
/// To properly validate code point, use cb_utf16_validate().
///
/// @param cp16 UTF-16 code point.
/// @return
///     - @c 1-2 : Number of code units in sequence.
///     - @c 0   : UTF-16 sequence is invalid.
int cb_cp16_code_unit_count( CB_UTFCodePoint16 cp16 );
/// @brief Count number of code units in UTF-32 code point.
///
/// @note
/// This function only exists for parity with 
/// cb_cp8_code_unit_count() and
/// cb_cp16_code_unit_count().
/// To properly validate code point, use cb_utf32_validate().
///
/// @param cp32 UTF-32 code point.
/// @return Always 1.
int cb_cp32_code_unit_count( CB_UTFCodePoint32 cp32 );

/// @brief Validate next UTF-8 code point.
/// @param      len             Length of string in code units. (bytes)
/// @param[in]  utf8            UTF-8 String. This function will read 4 code units at most.
/// @param[out] opt_out_rune    (optional) Pointer to write decoded rune if no error was encountered.
/// @param[out] opt_out_error   (optional) Pointer to write start of error if error encountered.
/// @param[out] opt_out_advance (optional) Pointer to write number of bytes to advance string by.
/// @return Result.
CB_UnicodeValidationResult cb_utf8_validate(
    int len, const uint8_t* utf8, uint32_t* opt_out_rune,
    const uint8_t** opt_out_error, int* opt_out_advance );
/// @brief Validate next UTF-16 code point.
/// @param      len             Length of string in code units. (shorts)
/// @param[in]  utf16           UTF-16 String. This function will read 2 code units at most.
/// @param[out] opt_out_rune    (optional) Pointer to write decoded rune if no error was encountered.
/// @param[out] opt_out_error   (optional) Pointer to write start of error if error encountered.
/// @param[out] opt_out_advance (optional) Pointer to write number of bytes to advance string by.
/// @return Result.
CB_UnicodeValidationResult cb_utf16_validate(
    int len, const uint16_t* utf16, uint32_t* opt_out_rune,
    const uint16_t** opt_out_error, int* opt_out_advance );
/// @brief Validate next UTF-32 code point.
/// @param      len             Length of string in code units. (shorts)
/// @param[in]  utf32           UTF-32 String. This function will read 1 code unit at most.
/// @param[out] opt_out_rune    (optional) Pointer to write decoded rune if no error was encountered.
/// @param[out] opt_out_error   (optional) Pointer to write start of error if error encountered.
/// @param[out] opt_out_advance (optional) Pointer to write number of bytes to advance string by.
/// @return Result.
CB_UnicodeValidationResult cb_utf32_validate(
    int len, const uint32_t* utf32, uint32_t* opt_out_rune,
    const uint32_t** opt_out_error, int* opt_out_advance );

/// @brief Get next UTF-8 code point from UTF-8 string.
///
/// @details
/// If @c len is zero, function returns zero otherwise function will always return
/// a minimum of 1, even if the next code point in string is invalid.
/// CB_UNICODE_CP8_REPLACEMENT_CHARACTER is written to @c out_cp8 if code point is invalid.
///
/// @param      len     Length of UTF-8 string in code units. (bytes)
/// @param[in]  utf8    UTF-8 String.
/// @param[out] out_cp8 Pointer to write UTF-8 code point to.
/// @return Number of code units (bytes) to advance string by.
int cb_cp8_from_string( int len, const uint8_t* utf8, CB_UTFCodePoint8* out_cp8 );
/// @brief Get next UTF-16 code point from UTF-16 string.
///
/// @details
/// If @c len is zero, function returns zero otherwise function will always return
/// a minimum of 1, even if the next code point in string is invalid.
/// CB_UNICODE_CP16_REPLACEMENT_CHARACTER is written to @c out_cp16 if code point is invalid.
///
/// @param      len      Length of UTF-16 string in code units. (shorts)
/// @param[in]  utf16    UTF-16 String.
/// @param[out] out_cp16 Pointer to write UTF-16 code point to.
/// @return Number of code units (shorts) to advance string by.
int cb_cp16_from_string( int len, const uint16_t* utf16, CB_UTFCodePoint16* out_cp16 );
/// @brief Get next UTF-32 code point from UTF-32 string.
///
/// @details
/// If @c len is zero, function returns zero otherwise function will always return
/// a minimum of 1, even if the next code point in string is invalid.
/// CB_UNICODE_CP32_REPLACEMENT_CHARACTER is written to @c out_cp32 if code point is invalid.
///
/// @param      len      Length of UTF-32 string in code units. (ints)
/// @param[in]  utf32    UTF-32 String.
/// @param[out] out_cp32 Pointer to write UTF-32 code point to.
/// @return Number of code units (ints) to advance string by.
int cb_cp32_from_string( int len, const uint32_t* utf32, CB_UTFCodePoint32* out_cp32 );


/// @brief Convert UTF-16 code point to UTF-8 code point.
/// @param cp16 UTF-16 code point.
/// @return UTF-8 code point.
CB_UTFCodePoint8 cb_cp8_from_cp16( CB_UTFCodePoint16 cp16 );
/// @brief Convert UTF-32 code point to UTF-8 code point.
/// @param cp32 UTF-32 code point.
/// @return UTF-8 code point.
CB_UTFCodePoint8 cb_cp8_from_cp32( CB_UTFCodePoint32 cp32 );

/// @brief Convert UTF-8 code point to UTF-16 code point.
/// @param cp8 UTF-8 code point.
/// @return UTF-16 code point.
CB_UTFCodePoint16 cb_cp16_from_cp8( CB_UTFCodePoint8 cp8 );
/// @brief Convert UTF-32 code point to UTF-16 code point.
/// @param cp32 UTF-32 code point.
/// @return UTF-16 code point.
CB_UTFCodePoint16 cb_cp16_from_cp32( CB_UTFCodePoint32 cp32 );

/// @brief Convert UTF-8 code point to UTF-32 code point.
/// @param cp8 UTF-8 code point.
/// @return UTF-32 code point.
CB_UTFCodePoint32 cb_cp32_from_cp8( CB_UTFCodePoint8 cp8 );
/// @brief Convert UTF-16 code point to UTF-32 code point.
/// @param cp16 UTF-16 code point.
/// @return UTF-32 code point.
CB_UTFCodePoint32 cb_cp32_from_cp16( CB_UTFCodePoint16 cp16 );


/// @brief Check if file exists at given path.
/// @param[in] path_utf8 Path to file.
/// @return
///     - @c true  : File exists at given path.
///     - @c false : File does not exist or something else (directory) exists at that path.
bool cb_file_exists( const char* path_utf8 );

/// @brief Check if directory exists at given path.
/// @param[in] path_utf8 Path to directory.
/// @return
///     - @c true  : Directory exists at given path.
///     - @c false : Directory does not exist or something else (file) exists at that path.
bool cb_directory_exists( const char* path_utf8 );

/// @brief Copy contents of one directory to another, recursively.
/// @param[in] dst                      Path to destination directory.
/// @param[in] src                      Path to source directory.
/// @param     overwrite_existing_names If function should overwrite existing file/directories.
/// @param     fail_if_dst_exists       If function should fail if dst exists.
///                                       Otherwise, overwrites dst.
/// @return
///     - @c true  : Successfully copied all files.
///     - @c false : Failed to copy files.
bool cb_directory_copy(
    const char* dst, const char* src,
    bool overwrite_existing_names,
    bool fail_if_dst_exists );
/// @brief Move contents of one directory to another, recursively.
/// @param[in] dst                      Path to destination directory.
/// @param[in] src                      Path to source directory.
/// @param     overwrite_existing_names If function should overwrite existing file/directories.
/// @param     fail_if_dst_exists       If function should fail if dst exists.
///                                       Otherwise, overwrites dst.
/// @return
///     - @c true  : Successfully copied all files and removed src directory.
///     - @c false : Failed to copy files.
bool cb_directory_move(
    const char* dst, const char* src,
    bool overwrite_existing_names,
    bool fail_if_dst_exists );

/// @brief Create directories if they don't exist.
/// @param ... Directories to create.
/// @return
///     - @c true  : Successfully created all directories.
///     - @c false : Failed to create one or more directories.
#define cb_make_directories( ... ) \
    _cb_internal_make_directories( __VA_ARGS__, NULL )

/// @brief Read entire file.
/// @param[in]  path_utf8       Path to file.
/// @param[out] out_buffer_size Pointer to write size of buffer.
/// @param[out] out_buffer      Pointer to write buffer pointer to. Free with CB_FREE()
/// @return
///     - @c true  : Opened and read from file successfully.
///     - @c false : Failed to open/read from file.
bool cb_read_entire_file(
    const char* path_utf8, uintptr_t* out_buffer_size, void** out_buffer );

/// @brief Check which file was created more recently.
/// @param[in] file_a Path to first file.
/// @param[in] file_b Path to second file.
/// @return
///     - @c  0 : @c file_a is newer.
///     - @c  1 : @c file_b is newer.
///     - @c -1 : Failed to get file time of @c file_a
///     - @c -2 : Failed to get file time of @c file_b
int cb_which_file_is_newer( const char* file_a, const char* file_b );

/// @brief Check if file a is newer than list of files.
/// @param[in] file_a       Path to first file.
/// @param     file_b_count Number of other files to check.
/// @param[in] file_b       List of other files to check.
///                           Must hold @c file_b_count number of file paths.
/// @return
///     - @c  0 : @c file_a is newer.
///     - @c  1 : One of the files in @c file_b is newer.
///     - @c -1 : Failed to get file time of @c file_a
///     - @c -2 : Failed to get file time of one of the files in @c file_b
int cb_which_file_is_newer_many_array(
    const char* file_a, int file_b_count, const char** file_b );
/// @brief Check if file a is newer than list of files.
/// @param[in] file_a Path to first file.
/// @param     ...    Other paths to check.
/// @return
///     - @c  0 : @c file_a is newer.
///     - @c  1 : One of the files in @c ... is newer.
///     - @c -1 : Failed to get file time of @c file_a
///     - @c -2 : Failed to get file time of one of the files in @c ...
#define cb_which_file_is_newer_many( file_a, ... ) \
    cb_which_file_is_newer_many_array( \
        file_a, CB_ARRAY_LEN( ((const char*[]){ __VA_ARGS__ }) ), \
        ((const char*[]){ __VA_ARGS__ }) )

/// @brief Flatten command into a string.
///
/// @note
/// This function does not append a null terminator to string.
///
/// @param         command Command to flatten.
/// @param[in,out] string  Pointer to string builder to flatten command into.
void cb_command_flatten( CB_Command command, CB_StringBuilder* string );

/// @brief Create new command builder.
/// @param ... Command builder + initial arguments.
#define cb_command_builder_new( ... ) \
    _cb_internal_command_builder_new( __VA_ARGS__, NULL )
/// @brief Append commands to end of command builder.
/// @param ... Command builder + commands to append.
#define cb_command_builder_append( ... ) \
    _cb_internal_command_builder_append( __VA_ARGS__, NULL )
/// @brief Append null pointer to end of command.
///
/// @note
/// A null pointer is required at the end of command in order to run command.
///
/// @param[in] builder Pointer to command builder.
void cb_command_builder_add_null_terminator( CB_CommandBuilder* builder );
/// @brief Remove null pointer from end of command.
///
/// @note
/// A null pointer is required at the end of command in order to run command.
///
/// @param[in] builder Pointer to command builder.
void cb_command_builder_remove_null_terminator( CB_CommandBuilder* builder );
/// @brief Create command builder from existing command.
/// @param[out] builder Pointer to command builder.
/// @param      cmd     Command to create builder from.
void cb_command_builder_from_cmd( CB_CommandBuilder* builder, CB_Command cmd );
/// @brief Remove command from builder.
/// @param[in] builder Pointer to command builder to remove command from.
/// @param     index   Index of command to remove.
void cb_command_builder_remove( CB_CommandBuilder* builder, int index );
/// @brief Free command builder.
/// @param[in] builder Pointer to command builder to free.
void cb_command_builder_free( CB_CommandBuilder* builder );

/// @brief Create process environment builder.
/// @param[out] out_environment Pointer to write new environment builder to.
/// @param[in]  opt_capacity    (optional) Initial capacity to allocate for environment builder.
void cb_environment_builder_new(
    CB_EnvironmentBuilder* out_environment, int* opt_capacity );
/// @brief Free environment builder.
/// @param[in] environment Pointer to environment builder to free.
void cb_environment_builder_free(
    CB_EnvironmentBuilder* environment );
/// @brief Append new environment variable to end of environment builder.
/// @param[in] builder Pointer to environment builder.
/// @param[in] name    Name of variable.
/// @param[in] value   Value of variable.
void cb_environment_builder_append(
    CB_EnvironmentBuilder* environment, const char* name, const char* value );
/// @brief Remove environment variable from environment builder.
/// @param[in] builder Pointer to environment builder.
/// @param[in] name    Name of environment variable to remove.
/// @return
///     - @c true  : Environment variable found and removed.
///     - @c false : Failed to find environment variable.
bool cb_environment_builder_remove(
    CB_EnvironmentBuilder* environment, const char* name );
/// @brief Remove environment variable from environment builder.
/// @param[in] builder Pointer to environment builder.
/// @param     index   Index of variable to remove.
void cb_environment_builder_remove_by_index(
    CB_EnvironmentBuilder* environment, int index );
/// @brief Replace environment variable in environment builder.
/// @param[in] environment Pointer to environment builder.
/// @param[in] name        Name of variable whose value should be replaced.
/// @param[in] new_value   New value of variable.
/// @return
///     - @c true  : Environment variable found and replaced.
///     - @c false : Failed to find environment variable.
bool cb_environment_builder_replace(
    CB_EnvironmentBuilder* environment, const char* name, const char* new_value );
/// @brief Replace environment variable in environment builder.
/// @param[in] environment Pointer to environment builder.
/// @param[in] index       Index of variable whose value should be replaced.
/// @param[in] new_value   New value of variable.
void cb_environment_builder_replace_by_index(
    CB_EnvironmentBuilder* environment, int index, const char* new_value );

/// @brief Execute process command.
/// @param      cmd                   Command to execute.
/// @param[out] opt_out_exit_code     (optional) Pointer to write process exit code to. If -1, process exited abnormally.
/// @param[in]  opt_working_directory (optional) Working directory path.
/// @param[in]  opt_environment       (optional) Environment to execute process in.
/// @param[in]  opt_stdin             (optional) Pointer to stdin pipe.
/// @param[in]  opt_stdout            (optional) Pointer to stdout pipe.
/// @param[in]  opt_stderr            (optional) Pointer to stderr pipe.
/// @return
///     - @c true  : Executed command successfully.
///     - @c false : Failed to execute command.
bool cb_process_exec(
    CB_Command             cmd,
    int*                   opt_out_exit_code,
    const char*            opt_working_directory,
    CB_EnvironmentBuilder* opt_environment,
    CB_PipeRead*           opt_stdin,
    CB_PipeWrite*          opt_stdout,
    CB_PipeWrite*          opt_stderr );

/// @brief Execute process command.
/// @param ... Command + arguments.
/// @return
///     - 0-255 : Process exited normally and this is the exit code.
///     - -1    : Process exited abnormally.
///     - -2    : Error occurred when executing process.
#define cb_process_exec_quick( ... )  \
    _cb_internal_process_exec_quick( NULL, NULL, NULL, NULL, NULL, __VA_ARGS__, NULL )
/// @brief Execute process command.
/// @param[in]  opt_working_directory (optional,const char*)            Working directory path.
/// @param[in]  opt_environment       (optional,CB_EnvironmentBuilder*) Environment to execute process in.
/// @param[in]  opt_stdin             (optional,CB_PipeRead*)           Pointer to stdin pipe.
/// @param[in]  opt_stdout            (optional,CB_PipeWrite*)          Pointer to stdout pipe.
/// @param[in]  opt_stderr            (optional,CB_PipeWrite*)          Pointer to stderr pipe.
/// @param ... Command + arguments.
/// @return
///     - 0-255 : Process exited normally and this is the exit code.
///     - -1    : Process exited abnormally.
///     - -2    : Error occurred when executing process.
#define cb_process_exec_quick_ex( \
    opt_working_directory,        \
    opt_environment,              \
    opt_stdin,                    \
    opt_stdout,                   \
    opt_stderr,                   \
    ...                           \
) _cb_internal_process_exec_quick( \
    opt_working_directory, opt_environment, \
    opt_stdin, opt_stdout, opt_stderr, __VA_ARGS__, NULL )

/// @brief Wait for series of processes.
/// @param      count              Number of process IDs to wait for.
/// @param[in]  pids               Pointer to array of process IDs.
///                                  Must contain @c count number of process IDs.
/// @param[out] opt_out_exit_codes Pointer to array of exit codes.
///                                  If pointer is valid,
///                                  it must contain @c count number of integers.
void cb_process_wait_many( int count, CB_ProcessID* pids, int* opt_out_exit_codes );

/// @brief Set logging level.
/// @param level Logging level.
void cb_log_level_set( CB_LogLevel level );
/// @brief Query current logging level.
/// @return Current logging level.
CB_LogLevel cb_log_level_query(void);
/// @brief Check if logging level is valid.
/// @param level Logging level to check against global logging level.
/// @return
///     - @c true  : Logging level is valid.
///     - @c false : Logging level is not valid.
bool cb_log_level_is_valid( CB_LogLevel level );
/// @brief Write logging message.
/// @param     level Level of logging message.
/// @param[in] fmt   Format string.
/// @param[in] va    Variadic list of format arguments.
void cb_write_log_va( CB_LogLevel level, const char* fmt, va_list va );
/// @brief Write logging message.
/// @param     level Level of logging message.
/// @param[in] fmt   Format string.
/// @param     ...   Format arguments.
void cb_write_log( CB_LogLevel level, const char* fmt, ... );


// NOTE(alicia): Platform Specific Functions

/// @brief Query current time.
/// @note
/// For high resolution timing, use cb_time_msec() or cb_time_sec().
/// @return Current time as Posix timestamp.
CB_Time cb_time_query(void);
/// @brief Query current time using high resolution timer.
/// @return Current milliseconds.
double cb_time_msec(void);
/// @brief Query current time using high resolution timer.
/// @return Current seconds.
double cb_time_sec(void);

/// @brief Check to see if path points to file, directory or nothing.
/// @param[in] path_utf8 Path to file.
/// @return File type at path.
CB_FileType cb_path_query_type( const char* path_utf8 );
/// @brief Query time that file was last modified by path.
/// @param[in]  path_utf8 Path to file.
/// @param[out] out_time  Pointer to write time to.
/// @return
///     - @c true  : Queried time successfully.
///     - @c false : Failed to query time.
bool cb_path_query_time_modify( const char* path_utf8, CB_Time* out_time );
/// @brief Query time that file was created by path.
/// @param[in]  path_utf8 Path to file.
/// @param[out] out_time  Pointer to write time to.
/// @return
///     - @c true  : Queried time successfully.
///     - @c false : Failed to query time.
bool cb_path_query_time_create( const char* path_utf8, CB_Time* out_time );
/// @brief Query file info.
/// @param[in]  path_utf8 Path to file.
/// @param[out] out_info  Pointer to write info to.
/// @return
///     - @c true  : Queried file info successfully.
///     - @c false : File does not exist at given path or permissions locked.
bool cb_path_query_info( const char* path_utf8, CB_FileInfo* out_info );
/// @brief Create canonical path.
///
/// @warning
/// This is the only function that allocates using malloc
/// so use free() to free the result!
///
/// @param[in] path_utf8 Path to canonicalize.
/// @return
///     - @c NULL : failed to canonicalize path.
///     - path    : Canonical path. Must be freed with free()!
char* cb_path_canonicalize( const char* path_utf8 );

/// @brief Open file.
/// @param[in]  path_utf8 UTF-8 encoded path.
/// @param      flags     Flags.
/// @param[out] out_file  Pointer to write file handle to.
/// @return
///     - @c true  : Opened file successfully.
///     - @c false : Failed to open file.
bool cb_file_open( const char* path_utf8, CB_FileOpenFlags flags, CB_File* out_file );
/// @brief Close file.
/// @param[in] file Pointer to file handle.
void cb_file_close( CB_File* file );
/// @brief Seek file.
/// @param[in] file   Pointer to file handle.
/// @param     offset Offset to seek by.
/// @param     whence From where seek should start from.
/// @return New offset after seek.
intptr_t cb_file_seek( CB_File* file, intptr_t offset, CB_FileSeek whence );
/// @brief Truncate size of file to current offset.
/// @param[in] file Pointer to file handle.
void cb_file_truncate( CB_File* file );
/// @brief Read from file.
/// @param[in]  file         Pointer to file handle.
/// @param      buffer_size  Size of buffer to write contents of file to.
/// @param[in]  buffer       Pointer to buffer to write to.
/// @param[out] opt_out_read (optional) Pointer to write number of bytes read.
/// @return
///     - @c true  : Read from file successfully.
///     - @c false : Failed to read from file.
bool cb_file_read(
    CB_File* file, uintptr_t buffer_size, void* buffer, uintptr_t* opt_out_read );
/// @brief Write to file.
/// @param[in]  file          Pointer to file handle.
/// @param      buffer_size   Size of buffer read from.
/// @param[in]  buffer        Pointer to buffer to read from.
/// @param[out] opt_out_write (optional) Pointer to write number of bytes written.
/// @return
///     - @c true  : Written to file successfully.
///     - @c false : Failed to write to file.
bool cb_file_write(
    CB_File* file, uintptr_t buffer_size, const void* buffer, uintptr_t* opt_out_write );
/// @brief Write formatted string to file.
/// @param[in] file Pointer to file handle.
/// @param[in] fmt  Format string.
/// @param[in] va   Variadic arguments.
void cb_file_write_fmt_va( CB_File* file, const char* fmt, va_list va );
/// @brief Write formatted string to file.
/// @param[in] file Pointer to file handle.
/// @param[in] fmt  Format string.
/// @param     ...  Arguments.
void cb_file_write_fmt( CB_File* file, const char* fmt, ... );
/// @brief Delete a file.
/// @param[in] path_utf8 Path to file to remove.
/// @return
///     - @c true  : Removed file successfully.
///     - @c false : Failed to remove file.
bool cb_file_remove( const char* path_utf8 );
/// @brief Copy entire file from src to dst.
/// @param[in] dst                Path to destination file.
/// @param[in] src                Path to source file.
/// @param     fail_if_dst_exists If function should fail if dst exists.
/// @return
///     - @c true  : Successfully copied file.
///     - @c false : Failed to copy file.
bool cb_file_copy( const char* dst, const char* src, bool fail_if_dst_exists );
/// @brief Move entire file from src to dst.
/// @param[in] dst                Path to destination file.
/// @param[in] src                Path to source file.
/// @param     fail_if_dst_exists If function should fail if dst exists.
/// @return
///     - @c true  : Successfully copied file and remove src.
///     - @c false : Failed to copy file.
bool cb_file_move( const char* dst, const char* src, bool fail_if_dst_exists );


/// @brief Create a directory.
/// @param[in] path_utf8 Path to create directory at.
/// @return
///     - @c true  : Created directory successfully.
///     - @c false : Failed to create directory.
bool cb_directory_create( const char* path_utf8 );
/// @brief Delete a directory.
/// @param[in] path_utf8 Path to delete directory at.
/// @param     recursive If true, deletes directory contents.
/// @return
///     - @c true  : If @c recursive,
///               deleted directory contents and directory, otherwise deleted directory.
///     - @c false : If @c recursive,
///               failed to delete item in directory or directory itself,
///               otherwise directory was not empty or could not be found.
bool cb_directory_remove( const char* path_utf8, bool recursive );
/// @brief Go through items in directory.
/// @param[in] path_utf8 Path to directory to walk.
/// @param[in] callback  Callback function to call per item in directory.
/// @param[in] params    (optional) Additional parameter for callback function.
/// @return
///     - @c true  : Successfully walked directory.
///     - @c false : Failed to walk directory.
bool cb_directory_walk( const char* path_utf8, CB_DirectoryWalkFN* callback, void* params );

/// @brief Change directory.
/// @param[in] new_cwd Path to change to.
/// @return
///     - @c true  : Changed directory successfully.
///     - @c false : Failed to change directory.
bool cb_working_directory_set( const char* new_cwd );
/// @brief Query current directory.
/// @note
/// Must be freed using CB_FREE(). Buffer size is strlen() + 1.
/// @return Current directory.
const char* cb_working_directory_query(void);

/// @brief Open pipe.
/// @param[out] out_read  Pointer to write 'read' end of pipe.
/// @param[out] out_write Pointer to write 'write' end of pipe.
/// @return
///      - @c true  : Opened pipe successfully.
///      - @c false : Failed to open pipe.
bool cb_pipe_open( CB_PipeRead* out_read, CB_PipeWrite* out_write );
/// @brief Close pipe.
/// @param[in] pipe Pointer to pipe to close.
void cb_pipe_close( CB_Pipe* pipe );
/// @brief Get stdin pipe.
///
/// @note
/// This is a cross-platform way to get standard pipes as
/// their file descriptors are not fixed integers on other platforms (Windows).
///
/// @return Pipe.
CB_PipeRead cb_pipe_stdin(void);
/// @brief Get stdout pipe.
///
/// @note
/// This is a cross-platform way to get standard pipes as
/// their file descriptors are not fixed integers on other platforms (Windows).
///
/// @return Pipe.
CB_PipeWrite cb_pipe_stdout(void);
/// @brief Get stderr pipe.
///
/// @note
/// This is a cross-platform way to get standard pipes as
/// their file descriptors are not fixed integers on other platforms (Windows).
///
/// @return Pipe.
CB_PipeWrite cb_pipe_stderr(void);

/// @brief Query value of environment variable.
/// @note
/// Value must be freed with CB_FREE(). Buffer size is strlen() + 1.
/// @param[in] name Name of environment variable.
/// @return Value of variable. Returns null if variable could not be found.
const char* cb_environment_query( const char* name );
/// @brief Set value of environment variable.
/// @param[in] name      Name of environment variable.
/// @param[in] new_value New value to set environment.
/// @return
///     - @c true  : Set environment variable successfully.
///     - @c false : Failed to set environment variable.
bool cb_environment_set( const char* name, const char* new_value );

/// @brief Execute process command asynchronously.
/// @note
/// Process ID must be used with one of the following functions:
///     - cb_process_discard()    to free process id.
///     - cb_process_wait_timed() to wait on process. If times out, process id must still be freed.
///     - cb_process_wait()       to wait on process. Automatically frees process id.
///     - cb_process_kill()       to kill process prematurely. Automatically frees process id.
/// @param      cmd                   Command to execute.
/// @param[out] out_pid               Pointer to write process ID to.
/// @param[in]  opt_working_directory (optional) Working directory path.
/// @param[in]  opt_environment       (optional) Environment to execute process in.
/// @param[in]  opt_stdin             (optional) Pointer to stdin pipe.
/// @param[in]  opt_stdout            (optional) Pointer to stdout pipe.
/// @param[in]  opt_stderr            (optional) Pointer to stderr pipe.
/// @return
///     - @c true  : Executed command successfully.
///     - @c false : Failed to execute command.
bool cb_process_exec_async(
    CB_Command             cmd,
    CB_ProcessID*          out_pid,
    const char*            opt_working_directory,
    CB_EnvironmentBuilder* opt_environment,
    CB_PipeRead*           opt_stdin,
    CB_PipeWrite*          opt_stdout,
    CB_PipeWrite*          opt_stderr );

/// @brief Discard process ID.
/// @param[in] pid Process ID to discard.
void cb_process_discard( CB_ProcessID* pid );
/// @brief Wait for process.
/// @param[in] pid ID of process to wait for. It's automatically discarded when process exits.
/// @return
///     - 0-255 : Process exited normally and this is the exit code.
///     - -1    : Process exited abnormally.
///     - -2    : Error occurred when waiting for process.
int cb_process_wait( CB_ProcessID* pid );
/// @brief Wait for process.
/// @param[in]  pid               ID of process to wait for.
/// @param      msec              Milliseconds to wait for.
/// @param[out] opt_out_exit_code (optional) Pointer to write process exit code to.
/// @return
///     - @c true  : Process finished before @c msec. @c pid has automatically been discarded.
///     - @c false : Process timed out.
bool cb_process_wait_timed( CB_ProcessID* pid, uint32_t msec, int* opt_out_exit_code );
/// @brief Kill process prematurely.
/// @param[in] pid ID of process to kill.
void cb_process_kill( CB_ProcessID* pid );
/// @brief Check if process is in path.
/// @param[in] process_name Name of process to check for.
/// @return
///     - @c true  : Process was found.
///     - @c false : Process could not be found.
bool cb_process_is_in_path( const char* process_name );

// NOTE(alicia): Internal Functions

int _cb_internal_process_exec_quick(
    const char*            opt_working_directory,
    CB_EnvironmentBuilder* opt_environment,
    CB_PipeRead*           opt_stdin,
    CB_PipeWrite*          opt_stdout,
    CB_PipeWrite*          opt_stderr,
    ... );

void _cb_internal_command_builder_new( CB_CommandBuilder* builder, ... );
void _cb_internal_command_builder_append( CB_CommandBuilder* builder, ... );
bool _cb_internal_make_directories( const char* first, ... );

static inline void _cb_internal_unused( int _, ... ) { (void)_; }

#if defined(CB_STRIP_PREFIXES)

typedef CB_Time                    Time;
typedef CB_LogLevel                LogLevel;
typedef CB_FileOpenFlags           FileOpenFlags;
typedef CB_FileSeek                FileSeek;
typedef CB_FileType                FileType;
typedef CB_File                    File;
typedef CB_FileInfo                FileInfo;
typedef CB_Pipe                    Pipe;
typedef CB_PipeWrite               PipeWrite;
typedef CB_PipeRead                PipeRead;
typedef CB_DirectoryWalkControl    DirectoryWalkControl;
typedef CB_DirectoryWalkInfo       DirectoryWalkInfo;
typedef CB_DirectoryWalkFN         DirectoryWalkFN;
typedef CB_StringSlice             StringSlice;
typedef CB_StringSliceList         StringSliceList;
typedef CB_StringBuilder           StringBuilder;
typedef CB_Command                 Command;
typedef CB_CommandBuilder          CommandBuilder;
typedef CB_EnvironmentBuilder      EnvironmentBuilder;
typedef CB_ProcessID               ProcessID;
typedef CB_UTFCodePoint8           UTFCodePoint8;
typedef CB_UTFCodePoint16          UTFCodePoint16;
typedef CB_UTFCodePoint32          UTFCodePoint32;
typedef CB_UnicodeValidationResult UnicodeValidationResult;

#define initialize                               cb_initialize
#define rebuild                                  cb_rebuild
#define local_buf                                cb_local_buf
#define local_buf_fmt_va                         cb_local_buf_fmt_va
#define local_buf_fmt                            cb_local_buf_fmt
#define alloc_fmt_va                             cb_alloc_fmt_va
#define alloc_fmt                                cb_alloc_fmt
#define string_cmp                               cb_string_cmp
#define string_find                              cb_string_find
#define string_find_unicode                      cb_string_find_unicode
#define string_find_rev                          cb_string_find_rev
#define string_find_unicode_rev                  cb_string_find_unicode_rev
#define string_find_unicode_rev                  cb_string_find_unicode_rev
#define string_find_set                          cb_string_find_set
#define string_find_set_rev                      cb_string_find_set_rev
#define string_find_set_unicode                  cb_string_find_set_unicode
#define string_find_set_unicode_rev              cb_string_find_set_unicode_rev
#define string_find_phrase                       cb_string_find_phrase
#define string_find_phrase_rev                   cb_string_find_phrase_rev
#define string_advance                           cb_string_advance
#define string_truncate                          cb_string_truncate
#define string_trim                              cb_string_trim
#define string_clip                              cb_string_clip
#define string_trim_leading_whitespace           cb_string_trim_leading_whitespace
#define string_trim_trailing_whitespace          cb_string_trim_trailing_whitespace
#define string_trim_surrounding_whitespace       cb_string_trim_surrounding_whitespace
#define string_split                             cb_string_split
#define string_split_by_char                     cb_string_split_by_char
#define string_split_by_char_unicode             cb_string_split_by_char_unicode
#define string_split_by_set                      cb_string_split_by_set
#define string_split_by_set_unicode              cb_string_split_by_set_unicode
#define string_split_by_phrase                   cb_string_split_by_phrase
#define string_split_by_char_list                cb_string_split_by_char_list
#define string_split_by_char_unicode_list        cb_string_split_by_char_unicode_list
#define string_split_by_set_list                 cb_string_split_by_set_list
#define string_split_by_set_unicode_list         cb_string_split_by_set_unicode_list
#define string_split_by_phrase_list              cb_string_split_by_phrase_list
#define cstr_from_string                         cb_cstr_from_string
#define string_builder_from_string               cb_string_builder_from_string
#define string_unicode_next                      cb_string_unicode_next
#define utf8_len                                 cb_utf8_len
#define utf8_next                                cb_utf8_next
#define utf8_index                               cb_utf8_index
#define cp8_from_code_units                      cb_cp8_from_code_units
#define cp16_from_code_units                     cb_cp16_from_code_units
#define cp32_from_code_units                     cb_cp32_from_code_units
#define cp16_from_bytes                          cb_cp16_from_bytes
#define cp32_from_bytes                          cb_cp32_from_bytes
#define cp16_read_byte                           cb_cp16_read_byte
#define cp32_read_byte                           cb_cp32_read_byte
#define unicode_from_cp8                         cb_unicode_from_cp8
#define unicode_from_cp16                        cb_unicode_from_cp16
#define unicode_from_cp32                        cb_unicode_from_cp32
#define cp8_code_unit_count                      cb_cp8_code_unit_count
#define cp16_code_unit_count                     cb_cp16_code_unit_count
#define cp32_code_unit_count                     cb_cp32_code_unit_count
#define utf8_validate                            cb_utf8_validate
#define utf16_validate                           cb_utf16_validate
#define utf32_validate                           cb_utf32_validate
#define cp8_from_string                          cb_cp8_from_string
#define cp16_from_string                         cb_cp16_from_string
#define cp32_from_string                         cb_cp32_from_string
#define cp8_from_cp16                            cb_cp8_from_cp16
#define cp8_from_cp32                            cb_cp8_from_cp32
#define cp16_from_cp8                            cb_cp16_from_cp8
#define cp16_from_cp32                           cb_cp16_from_cp32
#define cp32_from_cp8                            cb_cp32_from_cp8
#define cp32_from_cp16                           cb_cp32_from_cp16
#define file_exists                              cb_file_exists
#define directory_exists                         cb_directory_exists
#define directory_copy                           cb_directory_copy
#define directory_move                           cb_directory_move
#define make_directories                         cb_make_directories
#define read_entire_file                         cb_read_entire_file
#define which_file_is_newer                      cb_which_file_is_newer
#define which_file_is_newer_many_array           cb_which_file_is_newer_many_array
#define which_file_is_newer_many                 cb_which_file_is_newer_many
#define command_flatten                          cb_command_flatten
#define command_builder_new                      cb_command_builder_new
#define command_builder_append                   cb_command_builder_append
#define command_builder_add_null_terminator      cb_command_builder_add_null_terminator
#define command_builder_remove_null_terminator   cb_command_builder_remove_null_terminator
#define command_builder_from_cmd                 cb_command_builder_from_cmd
#define command_builder_remove                   cb_command_builder_remove
#define command_builder_free                     cb_command_builder_free
#define environment_builder_new                  cb_environment_builder_new
#define environment_builder_free                 cb_environment_builder_free
#define environment_builder_append               cb_environment_builder_append
#define environment_builder_remove               cb_environment_builder_remove
#define environment_builder_remove_by_index      cb_environment_builder_remove_by_index
#define environment_builder_replace              cb_environment_builder_replace
#define environment_builder_replace_by_index     cb_environment_builder_replace_by_index
#define process_exec                             cb_process_exec
#define process_exec_quick                       cb_process_exec_quick
#define process_exec_quick_ex                    cb_process_exec_quick_ex
#define process_wait_many                        cb_process_wait_many
#define log_level_set                            cb_log_level_set
#define log_level_query                          cb_log_level_query
#define log_level_is_valid                       cb_log_level_is_valid
#define write_log_va                             cb_write_log_va
#define write_log                                cb_write_log
#define time_query                               cb_time_query
#define time_msec                                cb_time_msec
#define time_sec                                 cb_time_sec
#define path_query_type                          cb_path_query_type
#define path_query_time_modify                   cb_path_query_time_modify
#define path_query_time_create                   cb_path_query_time_create
#define path_query_info                          cb_path_query_info
#define path_canonicalize                        cb_path_canonicalize
#define file_open                                cb_file_open
#define file_close                               cb_file_close
#define file_seek                                cb_file_seek
#define file_truncate                            cb_file_truncate
#define file_read                                cb_file_read
#define file_write                               cb_file_write
#define file_write_fmt_va                        cb_file_write_fmt_va
#define file_write_fmt                           cb_file_write_fmt
#define file_remove                              cb_file_remove
#define file_copy                                cb_file_copy
#define file_move                                cb_file_move
#define directory_create                         cb_directory_create
#define directory_remove                         cb_directory_remove
#define directory_walk                           cb_directory_walk
#define working_directory_set                    cb_working_directory_set
#define working_directory_query                  cb_working_directory_query
#define pipe_open                                cb_pipe_open
#define pipe_close                               cb_pipe_close
#define pipe_stdin                               cb_pipe_stdin
#define pipe_stdout                              cb_pipe_stdout
#define pipe_stderr                              cb_pipe_stderr
#define environment_query                        cb_environment_query
#define environment_set                          cb_environment_set
#define process_exec_async                       cb_process_exec_async
#define process_discard                          cb_process_discard
#define process_wait                             cb_process_wait
#define process_wait_timed                       cb_process_wait_timed
#define process_kill                             cb_process_kill
#define process_is_in_path                       cb_process_is_in_path

#endif /* Strip prefixes */

#endif /* header guard */























#if defined(CB_IMPLEMENTATION)
/**
 * @file   cbuild.h
 * @brief  This is where C-Build's implementation begins :)
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   December 28, 2024
*/
#include <time.h>
#include <ctype.h>

struct CB_LocalBuffer {
    char buffer[CB_LOCAL_BUFFER_CAPACITY];
};

struct CB_State {
    CB_LogLevel            level;
    struct CB_LocalBuffer* buffers;
    uint32_t               buffer_counter;

    void* platform_state;
};

static
struct CB_State global_state = {
    .level          = CB_LOG_FATAL,
    .buffers        = NULL,
    .buffer_counter = 0,
    .platform_state = NULL,
};

void cb_initialize(
    CB_LogLevel  log_level,
    const char*  path_executable,
    const char*  path_source,
    int          argc,
    const char** argv
) {
    cb_log_level_set( log_level );

    if( !cb_file_exists( __FILE__ ) ) {
        CB_PANIC( "cbuild MUST be run from its source code directory!" );
    }
    if( !cb_file_exists( path_source ) ) {
        CB_PANIC( "cbuild MUST be run from its source code directory!" );
    }

    bool should_rebuild =
        cb_which_file_is_newer_many( path_executable, path_source, __FILE__ ) != 0;

    if( !should_rebuild ) {
        const char* old_name = cb_local_buf_fmt( "%s.old", path_executable );
        if( cb_file_exists( old_name ) ) {
            cb_file_remove( old_name );
        }
        return;
    }

    CB_ATTEN( "changes detected in cbuild source, rebuilding . . ." );
    cb_rebuild( path_executable, path_source, argc, argv, NULL, true );
}

CB_NO_RETURN
void cb_rebuild(
    const char*  path_executable,
    const char*  path_source,
    int          argc,
    const char** argv,
    CB_Command*  opt_cmd_override,
    bool         should_reload 
) {
    double start = cb_time_msec();
    CB_StringSlice executable = CB_STRING_FROM_CSTR( path_executable );

    CB_CommandBuilder builder = {};
    CB_StringBuilder  string  = {};

    if( opt_cmd_override ) {
        for( int i = 0; i < opt_cmd_override->len; ++i ) {
            cb_command_builder_append( &builder, opt_cmd_override->buf[i] );
        }
    } else {
        cb_command_builder_append( &builder, CB_RECOMPILE_COMPILER, path_source );

        if( strcmp( CB_RECOMPILE_COMPILER, "cl" ) == 0 ) {
            CB_STRING_APPEND( &string, "/Fe" );
            CB_APPEND( &string, executable.len, executable.cbuf );
            CB_PUSH( &string, 0 );

            cb_command_builder_append( &builder, "/nologo", string.buf );

            string.len = 0;
        } else {
            cb_command_builder_append( &builder, "-o", path_executable );
        }

    }

    cb_command_builder_add_null_terminator( &builder );
    cb_command_flatten( builder.cmd, &string );
    CB_PUSH( &string, 0 );

    CB_ATTEN( "rebuilding with command: %s", string.buf );
    string.len = 0;

    CB_APPEND( &string, executable.len, executable.cbuf );
    CB_STRING_APPEND( &string, ".old\0" );

    switch( cb_path_query_type( string.buf ) ) {
        case CB_FTYPE_NULL:
            break;
        case CB_FTYPE_FILE: {
            if( !cb_file_remove( string.buf ) ) {
                CB_PANIC( "failed to remove %s!", string.buf );
            }
        } break;
        case CB_FTYPE_DIRECTORY: {
            CB_PANIC( "%s is a directory!", string.buf );
        } break;
    }

    if( !cb_file_move( string.buf, path_executable, false ) ) {
        CB_PANIC( "failed to rename existing executable!" );
    }

    CB_ProcessID pid = CB_PROCESS_ID_NULL();
    if( !cb_process_exec_async(
        builder.cmd,
        &pid,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    ) ) {
        CB_PANIC( "failed to execute rebuild command!" );
    }
    int result = cb_process_wait( &pid );
    if( result != 0 ) {
        if( !cb_file_move( path_executable, string.buf, false ) ) {
            CB_ERROR( "failed to move %s to %s!", string.buf, path_executable );
        }
        CB_PANIC( "failed to rebuild!" );
    }

    string.len = 0;

    if( strcmp( builder.buf[0], "cl" ) == 0 ) {
        cb_string_builder_from_string( &string, executable );
        int dot = 0;
        if( ( dot = cb_string_find_rev( string.slice, '.' ) ) >= 0 ) {
            string.len = dot;
        }
        CB_STRING_APPEND( &string, ".obj\0" );

        if( cb_file_exists( string.buf ) ) {
            cb_file_remove( string.buf );
        }
    }

    double end = cb_time_msec();
    CB_INFO( "rebuilt in %fms", end - start );

    if( !should_reload ) {
        exit( 0 );
    }

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    CB_WARN(
        "Windows does not support automatically "
        "reloading cbuild. Please run command again." );
    exit( 0 );
#endif

    CB_ATTEN( "reloading . . ." );

    cb_command_builder_free( &builder );
    cb_command_builder_append( &builder, path_executable );
    for( int i = 1; i < argc; ++i ) {
        cb_command_builder_append( &builder, argv[i] );
    }
    cb_command_builder_add_null_terminator( &builder );

    int exit_code = 0;
    if( !cb_process_exec( builder.cmd, &exit_code, NULL, NULL, NULL, NULL, NULL ) ) {
        CB_PANIC( "failed to reload!" );
    }
    exit( exit_code );
}

char* cb_local_buf(void) {
    if( !global_state.buffers ) {
        global_state.buffers = CB_ALLOC(
            NULL, 0, sizeof(struct CB_LocalBuffer) * CB_LOCAL_BUFFER_COUNT );
    }

    char* result =
        global_state.buffers[global_state.buffer_counter % CB_LOCAL_BUFFER_COUNT].buffer;
    global_state.buffer_counter++;
    return memset( result, 0, CB_LOCAL_BUFFER_CAPACITY );
}
char* cb_local_buf_fmt_va( const char* fmt, va_list va ) {
    char* local = cb_local_buf();
    vsnprintf( local, CB_LOCAL_BUFFER_CAPACITY, fmt, va );
    return local;
}
char* cb_local_buf_fmt( const char* fmt, ... ) {
    va_list va;
    va_start( va, fmt );
    char* result = cb_local_buf_fmt_va( fmt, va );
    va_end( va );
    return result;
}

char* cb_alloc_fmt_va( const char* fmt, va_list va ) {
    va_list copy;
    va_copy( copy, va );
    int size = vsnprintf( NULL, 0, fmt, copy );
    va_end( copy );

    if( !size ) {
        return NULL;
    }

    char* result = CB_ALLOC( NULL, 0, size + 1 );
    vsnprintf( result, size + 1, fmt, va );
    return result;
}
char* cb_alloc_fmt( const char* fmt, ... ) {
    va_list va;
    va_start( va, fmt );
    char* result = cb_alloc_fmt_va( fmt, va );
    va_end( va );
    return result;
}

int cb_string_cmp( CB_StringSlice a, CB_StringSlice b ) {
    if( a.len == b.len ) {
        return memcmp( a.buf, b.buf, a.len ) == 0;
    }
    return (a.len < b.len) ? -1 : 1;
}
int cb_string_find( CB_StringSlice string, char c ) {
    for( int i = 0; i < string.len; ++i ) {
        if( string.cbuf[i] == c ) {
            return i;
        }
    }
    return -1;
}
int cb_string_find_unicode( CB_StringSlice string, uint32_t c ) {
    int string_unicode_len = cb_utf8_len( string.len, string.cbuf );

    for( int i = 0; i < string_unicode_len; ++i ) {
        uint32_t    current = 0;
        const char* at      = cb_utf8_index( string.len, string.cbuf, i, &current );

        if( current == c ) {
            return (int)(at - string.cbuf);
        }
    }

    return -1;
}
int cb_string_find_rev( CB_StringSlice string, char c ) {
    for( int i = string.len; i-- > 0; ) {
        if( string.cbuf[i] == c ) {
            return i;
        }
    }
    return -1;
}
int cb_string_find_unicode_rev( CB_StringSlice string, uint32_t c ) {
    int string_unicode_len = cb_utf8_len( string.len, string.cbuf );

    for( int i = string_unicode_len; i-- > 0; ) {
        uint32_t    current = 0;
        const char* at      = cb_utf8_index( string.len, string.cbuf, i, &current );

        if( current == c ) {
            return (int)(at - string.cbuf);
        }
    }

    return -1;
}
int cb_string_find_set( CB_StringSlice string, CB_StringSlice set ) {
    for( int i = 0; i < string.len; ++i ) {
        for( int j = 0; j < set.len; ++j ) {
            if( string.cbuf[i] == set.cbuf[j] ) {
                return i;
            }
        }
    }
    return -1;
}
int cb_string_find_set_rev( CB_StringSlice string, CB_StringSlice set ) {
    for( int i = string.len; i-- > 0; ) { 
        for( int j = 0; j < set.len; ++j ) {
            if( string.cbuf[i] == set.cbuf[j] ) {
                return i;
            }
        }
    }
    return -1;
}
int cb_string_find_set_unicode( CB_StringSlice string, int set_len, uint32_t* set ) {
    CB_StringSlice remaining = string;
    while( remaining.len ) {
        uint32_t character = 0;
        CB_StringSlice next_remaining = cb_string_unicode_next( remaining, &character );

        for( int i = 0; i < set_len; ++i ) {
            if( character == set[i] ) {
                return (int)(remaining.cbuf - string.cbuf);
            }
        }

        remaining = next_remaining;
    }
    return -1;
}
int cb_string_find_set_unicode_rev( CB_StringSlice string, int set_len, uint32_t* set ) {
    int string_unicode_len = cb_utf8_len( string.len, string.cbuf );
    for( int i = string_unicode_len; i-- > 0; ) {
        uint32_t character = 0;
        const char* ptr = cb_utf8_index( string.len, string.cbuf, i, &character );
        for( int j = 0; j < set_len; ++j ) {
            if( character == set[j] ) {
                return (int)(ptr - string.cbuf);
            }
        }
    }
    return -1;
}
int cb_string_find_phrase( CB_StringSlice string, CB_StringSlice phrase ) {
    for( int i = 0; i < string.len; ++i ) {
        if( string.len - i < phrase.len ) {
            break;
        }

        if( string.cbuf[i] == phrase.cbuf[i] ) {
            CB_StringSlice chunk = cb_string_truncate(
                CB_ADVANCE( CB_StringSlice, &string, i ), phrase.len );
            if( cb_string_cmp( chunk, phrase ) ) {
                return i;
            }
        }
    }
    return -1;
}
int cb_string_find_phrase_rev( CB_StringSlice string, CB_StringSlice phrase ) {
    for( int i = string.len; i-- > 0; ) {
        if( string.len - i < phrase.len ) {
            continue;
        }

        if( string.cbuf[i] == phrase.cbuf[i] ) {
            CB_StringSlice chunk = cb_string_truncate(
                CB_ADVANCE( CB_StringSlice, &string, i ), phrase.len );
            if( cb_string_cmp( chunk, phrase ) ) {
                return i;
            }
        }
    }
    return -1;
}
CB_StringSlice cb_string_advance( CB_StringSlice string, int amount ) {
    return CB_ADVANCE( CB_StringSlice, &string, amount );
}
CB_StringSlice cb_string_truncate( CB_StringSlice string, int max ) {
    if( string.len < max ) {
        return string;
    } else {
        return CB_STRING_SLICE( max, string.cbuf );
    }
}
CB_StringSlice cb_string_trim( CB_StringSlice string, int amount ) {
    if( string.len < amount ) {
        return CB_STRING_SLICE( 0, string.cbuf );
    } else {
        return CB_STRING_SLICE( string.len - amount, string.cbuf );
    }
}
CB_StringSlice cb_string_clip( CB_StringSlice string, int from_inc, int to_exc ) {
    return CB_STRING_SLICE( (to_exc - from_inc), string.cbuf + from_inc );
}
CB_StringSlice cb_string_trim_leading_whitespace( CB_StringSlice string ) {
    for( int i = 0; i < string.len; ++i ) {
        if( !isspace( string.cbuf[i] ) ) {
            return CB_ADVANCE( CB_StringSlice, &string, i );
        }
    }
    return string;
}
CB_StringSlice cb_string_trim_trailing_whitespace( CB_StringSlice string ) {
    for( int i = string.len; i-- > 0; ) {
        if( !isspace( string.cbuf[i] ) ) {
            return cb_string_truncate( string, i + 1 );
        }
    }
    return string;
}
CB_StringSlice cb_string_trim_surrounding_whitespace( CB_StringSlice string ) {
    return cb_string_trim_leading_whitespace( cb_string_trim_trailing_whitespace( string ) );
}
void cb_string_split(
    CB_StringSlice string, int index, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right 
) {
    if( should_include ) {
        *out_left  = cb_string_truncate( string, index + 1 );
    } else {
        *out_left  = cb_string_truncate( string, index );
    }
    *out_right = CB_ADVANCE( CB_StringSlice, &string, index + 1 );
}
bool cb_string_split_by_char(
    CB_StringSlice string, char c, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right
) {
    int index = 0;
    if( ( index = cb_string_find( string, c ) ) < 0 ) {
        *out_left = string;
        return false;
    }

    cb_string_split( string, index, should_include, out_left, out_right );
    return true;
}
bool cb_string_split_by_char_unicode(
    CB_StringSlice string, uint32_t c, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right
) {
    int index = 0;
    if( ( index = cb_string_find_unicode( string, c ) ) < 0 ) {
        *out_left = string;
        return false;
    }

    cb_string_split( string, index, should_include, out_left, out_right );
    return true;
}
bool cb_string_split_by_set(
    CB_StringSlice string, CB_StringSlice set, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right
) {
    int index = 0;
    if( ( index = cb_string_find_set( string, set ) ) < 0 ) {
        *out_left = string;
        return false;
    }

    cb_string_split( string, index, should_include, out_left, out_right );
    return true;
}
bool cb_string_split_by_set_unicode(
    CB_StringSlice string, int set_len, uint32_t* set, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right
) {
    int index = 0;
    if( ( index = cb_string_find_set_unicode( string, set_len, set ) ) < 0 ) {
        *out_left = string;
        return false;
    }

    cb_string_split( string, index, should_include, out_left, out_right );
    return true;
}
bool cb_string_split_by_phrase(
    CB_StringSlice string, CB_StringSlice phrase, bool should_include,
    CB_StringSlice* out_left, CB_StringSlice* out_right
) {
    int index = 0;
    if( ( index = cb_string_find_phrase( string, phrase ) ) < 0 ) {
        *out_left = string;
        return false;
    }

    cb_string_split( string, index, should_include, out_left, out_right );
    return true;
}
int cb_string_split_by_char_list(
    CB_StringSlice string, char c,
    bool should_include, CB_StringSliceList* out_list
) {
    CB_StringSlice remaining = string;
    int count = 0;
    while( remaining.len ) {
        CB_StringSlice item = {};
        if( cb_string_split_by_char( remaining, c, should_include, &item, &remaining ) ) {
            CB_PUSH( out_list, item );
            count++;
        } else {
            CB_PUSH( out_list, item );
            count++;
            break;
        }
    }
    return count;
}
int cb_string_split_by_char_unicode_list(
    CB_StringSlice string, uint32_t c,
    bool should_include, CB_StringSliceList* out_list 
) {
    CB_StringSlice remaining = string;
    int count = 0;
    while( remaining.len ) {
        CB_StringSlice item = {};
        if( cb_string_split_by_char_unicode(
            remaining, c, should_include, &item, &remaining 
        ) ) {
            CB_PUSH( out_list, item );
            count++;
        } else {
            CB_PUSH( out_list, item );
            count++;
            break;
        }
    }
    return count;
}
int cb_string_split_by_set_list(
    CB_StringSlice string, CB_StringSlice set,
    bool should_include, CB_StringSliceList* out_list
) {
    CB_StringSlice remaining = string;
    int count = 0;
    while( remaining.len ) {
        CB_StringSlice item = {};
        if( cb_string_split_by_set(
            remaining, set, should_include, &item, &remaining
        ) ) {
            CB_PUSH( out_list, item );
            count++;
        } else {
            CB_PUSH( out_list, item );
            count++;
            break;
        }
    }
    return count;
}
int cb_string_split_by_set_unicode_list(
    CB_StringSlice string, int set_len, uint32_t* set,
    bool should_include, CB_StringSliceList* out_list
) {
    CB_StringSlice remaining = string;
    int count = 0;
    while( remaining.len ) {
        CB_StringSlice item = {};
        if( cb_string_split_by_set_unicode(
            remaining, set_len, set, should_include, &item, &remaining
        ) ) {
            CB_PUSH( out_list, item );
            count++;
        } else {
            CB_PUSH( out_list, item );
            count++;
            break;
        }
    }
    return count;
}
int cb_string_split_by_phrase_list(
    CB_StringSlice string, CB_StringSlice phrase,
    bool should_include, CB_StringSliceList* out_list
) {
    CB_StringSlice remaining = string;
    int count = 0;
    while( remaining.len ) {
        CB_StringSlice item = {};
        if( cb_string_split_by_phrase(
            remaining, phrase, should_include, &item, &remaining
        ) ) {
            CB_PUSH( out_list, item );
            count++;
        } else {
            CB_PUSH( out_list, item );
            count++;
            break;
        }
    }
    return count;
}

char* cb_cstr_from_string(
    CB_StringSlice string, bool replace_null, uint32_t replacement_character
) {
    int   buffer_size = 0;
    char* buffer      = NULL;

    if( replace_null ) {
        CB_UTFCodePoint8 replacement_cp8 =
            cb_cp8_from_cp32( cb_cp32_from_code_units( replacement_character ) );
        int replacement_cp8_count = cb_cp8_code_unit_count( replacement_cp8 );

        int null_characters_found = 0;
        for( int i = 0; i < string.len; ++i ) {
            if( !string.cbuf[i] ) {
                null_characters_found++;
            }
        }

        if( !null_characters_found ) {
            goto dont_replace_null;
        }

        buffer_size =
            (null_characters_found * replacement_cp8_count) +
            (string.len - null_characters_found)             + 1;

        buffer      = CB_ALLOC( NULL, 0, buffer_size );
        int index = 0;

        for( int i = 0; i < string.len; ++i ) {
            if( string.cbuf[i] ) {
                buffer[index++] = string.cbuf[i];
            } else {
                for( int j = 0; j < replacement_cp8_count; ++j ) {
                    buffer[index++] = replacement_cp8.code_units[j];
                }
            }
        }

        return buffer;
    }

dont_replace_null:

    buffer_size = string.len + 1;
    buffer      = CB_ALLOC( NULL, 0, buffer_size );

    return memcpy( buffer, string.cbuf, string.len );
}
void cb_string_builder_from_string(
    CB_StringBuilder* builder, CB_StringSlice string 
) {
    CB_APPEND( builder, string.len, string.cbuf );
}
CB_StringSlice cb_string_unicode_next( CB_StringSlice string, uint32_t* out_character ) {
    CB_UTFCodePoint8 cp8 = {};
    int adv = cb_cp8_from_string( string.len, string.bytes, &cp8 );
    *out_character = cb_unicode_from_cp8( cp8 );
    return cb_string_advance( string, adv );
}

int _cb_internal_cstr_utf8_len( const uint8_t* cstr ) {
    int result = 0;
    while( *cstr ) {
        if( (*cstr & 0xC0) != 0x80 ) {
            result++;
        }
        cstr++;
    }
    return result;
}
int _cb_internal_string_utf8_len( int len, const uint8_t* str ) {
    int result = 0;
    for( int i = 0; i < len; ++i ) {
        if( (str[i] & 0xC0) != 0x80 ) {
            result++;
        }
    }
    return result;
}
int cb_utf8_len( int opt_len, const char* str_utf8 ) {
    if( !str_utf8 ) {
        return 0;
    }

    if( opt_len ) {
        return _cb_internal_string_utf8_len( opt_len, (const uint8_t*)str_utf8 );
    } else {
        return _cb_internal_cstr_utf8_len( (const uint8_t*)str_utf8 );
    }
}
const char* _cb_internal_cstr_utf8_next( const uint8_t* str, uint32_t* out_character ) {
    if( !(str[0] & ~0x7F) ) {
        CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], 0, 0, 0 );
        *out_character = cb_unicode_from_cp8( cp8 );
        return (const char*)( str + 1 );
    } else if( (str[0] & 0xE0) == 0xC0 ) {
        if( str[1] ) {
            CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], str[1], 0, 0 );
            *out_character = cb_unicode_from_cp8( cp8 );
            return (const char*)( str + 2 );
        } else {
            goto _cb_internal_cstr_utf8_next_error;
        }
    } else if( (str[0] & 0xF0) == 0xE0 ) {
        if( str[1] && str[2] ) {
            CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], str[1], str[2], 0 );
            *out_character = cb_unicode_from_cp8( cp8 );
            return (const char*)( str + 3 );
        } else {
            goto _cb_internal_cstr_utf8_next_error;
        }
    } else if( (str[0] & 0xF8) == 0xF0 ) {
        if( str[1] && str[2] && str[3] ) {
            CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], str[1], str[2], str[3] );
            *out_character = cb_unicode_from_cp8( cp8 );
            return (const char*)( str + 4 );
        } else {
            goto _cb_internal_cstr_utf8_next_error;
        }
    }

_cb_internal_cstr_utf8_next_error:
    *out_character = CB_UNICODE_CP32_REPLACEMENT_CHARACTER.rune;
    return (const char*)( str + 1 );
}
const char* _cb_internal_string_utf8_next(
    int len, const uint8_t* str, uint32_t* out_character
) {
    if( !(str[0] & ~0x7F) ) {
        CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], 0, 0, 0 );
        *out_character = cb_unicode_from_cp8( cp8 );
        return (const char*)( str + 1 );
    } else if( (str[0] & 0xE0) == 0xC0 ) {
        if( len >= 2 ) {
            CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], str[1], 0, 0 );
            *out_character = cb_unicode_from_cp8( cp8 );
            return (const char*)( str + 2 );
        } else {
            goto _cb_internal_string_utf8_next_error;
        }
    } else if( (str[0] & 0xF0) == 0xE0 ) {
        if( len >= 3 ) {
            CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], str[1], str[2], 0 );
            *out_character = cb_unicode_from_cp8( cp8 );
            return (const char*)( str + 3 );
        } else {
            goto _cb_internal_string_utf8_next_error;
        }
    } else if( (str[0] & 0xF8) == 0xF0 ) {
        if( len >= 4 ) {
            CB_UTFCodePoint8 cp8 = cb_cp8_from_code_units( str[0], str[1], str[2], str[3] );
            *out_character = cb_unicode_from_cp8( cp8 );
            return (const char*)( str + 4 );
        } else {
            goto _cb_internal_string_utf8_next_error;
        }
    }

_cb_internal_string_utf8_next_error:
    *out_character = CB_UNICODE_CP32_REPLACEMENT_CHARACTER.rune;
    return (const char*)( str + 1 );
}
const char* cb_utf8_next(
    int opt_len, const char* str_utf8, uint32_t* out_character
) {
    if( !str_utf8 ) {
        *out_character = CB_UNICODE_CP32_REPLACEMENT_CHARACTER.rune;
        return NULL;
    }

    if( opt_len ) {
        return _cb_internal_string_utf8_next(
            opt_len, (const uint8_t*)str_utf8, out_character );
    } else {
        if( !*str_utf8 ) {
            *out_character = CB_UNICODE_CP32_REPLACEMENT_CHARACTER.rune;
            return NULL;
        }
        return _cb_internal_cstr_utf8_next(
            (const uint8_t*)str_utf8, out_character );
    }
}
const uint8_t* _cb_internal_cstr_utf8_index(
    const uint8_t* str, int index, uint32_t* out_character
) {
    int counter = 0;
    const uint8_t* at = str;
    while( *at ) {
        if( counter == index ) {
            break;
        }
        if( (*at & 0xC0) != 0x80 ) {
            counter++;
        }
        at++;
    }
    CB_ASSERT( counter == index, "cb_utf8_index: index is out of bounds!" );

    CB_UTFCodePoint8 cp8 = {};
    cb_cp8_from_string( strlen( (const char*)at ), at, &cp8 );
    *out_character = cb_unicode_from_cp8( cp8 );
    return at;
}
const uint8_t* _cb_internal_string_utf8_index(
    int len, const uint8_t* str, int index, uint32_t* out_character
) {
    int counter     = 0;
    int byte_offset = 0;
    for( ; byte_offset < len; ++byte_offset ) {
        if( counter == index ) {
            break;
        }
        if( (str[byte_offset] & 0xC0) != 0x80 ) {
            counter++;
        }
    }
    CB_ASSERT( counter == index, "cb_utf8_index: index is out of bounds!" );

    CB_UTFCodePoint8 cp8 = {};
    cb_cp8_from_string( len - byte_offset, str + byte_offset, &cp8 );
    *out_character = cb_unicode_from_cp8( cp8 );
    return str + byte_offset;
}
const char* cb_utf8_index(
    int opt_len, const char* str_utf8, int index, uint32_t* out_character 
) {
    if( !str_utf8 ) {
        *out_character = CB_UNICODE_CP32_REPLACEMENT_CHARACTER.rune;
        return NULL;
    }
    if( opt_len ) {
        return (const char*)_cb_internal_string_utf8_index(
            opt_len, (const uint8_t*)str_utf8, index, out_character );
    } else {
        return (const char*)_cb_internal_cstr_utf8_index(
            (const uint8_t*)str_utf8, index, out_character );
    }
}
uint32_t cb_unicode_from_cp8( CB_UTFCodePoint8 cp8 ) {
    return cb_cp32_from_cp8( cp8 ).rune;
}
uint32_t cb_unicode_from_cp16( CB_UTFCodePoint16 cp16 ) {
    return cb_cp32_from_cp16( cp16 ).rune;
}
uint32_t cb_unicode_from_cp32( CB_UTFCodePoint32 cp32 ) {
    return cp32.rune;
}
int cb_cp8_code_unit_count( CB_UTFCodePoint8 cp8 ) {
    if( !(cp8.code_units[0] & ~0x7F) ) {
        return 1;
    } else if( (cp8.code_units[0] & 0xE0) == 0xC0 ) {
        return 2;
    } else if( (cp8.code_units[0] & 0xF0) == 0xE0 ) {
        return 3;
    } else if( (cp8.code_units[0] & 0xF8) == 0xF0 ) {
        return 4;
    } else {
        return 0;
    }
}
int cb_cp16_code_unit_count( CB_UTFCodePoint16 cp16 ) {
    if( cp16.code_units[0] <= 0xD7FF ) {
        return 1;
    } else if( cp16.code_units[0] <= 0xDBFF ) {
        return 2;
    } else {
        return 0;
    }
}
int cb_cp32_code_unit_count( CB_UTFCodePoint32 cp32 ) {
    CB_UNUSED(cp32);
    return 1;
}
CB_UnicodeValidationResult cb_utf8_validate(
    int len, const uint8_t* utf8, uint32_t* opt_out_rune,
    const uint8_t** opt_out_error, int* opt_out_advance 
) {
    if( !len ) {
        return CB_UNICODE_RESULT_UNFINISHED;
    }

    if( !(utf8[0] & ~0x7F) ) {
        if( opt_out_rune ) {
            *opt_out_rune = utf8[0];
        }
        if( opt_out_advance ) {
            *opt_out_advance = 1;
        }
        return CB_UNICODE_RESULT_OK;
    } else if( (utf8[0] & 0xE0) == 0xC0 ) {
        if( len >= 2 ) {
            uint32_t rune = utf8[0] & 0x1F;

            if( (utf8[1] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 1;
                }
                return CB_UNICODE_RESULT_INVALID;
            }

            rune = ( rune << 6 ) | ( utf8[1] & 0x3F );

            if( opt_out_rune ) {
                *opt_out_rune = rune;
            }
            if( opt_out_advance ) {
                *opt_out_advance = 2;
            }
            return CB_UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 2 - len;
            }
            return CB_UNICODE_RESULT_UNFINISHED;
        }
    } else if( (utf8[0] & 0xF0) == 0xE0 ) {
        if( len >= 3 ) {
            uint32_t rune = utf8[0] & 0x0F;

            if( (utf8[1] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 1;
                }
                return CB_UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( utf8[1] & 0x3F );

            if( (utf8[2] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 2;
                }
                return CB_UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( utf8[2] & 0x3F );

            if( opt_out_rune ) {
                *opt_out_rune = rune;
            }
            if( opt_out_advance ) {
                *opt_out_advance = 3;
            }
            return CB_UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 3 - len;
            }
            return CB_UNICODE_RESULT_UNFINISHED;
        }
    } else if( (utf8[0] & 0xF8) == 0xF0 ) {
        if( len >= 4 ) {
            uint32_t rune = utf8[0] & 0x07;

            if( (utf8[1] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 1;
                }
                return CB_UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( utf8[1] & 0x37 );

            if( (utf8[2] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 2;
                }
                return CB_UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( utf8[2] & 0x37 );

            if( (utf8[3] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 3;
                }
                return CB_UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( utf8[3] & 0x37 );

            if( rune > 0x10FFFF ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 3;
                }
                return CB_UNICODE_RESULT_INVALID;
            }

            if( opt_out_advance ) {
                *opt_out_advance = 4;
            }
            return CB_UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 4 - len;
            }
            return CB_UNICODE_RESULT_UNFINISHED;
        }
    }

    if( opt_out_error ) {
        *opt_out_error = utf8;
    }
    return CB_UNICODE_RESULT_INVALID;
}
CB_UnicodeValidationResult cb_utf16_validate(
    int len, const uint16_t* utf16, uint32_t* opt_out_rune,
    const uint16_t** opt_out_error, int* opt_out_advance
) {
    if( !len ) {
        return CB_UNICODE_RESULT_UNFINISHED;
    }

    if( utf16[0] <= 0xD7FF ) {
        // FIXME(alicia): may still be invalid
        uint32_t rune = utf16[0];

        if( opt_out_rune ) {
            *opt_out_rune = rune;
        }
        if( opt_out_advance ) {
            *opt_out_advance = 1;
        }
        return CB_UNICODE_RESULT_OK;
    } else if( utf16[0] <= 0xDBFF ) {
        if( len >= 2 ) {
            uint32_t rune = 0;

            uint16_t hi = ( utf16[0] - 0xD800 ) * 0x400;
            uint16_t lo = ( utf16[1] - 0xDC00 );

            rune = (lo | hi) + 0x10000;

            if( rune > 0x10FFFF ) {
                if( opt_out_error ) {
                    *opt_out_error = utf16 + 2;
                }
                return CB_UNICODE_RESULT_INVALID;
            }
            // FIXME(alicia): may still be invalid

            if( opt_out_rune ) {
                *opt_out_rune = rune;
            }
            if( opt_out_advance ) {
                *opt_out_advance = 2;
            }
            return CB_UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 2 - len;
            }
            return CB_UNICODE_RESULT_UNFINISHED;
        }
    }

    if( opt_out_error ) {
        *opt_out_error = utf16;
    }
    return CB_UNICODE_RESULT_INVALID;
}
CB_UnicodeValidationResult cb_utf32_validate(
    int len, const uint32_t* utf32, uint32_t* opt_out_rune,
    const uint32_t** opt_out_error, int* opt_out_advance
) {
    if( !len ) {
        return CB_UNICODE_RESULT_UNFINISHED;
    }

    if( *utf32 > 0x10FFFF ) {
        if( opt_out_error ) {
            *opt_out_error = utf32;
        }
        return CB_UNICODE_RESULT_INVALID;
    }

    // FIXME(alicia): may still be invalid

    if( opt_out_rune ) {
        *opt_out_rune = *utf32;
    }
    if( opt_out_advance ) {
        *opt_out_advance = 1;
    }
    return CB_UNICODE_RESULT_OK;
}
int cb_cp8_from_string( int len, const uint8_t* utf8, CB_UTFCodePoint8* out_cp8 ) {
    if( !(utf8[0] & ~0x7F) ) {
        *out_cp8 = cb_cp8_from_code_units( utf8[0], 0, 0, 0 );
        return 1;
    } else if( (utf8[0] & 0xE0) == 0xC0 ) {
        if( len >= 2 ) {
            *out_cp8 = cb_cp8_from_code_units( utf8[0], utf8[1], 0, 0 );
            return 2;
        } else {
            goto cb_cp8_from_string_error;
        }
    } else if( (utf8[0] & 0xF0) == 0xE0 ) {
        if( len >= 3 ) {
            *out_cp8 = cb_cp8_from_code_units( utf8[0], utf8[1], utf8[2], 0 );
            return 3;
        } else {
            goto cb_cp8_from_string_error;
        }
    } else if( (utf8[0] & 0xF8) == 0xF0 ) {
        if( len >= 4 ) {
            *out_cp8 = cb_cp8_from_code_units( utf8[0], utf8[1], utf8[2], utf8[3] );
            return 4;
        } else {
            goto cb_cp8_from_string_error;
        }
    }

cb_cp8_from_string_error:
    *out_cp8 = CB_UNICODE_CP8_REPLACEMENT_CHARACTER;
    return 1;
}
int cb_cp16_from_string( int len, const uint16_t* utf16, CB_UTFCodePoint16* out_cp16 ) {
    if( !len ) {
        return 0;
    }

    if( utf16[0] <= 0xD7FF ) {
        *out_cp16 = cb_cp16_from_code_units( utf16[0], 0 );
        return 1;
    } else if( utf16[0] <= 0xDBFF ) {
        if( len >= 2 ) {
            *out_cp16 = cb_cp16_from_code_units( utf16[0], utf16[1] );
            return 2;
        } else {
            goto cb_cp16_from_string_error;
        }
    }

cb_cp16_from_string_error:
    *out_cp16 = CB_UNICODE_CP16_REPLACEMENT_CHARACTER;
    return 1;
}
int cb_cp32_from_string( int len, const uint32_t* utf32, CB_UTFCodePoint32* out_cp32 ) {
    if( !len ) {
        return 0;
    }

    if( *utf32 > 0x10FFFF ) {
        *out_cp32 = CB_UNICODE_CP32_REPLACEMENT_CHARACTER;
        return 1;
    }

    out_cp32->rune = *utf32;
    return 1;
}
CB_UTFCodePoint8 cb_cp8_from_cp16( CB_UTFCodePoint16 cp16 ) {
    CB_UTFCodePoint32 cp32 = cb_cp32_from_cp16( cp16 );
    return cb_cp8_from_cp32( cp32 );
}
CB_UTFCodePoint8 cb_cp8_from_cp32( CB_UTFCodePoint32 cp32 ) {
    CB_UTFCodePoint8 result;
    if( cp32.rune <= 0x007F ) {
        result = cb_cp8_from_bytes( (uint8_t)cp32.rune, 0, 0, 0 );
    } else if( cp32.rune <= 0x07FF ) {
        result = cb_cp8_from_bytes(
            ((cp32.rune >> 6) & 0x1F) | 0xC0, 
            ((cp32.rune     ) & 0x3F) | 0x80,
            0, 0 );
    } else if( cp32.rune <= 0xFFFF ) {
        result = cb_cp8_from_bytes(
            ((cp32.rune >> 12) & 0x0F) | 0xE0,
            ((cp32.rune >>  6) & 0x3F) | 0x80,
            ((cp32.rune      ) & 0x3F) | 0x80,
            0 );
    } else if( cp32.rune <= 0x10FFFF ) {
        result = cb_cp8_from_bytes(
            ((cp32.rune >> 18) & 0x07) | 0xF0,
            ((cp32.rune >> 12) & 0x3F) | 0x80,
            ((cp32.rune >>  6) & 0x3F) | 0x80,
            ((cp32.rune      ) & 0x3F) | 0x80 );
    } else {
        result = CB_UNICODE_CP8_REPLACEMENT_CHARACTER;
    }
    return result;
}
CB_UTFCodePoint16 cb_cp16_from_cp8( CB_UTFCodePoint8 cp8 ) {
    CB_UTFCodePoint32 cp32 = cb_cp32_from_cp8( cp8 );
    return cb_cp16_from_cp32( cp32 );
}
CB_UTFCodePoint16 cb_cp16_from_cp32( CB_UTFCodePoint32 cp32 ) {
    CB_UTFCodePoint16 result;

    if( cp32.rune <= 0xFFFF ) {
        if( cp32.rune >= 0xD800 && cp32.rune <= 0xDFFF ) {
            result = CB_UNICODE_CP16_REPLACEMENT_CHARACTER;
        } else {
            result.code_units[0] = cp32.rune;
        }
    } else if( cp32.rune <= 0x10FFFF ) {
        uint32_t value = cp32.rune - 0x10000;

        result.code_units[0] = (uint16_t)((value >>    10) + 0xD800);
        result.code_units[1] = (uint16_t)((value  & 0x3FF) + 0xDC00);
    } else {
        result = CB_UNICODE_CP16_REPLACEMENT_CHARACTER;
    }

    return result;
}
CB_UTFCodePoint32 cb_cp32_from_cp8( CB_UTFCodePoint8 cp8 ) {
    CB_UTFCodePoint32 result;

    if( cp8.code_units[0] < 0x80 ) {

        result.rune = cp8.code_units[0];
        
    } else if( (cp8.code_units[0] & 0xE0) == 0xC0 ) {


        result.rune = cp8.code_units[0] & 0x1F;
        result.rune = (result.rune << 6) | (cp8.code_units[1] & 0x3F);

    } else if( (cp8.code_units[0] & 0xF0) == 0xE0 ) {

        result.rune = cp8.code_units[0] & 0x0F;
        result.rune = (result.rune << 6) | (cp8.code_units[1] & 0x3F);
        result.rune = (result.rune << 6) | (cp8.code_units[2] & 0x3F);

    } else if( (cp8.code_units[0] & 0xF8) ) {

        result.rune = cp8.code_units[0] & 0x07;
        result.rune = (result.rune << 6) | (cp8.code_units[1] & 0x37);
        result.rune = (result.rune << 6) | (cp8.code_units[2] & 0x37);
        result.rune = (result.rune << 6) | (cp8.code_units[3] & 0x37);

    } else {

        result = CB_UNICODE_CP32_REPLACEMENT_CHARACTER;

    }
    return result;
}
CB_UTFCodePoint32 cb_cp32_from_cp16( CB_UTFCodePoint16 cp16 ) {
    CB_UTFCodePoint32 result;

    if( cp16.code_units[0] <= 0xD7FF ) {
        result.rune = cp16.code_units[0];
    } else if( cp16.code_units[0] <= 0xDBFF ) {
        uint16_t hi = ( cp16.code_units[0] - 0xD800 ) * 0x400;
        uint16_t lo = ( cp16.code_units[1] - 0xDC00 );

        result.rune = (lo | hi) + 0x10000;
    } else {
        result = CB_UNICODE_CP32_REPLACEMENT_CHARACTER;
    }

    return result;
}


bool cb_file_exists( const char* path_utf8 ) {
    CB_FileType type = cb_path_query_type( path_utf8 );
    return type == CB_FTYPE_FILE;
}
bool cb_directory_exists( const char* path_utf8 ) {
    CB_FileType type = cb_path_query_type( path_utf8 );
    return type == CB_FTYPE_DIRECTORY;
}

struct InternalCB_DirectoryCopyWalkParams {
    bool             result;
    bool             overwrite_existing;
    int              src_len;
    int              dst_len;
    CB_StringBuilder src_builder;
    CB_StringBuilder dst_builder;
};

CB_DirectoryWalkControl
_cb_internal_directory_copy_walk( const CB_DirectoryWalkInfo* info, void* in_params ) {
    struct InternalCB_DirectoryCopyWalkParams* params =
        (struct InternalCB_DirectoryCopyWalkParams*)in_params;

    if( params->src_len < 0 ) {
        params->src_len = info->path_name_offset - 1;
        CB_APPEND( &params->src_builder, params->src_len, info->path );
    }

    CB_PUSH( &params->src_builder, '/' );
    CB_APPEND(
        &params->src_builder,
        info->path_len - params->src_len,
        info->path + params->src_len );
    CB_PUSH( &params->src_builder, 0 );

    CB_PUSH( &params->dst_builder, '/' );
    CB_APPEND(
        &params->dst_builder,
        info->path_len - params->src_len,
        info->path + params->src_len );
    CB_PUSH( &params->dst_builder, 0 );

    switch( info->type ) {
        case CB_FTYPE_NULL:
            break;
        case CB_FTYPE_FILE: {
            CB_INFO( "copying '%s' to '%s'", params->src_builder.buf, params->dst_builder.buf );
            cb_file_copy( params->dst_builder.buf, params->src_builder.buf, false );
        } break;
        case CB_FTYPE_DIRECTORY: {
            CB_INFO( "creating directory '%s'", params->dst_builder.buf );
            cb_directory_create( params->dst_builder.buf );
        } break;
    }
    params->dst_builder.len = params->dst_len;
    params->src_builder.len = params->src_len;

    return CB_DWC_CONTINUE;
}

bool cb_directory_copy(
    const char* dst, const char* src,
    bool overwrite_existing_names,
    bool fail_if_dst_exists
) {
    CB_FileType type = cb_path_query_type( dst );
    switch( type ) {
        case CB_FTYPE_NULL:
            break;
        case CB_FTYPE_FILE: {
            CB_ERROR( 
                "cb_directory_copy(): destination '%s' points to a file, not a directory!" );
        } return false;
        case CB_FTYPE_DIRECTORY: {
            if( fail_if_dst_exists ) {
                CB_ERROR( "cb_directory_copy(): destination '%s' already exists!" );
                return false;
            }
        } break;
    }

    int dst_len = strlen( dst );
    if( dst[dst_len - 1] == '/' ) {
        dst_len--;
    }

    struct InternalCB_DirectoryCopyWalkParams params = {
        .result             = true,
        .overwrite_existing = overwrite_existing_names,
        .src_len            = -1,
        .dst_len            = strlen(dst),
        .src_builder        = {},
        .dst_builder        = {},
    };

    CB_APPEND( &params.dst_builder, params.dst_len, dst );

    if( !cb_directory_walk( src, _cb_internal_directory_copy_walk, &params ) ) {
        CB_FREE( params.src_builder.buf, params.src_builder.cap );
        CB_FREE( params.dst_builder.buf, params.dst_builder.cap );
        return false;
    }
    CB_FREE( params.src_builder.buf, params.src_builder.cap );
    CB_FREE( params.dst_builder.buf, params.dst_builder.cap );

    return params.result;
}
bool cb_directory_move(
    const char* dst, const char* src,
    bool overwrite_existing_names,
    bool fail_if_dst_exists
) {
    if( !cb_directory_copy(
        dst, src,
        overwrite_existing_names,
        fail_if_dst_exists
    ) ) {
        return false;
    }
    return cb_directory_remove( src, true );
}

bool cb_read_entire_file(
    const char* path_utf8, uintptr_t* out_buffer_size, void** out_buffer
) {
    CB_File file;
    if( !cb_file_open( path_utf8, CB_FOPEN_READ, &file ) ) {
        return false;
    }
    uintptr_t size = cb_file_seek( &file, 0, CB_FSEEK_END );
    cb_file_seek( &file, 0, CB_FSEEK_SET );

    void* buffer = CB_ALLOC( NULL, 0, size );
    if( !cb_file_read( &file, size, buffer, NULL ) ) {
        cb_file_close( &file );
        CB_FREE( buffer, size );
        return false;
    }

    cb_file_close( &file );

    *out_buffer_size = size;
    *out_buffer      = buffer;
    return true;
}

int cb_which_file_is_newer( const char* file_a, const char* file_b ) {
    CB_FileInfo file_info_a, file_info_b; 

    if( !cb_path_query_info( file_a, &file_info_a ) ) {
        return -1;
    }
    if( !cb_path_query_info( file_b, &file_info_b ) ) {
        return -2;
    }

    return difftime( file_info_a.time.create, file_info_b.time.create ) < 0.0;
}

int cb_which_file_is_newer_many_array(
    const char* file_a, int file_b_count, const char** file_b
) {
    CB_FileInfo file_info_a, file_info_b; 

    if( !cb_path_query_info( file_a, &file_info_a ) ) {
        CB_ERROR( "cb_which_file_is_newer_many_array(): failed to stat file_a!" );
        return -1;
    }

    for( int i = 0; i < file_b_count; ++i ) {
        if( !cb_path_query_info( file_b[i], &file_info_b ) ) {
            CB_ERROR( "cb_which_file_is_newer_many_array(): failed to stat file_b[%i]!", i );
            return -2;
        }

        bool b_is_newer = difftime( file_info_a.time.create, file_info_b.time.create ) < 0.0;

        if( b_is_newer ) {
            return 1;
        }
    }

    return 0;
}

void cb_command_flatten( CB_Command command, CB_StringBuilder* string ) {
    for( int i = 0; i < command.len; ++i ) {
        if( !command.buf[i] ) {
            break;
        }

        CB_StringSlice argument = CB_STRING_FROM_CSTR( command.buf[i] );
        argument = cb_string_trim_surrounding_whitespace( argument );

        if( !argument.len ) {
            continue;
        }

        if( argument.buf[0] == '"' && argument.buf[argument.len - 1] == '"' ) {
            argument = cb_string_trim( CB_ADVANCE( CB_StringSlice, &argument, 1 ), 1 );
        }

        if( !argument.len ) {
            continue;
        }

        CB_APPEND( string, argument.len, argument.cbuf );
        if( (i + 1) != command.len ) {
            CB_PUSH( string, ' ' );
        }
    }
}
void cb_command_builder_from_cmd( CB_CommandBuilder* builder, CB_Command cmd ) {
    for( int i = 0; i < cmd.len; ++i ) {
        int   len    = strlen( cmd.buf[i] );
        char* buffer = CB_ALLOC( NULL, 0, len + 1 );

        CB_PUSH( builder, memcpy( buffer, cmd.buf[i], len ) );
    }
}
void cb_command_builder_remove( CB_CommandBuilder* builder, int index ) {
    char* buffer = (char*)(builder->buf[index]);
    memmove( builder->buf + index, builder->buf + index + 1, builder->len - (index + 1) );
    builder->len--;

    if( buffer ) {
        CB_FREE( buffer, strlen( buffer ) + 1 );
    }
}
void cb_command_builder_free( CB_CommandBuilder* builder ) {
    if( !builder ) {
        return;
    }

    for( int i = 0; i < builder->len; ++i ) {
        char* buffer = (char*)(builder->buf[i]);
        if( buffer ) {
            CB_FREE( buffer, strlen( buffer ) + 1 );
        }
    }
    CB_FREE( builder->buf, sizeof(const char*) * builder->len );
    memset( builder, 0, sizeof(*builder) );
}
void cb_command_builder_add_null_terminator( CB_CommandBuilder* builder ) {
    CB_PUSH( builder, NULL );
}
void cb_command_builder_remove_null_terminator( CB_CommandBuilder* builder ) {
    if( builder->len && !builder->buf[builder->len - 1] ) {
        builder->len--;
    }
}

void cb_environment_builder_new(
    CB_EnvironmentBuilder* out_environment, int* opt_capacity
) {
    int cap = 5;
    if( opt_capacity ) {
        cap = *opt_capacity;
    }

    const char** names  = CB_ALLOC( NULL, 0, sizeof(const char*) * cap );
    const char** values = CB_ALLOC( NULL, 0, sizeof(const char*) * cap );

    out_environment->cap   = cap;
    out_environment->len   = 0;
    out_environment->name  = names;
    out_environment->value = values;
}
void cb_environment_builder_free(
    CB_EnvironmentBuilder* environment
) {
    for( int i = 0; i < environment->len; ++i ) {
        char* name  = (char*)(environment->name[i]);
        char* value = (char*)(environment->value[i]);

        if( name ) {
            CB_FREE( name, strlen(name) + 1 );
        }
        if( value ) {
            CB_FREE( value, strlen(value) + 1 );
        }
    }

    CB_FREE( environment->name , sizeof(const char*) * environment->cap );
    CB_FREE( environment->value, sizeof(const char*) * environment->cap  );
}
void cb_environment_builder_append(
    CB_EnvironmentBuilder* environment, const char* name, const char* value
) {
    if( (environment->len + 1) >= environment->cap ) {
        int new_cap = environment->cap + 5;

        environment->name = CB_ALLOC(
            environment->name,
            sizeof(const char*) * environment->cap,
            sizeof(const char*) * new_cap );

        environment->value = CB_ALLOC(
            environment->value,
            sizeof(const char*) * environment->cap,
            sizeof(const char*) * new_cap );

        environment->cap = new_cap;
    }

    int name_len  = strlen( name );
    int value_len = strlen( value );
    environment->name[environment->len]  =
        memcpy( CB_ALLOC( NULL, 0, name_len + 1 ), name, name_len );
    environment->value[environment->len] =
        memcpy( CB_ALLOC( NULL, 0, value_len + 1 ), value, value_len );

    environment->len++;
}
bool cb_environment_builder_remove(
    CB_EnvironmentBuilder* environment, const char* name
) {
    for( int i = 0; i < environment->len; ++i ) {
        const char* current = environment->name[i];
        if( strcmp( current, name ) == 0 ) {
            cb_environment_builder_remove_by_index( environment, i );
            return true;
        }
    }
    return false;
}
void cb_environment_builder_remove_by_index(
    CB_EnvironmentBuilder* environment, int index
) {
    char* name_buffer  = (char*)(environment->name[index]);
    char* value_buffer = (char*)(environment->value[index]);

    if( name_buffer ) {
        CB_FREE( name_buffer, strlen(name_buffer) + 1 );
    }
    if( value_buffer ) {
        CB_FREE( value_buffer, strlen(value_buffer) + 1 );
    }

    memmove(
        environment->name + index,
        environment->name + index + 1,
        sizeof(const char*) * (environment->len - (index + 1)) );
    memmove(
        environment->value + index,
        environment->value + index + 1,
        sizeof(const char*) * (environment->len - (index + 1)) );

    environment->len--;
}
bool cb_environment_builder_replace(
    CB_EnvironmentBuilder* environment, const char* name, const char* new_value
) {
    for( int i = 0; i < environment->len; ++i ) {
        const char* current = environment->name[i];
        if( strcmp( current, name ) == 0 ) {
            cb_environment_builder_replace_by_index( environment, i, new_value );
            return true;
        }
    }
    return false;
}
void cb_environment_builder_replace_by_index(
    CB_EnvironmentBuilder* environment, int index, const char* new_value
) {
    char* buffer = (char*)(environment->value[index]);
    CB_FREE( buffer, strlen(buffer) + 1 );

    int new_value_len = strlen( new_value );
    buffer = CB_ALLOC( NULL, 0, new_value_len + 1 );

    environment->value[index] = memcpy( buffer, new_value, new_value_len );
}

bool cb_process_exec(
    CB_Command             cmd,
    int*                   opt_out_exit_code,
    const char*            opt_working_directory,
    CB_EnvironmentBuilder* opt_environment,
    CB_PipeRead*           opt_stdin,
    CB_PipeWrite*          opt_stdout,
    CB_PipeWrite*          opt_stderr
) {
    CB_ProcessID pid;
    bool success = cb_process_exec_async(
        cmd, &pid, opt_working_directory,
        opt_environment, opt_stdin, opt_stdout, opt_stderr );

    if( !success ) {
        return false;
    }

    int exit_code = cb_process_wait( &pid );

    if( opt_out_exit_code ) {
        *opt_out_exit_code = exit_code;
    }

    return true;
}

void cb_process_wait_many( int count, CB_ProcessID* pids, int* opt_out_exit_codes ) {
    for( int i = 0; i < count; ++i ) {
        int exit_code = cb_process_wait( pids + i );
        if( opt_out_exit_codes ) {
            opt_out_exit_codes[i] = exit_code;
        }
    }
}

void cb_log_level_set( CB_LogLevel level ) {
    global_state.level = level;
}
CB_LogLevel cb_log_level_query(void) {
    return global_state.level;
}
bool cb_log_level_is_valid( CB_LogLevel level ) {
    return (level == CB_LOG_FATAL) || (level >= global_state.level);
}
void cb_write_log_va( CB_LogLevel level, const char* fmt, va_list va ) {
    if( !cb_log_level_is_valid( level ) ) {
        return;
    }

    const char* prefix = "";
    const char* color  = "";
    FILE* output_file  = stdout;
    switch( level ) {
        case CB_LOG_INFO: {
            prefix = "INF";
        } break;
        case CB_LOG_WARN: {
            prefix = "WRN";
            color  = CB_COLOR_YELLOW;
        } break;
        case CB_LOG_ERROR: {
            prefix      = "ERR";
            output_file = stderr;
            color       = CB_COLOR_RED;
        } break;
        case CB_LOG_ATTENTION: {
            prefix = "!!!";
            color  = CB_COLOR_CYAN;
        } break;
        case CB_LOG_FATAL: {
            prefix      = "FTL";
            output_file = stderr;
            color       = CB_COLOR_MAGENTA;
        } break;
    }

    fprintf( output_file, "%s[CBUILD %s] ", color, prefix );
    vfprintf( output_file, fmt, va );
    fprintf( output_file, CB_COLOR_RESET "\n" );
}
void cb_write_log( CB_LogLevel level, const char* fmt, ... ) {
    va_list va;
    va_start( va, fmt );
    cb_write_log_va( level, fmt, va );
    va_end( va );
}

int _cb_internal_process_exec_quick(
    const char*            opt_working_directory,
    CB_EnvironmentBuilder* opt_environment,
    CB_PipeRead*           opt_stdin,
    CB_PipeWrite*          opt_stdout,
    CB_PipeWrite*          opt_stderr,
    ...
) {
    CB_CommandBuilder builder = {};
    va_list va;
    va_start( va, opt_stderr );

    for( ;; ) {
        const char* arg = va_arg( va, const char* );
        if( !arg ) {
            break;
        }
        cb_command_builder_append( &builder, arg );
    }

    va_end( va );

    if( !builder.len ) {
        CB_ERROR( "cb_process_exec_quick: no command or arguments provided!" );
        return -2;
    }

    cb_command_builder_add_null_terminator( &builder );

    int exit_code = 0;
    bool success  = cb_process_exec(
        builder.cmd, &exit_code, opt_working_directory,
        opt_environment, opt_stdin, opt_stdout, opt_stderr );

    cb_command_builder_free( &builder );

    if( !success ) {
        return -2;
    }

    return exit_code;
}

void _cb_internal_command_builder_new( CB_CommandBuilder* builder, ... ) {
    va_list va;
    va_start( va, builder );
    for( ;; ) {
        const char* argument = va_arg( va, const char* );
        if( !argument ) {
            break;
        }

        int len = strlen( argument );
        if( !len ) {
            continue;
        }

        char* buffer = CB_ALLOC( NULL, 0, len + 1 );

        CB_PUSH( builder, memcpy( buffer, argument, len ) );
    }
    va_end( va );
}
void _cb_internal_command_builder_append( CB_CommandBuilder* builder, ... ) {
    va_list va;
    va_start( va, builder );
    for( ;; ) {
        const char* argument = va_arg( va, const char* );
        if( !argument ) {
            break;
        }

        int len = strlen( argument );
        if( !len ) {
            continue;
        }

        char* buffer = CB_ALLOC( NULL, 0, len + 1 );

        CB_PUSH( builder, memcpy( buffer, argument, len ) );
    }
    va_end( va );
}
bool _cb_internal_make_directories( const char* first, ... ) {
    va_list va;
    va_start( va, first );
    const char* path = first;

    bool result = true;

    for( ;; ) {
        if( !path ) {
            break;
        }

        if( cb_directory_exists( path ) ) {
            path = va_arg( va, const char* );
            continue;
        }

        if( !cb_directory_create( path ) ) {
            result = false;
            break;
        }
        CB_INFO( "mkdir %s", path );

        path = va_arg( va, const char* );
    }

    va_end( va );
    return result;
}

void cb_file_write_fmt( CB_File* file, const char* fmt, ... ) {
    va_list va;
    va_start( va, fmt );
    cb_file_write_fmt_va( file, fmt, va );
    va_end( va );
}


// NOTE(alicia): POSIX Implementation

#if CB_PLATFORM_IS_POSIX

#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ftw.h>

struct CB_FTWState {
    CB_DirectoryWalkFN* callback;
    void*               callback_params;
    int                 skip_level;
};

struct CB_PosixState {
    struct CB_FTWState ftw_state;
    CB_StringBuilder   cwd;
    bool               cwd_obtained;
};

struct CB_PosixState* _cb_internal_platform_get_state(void) {
    if( !global_state.platform_state ) {
        global_state.platform_state = CB_ALLOC( NULL, 0, sizeof(struct CB_PosixState) );
    }
    return (struct CB_PosixState*)global_state.platform_state;
}
#define STATE _cb_internal_platform_get_state()

static
double _cb_internal_ts_to_ms( struct timespec* ts ) {
    double result = 0.0;
    result += (double)ts->tv_nsec / 1000000.0;
    result += (double)ts->tv_sec * 1000.0;
    return result;
}
static
CB_FileType _cb_internal_file_type_from_stat( const struct stat* st ) {
    if( S_ISREG( st->st_mode ) ) {
        return CB_FTYPE_FILE;
    } else if( S_ISDIR( st->st_mode ) ) {
        return CB_FTYPE_DIRECTORY;
    } else {
        return CB_FTYPE_NULL;
    }
}

CB_Time cb_time_query(void) {
    return time( NULL );
}
double cb_time_msec(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC_RAW, &ts );

    return _cb_internal_ts_to_ms( &ts );
}
double cb_time_sec(void) {
    return cb_time_msec() / 1000.0;
}

CB_FileType cb_path_query_type( const char* path_utf8 ) {
    struct stat st;
    if( stat( path_utf8, &st ) ) {
        return CB_FTYPE_NULL;
    }

    return _cb_internal_file_type_from_stat( &st );
}
bool cb_path_query_time_modify( const char* path_utf8, CB_Time* out_time ) {
    struct stat st;
    if( stat( path_utf8, &st ) ) {
        int errnum = errno;
        CB_ERROR(
            "POSIX: cb_path_query_time_modify(): failed to stat '%s'! reason: %s",
            path_utf8, strerror(errnum) );
        return false;
    }
    *out_time = st.st_mtime;
    return true;
}
bool cb_path_query_time_create( const char* path_utf8, CB_Time* out_time ) {
    struct stat st;
    if( stat( path_utf8, &st ) ) {
        int errnum = errno;
        CB_ERROR(
            "POSIX: cb_path_query_time_create(): failed to stat '%s'! reason: %s",
            path_utf8, strerror(errnum) );
        return false;
    }
    *out_time = st.st_ctime;
    return true;
}
bool cb_path_query_info( const char* path_utf8, CB_FileInfo* out_info ) {
    struct stat st;
    if( stat( path_utf8, &st ) != 0 ) {
        int errnum = errno;
        CB_ERROR(
            "POSIX: cb_path_query_info(): failed to stat '%s'! reason: %s",
            path_utf8, strerror(errnum) );
        return false;
    }

    out_info->size        = st.st_size;
    out_info->time.create = st.st_ctime;
    out_info->time.modify = st.st_mtime;
    out_info->type        = _cb_internal_file_type_from_stat( &st );

    return true;
}
char* cb_path_canonicalize( const char* path_utf8 ) {
    char* result = realpath( path_utf8, NULL );
    return result;
}

bool cb_file_open( const char* path_utf8, CB_FileOpenFlags flags, CB_File* out_file ) {
    int    oflag = 0;
    mode_t mode  = S_IRWXU;
    if(
        (( flags & ( CB_FOPEN_READ | CB_FOPEN_WRITE )) ==
        ( CB_FOPEN_READ | CB_FOPEN_WRITE ) )
    ) {
        oflag = O_RDWR;
    } else if( (( flags & ( CB_FOPEN_READ )) == ( CB_FOPEN_READ ) ) ) {
        oflag = O_RDONLY;
    } else if( (( flags & ( CB_FOPEN_WRITE )) == ( CB_FOPEN_WRITE ) ) ) {
        oflag = O_WRONLY;
    }

    if( (( flags & ( CB_FOPEN_APPEND )) == ( CB_FOPEN_APPEND ) ) ) {
        oflag |= O_APPEND;
    }

    if( (( flags & ( CB_FOPEN_CREATE )) == ( CB_FOPEN_CREATE ) ) ) {
        oflag |= O_CREAT | O_EXCL;
    }
    if( (( flags & ( CB_FOPEN_TRUNCATE )) == ( CB_FOPEN_TRUNCATE ) ) ) {
        oflag |= O_TRUNC;
    }

    int fd     = open( path_utf8, oflag, mode );
    int errnum = errno;

    if( fd < 0 ) {
        CB_ERROR(
            "POSIX: cb_file_open(): "
            "failed to open '%s'! reason: %s", path_utf8, strerror(errnum));
        return false;
    }
    out_file->_internal_handle = fd;
    return true;
}
void cb_file_close( CB_File* file ) {
    close( file->_internal_handle );
    file->_internal_handle = 0;
}
intptr_t cb_file_seek( CB_File* file, intptr_t offset, CB_FileSeek type ) {
    int whence = SEEK_CUR;
    switch( type ) {
        case CB_FSEEK_CUR: {
            whence = SEEK_CUR;
        } break;
        case CB_FSEEK_SET: {
            whence = SEEK_SET;
        } break;
        case CB_FSEEK_END: {
            whence = SEEK_END;
        } break;
    }
    return lseek64( file->_internal_handle, offset, whence );
}
void cb_file_truncate( CB_File* file ) {
    ftruncate64(
        file->_internal_handle,
        cb_file_seek( file, 0, CB_FSEEK_CUR ) );
}
bool cb_file_read(
    CB_File* file, uintptr_t buffer_size, void* buffer, uintptr_t* opt_out_read
) {
    intptr_t result = read( file->_internal_handle, buffer, buffer_size );
    if( result < 0 ) {
        CB_ERROR(
            "POSIX: failed to read %zu! reason: %s",
            buffer_size, strerror(errno) );
        return false;
    }
    if( opt_out_read ) {
        *opt_out_read = (uintptr_t)result;
    }
    return true;
}
bool cb_file_write(
    CB_File* file, uintptr_t buffer_size, const void* buffer, uintptr_t* opt_out_write
) {
    intptr_t result = write( file->_internal_handle, buffer, buffer_size );
    if( result < 0 ) {
        CB_ERROR(
            "POSIX: failed to write %zu! reason: %s",
            buffer_size, strerror(errno) );
        return false;
    }
    if( opt_out_write ) {
        *opt_out_write = (uintptr_t)result;
    }
    return true;
}
void cb_file_write_fmt_va( CB_File* file, const char* fmt, va_list va ) {
    vdprintf( file->_internal_handle, fmt, va );
}
bool cb_file_remove( const char* path_utf8 ) {
    int result = unlink( path_utf8 );
    int errnum = errno;
    if( result ) {
        CB_ERROR( 
            "POSIX: cb_file_remove(): failed to remove '%s'! reason: %s",
            path_utf8, strerror(errnum) );
        return false;
    }
    return true;
}
bool cb_file_copy( const char* dst, const char* src, bool fail_if_dst_exists ) {
    CB_File fdst = {}, fsrc = {};

    CB_FileOpenFlags dst_flags = CB_FOPEN_WRITE;
    CB_FileType ft = cb_path_query_type( dst );

    switch( ft ) {
        case CB_FTYPE_NULL: {
            dst_flags |= CB_FOPEN_CREATE;
        } break;
        case CB_FTYPE_FILE: {
            if( fail_if_dst_exists ) {
                CB_ERROR(
                    "cb_file_copy: '%s' already exists and "
                    "fail_if_dst_exists is true!", dst );
                return false;
            }
            dst_flags |= CB_FOPEN_TRUNCATE;
        } break;
        case CB_FTYPE_DIRECTORY: {
            CB_ERROR( "cb_file_copy: '%s' already exists and it's a directory!", dst );
            return false;
        } break;
    }

    if( !cb_file_open( src, CB_FOPEN_READ, &fsrc ) ) {
        return false;
    }
    if( !cb_file_open( dst, dst_flags, &fdst ) ) {
        cb_file_close( &fsrc ); 
        return false;
    }

    uint8_t bytes[CB_KIBIBYTES(1)];
    uintptr_t src_size = cb_file_seek( &fsrc, 0, CB_FSEEK_END );
    cb_file_seek( &fsrc, 0, CB_FSEEK_SET );

    while( src_size ) {
        uintptr_t max_copy = src_size;
        if( max_copy > sizeof(bytes) ) {
            max_copy = sizeof(bytes);
        }

        cb_file_read( &fsrc, max_copy, bytes, NULL );
        cb_file_write( &fdst, max_copy, bytes, NULL );

        src_size -= max_copy;
    }

    cb_file_close( &fsrc );
    cb_file_close( &fdst );
    return true;
}
bool cb_file_move( const char* dst, const char* src, bool fail_if_dst_exists ) {
    if( !cb_file_copy( dst, src, fail_if_dst_exists ) ) {
        return false;
    }
    return cb_file_remove( src );
}

bool cb_directory_create( const char* path_utf8 ) {
    if( mkdir( path_utf8, S_IRWXU ) ) {
        int errnum = errno;
        CB_ERROR(
            "POSIX: cb_directory_create(): failed to create '%s'! reason: %s",
            path_utf8, strerror(errnum) );
        return false;
    }
    return true;
}
bool _cb_internal_directory_remove_recursive( const char* path_utf8 ) {
    // TODO(alicia): 
    CB_UNIMPLEMENTED( "POSIX directory remove recursive!" );
    CB_UNUSED( path_utf8 );
    return false;
}
bool cb_directory_remove( const char* path_utf8, bool recursive ) {
    if( recursive ) {
        return _cb_internal_directory_remove_recursive( path_utf8 );
    } else {
        if( rmdir( path_utf8 ) ) {
            int errnum = errno;
            CB_ERROR(
                "POSIX: cb_directory_remove(): "
                "failed to remove directory '%s'! reason: %s",
                path_utf8, strerror(errnum));
            return false;
        }
        return true;
    }
}
int _cb_internal_nftw(
    const char* filename, const struct stat* st, int flag, struct FTW* _info
) {
    CB_UNUSED(flag);
    struct CB_FTWState* state = &STATE->ftw_state;

    if( state->skip_level >= 0 ) {
        if( _info->level == state->skip_level ) {
            return 0;
        } else {
            state->skip_level = -1;
        }
    }

    CB_DirectoryWalkInfo info = {};
    info.type             = _cb_internal_file_type_from_stat( st );
    info.file_size        = st->st_size;
    info.path             = filename;
    info.path_len         = strlen( filename );
    info.path_name_offset = _info->base;
    info.level            = _info->level;

    switch( state->callback( &info, state->callback_params ) ) {
        case CB_DWC_CONTINUE: return 0;
        case CB_DWC_STOP:     return 1;
        case CB_DWC_SKIP: {
            state->skip_level = _info->level + 1;
        } return 0;
    }

    return 1;
}
bool cb_directory_walk( const char* path_utf8, CB_DirectoryWalkFN* callback, void* params ) {
    struct CB_FTWState* state = memset( &STATE->ftw_state, 0, sizeof(*state) );

    state->callback        = callback;
    state->callback_params = params;
    state->skip_level      = -1;

    if( nftw( path_utf8, _cb_internal_nftw, 5, 0 ) < 0 ) {
        CB_ERROR(
            "POSIX: cb_directory_walk(): failed to walk '%s'! reason: %s",
            path_utf8, strerror(errno) );
        return false;
    }
    return true;
}

bool cb_working_directory_set( const char* new_cwd ) {
    if( chdir( new_cwd ) ) {
        int errnum = errno;
        CB_ERROR(
            "POSIX: cb_working_directory_set(): failed to change to '%s'! reason: %s",
            new_cwd, strerror(errnum) );
        return false;
    }
    STATE->cwd_obtained = false;
    return true;
}
const char* cb_working_directory_query(void) {
    CB_StringBuilder* builder = &STATE->cwd;
    if( STATE->cwd_obtained ) {
        return builder->buf;
    }

    #define GROW_RATE 128

    for( ;; ) {
        if( getcwd( builder->buf, builder->cap ) ) {
            break;
        }
        builder->buf  = CB_ALLOC( builder->buf, builder->cap, builder->cap + 128 );
        builder->cap += 128;
    }

    STATE->cwd_obtained = true;

    #undef GROW_RATE
    return builder->buf;
}

bool cb_pipe_open( CB_PipeRead* out_read, CB_PipeWrite* out_write ) {
    int fd[2];
    if( pipe( fd ) ) {
        int errnum = errno;
        CB_ERROR(
            "POSIX: cb_pipe_open(): failed to open pipes! reason: %s", strerror(errnum) );
        return false;
    }

    out_read->_internal_handle  = fd[0];
    out_write->_internal_handle = fd[1];
    return true;
}
void cb_pipe_close( CB_Pipe* pipe ) {
    close( pipe->_internal_handle );
    pipe->_internal_handle = -1;
}
CB_PipeRead cb_pipe_stdin(void) {
    CB_PipeRead result;
    result._internal_handle = STDIN_FILENO;
    return result;
}
CB_PipeWrite cb_pipe_stdout(void) {
    CB_PipeWrite result;
    result._internal_handle = STDOUT_FILENO;
    return result;
}
CB_PipeWrite cb_pipe_stderr(void) {
    CB_PipeWrite result;
    result._internal_handle = STDERR_FILENO;
    return result;
}

const char* cb_environment_query( const char* name ) {
    const char* value = getenv( name );
    if( !value ) {
        return NULL;
    }
    int len = strlen( value );
    char* result = CB_ALLOC( NULL, 0, len + 1 );
    return memcpy( result, value, len );
}
bool cb_environment_set( const char* name, const char* new_value ) {
    if( setenv( name, new_value, true ) ) {
        int errnum = errno;
        CB_ERROR(
            "POSIX: cb_environment_set(): "
            "failed to set variable '%s'! reason: %s", name, strerror(errnum) );
        return false;
    }
    return true;
}

bool cb_process_exec_async(
    CB_Command             cmd,
    CB_ProcessID*          out_pid,
    const char*            opt_working_directory,
    CB_EnvironmentBuilder* opt_environment,
    CB_PipeRead*           opt_stdin,
    CB_PipeWrite*          opt_stdout,
    CB_PipeWrite*          opt_stderr
) {
    int pipe_stdin  = opt_stdin  ? opt_stdin->_internal_handle  : STDIN_FILENO;
    int pipe_stdout = opt_stdout ? opt_stdout->_internal_handle : STDOUT_FILENO;
    int pipe_stderr = opt_stderr ? opt_stderr->_internal_handle : STDERR_FILENO;

    pid_t pid = fork();
    if( pid < 0 ) {
        CB_ERROR( "POSIX: cb_process_exec_async(): failed to fork!" );
        return false;
    }

    if( pid ) { // thread that ran process_exec
        out_pid->_internal_handle = pid;
        return true;
    }

    // thread where process will run

    if( opt_working_directory ) {
        CB_INFO( "chdir: '%s'", opt_working_directory );
        chdir( opt_working_directory );
    }
    if( dup2( pipe_stdin,  STDIN_FILENO ) < 0 ) {
        CB_ERROR( "POSIX: failed to duplicate stdin!" );
    }
    if( dup2( pipe_stdout, STDOUT_FILENO ) < 0 ) {
        CB_ERROR( "POSIX: failed to duplicate stdout!" );
    }
    if( dup2( pipe_stderr, STDERR_FILENO ) < 0 ) {
        CB_ERROR( "POSIX: failed to duplicate stderr!" );
    }

    CB_StringBuilder builder = {};
    if( opt_environment ) {
        for( int i = 0; i < opt_environment->len; ++i ) {
            setenv( opt_environment->name[i], opt_environment->value[i], true );
            int name_len  = strlen( opt_environment->name[i] );
            int value_len = strlen( opt_environment->value[i] );

            CB_APPEND( &builder, name_len, opt_environment->name[i] );
            CB_PUSH( &builder, '=' );
            CB_APPEND( &builder, value_len, opt_environment->value[i] );
            CB_PUSH( &builder, ' ' );
        }
    }

    cb_command_flatten( cmd, &builder );
    CB_PUSH( &builder, 0 );
    CB_INFO( "  > %s", builder.buf );
    CB_FREE( builder.buf, builder.cap );

    // NOTE(alicia): execvp should only return if command failed to execute.
    execvp( cmd.buf[0], (char* const*)(cmd.buf) );

    CB_FATAL( "POSIX: failed to execute command!" );
    abort();
}

void cb_process_discard( CB_ProcessID* pid ) {
    CB_UNUSED(pid);
}
int cb_process_wait( CB_ProcessID* pid ) {
    int wstatus = 0;
    pid_t value = waitpid( pid->_internal_handle, &wstatus, 0 );
    if( value < 0 ) {
        CB_ERROR(
            "POSIX: cb_process_wait(): "
            "failed to wait for pid! reason: %s", strerror(errno) );
        return -2;
    }
    if( WIFEXITED( wstatus ) ) {
        return WEXITSTATUS( wstatus );
    } else {
        return -1;
    }
}
bool cb_process_wait_timed( CB_ProcessID* pid, uint32_t msec, int* opt_out_exit_code ) {
    if( msec == CB_WAIT_INFINITE ) {
        int res = cb_process_wait( pid );
        if( res < 0 ) {
            return false;
        }
        if( opt_out_exit_code ) {
            *opt_out_exit_code = res;
        }
        return true;
    }

    for( uint32_t i = 0; i < msec; ++i ) {
        int wstatus = 0;
        pid_t value = waitpid( pid->_internal_handle, &wstatus, WNOHANG );
        if( value == 0 ) {
            sleep(1);
            continue;
        }

        if( value < 0 ) {
            CB_ERROR(
                "POSIX: cb_process_wait_timed(): "
                "failed to wait for pid! reason: %s", strerror(errno) );
            return false;
        }

        if( opt_out_exit_code ) {
            if( WIFEXITED( wstatus ) ) {
                *opt_out_exit_code = WEXITSTATUS( wstatus );
            } else {
                *opt_out_exit_code = -1;
            }
        }

        return true;
    }

    return false;
}
void cb_process_kill( CB_ProcessID* pid ) {
    if( kill( pid->_internal_handle, SIGKILL ) ) {
        CB_ERROR(
            "POSIX: cb_process_kill(): failed to kill pid! reason: %s",
            strerror(errno) );
    }
}

bool cb_process_is_in_path( const char* process_name ) {
    char mini_buf[255 + sizeof("which -s %s")] = {};
    snprintf( mini_buf, sizeof(mini_buf), "which -s %s", process_name );
    return system( mini_buf ) == 0;
}

#undef STATE

#endif /* POSIX Implementation */

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS

#include <windows.h>

#define UTF16_SIZE CB_KIBIBYTES(4)
#define UTF8_SIZE  CB_KIBIBYTES(4)

enum CB_WindowsDirectoryWalkResult {
    CB_WINDOWS_DIRECTORY_WALK_OK,
    CB_WINDOWS_DIRECTORY_WALK_STOP,
    CB_WINDOWS_DIRECTORY_WALK_ERROR,
};

struct CB_WindowsStringBuilderUTF16 {
    int       cap, len;
    uint16_t* buf;
};

struct CB_WindowsState {
    uint16_t buf_utf16[UTF16_SIZE];
    uint8_t  buf_utf8[UTF8_SIZE];

    bool     cwd_obtained;
    uint8_t  cwd[UTF8_SIZE];
};
static
struct CB_WindowsState* _cb_internal_platform_get_state(void) {
    if( !global_state.platform_state ) {
        global_state.platform_state = CB_ALLOC( NULL, 0, sizeof(struct CB_WindowsState) );
    }
    return (struct CB_WindowsState*)global_state.platform_state;
}
#define STATE _cb_internal_platform_get_state()

static
uint16_t* cb_windows_utf16_buf(void) {
    return (uint16_t*)memset( STATE->buf_utf16, 0, sizeof(uint16_t) * UTF16_SIZE );
}
static
uint8_t* cb_windows_utf8_buf(void) {
    return (uint8_t*)memset( STATE->buf_utf8, 0, UTF8_SIZE );
}
static
int cb_windows_cvt_utf8_from_utf16(
    int utf8_cap, uint8_t* utf8_buf,
    int utf16_len, const uint16_t* utf16_buf 
) {
    if( !utf8_cap ) {
        return 0;
    }

    int utf8_len = 0;

    while( utf16_len ) {
        CB_UTFCodePoint16 cp16 = {};
        int adv = cb_cp16_from_string( utf16_len, utf16_buf, &cp16 );
        utf16_len -= adv;
        utf16_buf += adv;

        CB_UTFCodePoint8 cp8 = {};
        cp8 = cb_cp8_from_cp16( cp16 );
        int code_unit_count = cb_cp8_code_unit_count( cp8 );

        int remaining  = (utf8_cap - utf8_len);
        int max_append = code_unit_count;
        if( max_append > remaining ) {
            max_append = remaining;
        }

        memcpy( utf8_buf + utf8_len, cp8.bytes, max_append );
        utf8_len += max_append;
    }
    if( utf8_len == utf8_cap ) {
        utf8_len--;
        utf8_buf[utf8_len] = 0;
    } else {
        utf8_buf[utf8_len] = 0;
    }

    return utf8_len;
}
static
int cb_windows_cvt_utf16_from_utf8(
    int utf16_cap, uint16_t* utf16_buf,
    int utf8_len, const uint8_t* utf8_buf
) {
    if( !utf16_cap ) {
        return 0;
    }

    int utf16_len = 0;

    while( utf8_len ) {
        CB_UTFCodePoint8 cp8 = {};
        int adv = cb_cp8_from_string( utf8_len, utf8_buf, &cp8 );
        utf8_len -= adv;
        utf8_buf += adv;

        CB_UTFCodePoint16 cp16 = {};
        cp16 = cb_cp16_from_cp8( cp8 );
        int code_unit_count = cb_cp16_code_unit_count( cp16 );

        int remaining  = (utf16_cap - utf16_len);
        int max_append = code_unit_count;
        if( max_append > remaining ) {
            max_append = remaining;
        }

        memcpy( utf16_buf + utf16_len, cp16.bytes, sizeof(uint16_t) * max_append );
        utf16_len += max_append;
    }
    if( utf16_len == utf16_cap ) {
        utf16_len--;
        utf16_buf[utf16_len] = 0;
    } else {
        utf16_buf[utf16_len] = 0;
    }

    return utf16_len;
}
static
const char* cb_windows_error_string( DWORD error_code ) {
    uint16_t utf16_buf[255] = {};

    int utf16_len = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code,
        0, utf16_buf, sizeof(utf16_buf), 0 );

    uint8_t* utf8_buf = cb_windows_utf8_buf();
    cb_windows_cvt_utf8_from_utf16(
        UTF8_SIZE, utf8_buf, utf16_len, utf16_buf );

    return (const char*)utf8_buf;
}
static
int cb_windows_utf16_path_from_utf8(
    int utf16_cap, uint16_t* utf16_buf,
    int utf8_len, const uint8_t* utf8_buf
) {
    enum {
        CB_WINDOWS_PATH_RELATIVE,
        CB_WINDOWS_PATH_HOME,
        CB_WINDOWS_PATH_ABSOLUTE
    } path_type = CB_WINDOWS_PATH_RELATIVE;

    if( !utf8_len ) {
        return 0;
    }

    #define push( c ) do { \
        if( utf16_len != utf16_cap ) { \
            utf16_buf[utf16_len++] = (c); \
        } \
    } while(0)

    #define append( count, buf ) do { \
        int max_copy = (count); \
        if( max_copy > (utf16_cap - utf16_len) ) { \
            max_copy = (utf16_cap - utf16_len); \
        } \
        memcpy( utf16_buf + utf16_len, (buf), sizeof(uint16_t) * max_copy ); \
        utf16_len += max_copy; \
    } while(0)

    int utf16_len = 0;

    if( utf8_len >= sizeof("A:") ) {
        if(
            isalpha( utf8_buf[0] ) &&
            (utf8_buf[1] == ':')   &&
            (utf8_buf[2] == '/' || utf8_buf[2] == '\\')
        ) {
            path_type = CB_WINDOWS_PATH_ABSOLUTE;
        }
    } else if( utf8_buf[0] == '~' ) {
        path_type = CB_WINDOWS_PATH_HOME;
    }

    append( sizeof("\\\\?"), L"\\\\?\\" );

    switch( path_type ) {
        case CB_WINDOWS_PATH_RELATIVE: {
            utf16_len += GetCurrentDirectoryW( utf16_cap - utf16_len, utf16_buf + utf16_len );
        } break;
        case CB_WINDOWS_PATH_HOME: {
            utf16_len += GetEnvironmentVariableW(
                L"HOMEDRIVE", utf16_buf + utf16_len, utf16_cap - utf16_len );
            utf16_len += GetEnvironmentVariableW(
                L"HOMEPATH", utf16_buf + utf16_len, utf16_cap - utf16_len );
            if( utf16_buf[utf16_len - 1] != L'\\' ) {
                push( '\\' );
            }
        } break;
        case CB_WINDOWS_PATH_ABSOLUTE:
            break;
    }

    int minimum = sizeof("\\\\?\\A:");
    if( minimum >= utf16_cap ) {
        minimum = utf16_cap;
    }

    while( utf8_len ) {
        int chunk_len = utf8_len;
        for( int i = 0; i < utf8_len; ++i ) {
            if(
                (utf8_buf[i] == '\\' ) || 
                (utf8_buf[i] == '/' ) 
            ) {
                chunk_len = i;
                break;
            }
        }

        if( (chunk_len == 1) && (memcmp( ".", utf8_buf, chunk_len ) == 0) ) {
        } else if( (chunk_len == 2) && (memcmp( "..", utf8_buf, chunk_len ) == 0) ) {
            for( int i = utf16_len; i-- > 0; ) {
                if( utf16_buf[i] == '\\' ) {
                    utf16_len = i;
                    break;
                }
            }
            
            if( utf16_len < minimum ) {
                utf16_len = minimum;
            }
        } else {
            if( utf16_len && (utf16_buf[utf16_len - 1] != '\\') ) {
                push( '\\' );
            }
            utf16_len += cb_windows_cvt_utf16_from_utf8(
                utf16_cap - utf16_len, utf16_buf + utf16_len, chunk_len, utf8_buf );
        }

        if( chunk_len == utf8_len ) {
            utf8_len -= chunk_len;
            utf8_buf += chunk_len;
        } else {
            utf8_len -= chunk_len + 1;
            utf8_buf += chunk_len + 1;
        }
    }

    if( (utf16_len == utf16_cap) && utf16_cap ) {
        utf16_len--;
        utf16_buf[utf16_len] = 0;
    } else {
        utf16_buf[utf16_len] = 0;
    }

    #undef push
    #undef append

    return utf16_len;
}
static
wchar_t* cb_windows_path( const char* path ) {
    int path_len = strlen( path );
    if( path_len < MAX_PATH ) {
        uint16_t* buf = cb_windows_utf16_buf();
        cb_windows_cvt_utf16_from_utf8( UTF16_SIZE, buf, path_len, (const uint8_t*)path );
        return (wchar_t*)buf;
    } else {
        uint16_t* buf = cb_windows_utf16_buf();
        cb_windows_utf16_path_from_utf8( UTF16_SIZE, buf, path_len, path );
        return (wchar_t*)buf;
    }
}
static
bool cb_windows_read32(
    HANDLE handle, uint32_t bytes, void* buf, uint32_t* opt_out_read
) {
    DWORD read = 0;
    if( !ReadFile( handle, buf, bytes, &read, NULL ) ) {
        return false;
    }
    if( opt_out_read ) {
        *opt_out_read = read;
    }
    return true;
}
static
bool cb_windows_write32(
    HANDLE handle, uint32_t bytes, const void* buf, uint32_t* opt_out_write
) {
    DWORD read = 0;
    if( !WriteFile( handle, buf, bytes, &read, NULL ) ) {
        return false;
    }
    if( opt_out_write ) {
        *opt_out_write = read;
    }
    return true;
}
static
CB_Time cb_windows_time_from_filetime( FILETIME ft ) {
    #define WINDOWS_TICKS_PER_SECOND (10000000)
    #define WINDOWS_TO_POSIX_DIFF    (11644473600ULL)

    ULARGE_INTEGER uli;
    uli.LowPart  = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    CB_Time res = (CB_Time)(
        (uli.QuadPart / WINDOWS_TICKS_PER_SECOND) - WINDOWS_TO_POSIX_DIFF );

    #undef WINDOWS_TICKS_PER_SECOND
    #undef WINDOWS_TO_POSIX_DIFF
    return res;
}
static
CB_FileType cb_windows_file_type_from_attrib( DWORD attrib ) {
    if( attrib == INVALID_FILE_ATTRIBUTES ) {
        return CB_FTYPE_NULL;
    }

    if( attrib & FILE_ATTRIBUTE_DIRECTORY ) {
        return CB_FTYPE_DIRECTORY;
    } else {
        return CB_FTYPE_FILE;
    }
}
static
bool cb_windows_directory_remove( int* path_len, wchar_t* path, WIN32_FIND_DATAW* data ) {
    HANDLE handle = FindFirstFileExW(
        path, FindExInfoBasic, data, FindExSearchNameMatch, 0, 0 );
    if( handle == INVALID_HANDLE_VALUE ) {
        CB_ERROR(
            "Windows: cb_directory_remove(): failed to open %S! reason: %s",
            path, cb_windows_error_string( GetLastError() ) );
        return false;
    }
    *path_len       -= 2;
    path[*path_len]  = 0;

    usize original_len = *path_len;
    for( ;; ) {
        if(
            (wcscmp(  L".", data->cFileName ) == 0) ||
            (wcscmp( L"..", data->cFileName ) == 0)
        ) {
            if( FindNextFileW( handle, data ) ) {
                continue;
            } else {
                break;
            }
        }

        *path_len        = original_len;
        path[*path_len]  = '\\';
        *path_len       += 1;

        usize file_name_len = wcslen( data->cFileName );
        memcpy( path + *path_len, data->cFileName, sizeof(wchar_t) * file_name_len );

        *path_len       += file_name_len;
        path[*path_len]  = 0;

        if( data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            path[*path_len] = '\\';
            *path_len += 1;
            path[*path_len] = '*';
            *path_len += 1;
            path[*path_len] = 0;

            if( cb_windows_directory_remove( path_len, path, data ) ) {
                continue;
            } else {
                break;
            }
        }

        if( !DeleteFileW( path ) ) {
            CB_ERROR(
                "Windows: cb_directory_remove(): failed to remove file %S! reason: %s",
                path, cb_windows_error_string( GetLastError() ) );
            break;
        }
    }

    CloseHandle( handle );
    if( RemoveDirectoryW( path ) ) {
        return true;
    }

    CB_ERROR(
        "Windows: cb_directory_remove(): failed to remove directory %S! reason: %s",
        path, cb_windows_error_string( GetLastError() ) );
    return false;
}
static enum CB_WindowsDirectoryWalkResult
cb_windows_directory_walk(
    CB_DirectoryWalkFN* callback, void* params,
    int utf8_cap, uint8_t* utf8_buf,
    int utf16_cap, int* utf16_len, uint16_t* utf16_buf,
    WIN32_FIND_DATAW* data, CB_DirectoryWalkInfo* info
) {
    HANDLE handle = FindFirstFileExW(
        utf16_buf, FindExInfoBasic, data, FindExSearchNameMatch, 0, 0 );
    if( handle == INVALID_HANDLE_VALUE ) {
        CB_ERROR( "Windows: cb_directory_walk(): failed to open %S!", utf16_buf );
        return CB_WINDOWS_DIRECTORY_WALK_ERROR;
    }

    #define push( c ) do { \
        if( (*utf16_len + 1) != utf16_cap ) { \
            utf16_buf[*utf16_len] = c; \
            *utf16_len += 1; \
            utf16_buf[*utf16_len] = 0; \
        } \
    } while(0)
    #define append( count, buf ) do { \
        int max_copy = (count); \
        if( (max_copy + 1) > (utf16_cap - *utf16_len) ) { \
            max_copy = (utf16_cap - *utf16_len); \
        } \
        memcpy( utf16_buf + *utf16_len, buf, sizeof(uint16_t) * max_copy ); \
        *utf16_len += max_copy; \
        utf16_buf[*utf16_len] = 0; \
    } while(0)

    *utf16_len            -= 2;
    utf16_buf[*utf16_len]  = 0;

    int original_len = *utf16_len;

    // TODO(alicia): bounds check
    int directory_len = cb_windows_cvt_utf8_from_utf16(
        utf8_cap, utf8_buf, original_len - sizeof("\\\\?"), utf16_buf + sizeof("\\\\?") );
    utf8_buf[directory_len] = '\\';

    for( ;; ) {
        if(
            (wcscmp(  L".", data->cFileName ) == 0) ||
            (wcscmp( L"..", data->cFileName ) == 0)
        ) {
            if( FindNextFileW( handle, data ) ) {
                continue;
            } else {
                break;
            }
        }

        *utf16_len = original_len;
        push( '\\' );

        int file_len = wcslen( data->cFileName );
        append( file_len, data->cFileName );

        int file_len_utf8 = cb_windows_cvt_utf8_from_utf16(
            utf8_cap - (directory_len + 1), utf8_buf + (directory_len + 1),
            *utf16_len, utf16_buf );

        info->path = utf8_buf;

        // NOTE(alicia): + 1 is for path separator.
        info->path_len         = directory_len + file_len_utf8 + 1;
        info->path_name_offset = directory_len + 1;

#if CB_ARCH_IS_64BIT
        /* read file size */ {
            ULARGE_INTEGER li;
            li.LowPart  = data->nFileSizeLow;
            li.HighPart = data->nFileSizeHigh;

            info->file_size = li.QuadPart;
        }
#else
        info->file_size = data->nFileSizeLow;
#endif

        info->type = cb_windows_file_type_from_attrib( data->dwFileAttributes );
        CB_DirectoryWalkControl control = callback( info, params );

        switch( control ) {
            case CB_DWC_CONTINUE: if( info->type == CB_FTYPE_DIRECTORY ) {
                append( 2, L"\\*" );

                info->level++;

                enum CB_WindowsDirectoryWalkResult result =
                    cb_windows_directory_walk(
                        callback, params, utf8_cap,
                        utf8_buf, utf16_cap, utf16_len, utf16_buf, data, info );

                switch( result ) {
                    case CB_WINDOWS_DIRECTORY_WALK_OK:
                        break;
                    case CB_WINDOWS_DIRECTORY_WALK_STOP:
                    case CB_WINDOWS_DIRECTORY_WALK_ERROR: {
                        CloseHandle( handle );
                    } return result;
                }

                info->level--;
            } break;
            case CB_DWC_STOP: {
                CloseHandle( handle );
            } return CB_WINDOWS_DIRECTORY_WALK_STOP;
            case CB_DWC_SKIP:
                break;
        }

        if( !FindNextFileW( handle, data ) ) {
            break;
        }
    }

    #undef push
    #undef append
    CloseHandle( handle );
    return CB_WINDOWS_DIRECTORY_WALK_OK;
}


CB_Time cb_time_query(void) {
    FILETIME ft;
    memset( &ft, 0, sizeof(ft) );
    GetSystemTimeAsFileTime( &ft );

    return cb_windows_time_from_filetime( ft );
}
double cb_time_msec(void) {
    LARGE_INTEGER qpf;
    QueryPerformanceFrequency( &qpf );
    LARGE_INTEGER qpc;
    QueryPerformanceCounter( &qpc );

    return ((f64)qpc.QuadPart / (f64)qpf.QuadPart) * 1000.0;
}
double cb_time_sec(void) {
    LARGE_INTEGER qpf;
    QueryPerformanceFrequency( &qpf );
    LARGE_INTEGER qpc;
    QueryPerformanceCounter( &qpc );

    return ((f64)qpc.QuadPart / (f64)qpf.QuadPart);
}

CB_FileType cb_path_query_type( const char* path_utf8 ) {
    wchar_t* wpath = cb_windows_path( path_utf8 );
    DWORD attrib  = GetFileAttributesW( wpath );
    
    return cb_windows_file_type_from_attrib( attrib );
}
bool cb_path_query_time_modify( const char* path_utf8, CB_Time* out_time ) {
    wchar_t* wpath = cb_windows_path( path_utf8 );
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    if( !GetFileAttributesExW( wpath, GetFileExInfoStandard, &data ) ) {
        CB_ERROR(
            "Windows: cb_path_query_time_modify() failed to query %s info! reason: %s",
            path_utf8, cb_windows_error_string( GetLastError() ) );
        return false;
    }

    *out_time = cb_windows_time_from_filetime( data.ftLastWriteTime );
    return true;
}
bool cb_path_query_time_create( const char* path_utf8, CB_Time* out_time ) {
    wchar_t* wpath = cb_windows_path( path_utf8 );
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    if( !GetFileAttributesExW( wpath, GetFileExInfoStandard, &data ) ) {
        CB_ERROR(
            "Windows: cb_path_query_time_create() failed to query %s info! reason: %s",
            path_utf8, cb_windows_error_string( GetLastError() ) );
        return false;
    }

    *out_time = cb_windows_time_from_filetime( data.ftCreationTime );
    return true;
}
bool cb_path_query_info( const char* path_utf8, CB_FileInfo* out_info ) {
    uint16_t* wpath = cb_windows_path( path_utf8 );
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    if( !GetFileAttributesExW( wpath, GetFileExInfoStandard, &data ) ) {
        CB_ERROR(
            "Windows: cb_path_query_info() failed to query %s info! reason: %s",
            path_utf8, cb_windows_error_string( GetLastError() ) );
        return false;
    }

#if CB_ARCH_IS_64BIT
    /* Read file size */ {
        ULARGE_INTEGER li;
        li.LowPart  = data.nFileSizeLow;
        li.HighPart = data.nFileSizeHigh;

        out_info->size = li.QuadPart;
    }
#else
    out_info->size = data.nFileSizeLow;
#endif
    
    out_info->type        = cb_windows_file_type_from_attrib( data.dwFileAttributes );
    out_info->time.create = cb_windows_time_from_filetime( data.ftCreationTime );
    out_info->time.modify = cb_windows_time_from_filetime( data.ftLastWriteTime );

    return true;
}
char* cb_path_canonicalize( const char* path_utf8 ) {
    uint16_t* utf16_buf = cb_windows_utf16_buf();
    int utf16_len = cb_windows_utf16_path_from_utf8(
        UTF16_SIZE, utf16_buf, strlen(path_utf8), path_utf8 );
    if( !utf16_len ) {
        return NULL;
    }
    utf16_buf += sizeof("\\\\?");
    utf16_len -= sizeof("\\\\?");

    uint8_t* temp_utf8 = cb_windows_utf8_buf();
    int utf8_len = cb_windows_cvt_utf8_from_utf16(
        UTF8_SIZE, temp_utf8, utf16_len, utf16_buf );

    char* result = malloc( utf8_len + 1 );
    memcpy( result, temp_utf8, utf8_len );
    result[utf8_len] = 0;
    return result;
}

bool cb_file_open( const char* path_utf8, CB_FileOpenFlags flags, CB_File* out_file ) {
    wchar_t* wpath = cb_windows_path( path_utf8 );

    DWORD dwDesiredAccess       = 0;
    DWORD dwShareMode           = 0;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes  = 0;

    if( ( flags & ( CB_FOPEN_READ ) ) == ( CB_FOPEN_READ ) ) {
        dwDesiredAccess |= GENERIC_READ;
    }
    if( ( flags & ( CB_FOPEN_WRITE ) ) == ( CB_FOPEN_WRITE ) ) {
        dwDesiredAccess |= GENERIC_WRITE;
    }

    if( ( flags & ( CB_FOPEN_CREATE ) ) == ( CB_FOPEN_CREATE ) ) {
        dwCreationDisposition = OPEN_ALWAYS;
    } else if( ( flags & ( CB_FOPEN_TRUNCATE ) ) == ( CB_FOPEN_TRUNCATE ) ) {
        dwCreationDisposition = TRUNCATE_EXISTING;
    }

    bool append = ( flags & ( CB_FOPEN_APPEND ) ) == ( CB_FOPEN_APPEND );

    HANDLE handle = CreateFileW(
        wpath, dwDesiredAccess, dwShareMode, 0,
        dwCreationDisposition, dwFlagsAndAttributes, 0 );

    if( !handle || (handle == INVALID_HANDLE_VALUE) ) {
        CB_ERROR(
            "Windows: cb_file_open(): failed to open file %s! reason: %s",
            path_utf8, cb_windows_error_string( GetLastError() ) );
        return false;
    }

    out_file->_internal_handle = handle;

    if( append ) {
        cb_file_seek( out_file, 0, CB_FSEEK_END );
    }

    return true;
}
void cb_file_close( CB_File* file ) {
    CloseHandle( file->_internal_handle );
    file->_internal_handle = 0;
}
intptr_t cb_file_seek( CB_File* file, intptr_t offset, CB_FileSeek type ) {
    DWORD dwMoveMethod = FILE_BEGIN;
    switch( type ) {
        case CB_FSEEK_CUR: {
            dwMoveMethod = FILE_CURRENT;
        } break;
        case CB_FSEEK_SET: {
            dwMoveMethod = FILE_BEGIN;
        } break;
        case CB_FSEEK_END: {
            dwMoveMethod = FILE_END;
        } break;
    }
#if CB_ARCH_IS_64BIT
    LARGE_INTEGER move = { .QuadPart = offset }, new_pointer = {};
    SetFilePointerEx( file->_internal_handle, move, &new_pointer, dwMoveMethod );
    return new_pointer.QuadPart;
#else
    return SetFilePointer( file->_internal_handle, offset, NULL, dwMoveMethod );
#endif
}
void cb_file_truncate( CB_File* file ) {
    SetEndOfFile( file->_internal_handle );
}
bool cb_file_read(
    CB_File* file, uintptr_t buffer_size, void* buffer, uintptr_t* opt_out_read
) {
#if CB_ARCH_IS_64BIT
    uint32_t part_a_size = buffer_size > 0xFFFFFFFF ? 0xFFFFFFFF : buffer_size;
    void*    part_a      = buffer;
    uint32_t part_b_size = part_a_size == 0xFFFFFFFF ? buffer_size - part_a_size : 0;
    void*    part_b      = (uint8_t*)buffer + part_a_size;

    uintptr_t total_read = 0;
    uint32_t  read       = 0;
    if( !cb_windows_read32( file->_internal_handle, part_a_size, part_a, &read ) ) {
        return false;
    }
    if( !part_b_size ) {
        if( opt_out_read ) {
            *opt_out_read = read;
        }
        return true;
    }

    total_read += read;
    read        = 0;
    if( !cb_windows_read32( file->_internal_handle, part_b_size, part_b, &read ) ) {
        return false;
    }
    total_read += read;

    if( opt_out_read ) {
        *opt_out_read = total_read;
    }
    return true;
#else
    return cb_windows_read32( file->_internal_handle, buffer_size, buffer, opt_out_read );
#endif
}
bool cb_file_write(
    CB_File* file, uintptr_t buffer_size, const void* buffer, uintptr_t* opt_out_write
) {
#if CB_ARCH_IS_64BIT
    uint32_t    part_a_size = buffer_size > 0xFFFFFFFF ? 0xFFFFFFFF : buffer_size;
    const void* part_a      = buffer;
    uint32_t    part_b_size = part_a_size == 0xFFFFFFFF ? buffer_size - part_a_size : 0;
    const void* part_b      = (const uint8_t*)buffer + part_a_size;

    uintptr_t total_write = 0;
    uint32_t  write       = 0;
    if( !cb_windows_write32( file->_internal_handle, part_a_size, part_a, &write ) ) {
        return false;
    }
    if( !part_b_size ) {
        if( opt_out_write ) {
            *opt_out_write = write;
        }
        return true;
    }

    total_write += write;
    write        = 0;
    if( !cb_windows_write32( file->_internal_handle, part_b_size, part_b, &write ) ) {
        return false;
    }
    total_write += write;

    if( opt_out_write ) {
        *opt_out_write = total_write;
    }
    return true;
#else
    return cb_windows_write32( file->_internal_handle, buffer_size, buffer, opt_out_write );
#endif
}
void cb_file_write_fmt_va( CB_File* file, const char* fmt, va_list va ) {
    va_list cva;
    va_copy( cva, va );
    int len = vsnprintf( NULL, 0, fmt, cva );
    va_end( cva );

    if( len < UTF8_SIZE ) {
        uint8_t* utf8 = cb_windows_utf8_buf();
        vsnprintf( utf8, UTF8_SIZE, fmt, va );
        cb_file_write( file, len, utf8, NULL );
    } else {
        uint8_t* utf8 = CB_ALLOC( NULL, 0, len + 1 );
        vsnprintf( utf8, UTF8_SIZE, fmt, va );
        cb_file_write( file, len, utf8, NULL );
        CB_FREE( utf8, len + 1 );
    }
}
bool cb_file_remove( const char* path_utf8 ) {
    wchar_t* wpath = cb_windows_path( path_utf8 );
    if( DeleteFileW( wpath ) ) {
        return true;
    }
    CB_ERROR(
        "Windows: cb_file_remove(): failed to remove %s! reason: %s",
        path_utf8, cb_windows_error_string( GetLastError() ) );
    return false;
}
bool cb_file_copy( const char* dst, const char* src, bool fail_if_dst_exists ) {
    int dst_utf8_len = strlen( dst );
    int src_utf8_len = strlen( src );

    uint16_t* dst_name = cb_windows_utf16_buf(); 
    uint16_t* src_name = dst_name + (UTF16_SIZE / 2);

    if( dst_utf8_len < MAX_PATH ) {
        cb_windows_cvt_utf16_from_utf8( UTF16_SIZE / 2, dst_name, dst_utf8_len, dst );
    } else {
        cb_windows_utf16_path_from_utf8( UTF16_SIZE / 2, dst_name, dst_utf8_len, dst );
    }

    if( src_utf8_len < MAX_PATH ) {
        cb_windows_cvt_utf16_from_utf8( UTF16_SIZE / 2, src_name, src_utf8_len, src );
    } else {
        cb_windows_utf16_path_from_utf8( UTF16_SIZE / 2, src_name, src_utf8_len, src );
    }

    if( CopyFileW( src_name, dst_name, fail_if_dst_exists ) ) {
        return true;
    }
    CB_ERROR(
        "Windows: cb_file_copy(): failed to copy %s to %s! reason: %s",
        src, dst, cb_windows_error_string( GetLastError() ) );
    return false;
}
bool cb_file_move( const char* dst, const char* src, bool fail_if_dst_exists ) {
    int dst_utf8_len = strlen( dst );
    int src_utf8_len = strlen( src );

    uint16_t* dst_name = cb_windows_utf16_buf(); 
    uint16_t* src_name = dst_name + (UTF16_SIZE / 2);

    if( dst_utf8_len < MAX_PATH ) {
        cb_windows_cvt_utf16_from_utf8( UTF16_SIZE / 2, dst_name, dst_utf8_len, dst );
    } else {
        cb_windows_utf16_path_from_utf8( UTF16_SIZE / 2, dst_name, dst_utf8_len, dst );
    }

    if( src_utf8_len < MAX_PATH ) {
        cb_windows_cvt_utf16_from_utf8( UTF16_SIZE / 2, src_name, src_utf8_len, src );
    } else {
        cb_windows_utf16_path_from_utf8( UTF16_SIZE / 2, src_name, src_utf8_len, src );
    }

    if( fail_if_dst_exists ) {
        DWORD attrib = GetFileAttributesW( dst_name );
        
        if( attrib != INVALID_FILE_ATTRIBUTES ) {
            CB_ERROR(
                "Windows: cb_file_move(): failed to move %s to %s! reason: %s",
                src, dst, "Destination file already exists!" );
            return false;
        }
    }

    if( MoveFileW( src_name, dst_name ) ) {
        return true;
    }
    CB_ERROR(
        "Windows: cb_file_move(): failed to move %s to %s! reason: %s",
        src, dst, cb_windows_error_string( GetLastError() ) );
    return false;
}

bool cb_directory_create( const char* path_utf8 ) {
    wchar_t* wpath = cb_windows_path( path_utf8 );
    if( CreateDirectoryW( wpath, NULL ) ) {
        return true;
    }
    CB_ERROR(
        "Windows: cb_directory_create(): failed to create %s! reason: %s",
        path_utf8, cb_windows_error_string( GetLastError() ) );
    return false;
}
bool cb_directory_remove( const char* path_utf8, bool recursive ) {
    if( recursive ) {
        uint16_t* wpath     = cb_windows_utf16_buf();
        int       wpath_len = cb_windows_utf16_path_from_utf8(
            UTF16_SIZE, wpath, strlen( path_utf8 ), path_utf8 );

        if( (wpath_len + sizeof("\\*") ) >= UTF16_SIZE ) {
            CB_ERROR(
                "Windows: cb_directory_remove(): failed to remove %s! path is too long!",
                path_utf8 );
            return false;
        }

        wpath[wpath_len++] = '\\';
        wpath[wpath_len++] = '*';
        wpath[wpath_len]   = 0;

        WIN32_FIND_DATAW data = {};
        return cb_windows_directory_remove( &wpath_len, wpath, &data );
    } else {
        uint16_t* wpath = cb_windows_path( path_utf8 );
        if( RemoveDirectoryW( wpath ) ) {
            return true;
        }
        CB_ERROR(
            "Windows: cb_directory_remove(): failed to remove %s! reason: %s",
            path_utf8, cb_windows_error_string( GetLastError() ) );
        return false;
    }
}
bool cb_directory_walk( const char* path_utf8, CB_DirectoryWalkFN* callback, void* params ) {
    uint8_t*  utf8_buf  = cb_windows_utf8_buf();
    uint16_t* utf16_buf = cb_windows_utf16_buf();

    int utf16_len = cb_windows_utf16_path_from_utf8(
        UTF16_SIZE, utf16_buf, strlen( path_utf8 ), utf8_buf );
    if( (utf16_len + sizeof("\\*")) > UTF16_SIZE ) {
        CB_ERROR(
            "Windows: cb_directory_walk(): failed to remove %s! path is too long!",
            path_utf8 );
        return false;
    }

    utf16_buf[utf16_len++] = '\\';
    utf16_buf[utf16_len++] = '*';
    utf16_buf[utf16_len]   = 0;

    WIN32_FIND_DATAW     data = {};
    CB_DirectoryWalkInfo info = {};

    return cb_windows_directory_walk(
        callback, params, UTF8_SIZE, utf8_buf,
        UTF16_SIZE, &utf16_len, utf16_buf, &data, &info ) != CB_WINDOWS_DIRECTORY_WALK_ERROR;
}

bool cb_working_directory_set( const char* new_cwd ) {
    wchar_t* wpath = cb_windows_path( new_cwd );
    if( SetCurrentDirectoryW( wpath ) ) {
        STATE->cwd_obtained = false;
        return true;
    }

    CB_ERROR(
        "Windows: cb_working_directory_set(): failed to set directory! reason: %s",
        cb_windows_error_string( GetLastError() ) );
    return false;
}
const char* cb_working_directory_query(void) {
    if( STATE->cwd_obtained ) {
        return (const char*)STATE->cwd;
    }

    uint16_t* buf = cb_windows_utf16_buf();
    DWORD len = GetCurrentDirectoryW( UTF16_SIZE, buf );

    if( wcscmp( buf, L"\\\\?\\" ) == 0 ) {
        buf += sizeof("\\\\?");
        len -= sizeof("\\\\?");
    }

    cb_windows_cvt_utf8_from_utf16( UTF8_SIZE, STATE->cwd, len, buf );

    STATE->cwd_obtained = true;
    return (const char*)STATE->cwd;
}

bool cb_pipe_open( CB_PipeRead* out_read, CB_PipeWrite* out_write ) {
    HANDLE read, write;

    // NOTE(alicia): 
    // bInheritHandle set to TRUE because
    // otherwise these pipes can't be passed to a child process.
    SECURITY_ATTRIBUTES sa = {};
    sa.nLength        = sizeof(sa);
    sa.bInheritHandle = TRUE;

    if( !CreatePipe( &read, &write, NULL, 0 ) ) {
        CB_ERROR(
            "Windows: cb_pipe_open(): failed to create pipes! reason: %s",
            cb_windows_error_string( GetLastError() ) );
        return false;
    }
    out_read->_internal_handle  = read;
    out_write->_internal_handle = write;
    return true;
}
void cb_pipe_close( CB_Pipe* pipe ) {
    CloseHandle( pipe->_internal_handle );
    memset( pipe, 0, sizeof(*pipe) );
}
CB_PipeRead cb_pipe_stdin(void) {
    CB_PipeRead result;
    result._internal_handle = GetStdHandle( STD_INPUT_HANDLE );
    return result;
}
CB_PipeWrite cb_pipe_stdout(void) {
    CB_PipeWrite result;
    result._internal_handle = GetStdHandle( STD_OUTPUT_HANDLE );
    return result;
}
CB_PipeWrite cb_pipe_stderr(void) {
    CB_PipeWrite result;
    result._internal_handle = GetStdHandle( STD_ERROR_HANDLE );
    return result;
}

const char* cb_environment_query( const char* name ) {
    uint16_t* wname  = cb_windows_utf16_buf();
    uint16_t* wvalue = wname + (UTF16_SIZE / 2);

    int wname_len = cb_windows_cvt_utf16_from_utf8(
        UTF16_SIZE, wname, strlen(name), (const uint8_t*)name );

    if( wname_len >= (UTF16_SIZE / 2) ) {
        CB_ERROR( "Windows: cb_environment_query(): name exceeded %d in length!", (int)UTF16_SIZE );
        return NULL;
    }

    int wvalue_len = GetEnvironmentVariableW( wname, wvalue, (UTF16_SIZE / 2) );

    uint8_t* utf8_buf = cb_windows_utf8_buf();
    int utf8_len = cb_windows_cvt_utf8_from_utf16( UTF8_SIZE, utf8_buf, wvalue_len, wvalue );

    char* result = CB_ALLOC( NULL, 0, utf8_len + 1 );

    return (const char*)memcpy( result, utf8_buf, utf8_len );
}
bool cb_environment_set( const char* name, const char* new_value ) {
    if( !name || !new_value ) {
        return false;
    }
    uint16_t* wname  = cb_windows_utf16_buf();
    uint16_t* wvalue = wname + (UTF16_SIZE / 2);

    cb_windows_cvt_utf16_from_utf8( UTF16_SIZE / 2, wname, strlen(name), name );
    cb_windows_cvt_utf16_from_utf8( UTF16_SIZE / 2, wvalue, strlen(new_value), new_value );

    if( SetEnvironmentVariableW( wname, wvalue ) ) {
        return true;
    }

    CB_ERROR(
        "Windows: cb_environment_set(): "
        "failed to set %s to %s! reason: %s",
        name, new_value, cb_windows_error_string( GetLastError() ) );
    return false;
}

bool cb_process_exec_async(
    CB_Command             cmd,
    CB_ProcessID*          out_pid,
    const char*            opt_working_directory,
    CB_EnvironmentBuilder* opt_environment,
    CB_PipeRead*           opt_stdin,
    CB_PipeWrite*          opt_stdout,
    CB_PipeWrite*          opt_stderr
) {
    STARTUPINFOW        startup = {};
    PROCESS_INFORMATION info    = {};

    startup.cb         = sizeof(startup);
    startup.hStdInput  = GetStdHandle( STD_INPUT_HANDLE );
    startup.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    startup.hStdError  = GetStdHandle( STD_ERROR_HANDLE );

    BOOL bInheritHandle = FALSE;
    if( opt_stdin ) {
        startup.hStdInput = (HANDLE)opt_stdin->_internal_handle;
        bInheritHandle    = TRUE;
    }
    if( opt_stdout ) {
        startup.hStdOutput = (HANDLE)opt_stdout->_internal_handle;
        bInheritHandle     = TRUE;
    }
    if( opt_stderr ) {
        startup.hStdError = (HANDLE)opt_stderr->_internal_handle;
        bInheritHandle    = TRUE;
    }
    if( bInheritHandle ) {
        startup.dwFlags |= STARTF_USESTDHANDLES;
    }

    struct CB_WindowsStringBuilderUTF16 cmdline = {};
    uint16_t* utf16_buf = cb_windows_utf16_buf();

    for( int i = 0; i < cmd.len; ++i ) {
        if( !cmd.buf[i] ) {
            continue;
        }
        CB_StringSlice arg  = CB_STRING_FROM_CSTR( cmd.buf[i] );
        bool has_whitespace = cb_string_find( arg, ' ' );
        if( has_whitespace ) {
            CB_PUSH( &cmdline, L'\"' );
        }

        int arg_len_utf16 = cb_windows_cvt_utf16_from_utf8(
            UTF16_SIZE, utf16_buf, arg.len, arg.buf );
        CB_APPEND( &cmdline, arg_len_utf16, utf16_buf );

        if( has_whitespace ) {
            CB_PUSH( &cmdline, L'\"' );
        }

        if( (i + 1) < cmd.len ) {
            CB_PUSH( &cmdline, L' ' );
        } else {
            CB_PUSH( &cmdline, 0 );
        }
    }

    struct CB_WindowsStringBuilderUTF16 env = {};
    if( opt_environment ) {
        LPWCH env_block = GetEnvironmentStringsW();
        if( !env_block ) {
            CB_ERROR(
                "Windows: cb_process_exec(): failed to obtain environment block "
                "of the calling process!" );
            CB_FREE( cmdline.buf, sizeof(uint16_t) * cmdline.cap );
            return false;
        }

        wchar_t* scan = env_block;
        while( *scan ) {
            int block_len = wcslen( scan );
            CB_APPEND( &env, block_len, scan );
            CB_PUSH( &env, 0 );
            scan += block_len + 1;
        }

        FreeEnvironmentStringsW( env_block );

        // TODO(alicia): do I need to check for duplicate entries?
        uint16_t* wname  = utf16_buf;
        uint16_t* wvalue = utf16_buf + (UTF16_SIZE / 2);
        for( int i = 0; i < opt_environment->len; ++i ) {
            const char* name  = opt_environment->name[i];
            const char* value = opt_environment->value[i];

            int wname_len = cb_windows_cvt_utf16_from_utf8(
                UTF16_SIZE / 2, wname, strlen(name), name );
            int wvalue_len = cb_windows_cvt_utf16_from_utf8(
                UTF16_SIZE / 2, wvalue, strlen(value), value );

            CB_APPEND( &env, wname_len, wname );
            CB_PUSH( &env, L'=' );
            CB_APPEND( &env, wvalue_len, wvalue );
            CB_PUSH( &env, 0 );
        }
        CB_PUSH( &env, 0 );
    }
    uint16_t* working_directory = 0;
    if( opt_working_directory ) {
        working_directory = cb_windows_path( opt_working_directory );
        CB_INFO( "chdir: '%s'", opt_working_directory );
    }

    DWORD dwCreationFlags = 0;
    if( opt_environment ) {
        CB_StringBuilder env_string = {};
        for( int i = 0; i < opt_environment->len; ++i ) {
            int name_len  = strlen( opt_environment->name[i] );
            int value_len = strlen( opt_environment->value[i] );

            CB_APPEND( &env_string, name_len, opt_environment->name[i] );
            CB_PUSH( &env_string, '=' );
            CB_APPEND( &env_string, value_len, opt_environment->value[i] );
            CB_PUSH( &env_string, ' ' );
        }
        CB_INFO( "  > %s%S", env_string.buf, cmdline.buf );
        CB_FREE( env_string.buf, env_string.cap );

        dwCreationFlags = CREATE_UNICODE_ENVIRONMENT;
    } else {
        CB_INFO( "  > %S", cmdline.buf );
    }

    BOOL result = CreateProcessW(
        NULL, cmdline.buf, NULL, NULL, bInheritHandle,
        dwCreationFlags, env.buf, working_directory, &startup, &info );
    DWORD error_code = GetLastError();

    CB_FREE( cmdline.buf, sizeof(uint16_t) * cmdline.cap );
    if( opt_environment ) {
        CB_FREE( env.buf, sizeof(uint16_t) * env.cap );
    }

    if( result ) {
        out_pid->_internal_handle = info.hProcess;
        CloseHandle( info.hThread );
        return true;
    }

    CB_ERROR(
        "Windows: cb_process_exec(): failed to execute process %s! reason: %s",
        cmd.buf[0], cb_windows_error_string( error_code ) );
    return false;
}

void cb_process_discard( CB_ProcessID* pid ) {
    CloseHandle( pid->_internal_handle );
    pid->_internal_handle = 0;
}
int cb_process_wait( CB_ProcessID* pid ) {
    int exit_code = 0;
    if( cb_process_wait_timed( pid, INFINITE, &exit_code ) ) {
        return exit_code;
    }
    return -2;
}
bool cb_process_wait_timed( CB_ProcessID* pid, uint32_t msec, int* opt_out_exit_code ) {
    DWORD result = WaitForSingleObject( pid->_internal_handle, msec );
    switch( result ) {
        case WAIT_OBJECT_0:
            break;
        case WAIT_TIMEOUT:
            return false;
        default: {
            if( opt_out_exit_code ) {
                *opt_out_exit_code = -2;
            }
            CB_ERROR(
                "Windows: cb_process_wait(): failed to wait for pid! reason: %s",
                cb_windows_error_string( GetLastError() ) );
        } return false;
    }

    DWORD exit_code = 0;
    if( !GetExitCodeProcess( pid->_internal_handle, &exit_code ) ) {
        CB_ERROR(
            "Windows: cb_process_wait(): failed to get exit code! reason: %s",
            cb_windows_error_string( GetLastError() ) );
        if( opt_out_exit_code ) {
            *opt_out_exit_code = -2;
        }
        return false;
    }

    if( opt_out_exit_code ) {
        *opt_out_exit_code = exit_code;
    }
    cb_process_discard( pid );
    return true;
}
void cb_process_kill( CB_ProcessID* pid ) {
    TerminateProcess( pid->_internal_handle, 0 );
    cb_process_discard( pid );
}
bool cb_process_is_in_path( const char* process_name ) {
    uint8_t* utf8_buf = cb_windows_utf8_buf();
    snprintf( utf8_buf, UTF8_SIZE, "WHERE %s /Q\0", process_name );
    return system( utf8_buf ) == 0;
}

#undef STATE
#undef UTF16_SIZE
#undef UTF8_SIZE

#endif /* Windows Implementation */

#endif /* CB_IMPLEMENTATION end */
