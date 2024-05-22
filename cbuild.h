#if !defined(CBUILD_H)
#define CBUILD_H
/**
 * @file   cbuild.h
 * @brief  C build system.
 * @details
 * Single header library, include to get access to API,
 * include again (ONCE) with CBUILD_IMPLEMENTATION defined.
 * Define THREAD_COUNT before implementation to set maximum
 * number of threads allowed.
 * Inspired by https://github.com/tsoding/nobuild
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   April 06, 2024
*/
// IWYU pragma: begin_exports
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#if !defined(_WIN32)
    #include <unistd.h>
#endif /* POSIX includes */
// IWYU pragma: end_exports

// NOTE(alicia): begin: Types

/// @brief Enumerator describing logging level.
/// @details Greater value is more exclusive.
typedef enum {
    /// @brief Logging level info.
    /// @details Info, warning and error messages are allowed.
    LOG_LEVEL_INFO    = 0,
    /// @brief Logging level warning.
    /// @details Warning and error messages are allowed.
    LOG_LEVEL_WARNING = 1,
    /// @brief Logging level error. Most restrictive logging level.
    /// @details Only error messages are allowed.
    LOG_LEVEL_ERROR   = 2,
} LogLevel;

#if defined(_WIN32)
/// @brief Windows does not define ssize_t so this is here.
typedef intptr_t ssize_t;
#endif /* Windows types */

/// @brief 8-bit Boolean.
/// @details When returned from library, guaranteed to be true(1) or false(0) only.
typedef uint8_t  bool8_t;
/// @brief 16-bit Boolean.
/// @details When returned from library, guaranteed to be true(1) or false(0) only.
typedef uint16_t bool16_t;
/// @brief 32-bit Boolean.
/// @details When returned from library, guaranteed to be true(1) or false(0) only.
typedef uint32_t bool32_t;
/// @brief 64-bit Boolean.
/// @details When returned from library, guaranteed to be true(1) or false(0) only.
typedef uint64_t bool64_t;
/// @brief Boolean (same as _Bool).
/// @details When returned from library, guaranteed to be true(1) or false(0) only.
typedef _Bool    bool_t;

/// @brief String that is dynamically allocated on the heap.
typedef char DynamicString;
/// @brief Slice of string.
/// @details Not guaranteed to be null-terminated unless specified.
typedef struct StringSlice {
    /// @brief Length of string slice.
    /// @details Does not include null-terminator when created from null-terminated string.
    size_t len;
    /// @brief Pointer to start of UTF-8 string buffer.
    const char* cc;
} StringSlice;
/// @brief Volatile 32-bit unsigned integer. For use with atomic operations.
typedef volatile uint32_t Atom;
/// @brief Volatile 64-bit unsigned integer. For use with atomic operations.
typedef volatile uint64_t Atom64;
/// @brief Ways of seeking file. 0 can also be used to seek from current position.
typedef enum {
    /// @brief Seek file from current position.
    FS_CURRENT = 0,
    /// @brief Seek file from beginning.
    FS_BEGIN   = 1,
    /// @brief Seek file from end.
    FS_END     = 2,
} FileSeek;
/// @brief Enumerator describing a type of file.
typedef enum {
    /// @brief File type is unknown.
    FT_UNKNOWN   = 0,
    /// @brief File type is regular file.
    FT_FILE      = 1,
    /// @brief File type is directory.
    FT_DIRECTORY = 2,
} FileType;
/// @brief Bitfield for describing how to open a file.
typedef enum {
    /// @brief Open file for reading.
    FF_READ     = (1 << 0),
    /// @brief Open file for writing.
    FF_WRITE    = (1 << 1),
    /// @brief Create file if it does not already exist.
    /// @details Valid if file is opened for reading and/or writing.
    FF_CREATE   = (1 << 2),
    /// @brief Truncate file if it already exists.
    /// @details Valid if file is opened for writing exclusively and FF_APPEND is not set.
    FF_TRUNCATE = (1 << 3),
    /// @brief Move file pointer to end of file.
    /// @details Valid if FF_TRUNCATE is not set.
    FF_APPEND   = (1 << 4),
} FileFlags;
/// @brief File info struct.
typedef struct File {
#if defined(_WIN32)
    /// @brief Platform handle to to file.
    void* handle;
#else /* Windows */
    /// @brief Platform handle to to file.
    int handle; // fd
#endif /* POSIX */
    /// @brief Type of file.
    FileType type;
    /// @brief Flags used to open the file.
    FileFlags flags;
    /// @brief POSIX time representing when file was created.
    time_t create;
    /// @brief POSIX time representing when file was last modified.
    time_t modify;
    /// @brief Size of file.
    /// @details Updates when file is written to or truncated but it's better to use file_query_time.
    size_t size;
} File;
typedef enum {
    FIND_FLAG_INCLUDE_HIDDEN    = (1 << 0),
    FIND_FLAG_INCLUDE_DIRECTORY = (1 << 1),
    FIND_FLAG_APPEND            = (1 << 2),
} FileFindFlags;
/// @brief Result of functions related to finding files.
typedef struct FindFiles {
    size_t         count; // duplicate of darray_len( paths )
    StringSlice*   paths;
    DynamicString* buffer;
} FindFiles;
/// @brief ID of process.
typedef struct ProcessID {
#if defined(_WIN32)
    /// @brief Platform handle to process.
    void* handle;
#else /* Windows */
    /// @brief Platform handle to process.
    pid_t handle;
#endif /* POSIX */
} ProcessID;
/// @brief Pipe for interprocess communication.
typedef struct Pipe {
#if defined(_WIN32)
    /// @brief Platform handle to a pipe.
    void* handle;
#else
    /// @brief Platform handle to a pipe.
    int handle;
#endif
} Pipe;
/// @brief Mutex for locking resources.
typedef struct Mutex {
    /// @brief Platform handle to a mutex.
    void* handle;
} Mutex;
/// @brief Semaphore for multi-threading.
typedef struct Semaphore {
    /// @brief Platform handle to a semaphore.
    void* handle;
} Semaphore;
/// @brief Enumerator describing a compiler.
typedef enum {
    /// @brief Compiler is unknown.
    COMPILER_UNKNOWN = 0,
    /// @brief Compiler is GCC.
    COMPILER_GCC     = 1,
    /// @brief Compiler is clang(LLVM).
    COMPILER_CLANG   = 2,
    /// @brief Compiler is Microsoft Visual C.
    COMPILER_MSVC    = 3,
} Compiler;
/// @brief Enumerator describing a platform.
typedef enum {
    /// @brief Platform is unknown.
    PLATFORM_UNKNOWN = 0,
    /// @brief Platform is GNU/Linux.
    PLATFORM_LINUX   = 1,
    /// @brief Platform is MacOS.
    PLATFORM_MACOS   = 2,
    /// @brief Platform is Windows.
    PLATFORM_WINDOWS = 3,
} Platform;
/// @brief Enumerator describing a CPU architecture.
typedef enum {
    /// @brief Architecture is unknown.
    ARCH_UNKNOWN = 0,
    /// @brief Architecture is ARM based.
    ARCH_ARM     = 1,
    /// @brief Architecture is x86 based.
    ARCH_X86     = 2,
} Architecture;
/// @brief Function prototype for sorting functions.
/// @param     stride Size of @c a and @c b.
/// @param[in] a, b   Constant pointers to items to be compared.
/// @return
///     - @c True:  @c a will be placed before @c b.
///     - @c False: @c b will be placed before @c a. 
typedef bool_t SortCmpFN( size_t stride, const void* a, const void* b );
/// @brief Function prototype for filtering items out of an array.
/// @param     index  Index of the current item.
/// @param     stride Size of the current item.
/// @param[in] item   Constant pointer to item to be checked for filtering.
/// @param[in] params (optional) Pointer to optional parameters.
/// @return
///     - @c True:  @c item passed filter test.
///     - @c False: @c item did not pass filter test and will be filtered out.
typedef bool_t FilterFN(
    size_t index, size_t stride, const void* item, void* params );
/// @brief Function prototype for multi-threading job.
/// @param     thread_id ID of the current thread. Useful for TLS, other global data or logging.
/// @param[in] params    (optional) Pointer to optional parameters.
typedef void JobFN( uint32_t thread_id, void* params );

// NOTE(alicia): end: Types

// TODO(alicia): begin: get rid of these

typedef int FileTimeCmp;
#define FILE_TIME_CMP_EQUAL           0
#define FILE_TIME_CMP_LEFT_IS_NEWER  -1
#define FILE_TIME_CMP_RIGHT_IS_NEWER  1
#define FILE_TIME_CMP_ERROR           2

typedef unsigned int FileTimeCmpWhich;
#define FILE_TIME_CMP_MODIFY 0
#define FILE_TIME_CMP_CREATE 1

// TODO(alicia): end

// NOTE(alicia): begin: Miscellaneous Macros

/// @brief Calculate length of static array.
/// @param[in] array (any[]) Constant compile-time array.
/// @return (size_t) Number of items array is capable of holding.
#define static_array_len( array )\
    (sizeof((array)) / sizeof((array)[0]))

static inline void _0(int a,...) {(void)a;}
/// @brief Mark variables/parameters as unused.
/// @param ... (any) Items to mark as unused.
#define unused( ... ) _0( 0, __VA_ARGS__ )

/// @brief Convert kilobytes to bytes.
/// @param kb (size_t) Kilobytes.
/// @return (size_t) @c kb as bytes.
#define kilobytes( kb ) (kb * 1000ULL)
/// @brief Convert megabytes to bytes.
/// @param mb (size_t) Megabytes.
/// @return (size_t) @c mb as bytes.
#define megabytes( mb ) (kilobytes(mb) * 1000ULL)
/// @brief Convert gigabytes to bytes.
/// @param gb (size_t) Gigabytes.
/// @return (size_t) @c gb as bytes.
#define gigabytes( gb ) (megabytes(gb) * 1000ULL)
/// @brief Convert Terabytes to bytes.
/// @param tb (size_t) Terabytes.
/// @return (size_t) @c tb as bytes.
#define terabytes( tb ) (gigabytes(tb) * 1000ULL)
/// @brief Convert kibibytes to bytes.
/// @param kb (size_t) Kibibytes.
/// @return (size_t) @c kb as bytes.
#define kibibytes( kb ) (kb * 1024ULL)
/// @brief Convert mebibytes to bytes.
/// @param mb (size_t) Mebibytes.
/// @return (size_t) @c mb as bytes.
#define mebibytes( mb ) (kibibytes(mb) * 1024ULL)
/// @brief Convert gibibytes to bytes.
/// @param gb (size_t) Gibibytes.
/// @return (size_t) @c gb as bytes.
#define gibibytes( gb ) (mebibytes(gb) * 1024ULL)
/// @brief Convert tebibytes to bytes.
/// @param tb (size_t) Tebibytes.
/// @return (size_t) @c tb as bytes.
#define tebibytes( tb ) (gibibytes(tb) * 1024ULL)

// NOTE(alicia): end: Miscellaneous Macros

// NOTE(alicia): begin: Constants

/// @brief Maximum number of threads allowed.
/// @warning Do not redefine this.
#define MAX_THREADS (16)
#if !defined(THREAD_COUNT)
    /// @brief Number of threads cbuild is allowed to create.
    #define THREAD_COUNT (8)
#else
    #if THREAD_COUNT > MAX_THREADS
        /// @brief Number of threads cbuild is allowed to create.
        #define THREAD_COUNT MAX_THREADS
    #elif THREAD_COUNT <= 0
        /// @brief Number of threads cbuild is allowed to create.
        #define THREAD_COUNT (1)
    #endif
#endif

#if defined(__clang__)
    /// @brief Compiler used to compile cbuild.
    /// @details Current: clang (LLVM)
    #define COMPILER_CURRENT (COMPILER_CLANG)
#elif defined(__GNUC__) /* __clang__ is defined */
    /// @brief Compiler used to compile cbuild.
    /// @details Current: gcc
    #define COMPILER_CURRENT (COMPILER_GCC)
#elif defined(_MSC_VER) /* __GNUC__ is defined */
    /// @brief Compiler used to compile cbuild.
    /// @details Current: cl (MSVC)
    #define COMPILER_CURRENT (COMPILER_MSVC)
#else /* _MSC_VER is defined */
    /// @brief Compiler used to compile cbuild.
    /// @details Current: cc (Unknown)
    #define COMPILER_CURRENT (COMPILER_UNKNOWN)
#endif /* No compiler is defined */
#if defined(_WIN32)
    /// @brief Platform where cbuild was compiled.
    /// @details Current: Windows.
    #define PLATFORM_CURRENT  (PLATFORM_WINDOWS)
    /// @brief Is platform POSIX-compliant? (bool_t)
    /// @details Current: false
    #define PLATFORM_IS_POSIX (false)
#elif defined(__linux__) /* _WIN32 is defined */
    /// @brief Platform where cbuild was compiled.
    /// @details Current: GNU/Linux.
    #define PLATFORM_CURRENT  (PLATFORM_LINUX)
    /// @brief Is platform POSIX-compliant? (bool_t)
    /// @details Current: true
    #define PLATFORM_IS_POSIX (true)
#elif defined(__APPLE__) /* __linux__ is defined */
    /// @brief Platform where cbuild was compiled.
    /// @details Current: MacOS.
    #define PLATFORM_CURRENT  (PLATFORM_MACOS)
    /// @brief Is platform POSIX-compliant? (bool_t)
    /// @details Current: true
    #define PLATFORM_IS_POSIX (true)
#else /* __APPLE__ is defined */
    /// @brief Platform where cbuild was compiled.
    /// @details Current: Unknown.
    #define PLATFORM_CURRENT  (PLATFORM_UNKNOWN)
    /// @brief Is platform POSIX-compliant? (bool_t)
    /// @details Current: true
    #define PLATFORM_IS_POSIX (true)
#endif /* No platform is defined */
#if defined(__MINGW32__)
    /// @brief Is platform MINGW? (bool_t)
    /// @details Current: true
    #define PLATFORM_IS_MINGW (true)
#else /* __MINGW32__ defined */
    /// @brief Is platform MINGW? (bool_t)
    /// @details Current: false
    #define PLATFORM_IS_MINGW (false)
#endif /* __MINGW32__ not defined */
#if defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64)
    /// @brief Architecture were cbuild was compiled.
    /// @details Current: ARM
    #define ARCH_CURRENT (ARCH_ARM)
#elif defined(__i386__) || defined(_M_IX86) || defined(__X86__) || defined(__x86_64__) /* Arm is defined */
    /// @brief Architecture were cbuild was compiled.
    /// @details Current: x86
    #define ARCH_CURRENT (ARCH_X86)
#else /* x86 is defined */
    /// @brief Architecture were cbuild was compiled.
    /// @details Current: Unknown
    #define ARCH_CURRENT (ARCH_UNKNOWN)
#endif /* No architecture is defined */
#if UINT64_MAX == UINTPTR_MAX
    /// @brief Is architecture 64-bit? (bool_t)
    /// @details Current: true
    #define ARCH_IS_64BIT (true)
#else /* UINT64_MAX == UINTPTR_MAX */
    /// @brief Is architecture 64-bit? (bool_t)
    /// @details Current: false
    #define ARCH_IS_64BIT (false)
#endif /* UINT64_MAX != UINTPTR_MAX */

/// @brief Maximum allowed path length.
#define PATH_MAX_LEN (4096)

// NOTE(alicia): end: Constants

// NOTE(alicia): begin: Functions

/// @brief Query command line name of compiler used to compile cbuild.
/// @return C String (never null) of compiler used to compile cbuild.
const char* cbuild_compiler_string(void);
/// @brief Query command line used to compile cbuild.
/// @return C String (never null) of command line used to compile cbuild.
const char* cbuild_command_line(void);
/// @brief Query time that cbuild was compiled at.
/// @return C String (never null) of time cbuild was compiled at.
const char* cbuild_compile_time(void);

void _init_( const char* argv0, const char* file, LogLevel log );
/// @brief Initialize cbuild. Call this before using any library functions.
/// @param log_level (LogLevel) Level to set logger to.
#define init( log_level )\
    _init_( argv[0], __FILE__, log_level )
/// @brief Set logging level.
/// @param level Logging level to set.
void logger_level_set( LogLevel level );
/// @brief Get current logging level.
/// @return Current logging level.
LogLevel logger_level_query(void);
/// @brief Write logging message.
/// @details
/// @c level Determines whether message gets sent to stdout or stderr,
/// color of logging message and whether it gets printed at all
/// (that's also based on the current logging level).
/// @param     level  Logging level of message.
/// @param[in] format Constant null-terminated UTF-8 formatting string (C-style formatting).
/// @param     va     Variadic list for formatting message.
void logger_va( LogLevel level, const char* format, va_list va );
/// @brief Write logging message.
/// @details
/// @c level Determines whether message gets sent to stdout or stderr,
/// color of logging message and whether it gets printed at all
/// (that's also based on the current logging level).
/// @param     level  Logging level of message.
/// @param[in] format Constant null-terminated UTF-8 formatting string (C-style formatting).
/// @param     ...    Variadic arguments for formatting message.
void logger( LogLevel level, const char* format, ... );
/// @brief Write info message.
/// @details
/// Written to stdout without coloring.
/// Only printed if logging level is set to LOG_LEVEL_INFO.
/// @param ... (format, ...)
///     @c format: Constant null-terminated UTF-8
///         formatting string (C-style formatting).
///     @c ...:    Variadic arguments for formatting message.
#define cb_info( ... )  logger( LOG_LEVEL_INFO, __VA_ARGS__ )
/// @brief Write warning message.
/// @details
/// Written to stdout with yellow color.
/// Only printed if logging level is set to LOG_LEVEL_INFO or LOG_LEVEL_WARNING.
/// @param ... (format, ...)
///     @c format: Constant null-terminated UTF-8
///         formatting string (C-style formatting).
///     @c ...:    Variadic arguments for formatting message.
#define cb_warn( ... )  logger( LOG_LEVEL_WARNING, __VA_ARGS__ )
/// @brief Write error message.
/// @details
/// Written to stderr with red color.
/// Always printed.
/// @param ... (format, ...)
///     @c format: Constant null-terminated UTF-8
///         formatting string (C-style formatting).
///     @c ...:    Variadic arguments for formatting message.
#define cb_error( ... ) logger( LOG_LEVEL_ERROR, __VA_ARGS__ )

/// @brief Allocate memory on the heap. Memory returned is always zeroed.
/// @details
/// Never free or realloc using C standard library functions.
/// @param size Size of allocation in bytes.
/// @return
///     - @c NULL: If allocation failed.
///     - Pointer: Pointer to allocated buffer if allocation succeeded.
void* memory_alloc( size_t size );
/// @brief Reallocate memory on the heap. New memory returned is always zeroed.
/// @details
/// Only use memory returned from memory_realloc or memory_free.
/// Never free or realloc using C standard library functions.
/// @param[in] memory   Pointer to start of buffer allocated with memory_alloc.
/// @param     old_size Size of buffer before reallocating. Must be less than @c new_size.
/// @param     new_size Size of buffer after reallocating. Must be greater than @c old_size.
/// @return
///     - @c NULL: If reallocation failed.
///     - Pointer: Pointer to buffer if reallocation succeeded.
void* memory_realloc( void* memory, size_t old_size, size_t new_size );
/// @brief Free memory allocated on the heap.
/// @details
/// Only use memory returned from memory_realloc or memory_free.
/// @param[in] memory Pointer to start of buffer allocated with memory_alloc.
/// @param     size   Size of allocated buffer.
void memory_free( void* memory, size_t size );
/// @brief Query bytes currently allocated on the heap.
/// @return Bytes currently allocated.
size_t memory_query_allocated(void);
/// @brief Query bytes allocated on the heap over the entire run of the program.
/// @return Bytes allocatd.
size_t memory_query_total_allocated(void);
/// @brief Set range of memory to given value.
/// @param[in] memory Pointer to start of memory range.
/// @param     value  Value to set bytes in memory range to.
/// @param     size   Size of memory range to set.
void memory_set( void* memory, int8_t value, size_t size );
/// @brief Set range of memory to zero.
/// @param[in] memory Pointer to start of memory range.
/// @param     size   Size of memory range to set.
void memory_zero( void* memory, size_t size );
/// @brief Copy range of memory to equally sized range.
///     Memory ranges must not overlap.
/// @param[in] dst  Pointer to start of destination memory range.
/// @param[in] src  Constant pointer to start of source memory range.
/// @param     size Size of memory ranges.
void memory_copy( void* restrict dst, const void* restrict src, size_t size );
/// @brief Copy range of memory to equally sized range. Memory ranges can be overlapping each other.
/// @param[in] dst  Pointer to start of destination memory range.
/// @param[in] src  Constant pointer to start of source memory range.
/// @param     size Size of memory ranges.
void memory_move( void* dst, const void* src, size_t size );
/// @brief Compare two memory ranges for equality.
/// @param[in] a, b Constant pointers to equally sized memory ranges.
/// @param     size Size of memory ranges to compare.
/// @return
///     - @c True:  If memory ranges are identical.
///     - @c False: If memory ranges differ.
bool_t memory_cmp( const void* a, const void* b, size_t size );
/// @brief Calculate ascii length of null-terminated string.
/// @param[in] string (optional) Pointer to start of null-terminated ascii string.
/// @return Length of ascii null-terminated string.
size_t cstr_len( const char* string );
/// @brief Calculate UTF-8 length of null-terminated string.
/// @details
/// This function does NOT check that string is valid UTF-8.
/// @param[in] string (optional) Constant pointer to start of null-terminated UTF-8 string.
/// @return Length of UTF-8 null-terminated string.
size_t cstr_len_utf8( const char* string );
/// @brief Compare two null-terminated strings for equality.
/// @param[in] a, b Constant pointers to strings to compare.
/// @return
///     - @c True:  If @c a and @c b are identical.
///     - @c False: If @c a and @c b differ.
bool_t cstr_cmp( const char* a, const char* b );
/// @brief Find ascii character in null-terminated string.
/// @param[in]  string      Constant pointer to null-terminated string to search in.
/// @param      c           Ascii character to search for.
/// @param[out] opt_out_pos (optional) Pointer to write index of character to. (if it was found)
/// @return
///     - @c True: If @c c was found in @c string. If @c opt_out_pos is not NULL, index of @c c is written to it.
///     - @c False: @c c was not found in @c string.
bool_t cstr_find( const char* string, char c, size_t* opt_out_pos );
/// @brief Find last matching ascii character in null-terminated string.
/// @param[in]  string      Constant pointer to null-terminated string to search in.
/// @param      c           Ascii character to search for.
/// @param[out] opt_out_pos (optional) Pointer to write index of character. (if it was found)
/// @return
///     - @c True: If @c c was found in @c string. If @c opt_out_pos is not NULL, index of @c c is written to it.
///     - @c False: @c c was not found in @c string.
bool_t cstr_find_last( const char* string, char c, size_t* opt_out_pos );
/// @brief Find set of ascii characters in null-terminated string.
/// @param[in]  string      Constant pointer to null-terminated string to search in.
/// @param[in]  set         Constant pointer to null-terminated string containg unique ascii characters to search for.
/// @param[out] opt_out_pos (optional) Pointer to write index of first character in set found. (if any were found)
/// @return
///     - @c True:  If any characters in @c set were found in @c string.
///     - @c False: If no characters in @c set were found in @c string.
bool_t cstr_find_set( const char* string, const char* set, size_t* opt_out_pos );
/// @brief Find substring in null-terminated string.
/// @param[in]  string      Constant pointer to null-terminated string to search in.
/// @param[in]  substr      Constant pointer to null-terminated string to search for.
/// @param[out] opt_out_pos (optional) Pointer to write index of first character in @c substr found in @c string. (if substring was found)
/// @return
///     - @c True:  If @c substr was found in @c string.
///     - @c False: If @c substr was not found in @c string.
bool_t cstr_find_cstr(
    const char* string, const char* substr, size_t* opt_out_pos );

/// @brief Create empty string slice.
/// @return (StringSlice) Empty slice.
#define string_slice_empty()\
    (StringSlice){ .cc=0, .len=0 }
/// @brief Create string slice from string literal.
/// @param literal (string literal) String literal.
/// @return (StringSlice) String slice.
#define string_slice_text( literal )\
    (StringSlice){ .cc=literal, .len=sizeof(literal) - 1 }
/// @brief Create string slice from length and C string.
/// @param opt_len (optional) Length of string. If zero, calculates length of string using cstr_len.
/// @param string (const char*) Constant pointer to string.
/// @return (StringSlice) String slice.
#define string_slice_new( opt_len, string )\
    (StringSlice){ .cc=string, .len=opt_len ? opt_len : cstr_len( string ) }
/// @brief Create string slice from dynamic string.
/// @param dstring (DynamicString*) Pointer to dynamic string array.
/// @return (StringSlice) String slice.
#define string_slice_from_dstring( dstring )\
    (StringSlice){ .cc=dstring, .len=dstring_len( dstring ) }
/// @brief Check if string slice is empty.
/// @param string String slice to check.
/// @return
///     - @c True:  @c string's length is not zero.
///     - @c False: @c string's length is zero.
bool_t string_slice_is_empty( StringSlice string );
/// @brief Check if string slice points to a null-terminated string.
/// @param string String slice to check.
/// @return
///     - @c True:  @c string is null-terminated.
///     - @c False: @c string is not null-terminated.
bool_t string_slice_is_null_terminated( StringSlice string );
/// @brief Compare two string slices for equality.
/// @param a, b String slices to compare.
/// @return
///     - @c True:  Contents of @c a and @c b are equal and they have matching lengths.
///     - @c False: Contents of @c a and @c b do not match or their lengths are different.
bool_t string_slice_cmp( StringSlice a, StringSlice b );
/// @brief Compare two string slice for equality. Compares up to shortest length.
/// @details
/// If either string is empty, returns false.
/// @param a, b String slices to compare.
/// @return
///     - @c True:  Contents of @c a and @c b match up to shortest length.
///     - @c False: Contents of @c a and @c b do not match.
bool_t string_slice_cmp_clamped( StringSlice a, StringSlice b );
/// @brief Find ascii character in string slice.
/// @param      string      String slice to search in.
/// @param      c           Character to search for.
/// @param[out] opt_out_pos (optional) Pointer to write position to if @c c is found.
/// @return
///     - @c True:  Character @c c was found in @c string.
///     - @c False: Character @c c was not found in @c string.
bool_t string_slice_find( StringSlice string, char c, size_t* opt_out_pos );
/// @brief Find ascii character in string slice. Searches from end of slice.
/// @param      string      String slice to search in.
/// @param      c           Character to search for.
/// @param[out] opt_out_pos (optional) Pointer to write position to if @c c is found.
/// @return
///     - @c True:  Character @c c was found in @c string.
///     - @c False: Character @c c was not found in @c string.
bool_t string_slice_find_last( StringSlice string, char c, size_t* opt_out_pos );
/// @brief Search for any ascii character in set in string slice.
/// @param      string      String slice to search in.
/// @param      set         Set of characters to search for.
/// @param[out] opt_out_pos (optional) Pointer to write position of first character in set if any are found.
/// @return
///     - @c True:  Any character in @c set was found in @c string.
///     - @c False: No character in @c set was found in @c string.
bool_t string_slice_find_set( StringSlice string, StringSlice set, size_t* opt_out_pos );
/// @brief Search for any ascii character in set in string slice. Searches from end of slice.
/// @param      string      String slice to search in.
/// @param      set         Set of characters to search for.
/// @param[out] opt_out_pos (optional) Pointer to write position of first character in set if any are found.
/// @return
///     - @c True:  Any character in @c set was found in @c string.
///     - @c False: No character in @c set was found in @c string.
bool_t string_slice_find_last_set(
    StringSlice string, StringSlice set, size_t* opt_out_pos );
/// @brief Search for substring in string slice.
/// @param      source      String slice to search in.
/// @param      substring   Substring to search for.
/// @param[out] opt_out_pos (optional) Pointer to write position of first character in substring found in source string.
/// @return
///     - @c True:  @c substring was found in @c source string.
///     - @c False: @c substring was not found in @c source string.
bool_t string_slice_find_string_slice(
    StringSlice source, StringSlice substring, size_t* opt_out_pos );
/// @brief Get pointer to first character in string slice.
/// @param string String to get first character from.
/// @return
///     - @c NULL:    @c string is empty.
///     - @c Pointer: Pointer to first character in @c string.
char* string_slice_first( StringSlice string );
/// @brief Get pointer to last character in string slice.
/// @param string String to get last character from.
/// @return
///     - @c NULL:    @c string is empty.
///     - @c Pointer: Pointer to last character in @c string.
char* string_slice_last( StringSlice string );
/// @brief Move string slice up by one character and shorten by one character.
/// @param string String slice to advance.
/// @return
///     - @c Empty slice: Reached the end of @c string.
///     - @c Slice: Shortened slice.
StringSlice string_slice_advance( StringSlice string );
/// @brief Move string slice up by n characters and shorten by n characters.
/// @param string String slice to advance.
/// @param amount Amount to advance by. If greater than @c string.len, returns empty string.
/// @return
///     - @c Empty slice: Reached the end of @c string.
///     - @c Slice: Shortened slice.
StringSlice string_slice_advance_by( StringSlice string, size_t amount );
/// @brief Shorten length of string slice.
/// @param string String slice to shorten.
/// @param max    Maximum length.
/// @return
///     - @c Empty slice:     @c max was zero.
///     - @c Same slice:      @c max was greater than @c string.len.
///     - @c Shortened slice: @c string length is set to @c max.
StringSlice string_slice_truncate( StringSlice string, size_t max );
/// @brief Subtract from string slice's length.
/// @param string String slice to shorten.
/// @param amount Amount of characters to shorten slice by.
/// @return
///     - @c Empty slice:     @c amount is greater than or equals to @c string.len
///     - @c Shortened slice: @c amount is less than @c string.len
StringSlice string_slice_trim( StringSlice string, size_t amount );
/// @brief Remove leading whitespace from string slice.
/// @param string String to remove leading whitespace from.
/// @return Trimmed string slice. Empty if entire string was whitespace.
StringSlice string_slice_trim_ws_lead( StringSlice string );
/// @brief Remove trailing whitespace from string slice.
/// @param string String to remove trailing whitespace from.
/// @return Trimmed string slice. Empty if entire string was whitespace.
StringSlice string_slice_trim_ws_trail( StringSlice string );
/// @brief Remove leading and trailing whitespace from string slice.
/// @param string String to remove whitespace from.
/// @return Trimmed string slice. Empty if entire string was whitespace.
StringSlice string_slice_trim_ws( StringSlice string );
/// @brief Calculate UTF-8 length of string slice.
/// @details
/// This function does NOT check that string is valid UTF-8.
/// @param string String to calculate length of.
/// @return UTF-8 length of string.
size_t string_slice_len_utf8( StringSlice string );
StringSlice* string_slice_darray_from_arg( int argc, char** argv );
StringSlice* string_slice_darray_from_delim( StringSlice source, StringSlice delim );

const char* local_fmt( const char* format, ... );
const char* local_fmt_va( const char* format, va_list va );
const char* local_fmt_bytesize( size_t bytes );

DynamicString* dstring_empty( size_t size );
DynamicString* dstring_new( size_t opt_len, const char* source );
#define  dstring_literal( literal ) dstring_new( sizeof(literal) - 1, literal )
#define  dstring_from_string_slice( substr ) dstring_new( (substr)->len, (substr)->cc )
DynamicString* dstring_concat_array_string_slice( size_t len, StringSlice* strings, StringSlice sep );
DynamicString* dstring_concat_array( size_t len, const char** strings, const char* sep );
DynamicString* dstring_fmt_va( const char* format, va_list va );
DynamicString* dstring_fmt( const char* format, ... );
DynamicString* dstring_clone( const DynamicString* source );
void     dstring_free( DynamicString* string );
void*    dstring_head( DynamicString* string );
DynamicString* dstring_grow( DynamicString* string, size_t amount );
void     dstring_clear( DynamicString* string );
void     dstring_truncate( DynamicString* string, size_t max_len );
size_t   dstring_len( const DynamicString* string );
size_t   dstring_cap( const DynamicString* string );
size_t   dstring_remaining( const DynamicString* string );
bool_t    dstring_is_empty( const DynamicString* string );
bool_t    dstring_is_full( const DynamicString* string );
DynamicString* dstring_concat( const DynamicString* left, const DynamicString* right );
DynamicString* dstring_concat_string_slice( StringSlice left, StringSlice right );
DynamicString* dstring_concat_str( const char* left, const char* right );
DynamicString* dstring_prepend( DynamicString* string, const DynamicString* prepend );
DynamicString* dstring_prepend_string_slice( DynamicString* string, StringSlice prepend );
DynamicString* dstring_prepend_str( DynamicString* string, const char* prepend );
DynamicString* dstring_append( DynamicString* string, const DynamicString* append );
DynamicString* dstring_append_string_slice( DynamicString* string, StringSlice append );
DynamicString* dstring_append_str( DynamicString* string, const char* append );
DynamicString* dstring_insert( DynamicString* string, const DynamicString* insert, size_t index );
DynamicString* dstring_insert_string_slice( DynamicString* string, StringSlice insert, size_t index );
DynamicString* dstring_insert_str( DynamicString* string, const char* insert, size_t index );
DynamicString* dstring_push( DynamicString* string, char c );
bool_t    dstring_pop( DynamicString* string, char* opt_out_c );
bool_t    dstring_remove( DynamicString* string, size_t index );
DynamicString* dstring_emplace( DynamicString* string, char c, size_t index );

void array_sort( SortCmpFN* cmp, size_t stride, size_t len, void* array );
void swap_generic( size_t stride, void* a, void* b, void* swap );

void*  darray_empty_generic( size_t stride, size_t cap );
void*  darray_new_generic( size_t stride, size_t len, const void* source );
void*  darray_grow( void* darray, size_t amount );
void*  darray_clone( void* darray );
void   darray_free( void* darray );
void*  darray_head( void* darray );
void   darray_set_len( void* darray, size_t len );
size_t darray_len( const void* darray );
size_t darray_cap( const void* darray );
size_t darray_stride( const void* darray );
size_t darray_remaining( const void* darray );
bool_t  darray_is_empty( const void* darray );
bool_t  darray_is_full( const void* darray );
void   darray_truncate( void* darray, size_t max );
void   darray_clear( void* darray );
void*  darray_filter( void* darray, FilterFN* filter, void* filter_params );
void*  darray_filter_self( void* darray, FilterFN* filter, void* filter_params );
void*  darray_concat( const void* left, const void* right );
void*  darray_concat_arrays(
    size_t stride, size_t left_len, const void* left,
    size_t right_len, const void* right );
void*  darray_split( void* darray, size_t at );
void*  darray_extend( void* darray, size_t count, const void* extend );
void*  darray_get( void* darray, size_t index );
void*  darray_push( void* darray, const void* item );
bool_t  darray_push_try( void* darray, const void* item );
void*  darray_pop( void* darray );
void*  darray_insert( void* darray, size_t index, const void* item );
bool_t  darray_remove( void* darray, size_t at, void* opt_out_item );
#define darray_empty( type, cap )\
    (type*)darray_empty_generic( sizeof(type), cap )
#define darray_new( type, ... )\
    (type*)darray_new_generic( sizeof(type),\
        static_array_len((type[]){__VA_ARGS__}), (type[]){__VA_ARGS__} )
#define darray_from_array( array )\
    darray_new_generic( sizeof((array)[0]), static_array_len( (array) ), array )

#if defined(_MSC_VER)
    void memory_fence(void);
#else
    #define memory_fence()\
        __sync_synchronize()        
#endif

uint32_t atomic_add( Atom* atom, uint32_t val );
uint32_t atomic_sub( Atom* atom, uint32_t val );
uint64_t atomic_add64( Atom64* atom, uint64_t val );
uint64_t atomic_sub64( Atom64* atom, uint64_t val );

void  thread_sleep( uint32_t ms );

#if COMPILER_CURRENT == COMPILER_MSVC
    #define mutex_null() {0} 
#else
    #define mutex_null() (Mutex){ .handle=0 }
#endif
bool_t mutex_create( Mutex* out_mtx );
void  mutex_destroy( Mutex* mtx );
bool_t mutex_lock( Mutex* mtx, uint32_t ms );
void  mutex_unlock( Mutex* mtx );

#define semaphore_null()  (Semaphore){ .handle=0 }
bool_t semaphore_create( Semaphore* out_sem );
void  semaphore_destroy( Semaphore* sem );
bool_t semaphore_wait( Semaphore* sem, uint32_t ms );
void  semaphore_signal( Semaphore* sem );

bool_t job_enqueue( JobFN* job, void* params, uint32_t ms );
bool_t job_wait( uint32_t ms );
bool_t job_wait_all( uint32_t ms );

#define pipe_null()       (Pipe){ .handle=0 }
bool_t    pipe_create( Pipe* out_read, Pipe* out_write );
uint32_t pipe_peek( Pipe* rpipe );
bool_t    pipe_read(
    Pipe* rpipe, uint32_t buf_size, void* buf, uint32_t* out_read_size );
bool_t    pipe_write(
    Pipe* wpipe, uint32_t buf_size, const void* buf, uint32_t* out_write_size );
bool_t    pipe_write_fmt_va( Pipe* wpipe, const char* format, va_list va );
bool_t    pipe_write_fmt( Pipe* wpipe, const char* format, ... );
void     pipe_close( Pipe* pipe );
bool_t    pipe_check( Pipe* pipe );

#define pid_null() (ProcessID){ .handle=0 }
bool_t program_in_path( const char* prog );
bool_t program_exec( const char* command_line, int* opt_out_res );
bool_t program_exec_async(
    const char* command_line,
    Pipe* opt_stdin_r, Pipe* opt_stdout_w, Pipe* opt_stderr_w,
    ProcessID* out_pid );
bool_t program_wait( ProcessID pid, uint32_t ms, int* opt_out_res );
void  program_terminate( ProcessID pid );
void  program_close( ProcessID pid );

double time_seconds(void);
double time_milliseconds(void);

DynamicString*   path_push_chunk( DynamicString* path, const char* chunk );
DynamicString*   path_push_chunk_string_slice( DynamicString* path, StringSlice chunk );
bool_t      path_peek_chunk(
    const char* path, size_t* out_chunk_len, const char** out_chunk );
bool_t      path_peek_chunk_string_slice( StringSlice path, StringSlice* out_chunk );
bool_t      path_pop_chunk(
    DynamicString* path, size_t opt_chunk_size, char* opt_out_chunk );
bool_t      path_file_name( StringSlice path, StringSlice* out_chunk );
StringSlice* path_split_chunks_string_slice( StringSlice path );
StringSlice* path_split_chunks( const char* path );
bool_t      path_glob_match_string_slice( StringSlice path, StringSlice glob );
bool_t      path_glob_match( const char* path, const char* glob );

bool_t path_find_files( const char* path, FindFiles* out_result );
bool_t path_find_files_string_slice( StringSlice path, FindFiles* out_result );
void  path_find_files_free( FindFiles* ff );

#define file_null()\
    (File){ .handle=0, .type=FT_UNKNOWN, .create=0, .modify=0, .size=0 }
bool_t file_open( const char* path, FileFlags flags, File* out_file );
bool_t file_write( File* file, size_t size, const void* buf );
bool_t file_write_fmt_va( File* file, const char* format, va_list va );
bool_t file_write_fmt( File* file, const char* format, ... );
bool_t file_read( File* file, size_t size, void* buf );
void   file_close( File* file );
bool_t file_check( File* file );
bool_t file_truncate( File* file );
bool_t file_seek( File* file, FileSeek type, ssize_t seek );
bool_t file_query_seek( File* file, ssize_t* out_seek );
bool_t file_path_exists( const char* path );
bool_t file_path_query_create( const char* path, time_t* out_time );
bool_t file_path_query_modify( const char* path, time_t* out_time );
bool_t file_path_move( const char* dst, const char* src );
bool_t file_path_copy( const char* dst, const char* src );
bool_t file_path_remove( const char* path );

bool_t file_find_string_slice(
    StringSlice pattern, FileFindFlags flags, DynamicString** out_result );

// TODO(alicia): rewrite
FileTimeCmp file_path_time_cmp(
    const char* lhs, const char* rhs, FileTimeCmpWhich which );
FileTimeCmp file_path_time_cmp_multi(
    const char* lhs, size_t count, const char** rhs, FileTimeCmpWhich which );
FileTimeCmp file_path_time_cmp_multi_string_slice(
    const char* lhs, size_t count, StringSlice* rhs, FileTimeCmpWhich which );

// NOTE(alicia): end: Functions

#endif /* Header Guard */

#if defined(CBUILD_IMPLEMENTATION) || defined(_CLANGD)
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>

// CBUILD_DEBUG

#if !defined(CBUILD_COMPILE_ARGS)
    #define CBUILD_COMPILE_ARGS ""
#endif

#if !defined(COMPILER) || !defined(COMPILER_BUILD_LINE) || !defined(COMPILER_CFLAGS) || !defined(COMPILER_ADDITIONAL_FLAGS)
#if defined(__clang__)
    #define COMPILER            "clang"
    #define COMPILER_BUILD_LINE " %s -o %s "
    #if defined(CBUILD_DEBUG)
        #if defined(__MINGW32_MAJOR_VERSION)
            #define COMPILER_CFLAGS\
                " -O0 -g -gcodeview -fuse-ld=lld -Wl,/debug "
        #else
            #define COMPILER_CFLAGS " -O0 -g"
        #endif
    #else
        #define COMPILER_CFLAGS " -O2 "
    #endif
    #if defined(_WIN32)
        #if defined(__MINGW32__)
            #define COMPILER_ADDITIONAL_FLAGS "-femulated-tls"
        #endif
    #else
        #define COMPILER_ADDITIONAL_FLAGS\
            " -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=199309L -lpthread"
    #endif
#elif defined(__GNUC__)
    #define COMPILER "gcc"
    #define COMPILER_BUILD_LINE " %s -o %s "
    #if defined(CBUILD_DEBUG)
        #define COMPILER_CFLAGS " -O0 -g "
    #else
        #define COMPILER_CFLAGS " -O2 "
    #endif
    #if !defined(_WIN32)
        #define COMPILER_ADDITIONAL_FLAGS\
            " -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=199309L -lpthread"
    #endif
#elif defined(_MSC_VER)
    #define COMPILER "cl"
    #define COMPILER_BUILD_LINE " %s /Fe:%s /nologo "
    #if defined(CBUILD_DEBUG)
        #define COMPILER_CFLAGS " /Zi "
    #else
        #define COMPILER_CFLAGS " /O2 "
    #endif
#else
    #define COMPILER "cc"
    #define COMPILER_BUILD_LINE " %s -o %s "
    #if defined(CBUILD_DEBUG)
        #define COMPILER_CFLAGS " -O0 -g "
    #else
        #define COMPILER_CFLAGS " -O2 "
    #endif
    #if !defined(_WIN32)
        #define COMPILER_ADDITIONAL_FLAGS " -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=199309L -lpthread"
    #endif
#endif
#endif /* redefine self */

#if !defined(COMPILER_ADDITIONAL_FLAGS)
    #define COMPILER_ADDITIONAL_FLAGS ""
#endif

typedef struct {
    size_t len, cap;
    char   buf[];
} dstring_header;

typedef struct {
    size_t len, cap, stride;
    char   buf[];
} darray_header;

#define MAX_GLOBAL_STRING_SIZE  (1024)
#define MAX_GLOBAL_STRING_COUNT (16)
static char global_string_buffers[MAX_GLOBAL_STRING_COUNT][MAX_GLOBAL_STRING_SIZE];
Atom global_string_buffer_index = 0;

static const char* global_argv0        = NULL;
static const char* global_compiler     = "";
static const char* global_command_line = "";
static const char* global_compile_time = "";

static LogLevel global_log_level = LOG_LEVEL_INFO;

bool_t internal_thread_create( JobFN* proc, void* params );

static Mutex global_log_mtx = mutex_null();

// NOTE(alicia): C-stdlib implementation

static bool_t file_flags_valid( FileFlags flags ) {
    if( !( flags & FF_READ | FF_WRITE ) ) {
        cb_error( "File flags must have READ and/or WRITE set!" );
        return false;
    }
    if( flags & FF_TRUNCATE ) {
        if( flags & FF_APPEND ) {
            cb_error( "File flag APPEND and TRUNCATE cannot be set at the same time!" );
            return false;
        }
        if( flags & FF_READ ) {
            cb_error( "File flag TRUNCATE and READ cannot be set at the same time!" );
            return false;
        }
    }

    return true;
}

FileTimeCmp file_path_time_cmp(
    const char* lhs, const char* rhs, FileTimeCmpWhich which
) {
    time_t lhs_t = 0, rhs_t = 0;
    switch( which ) {
        case FILE_TIME_CMP_MODIFY: {
            if( !file_path_query_modify( lhs, &lhs_t ) ) {
                return FILE_TIME_CMP_ERROR;
            }
            if( !file_path_query_modify( rhs, &rhs_t ) ) {
                return FILE_TIME_CMP_ERROR;
            }
        } break;
        case FILE_TIME_CMP_CREATE:
        default: {
            if( !file_path_query_create( lhs, &lhs_t ) ) {
                return FILE_TIME_CMP_ERROR;
            }
            if( !file_path_query_create( rhs, &rhs_t ) ) {
                return FILE_TIME_CMP_ERROR;
            }
        } break;
    }

    if( lhs_t == rhs_t ) {
        return FILE_TIME_CMP_EQUAL;
    }

    double diff = difftime( lhs_t, rhs_t );

    if( diff > 0.0 ) {
        return FILE_TIME_CMP_LEFT_IS_NEWER;
    } else {
        return FILE_TIME_CMP_RIGHT_IS_NEWER;
    }
}
FileTimeCmp file_path_time_cmp_multi(
    const char* lhs, size_t count, const char** rhs, FileTimeCmpWhich which
) {
    FileTimeCmp res = FILE_TIME_CMP_EQUAL;
    for( size_t i = 0; i < count; ++i ) {
        FileTimeCmp this = file_path_time_cmp( lhs, rhs[i], which );

        switch( this ) {
            case FILE_TIME_CMP_ERROR:
            case FILE_TIME_CMP_RIGHT_IS_NEWER: {
                return this;
            } break;
            default: {
                res = this;
            } break;
        }
    }
    return res;
}
FileTimeCmp file_path_time_cmp_multi_string_slice(
    const char* lhs, size_t count, StringSlice* rhs, FileTimeCmpWhich which
) {
    FileTimeCmp res = FILE_TIME_CMP_EQUAL;
    for( size_t i = 0; i < count; ++i ) {
        FileTimeCmp this = file_path_time_cmp( lhs, rhs[i].cc, which );

        switch( this ) {
            case FILE_TIME_CMP_ERROR:
            case FILE_TIME_CMP_RIGHT_IS_NEWER: {
                return this;
            } break;
            default: {
                res = this;
            } break;
        }
    }
    return res;
}

bool_t path_find_files( const char* path, FindFiles* out_result ) {
    return path_find_files_string_slice( string_slice_new( 0, path ), out_result );
}
void path_find_files_free( FindFiles* ff ) {
    if( ff ) {
        if( ff->paths ) {
            darray_free( ff->paths );
        }
        if( ff->buffer ) {
            dstring_free( ff->buffer );
        }
        memory_zero( ff, sizeof(*ff) );
    }
}

bool_t pipe_write_fmt_va( Pipe* wpipe, const char* format, va_list va ) {
    DynamicString* str = dstring_fmt_va( format, va );
    if( !str ) {
        cb_error( "pipe_write_fmt: failed to format string!" );
        return false;
    }
    StringSlice substr = string_slice_from_dstring( str );
    while( substr.len ) {
        uint32_t written = 0;
        if( !pipe_write( wpipe, substr.len, substr.cc, &written ) ) {
            dstring_free( str );
            return false;
        }

        substr = string_slice_advance_by( substr, written );
    }
    dstring_free( str );
    return true;
}
bool_t pipe_write_fmt( Pipe* wpipe, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    bool_t res = pipe_write_fmt_va( wpipe, format, va );
    va_end( va );
    return res;
}

struct JobEntry {
    JobFN* fn;
    void*  params;
};

struct JobQueue {
    Semaphore wakeup;
    Atom status;
    Atom front, back;
    Atom len;
    struct JobEntry entries[32];
};
static struct JobQueue* global_queue = NULL;

bool_t job_dequeue( struct JobQueue* queue, struct JobEntry* out_entry ) {
    if( !queue->len ) {
        return false;
    }
    memory_fence();

    Atom front = atomic_add( &queue->front, 1 );

    memory_fence();

    *out_entry = queue->entries[ (front + 1) % static_array_len( queue->entries ) ];
    return true;
}

void job_queue_proc( uint32_t id, void* params ) {
    struct JobQueue* queue = params;
    memory_fence();

    while( !queue->status ) {}
    memory_fence();

    if( queue->status > 1 ) {
        return;
    }

    for(;;) {
        struct JobEntry entry;
        memory_zero( &entry, sizeof(entry) );

        semaphore_wait( &queue->wakeup, UINT32_MAX );
        memory_fence();

        if( job_dequeue( queue, &entry ) ) {
            entry.fn( id, entry.params );
            memory_fence();
            atomic_sub( &queue->len, 1 );
        }
    }
}

bool_t ___create_queue(void) {
    if( global_queue ) {
        return true;
    }
    cb_info( "creating job queue . . ." );
    size_t buf_size = sizeof(struct JobQueue);
    struct JobQueue* queue = memory_alloc( buf_size );
    if( !queue ) {
        cb_error( "failed to allocate job queue!" );
        return false;
    }
    if( !semaphore_create( &queue->wakeup ) ) {
        cb_error( "failed to create wakup semaphore!" );
        memory_free( queue, buf_size );
        return false;
    }
    queue->front = -1;

    memory_fence();

    for( size_t i = 0; i < THREAD_COUNT; ++i ) {
        if( !internal_thread_create( job_queue_proc, queue ) ) {
            cb_error( "failed to spawn thread!" );
            semaphore_destroy( &queue->wakeup );

            atomic_add( &queue->status, 2 );

            thread_sleep(2);
            memory_free( queue, buf_size );
            return false;
        }
    }

    memory_fence();
    atomic_add( &queue->status, 1 );

    cb_info( "job queue created with %u threads.", THREAD_COUNT );
    global_queue = queue;

    return true;
}

bool_t job_enqueue( JobFN* job, void* params, uint32_t ms ) {
    if( !___create_queue() ) {
        return false;
    }

    if( ms ) {
        if( global_queue->len >= static_array_len( global_queue->entries ) ) {
            if( !job_wait( ms ) ) {
                return false;
            }
        }
    } else {
        if( global_queue->len >= static_array_len( global_queue->entries ) ) {
            return false;
        }
    }

    memory_fence();
    Atom back = atomic_add( &global_queue->back, 1 );
    atomic_add( &global_queue->len, 1 );

    memory_fence();
    
    struct JobEntry entry;
    entry.fn     = job;
    entry.params = params;

    global_queue->entries[back % static_array_len( global_queue->entries )] = entry;

    memory_fence();

    semaphore_signal( &global_queue->wakeup );

    #undef check
    return true;
}
bool_t job_wait( uint32_t ms ) {
    if( !global_queue ) {
        return true;
    }

    uint32_t current = global_queue->len;
    if( !current ) {
        return true;
    }

    if( ms == UINT32_MAX ) {
        while( global_queue->len >= current ) {
            thread_sleep(1);
        }
        return true;
    } else for( uint32_t i = 0; i < ms; ++i ) {
        if( global_queue->len < current ) {
            return true;
        }
        thread_sleep(1);
    }
    return false;
}
bool_t job_wait_all( uint32_t ms ) {
    if( !global_queue ) {
        return true;
    }

    if( ms == UINT32_MAX ) {
        while( global_queue->len ) {
            thread_sleep(1);
        }
        return true;
    } else for( uint32_t i = 0; i < ms; ++i ) {
        if( !global_queue->len ) {
            return true;
        }
        thread_sleep(1);
    }
    return false;
}

bool_t file_write_fmt_va( File* file, const char* format, va_list va ) {
    DynamicString* str = dstring_fmt_va( format, va );
    if( !str ) {
        cb_error( "file_write_fmt: failed to format string!" );
    }
    if( !dstring_len( str ) ) {
        dstring_free( str );
        return true;
    }

    bool_t res = file_write( file, dstring_len( str ), str );
    dstring_free( str );
    return res;
}
bool_t file_write_fmt( File* file, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    bool_t res = file_write_fmt_va( file, format, va );
    va_end( va );
    return res;
}
StringSlice* path_split_chunks_string_slice( StringSlice path ) {
    if( !path.len || !path.cc ) {
        cb_warn( "path_split_chunks: attempted to split empty path!" );
        return NULL;
    }

    StringSlice* res = darray_empty( StringSlice, 10 );
    if( !res ) {
        cb_error( "path_split_chunks: failed to allocate result!" );
        return NULL;
    }
    
    StringSlice subpath = path;
    while( subpath.len ) {
        size_t sep = 0;
        if( string_slice_find( subpath, '/', &sep ) ) {
            if( !sep ) {
                subpath = string_slice_advance( subpath );
                continue;
            }

            StringSlice chunk = subpath;
            chunk.len       = sep;

            StringSlice* _new = darray_push( res, &chunk );
            if( !_new ) {
                darray_free( res );
                cb_error( "path_split_chunks: failed to reallocate result!" );
                return NULL;
            }

            res     = _new;
            subpath = string_slice_advance_by( subpath, sep + 1 );
        } else {
            StringSlice* _new = darray_push( res, &subpath );
            if( !_new ) {
                darray_free( res );
                cb_error( "path_split_chunks: failed to reallocate result!" );
                return NULL;
            }
            res = _new;
            break;
        }
    }

    return res;
}
StringSlice* path_split_chunks( const char* path ) {
    return path_split_chunks_string_slice( string_slice_new( 0, path ) );
}
bool_t path_glob_match_string_slice( StringSlice path, StringSlice glob ) {
    if( glob.len == 1 && *glob.cc == '*' ) {
        return true;
    }

    while( path.len && *glob.cc != '*' ) {
        if( *glob.cc != *path.cc && *glob.cc != '?' ) {
            return false;
        }
        glob = string_slice_advance( glob );
        path = string_slice_advance( path );
    }

    StringSlice mp, cp;
    while( path.len ) {
        if( *glob.cc == '*' ) {
            glob = string_slice_advance( glob );
            if( !glob.len ) {
                return true;
            }

            mp = glob;
            cp = string_slice_advance( path );
        } else if( *glob.cc == *path.cc || *glob.cc == '?' ) {
            glob = string_slice_advance( glob );
            path = string_slice_advance( path );
        } else {
            glob = mp;
            cp   = string_slice_advance( cp );
            path = cp;
        }
    }

    while( glob.len && *glob.cc == '*' ) {
        glob = string_slice_advance( glob );
    }
    return glob.len ? false : true;
}
bool_t path_glob_match( const char* path, const char* glob ) {
    return path_glob_match_string_slice( string_slice_new( 0, path ), string_slice_new( 0, glob ) );
}
DynamicString* path_push_chunk( DynamicString* path, const char* chunk ) {
    return path_push_chunk_string_slice( path, string_slice_new( 0, chunk ) );
}
DynamicString* path_push_chunk_string_slice( DynamicString* path, StringSlice chunk ) {
    if( !chunk.len ) {
        return path;
    }
    dstring_header* head = dstring_head( path );
    if( head->len + chunk.len + 2 > head->cap ) {
        head = dstring_head( dstring_grow( head->buf, chunk.len + 2 ) );
        if( !head ) {
            return NULL;
        }
    }

    if( head->len && (chunk.cc[0] != '/' && head->buf[head->len - 1] != '/') ) {
        head = dstring_head( dstring_push( head->buf, '/' ) );
    }

    return dstring_append_string_slice( head->buf, chunk );
}
bool_t path_peek_chunk(
    const char* path, size_t* out_chunk_len, const char** out_chunk
) {
    StringSlice chunk;
    memory_zero( &chunk, sizeof(chunk) );
    bool_t res = path_peek_chunk_string_slice(
        string_slice_new( 0, path ), &chunk );

    if( res ) {
        *out_chunk_len = chunk.len;
        *out_chunk     = chunk.cc;
    }
    return res;
}
bool_t path_peek_chunk_string_slice(
    StringSlice path, StringSlice* out_chunk
) {
    if( !path.len ) {
        return false;
    }
    size_t sep = 0;
    if( string_slice_find_last( path, '/', &sep ) ) {
        if( !sep ) {
            return false;
        }
        StringSlice chunk = path;
        chunk = string_slice_advance_by( chunk, sep + 1 );

        *out_chunk = chunk;
        return out_chunk->len != 0;
    }

    *out_chunk = path;
    return true;
}
bool_t path_pop_chunk(
    DynamicString* path, size_t opt_chunk_size, char* opt_out_chunk
) {
    StringSlice chunk = string_slice_empty();
    if( !path_peek_chunk_string_slice( string_slice_from_dstring( path ), &chunk ) ) {
        return false;
    }

    if( opt_chunk_size && opt_out_chunk ) {
        size_t max_copy = opt_chunk_size - 1;
        if( max_copy ) {
            memory_copy( opt_out_chunk, chunk.cc, max_copy );
            opt_out_chunk[max_copy] = 0;
        }
    }

    size_t truncate = chunk.cc == path ? chunk.len : chunk.len + 1;

    dstring_truncate( path, dstring_len( path ) - truncate );
    return true;
}
bool_t path_file_name( StringSlice path, StringSlice* out_chunk ) {
    if( !path.len ) {
        return false;
    }

    size_t sep = 0;
    if( string_slice_find_last( path, '/', &sep ) ) {
        if( !sep || sep == path.len - 1 ) {
            return false;
        }

        StringSlice file_name = string_slice_advance_by( path, sep + 1 );
        if( !file_name.len ) {
            return false;
        }

        switch( file_name.len ) {
            case 1: {
                if( string_slice_cmp( file_name, string_slice_text( "." ) ) ) {
                    return false;
                }
            } break;
            case 2: {
                if( string_slice_cmp( file_name, string_slice_text( ".." ) ) ) {
                    return false;
                }
            } break;
            default: break;
        }

        *out_chunk = file_name;
        return true;
    }

    if( string_slice_cmp( path, string_slice_text( "~" ) ) ) {
        return false;
    }
    *out_chunk = path;
    return true;
}

const char* cbuild_compiler_string(void) {
    return global_compiler;
}
const char* cbuild_command_line(void) {
    return global_command_line;
}
const char* cbuild_compile_time(void) {
    return global_compile_time;
}

static char* get_global_string(void) {
    int index = atomic_add( &global_string_buffer_index, 1 );
    index    %= MAX_GLOBAL_STRING_COUNT;
    memory_fence();

    char* buf = global_string_buffers[index];
    memory_zero( buf, MAX_GLOBAL_STRING_SIZE );
    return buf;
}
const char* local_fmt_va( const char* format, va_list va ) {
    char* buf = get_global_string();
   
    vsnprintf( buf, MAX_GLOBAL_STRING_SIZE, format, va );
    return buf;
}
const char* local_fmt( const char* format, ... ) {
    va_list va;
    va_start( va, format );
    const char* res = local_fmt_va( format, va );
    va_end( va );
    return res;
}
const char* local_fmt_bytesize( size_t bytes ) {
    const char* unit = "B ";
    size_t whole = bytes;
    size_t rem   = 0;
    if( whole >= 1000 ) {
        unit  = "KB";
        rem   = whole % 1000;
        whole /= 1000;
        if( whole >= 1000 ) {
            unit   = "MB";
            rem    = whole % 1000;
            whole /= 1000;
            if( whole >= 1000 ) {
                unit   = "GB";
                rem    = whole % 1000;
                whole /= 1000;
                if( whole >= 1000 ) {
                    unit   = "TB";
                    rem    = whole % 1000;
                    whole /= 1000;
                }
            }
        }
    }
    char* buf = get_global_string();
    snprintf( buf, MAX_GLOBAL_STRING_SIZE, "%zu.%03zu%s", whole, rem, unit );
    return buf;
}

void swap_generic( size_t stride, void* a, void* b, void* swap ) {
    memory_copy( swap, a, stride );
    memory_copy( a, b, stride );
    memory_copy( b, swap, stride );
}

static ssize_t quicksort_partition(
    ssize_t low, ssize_t hi, size_t stride,
    void* buffer, SortCmpFN* cmp, void* swap
) {
    char* buf = buffer;
    void* piv = buf + (hi * stride);

    ssize_t i = low - 1;

    for( ssize_t j = low; j <= hi - 1; ++j ) {
        void* at_j = buf + (j * stride);
        if( cmp( stride, at_j, piv ) ) {
            ++i;
            void* at_i = buf + (i * stride);
            swap_generic( stride, at_i, at_j, swap );
        }
    }

    i++;
    swap_generic( stride, buf + (i * stride), buf + (hi * stride), swap );
    return i;
}

static void quicksort(
    ssize_t from, ssize_t to, size_t stride,
    void* buf, SortCmpFN* cmp, void* swap
) {
    while( from < to ) {
        ssize_t pi = quicksort_partition( from, to, stride, buf, cmp, swap );

        if( pi - from < to - pi ) {
            quicksort( from, pi - 1, stride, buf, cmp, swap );
            from = pi + 1;
        } else {
            quicksort( pi + 1, to, stride, buf, cmp, swap );
            to = pi - 1;
        }
    }
}

void array_sort( SortCmpFN* cmp, size_t stride, size_t len, void* array ) {
    void* swap = memory_alloc( stride );
    assert( swap );
    assert( len );

    quicksort( 0, len - 1, stride, array, cmp, swap );

    memory_free( swap, stride );
}

void* darray_empty_generic( size_t stride, size_t cap ) {
    darray_header* head = memory_alloc( (stride * cap) + sizeof(*head) );
    head->stride = stride;
    head->cap    = cap;
    return head->buf;
}
void* darray_new_generic( size_t stride, size_t len, const void* source ) {
    darray_header* head = darray_head( darray_empty_generic( stride, len ) );
    if( !head ) {
        return NULL;
    }
    memory_copy( head->buf, source, stride * len );
    head->len = len;
    return head->buf;
}
void* darray_grow( void* darray, size_t amount ) {
    darray_header* head = darray_head( darray );
    size_t old_size = (head->stride * head->cap) + sizeof(*head);
    size_t new_size = old_size + (head->stride * amount);

    darray_header* res = memory_realloc( head, old_size, new_size );
    if( !res ) {
        return NULL;
    }
    res->cap += amount;
    return res->buf;
}
void* darray_clone( void* darray ) {
    darray_header* head = darray_head( darray );
    return darray_new_generic( head->stride, head->len, head->buf );
}
void darray_free( void* darray ) {
    if( darray ) {
        darray_header* head = darray_head( darray );
        size_t size = (head->cap * head->stride) + sizeof(*head);
        memory_free( head, size );
    }
}
void* darray_head( void* darray ) {
    char* array = darray;
    return array ? array - sizeof(darray_header) : 0;
}
void darray_set_len( void* darray, size_t len ) {
    darray_header* head = darray_head( darray );
    size_t max = len > head->cap ? head->cap : len;
    head->len = max;
}
size_t darray_len( const void* darray ) {
    darray_header* head = darray_head( (void*)darray );
    return head->len;
}
size_t darray_cap( const void* darray ) {
    darray_header* head = darray_head( (void*)darray );
    return head->cap;
}
size_t darray_stride( const void* darray ) {
    darray_header* head = darray_head( (void*)darray );
    return head->stride;
}
size_t darray_remaining( const void* darray ) {
    darray_header* head = darray_head( (void*)darray );
    return head->cap - head->len;
}
bool_t darray_is_empty( const void* darray ) {
    darray_header* head = darray_head( (void*)darray );
    return head->len != 0;
}
bool_t darray_is_full( const void* darray ) {
    darray_header* head = darray_head( (void*)darray );
    return head->len >= head->cap;
}
void darray_truncate( void* darray, size_t max ) {
    darray_header* head = darray_head( darray );
    if( max >= head->len ) {
        return;
    }

    memory_zero(
        head->buf + (head->stride * max),
        (head->cap * head->stride) - (max * head->stride) );
    head->len = max;
}
void darray_clear( void* darray ) {
    darray_header* head = darray_head( darray );
    memory_zero( head->buf, head->stride * head->len );
    head->len = 0;
}
void* darray_filter( void* darray, FilterFN* filter, void* filter_params ) {
    darray_header* head = darray_head( darray );
    size_t init_cap     = (head->len / 2) ? (head->len / 2) : 2;
    darray_header* res  =
        darray_head( darray_empty_generic( head->stride, init_cap ) );
    if( !res ) {
        return NULL;
    }

    for( size_t i = 0; i < head->len; ++i ) {
        void* p = head->buf + (head->stride * i);
        if( filter( i, head->stride, p, filter_params ) ) {
            darray_header* _new = darray_head( darray_push( res->buf, p ) );
            if( !_new ) {
                darray_free( res->buf );
                return NULL;
            }
            res = _new;
        }
    }
    return res->buf;
}
void* darray_filter_self( void* darray, FilterFN* filter, void* filter_params ) {
    // TODO(alicia): i know theres a smarter way to do this hmmm . . . 
    void* filtered = darray_filter( darray, filter, filter_params );
    if( !filtered ) {
        return NULL;
    }
    darray_free( darray );
    return filtered;
}
void* darray_concat( const void* left, const void* right ) {
    darray_header* hleft, *hright;
    hleft  = darray_head( (void*)left );
    hright = darray_head( (void*)right );
    assert( hleft->stride == hright->stride );

    darray_header* res =
        darray_head( darray_empty_generic( hleft->stride, hleft->len + hright->len ) );
    if( !res ) {
        return NULL;
    }
    memory_copy( res->buf, left, hleft->len * res->stride );
    memory_copy(
        res->buf + (hleft->len * res->stride), right, hright->len * res->stride );
    res->len += hleft->len + hright->len;
    return res->buf;
}
void* darray_concat_arrays(
    size_t stride, size_t left_len, const void* left,
    size_t right_len, const void* right
) {
    darray_header* res =
        darray_head( darray_empty_generic( stride, left_len + right_len ) );
    if( !res ) {
        return NULL;
    }
    memory_copy( res->buf, left, left_len * res->stride );
    memory_copy(
        res->buf + (left_len * res->stride), right, right_len * res->stride );
    res->len += left_len + right_len;
    return res->buf;
}
void* darray_split( void* darray, size_t at ) {
    darray_header* head = darray_head( darray );
    if( at >= head->len ) {
        return NULL;
    }

    darray_header* res =
        darray_head( darray_empty_generic( head->stride, head->len - at ) );
    if( !res ) {
        return NULL;
    }
    memory_copy(
        res->buf, head->buf + (head->stride * at),
        head->stride * (head->cap - at) );
    res->len = head->len - at;
    darray_truncate( darray, at );
    return res->buf;
}
void* darray_extend( void* darray, size_t count, const void* extend ) {
    darray_header* head = darray_head( darray );
    if( head->len + count >= head->cap ) {
        head = darray_head( darray_grow( darray, count + 4 ) );
        if( !head ) {
            return NULL;
        }
    }
    memory_copy( head->buf + (head->stride * head->len), extend, count * head->stride );
    head->len += count;
    return head->buf;
}
void* darray_get( void* darray, size_t index ) {
    darray_header* head = darray_head( darray );
    if( index >= head->len ) {
        return NULL;
    }
    return head->buf + (head->stride * index);
}
void* darray_push( void* darray, const void* item ) {
    darray_header* head = darray_head( darray );
    if( head->len >= head->cap ) {
        head = darray_head( darray_grow( darray, 4 ) );
        if( !head ) {
            return NULL;
        }
    }
    memory_copy( head->buf + (head->stride * head->len), item, head->stride );
    head->len++;
    return head->buf;
}
bool_t darray_push_try( void* darray, const void* item ) {
    darray_header* head = darray_head( darray );
    if( head->len >= head->cap ) {
        return false;
    }

    memory_copy( head->buf + (head->stride * head->len), item, head->stride );
    head->len++;

    return true;
}
void* darray_pop( void* darray ) {
    darray_header* head = darray_head( darray );
    if( !head->len ) {
        return NULL;
    }
    return head->buf + (head->stride * head->len--);
}
void* darray_insert( void* darray, size_t index, const void* item ) {
    darray_header* head = darray_head( darray );
    assert( index < head->len );
    if( head->len >= head->cap ) {
        head = darray_head( darray_grow( head->buf, 4 ) );
        if( !head ) {
            return NULL;
        }
    }

    void* dst = head->buf + (head->stride * (index + 1));
    void* src = head->buf + (head->stride * index);
    size_t size = (head->len - index) * head->stride;
    memory_move( dst, src, size );
    memory_copy( src, item, head->stride );
    head->len++;
    return head->buf;
}
bool_t darray_remove( void* darray, size_t at, void* opt_out_item ) {
    darray_header* head = darray_head( darray );
    if( !head->len || at >= head->len ) {
        return false;
    }
    if( opt_out_item ) {
        memory_copy( opt_out_item, head->buf + (head->stride * at), head->stride );
    }

    if( at != head->len - 1 ) {
        memory_move(
            head->buf + (head->stride * at),
            head->buf + (head->stride * (at + 1)),
            head->stride * (head->len - at) );
    }
    head->len--;
    return true;
}

bool_t string_slice_is_empty( StringSlice substr ) {
    return substr.len == 0;
}
bool_t string_slice_is_null_terminated( StringSlice substr ) {
    if( !substr.len ) {
        return false;
    }

    if( !substr.cc[substr.len - 1] || !substr.cc[substr.len] ) {
        return true;
    }
    return false;
}
bool_t string_slice_cmp( StringSlice a, StringSlice b ) {
    if( a.len != b.len ) {
        return false;
    }
    return memory_cmp( a.cc, b.cc, a.len );
}
bool_t string_slice_cmp_clamped( StringSlice a, StringSlice b ) {
    if( string_slice_is_empty( a ) || string_slice_is_empty( b ) ) {
        return false;
    }
    size_t min = a.len > b.len ? b.len : a.len;
    return memory_cmp( a.cc, b.cc, min );
}
bool_t string_slice_find( StringSlice substr, char c, size_t* opt_out_pos ) {
    const char* ptr = memchr( substr.cc, c, substr.len );
    if( !ptr ) {
        return false;
    }
    if( opt_out_pos ) {
        *opt_out_pos = ptr - substr.cc;
    }
    return true;
}
bool_t string_slice_find_last( StringSlice substr, char c, size_t* opt_out_pos ) {
    for( size_t i = substr.len; i-- > 0; ) {
        if( substr.cc[i] == c ) {
            if( opt_out_pos ) {
                *opt_out_pos = i;
            }
            return true;
        }
    }
    return false;
}
bool_t string_slice_find_set( StringSlice substr, StringSlice set, size_t* opt_out_pos ) {
    for( size_t i = 0; i < set.len; ++i ) {
        if( string_slice_find( substr, set.cc[i], opt_out_pos ) ) {
            return true;
        }
    }
    return false;
}
bool_t string_slice_find_last_set( StringSlice substr, StringSlice set, size_t* out_pos ) {
    for( size_t i = 0; i < set.len; ++i ) {
        if( string_slice_find_last( substr, set.cc[i], out_pos ) ) {
            return true;
        }
    }
    return false;
}
bool_t string_slice_find_string_slice( StringSlice source, StringSlice substr, size_t* out_pos ) {
    StringSlice rem = source;
    while( rem.len ) {
        if( rem.len < substr.len ) {
            return false;
        }

        size_t start = 0;
        if( string_slice_find( rem, substr.cc[0], &start ) ) {
            rem = string_slice_advance_by( rem, start );
            if( rem.len < substr.len ) {
                return false;
            }

            StringSlice cmp = rem;
            cmp.len       = substr.len;
            if( string_slice_cmp( cmp, substr ) ) {
                *out_pos = rem.cc - source.cc;
                return true;
            }
            rem = string_slice_advance( rem );
        } else {
            return false;
        }
    }
    return false;
}
StringSlice string_slice_advance( StringSlice substr ) {
    if( !substr.len ) {
        return substr;
    }
    StringSlice res = substr;
    res.cc++;
    res.len--;
    return res;
}
StringSlice string_slice_advance_by( StringSlice substr, size_t amount ) {
    if( !substr.len ) {
        return substr;
    }

    StringSlice res = substr;
    if( amount > res.len ) {
        res.cc  = string_slice_last( res );
        res.len = 0;
        return res;
    }
    res.cc  += amount;
    res.len -= amount;
    return res;
}
StringSlice string_slice_truncate( StringSlice substr, size_t max ) {
    if( max > substr.len ) {
        return substr;
    }
    StringSlice res = substr;
    res.len       = max;
    return res;
}
StringSlice string_slice_trim( StringSlice substr, size_t amount ) {
    StringSlice res = substr;
    if( amount > substr.len ) {
        res.len = 0;
        return res;
    }
    res.len -= amount;
    return res;
}
char* string_slice_first( StringSlice substr ) {
    if( !substr.len ) {
        return NULL;
    }
    return (char*)substr.cc;
}
char* string_slice_last( StringSlice substr ) {
    if( !substr.len ) {
        return NULL;
    }
    return (char*)substr.cc + (substr.len - 1);
}
StringSlice string_slice_trim_ws_lead( StringSlice substr ) {
    StringSlice res = substr;
    while( res.len ) {
        if( isspace( *res.cc ) ) {
            res = string_slice_advance( res );
        } else {
            break;
        }
    }
    return res;
}
StringSlice string_slice_trim_ws_trail( StringSlice substr ) {
    StringSlice res = substr;
    while( res.len ) {
        if( isspace( *string_slice_last( res ) ) ) {
            res.len--;
        } else {
            break;
        }
    }
    return res;
}
StringSlice string_slice_trim_ws( StringSlice substr ) {
    return string_slice_trim_ws_lead( string_slice_trim_ws_trail( substr ) );
}
size_t string_slice_len_utf8( StringSlice substr ) {
    const unsigned char* ucc = (const unsigned char*)substr.cc;
    size_t res = 0;
    for( size_t i = 0; i < substr.len; ++i ) {
        if( (ucc[i] & 0xC0) != 0x80 ) {
            res++;
        }
    }
    return res;
}
StringSlice* string_slice_darray_from_arg( int argc, char** argv ) {
    if( argc <= 0 ) {
        return NULL;
    }
    StringSlice* res = darray_empty( StringSlice, argc );
    if( !res ) {
        return NULL;
    }

    for( int i = 0; i < argc; ++i ) {
        StringSlice ss = string_slice_new( 0, argv[i] );
        res = darray_push( res, &ss );
    }
    return res;
}
StringSlice* string_slice_darray_from_delim( StringSlice source, StringSlice delim ) {
    StringSlice substr = source;
    size_t cap = 0;
    while( substr.len ) {
        size_t delim_pos = 0;
        if( string_slice_find_string_slice( substr, delim, &delim_pos ) ) {
            cap++;

            substr = string_slice_advance_by( substr, delim_pos + delim.len );
        } else {
            cap++;
            break;
        }
    }

    StringSlice* res = darray_empty( StringSlice, cap );
    if( !res ) {
        return NULL;
    }

    substr = source;
    while( substr.len ) {
        size_t delim_pos = 0;
        if( string_slice_find_string_slice( substr, delim, &delim_pos ) ) {
            StringSlice item = substr;
            item.len       = delim_pos;

            res    = darray_push( res, &item );
            substr = string_slice_advance_by( substr, item.len + delim.len );
        } else {
            res = darray_push( res, &substr );
            break;
        }
    }

    return res;
}

size_t cstr_len( const char* string ) {
    return strlen( string );
}
size_t cstr_len_utf8( const char* string ) {
    return string_slice_len_utf8( string_slice_new( 0, string ) );
}
bool_t cstr_cmp( const char* a, const char* b ) {
    return strcmp( a, b ) == 0;
}
bool_t cstr_find( const char* string, char c, size_t* opt_out_pos ) {
    char* res = strchr( string, c );
    if( !res ) {
        return false;
    }
    if( opt_out_pos ) {
        *opt_out_pos = res - string;
    }
    return true;
}
bool_t cstr_find_last( const char* string, char c, size_t* opt_out_pos ) {
    char* res = strrchr( string, c );
    if( !res ) {
        return false;
    }
    if( opt_out_pos ) {
        *opt_out_pos = res - string;
    }
    return true;
}
bool_t cstr_find_set( const char* string, const char* set, size_t* opt_out_pos ) {
    char* res = strpbrk( string, set );
    if( !res ) {
        return false;
    }
    if( opt_out_pos ) {
        *opt_out_pos = res - string;
    }
    return true;
}
bool_t cstr_find_cstr(
    const char* string, const char* substr, size_t* opt_out_pos
) {
    char* res = strstr( string, substr );
    if( !res ) {
        return false;
    }
    if( opt_out_pos ) {
        *opt_out_pos = res - string;
    }
    return true;
}

DynamicString* dstring_empty( size_t size ) {
    dstring_header* res = memory_alloc( sizeof(*res) + size + 1 );
    if( !res ) {
        return NULL;
    }
    res->cap = size + 1;
    return res->buf;
}
DynamicString* dstring_new( size_t opt_len, const char* source ) {
    size_t len = opt_len;
    if( !len ) {
        len = cstr_len( source );
        if( !len ) {
            return NULL;
        }
    }
    dstring_header* res = memory_alloc( sizeof(*res) + len + 16 );
    if( !res ) {
        return NULL;
    }
    res->len = len;
    res->cap = len + 16;
    memory_copy( res->buf, source, res->len );

    return res->buf;
}
DynamicString* dstring_concat_array_string_slice( size_t len, StringSlice* strings, StringSlice sep ) {
    size_t total_size = 0;
    for( size_t i = 0; i < len; ++i ) {
        total_size += strings[i].len + sep.len;
    }
    DynamicString* res = dstring_empty( total_size );
    if( !res ) {
        return NULL;
    }
    for( size_t i = 0; i < len; ++i ) {
        DynamicString* _new = dstring_append_string_slice( res, strings[i] );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;

        if( i + 1 >= len ) {
            break;
        }
        _new = dstring_append_string_slice( res, sep );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;
    }
    return res;
}
DynamicString* dstring_concat_array( size_t len, const char** strings, const char* sep ) {
    size_t total_size = 0;
    StringSlice _sep    = string_slice_new( 0, sep );
    for( size_t i = 0; i < len; ++i ) {
        total_size += cstr_len( strings[i] ) + _sep.len;
    }
    DynamicString* res = dstring_empty( total_size );
    if( !res ) {
        return NULL;
    }
    for( size_t i = 0; i < len; ++i ) {
        DynamicString* _new = dstring_append_str( res, strings[i] );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;

        if( i + 1 >= len ) {
            break;
        }
        _new = dstring_append_string_slice( res, _sep );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;
    }
    return res;
}
DynamicString* dstring_fmt_va( const char* format, va_list va ) {
    va_list len, fmt;
    va_copy( len, va );
    va_copy( fmt, len );

    size_t size = vsnprintf( 0, 0, format, len );

    va_end( len );

    dstring_header* res = dstring_head( dstring_empty( size ) );
    if( !res ) {
        va_end( fmt );
        return NULL;
    }

    vsnprintf( res->buf, res->cap, format, fmt );
    res->len += size;

    va_end( fmt );

    return res->buf;
}
DynamicString* dstring_fmt( const char* format, ... ) {
    va_list va;
    va_start( va, format );
    DynamicString* res = dstring_fmt_va( format, va );
    va_end( va );
    return res;
}
DynamicString* dstring_clone( const DynamicString* source ) {
    dstring_header* head = dstring_head( (DynamicString*)source );
    dstring_header* res  = memory_alloc( sizeof(*res) + head->len + 16 );
    if( !res ) {
        return NULL;
    }
    res->cap = head->len + 16;
    res->len = head->len;
    memory_copy( res->buf, source, head->len );
    return res->buf;
}
void dstring_free( DynamicString* string ) {
    if( string ) {
        dstring_header* head = dstring_head( string );
        size_t size = head->cap + sizeof(*head);
        memory_free( head, size );
    }
}
void* dstring_head( DynamicString* string ) {
    return string ? string - sizeof(dstring_header) : 0;
}
DynamicString* dstring_grow( DynamicString* string, size_t amount ) {
    dstring_header* head = dstring_head( string );
    size_t old_size = head->cap + sizeof(*head);
    size_t new_size = old_size + amount + 1;
    dstring_header* res = memory_realloc( head, old_size, new_size );
    if( !res ) {
        return NULL;
    }
    res->cap += amount + 1;
    return res->buf;
}
void dstring_clear( DynamicString* string ) {
    dstring_header* head = dstring_head( string );
    memory_zero( head->buf, head->len );
    head->len = 0;
}
void dstring_truncate( DynamicString* string, size_t max_len ) {
    dstring_header* head = dstring_head( string );
    size_t len  = max_len > head->len ? head->len : max_len;
    size_t zero = head->len - len;

    if( zero ) {
        memory_zero( string + len, zero );
    }
    head->len = len;
}
size_t dstring_len( const DynamicString* string ) {
    dstring_header* head = dstring_head( (DynamicString*)string );
    return head->len;
}
size_t dstring_cap( const DynamicString* string ) {
    dstring_header* head = dstring_head( (DynamicString*)string );
    return head->cap;
}
size_t dstring_remaining( const DynamicString* string ) {
    dstring_header* head = dstring_head( (DynamicString*)string );
    return head->cap - head->len;
}
bool_t dstring_is_empty( const DynamicString* string ) {
    dstring_header* head = dstring_head( (DynamicString*)string );
    return head->len == 0;
}
bool_t dstring_is_full( const DynamicString* string ) {
    dstring_header* head = dstring_head( (DynamicString*)string );
    return head->len >= head->cap;
}
DynamicString* dstring_concat( const DynamicString* left, const DynamicString* right ) {
    dstring_header* hleft, *hright;
    hleft  = dstring_head( (DynamicString*)left );
    hright = dstring_head( (DynamicString*)right );

    dstring_header* res = dstring_head( dstring_empty( hleft->len + hright->len ) );
    if( !res ) {
        return NULL;
    }

    memory_copy( res->buf, left, hleft->len );
    memory_copy( res->buf + hleft->len, right, hright->len );
    res->len += hleft->len + hright->len;

    return res->buf;
}
DynamicString* dstring_concat_string_slice( StringSlice left, StringSlice right ) {
    dstring_header* res = dstring_head( dstring_empty( left.len + right.len ) );
    if( !res ) {
        return NULL;
    }

    memory_copy( res->buf, left.cc, left.len );
    memory_copy( res->buf + left.len, right.cc, right.len );
    res->len += left.len + right.len;

    return res->buf;
}
DynamicString* dstring_concat_str( const char* left, const char* right ) {
    StringSlice _left  = string_slice_new( 0, left );
    StringSlice _right = string_slice_new( 0, right );
    return dstring_concat_string_slice( _left, _right );
}
DynamicString* dstring_prepend( DynamicString* string, const DynamicString* prepend ) {
    dstring_header* head     = dstring_head( string );
    dstring_header* hprepend = dstring_head( (DynamicString*)prepend );

    size_t new_len = head->len + hprepend->len;
    if( new_len + 1 > head->cap ) {
        head = dstring_head( dstring_grow( string, hprepend->len + 8 ) );
        if( !head ) {
            return NULL;
        }
    }

    memory_move( head->buf + hprepend->len, head->buf, head->len );
    memory_copy( head->buf, prepend, hprepend->len );
    head->len = new_len;

    return head->buf;
}
DynamicString* dstring_prepend_string_slice( DynamicString* string, StringSlice prepend ) {
    dstring_header* head     = dstring_head( string );

    size_t new_len = head->len + prepend.len;
    if( new_len + 1 > head->cap ) {
        head = dstring_head( dstring_grow( string, prepend.len + 8 ) );
        if( !head ) {
            return NULL;
        }
    }

    memory_move( head->buf + prepend.len, head->buf, head->len );
    memory_copy( head->buf, prepend.cc, prepend.len );
    head->len = new_len;

    return head->buf;
}
DynamicString* dstring_prepend_str( DynamicString* string, const char* prepend ) {
    return dstring_prepend_string_slice( string, string_slice_new( 0, prepend ) );
}
DynamicString* dstring_append( DynamicString* string, const DynamicString* append ) {
    dstring_header* head    = dstring_head( string );
    dstring_header* happend = dstring_head( (DynamicString*)append );

    size_t new_len = head->len + happend->len;
    if( new_len + 1 > head->cap ) {
        head = dstring_head( dstring_grow( string, happend->len + 8 ) );
        if( !head ) {
            return NULL;
        }
    }

    memory_copy( head->buf + head->len, append, happend->len );
    head->len = new_len;

    return head->buf;
}
DynamicString* dstring_append_string_slice( DynamicString* string, StringSlice append ) {
    dstring_header* head    = dstring_head( string );

    size_t new_len = head->len + append.len;
    if( new_len + 1 > head->cap ) {
        head = dstring_head( dstring_grow( string, append.len + 8 ) );
        if( !head ) {
            return NULL;
        }
    }

    memory_copy( head->buf + head->len, append.cc, append.len );
    head->len = new_len;

    return head->buf;
}
DynamicString* dstring_append_str( DynamicString* string, const char* append ) {
    return dstring_append_string_slice( string, string_slice_new( 0, append ) );
}
DynamicString* dstring_insert( DynamicString* string, const DynamicString* insert, size_t index ) {
    dstring_header* head    = dstring_head( string );

    size_t i = index;
    if( i >= head->len ) {
        i = head->len ? head->len - 1 : 0;
    }

    dstring_header* hinsert = dstring_head( (DynamicString*)insert );

    size_t new_len = head->len + hinsert->len;
    if( new_len + 1 > head->cap ) {
        head = dstring_head( dstring_grow( string, hinsert->len + 8 ) );
        if( !head ) {
            return NULL;
        }
    }

    memory_move( head->buf + i + hinsert->len, head->buf + i, head->len - i );
    memory_copy( head->buf + i, insert, hinsert->len );
    head->len = new_len;

    return head->buf;
}
DynamicString* dstring_insert_string_slice( DynamicString* string, StringSlice insert, size_t index ) {
    dstring_header* head    = dstring_head( string );

    size_t i = index;
    if( i >= head->len ) {
        i = head->len ? head->len - 1 : 0;
    }

    size_t new_len = head->len + insert.len;
    if( new_len + 1 > head->cap ) {
        head = dstring_head( dstring_grow( string, insert.len + 8 ) );
        if( !head ) {
            return NULL;
        }
    }

    memory_move( head->buf + i + insert.len, head->buf + i, head->len - i );
    memory_copy( head->buf + i, insert.cc, insert.len );
    head->len = new_len;

    return head->buf;
}
DynamicString* dstring_insert_str( DynamicString* string, const char* insert, size_t index ) {
    return dstring_insert_string_slice( string, string_slice_new( 0, insert ), index );
}
DynamicString* dstring_push( DynamicString* string, char c ) {
    dstring_header* head = dstring_head( string );
    if( head->len + 1 >= head->cap ) {
        head = dstring_head( dstring_grow( string, 16 ) );
        if( !head ) {
            return NULL;
        }
    }
    head->buf[head->len++] = c;
    head->buf[head->len]   = 0;
    return head->buf;
}
bool_t dstring_pop( DynamicString* string, char* opt_out_c ) {
    dstring_header* head = dstring_head( string );
    if( !head->len ) {
        return false;
    }

    char c = head->buf[head->len - 1];
    if( opt_out_c ) {
        *opt_out_c = c;
    }
    head->buf[--head->len] = 0;
    return true;
}
bool_t dstring_remove( DynamicString* string, size_t index ) {
    dstring_header* head = dstring_head( string );
    if( index >= head->len ) {
        return false;
    }
    memory_move( head->buf + index, head->buf + index + 1, head->cap - (index + 1) );
    head->len--;
    return true;
}
DynamicString* dstring_emplace( DynamicString* string, char c, size_t index ) {
    dstring_header* head = dstring_head( string );
    if( index >= head->len ) {
        return NULL;
    }

    if( head->len + 1 >= head->cap ) {
        head = dstring_head( dstring_grow( string, 16 ) );
        if( !head ) {
            return NULL;
        }
    }
    memory_move( head->buf + index + 1, head->buf + index, head->cap - index );
    head->buf[index] = c;
    head->len++;
    return head->buf;
}

static Atom64 global_memory_allocated       = 0;
static Atom64 global_total_memory_allocated = 0;

void* memory_alloc( size_t size ) {
    void* res = malloc( size );
    if( !res ) {
        return NULL;
    }
    atomic_add64( &global_memory_allocated, size );
    atomic_add64( &global_total_memory_allocated, size );

    memset( res, 0, size );
    return res;
}
void* memory_realloc( void* memory, size_t old_size, size_t new_size ) {
    assert( new_size >= old_size );
    char* res = realloc( memory, new_size );
    if( !res ) {
        return NULL;
    }
    size_t diff = new_size - old_size;
    atomic_add64( &global_memory_allocated, diff );
    atomic_add64( &global_total_memory_allocated, diff );

    memset( res + old_size, 0, diff );
    return res;
}
void memory_free( void* memory, size_t size ) {
    atomic_sub64( &global_memory_allocated, size );
    free( memory );
}
size_t memory_query_allocated(void) {
    return global_memory_allocated;
}
size_t memory_query_total_allocated(void) {
    return global_total_memory_allocated;
}
void memory_set( void* memory, int8_t value, size_t size ) {
    memset( memory, value, size );
}
void memory_zero( void* memory, size_t size ) {
    memory_set( memory, 0, size );
}
void memory_copy( void* restrict dst, const void* restrict src, size_t size ) {
    memcpy( dst, src, size );
}
void memory_move( void* dst, const void* src, size_t size ) {
    memmove( dst, src, size );
}
bool_t memory_cmp( const void* a, const void* b, size_t size ) {
    return memcmp( a, b, size ) == 0;
}
void logger_level_set( LogLevel level ) {
    if( level <= LOG_LEVEL_ERROR ) {
        global_log_level = level;
    }
}
LogLevel logger_level_query(void) {
    return global_log_level;
}
void logger_va( LogLevel level, const char* format, va_list va ) {
    level = level > LOG_LEVEL_ERROR ? LOG_LEVEL_ERROR : level;
    if( level < global_log_level ) {
        return;
    }

    static const char* const prefixes[] = {
        "[I] cbuild: ",
        "\033[1;33m" "[W] cbuild: ",
        "\033[1;31m" "[E] cbuild: "
    };
    FILE* streams[] = { stdout, stdout, stderr };

    if( global_queue ) {
        mutex_lock( &global_log_mtx, UINT32_MAX );
    }
    memory_fence();

    fprintf( streams[level], "%s", prefixes[level] );
    vfprintf( streams[level], format, va );
    fprintf( streams[level], "\033[1;00m\n" );

    memory_fence();
    if( global_queue ) {
        mutex_unlock( &global_log_mtx );
    }
}
void logger( LogLevel level, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    logger_va( level, format, va );
    va_end( va );
}
bool_t program_exec( const char* command_line, int* opt_out_res ) {
    ProcessID pid;
    if( !program_exec_async( command_line, NULL, NULL, NULL, &pid ) ) {
        return false;
    }
    program_wait( pid, UINT32_MAX, opt_out_res );
    program_close( pid );

    return true;
}

void platform_init(void);
void _init_( const char* argv0, const char* file, LogLevel log ) {
    mutex_create( &global_log_mtx );
    global_argv0        = argv0;
    global_compiler     = COMPILER;
    global_command_line = dstring_fmt(
        COMPILER COMPILER_BUILD_LINE COMPILER_CFLAGS
        COMPILER_ADDITIONAL_FLAGS CBUILD_COMPILE_ARGS,
        file, argv0 );
    global_compile_time = __TIMESTAMP__;
    
    logger_level_set( log );
    platform_init();

    double start = time_milliseconds();

    time_t exe_m, cbuild_m, main_m;
    file_path_query_modify( argv0, &exe_m );
    file_path_query_modify( __FILE__, &cbuild_m );
    file_path_query_modify( file, &main_m );

    double diff_main   = difftime( exe_m, main_m );
    double diff_cbuild = difftime( exe_m, cbuild_m );

    DynamicString* old_name = dstring_concat_str( argv0, ".old" );

    if( diff_main > 0.0 && diff_cbuild > 0.0 ) {
        FILE* fold = fopen( old_name, "r" );
        if( fold ) {
            fclose( fold );
            cb_info( "removing %s . . .", old_name );
            file_path_remove( old_name );
        }
        dstring_free( old_name );
        return;
    }

    cb_info( "changes in cbuild files detected, recompiling . . ." );
    if( !program_in_path( COMPILER ) ) {
        cb_error( "failed to find compiler '" COMPILER "' in path!" );
#if defined(_WIN32)
        if( string_slice_cmp( string_slice_text( "cl" ), string_slice_text( COMPILER ) ) ) {
            cb_error( "run vcvarsall.bat or run cbuild in Developer Command Prompt." );
        }
#endif
        exit(-1);
    }

    cb_info( "renaming self to %s . . . ", old_name );
    if( !file_path_move( old_name, argv0 ) ) {
        // cb_warn( "failed to rename to %s!", old_name );
        cb_warn( "attempting to remove %s . . .", old_name );
        file_path_remove( old_name );
        if( !file_path_move( old_name, argv0 ) ) {
            cb_error( "failed to rename again! quitting . . . " );
            exit(-1);
        }

        cb_info( "renamed self successfully!" );
    }

    cb_info( "recompile line: %s", global_command_line );
    int res = 0;
    if( program_exec( global_command_line, &res ) && res == 0 ) {
        cb_info( "recompiled successfully!" );
        cb_warn( "%s needs to be re-run!", argv0 );
        double end = time_milliseconds();
        cb_info( "rebuild took: %.2fms", end - start );
        exit(0);
    } else {
        cb_error( "failed to rebuild, renaming self back . . ." );
        file_path_move( argv0, old_name );
        exit(-1);
    }
}

// NOTE(alicia): platform-specific implementation
// SWIN32

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <process.h>
#include <wchar.h>

static LARGE_INTEGER global_qpf;

#define WIN32_MAX_PATH_BUFFERS (16)
static Atom global_win32_path_buffer_index = 0;
static volatile wchar_t* global_win32_path_buffer;

#define WIN32_TICKS_PER_SECOND (10000000)
#define WIN32_TO_POSIX_DIFF    (11644473600ULL)

#if defined(_MSC_VER)
    void memory_fence(void) {
        MemoryBarrier();
    }
#endif

static time_t win32_filetime_to_posix_time( FILETIME ft ) {
    ULARGE_INTEGER uli;
    uli.LowPart  = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    return (time_t)(
        (uli.QuadPart / WIN32_TICKS_PER_SECOND) - WIN32_TO_POSIX_DIFF );
}
static wchar_t* win32_get_path_buffer(void) {
    size_t index = atomic_add( &global_win32_path_buffer_index, 1 );
    index %= WIN32_MAX_PATH_BUFFERS;
    size_t offset = index * PATH_MAX_LEN;
    memory_fence();

    wchar_t* buf = (wchar_t*)(global_win32_path_buffer + offset);
    memory_zero( buf, sizeof(wchar_t) * PATH_MAX_LEN );
    return buf;
}
static void win32_utf8_to_wide(
    size_t utf8_len, const char* utf8,
    size_t wide_cap, wchar_t* out_wide, bool_t dont_zero
) {
    assert( wide_cap && utf8_len );

    if( !dont_zero ) {
        memory_zero( out_wide, wide_cap * sizeof(wchar_t) );
    }

    size_t remaining      = utf8_len;
    size_t remaining_wide = wide_cap - 1;
    while( remaining && remaining_wide ) {
        size_t offset      = utf8_len - remaining;
        size_t convert_len = remaining >= INT32_MAX - 1 ? INT32_MAX - 1 : remaining;
        if( convert_len > remaining_wide ) {
            convert_len = remaining_wide;
        }

        MultiByteToWideChar(
            CP_UTF8, 0, utf8 + offset, convert_len,
            out_wide + offset, convert_len );

        remaining      -= convert_len;
        remaining_wide -= convert_len;
    }
}
static void win32_wide_to_utf8(
    size_t wide_len, const wchar_t* wide,
    size_t utf8_cap, char* out_utf8, bool_t dont_zero
) {
    assert( wide_len && utf8_cap );

    if( !dont_zero ) {
        memory_zero( out_utf8, utf8_cap );
    }

    size_t remaining      = wide_len;
    size_t remaining_utf8 = utf8_cap - 1;
    while( remaining && remaining_utf8 ) {
        size_t offset      = wide_len - remaining;
        size_t convert_len = remaining >= INT32_MAX - 1 ? INT32_MAX - 1 : remaining;
        if( convert_len > remaining_utf8 ) {
            convert_len = remaining_utf8;
        }

        WideCharToMultiByte(
            CP_UTF8, 0, wide + offset, convert_len,
            out_utf8 + offset, convert_len, 0, 0 );

        remaining      -= convert_len;
        remaining_utf8 -= convert_len;
    }

}
static char* win32_wide_to_narrow( size_t opt_len, wchar_t* wide ) {
    if( !wide ) {
        return NULL;
    }
    size_t len = opt_len;
    if( !len ) {
        len = wcslen( wide );
        if( !len ) {
            return NULL;
        }
    }

    size_t buf_size = PATH_MAX_LEN * sizeof(wchar_t);
    char* buf       = (char*)win32_get_path_buffer();

    // NOTE(alicia): size_t to int not a problem because buf_size can't be
    // greater than INT32_MAX anyway
    WideCharToMultiByte( CP_UTF8, 0, wide, len, buf, buf_size, 0, 0 );
    return buf;
}
static void win32_wide_to_narrow_buffer(
    int opt_len, wchar_t* wide, int buf_size, char* buf
) {
    if( !wide ) {
        return;
    }
    int len = opt_len;
    if( !len ) {
        len = wcslen( wide );
        if( !len ) {
            return;
        }
    }
    WideCharToMultiByte( CP_UTF8, 0, wide, len, buf, buf_size, 0, 0 );
    return;
}
static char* win32_wide_to_narrow_alloc( int opt_len, wchar_t* wide ) {
    if( !wide ) {
        return NULL;
    }
    int len = opt_len;
    if( !len ) {
        len = wcslen( wide );
        if( !len ) {
            return NULL;
        }
    }
    char* buf = memory_alloc( len + 1 );
    if( !buf ) {
        return NULL;
    }

    WideCharToMultiByte( CP_UTF8, 0, wide, len, buf, len, 0, 0 );
    return buf;
}

struct Win32WriteBuffer {
    size_t cap;
    size_t len;
    void*  buffer;
};
size_t win32_write( struct Win32WriteBuffer* buf, size_t size, const void* write ) {
    size_t rem      = buf->cap - buf->len;
    size_t max_copy = size;
    if( max_copy > rem ) {
        max_copy = rem;
    }

    memory_copy( (char*)buf->buffer + buf->len, write, max_copy );
    buf->len += max_copy;

    return size - max_copy;
}

static size_t win32_path_posix_to_win32(
    size_t posix_len, const char* posix,
    size_t win32_cap, wchar_t* out_win32, bool_t dont_zero
) {
    #define PATH_IS_RELATIVE 0
    #define PATH_IS_HOME     1
    #define PATH_IS_ABSOLUTE 2
    assert( win32_cap );

    int path_type = PATH_IS_RELATIVE;
    if( posix_len >= sizeof("A:") ) {
        if( isalpha( posix[0] ) && posix[1] == ':' ) {
            path_type = PATH_IS_ABSOLUTE;
        }
    } else if( posix[0] == '~' ) {
        path_type = PATH_IS_HOME;
    }

    if( !dont_zero ) {
        memory_zero( out_win32, sizeof(wchar_t) * win32_cap );
    }

    StringSlice home       = string_slice_empty();
    StringSlice home_drive = string_slice_empty();
    size_t      cwd_len    = 0;

    switch( path_type ) {
        case PATH_IS_RELATIVE: {
            cwd_len = GetCurrentDirectoryW( 0, 0 );
        } break;
        case PATH_IS_HOME: {
            home_drive.cc = getenv( "HOMEDRIVE" );
            home.cc       = getenv( "HOMEPATH" );

            home_drive.len = cstr_len( home_drive.cc );
            home.len       = cstr_len( home.cc );
        } break;
        case PATH_IS_ABSOLUTE: break;
    }

    #define LOCAL_BUFFER_CAP (512)
    wchar_t local[LOCAL_BUFFER_CAP];
    memory_zero( local, sizeof(local) );

    struct Win32WriteBuffer wb;
    wb.cap    = sizeof(wchar_t) * (win32_cap - 1);
    wb.len    = 0;
    wb.buffer = out_win32;

    StringSlice slice;
    slice.len = posix_len;
    slice.cc  = posix;

    size_t result =
        win32_write( &wb, sizeof(L"\\\\?\\") - sizeof(wchar_t), L"\\\\?\\" );

    switch( path_type ) {
        case PATH_IS_RELATIVE: {
            GetCurrentDirectoryW( LOCAL_BUFFER_CAP, local );
            result += win32_write( &wb, cwd_len * sizeof(wchar_t), local );
        } break;
        case PATH_IS_HOME: {
            win32_utf8_to_wide(
                home_drive.len, home_drive.cc, LOCAL_BUFFER_CAP, local, false );
            result += win32_write( &wb, home_drive.len, local );
            win32_utf8_to_wide(
                home.len, home.cc, LOCAL_BUFFER_CAP, local, true );
            result += win32_write( &wb, home.len, local );

            slice = string_slice_advance( slice );
        } break;
        case PATH_IS_ABSOLUTE: break;
    }

    while( slice.len ) {
        size_t sep = 0;
        if( string_slice_find( slice, '/', &sep ) ) {
            if( !sep ) {
                slice = string_slice_advance( slice );
                continue;
            }

            StringSlice chunk = slice;
            chunk.len         = sep;

            switch( chunk.len ) {
                case 1: {
                    if( chunk.cc[0] == '.' ) {
                        slice = string_slice_advance_by( slice, sep + 1 );
                        continue;
                    }
                } break;
                case 2: {
                    if( chunk.cc[0] == '.' && chunk.cc[1] == '.' ) {
                        slice = string_slice_advance_by( slice, sep + 1 );
                        continue;
                    }
                } break;
            }

        } else {

        }
    }

    #undef PATH_IS_RELATIVE
    #undef PATH_IS_HOME    
    #undef PATH_IS_ABSOLUTE
    return result;
}
wchar_t* win32_path_wide( size_t buf_cap, wchar_t* buf, StringSlice path ) {
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

    StringSlice subpath = path;

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

            home_drive_len = cstr_len( home_drive );
            home_len       = cstr_len( home );
        } break;
        default: break;
    }

    memory_copy( buf, L"\\\\?\\", sizeof(L"\\\\?\\") - sizeof(wchar_t) );
    switch( path_type ) {
        case PATH_RELATIVE:  {
            offset += GetCurrentDirectoryW(
                buf_cap - offset, buf + offset );
        } break;
        case PATH_ABSOLUTE: break;
        case PATH_HOME: {
            MultiByteToWideChar(
                CP_UTF8, 0, home_drive, home_drive_len,
                buf + offset, buf_cap - offset );
            offset += home_drive_len;
            MultiByteToWideChar(
                CP_UTF8, 0, home, home_len,
                buf + offset, buf_cap - offset );
            offset += home_len;

            subpath = string_slice_advance( subpath );
        } break;
        default: break;
    }

    size_t last_chunk_len = 0;
    while( subpath.len ) {
        size_t sep = 0;
        if( string_slice_find( subpath, '/', &sep ) ) {
            if( !sep ) {
                subpath = string_slice_advance( subpath );
                continue;
            }

            StringSlice chunk = subpath;
            chunk.len       = sep;

            if( chunk.len < 3 ) {
                if( string_slice_cmp( chunk, string_slice_text( "." ) ) ) {
                    subpath = string_slice_advance_by( subpath, chunk.len + 1 );
                    continue;
                }
                if( string_slice_cmp( chunk, string_slice_text( ".." ) ) ) {
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
                    subpath = string_slice_advance_by( subpath, chunk.len + 1 );
                    continue;
                }
            }

            if( buf[offset - 1] != '\\' ) {
                buf[offset++] = '\\';
            }
            if( offset + chunk.len >= buf_cap ) {
                break;
            }
            MultiByteToWideChar(
                CP_UTF8, 0, chunk.cc, chunk.len,
                buf + offset, buf_cap - offset );
            offset += chunk.len;
            last_chunk_len = chunk.len;

            subpath = string_slice_advance_by( subpath, chunk.len + 1 );
        } else {
            if( string_slice_cmp( subpath, string_slice_text( "." ) ) ) {
                break;
            }
            if( string_slice_cmp( subpath, string_slice_text( ".." ) ) ) {
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
            if( offset + subpath.len >= buf_cap ) {
                break;
            }
            MultiByteToWideChar(
                CP_UTF8, 0, subpath.cc, subpath.len,
                buf + offset, buf_cap - offset );
            offset += subpath.len;
            break;
        }
    }
    buf[offset] = 0;

    #undef PATH_RELATIVE
    #undef PATH_HOME    
    return buf;
}
wchar_t* win32_path( StringSlice path ) {
    wchar_t* buf = win32_get_path_buffer();
    return win32_path_wide( PATH_MAX_LEN, buf, path );
}
wchar_t* win32_path_alloc( StringSlice path ) {
    size_t buf_cap  = ( 512 + path.len );
    size_t buf_size = sizeof(wchar_t) * buf_cap;
    wchar_t* buf = memory_alloc( buf_size );
    if( !buf ) {
        return NULL;
    }

    // TODO(alicia): win32_path_wide bounds checking!
    wchar_t* res = win32_path_wide( buf_cap, buf, path );
    if( !res ) {
        memory_free( buf, buf_size );
        return NULL;
    }

    return res;
}

bool_t win32_path_find_dirs(
    StringSlice pattern, size_t* out_count, DynamicString** buffer
) {
    size_t dir_count = 0;

    size_t   wide_len    = pattern.len + 32;
    size_t   wide_size   = wide_len * sizeof( wchar_t );
    wchar_t* wide_buffer = memory_alloc( wide_size );

    wchar_t* wpath = win32_path_wide( wide_len, wide_buffer, pattern );

    bool_t recursive_dir = false;

    StringSlice path = pattern; {
        StringSlice last_chunk = string_slice_empty();
        path_peek_chunk_string_slice( pattern, &last_chunk );

        if( string_slice_cmp( last_chunk, string_slice_text( "**" ) ) ) {
            recursive_dir = true;
        }

        path = string_slice_trim( path, last_chunk.len + 1 );

        if( !path.len ) {
            path = string_slice_text( "./" );
        }
    }

    WIN32_FIND_DATAW fd;
    memory_zero( &fd, sizeof(fd) );
    HANDLE f = FindFirstFileW( wpath, &fd );

    if( f == INVALID_HANDLE_VALUE ) {
        cb_error( "path_find_files: failed to allocate wide path buffer!" );
        memory_free( wide_buffer, wide_size );
        return false;
    }

    memory_zero( wide_buffer, wide_size );

    do {
        if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
            continue;
        }

        bool_t is_self = wcscmp( fd.cFileName, L"." ) == 0;
        if( !recursive_dir ) {
            if( is_self ) {
                continue;
            }
        }

        if(
            wcscmp( fd.cFileName, L".." )   == 0 ||
            wcscmp( fd.cFileName, L".git" ) == 0
        ) {
            continue;
        }

        DynamicString* _new = dstring_append_string_slice( *buffer, path );
        if( !_new ) {
            cb_error( "path_find_files: failed to add path to list!" );
            FindClose( f );
            memory_free( wide_buffer, wide_size );
            return false;
        }
        *buffer = _new;

        if( !is_self ) {
            {
                char* last = string_slice_last( path );
                if( !last || *last != '/' ) {
                    _new = dstring_push( *buffer, '/' );
                    if( !_new ) {
                        cb_error( "path_find_files: failed to add path to list!" );
                        FindClose( f );
                        memory_free( wide_buffer, wide_size );
                        return false;
                    }
                    *buffer = _new;
                }
            }

            size_t find_len = wcslen( fd.cFileName );

            if( dstring_remaining( *buffer ) < find_len + 2 ) {
                _new = dstring_grow( *buffer, find_len + 2 );
                if( !_new ) {
                    cb_error( "path_find_files: failed to add path to list!" );
                    FindClose( f );
                    memory_free( wide_buffer, wide_size );
                    return false;
                }
                *buffer = _new;
            }

            win32_wide_to_narrow_buffer(
                find_len, fd.cFileName,
                find_len + 1, *buffer + dstring_len( *buffer ) ); {
                dstring_header* head = dstring_head( *buffer );
                head->len += find_len;
            }
        }

        *buffer = dstring_push( *buffer, 0 );
        dir_count++;

    } while( FindNextFileW( f, &fd ) );
    FindClose( f );
    memory_free( wide_buffer, wide_size );

    *out_count = dir_count;
    return true;
}
bool_t win32_path_find_dirs_recursive(
    StringSlice base, StringSlice pattern, bool_t is_recursive_level,
    DynamicString** accum, size_t* out_count, DynamicString** buffer
) {
    StringSlice chunk = pattern; {
        size_t sep = 0;
        if( string_slice_find( pattern, '/', &sep ) ) {
            chunk.len = sep;
        }

        if( is_recursive_level ) {
            chunk.len--;
        }
    }

    DynamicString* _new = dstring_append_string_slice( *accum, base );
    if( !_new ) {
        cb_error( "path_find_files: failed to reallocate accumulator!" );
        return false;
    }
    *accum = _new;

    _new = path_push_chunk_string_slice( *accum, chunk );
    if( !_new ) {
        cb_error( "path_find_files: failed to reallocate accumulator!" );
        return false;
    }
    *accum = _new;

    size_t dir_offset = 0;
    size_t buf_len    = dstring_len( *buffer );
    if( buf_len ) {
        dir_offset += buf_len;
    }

    size_t count = 0;
    if( !win32_path_find_dirs( string_slice_from_dstring( *accum ), &count, buffer ) ) {
        return false;
    }

    if( is_recursive_level ) {
        chunk.len++;
    }

    bool_t recursive_dir = string_slice_cmp( chunk, string_slice_text( "**" ) );

    if( recursive_dir ) {
        char* dir = *buffer + dir_offset;
        for( size_t i = 0; i < count; ++i ) {
            dstring_clear( *accum );
            StringSlice subdir = string_slice_new( 0, dir );

            if( string_slice_cmp( subdir, base ) ) {
                goto win32_path_find_dirs_recursive_skip;
            }

            size_t rcount = 0;
            if( !win32_path_find_dirs_recursive(
                subdir, chunk, true,
                accum, &rcount, buffer
            ) ) {
                return false;
            }
            if( !rcount ) {
                break;
            }
            count += rcount;

win32_path_find_dirs_recursive_skip:
            dir_offset += cstr_len( *buffer + dir_offset ) + 1;
            dir         = *buffer + dir_offset;
        }
    }

    *out_count += count;
    pattern = string_slice_advance_by( pattern, chunk.len + 1 );

    if( !pattern.len ) {
        return true;
    }

    char* dir = *buffer + dir_offset;
    for( size_t i = 0; i < count; ++i ) {
        dstring_clear( *accum );

        StringSlice subdir = string_slice_new( 0, dir );

        if( !win32_path_find_dirs_recursive(
            subdir, pattern, false,
            accum, out_count, buffer
        ) ) {
            return false;
        }

        dir_offset += cstr_len( *buffer + dir_offset ) + 1;
        dir         = *buffer + dir_offset;
    }

    return true;
}
bool_t win32_path_find_files(
    size_t dir_count, DynamicString* dirs,
    StringSlice pattern, size_t* out_count, DynamicString** buffer
) {
    size_t largest_dir = 0; {
        char* at = dirs;
        for( size_t dir_index = 0; dir_index < dir_count; ++dir_index ) {
            size_t len = cstr_len( at );
            if( len > largest_dir ) {
                largest_dir = len;
            }
            at += len + 1;
        }
    }

    size_t max_path = sizeof(L"//?/A://") + largest_dir + pattern.len + 255;

    size_t wide_size     = max_path * sizeof(wchar_t);
    wchar_t* wide_buffer = memory_alloc( wide_size );
    if( !wide_buffer ) {
        cb_error( "path_find_files: failed to allocate wide buffer!" );
        return false;
    }
    DynamicString* path = dstring_empty( max_path );
    if( !path ) {
        memory_free( wide_buffer, wide_size );
        cb_error( "path_find_files: failed to allocate path buffer!" );
        return false;
    }

    char*  at = dirs;
    size_t path_count = 0;
    for( size_t dir_index = 0; dir_index < dir_count; ++dir_index ) {
        StringSlice dir = string_slice_new( 0, at );
        at += dir.len + 1; {
            DynamicString* _new = dstring_append_string_slice( path, dir );
            if( !_new ) {
                cb_error( "path_find_files: failed to construct path!" );
                memory_free( wide_buffer, wide_size );
                dstring_free( path );
                return false;
            }
            path = _new;
        } {
            DynamicString* _new = path_push_chunk_string_slice( path, pattern );
            if( !_new ) {
                cb_error( "path_find_files: failed to construct path!" );
                memory_free( wide_buffer, wide_size );
                dstring_free( path );
                return false;
            }
            path = _new;
        }

        wchar_t* wpath =
            win32_path_wide( max_path, wide_buffer, string_slice_from_dstring( path ) );
        assert( wpath ); // assert that wide_buffer was big enough

        WIN32_FIND_DATAW fd;
        memory_zero( &fd, sizeof(fd) );
        HANDLE f = FindFirstFileW( wpath, &fd );

        memory_zero( wide_buffer, max_path * sizeof(wchar_t) );

        path_pop_chunk( path, 0, 0 );
        path = dstring_push( path, '/' ); // guaranteed to work at this point

        if( f == INVALID_HANDLE_VALUE ) {
            dstring_clear( path );
            continue;
        }

        size_t start_len = dstring_len( path );
        do {
            if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                continue;
            }

            StringSlice subpath;
            subpath.len = wcslen( fd.cFileName );
            if( !subpath.len ) {
                continue;
            }

            win32_wide_to_narrow_buffer(
                subpath.len, fd.cFileName,
                max_path * sizeof(wchar_t), (char*)wide_buffer );

            subpath.cc = (char*)wide_buffer; {
                DynamicString* _new = dstring_append_string_slice( path, subpath );
                if( !_new ) {
                    cb_error( "path_find_files: failed to create new path!" );
                    memory_free( wide_buffer, wide_size );
                    dstring_free( path );
                    FindClose( f );
                    return false;
                }
                path = _new;

                dstring_header* header = dstring_head( path );
                header->len++; // include null in string.
            }
            memory_zero( wide_buffer, subpath.len );

            DynamicString* buf = dstring_append_string_slice(
                *buffer, string_slice_from_dstring( path ) );
            if( !buf ) {
                cb_error( "path_find_files: failed to add new path to buffer!" );
                memory_free( wide_buffer, wide_size );
                dstring_free( path );
                FindClose( f );
                return false;
            }
            *buffer = buf;
            dstring_truncate( path, start_len );

            path_count++;
        } while( FindNextFileW( f, &fd ) );
        FindClose( f );

        dstring_clear( path );
    }
    memory_free( wide_buffer, wide_size );
    dstring_free( path );

    if( path_count ) {
        *out_count = path_count;
        return true;
    } else {
        cb_warn( "no files found!" );
        return false;
    }
}
bool_t path_find_files_string_slice( StringSlice path, FindFiles* out_result ) {
    StringSlice file_pattern = string_slice_empty();
    char*  dir             = ".";
    bool_t  dir_allocated   = false;

    if( path_file_name( path, &file_pattern ) && !string_slice_cmp( path, file_pattern ) ) {
        StringSlice remaining_path = path;
        remaining_path.len -= file_pattern.len;
        if( remaining_path.cc[remaining_path.len - 1] == '/' ) {
            remaining_path.len--;
        }

        size_t first_pat = 0;
        if( !string_slice_find_set( remaining_path, string_slice_text( "?*" ), &first_pat ) ) {
            dir = dstring_from_string_slice( &remaining_path );
            if( !dir ) {
                cb_error( "path_find_files: failed to allocate dir!" );
                return false;
            }
            dir_allocated = true;
            goto path_find_files_simple;
        }

        StringSlice base = string_slice_empty(), pat = string_slice_empty(); {
            base     = remaining_path;
            base.len = first_pat;

            size_t sep = 0;
            if( string_slice_find_last( base, '/', &sep ) ) {
                base.len = sep;
                pat      = string_slice_advance_by( remaining_path, sep + 1 );
            } else {
                base = string_slice_text( "./" );
                pat  = remaining_path;
            }
        }

        DynamicString* accum = dstring_empty( 255 );
        if( !accum ) {
            cb_error( "path_find_files: failed to allocate accumulation buffer!" );
            return false;
        }
        size_t   dir_count = 0;
        DynamicString* dirs      = dstring_empty( 255 );
        if( !dirs ) {
            cb_error( "path_find_files: failed to allocate directories buffer!" );
            dstring_free( accum );
            return false;
        }
        bool_t res = win32_path_find_dirs_recursive(
            base, pat, false,
            &accum, &dir_count, &dirs );
        dstring_free( accum );

        if( !res ) {
            dstring_free( dirs );
            return false;
        }

        if( !dir_count ) {
            // no directories matching pattern found
            dstring_free( dirs );
            out_result->count  = 0;
            out_result->paths  = 0;
            out_result->buffer = 0;

            return true;
        }

        DynamicString* file_buf = dstring_empty( 255 );

        if( !file_buf ) {
            dstring_free( dirs );
            cb_error( "path_find_files: failed to allocate files buffer!" );
            return false;
        }

        size_t file_count = 0;
        res = win32_path_find_files(
            dir_count, dirs, file_pattern, &file_count, &file_buf );

        dstring_free( dirs );

        if( !res ) {
            dstring_free( file_buf );
            return false;
        }

        if( !file_buf ) {
            dstring_free( file_buf );
            out_result->buffer = 0;
            out_result->paths  = 0;
            out_result->count  = 0;
            return true;
        }

        StringSlice* files = darray_empty( StringSlice, file_count );
        if( !files ) {
            dstring_free( file_buf );
            cb_error( "path_find_files: failed to allocate files array!" );
            return false;
        }

        StringSlice remaining_files = string_slice_from_dstring( file_buf );
        for(
            size_t file_index = 0;
            file_index < file_count && remaining_files.len;
            ++file_index
        ) {
            StringSlice file = remaining_files;
            file.len       = cstr_len( file.cc );

            darray_push( files, &file );

            remaining_files = string_slice_advance_by( remaining_files, file.len + 1 );
        }

        out_result->buffer = file_buf;
        out_result->paths  = files;
        out_result->count  = file_count;
        return true;
    }

    dir          = ".";
    file_pattern = path;

path_find_files_simple:
    {
        DynamicString* buf = dstring_empty(255);
        if( !buf ) {
            cb_error( "path_find_files: failed to allocate buffer!" );
            return false;
        }

        size_t path_count = 0;
        bool_t res = win32_path_find_files( 1, dir, file_pattern, &path_count, &buf );

        if( dir_allocated ) {
            dstring_free( dir );
        }
        if( !res ) {
            dstring_free( buf );
            return false;
        }

        StringSlice* paths = darray_empty( StringSlice, path_count );
        if( !paths ) {
            cb_error( "path_find_files: failed to allocate paths array!" );
            dstring_free( buf );
            return false;
        }

        StringSlice subbuf = string_slice_from_dstring( buf );
        for( size_t i = 0; i < path_count && subbuf.len; ++i ) {
            StringSlice path = subbuf;
            path.len = cstr_len( path.cc );
            darray_push( paths, &path );

            subbuf = string_slice_advance_by( subbuf, path.len + 1 );
        }

        out_result->count  = darray_len( paths );
        out_result->paths  = paths;
        out_result->buffer = buf;
        return true;
    }
}

bool_t mutex_create( Mutex* out_mtx ) {
    HANDLE handle = CreateMutexA( NULL, FALSE, NULL );
    if( !handle ) {
        cb_error( "mutex_create: failed to create mutex!" );
        return false;
    }
    out_mtx->handle = handle;
    return true;
}
void mutex_destroy( Mutex* mtx ) {
    CloseHandle( mtx->handle );
    mtx->handle = 0;
}
bool_t mutex_lock( Mutex* mtx, uint32_t ms ) {
    memory_fence();

    DWORD res = WaitForSingleObject( mtx->handle, ms );

    memory_fence();
    if( ms == UINT32_MAX ) {
        return true;
    }
    return res != WAIT_TIMEOUT;
}
void mutex_unlock( Mutex* mtx ) {
    ReleaseMutex( mtx->handle );
}

bool_t semaphore_create( Semaphore* out_sem ) {
    HANDLE handle = CreateSemaphoreA( NULL, 0, INT32_MAX, 0 );
    if( !handle ) {
        return false;
    }
    out_sem->handle = handle;
    return true;
}
void semaphore_destroy( Semaphore* sem ) {
    CloseHandle( sem->handle );
    sem->handle = 0;
}
bool_t semaphore_wait( Semaphore* sem, uint32_t ms ) {
    DWORD res = WaitForSingleObject( sem->handle, ms );

    if( ms == UINT32_MAX ) {
        return true;
    }
    return res != WAIT_TIMEOUT;
}
void semaphore_signal( Semaphore* sem ) {
    ReleaseSemaphore( sem->handle, 1, NULL );
}
struct Win32ThreadParams {
    JobFN*   proc;
    void*    params;
    uint32_t id;
};
volatile Atom global_thread_id = 1;

unsigned int thread_proc( void* params ) {
    struct Win32ThreadParams* p = params;
    p->proc( p->id, p->params );
    memory_fence();
    memory_free( p, sizeof(*p) );

    return 0;
}
bool_t internal_thread_create( JobFN* thread, void* params ) {
    struct Win32ThreadParams* p = memory_alloc( sizeof( *p ) );
    if( !p ) {
        cb_error( "failed to create thread!" );
        return false;
    }
    p->id     = atomic_add( &global_thread_id, 1 );
    p->proc   = thread;
    p->params = params;
    memory_fence();

    HANDLE h = (HANDLE)_beginthreadex( 0, 0, thread_proc, p, 0, 0 );

    if( !h ) {
        cb_error( "failed to create thread!" );
        return false;
    }

    return true;
}

bool_t file_open( const char* path, FileFlags flags, File* out_file ) {
    if( !file_flags_valid( flags ) ) {
        return false;
    }

    DWORD dwDesiredAccess       = 0;
    if( flags & FF_READ ) {
        dwDesiredAccess |= GENERIC_READ;
    }
    if( flags & FF_WRITE ) {
        dwDesiredAccess |= GENERIC_WRITE;
    }
    DWORD dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    if( flags & FF_CREATE ) {
        dwCreationDisposition = CREATE_ALWAYS;
    } else if( flags & FF_TRUNCATE ) {
        dwCreationDisposition = TRUNCATE_EXISTING;
    }
    DWORD dwFlagsAndAttributes = 0;

    wchar_t* wpath = win32_path( string_slice_new( 0, path ) );
    if( !wpath ) {
        cb_error( "file_open: failed to allocate path buffer!" );
        return false;
    }
    HANDLE handle = CreateFileW(
        wpath, dwDesiredAccess, dwShareMode, NULL,
        dwCreationDisposition, dwFlagsAndAttributes, NULL );

    if( handle == INVALID_HANDLE_VALUE ) {
        DWORD err    = GetLastError();
        wchar_t* buf = 0;
        DWORD len    = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            0, err, 0, (LPWSTR)&buf, 0, 0 );

        if( len ) {
            if( len > 2 ) {
                buf[len - 1] = 0;
            }
            cb_error( "file_open: failed to open file '%s'! reason: %S", path, buf );
            LocalFree( buf );
        } else {
            cb_error( "file_open: failed to open file '%s'!", path );
        }

        return false;
    }

    File result;
    memory_zero( &result, sizeof(result) );
    result.handle = handle; {
        BY_HANDLE_FILE_INFORMATION fi;
        memory_zero( &fi, sizeof(fi) );
        if( !GetFileInformationByHandle( result.handle, &fi ) ) {
            cb_warn( "file_open: failed to query file info!" );
        } else {
            ULARGE_INTEGER sz;
            sz.LowPart  = fi.nFileSizeLow;
            sz.HighPart = fi.nFileSizeHigh;
            result.size = sz.QuadPart;

            result.create = win32_filetime_to_posix_time( fi.ftCreationTime );
            result.modify = win32_filetime_to_posix_time( fi.ftLastWriteTime );

            if( fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                result.type = FT_DIRECTORY;
            } else {
                result.type = FT_FILE;
            }
        }
    }

    if( flags & FF_APPEND ) {
        file_seek( result.handle, FS_BEGIN, result.size );
    }

    result.flags = flags;
    *out_file    = result;
    return true;
}
bool_t file_path_exists( const char* path ) {
    wchar_t* wpath = win32_path( string_slice_new( 0, path ) );
    if( !wpath ) {
        cb_error( "file_path_exists: failed to allocate wide path buffer!" );
        return false;
    }

    DWORD dwDesiredAccess       = 0;
    DWORD dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes  = 0;
    HANDLE handle = CreateFileW(
        wpath, dwDesiredAccess, dwShareMode,
        NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL );

    if( handle == INVALID_HANDLE_VALUE ) {
        return false;
    }

    CloseHandle( handle );
    return true;
}
bool_t file_path_move( const char* dst, const char* src ) {
    wchar_t* wdst = win32_path( string_slice_new( 0, dst ) );
    if( !wdst ) {
        cb_error( "file_path_move: failed to allocate path buffer!" );
        return false;
    }
    wchar_t* wsrc = win32_path( string_slice_new( 0, src ) );
    if( !wsrc ) {
        cb_error( "file_path_move: failed to allocate path buffer!" );
        return false;
    }

    bool_t res = MoveFileW( wsrc, wdst ) != FALSE;
    if( !res ) {
        cb_error( "file_path_move: failed to move %S to %S!", wsrc, wdst );
    }
    return res;
}
bool_t file_path_copy( const char* dst, const char* src ) {
    wchar_t* wdst = win32_path( string_slice_new( 0, dst ) );
    if( !wdst ) {
        cb_error( "file_path_copy: failed to allocate path buffer!" );
        return false;
    }
    wchar_t* wsrc = win32_path( string_slice_new( 0, src ) );
    if( !wsrc ) {
        cb_error( "file_path_copy: failed to allocate path buffer!" );
        return false;
    }

    bool_t res = CopyFileW( wsrc, wdst, FALSE ) != FALSE;
    if( !res ) {
        cb_error( "file_path_copy: failed to copy %S to %S!", wsrc, wdst );
    }
    return res;
}
bool_t file_path_remove( const char* file ) {
    wchar_t* wfile = win32_path( string_slice_new( 0, file ) );
    if( !wfile ) {
        cb_error( "file_path_remove: failed to allocate path buffer!" );
        return false;
    }

    bool_t res = DeleteFileW( wfile ) != FALSE;
    if( !res ) {
        cb_error( "file_path_remove: failed to remove file %S!", wfile );
    }
    return res;
}
bool_t file_write( File* file, size_t size, const void* buf ) {
#if UINTPTR_MAX == UINT64_MAX
    DWORD bytes_written = 0;
    DWORD part0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD part1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    if( part0 ) {
        BOOL res = WriteFile( file->handle, buf, part0, &bytes_written, NULL );
        if( !res ) {
            return false;
        }
        file->size += bytes_written;
    }
    if( part1 ) {
        BOOL res = WriteFile(
            file->handle, (char*)buf + part0, part1, &bytes_written, NULL );
        if( !res ) {
            return false;
        }
        file->size += bytes_written;
    }
    return true;
#else
    DWORD bytes_written = 0;
    BOOL res = WriteFile( file->handle, buf, size, &bytes_written, NULL ) == TRUE;
    file->size += bytes_written;
    return res;
#endif
}
bool_t file_read( File* file, size_t size, void* buf ) {
#if UINTPTR_MAX == UINT64_MAX
    DWORD bytes_read = 0;
    DWORD part0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD part1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    if( part0 ) {
        BOOL res = ReadFile( file->handle, buf, part0, &bytes_read, NULL );
        if( !res ) {
            return false;
        }
    }
    if( part1 ) {
        BOOL res = ReadFile(
            file->handle, (char*)buf + part0, part1, &bytes_read, NULL );
        if( !res ) {
            return false;
        }
    }
    return true;
#else
    DWORD bytes_read = 0;
    return ReadFile( file->handle, buf, size, &bytes_read, NULL ) == TRUE;
#endif

}
void file_close( File* file ) {
    if( file && file->handle ) {
        CloseHandle( file->handle );
    }
    memory_zero( file, sizeof(*file) );
}
bool_t file_check( File* file ) {
    if( file ) {
        if( !file->handle || file->handle == INVALID_HANDLE_VALUE ) {
            return false;
        }
        return true;
    }
    return false;
}
bool_t file_truncate( File* file ) {
    if( SetEndOfFile( file->handle ) == FALSE ) {
        cb_error( "file_truncate: failed to truncate file!" );
        return false;
    }

    LARGE_INTEGER li;
    memory_zero( &li, sizeof(li) );
    if( GetFileSizeEx( file->handle, &li ) ) {
        file->size = li.QuadPart;
    }
    return true;
}
bool_t file_seek( File* file, FileSeek type, ssize_t seek ) {
    LARGE_INTEGER li;
    li.QuadPart = seek;
    DWORD dwMoveMethod = 0;
    switch( seek ) {
        case FS_BEGIN: {
            dwMoveMethod = FILE_BEGIN;
        } break;
        case FS_END: {
            dwMoveMethod = FILE_END;
        } break;
        default: {
            dwMoveMethod = FILE_CURRENT;
        } break;
    }
    if( SetFilePointerEx( file->handle, li, NULL, dwMoveMethod ) == FALSE ) {
        cb_error( "file_seek: failed to seek!" );
        return false;
    }

    memory_zero( &li, sizeof(li) );
    if( GetFileSizeEx( file->handle, &li ) ) {
        file->size = li.QuadPart;
    }

    return true;
}
bool_t file_query_seek( File* file, ssize_t* out_seek ) {
    LARGE_INTEGER move;
    move.QuadPart = 0;
    LARGE_INTEGER li;
    if( SetFilePointerEx( file->handle, move, &li, FILE_CURRENT ) == FALSE ) {
        cb_error( "file_query_seek: failed to query file pointer!" );
        return false;
    }
    *out_seek = li.QuadPart;
    return true;
}
bool_t file_path_query_create( const char* path, time_t* out_time ) {
    wchar_t* wpath = win32_path( string_slice_new( 0, path ) );
    if( !wpath ) {
        cb_error( "file_path_query_create: failed to allocate path buffer!" );
        return false;
    }
    HANDLE handle = CreateFileW(
        wpath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL );
    if( handle == INVALID_HANDLE_VALUE ) {
        cb_error( "file_path_query_create: failed to open '%s'!", path );
        return false;
    }

    FILETIME ft;
    memory_zero( &ft, sizeof(ft) );
    bool_t res = GetFileTime( handle, &ft, NULL, NULL );
    CloseHandle( handle );

    if( !res ) {
        cb_error( "file_path_query_create: failed to get file time!" );
        return false;
    }

    *out_time = win32_filetime_to_posix_time( ft );
    return true;
}
bool_t file_path_query_modify( const char* path, time_t* out_time ) {
    wchar_t* wpath = win32_path( string_slice_new( 0, path ) );
    if( !wpath ) {
        cb_error( "file_path_query_modify: failed to allocate path buffer!" );
        return false;
    }
    HANDLE handle = CreateFileW(
        wpath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL );
    if( handle == INVALID_HANDLE_VALUE ) {
        cb_error( "file_path_query_modify: failed to open '%s'!", path );
        return false;
    }

    FILETIME ft;
    memory_zero( &ft, sizeof(ft) );
    bool_t res = GetFileTime( handle, NULL, NULL, &ft );
    CloseHandle( handle );

    if( !res ) {
        cb_error( "file_path_query_modify: failed to get file time!" );
        return false;
    }

    *out_time = win32_filetime_to_posix_time( ft );
    return true;
}

void platform_init(void) {
    void* path_buffers =
        memory_alloc( (sizeof(wchar_t) * WIN32_MAX_PATH_BUFFERS * PATH_MAX_LEN) );
    assert( path_buffers );
    global_win32_path_buffer = path_buffers;
    memory_fence();

    QueryPerformanceFrequency( &global_qpf );
}
uint32_t atomic_add( Atom* ptr, uint32_t val ) {
    uint32_t res = _InterlockedExchangeAdd( (volatile long*)ptr, val );
    return res;
}
uint32_t atomic_sub( Atom* ptr, uint32_t val ) {
    uint32_t res = _InterlockedExchangeAdd( (volatile long*)ptr, -((long)val) );
    return res;
}
uint64_t atomic_add64( Atom64* ptr, uint64_t val ) {
    uint64_t res =
        _InterlockedExchangeAdd64( (volatile long long*)ptr, val );
    return res;
}
uint64_t atomic_sub64( Atom64* ptr, uint64_t val ) {
    uint64_t res =
        _InterlockedExchangeAdd64( (volatile long long*)ptr, -((long long)val) );
    return res;
}

void thread_sleep( uint32_t ms ) {
    Sleep( ms );
}
DynamicString* path_pwd(void) {
    dstring_header* res = dstring_head( dstring_empty( MAX_PATH ) );
    if( !res ) {
        return NULL;
    }

    DWORD len = GetCurrentDirectoryA( res->cap, res->buf );
    if( !len ) {
        dstring_free( res->buf );
        return NULL;
    }
    res->len = len;

    return res->buf;
}
double time_seconds(void) {
    LARGE_INTEGER qpc;
    QueryPerformanceCounter( &qpc );
    return (double)qpc.QuadPart / (double)global_qpf.QuadPart;
}
double time_milliseconds(void) {
    return time_seconds() * 1000.0;
}
bool_t program_in_path( const char* prog ) {
    DWORD dwCreationFlags = CREATE_SUSPENDED;
    STARTUPINFOA startup;
    memory_zero( &startup, sizeof(startup) );
    startup.cb = sizeof(startup);

    PROCESS_INFORMATION info;
    memory_zero( &info, sizeof(info) );

    bool_t res = CreateProcessA(
        NULL, (char*)prog, NULL, NULL, FALSE,
        dwCreationFlags, NULL, NULL, &startup, &info );

    if( res ) {
        TerminateProcess( info.hProcess, 0 );
        CloseHandle( info.hProcess );
        CloseHandle( info.hThread );
    }

    return res;
}
bool_t pipe_create( Pipe* out_read, Pipe* out_write ) {
    SECURITY_ATTRIBUTES attr;
    memory_zero( &attr, sizeof( attr ) );
    attr.nLength        = sizeof(attr);
    attr.bInheritHandle = TRUE;

    HANDLE pipes[2];
    bool_t res = CreatePipe( pipes + 0, pipes + 1, &attr, kibibytes(64) );
    if( !res ) {
        cb_error( "pipe_create: failed to create pipes!" );
        return false;
    }

    *out_read  = (Pipe){ .handle = pipes[0] };
    *out_write = (Pipe){ .handle = pipes[1] };

    return true;
}
uint32_t pipe_peek( Pipe* rpipe ) {
    DWORD bytes_available = 0;
    if( PeekNamedPipe( rpipe->handle, 0, 0, 0, &bytes_available, 0 ) ) {
        return bytes_available;
    }
    return 0;
}
bool_t pipe_read(
    Pipe* rpipe, uint32_t buf_size, void* buf, uint32_t* out_read_size
) {
    if( !pipe_peek( rpipe ) ) {
        return false;
    }
    DWORD read_size = 0;
    bool_t res = ReadFile( rpipe->handle, buf, buf_size, &read_size, NULL );
    if( res ) {
        *out_read_size = read_size;
        return true;
    }
    return false;
}
bool_t pipe_write(
    Pipe* wpipe, uint32_t buf_size, const void* buf, uint32_t* out_write_size
) {
    DWORD write_size = 0;
    bool_t res = WriteFile( wpipe->handle, buf, buf_size, &write_size, NULL );
    if( res ) {
        *out_write_size = write_size;
        return true;
    }
    return false;
}
void pipe_close( Pipe* pipe ) {
    if( pipe && pipe->handle ) {
        CloseHandle( pipe->handle );
        memory_zero( pipe, sizeof(*pipe) );
    }
}
bool_t pipe_check( Pipe* pipe ) {
    if( pipe ) {
        if( !pipe->handle || pipe->handle == INVALID_HANDLE_VALUE ) {
            return false;
        }
        return true;
    }
    return false;
}
bool_t program_exec_async(
    const char* command_line,
    Pipe* opt_stdin_r, Pipe* opt_stdout_w, Pipe* opt_stderr_w,
    ProcessID* out_pid
) {
    STARTUPINFOA        startup;
    PROCESS_INFORMATION info;

    memory_zero( &startup, sizeof(startup) );
    memory_zero( &info, sizeof(info) );

    startup.cb         = sizeof(startup);
    startup.hStdInput  = GetStdHandle( STD_INPUT_HANDLE );
    startup.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    startup.hStdError  = GetStdHandle( STD_ERROR_HANDLE );

    bool_t bInheritHandle = FALSE;
    if( opt_stdin_r ) {
        startup.hStdInput = opt_stdin_r->handle;
        bInheritHandle    = TRUE;
    }
    if( opt_stdout_w ) {
        startup.hStdOutput = opt_stdout_w->handle;
        bInheritHandle     = TRUE;
    }
    if( opt_stderr_w ) {
        startup.hStdError = opt_stderr_w->handle;
        bInheritHandle    = TRUE;
    }

    if( bInheritHandle ) {
        startup.dwFlags |= STARTF_USESTDHANDLES;
    }

    DWORD flags = 0;

    bool_t res = CreateProcessA(
        NULL, (char*)command_line, NULL, NULL,
        bInheritHandle, flags, NULL, NULL, &startup, &info );

    if( !res ) {
        cb_error( "failed to run command '%s'!", command_line );
        return false;
    }

    CloseHandle( info.hThread );
    out_pid->handle = info.hProcess;
    return true;
}
bool_t program_wait( ProcessID pid, uint32_t ms, int* opt_out_res ) {
    if( WaitForSingleObject( pid.handle, ms ) == WAIT_TIMEOUT ) {
        cb_error( "process %p timed out!", pid.handle );
        return false;
    }
    if( opt_out_res ) {
        DWORD exit_code = 0;
        if( !GetExitCodeProcess( pid.handle, &exit_code ) ) {
            cb_warn( "process %p finished but failed to get return code!", pid.handle );
            return true;
        }
        *opt_out_res = exit_code;
    }

    return true;
}
void program_terminate( ProcessID pid ) {
    TerminateProcess( pid.handle, 0 );
    CloseHandle( pid.handle );
}
void program_close( ProcessID pid ) {
    CloseHandle( pid.handle );
}

#else /* Platform Windows */

// NOTE(alicia): SPOSIX

// TODO(alicia): MAJOR CHANGES

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

void platform_init(void) {
    return;
}
bool_t file_open( const char* path, FileOpenFlags flags, File* out_file ) {
    int oflag = 0;
    if(
        (flags & (FILE_OPEN_FLAG_READ | FILE_OPEN_FLAG_WRITE)) ==
        (FILE_OPEN_FLAG_READ | FILE_OPEN_FLAG_WRITE)
    ) {
        oflag = O_RDWR;
        if( flags & FILE_OPEN_FLAG_TRUNCATE ) {
            oflag |= O_TRUNC;
        }
    } else if( flags & FILE_OPEN_FLAG_READ ) {
        oflag = O_RDONLY;
        if( flags & FILE_OPEN_FLAG_TRUNCATE ) {
            cb_error(
                "file_open: flags invalid! can't read and truncate! path: %s", path );
            return false;
        }
    } else if( flags & FILE_OPEN_FLAG_WRITE ) {
        oflag = O_WRONLY;
        if( flags & FILE_OPEN_FLAG_TRUNCATE ) {
            oflag |= O_TRUNC;
        }
    } else {
        cb_error( "file_open: flags invalid! path: '%s'", path );
        return false;
    }

    if( flags & FILE_OPEN_FLAG_CREATE ) {
        oflag |= O_CREAT;
    }
    if( flags & FILE_OPEN_FLAG_APPEND ) {
        oflag |= O_APPEND;
    }
    int fd = open( path, oflag );
    if( fd < 0 ) {
        cb_error( "file_open: failed to open '%s'!", path );
        return false;
    }

    *out_file = fd;
    return true;
}
bool_t file_write( File file, size_t size, const void* buf ) {
    return write( file, buf, size ) != -1;
}
bool_t file_read( File file, size_t size, void* buf ) {
    return read( file, buf, size ) != -1;
}
void file_close( File file ) {
    close( file );
}
bool_t file_truncate( File file ) {
    off_t pos = lseek( file, 0, SEEK_CUR );
    return ftruncate( file, pos ) == 0;
}
bool_t file_query_size( File file, size_t* out_size ) {
    off_t pos  = lseek( file, 0, SEEK_CUR );
    if( pos < 0 ) {
        cb_error( "file_query_size: failed to get current position!" );
        return false;
    }
    off_t size = lseek( file, 0, SEEK_END );
    lseek( file, pos, SEEK_SET );

    *out_size = size;
    return true;
}
bool_t file_seek( File file, FileSeek type, ssize_t seek ) {
    int whence = 0;
    switch( type ) {
        case FS_BEGIN: {
            whence = SEEK_SET;
        } break;
        case FS_END: {
            whence = SEEK_END;
        } break;
        default: {
            whence = SEEK_CUR;
        } break;
    }
    return lseek( file, seek, whence ) >= 0;
}
bool_t file_query_seek( File file, ssize_t* out_seek ) {
    off_t pos = lseek( file, 0, SEEK_CUR );
    if( pos < 0 ) {
        cb_error( "file_query_seek: failed to get current position!" );
        return false;
    }
    *out_seek = pos;
    return true;
}
bool_t file_query_info( File file, FileInfo* out_info ) {
    struct stat st;
    memory_zero( &st, sizeof(st) );
    if( fstat( file, &st ) < 0 ) {
        cb_error( "file_query_info: failed to stat %d!", file );
        return false;
    }

    out_info->file_size   = st.st_size;
    out_info->time_create = st.st_ctime;
    out_info->time_modify = st.st_mtime;

    if( S_ISREG( st.st_mode ) ) {
        out_info->type = FILE_TYPE_FILE;
    } else if( S_ISDIR( st.st_mode ) ) {
        out_info->type = FILE_TYPE_DIRECTORY;
    }

    return true;
}
bool_t file_query_info_path( const char* path, FileInfo* out_info ) {
    File fd;
    if( !file_open( path, FILE_OPEN_FLAG_READ, &fd ) ) {
        cb_error( "file_query_info_path: failed to open file '%s'", path );
        return false;
    }
    bool_t res = file_query_info( fd, out_info );
    file_close( fd );
    return res;
}
bool_t file_move( const char* dst, const char* src ) {
    if( rename( src, dst ) == 0 ) {
        return true;
    } else {
        cb_error( "failed to move '%s' to '%s'", src, dst );
        return false;
    }
}
bool_t file_copy( const char* dst, const char* src ) {
    #define MAX_COPY_BUF (1024)
    void* copy_buf = memory_alloc( MAX_COPY_BUF );
    if( !copy_buf ) {
        cb_error( "file_copy: failed to allocate copy buffer!" );
        return false;
    }

    File src_file, dst_file;
    if( !file_open( src, FILE_OPEN_FLAG_READ, &src_file ) ) {
        cb_error( "file_copy: failed to open source '%s'", src );
        memory_free( copy_buf );
        return false;
    }
    FileOpenFlags dst_flags = FILE_OPEN_FLAG_WRITE;
    // check if file exists
    if( file_open( dst, FILE_OPEN_FLAG_READ, &dst_file ) ) {
        file_close( dst_file );
        dst_flags |= FILE_OPEN_FLAG_TRUNCATE;
    } else {
        dst_flags |= FILE_OPEN_FLAG_CREATE;
    }
    if( !file_open( dst, dst_flags, &dst_file ) ) {
        cb_error( "file_copy: failed to open dest '%s'", dst );
        file_close( src_file );
        memory_free( copy_buf );
        return false;
    }

    off_t src_size = lseek( src_file, 0, SEEK_END );
    if( src_size < 0 ) {
        cb_error( "file_copy: failed to get source '%s' size!", src );
        file_close( src_file );
        file_close( dst_file );
        memory_free( copy_buf );
        return false;
    }
    lseek( src_file, 0, SEEK_SET );

    while( src_size > 0 ) {
        off_t read_size = MAX_COPY_BUF;
        if( read_size > src_size ) {
            read_size = src_size;
        }
        if( !file_read( src_file, read_size, copy_buf ) ) {
            cb_error( "file_copy: failed to read '%s'", src );
            file_close( src_file );
            file_close( dst_file );
            memory_free( copy_buf );
            return false;
        }
        if( !file_write( dst_file, read_size, copy_buf ) ) {
            cb_error( "file_copy: failed to write '%s'", dst );
            file_close( src_file );
            file_close( dst_file );
            memory_free( copy_buf );
            return false;
        }
        src_size -= read_size;
    }

    file_close( src_file );
    file_close( dst_file );
    memory_free( copy_buf );

    return true;
}
bool_t file_remove( const char* path ) {
    if( remove(path) == 0 ) {
        return true;
    } else {
        cb_error( "failed to remove file '%s'", path );
        return false;
    }
}

bool_t program_exec_async( const char* command_line, ProcessID* out_pid ) {
    pid_t pid = fork();
    if( pid < 0 ) {
        return false;
    }

    if( pid ) {
        *out_pid = pid;
        return true;
    } else {
        exit( system( command_line ) );
    }
}
bool_t program_wait( ProcessID pid, uint32_t ms, int* opt_out_res ) {
    if( ms != UINT32_MAX ) {
        double start   = time_milliseconds();
        double elapsed = 0.0;
        double target  = (double)ms;

        int status = 0;

        struct timespec ts;
        ts.tv_sec  = 0;
        ts.tv_nsec = 1000000;

        while( elapsed < target ) {
            waitpid( pid, &status, WNOHANG );
            if( WIFEXITED( status ) ) {
                if( opt_out_res ) {
                    *opt_out_res = status;
                    return true;
                }
            }

            elapsed = time_milliseconds() - start;
            nanosleep( &ts, NULL );
        }
        return false;
    } else {
        int status = 0;
        waitpid( pid, &status, 0 );
        if( opt_out_res ) {
            *opt_out_res = WEXITSTATUS(status);
        }
        return true;
    }
}
void program_terminate( ProcessID pid ) {
    kill( pid, SIGINT);
}
void program_close( ProcessID pid ) {
    unused(pid);
}

bool_t program_in_path( const char* prog ) {
    DynamicString* cmd = dstring_fmt( "which %s 2>&1 /dev/null", prog );

    if( !cmd ) {
        cb_error( "program_in_path: failed to allocate command buffer!" );
        return false;
    }

    bool_t result = system( cmd ) == 0;
    dstring_free( cmd );

    return result;
}
double time_seconds(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return (double)ts.tv_sec + (double)(ts.tv_nsec / 1000000000.0);
}
double time_milliseconds(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return ((double)ts.tv_sec * 1000.0) + ((double)ts.tv_nsec / 1000000.0);
}
DynamicString* path_pwd(void) {
    dstring_header* res = dstring_head( dstring_empty( 1024 ) );
    if( !res ) {
        return NULL;
    }
    
    if( !getcwd( res->buf, res->cap ) ) {
        memory_free( res );
        return NULL;
    }
    res->len = cstr_len( res->buf );

    return res->buf;
}

unsigned int atomic_add( Atom* ptr, unsigned int val ) {
    unsigned int res =
#if defined(__STDC_NO_ATOMICS__)
    __sync_fetch_and_add( (ptr), (val) );
#else
    atomic_fetch_add( ptr, val );
#endif
    return res;
}
unsigned long atomic_add64( Atom64* ptr, unsigned long val ) {
    unsigned long res =
#if defined(__STDC_NO_ATOMICS__)
    __sync_fetch_and_add( (ptr), (val) );
#else
    atomic_fetch_add( ptr, val );
#endif
    return res;
}

#endif /* Platform POSIX */

#endif /* Implementation */

