#if !defined(CBUILD_HEADER)
#define CBUILD_HEADER
/**
 * @file   cbuild.h
 * @brief  C Build System.
 * @details
 * Single header library for writing a build system in C.
 * Include to get API, include again (ONCE) with CBUILD_IMPLEMENTATION
 * defined to define implementation.
 *
 * Additional options can be defined before first include.
 *
 * Additional options:
 * - define CBUILD_THREAD_COUNT with number between 2 and 16 to set how many
 *   threads cbuild is allowed to initialize.
 * - define CBUILD_ASSERTIONS to get debug assertions in implementation and
 *   access to assertion() macro. The assertion() macro is otherwise defined
 *   to be unused( ... )
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 28, 2024
*/
// IWYU pragma: begin_exports
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
// IWYU pragma: end_exports

#if defined(__clang__)
    /// @brief Current compiler is clang (LLVM).
    #define COMPILER_CLANG 1
#elif defined(__GNUC__)
    /// @brief Current compiler is GCC.
    #define COMPILER_GCC 1
#elif defined(_MSC_VER)
    /// @brief Current compiler is Microsoft Visual C++.
    #define COMPILER_MSVC 1
#else
    /// @brief Current compiler is unknown.
    #define COMPILER_UNKNOWN 1
#endif

#if defined(_WIN32)
    /// @brief Current platform is Windows.
    #define PLATFORM_WINDOWS 1
    #if defined(__MINGW32__) || defined(__MINGW64__)
        /// @brief C Build compiled using MinGW subsystem.
        #define PLATFORM_MINGW 1
    #endif
#elif defined(__linux__)
    /// @brief Current platform is GNU/Linux.
    #define PLATFORM_LINUX 1
    /// @brief Current platform is POSIX compliant.
    #define PLATFORM_POSIX 1
#elif defined(__APPLE__)
    /// @brief Current platform is MacOS.
    #define PLATFORM_MACOS 1
    /// @brief Current platform is POSIX compliant.
    #define PLATFORM_POSIX 1
#else
    /// @brief Current platform is unknown.
    #define PLATFORM_UNKNOWN 1
    #if __has_include(<unistd.h>)
        /// @brief Current platform is POSIX compliant.
        #define PLATFORM_POSIX 1
    #endif
#endif

#if defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64)
    /// @brief Current CPU architecture is ARM-based.
    #define ARCH_ARM 1
#elif defined(__i386__) || defined(_M_IX86) || defined(__X86__) || defined(__x86_64__)
    /// @brief Current CPU architecture is x86 or x86-64.
    #define ARCH_X86 1
#else
    /// @brief Current CPU architecture is unknown.
    #define ARCH_UNKNOWN 1
#endif

#if UINT64_MAX == UINTPTR_MAX
    /// @brief Current CPU architecture is 64-bit.
    #define ARCH_64BIT 1
#else
    /// @brief Current CPU architecture is 32-bit.
    #define ARCH_32BIT 1
#endif

/// @brief Maximum path length expected.
#define CBUILD_PATH_CAPACITY (4096)

#if defined(PLATFORM_WINDOWS)
    #undef CBUILD_PATH_CAPACITY
    /// @brief Maximum path length expected.
    #define CBUILD_PATH_CAPACITY (8192) // this is to allow wide paths the same max length as narrow paths
#endif

/// @brief Number of local buffers per thread.
#define CBUILD_LOCAL_BUFFER_COUNT (4)
/// @brief Capacity of local buffers in bytes.
#define CBUILD_LOCAL_BUFFER_CAPACITY CBUILD_PATH_CAPACITY

/// @brief Maximum number of MT jobs.
#define CBUILD_MAX_JOBS (32)

/// @brief Minimum number of threads allowed.
#define CBUILD_THREAD_COUNT_MIN (1)
/// @brief Maximum number of threads allowed.
#define CBUILD_THREAD_COUNT_MAX (16)

#if !defined(CBUILD_THREAD_COUNT)
    /// @brief Number of threads to be spawned for job system.
    #define CBUILD_THREAD_COUNT (8)
#else
    #if CBUILD_THREAD_COUNT > CBUILD_THREAD_COUNT_MAX
        /// @brief Number of threads to be spawned for job system.
        #define CBUILD_THREAD_COUNT CBUILD_THREAD_COUNT_MAX
    #elif CBUILD_THREAD_COUNT < CBUILD_THREAD_COUNT_MIN
        /// @brief Number of threads to be spawned for job system.
        #define CBUILD_THREAD_COUNT CBUILD_THREAD_COUNT_MIN
    #endif
#endif

#if defined(COMPILER_MSVC)
    /// @brief Cross-compiler macro for declaring a thread local variable.
    #define make_tls( type )\
        __declspec( thread ) type
#else
    /// @brief Cross-compiler macro for declaring a thread local variable.
    #define make_tls( type )\
        _Thread_local type
#endif

/// @brief 8-bit unsigned.
typedef uint8_t   u8;
/// @brief 16-bit unsigned.
typedef uint16_t  u16;
/// @brief 32-bit unsigned.
typedef uint32_t  u32;
/// @brief 64-bit unsigned.
typedef uint64_t  u64;
/// @brief Pointer sized unsigned.
typedef uintptr_t usize;

/// @brief 8-bit signed.
typedef int8_t   i8;
/// @brief 16-bit signed.
typedef int16_t  i16;
/// @brief 32-bit signed.
typedef int32_t  i32;
/// @brief 64-bit signed.
typedef int64_t  i64;
/// @brief Pointer sized signed.
typedef intptr_t isize;

/// @brief Single-precision float.
typedef float  f32;
/// @brief Double-precision float.
typedef double f64;

/// @brief 8-bit Boolean, useful for struct packing.
/// @details When returned from library, guaranteed to be 0 or 1.
typedef u8  b8;
/// @brief 16-bit Boolean, useful for struct packing.
/// @details When returned from library, guaranteed to be 0 or 1.
typedef u16 b16;
/// @brief 32-bit Boolean, useful for struct packing.
/// @details When returned from library, guaranteed to be 0 or 1.
typedef u32 b32;
/// @brief 64-bit Boolean, useful for struct packing.
/// @details When returned from library, guaranteed to be 0 or 1.
typedef u64 b64;

#if defined(PLATFORM_WINDOWS)
    /// @brief Volatile 32-bit signed for atomic operations.
    typedef volatile long      atom;
    /// @brief Volatile 64-bit signed for atomic operations.
    typedef volatile long long atom64;
#else
    /// @brief Volatile 32-bit signed for atomic operations.
    typedef volatile i32 atom;
    /// @brief Volatile 64-bit signed for atomic operations.
    typedef volatile i64 atom64;
#endif

/// @brief String is UTF-8 encoded and null-terminated.
typedef char cstr;
/// @brief String is dynamically allocated on the heap,
/// UTF-8 encoded and null-terminated.
typedef char dstring;
/// @brief String slice. Not necessarily null-terminated.
/// @note When printing, use "%.*s" format specifier and
/// provide length followed by pointer in args.
typedef struct {
    /// @brief ASCII length of string.
    usize len;
    /// @brief Pointer to start of string buffer.
    const char* cc;
} string;
/// @brief Logger levels.
typedef enum {
    /// @brief Info level.
    /// @details
    /// Most permissive, all logger messages allowed.
    LOGGER_LEVEL_INFO,
    /// @brief Warning level.
    /// @details
    /// Warning, Error and Fatal messages allowed.
    LOGGER_LEVEL_WARNING,
    /// @brief Error level.
    /// @details
    /// Error and Fatal messages allowed.
    LOGGER_LEVEL_ERROR,
    /// @brief Fatal level.
    /// @details
    /// Most restrictive level, only fatal messages allowed.
    LOGGER_LEVEL_FATAL,
} LoggerLevel;
/// @brief Types of file seek.
typedef enum {
    /// @brief Seek from current position.
    FSEEK_CURRENT,
    /// @brief Seek from start of file.
    FSEEK_BEGIN,
    /// @brief Seek from end of file.
    FSEEK_END,
} FileSeek;
/// @brief Flags for opening file.
typedef enum {
    /// @brief Open file for reading.
    FOPEN_READ     = (1 << 0),
    /// @brief Open file for writing.
    FOPEN_WRITE    = (1 << 1),
    /// @brief Create file. File must not exist if using this flag.
    FOPEN_CREATE   = (1 << 2),
    /// @brief Open and truncate file. File can only be opened for writing.
    FOPEN_TRUNCATE = (1 << 3),
    /// @brief Open and set position to end of file. File must be opened for writing.
    FOPEN_APPEND   = (1 << 4),
} FileOpenFlags;

#if defined(PLATFORM_WINDOWS)
    /// @brief Cross-platform file descriptor.
    typedef isize FD;
    /// @brief Cross-platform process ID.
    typedef isize PID;
#else
    /// @brief Cross-platform file descriptor.
    typedef int   FD;
    /// @brief Cross-platform process ID.
    typedef pid_t PID;
#endif
/// @brief Opaque mutex handle.
typedef struct {
    void* handle;
} Mutex;
/// @brief Opaque semaphore handle.
typedef struct {
    void* handle;
} Semaphore;
/// @brief Cross-platform pipe for reading.
typedef FD ReadPipe;
/// @brief Cross-platform pipe for writing.
typedef FD WritePipe;
/// @brief Cross-platform generic pipe (reading or writing).
typedef FD Pipe;
/// @brief Command line arguments for creating a process.
typedef struct {
    /// @brief Number of arguments.
    usize        count;
    /// @brief Array of arguments. Last item in array is a null-pointer.
    const cstr** args;
} Command;
/// @brief Object for dynamically creating a command. Do not use fields directly.
typedef struct {
    usize*   args; // darray
    dstring* buf;
    b32      is_offsets;
} CommandBuilder;
/// @brief Result of walking a directory.
typedef struct {
    /// @brief Number of paths found in directory.
    usize    count;
    /// @brief Dynamic array of paths found in directory.
    string*  paths;
    dstring* buf;
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
typedef string StringSplitDelimFilterFN( usize index, string str, void* params );
/// @brief Filter prototype for dynamic array.
/// @param     index       Index of item being tested.
/// @param     item_stride Size of items in array.
/// @param[in] item        Pointer to item being tested.
/// @param[in] params      (optional) Pointer to additional parameters.
/// @return
///     - @c True  : Item passed filter, will be kept in.
///     - @c False : Item failed filter, will be filtered out.
typedef b32 DarrayFilterFN(
    usize index, usize item_stride, const void* item, void* params );

static inline void _0( int _, ... ) {(void)_;}
/// @brief Mark any variables/parameters as unused.
/// @param ... Variables to be marked as unused.
#define unused( ... ) _0( 0, __VA_ARGS__ )

#define __insert_panic() exit(-1)

#if defined(COMPILER_MSVC)
    #define __insert_unreachable() __assume(0)
    /// @brief Insert a debugger break statement.
    #define insert_break()         __debugbreak()
    #define __insert_crash()       __debugbreak()
#else
    #define __insert_unreachable() __builtin_unreachable()
    /// @brief Insert a debugger break statement.
    #define insert_break()         __builtin_debugtrap()
    #define __insert_crash()       __builtin_trap()
#endif

/// @brief Stringify macro name.
/// @param macro (any defined) Macro to stringify
/// @return String literal.
#define macro_to_string( macro ) #macro
/// @brief Stringify macro value.
/// @param macro (any defined) Macro whose value will be stringified.
/// @return String literal.
#define macro_value_to_string( macro ) macro_to_string( macro )
/// @brief Create command line argument that defines string literal macro.
/// @param id   (string literal) Name of macro.
/// @param text (string literal) String literal to define macro to.
/// @return String literal.
#define string_define( id, text )\
    "-D" id "=\"\\\"" text "\\\"\""
/// @brief Calculate length of static array.
/// @param array (any[]) Static array to calculate length of.
/// @return (usize) Length of @c array.
#define static_array_len( array ) (sizeof(array) / sizeof((array)[0]))
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

/// @brief Function for initializing cbuild. Must be called from main()
/// @param logger_level Level to set logger to.
#define init( logger_level )\
    _init_( logger_level, argv[0], __FILE__, argc, (const char**)argv )

/// @brief Allocate memory on the heap. Always returns zeroed memory.
/// @param size Size in bytes of memory to allocate.
/// @return
///     - Pointer : Allocation succeeded, pointer to start of buffer.
///     - NULL    : Allocation failed.
void* memory_alloc( usize size );
/// @brief Reallocate memory on the heap. Always returns zeroed memory.
/// @param memory   Pointer to block of memory to reallocate.
/// @param old_size Size in bytes of memory to reallocate.
/// @param new_size New size of memory to reallocate. Must be > @c old_size.
/// @return
///     - Pointer : Allocation succeeded, pointer to start of buffer.
///     - NULL    : Allocation failed.
void* memory_realloc( void* memory, usize old_size, usize new_size );
/// @brief Free memory allocated on the heap.
/// @param memory Pointer to block of memory to free.
/// @param size   Size in bytes of block of memory.
void  memory_free( void* memory, usize size );
/// @brief Query how many bytes are currently allocated.
/// @return Bytes allocated.
usize memory_query_usage(void);
/// @brief Query how many bytes have been allocated thus far.
/// @return Bytes allocated in total.
usize memory_query_total_usage(void);
/// @brief Copy value across block of memory.
/// @param memory     Block to modify, must be >= @c value_size in size.
/// @param value_size Size of value to stamp.
/// @param value      Pointer to value to stamp.
/// @param size       Total size of @c memory in bytes.
void memory_stamp( void* memory, usize value_size, const void* value, usize size );
/// @brief Set bytes in block of memory to a value.
/// @param memory 
void memory_set( void* memory, i8 value, usize size );
void memory_zero( void* memory, usize size );
void memory_copy( void* restrict dst, const void* restrict src, usize size );
void memory_move( void* dst, const void* src, usize size );
b32  memory_cmp( const void* a, const void* b, usize size );

b32 char_in_set( char c, string set );

usize cstr_len( const cstr* string );
usize cstr_len_utf8( const cstr* string );
b32 cstr_cmp( const cstr* a, const cstr* b );
b32 cstr_find( const cstr* string, char c, usize* opt_out_index );
b32 cstr_find_rev( const cstr* string, char c, usize* opt_out_index );
b32 cstr_find_set( const cstr* string, const cstr* set, usize* opt_out_index );
b32 cstr_find_set_rev( const cstr* string, const cstr* set, usize* opt_out_index );
b32 cstr_find_cstr( const cstr* string, const cstr* substr, usize* opt_out_index );
b32 cstr_find_cstr_rev( const cstr* string, const cstr* substr, usize* opt_out_index );

#define string_empty()\
    (string){ .cc=0, .len=0 }
#define string_new( length, buf )\
    (string){ .cc=buf, .len=length }
#define string_text( literal )\
    string_new( sizeof(literal) - 1, literal )
#define string_from_cstr( s )\
    string_new( cstr_len( s ), s )
#define string_from_dstring( d )\
    string_new( dstring_len( d ), d )
b32 string_is_empty( string str );
b32 string_is_null_terminated( string str );
b32 string_cmp( string a, string b );
b32 string_cmp_clamped( string a, string b );
b32 string_find( string str, char c, usize* opt_out_index );
b32 string_find_rev( string str, char c, usize* opt_out_index );
b32 string_find_set( string str, string set, usize* opt_out_index );
b32 string_find_set_rev( string str, string set, usize* opt_out_index );
b32 string_find_string( string str, string substr, usize* opt_out_index );
b32 string_find_string_rev( string str, string substr, usize* opt_out_index );
char* string_first( string str );
char* string_last( string str );
string string_adv( string str );
string string_adv_by( string str, usize stride );
string string_truncate( string str, usize max );
string string_trim( string str, usize amount );
string string_remove_ws_lead( string str );
string string_remove_ws_trail( string str );
string string_remove_ws_surround( string str );
void string_split_at(
    string src, usize at, b32 keep_split,
    string* opt_out_left, string* opt_out_right );
b32 string_split_char(
    string src, char c, b32 keep_split,
    string* opt_out_left, string* opt_out_right );
string* string_split_delim( string src, string delim, b32 keep_delim );
#define string_split_delim_char( src, delim, keep_delim )\
    string_split_delim( src, string_new( 1, (char[]){ delim } ), keep_delim )
string* string_split_delim_ex(
    string src, string delim, b32 keep_delim,
    StringSplitDelimFilterFN* filter, void* params );
#define string_split_delim_char_ex( src, delim, keep_delim, filter, params )\
    string_split_delim_ex( src, string_new( 1, (char[]){ delim } ),\
        keep_delim, filter, params )
usize string_len_utf8( string str );

dstring* dstring_empty( usize cap );
dstring* dstring_new( usize len, const char* str );
#define dstring_from_cstr( str ) dstring_new( cstr_len( str ), str )
#define dstring_from_string( str ) dstring_new( str.len, str.cc )
#define dstring_from_string_ptr( str ) dstring_new( (str)->len, (str)->cc )
#define dstring_text( literal ) dstring_new( sizeof(literal)-1, literal )
#define dstring_to_string( dstr ) string_from_dstring( dstr )
dstring* dstring_fmt_va( const cstr* format, va_list va );
dstring* dstring_fmt( const cstr* format, ... );
dstring* dstring_grow( dstring* str, usize amount );
dstring* dstring_clone( dstring* src );
dstring* dstring_concat( string lhs, string rhs );
#define dstring_concat_cstr( lhs, rhs )\
    dstring_concat( string_from_cstr( lhs ), string_from_cstr( rhs ) )
dstring* dstring_concat_multi(
    usize count, string* strings, string opt_separator );
dstring* dstring_concat_multi_cstr(
    usize count, const cstr** strings, const cstr* opt_separator );
dstring* dstring_append( dstring* str, string append );
#define dstring_append_cstr( dstr, append )\
    dstring_append( dstr, string_from_cstr( append ) )
#define dstring_append_text( dstr, literal )\
    dstring_append( dstr, string_text( literal ) )
dstring* dstring_prepend( dstring* str, string prepend );
#define dstring_prepend_cstr( dstr, prepend )\
    dstring_prepend( dstr, string_from_cstr( prepend ) )
#define dstring_prepend_text( dstr, literal )\
    dstring_prepend( dstr, string_text( literal ) )
dstring* dstring_insert( dstring* str, string insert, usize at );
#define dstring_insert_cstr( dstr, cstr, at )\
    dstring_insert( dstr, string_from_cstr( cstr ), at )
#define dstring_insert_text( dstr, literal, at )\
    dstring_insert( dstr, string_text( literal ), at )
dstring* dstring_push( dstring* str, char c );
dstring* dstring_emplace( dstring* str, char c, usize at );
b32 dstring_pop( dstring* str, char* opt_out_c );
b32 dstring_remove( dstring* str, usize index );
b32 dstring_remove_range( dstring* str, usize from_inclusive, usize to_exclusive );
void dstring_truncate( dstring* darray, usize max );
void dstring_trim( dstring* darray, usize amount );
void dstring_clear( dstring* str );
usize dstring_remaining( const dstring* str );
usize dstring_len( const dstring* str );
usize dstring_cap( const dstring* str );
usize dstring_total_size( const dstring* str );
b32 dstring_is_empty( const dstring* str );
b32 dstring_is_full( const dstring* str );
void* dstring_head( dstring* str );
const void* dstring_head_const( const dstring* str );
void dstring_free( dstring* str );

void* darray_empty( usize stride, usize cap );
void* darray_from_array( usize stride, usize len, const void* buf );
usize darray_static_memory_requirement( usize stride, usize cap );
void* darray_static( usize stride, usize cap, void* buf );
#define darray_literal( type, ... )\
    darray_from_array( sizeof(type),\
        sizeof((type[]){ __VA_ARGS__ }) / sizeof(type), (type[]){ __VA_ARGS__ } )
void* darray_join( usize stride,
    usize lhs_len, const void* lhs, usize rhs_len, const void* rhs );
void* darray_from_filter(
    usize stride, usize len, const void* src,
    DarrayFilterFN* filter, void* filter_params );
void* darray_grow( void* darray, usize amount );
void* darray_clone( const void* darray );
void darray_clear( void* darray );
void* darray_set_len( void* darray, usize len );
void darray_truncate( void* darray, usize max );
void darray_trim( void* darray, usize amount );
b32 darray_try_push( void* darray, const void* item );
b32 darray_try_emplace( void* darray, const void* item, usize at );
b32 darray_try_insert( void* darray, usize count, const void* items, usize at );
b32 darray_try_append( void* darray, usize count, const void* items );
b32 darray_pop( void* darray, void* opt_out_item );
void* darray_push( void* darray, const void* item );
void* darray_emplace( void* darray, const void* item, usize at );
void* darray_insert( void* darray, usize count, const void* items, usize at );
void* darray_append( void* darray, usize count, const void* items );
b32 darray_remove( void* darray, usize index );
b32 darray_remove_range( void* darray, usize from_inclusive, usize to_exclusive );
usize darray_remaining( const void* darray );
usize darray_len( const void* darray );
usize darray_cap( const void* darray );
usize darray_stride( const void* darray );
usize darray_total_size( const void* darray );
void* darray_head( void* darray );
const void* darray_head_const( const void* darray );
void darray_free( void* darray );

string path_cwd(void);
string path_home(void);
b32 path_is_absolute( const cstr* path );
b32 path_exists( const cstr* path );
b32 path_is_directory( const cstr* path );
usize path_chunk_count( string path );
string* path_chunk_split( string path );
b32 path_matches_glob( string path, string glob );
b32 path_walk_dir(
    const cstr* dir, b32 recursive,
    b32 include_dirs, WalkDirectory* out_result );
string* path_walk_split_glob( const WalkDirectory* wd, string glob );
void path_walk_free( WalkDirectory* wd );

#define file_null() (0)
b32 fd_open( const cstr* path, FileOpenFlags flags, FD* out_file );
void fd_close( FD* file );
b32 fd_write( FD* file, usize size, const void* buf, usize* opt_out_write_size );
b32 fd_write_fmt_va( FD* file, const char* format, va_list va );
b32 fd_write_fmt( FD* file, const char* format, ... );
b32 fd_read( FD* file, usize size, void* buf, usize* opt_out_read_size );
b32 fd_truncate( FD* file );
usize fd_query_size( FD* file );
void fd_seek( FD* file, FileSeek type, isize seek );
usize fd_query_position( FD* file );
time_t file_query_time_create( const cstr* path );
time_t file_query_time_modify( const cstr* path );
b32 file_move( const cstr* dst, const cstr* src );
b32 file_copy( const cstr* dst, const cstr* src );
b32 file_remove( const cstr* path );

b32 dir_create( const cstr* path );
b32 dir_remove( const cstr* path, b32 recursive );

atom   atomic_add( atom* atomic, atom val );
atom64 atomic_add64( atom64* atom, atom64 val );
atom   atomic_compare_swap( atom* atomic, atom comp, atom exch );
atom64 atomic_compare_swap64( atom64* atom, atom64 comp, atom64 exch );
void   fence(void);

#if defined(COMPILER_MSVC)
    #define mutex_null() {0}
#else
    #define mutex_null() (Mutex){ .handle=NULL }
#endif
b32  mutex_create( Mutex* out_mutex );
b32  mutex_is_valid( Mutex* mutex );
void mutex_lock( Mutex* mutex );
b32  mutex_lock_timed( Mutex* mutex, u32 ms );
void mutex_unlock( Mutex* mutex );
void mutex_destroy( Mutex* mutex );

#if defined(COMPILER_MSVC)
    #define semaphore_null() {0}
#else
    #define semaphore_null() (Semaphore){ .handle=NULL }
#endif
b32  semaphore_create( Semaphore* out_semaphore );
b32  semaphore_is_valid( Semaphore* semaphore );
void semaphore_wait( Semaphore* semaphore );
b32  semaphore_wait_timed( Semaphore* sem, u32 ms );
void semaphore_signal( Semaphore* semaphore );
void semaphore_destroy( Semaphore* semaphore );

void thread_sleep( u32 ms );

b32 job_enqueue( JobFN* job, void* params );
b32 job_enqueue_timed( JobFN* job, void* params, u32 ms );
b32 job_wait_next( u32 ms );
b32 job_wait_all( u32 ms );

u32 thread_id(void);

#if defined(COMPILER_MSVC)
    #define command_null() {0}
#else
    #define command_null() (Command){ .args=0, .count=0 }
#endif
#define command_new( ... )\
    (Command){ .args=(const char*[]){ __VA_ARGS__, 0 },\
        .count=sizeof((const char*[]){ __VA_ARGS__ }) / sizeof(const char*) }
#define command_flatten_dstring( command )\
    dstring_concat_multi_cstr( (command)->count, (command)->args, " " )
const cstr* command_flatten_local( const Command* command );
b32 command_builder_new( const cstr* path, CommandBuilder* out_builder );
b32 command_builder_push( CommandBuilder* builder, const cstr* arg );
Command command_builder_cmd( CommandBuilder* builder );
void command_builder_free( CommandBuilder* builder );

#if defined(PLATFORM_WINDOWS)
    #define pipe_null() (0)
#else
    #define pipe_null() (-1)
#endif
void pipe_open( ReadPipe* out_read, WritePipe* out_write );
#define pipe_read( read_pipe, size, buf, opt_out_read_size )\
    fd_read( read_pipe, size, buf, opt_out_read_size )
#define pipe_write( write_pipe, size, buf, opt_out_write_size )\
    fd_write( write_pipe, size, buf, opt_out_write_size )
void pipe_close( Pipe pipe );

#define pid_null() (0)
b32 process_in_path( const cstr* process_name );
PID process_exec(
    Command cmd, b32 redirect_void, ReadPipe* opt_stdin,
    WritePipe* opt_stdout, WritePipe* opt_stderr );
int process_wait( PID pid );
b32 process_wait_timed( PID pid, int* opt_out_res, u32 ms );

f64 timer_milliseconds(void);
f64 timer_seconds(void);

u8* local_byte_buffer(void);
char* local_fmt_va( const char* format, va_list va );
char* local_fmt( const char* format, ... );

void logger_set_level( LoggerLevel level );
LoggerLevel logger_get_level(void);
void logger_va( LoggerLevel level, const char* format, va_list va );
void logger( LoggerLevel level, const char* format, ... );

string cbuild_query_compiler(void);
const Command* cbuild_query_command_line(void);

#define cb_info( ... )  logger( LOGGER_LEVEL_INFO, __VA_ARGS__ )
#define cb_warn( ... )  logger( LOGGER_LEVEL_WARNING, __VA_ARGS__ )
#define cb_error( ... ) logger( LOGGER_LEVEL_ERROR, __VA_ARGS__ )
#define cb_fatal( ... ) logger( LOGGER_LEVEL_FATAL, __VA_ARGS__ )

#if defined(CBUILD_ASSERTIONS)
    #define assertion( condition, ... ) do {\
        if( !(condition) ) {\
            fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): assertion '"#condition"' failed! message: ", thread_id(), __LINE__, __FUNCTION__ );\
            fprintf( stderr, __VA_ARGS__ );\
            fprintf( stderr, "\033[1;00m\n" );\
            fflush( stderr );\
            fence();\
            __insert_panic();\
        }\
    } while(0)
#else
    #define assertion( ... ) unused( __VA_ARGS__ )
#endif

#define panic( ... ) do {\
    fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): panic! message: ", thread_id(), __LINE__, __FUNCTION__ );\
    fprintf( stderr, __VA_ARGS__ );\
    fprintf( stderr, "\033[1;00m\n" );\
    fflush( stderr );\
    fence();\
    __insert_panic();\
} while(0)

#define expect_crash( condition, ... ) do {\
    if( !(condition) ) {\
        fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): expected '"#condition"'! message: ", thread_id(), __LINE__, __FUNCTION__ );\
        fprintf( stderr, __VA_ARGS__ );\
        fprintf( stderr, "\033[1;00m\n" );\
        fflush( stderr );\
        fence();\
        __insert_crash();\
    }\
} while(0)

#define expect( condition, ... ) do {\
    if( !(condition) ) {\
        fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): expected '"#condition"'! message: ", thread_id(), __LINE__, __FUNCTION__ );\
        fprintf( stderr, __VA_ARGS__ );\
        fprintf( stderr, "\033[1;00m\n" );\
        fflush( stderr );\
        fence();\
        __insert_panic();\
    }\
} while(0)

#define unimplemented() do {\
    fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): unimplemented path!\033[1;00m\n", thread_id(), __LINE__, __FUNCTION__ );\
    fflush( stderr );\
    fence();\
    __insert_panic();\
} while(0)

#define unreachable() do {\
    fprintf( stderr, "\033[1;35m[F:%02u] "__FILE__":%i:%s(): reached unreachable path!\033[1;00m\n", thread_id(), __LINE__, __FUNCTION__ );\
    fflush( stderr );\
    fence();\
    __insert_unreachable();\
    __insert_panic();\
} while(0)
/*           ^^^^^ just in case compiler warns */

void _init_(
    LoggerLevel logger_level,
    const cstr* executable_name,
    const cstr* source_name,
    int argc, const char** argv );

#endif /* header guard */

#if defined(CBUILD_IMPLEMENTATION) || defined(_CLANGD)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if defined(PLATFORM_WINDOWS)
    #include <process.h>
#endif

struct GlobalBuffers {
    atom obtained; // atomic boolean

    string cwd;
    string home;

    ReadPipe  void_read;
    WritePipe void_write;
};
struct LocalBuffers {
    char buffers[CBUILD_THREAD_COUNT + 1]
        [CBUILD_LOCAL_BUFFER_COUNT][CBUILD_LOCAL_BUFFER_CAPACITY];
    atom indices[CBUILD_THREAD_COUNT + 1];
};
struct DynamicString {
    usize len, cap;
    char  buf[];
};
struct DynamicArray {
    usize len, cap, stride;
    u8 buf[];
};

struct JobEntry {
    JobFN* proc;
    void*  params;
};
struct JobQueue {
    Semaphore wakeup;
    atom front, back;
    atom pending, len;
    struct JobEntry entries[CBUILD_MAX_JOBS];
};

volatile struct JobQueue* global_queue = NULL;

atom   global_is_mt              = false; // boolean
atom64 global_memory_usage       = 0;
atom64 global_total_memory_usage = 0;

atom            global_thread_id_source = 1; // 0 is main thread
make_tls( u32 ) global_thread_id        = 0;

static Mutex       global_logger_mutex = mutex_null();
static LoggerLevel global_logger_level = LOGGER_LEVEL_INFO;

static Command global_command_line;

volatile struct LocalBuffers* global_local_buffers = NULL;
#if defined(COMPILER_MSVC)
    volatile struct GlobalBuffers global_buffers = {0};
#else
    volatile struct GlobalBuffers global_buffers =
        (struct GlobalBuffers){ .obtained=false };
#endif

void  thread_create( JobFN* func, void* params );
char* internal_cwd(void);
char* internal_home(void);
static b32 path_walk_dir_internal(
    dstring** path, b32 recursive, b32 include_dirs,
    usize* out_count, dstring** out_buffer );

static b32 job_dequeue( struct JobQueue* queue, struct JobEntry* out_entry ) {
    if( !queue->len ) {
        return false;
    }
    atomic_add( &queue->len, -1 );

    fence();
    atom front = atomic_add( &queue->front, 1 );
    fence();

    *out_entry = queue->entries[ (front + 1) % CBUILD_MAX_JOBS ];
    return true;
}
void job_queue_proc( void* params ) {
    struct JobQueue* queue = params;
    fence();

    for( ;; ) {
        struct JobEntry entry;
        memory_zero( &entry, sizeof(entry) );

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
    cb_info(
        "creating job queue with %u entries and %u threads . . .",
        CBUILD_MAX_JOBS, CBUILD_THREAD_COUNT );

    expect( mutex_create( &global_logger_mutex ), "failed to create logger mutex!" );
    atomic_add( &global_is_mt, 1 );

    usize queue_size       = sizeof(*global_queue);
    struct JobQueue* queue = memory_alloc( queue_size );

    expect( queue, "failed to allocate job queue!" );

    expect(
        semaphore_create( &queue->wakeup ),
        "failed to create job queue semaphore!" );
    queue->front = -1;

    fence();

    for( usize i = 0; i < CBUILD_THREAD_COUNT; ++i ) {
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
        global_local_buffers = memory_alloc( sizeof(*global_local_buffers) );
        expect( global_local_buffers,
            "failed to allocate local buffers! size: %zu",
            sizeof(*global_local_buffers) );
    }
    return global_local_buffers;
}
static volatile char* get_next_local_buffer( u32 id ) {
    volatile struct LocalBuffers* b = get_local_buffers();

    u32 index = 0;
    if( global_is_mt ) {
        atom atomic = atomic_add( &b->indices[id], 1 );
        index = (*(u32*)&atomic) % CBUILD_LOCAL_BUFFER_COUNT;
    } else {
        atom atomic     = b->indices[id];
        b->indices[id] += 1;
        index = (*(u32*)&atomic) % CBUILD_LOCAL_BUFFER_COUNT;
    }

    volatile char* result = b->buffers[id][index];
    memory_zero( (void*)result, CBUILD_LOCAL_BUFFER_CAPACITY );
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
            (ReadPipe*)&global_buffers.void_read,
            (WritePipe*)&global_buffers.void_write );
    }
    return &global_buffers;
}
static b32 validate_file_flags( FileOpenFlags flags ) {
    if( !( flags & FOPEN_READ | FOPEN_WRITE ) ) {
        cb_error( "FD flags must have READ and/or WRITE set!" );
        return false;
    }
    if( flags & FOPEN_TRUNCATE ) {
        if( flags & FOPEN_APPEND ) {
            cb_error( "FD flag APPEND and TRUNCATE cannot be set at the same time!" );
            return false;
        }
        if( flags & FOPEN_READ ) {
            cb_error( "FD flag TRUNCATE and READ cannot be set at the same time!" );
            return false;
        }
    }

    return true;
}
static b32 dir_remove_internal( const cstr* path );

#if !defined(CBUILD_COMPILER_NAME)
    #if defined(COMPILER_MSVC)
        #define CBUILD_COMPILER_NAME "cl"
    #elif defined(COMPILER_CLANG)
        #define CBUILD_COMPILER_NAME "clang"
    #elif defined(COMPILER_GCC)
        #define CBUILD_COMPILER_NAME "gcc"
    #else
        #define CBUILD_COMPILER_NAME "cc"
    #endif
#endif

#if !defined(CBUILD_COMPILER_OUTPUT_FLAG)
    #if defined(COMPILER_MSVC)
        #define CBUILD_COMPILER_OUTPUT_FLAG "-Fe:"
    #else
        #define CBUILD_COMPILER_OUTPUT_FLAG "-o"
    #endif
#endif

#if !defined(CBUILD_POSIX_FLAGS) && defined(PLATFORM_POSIX)
    #define CBUILD_POSIX_FLAGS "-pthread"
#endif

string cbuild_query_compiler(void) {
    return string_text( CBUILD_COMPILER_NAME );
}
const Command* cbuild_query_command_line(void) {
    return &global_command_line;
}

void _init_(
    LoggerLevel logger_level,
    const cstr* executable_name,
    const cstr* source_name,
    int argc, const char** argv
) {
    logger_set_level( logger_level );

    usize arguments_size   = sizeof(usize) * (argc + 1);
    const char** arguments = memory_alloc( arguments_size );
    expect( arguments, "failed to allocate argv buffer!" );

    memory_copy( arguments, argv, arguments_size - sizeof(usize) );

    global_command_line.count = argc;
    global_command_line.args  = arguments;

    (void)get_local_buffers();
    (void)get_global_buffers();

    expect( path_exists( __FILE__ ),
        "cbuild MUST be run from its source code directory!" );
    expect( path_exists( source_name ),
        "cbuild MUST be run from its source code directory!" );

    dstring* old_name = dstring_fmt( "%s.old", executable_name );
    expect( old_name, "failed to create old executable name!" );

    if( path_exists( old_name ) ) {
        file_remove( old_name );
    }

    b32 rebuild = false;
    if( path_exists( executable_name ) ) {
        time_t executable_modify = file_query_time_modify( executable_name );
        time_t source_modify     = file_query_time_modify( source_name );
        time_t header_modify     = file_query_time_modify( __FILE__ );

        f64 diff_source = difftime( executable_modify, source_modify );
        f64 diff_header = difftime( executable_modify, header_modify );

        rebuild = (diff_source < 0.0) || (diff_header < 0.0);
    } else {
        rebuild = true;
    }

    if( !rebuild ) {
        return;
    }
    // rebuild

    cb_info( "changes detected in cbuild script, rebuilding . . ." );

    f64 start = timer_milliseconds();

#if defined(COMPILER_MSVC)
    Command rebuild_cmd =

#if defined(CBUILD_ADDITIONAL_FLAGS)
    command_new(
        CBUILD_COMPILER_NAME, source_name, CBUILD_COMPILER_OUTPUT_FLAG,
        executable_name, CBUILD_ADDITIONAL_FLAGS,
        string_define( "CBUILD_ADDITIONAL_FLAGS", CBUILD_ADDTIONAL_FLAGS ),
        string_define( "CBUILD_COMPILER_NAME", CBUILD_COMPILER_NAME ),
        string_define( "CBUILD_COMPILER_OUTPUT_FLAG", CBUILD_COMPILER_OUTPUT_FLAG ),
        "-nologo" );
#else
    command_new(
        CBUILD_COMPILER_NAME, source_name, CBUILD_COMPILER_OUTPUT_FLAG,
        executable_name,
        string_define( "CBUILD_COMPILER_NAME", CBUILD_COMPILER_NAME ),
        string_define( "CBUILD_COMPILER_OUTPUT_FLAG", CBUILD_COMPILER_OUTPUT_FLAG ),
        "-nologo" );
#endif

#else /* MSVC */


    Command rebuild_cmd = command_new(
        CBUILD_COMPILER_NAME,
        source_name,
        CBUILD_COMPILER_OUTPUT_FLAG,
        executable_name,

#if defined(PLATFORM_POSIX)
        CBUILD_POSIX_FLAGS,
#endif

#if defined(CBUILD_ADDITIONAL_FLAGS)
        ,CBUILD_ADDITIONAL_FLAGS,
        string_define( "CBUILD_ADDITIONAL_FLAGS", CBUILD_ADDITIONAL_FLAGS ),
#endif
        string_define( "CBUILD_COMPILER_NAME", CBUILD_COMPILER_NAME ),
        string_define( "CBUILD_COMPILER_OUTPUT_FLAG", CBUILD_COMPILER_OUTPUT_FLAG )
#if defined(PLATFORM_POSIX)
        ,string_define( "CBUILD_POSIX_FLAGS", CBUILD_POSIX_FLAGS )
#endif

    );

#endif /* GCC style */

    cb_info(
        "rebuilding with command '%s' . . .",
        command_flatten_local( &rebuild_cmd ) );

    if( path_exists( old_name ) ) {
        expect( file_remove( old_name ), "could not remove old executable!" );
    }

    expect( file_move( old_name, executable_name ), "could not rename executable!" );

    fence();

    PID pid = process_exec( rebuild_cmd, false, 0, 0, 0 );
    int res = process_wait( pid );
    if( res != 0 ) {
        cb_fatal( "failed to rebuild!" );
        file_move( executable_name, old_name );

        exit(-1);
    }

    f64 end = timer_milliseconds();
    cb_info( "rebuilt in %fms", end - start );

#if defined(PLATFORM_WINDOWS)
    printf(
        "\033[1;33m"
        "[W:00] cbuild: "
        "windows does not support automatically reloading cbuild, "
        "please run it again."
        "\033[1;00m\n" );
    exit(0);
#else
    process_exec( global_command_line, false, 0, 0, 0 );
    exit(0);
#endif
}
usize memory_query_usage(void) {
    return global_memory_usage;
}
usize memory_query_total_usage(void) {
    return global_total_memory_usage;
}
void memory_set( void* memory, i8 value, usize size ) {
    memset( memory, value, size );
}
void memory_zero( void* memory, usize size ) {
    memory_set( memory, 0, size );
}
void memory_stamp( void* memory, usize value_size, const void* value, usize size ) {
    u8*   dst = memory;
    usize rem = size;
    while( rem ) {
        if( value_size > rem ) {
            break;
        }

        memory_copy( dst, value, value_size );
        dst += value_size;
    }
}
void memory_copy( void* restrict dst, const void* restrict src, usize size ) {
    memcpy( dst, src, size );
}
void memory_move( void* dst, const void* src, usize size ) {
    memmove( dst, src, size );
}
b32 memory_cmp( const void* a, const void* b, usize size ) {
    return memcmp( a, b, size ) == 0;
}

b32 char_in_set( char c, string set ) {
    for( usize i = 0; i < set.len; ++i ) {
        if( c == set.cc[i] ) {
            return true;
        }
    }
    return false;
}

usize cstr_len( const cstr* string ) {
    return strlen( string );
}
usize cstr_len_utf8( const cstr* str ) {
    return string_len_utf8( string_from_cstr( str ) );
}
b32 cstr_cmp( const cstr* a, const cstr* b ) {
    return strcmp( a, b ) == 0;
}
b32 cstr_find( const cstr* string, char c, usize* opt_out_index ) {
    char* res = strchr( string, c );
    if( !res ) {
        return false;
    }
    if( opt_out_index ) {
        *opt_out_index = res - string;
    }
    return true;
}
b32 cstr_find_rev( const cstr* string, char c, usize* opt_out_index ) {
    char* res = strrchr( string, c );
    if( !res ) {
        return false;
    }
    if( opt_out_index ) {
        *opt_out_index = res - string;
    }
    return true;
}
b32 cstr_find_set( const cstr* string, const cstr* set, usize* opt_out_index ) {
    char* res = strpbrk( string, set );
    if( !res ) {
        return false;
    }
    if( opt_out_index ) {
        *opt_out_index = res - string;
    }
    return true;
}
b32 cstr_find_set_rev( const cstr* str, const cstr* set, usize* opt_out_index ) {
    return string_find_set_rev(
        string_from_cstr( str ), string_from_cstr( set ), opt_out_index );
}
b32 cstr_find_cstr( const cstr* string, const cstr* substr, usize* opt_out_index ) {
    char* res = strstr( string, substr );
    if( !res ) {
        return false;
    }
    if( opt_out_index ) {
        *opt_out_index = res - string;
    }
    return true;
}
b32 cstr_find_cstr_rev(
    const cstr* str, const cstr* substr, usize* opt_out_index
) {
    return string_find_string_rev(
        string_from_cstr( str ), string_from_cstr( substr ), opt_out_index );
}

b32 string_is_empty( string str ) {
    return !(str.cc && str.len);
}
b32 string_is_null_terminated( string str ) {
    if( !str.cc[str.len] ) {
        return true;
    }
    if( str.len && !str.cc[str.len - 1] ) {
        return true;
    }
    return false;
}
b32 string_cmp( string a, string b ) {
    if( a.len != b.len ) {
        return false;
    }
    return memory_cmp( a.cc, b.cc, a.len );
}
b32 string_cmp_clamped( string a, string b ) {
    usize min = a.len > b.len ? b.len : a.len;
    return memory_cmp( a.cc, b.cc, min );
}
b32 string_find( string str, char c, usize* opt_out_index ) {
    const char* ptr = memchr( str.cc, c, str.len );
    if( !ptr ) {
        return false;
    }
    if( opt_out_index ) {
        *opt_out_index = ptr - str.cc;
    }
    return true;
}
b32 string_find_rev( string str, char c, usize* opt_out_index ) {
    for( usize i = str.len; i-- > 0; ) {
        if( str.cc[i] == c ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
b32 string_find_set( string str, string set, usize* opt_out_index ) {
    for( usize i = 0; i < set.len; ++i ) {
        if( string_find( str, set.cc[i], opt_out_index ) ) {
            return true;
        }
    }
    return false;
}
b32 string_find_set_rev( string str, string set, usize* opt_out_index ) {
    for( usize i = str.len; i-- > 0; ) {
        if( char_in_set( str.cc[i], set ) ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
b32 string_find_string( string str, string substr, usize* opt_out_index ) {
    string rem = str;
    if( rem.len < substr.len ) {
        return false;
    }

    while( rem.len ) {
        usize start = 0;
        if( string_find( rem, substr.cc[0], &start ) ) {
            rem = string_adv_by( rem, start );
            if( rem.len < substr.len ) {
                return false;
            }

            if( string_cmp_clamped( rem, substr ) ) {
                if( opt_out_index ) {
                    *opt_out_index = rem.cc - str.cc;
                }
                return true;
            }
            rem = string_adv( rem );
        } else {
            break;
        }
    }
    return false;
}
b32 string_find_string_rev( string str, string substr, usize* opt_out_index ) {
    if( str.len < substr.len ) {
        return false;
    }
    for( usize i = str.len; i-- > 0; ) {
        if( str.cc[i] != substr.cc[i] ) {
            continue;
        }
        if( str.len - i < substr.len ) {
            break;
        }
        string part = string_new( substr.len, str.cc + i );

        if( string_cmp( part, substr ) ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
char* string_first( string str ) {
    if( string_is_empty( str ) ) {
        return NULL;
    }
    return (char*)str.cc;
}
char* string_last( string str ) {
    if( string_is_empty( str ) ) {
        return NULL;
    }
    return (char*)( str.cc + (str.len - 1) );
}
string string_adv( string str ) {
    if( string_is_empty( str ) ) {
        return str;
    }
    return string_new( str.len - 1, str.cc + 1 );
}
string string_adv_by( string str, usize stride ) {
    if( string_is_empty( str ) ) {
        return str;
    }
    if( str.len <= stride ) {
        return string_new( 0, str.cc + (str.len - 1) );
    }
    return string_new( str.len - stride, str.cc + stride );
}
string string_truncate( string str, usize max ) {
    return string_new( max > str.len ? str.len : max, str.cc );
}
string string_trim( string str, usize amount ) {
    if( amount >= str.len ) {
        return string_new( 0, str.cc );
    }
    return string_new( str.len - amount, str.cc );
}
string string_remove_ws_lead( string str ) {
    string res = str;
    while( res.len ) {
        if( isspace( *res.cc ) ) {
            res = string_adv( res );
        } else {
            break;
        }
    }
    return res;
}
string string_remove_ws_trail( string str ) {
    string res = str;
    while( res.len ) {
        if( isspace( *string_last( res ) ) ) {
            res.len--;
        } else {
            break;
        }
    }
    return res;
}
string string_remove_ws_surround( string str ) {
    return string_remove_ws_lead( string_remove_ws_trail( str ) );
}
void string_split_at(
    string src, usize at, b32 keep_split, string* opt_out_left, string* opt_out_right
) {
    expect( at <= src.len,
        "index provided is outside string bounds! at: %zu", at );

    if( opt_out_left ) {
        *opt_out_left = string_truncate( src, at );
    }
    if( opt_out_right ) {
        *opt_out_right = string_adv_by( src, at + (keep_split ? 0 : 1) );
    }
}
b32 string_split_char(
    string src, char c, b32 keep_split, string* opt_out_left, string* opt_out_right
) {
    usize at = 0;
    if( !string_find( src, c, &at ) ) {
        return false;
    }
    string_split_at( src, at, keep_split, opt_out_left, opt_out_right );
    return true;
}
string* string_split_delim( string src, string delim, b32 keep_delim ) {
    usize  count  = 0;
    string substr = src;
    while( substr.len ) {
        usize pos = 0;
        if( string_find_string( substr, delim, &pos ) ) {
            count++;
            substr = string_adv_by( substr, pos + delim.len );
        } else {
            count++;
            break;
        }
    }

    string* res = darray_empty( sizeof(string), count );
    expect( res, "failed to allocate string buffer!" );

    if( count == 1 ) {
        return darray_push( res, &src );
    }

    substr = src;
    if( keep_delim ) {
        while( substr.len ) {
            usize pos = 0;
            if( string_find_string( substr, delim, &pos ) ) {
                string chunk = substr;
                chunk.len = pos + delim.len;

                expect( darray_try_push( res, &chunk ),
                    "misallocated result!" );

                substr = string_adv_by( substr, chunk.len );
            } else {
                expect( darray_try_push( res, &substr ),
                    "misallocated result!" );
                break;
            }
        }
    } else {
        while( substr.len ) {
            usize pos = 0;
            if( string_find_string( substr, delim, &pos ) ) {
                string chunk = substr;
                chunk.len    = pos;

                expect( darray_try_push( res, &chunk ),
                    "misallocated result!" );

                substr = string_adv_by( substr, chunk.len + delim.len );
            } else {
                expect( darray_try_push( res, &substr ),
                    "misallocated result!" );
                break;
            }
        }
    }

    return res;
}
string* string_split_delim_ex(
    string src, string delim, b32 keep_delim,
    StringSplitDelimFilterFN* filter, void* params
) {
    expect( filter, "no filter function provided!" );

    usize  count  = 0;
    string substr = src;
    while( substr.len ) {
        usize pos = 0;
        if( string_find_string( substr, delim, &pos ) ) {
            count++;
            substr = string_adv_by( substr, pos + delim.len );
        } else {
            count++;
            break;
        }
    }

    string* res = darray_empty( sizeof(string), count );
    expect( res, "failed to allocate string buffer!" );

    if( count == 1 ) {
        string filtered = filter( 0, src, params );
        if( filtered.len ) {
            expect( darray_try_push( res, &filtered ),
                "misallocated result!" );
        }
        return res;
    }

    usize index = 0;
    substr = src;
    if( keep_delim ) {
        while( substr.len ) {
            usize pos = 0;
            if( string_find_string( substr, delim, &pos ) ) {
                string chunk = substr;
                chunk.len    = pos + delim.len;

                chunk = filter( index, chunk, params );

                if( chunk.len ) {
                    expect( darray_try_push( res, &chunk ),
                        "misallocated result!" );
                    index++;
                }

                substr = string_adv_by( substr, chunk.len );
            } else {
                string filtered = filter( index++, substr, params );

                if( filtered.len ) {
                    expect( darray_try_push( res, &filtered ),
                        "misallocated result!" );
                }
                break;
            }
        }
    } else {
        while( substr.len ) {
            usize pos = 0;
            if( string_find_string( substr, delim, &pos ) ) {
                string chunk = substr;
                chunk.len    = pos;

                chunk = filter( index, chunk, params );

                if( chunk.len ) {
                    expect( darray_try_push( res, &chunk ),
                        "misallocated result!" );
                    index++;
                }

                substr = string_adv_by( substr, chunk.len + delim.len );
            } else {
                string filtered = filter( index++, substr, params );

                if( filtered.len ) {
                    expect( darray_try_push( res, &filtered ),
                        "misallocated result!" );
                }
                break;
            }
        }
    }

    return res;

}

usize string_len_utf8( string str ) {
    const unsigned char* ucc = (const unsigned char*)str.cc;
    usize res = 0;
    for( usize i = 0; i < str.len; ++i ) {
        if( (ucc[i] & 0xC0) != 0x80 ) {
            res++;
        }
    }
    return res;
}

dstring* dstring_empty( usize cap ) {
    usize capacity = cap ? cap : 1;
    struct DynamicString* res = memory_alloc( sizeof(*res) + capacity );
    res->cap = capacity;
    return res->buf;
}
dstring* dstring_new( usize len, const char* str ) {
    struct DynamicString* res = dstring_head( dstring_empty( len + 1 ) );
    if( !res ) {
        return NULL;
    }
    memory_copy( res->buf, str, len );
    res->len = len;
    return res->buf;
}
dstring* dstring_fmt_va( const cstr* format, va_list va ) {
    va_list va2;
    va_copy( va2, va );

    int size = vsnprintf( 0, 0, format, va2 );

    va_end( va2 );

    dstring* res = dstring_empty( size + 2 );
    if( !res ) {
        return NULL;
    }

    vsnprintf( res, size + 2, format, va );

    return res;
}
dstring* dstring_fmt( const cstr* format, ... ) {
    va_list va;
    va_start( va, format );
    dstring* res = dstring_fmt_va( format, va );
    va_end( va );
    return res;
}
dstring* dstring_grow( dstring* str, usize amount ) {
    struct DynamicString* res = dstring_head( str );
    usize old_size = sizeof(struct DynamicString) + res->cap;
    usize new_size = old_size + amount;

    res = memory_realloc( res, old_size, new_size );
    if( !res ) {
        return NULL;
    }

    res->cap += amount;
    return res->buf;
}
dstring* dstring_clone( dstring* src ) {
    return dstring_from_string( string_from_dstring( src ) );
}
dstring* dstring_concat( string lhs, string rhs ) {
    usize len        = lhs.len + rhs.len;
    usize total_size = len + 8;
    struct DynamicString* res = dstring_head( dstring_empty( total_size ) );

    memory_copy( res->buf, lhs.cc, lhs.len );
    memory_copy( res->buf + lhs.len, rhs.cc, rhs.len );

    res->len = len;

    return res->buf;
}
dstring* dstring_concat_multi( usize count, string* strings, string opt_separator ) {
    expect( count, "did not provide any strings!" );
    usize total_size = (count - 1) * opt_separator.len;
    for( usize i = 0; i < count; ++i ) {
        total_size += strings[i].len;
    }

    dstring* res = dstring_empty( total_size + 1 );

    if( opt_separator.len ) {
        for( usize i = 0; i < count; ++i ) {
            res = dstring_append( res, strings[i] );
            if( i + 1 != count ) {
                res = dstring_append( res, opt_separator );
            }
        }
    } else {
        for( usize i = 0; i < count; ++i ) {
            res = dstring_append( res, strings[i] );
        }
    }

    return res;
}
dstring* dstring_concat_multi_cstr(
    usize count, const cstr** strings, const cstr* opt_separator
) {
    expect( count, "did not provide any strings!" );
    usize seplen     = opt_separator ? cstr_len( opt_separator ) : 0;
    usize total_size = (count - 1) * seplen;
    for( usize i = 0; i < count; ++i ) {
        const cstr* current = strings[i];
        if( !current ) {
            continue;
        }
        total_size += cstr_len( current );
    }

    dstring* res = dstring_empty( total_size + 1 );

    if( opt_separator && seplen ) {
        string sep = string_new( seplen, opt_separator );
        for( usize i = 0; i < count; ++i ) {
            const cstr* current = strings[i];
            if( !current ) {
                continue;
            }

            res = dstring_append_cstr( res, current );
            if( i + 1 != count ) {
                res = dstring_append( res, sep );
            }
        }
    } else {
        for( usize i = 0; i < count; ++i ) {
            const cstr* current = strings[i];
            if( !current ) {
                continue;
            }
            res = dstring_append_cstr( res, current );
        }
    }

    return res;
}
dstring* dstring_append( dstring* str, string append ) {
    struct DynamicString* res = dstring_head( str );

    if( res->len + append.len + 1 > res->cap ) {
        res = dstring_head( dstring_grow( res->buf, append.len + 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    memory_copy( res->buf + res->len, append.cc, append.len );
    res->len += append.len;
    res->buf[res->len] = 0;
    return res->buf;
}
dstring* dstring_prepend( dstring* str, string prepend ) {
    struct DynamicString* res = dstring_head( str );

    if( res->len + prepend.len + 1 > res->cap ) {
        res = dstring_head( dstring_grow( res->buf, prepend.len + 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    memory_move( res->buf + prepend.len, res->buf, res->len + 1 );
    memory_copy( res->buf, prepend.cc, prepend.len );

    res->len += prepend.len;
    return res->buf;
}
dstring* dstring_insert( dstring* str, string insert, usize at ) {
    if( at == 0 ) {
        return dstring_prepend( str, insert );
    }
    struct DynamicString* res = dstring_head( str );
    if( res->len && res->len - 1 == at ) {
        return dstring_append( str, insert );
    }
    if( at >= res->len ) {
        cb_warn(
            "dstring_insert: attempted to insert past dstring bounds! "
            "len: %zu index: %zu", res->len, at );
        return NULL;
    }

    if( res->len + insert.len + 1 > res->cap ) {
        res = dstring_head( dstring_grow( res->buf, insert.len + 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    memory_move( res->buf + at + insert.len, res->buf + at, (res->len + 1) - at );
    memory_copy( res->buf + at, insert.cc, insert.len );
    res->len += insert.len;

    return res->buf;
}
dstring* dstring_push( dstring* str, char c ) {
    struct DynamicString* res = dstring_head( str );
    if( res->len + 2 >= res->cap ) {
        res = dstring_head( dstring_grow( str, 8 ) );
        if( !res ) {
            return NULL;
        }
    }

    res->buf[res->len++] = c;
    res->buf[res->len]   = 0;
    return res->buf;
}
dstring* dstring_emplace( dstring* str, char c, usize at ) {
    return dstring_insert( str, string_new( 1, &c ), at );
}
b32 dstring_pop( dstring* str, char* opt_out_c ) {
    struct DynamicString* head = dstring_head( str );
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
b32 dstring_remove( dstring* str, usize index ) {
    struct DynamicString* head = dstring_head( str );
    if( !head->len || index > head->len ) {
        cb_warn(
            "dstring_remove: attempted to remove past dstring bounds! "
            "len: %zu index: %zu", head->len, index );
        return false;
    }

    memory_move( head->buf + index, head->buf + index + 1, (head->len + 1) - index );
    head->len--;

    return true;
}
b32 dstring_remove_range( dstring* str, usize from_inclusive, usize to_exclusive ) {
    assertion( from_inclusive < to_exclusive,
        "dstring_remove_range: invalid range provided! (%zu, %zu]",
        from_inclusive, to_exclusive );
    struct DynamicString* head = dstring_head( str );
    if( !head->len || from_inclusive >= head->len || to_exclusive > head->len ) {
        cb_warn(
            "dstring_remove_range: attempted to remove past dstring bounds! "
            "len: %zu range: (%zu, %zu]", head->len, from_inclusive, to_exclusive );
        return false;
    }

    usize span = to_exclusive - from_inclusive;

    memory_move(
        head->buf + from_inclusive,
        head->buf + to_exclusive,
        (head->len + 1) - span );
    head->len -= span;

    return true;
}
void dstring_truncate( dstring* str, usize max ) {
    struct DynamicString* head = dstring_head( str );
    if( max >= head->len ) {
        return;
    }
    memory_zero( head->buf + max, head->len - max );
    head->len = max;
}
void dstring_trim( dstring* str, usize amount ) {
    usize len = dstring_len( str );
    dstring_truncate( str, amount > len ? 0 : len - amount );
}
void dstring_clear( dstring* str ) {
    struct DynamicString* head = dstring_head( str );

    memory_zero( head->buf, head->len );
    head->len = 0;
}
usize dstring_remaining( const dstring* str ) {
    // -1 to not include null-terminator
    return (dstring_cap( str ) - 1)  - dstring_len( str );
}
usize dstring_len( const dstring* str ) {
    return ((struct DynamicString*)str - 1)->len;
}
usize dstring_cap( const dstring* str ) {
    return ((struct DynamicString*)str - 1)->cap;
}
usize dstring_total_size( const dstring* str ) {
    return dstring_cap( str ) + sizeof(struct DynamicString);
}
b32 dstring_is_empty( const dstring* str ) {
    return dstring_len( str ) == 0;
}
b32 dstring_is_full( const dstring* str ) {
    return dstring_len( str ) == dstring_cap( str );
}
void* dstring_head( dstring* str ) {
    if( !str ) {
        return NULL;
    }
    return str - sizeof(struct DynamicString);
}
const void* dstring_head_const( const dstring* str ) {
    if( !str ) {
        return NULL;
    }
    return str - sizeof(struct DynamicString);
}
void dstring_free( dstring* str ) {
    struct DynamicString* head = dstring_head( str );
    if( !head ) {
        return;
    }
    usize total_size = head->cap + sizeof(*head);
    memory_free( head, total_size );
}

string path_cwd(void) {
    volatile struct GlobalBuffers* gb = get_global_buffers();
    return gb->cwd;
}
string path_home(void) {
    volatile struct GlobalBuffers* gb = get_global_buffers();
    return gb->home;
}
usize path_chunk_count( string path ) {
    string subpath = path;
    usize  count   = 0;

    while( subpath.len ) {
        usize sep = 0;
        if( string_find( subpath, '/', &sep ) ) {
            string chunk = subpath;
            chunk.len    = sep;

            subpath = string_adv_by( subpath, sep + 1 );
            count++;
        } else {
            count++;
            break;
        }
    }

    return count;
}
string* path_chunk_split( string path ) {
    usize   cap = path_chunk_count( path );
    string* res = darray_empty( sizeof(*res), cap ? cap : 1 );
    if( !res ) {
        return NULL;
    }

    string subpath = path;
    while( subpath.len ) {
        usize sep = 0;
        if( string_find( subpath, '/', &sep ) ) {
            string chunk = subpath;
            chunk.len    = sep;

            expect( darray_try_push( res, &chunk ), "push should have worked!" );

            subpath = string_adv_by( subpath, sep + 1 );
        } else {
            expect( darray_try_push( res, &subpath ), "push should have worked!" );
            break;
        }
    }

    return res;
}
b32 path_matches_glob( string path, string glob ) {
    if( glob.len == 1 && glob.cc[0] == '*' ) {
        return true;
    }

    while( path.len && *glob.cc != '*' ) {
        if( *glob.cc != *path.cc && *glob.cc != '?' ) {
            return false;
        }
        glob = string_adv( glob );
        path = string_adv( path );
    }

    string mp, cp;
    while( path.len ) {
        if( *glob.cc == '*' ) {
            glob = string_adv( glob );
            if( !glob.len ) {
                return true;
            }

            mp = glob;
            cp = string_adv( path );
        } else if( *glob.cc == *path.cc || *glob.cc == '?' ) {
            glob = string_adv( glob );
            path = string_adv( path );
        } else {
            glob = mp;
            cp   = string_adv( cp );
            path = cp;
        }
    }

    while( glob.len && *glob.cc == '*' ) {
        glob = string_adv( glob );
    }
    return glob.len ? false : true;

}
static b32 path_walk_split_glob_filter(
    usize index, usize stride, const void* item, void* params
) {
    unused(index, stride);

    string glob = *(string*)params;
    string path = *(string*)item;

    return path_matches_glob( path, glob );
}
string* path_walk_split_glob( const WalkDirectory* wd, string glob ) {
    assertion( wd && wd->paths, "walk result is null!" );

    string* res = darray_from_filter(
        sizeof(string), wd->count, wd->paths,
        path_walk_split_glob_filter, &glob );
    return res;
}
b32 path_walk_dir(
    const cstr* dir, b32 recursive,
    b32 include_dirs, WalkDirectory* out_result
) {
    assertion( dir, "no path provided!" );
    assertion( out_result, "no walk dir result provided!" );

    dstring* path = dstring_from_cstr( dir );
    if( !path ) {
        return false;
    }

    dstring* buffer = dstring_empty( 255 );
    if( !buffer ) {
        dstring_free( path );
        cb_error( "path_walk_dir: failed to allocate buffer!" );
        return false;
    }

    usize count = 0;
    b32 result = path_walk_dir_internal(
        &path, recursive, include_dirs, &count, &buffer );
    dstring_free( path );

    if( !result ) {
        dstring_free( buffer );
        return false;
    }

    if( !count ) {
        dstring_free( buffer );
        return true;
    }

    usize total = out_result->count + count;
    string* paths = darray_empty( sizeof(string), total );
    if( !paths ) {
        dstring_free( buffer );
        return false;
    }

    if( out_result->buf ) {
        dstring* concat =
            dstring_append( out_result->buf, string_from_dstring( buffer ) );
        dstring_free( buffer );
        if( !concat ) {
            return false;
        }
    } else {
        out_result->buf = buffer;
    }

    out_result->count = total;

    string rem = string_from_dstring( out_result->buf );
    while( rem.len ) {
        usize nul = 0;
        if( string_find( rem, 0, &nul ) ) {
            string current = rem;
            current.len    = nul;

            expect( darray_try_push( paths, &current ), "miscalculated path count!" );

            rem = string_adv_by( rem, nul + 1 );
        } else {
            expect( darray_try_push( paths, &rem ), "miscalculated path count!" );
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
            dstring_free( wd->buf );
        }

        memory_zero( wd, sizeof(*wd) );
    }
}
b32 dir_remove( const cstr* path, b32 recursive ) {
    if( !recursive ) {
        return dir_remove_internal( path );
    }

    WalkDirectory wd;
    memory_zero( &wd, sizeof(wd) );

    if( !path_walk_dir( path, true, false, &wd ) ) {
        cb_error( "dir_remove: failed to walk directory '%s'!", path );
        return false;
    }

    for( usize i = 0; i < wd.count; ++i ) {
        if( !file_remove( wd.paths[i].cc ) ) {
            cb_error( "dir_remove: failed to remove file '%s'!", wd.paths[i].cc );

            path_walk_free( &wd );
            return false;
        }
    }

    path_walk_free( &wd );

    if( !path_walk_dir( path, true, true, &wd ) ) {
        cb_error( "dir_remove: failed to walk directory '%s'!", path );
        return false;
    }

    for( usize i = 0; i < wd.count; ++i ) {
        if( !dir_remove_internal( wd.paths[i].cc ) ) {
            cb_error( "dir_remove: failed to remove dir '%s'!", wd.paths[i].cc );

            path_walk_free( &wd );
            return false;
        }
    }

    path_walk_free( &wd );
    return dir_remove_internal( path );
}

b32 fd_write_fmt_va( FD* file, const char* format, va_list va ) {
    char* formatted = local_fmt_va( format, va );
    return fd_write( file, cstr_len( formatted ), formatted, 0 );
}
b32 fd_write_fmt( FD* file, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    b32 res = fd_write_fmt_va( file, format, va );
    va_end( va );
    return res;
}

void* darray_empty( usize stride, usize cap ) {
    struct DynamicArray* res = memory_alloc( sizeof(*res) + (stride * cap) );
    if( !res ) {
        return NULL;
    }
    res->stride = stride;
    res->cap    = cap;
    return res->buf;
}
void* darray_from_array( usize stride, usize len, const void* buf ) {
    struct DynamicArray* res = darray_head( darray_empty( stride, len + 2 ) );
    if( !res ) {
        return NULL;
    }

    memory_copy( res->buf, buf, len * stride );
    res->len = len;

    return res->buf;
}
usize darray_static_memory_requirement( usize stride, usize cap ) {
    return (stride * cap) + sizeof(struct DynamicArray);
}
void* darray_static( usize stride, usize cap, void* buf ) {
    struct DynamicArray* res = buf;
    res->stride = stride;
    res->cap    = cap;
    res->len    = 0;
    return res->buf;
}
void* darray_join( usize stride,
    usize lhs_len, const void* lhs, usize rhs_len, const void* rhs
) {
    struct DynamicArray* res =
        darray_head( darray_empty( stride, lhs_len + rhs_len + 2 ) );
    if( !res ) {
        return NULL;
    }

    memory_copy( res->buf, lhs, stride * lhs_len );
    memory_copy( res->buf + (stride * lhs_len), rhs, rhs_len );
    res->len = lhs_len + rhs_len;

    return res;
}
void* darray_from_filter(
    usize stride, usize len, const void* src,
    DarrayFilterFN* filter, void* filter_params
) {
    const u8* src_bytes = src;

    void* res = darray_empty( stride, 10 );
    if( !res ) {
        return NULL;
    }

    for( usize i = 0; i < len; ++i ) {
        const u8* item = src_bytes + (stride * i);
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

void* darray_grow( void* darray, usize amount ) {
    struct DynamicArray* res = darray_head( darray );
    usize old_size = (res->stride * res->cap) + sizeof(*res);
    usize new_size = (res->stride * amount) + old_size;

    res = memory_realloc( res, old_size, new_size );
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
    memory_zero( head->buf, head->len * head->stride );
    head->len = 0;
}
void* darray_set_len( void* darray, usize len ) {
    if( len > darray_cap( darray ) ) {
        usize diff = len - darray_cap( darray );

        struct DynamicArray* head = darray_head( darray_grow( darray, diff ) );
        if( !head ) {
            return NULL;
        }

        head->len = len;
        return head->buf;
    } else if( len > darray_len( darray ) ) {
        struct DynamicArray* head = darray_head( darray );
        usize diff = head->len - len;
        memory_zero( head->buf + len, head->stride * diff );
        head->len = diff;

        return head->buf;
    } else {
        darray_truncate( darray, len );
        return darray;
    }
}
void darray_truncate( void* darray, usize max ) {
    struct DynamicArray* head = darray_head( darray );
    if( max >= head->len ) {
        return;
    }
    usize diff = head->len - max;
    memory_zero( head->buf + max, head->stride * diff );
    head->len = max;
}
void darray_trim( void* darray, usize amount ) {
    struct DynamicArray* head = darray_head( darray );
    darray_truncate( head->buf, amount > head->len ? 0 : head->len - amount );
}
b32 darray_try_push( void* darray, const void* item ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len == head->cap ) {
        return false;
    }

    memory_copy( head->buf + (head->stride * head->len), item, head->stride );
    head->len++;

    return true;
}
b32 darray_try_emplace( void* darray, const void* item, usize at ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len == head->cap ) {
        return false;
    }
    if( at >= head->len ) {
        cb_warn(
            "darray_emplace: attempted to emplace past darray bounds! "
            "len: %zu index: %zu", head->len, at );
        return false;
    }

    memory_move(
        head->buf + ( head->stride * ( at + 1 ) ),
        head->buf + ( head->stride * at ),
        head->stride * ( ( head->len + 1 ) - at ) );
    memory_copy( head->buf + ( head->stride * at ), item, head->stride );
    head->len++;

    return true;
}
b32 darray_try_insert( void* darray, usize count, const void* items, usize at ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len + count > head->cap ) {
        return false;
    }
    if( at >= head->len ) {
        cb_warn(
            "darray_insert: attempted to insert past darray bounds! "
            "len: %zu index: %zu", head->len, at );
        return false;
    }

    memory_move(
        head->buf + ( head->stride * ( at + count ) ),
        head->buf + ( head->stride * at ),
        head->stride * ( ( head->len + 1 ) - at ) );
    memory_copy( head->buf + ( head->stride * at ), items, head->stride * count );
    head->len += count;

    return true;
}
b32 darray_try_append( void* darray, usize count, const void* items ) {
    struct DynamicArray* head = darray_head( darray );
    if( head->len + count > head->cap ) {
        return false;
    }

    memory_copy(
        head->buf + (head->stride * head->len),
        items, head->stride * count );
    head->len += count;

    return true;
}
b32 darray_pop( void* darray, void* opt_out_item ) {
    struct DynamicArray* head = darray_head( darray );
    if( !head->len ) {
        return false;
    }

    head->len--;
    if( opt_out_item ) {
        memory_copy(
            opt_out_item, head->buf + (head->stride * head->len), head->stride );
    }

    memory_zero( head->buf + (head->stride * head->len), head->stride );
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
void* darray_emplace( void* darray, const void* item, usize at ) {
    if( at >= darray_len( darray ) ) {
        cb_warn(
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
void* darray_insert( void* darray, usize count, const void* items, usize at ) {
    struct DynamicArray* res = darray_head( darray );

    if( at >= res->len ) {
        cb_warn(
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
void* darray_append( void* darray, usize count, const void* items ) {
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
b32 darray_remove( void* darray, usize index ) {
    struct DynamicArray* head = darray_head( darray );
    if( !head->len || index > head->len ) {
        cb_warn(
            "darray_remove: attempted to remove past array bounds! "
            "len: %zu index: %zu", head->len, index );
        return false;
    }

    memory_move(
        head->buf + (head->stride * index),
        head->buf + (head->stride * (index + 1)),
        (head->stride * (head->len + 1) - index ) );
    head->len--;
    memory_zero( head->buf + (head->stride * head->len), head->stride );

    return true;
}
b32 darray_remove_range( void* darray, usize from_inclusive, usize to_exclusive ) {
    assertion( from_inclusive < to_exclusive,
        "darray_remove_range: invalid range provided! (%zu, %zu]",
        from_inclusive, to_exclusive );
    struct DynamicArray* head = darray_head( darray );
    if( !head->len || from_inclusive >= head->len || to_exclusive > head->len ) {
        cb_warn(
            "darray_remove_range: attempted to remove past array bounds! "
            "len: %zu range: (%zu, %zu]", head->len, from_inclusive, to_exclusive );
        return false;
    }

    usize span = to_exclusive - from_inclusive;

    memory_move(
        head->buf + (head->stride * from_inclusive),
        head->buf + (head->stride * to_exclusive),
        head->stride * ((head->len + 1) - span) );
    head->len -= span;

    return true;
}
usize darray_remaining( const void* darray ) {
    return darray_cap( darray ) - darray_len( darray );
}
usize darray_len( const void* darray ) {
    return ((struct DynamicArray*)darray - 1)->len;
}
usize darray_cap( const void* darray ) {
    return ((struct DynamicArray*)darray - 1)->cap;
}
usize darray_stride( const void* darray ) {
    return ((struct DynamicArray*)darray - 1)->stride;
}
usize darray_total_size( const void* darray ) {
    return
        sizeof(struct DynamicArray) +
        (darray_cap( darray ) * darray_stride( darray ));
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
        usize total_size = (head->cap * head->stride) + sizeof(*head);
        memory_free( head, total_size );
    }
}

b32 job_enqueue( JobFN* job, void* params ) {
    volatile struct JobQueue* queue = get_job_queue();

    if( queue->pending >= CBUILD_MAX_JOBS ) {
        cb_warn(
            "attempted to enqueue job while queue is full!" );
        return false;
    }

    struct JobEntry entry;
    entry.proc   = job;
    entry.params = params;

    fence();

    atom back = atomic_add( &queue->back, 1 );
    queue->entries[back % CBUILD_MAX_JOBS] = entry;

    fence();

    atomic_add( &queue->len, 1 );
    atomic_add( &queue->pending, 1 );

    semaphore_signal( (Semaphore*)&queue->wakeup );
    return true;
}
b32 job_enqueue_timed( JobFN* job, void* params, u32 ms ) {
    volatile struct JobQueue* queue = get_job_queue();

    while( queue->pending >= CBUILD_MAX_JOBS ) {
        if( !job_wait_next( ms ) ) {
            return false;
        }
    }

    expect( job_enqueue( job, params ),
        "enqueue unexpectedly failed!" );
    return true;
}
b32 job_wait_next( u32 ms ) {
    volatile struct JobQueue* queue = get_job_queue();

    u32 current = queue->pending;
    if( !current ) {
        return true;
    }

    if( ms == MT_WAIT_INFINITE ) {
        while( queue->pending >= current ) {
            thread_sleep( 1 );
        }
        return true;
    } else for( u32 i = 0; i < ms; ++i ) {
        if( queue->pending < current ) {
            return true;
        }
        thread_sleep( 1 );
    }

    return false;
}
b32 job_wait_all( u32 ms ) {
    volatile struct JobQueue* queue = get_job_queue();

    if( ms == MT_WAIT_INFINITE ) {
        while( queue->pending ) {
            thread_sleep(1);
        }
        return true;
    } else for( u32 i = 0; i < ms; ++i ) {
        if( !queue->pending ) {
            return true;
        }
        thread_sleep( 1 );
    }

    return false;
}

u32 thread_id(void) {
    return global_thread_id;
}

const cstr* command_flatten_local( const Command* command ) {
    char* buf = (char*)local_byte_buffer();
    usize len = 0;

    for( usize i = 0; i < command->count; ++i ) {
        const cstr* current = command->args[i];
        if( !current ) {
            continue;
        }

        usize current_len = cstr_len( current );
        memory_copy( buf + len, current, current_len );
        len += current_len;
        if( i + 1 != command->count ) {
            buf[len++] = ' ';
        }
    }

    return buf;
}
static void command_builder_set_offsets( CommandBuilder* builder, b32 is_offsets ) {
    if( builder->is_offsets == is_offsets ) {
        return;
    }

    usize arg_count = darray_len( builder->args ) - 1;
    if( is_offsets ) {
        for( usize i = 0; i < arg_count; ++i ) {
            usize offset = (const char*)builder->args[i] - builder->buf;
            builder->args[i] = offset;
        }
    } else {
        for( usize i = 0; i < arg_count; ++i ) {
            usize offset = builder->args[i];
            builder->args[i] = (usize)builder->buf + offset;
        }
    }

    builder->is_offsets = is_offsets;
}
b32 command_builder_new( const cstr* path, CommandBuilder* out_builder ) {
    assertion( path, "path is null!" );

    usize path_len = cstr_len( path );
    CommandBuilder res;
    res.is_offsets = true;
    res.buf        = dstring_empty( 33 + path_len );
    if( !res.buf ) {
        return false;
    }
    res.args = darray_empty( sizeof(usize), 4 );
    if( !res.args ) {
        return false;
    }

    dstring_append_cstr( res.buf, path );
    dstring_push( res.buf, 0 );

    const char* nul = 0;
    darray_push( res.args, &nul );
    darray_push( res.args, &nul );

    *out_builder   = res;
    return true;
}
b32 command_builder_push( CommandBuilder* builder, const cstr* arg ) {
    assertion( builder, "builder provided is null!" );
    assertion( builder->args && builder->buf, "builder provided is malformed!" );
    assertion( arg, "argument provided is null!" );

    command_builder_set_offsets( builder, true );

    if( darray_remaining( builder->args ) < 3 ) {
        usize* _new = darray_grow( builder->args, 4 );
        if( !_new ) {
            return false;
        }
        builder->args = _new;
    }

    usize arg_len = cstr_len( arg ) + 1;
    if( dstring_remaining( builder->buf ) < arg_len ) {
        dstring* _new = dstring_grow( builder->buf, arg_len + 32 );
        if( !_new ) {
            return false;
        }
        builder->buf = _new;
    }

    usize offset = dstring_len( builder->buf );
    darray_pop( builder->args, 0 );
    darray_try_push( builder->args, &offset );

    assertion(
        darray_try_push( builder->args, (usize[]){0} ),
        "reallocation miscalculated!" );
    assertion(
        dstring_append( builder->buf, string_new( arg_len, arg ) ),
        "reallocation miscalculated!" );
    return true;
}
Command command_builder_cmd( CommandBuilder* builder ) {
    assertion( builder, "builder is null!" );
    assertion( builder->args, "builder is malformed!" );

    command_builder_set_offsets( builder, false );

    Command res;
    res.count = darray_len( builder->args ) - 1;
    res.args  = (const char**)builder->args;
    
    return res;
}
void command_builder_free( CommandBuilder* builder ) {
    if( builder ) {
        if( builder->args ) {
            darray_free( builder->args );
        }
        if( builder->buf ) {
            dstring_free( builder->buf );
        }
        memory_zero( builder, sizeof(*builder) );
    }
}

u8* local_byte_buffer() {
    fence();
    return (u8*)get_next_local_buffer( thread_id() );
}
char* local_fmt_va( const char* format, va_list va ) {
    char* buf = (char*)local_byte_buffer();
    vsnprintf( buf, CBUILD_LOCAL_BUFFER_CAPACITY - 1, format, va ); // -1 to ensure null-terminated
    return buf;
}
char* local_fmt( const char* format, ... ) {
    va_list va;
    va_start( va, format );
    char* res = local_fmt_va( format, va );
    va_end( va );
    return res;
}

static b32 logger_check_level( LoggerLevel level ) {
    return level >= global_logger_level;
}

void logger_set_level( LoggerLevel level ) {
    global_logger_level = level;
}
LoggerLevel logger_get_level(void) {
    return global_logger_level;
}
void logger_va( LoggerLevel level, const char* format, va_list va ) {
    if( !logger_check_level( level ) ) {
        return;
    }

    static const char local_level_letters[] = {
        'I', // LOGGER_LEVEL_INFO
        'W', // LOGGER_LEVEL_WARNING
        'E', // LOGGER_LEVEL_ERROR
        'F', // LOGGER_LEVEL_FATAL
    };

    static const char* local_level_colors[] = {
        "",           // LOGGER_LEVEL_INFO
        "\033[1;33m", // LOGGER_LEVEL_WARNING
        "\033[1;31m", // LOGGER_LEVEL_ERROR
        "\033[1;35m", // LOGGER_LEVEL_FATAL
    };

    FILE* const level_output[] = {
        stdout, // LOGGER_LEVEL_INFO
        stdout, // LOGGER_LEVEL_WARNING
        stderr, // LOGGER_LEVEL_ERROR
        stderr, // LOGGER_LEVEL_FATAL
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
void logger( LoggerLevel level, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    logger_va( level, format, va );
    va_end( va );
}

#if defined(PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <limits.h>

#if !defined(PATH_MAX)
    #define PATH_MAX 260
#endif

struct Win32ThreadParams {
    JobFN* proc;
    void*  params;
    u32    id;
};

#define CBUILD_LOCAL_WIDE_CAPACITY (CBUILD_LOCAL_BUFFER_CAPACITY / 2)

static struct Win32ThreadParams global_win32_thread_params[CBUILD_THREAD_COUNT];

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
static wchar_t* win32_local_utf8_to_wide( string utf8 ) {
    assertion( utf8.cc && utf8.len < CBUILD_LOCAL_WIDE_CAPACITY, "invalid string!" );

    wchar_t* buf = (wchar_t*)local_byte_buffer();

    wchar_t* copy_dst = buf;
    string rem = utf8;
    while( rem.len ) {
        // TODO(alicia): this should check for valid utf8 bound
        usize max_convert = rem.len;
        if( rem.len > INT32_MAX ) {
            rem.len = INT32_MAX;
        }

        MultiByteToWideChar( CP_UTF8, 0, rem.cc, max_convert, copy_dst, max_convert );

        copy_dst += max_convert;
        rem       = string_adv_by( rem, max_convert );
    }

    return buf;
}
static string win32_local_wide_to_utf8( usize len, wchar_t* wide ) {
    assertion( wide && len < CBUILD_LOCAL_WIDE_CAPACITY, "invalid string!" );

    char* buf = (char*)local_byte_buffer();

    char* copy_dst = buf;
    usize rem      = len;

    while( rem ) {
        // TODO(alicia): check for valid windows unicode bounds?
        // is that even a thing
        usize max_convert = rem;
        if( max_convert > INT32_MAX ) {
            max_convert = INT32_MAX;
        }

        WideCharToMultiByte(
            CP_UTF8, 0, wide, max_convert, copy_dst, max_convert, 0, 0 );

        copy_dst += max_convert;
        rem      -= max_convert;
    }

    return string_new( len, buf );
}
static string win32_local_error_message( DWORD error_code ) {
    char* buf = (char*)local_byte_buffer();

    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code,
        0, buf, CBUILD_LOCAL_BUFFER_CAPACITY, 0 );

    return string_from_cstr( buf );
}
static wchar_t* win32_local_path_canon( string path ) {
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

    string subpath = path;

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

    wchar_t* buf = (wchar_t*)local_byte_buffer();
    memory_copy( buf, L"\\\\?\\", sizeof(L"\\\\?\\") - sizeof(wchar_t) );
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

            subpath = string_adv( subpath );
        } break;
        default: break;
    }

    size_t last_chunk_len = 0;
    while( subpath.len ) {
        size_t sep = 0;
        if( string_find( subpath, '/', &sep ) ) {
            if( !sep ) {
                subpath = string_adv( subpath );
                continue;
            }

            string chunk = subpath;
            chunk.len    = sep;

            if( chunk.len < 3 ) {
                if( string_cmp( chunk, string_text( "." ) ) ) {
                    subpath = string_adv_by( subpath, chunk.len + 1 );
                    continue;
                }
                if( string_cmp( chunk, string_text( ".." ) ) ) {
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
                    subpath = string_adv_by( subpath, chunk.len + 1 );
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

            subpath = string_adv_by( subpath, chunk.len + 1 );
        } else {
            if( string_cmp( subpath, string_text( "." ) ) ) {
                break;
            }
            if( string_cmp( subpath, string_text( ".." ) ) ) {
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

void* memory_alloc( usize size ) {
    void* res = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, size );
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
void* memory_realloc( void* memory, usize old_size, usize new_size ) {
    assertion( new_size >= old_size, "attempted to reallocate to smaller buffer!" );
    void* res = HeapReAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, memory, new_size );
    if( !res ) {
        return NULL;
    }
    usize diff = new_size - old_size;
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, diff );
        atomic_add64( &global_total_memory_usage, diff );
    } else {
        global_memory_usage       += diff;
        global_total_memory_usage += diff;
    }
    return res;
}
void memory_free( void* memory, usize size ) {
    if( !memory ) {
        cb_warn( "attempted to free null pointer!" );
        return;
    }
    HeapFree( GetProcessHeap(), HEAP_ZERO_MEMORY, memory );
    atom64 neg = size;
    neg = -neg;
    if( global_is_mt ) {
        atomic_add64( &global_memory_usage, neg );
    } else {
        global_memory_usage += neg;
    }
}
b32 path_is_absolute( const cstr* path ) {
    assertion( path, "null path!" );
    if( !path[0] || !path[1] ) {
        return false;
    }
    return
        isalpha( path[0] ) &&
        path[1] == ':';
}
static DWORD path_attributes_short( const cstr* path ) {
    return GetFileAttributesA( path );
}
static DWORD path_attributes_long( string path ) {
    wchar_t* wpath = win32_local_path_canon( path );
    return GetFileAttributesW( wpath );
}
static DWORD path_attributes( string path ) {
    if( path.len >= PATH_MAX ) {
        return path_attributes_long( path );
    } else {
        return path_attributes_short( path.cc );
    }
}
b32 path_is_directory( const cstr* path ) {
    string spath = string_from_cstr( path );
    DWORD attr   = path_attributes( spath );

    if( attr == INVALID_FILE_ATTRIBUTES ) {
        return false;
    } else {
        return attr & FILE_ATTRIBUTE_DIRECTORY;
    }
}
#if 0
static b32 path_exists_short( string path ) {
    DWORD attr = path_attributes( path );
    return attr != INVALID_FILE_ATTRIBUTES;

    if( attr == INVALID_FILE_ATTRIBUTES ) {
        return false;
    } else {
        return true;
    }

    DWORD dwDesiredAccess       = 0;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes  = 0;
    DWORD dwShareMode           =
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

    HANDLE handle = CreateFileA(
        path.cc, dwDesiredAccess, dwShareMode,
        NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL );
    if( handle == INVALID_HANDLE_VALUE ) {
        return false;
    } else {
        CloseHandle( handle );
        return true;
    }
}
static b32 path_exists_long( string path ) {
    DWORD dwDesiredAccess       = 0;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes  = 0;
    DWORD dwShareMode           =
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

    wchar_t* wpath = win32_local_path_canon( path );

    HANDLE handle = CreateFileW(
        wpath, dwDesiredAccess, dwShareMode,
        NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL );
    if( handle == INVALID_HANDLE_VALUE ) {
        return false;
    } else {
        CloseHandle( handle );
        return true;
    }
}
#endif
b32 path_exists( const cstr* path ) {
    assertion( path, "null path!" );
    string spath = string_from_cstr( path );
    return path_attributes( spath ) != INVALID_FILE_ATTRIBUTES;
}
static b32 path_walk_dir_internal_long(
    dstring** path, b32 recursive, b32 include_dirs,
    usize* out_count, dstring** out_buffer
) {
    usize original_len = dstring_len( *path );
    dstring* _new = dstring_append( *path, string_text( "/*" ) );
    if( !_new ) {
        return false;
    }
    *path = _new;

    wchar_t* wpath = win32_local_path_canon( string_from_dstring( *path ) );

    WIN32_FIND_DATAW fd;
    memory_zero( &fd, sizeof( fd ) );
    HANDLE find_file = FindFirstFileW( wpath, &fd );
    dstring_truncate( *path, original_len );

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

        _new = dstring_push( *path, '/' );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *path = _new;

        usize name_len = wcslen( fd.cFileName );
        string npath   = win32_local_wide_to_utf8( name_len, fd.cFileName );

        _new = dstring_append( *path, npath );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *path = _new;

        if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            if( include_dirs ) {
                _new = dstring_append(
                    *out_buffer, string_new( dstring_len( *path ) + 1 , *path ) );
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

            dstring_truncate( *path, original_len );
            continue;
        }

        _new = dstring_append(
            *out_buffer, string_new( dstring_len( *path ) + 1, *path ) );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *out_buffer = _new;
        *out_count += 1;

        dstring_truncate( *path, original_len );
    } while( FindNextFileW( find_file, &fd ) != FALSE );

    FindClose( find_file );
    return true;
}
static b32 path_walk_dir_internal_short(
    dstring** path, b32 recursive, b32 include_dirs,
    usize* out_count, dstring** out_buffer
) {
    usize original_len = dstring_len( *path );
    if( (original_len + 2) >= PATH_MAX ) { // to account for the /*
        return path_walk_dir_internal_long(
            path, recursive, include_dirs, out_count, out_buffer );
    }

    dstring* _new = dstring_append( *path, string_text( "/*" ) );
    if( !_new ) {
        return false;
    }
    *path = _new;

    WIN32_FIND_DATAA fd;
    memory_zero( &fd, sizeof( fd ) );
    HANDLE find_file = FindFirstFileA( *path, &fd );
    dstring_truncate( *path, original_len );

    if( find_file == INVALID_HANDLE_VALUE ) {
        return false;
    }

    do {
        if(
            cstr_cmp( fd.cFileName, "." )  ||
            cstr_cmp( fd.cFileName, ".." ) ||
            cstr_cmp( fd.cFileName, ".git" )
        ) {
            continue;
        }

        _new = dstring_push( *path, '/' );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *path = _new;

        usize name_len = cstr_len( fd.cFileName );
        _new = dstring_append( *path, string_new( name_len, fd.cFileName ) );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *path = _new;

        if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            if( include_dirs ) {
                _new = dstring_append(
                    *out_buffer, string_new( dstring_len( *path ) + 1 , *path ) );
                if( !_new ) {
                    FindClose( find_file );
                    return false;
                }
                *out_buffer = _new;
                *out_count += 1;
            }

            if( recursive ) {
                if( !path_walk_dir_internal_short(
                    path, recursive, include_dirs, out_count, out_buffer
                ) ) {
                    FindClose( find_file );
                    return false;
                }
            }

            dstring_truncate( *path, original_len );
            continue;
        }

        _new = dstring_append(
            *out_buffer, string_new( dstring_len( *path ) + 1, *path ) );
        if( !_new ) {
            FindClose( find_file );
            return false;
        }
        *out_buffer = _new;
        *out_count += 1;

        dstring_truncate( *path, original_len );
    } while( FindNextFileA( find_file, &fd ) != FALSE );

    FindClose( find_file );
    return true;
}
static b32 path_walk_dir_internal(
    dstring** path, b32 recursive, b32 include_dirs,
    usize* out_count, dstring** out_buffer
) {
    if( dstring_len( *path ) >= PATH_MAX ) {
        return path_walk_dir_internal_long(
            path, recursive, include_dirs, out_count, out_buffer );
    } else {
        return path_walk_dir_internal_short(
            path, recursive, include_dirs, out_count, out_buffer );
    }
}

static DWORD fd_open_dwaccess( FileOpenFlags flags ) {
    DWORD res = 0;
    if( flags & FOPEN_READ ) {
        res |= GENERIC_READ;
    }
    if( flags & FOPEN_WRITE ) {
        res |= GENERIC_WRITE;
    }
    return res;
}
static DWORD fd_open_dwcreationdisposition( FileOpenFlags flags ) {
    DWORD res = CREATE_ALWAYS;
    if( flags & FOPEN_CREATE ) {
        res = CREATE_ALWAYS;
    } else if( flags & FOPEN_TRUNCATE ) {
        res = TRUNCATE_EXISTING;
    }
    return res;
}
static b32 fd_open_short( string path, FileOpenFlags flags, FD* out_file ) {
    DWORD dwDesiredAccess       = fd_open_dwaccess( flags );
    DWORD dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD dwCreationDisposition = fd_open_dwcreationdisposition( flags );
    DWORD dwFlagsAndAttributes  = 0;

    HANDLE handle = CreateFileA(
        path.cc, dwDesiredAccess, dwShareMode, 0,
        dwCreationDisposition, dwFlagsAndAttributes, 0 );
    if( handle == INVALID_HANDLE_VALUE ) {
        DWORD error_code = GetLastError();
        string msg = win32_local_error_message( error_code );

        cb_error(
            "failed to open '%s'! reason: (0x%X) %s", path.cc, error_code, msg.cc );
        return false;
    }

    *out_file = (usize)handle;
    return true;
}
static b32 fd_open_long( string path, FileOpenFlags flags, FD* out_file ) {
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
        string msg = win32_local_error_message( error_code );

        cb_error(
            "failed to open '%S'! reason: (0x%X) %s", wide, error_code, msg.cc );
        return false;
    }

    *out_file = (usize)handle;
    return true;
}

b32 fd_open( const cstr* path, FileOpenFlags flags, FD* out_file ) {
    if( !validate_file_flags( flags ) ) {
        return false;
    }
    string path_str = string_from_cstr( path );
    if( path_str.len >= PATH_MAX ) {
        return fd_open_long( path_str, flags, out_file );
    } else {
        return fd_open_short( path_str, flags, out_file );
    }
}
void fd_close( FD* file ) {
    CloseHandle( (HANDLE)*file );
    *file = 0;
}

static b32 fd_write_32(
    FD* file, DWORD size, const void* buf, DWORD* opt_out_write_size
) {
    DWORD out_size = 0;
    BOOL res = WriteFile( (HANDLE)*file, buf, size, &out_size, 0 );
    if( opt_out_write_size ) {
        *opt_out_write_size = out_size;
    }
    return res;
}
#if defined(ARCH_64BIT)
static b32 fd_write_64(
    FD* file, usize size, const void* buf, usize* opt_out_write_size
) {
    DWORD size0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD size1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    usize write_total = 0;
    DWORD write_size  = 0;
    b32 res = fd_write_32( file, size0, buf, &write_size );
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
        res = fd_write_32( file, size1, (const u8*)buf + size0, &write_size );
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

static b32 fd_read_32(
    FD* file, DWORD size, void* buf, DWORD* opt_out_read_size
) {
    DWORD out_size = 0;
    BOOL res = ReadFile( (HANDLE)*file, buf, size, &out_size, 0 );
    if( opt_out_read_size ) {
        *opt_out_read_size = out_size;
    }
    return res;
}
#if defined(ARCH_64BIT)
static b32 fd_read_64(
    FD* file, usize size, void* buf, usize* opt_out_read_size
) {
    DWORD size0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD size1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    usize read_total = 0;
    DWORD read_size  = 0;
    b32 res = fd_read_32( file, size0, buf, &read_size );
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
        res = fd_read_32( file, size1, (u8*)buf + size0, &read_size );
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

b32 fd_write( FD* file, usize size, const void* buf, usize* opt_out_write_size ) {
#if defined(ARCH_64BIT)
    return fd_write_64( file, size, buf, opt_out_write_size );
#else
    return fd_write_32( file, size, buf, (DWORD*)opt_out_write_size );
#endif
}
b32 fd_read( FD* file, usize size, void* buf, usize* opt_out_read_size ) {
#if defined(ARCH_64BIT)
    return fd_read_64( file, size, buf, opt_out_read_size );
#else
    return fd_read_32( file, size, buf, (DWORD*)opt_out_read_size );
#endif
}
b32 fd_truncate( FD* file ) {
    return SetEndOfFile( (HANDLE)*file ) != FALSE;
}
usize fd_query_size( FD* file ) {
#if defined(ARCH_64BIT)
    LARGE_INTEGER li;
    expect( GetFileSizeEx( (HANDLE)*file, &li ), "failed to get file size!" );
    return li.QuadPart;
#else
    DWORD res = GetFileSize( (HANDLE)*file, 0 );
    return res;
#endif
}
void fd_seek( FD* file, FileSeek type, isize seek ) {
    DWORD dwMoveMethod = 0;
    switch( type ) {
        case FSEEK_CURRENT: {
            dwMoveMethod = FILE_CURRENT;
        } break;
        case FSEEK_BEGIN: {
            dwMoveMethod = FILE_BEGIN;
        } break;
        case FSEEK_END: {
            dwMoveMethod = FILE_END;
        } break;
    }

#if defined(ARCH_64BIT)
    LARGE_INTEGER li;
    li.QuadPart = seek;

    expect(
        SetFilePointerEx( (HANDLE)*file, li, 0, dwMoveMethod ) != FALSE,
        "failed to seek file!" );
#else
    SetFilePointer( (HANDLE)*file, seek, 0, dwMoveMethod );
#endif
}
usize fd_query_position( FD* file ) {
    DWORD dwMoveMethod = FILE_CURRENT;

#if defined(ARCH_64BIT)
    LARGE_INTEGER li;
    li.QuadPart = 0;

    LARGE_INTEGER res;
    expect(
        SetFilePointerEx( (HANDLE)*file, li, &res, dwMoveMethod ) != FALSE,
        "failed to query file position!" );

    return res.QuadPart;
#else
    DWORD res = SetFilePointer( (HANDLE)*file, 0, 0, dwMoveMethod );
    return res;
#endif

}
static void file_query_time_short(
    const cstr* path, FILETIME* out_create, FILETIME* out_modify
) {
    DWORD dwDesiredAccess       = 0;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes  = 0;
    DWORD dwShareMode           =
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

    HANDLE handle = CreateFileA(
        path, dwDesiredAccess, dwShareMode,
        NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL );
    expect( handle != INVALID_HANDLE_VALUE, "failed to open file!" );
    expect(
        GetFileTime( handle, out_create, 0, out_modify ),
        "failed to get file time!" );

    CloseHandle( handle );
}
static void file_query_time_long(
    string path, FILETIME* out_create, FILETIME* out_modify
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

    expect( handle != INVALID_HANDLE_VALUE, "failed to open file!" );
    expect(
        GetFileTime( handle, out_create, 0, out_modify ),
        "failed to get file time!" );

    CloseHandle( handle );
}
time_t file_query_time_create( const cstr* path ) {
    FILETIME create;
    memory_zero( &create, sizeof(create) );

    usize path_len = cstr_len( path );
    if( path_len >= PATH_MAX ) {
        file_query_time_long( string_new( path_len, path ), &create, 0 );
    } else {
        file_query_time_short( path, &create, 0 );
    }

    return win32_filetime_to_posix( create );
}
time_t file_query_time_modify( const cstr* path ) {
    FILETIME modify;
    memory_zero( &modify, sizeof(modify) );

    usize path_len = cstr_len( path );
    if( path_len >= PATH_MAX ) {
        file_query_time_long( string_new( path_len, path ), 0, &modify );
    } else {
        file_query_time_short( path, 0, &modify );
    }

    return win32_filetime_to_posix( modify );
}

static b32 file_move_short( const cstr* dst, const cstr* src ) {
    return MoveFileA( src, dst ) == TRUE;
}
static b32 file_move_long( string dst, string src ) {
    wchar_t* dst_wide = win32_local_path_canon( dst );
    wchar_t* src_wide = win32_local_path_canon( src );

    b32 res = MoveFileW( src_wide, dst_wide );
    return res == TRUE;
}

b32 file_move( const cstr* dst, const cstr* src ) {
    assertion( dst && src, "null path provided!" );
    usize dst_len = cstr_len( dst );
    usize src_len = cstr_len( src );

    if( dst_len >= PATH_MAX || src_len >= PATH_MAX ) {
        return
            file_move_long( string_new( dst_len, dst ), string_new( src_len, src ) );
    } else {
        return file_move_short( dst, src );
    }
}

static b32 file_copy_short( const cstr* dst, const cstr* src ) {
    return CopyFileA( src, dst, FALSE ) == TRUE;
}
static b32 file_copy_long( string dst, string src ) {
    wchar_t* dst_wide = win32_local_path_canon( dst );
    wchar_t* src_wide = win32_local_path_canon( src );

    b32 res = CopyFileW( src_wide, dst_wide, FALSE );
    return res == TRUE;
}

b32 file_copy( const cstr* dst, const cstr* src ) {
    assertion( dst && src, "null path provided!" );
    usize dst_len = cstr_len( dst );
    usize src_len = cstr_len( src );

    if( dst_len >= PATH_MAX || src_len >= PATH_MAX ) {
        return
            file_copy_long( string_new( dst_len, dst ), string_new( src_len, src ) );
    } else {
        return file_copy_short( dst, src );
    }
}
static b32 file_remove_short( const cstr* path ) {
    return DeleteFileA( path ) != FALSE;
}
static b32 file_remove_long( string path ) {
    wchar_t* path_wide = win32_local_path_canon( path );
    return DeleteFileW( path_wide ) != FALSE;
}
b32 file_remove( const cstr* path ) {
    usize path_len = cstr_len( path );
    if( path_len >= MAX_PATH ) {
        return file_remove_long( string_new( path_len, path ) );
    } else {
        return file_remove_short( path );
    }
}
static b32 dir_create_short( const cstr* path ) {
    if( CreateDirectoryA( path, 0 ) ) {
        return true;
    } else {
        DWORD res = GetLastError();
        return res == ERROR_ALREADY_EXISTS;
    }
}
static b32 dir_create_long( string path ) {
    wchar_t* wpath = win32_local_path_canon( path );
    if( CreateDirectoryW( wpath, 0 ) ) {
        return true;
    } else {
        DWORD res = GetLastError();
        return res == ERROR_ALREADY_EXISTS;
    }
}
b32 dir_create( const cstr* path ) {
    usize len = cstr_len( path );
    if( len >= PATH_MAX ) {
        return dir_create_long( string_new( len, path ) );
    } else {
        return dir_create_short( path );
    }
}
static b32 dir_remove_internal_short( const cstr* path ) {
    return RemoveDirectoryA( path );
}
static b32 dir_remove_internal_long( string path ) {
    wchar_t* wpath = win32_local_path_canon( path );
    return RemoveDirectoryW( wpath );
}
static b32 dir_remove_internal( const cstr* path ) {
    usize len = cstr_len( path );
    if( len >= PATH_MAX ) {
        return dir_remove_internal_long( string_new( len, path ) );
    } else {
        return dir_remove_internal_short( path );
    }
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

f64 timer_milliseconds(void) {
    return timer_seconds() * 1000.0;
}
f64 timer_seconds(void) {
    LARGE_INTEGER qpf, qpc;
    QueryPerformanceFrequency( &qpf );
    QueryPerformanceCounter( &qpc );

    return (f64)qpc.QuadPart / (f64)qpf.QuadPart;
}

b32 mutex_create( Mutex* out_mutex ) {
    HANDLE handle = CreateMutexA( 0, 0, 0 );
    if( !handle ) {
        return false;
    }
    out_mutex->handle = (void*)handle;
    return true;
}
b32 mutex_is_valid( Mutex* mutex ) {
    return mutex->handle != NULL;
}
void mutex_lock( Mutex* mutex ) {
    WaitForSingleObject( mutex->handle, INFINITE );
}
b32 mutex_lock_timed( Mutex* mutex, u32 ms ) {
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
    memory_zero( mutex, sizeof(*mutex) );
}

b32 semaphore_create( Semaphore* out_semaphore ) {
    HANDLE handle = CreateSemaphoreA( NULL, 0, INT32_MAX, 0 );
    if( !handle ) {
        return false;
    }
    out_semaphore->handle = handle;
    return true;
}
b32 semaphore_is_valid( Semaphore* semaphore ) {
    return semaphore->handle != NULL;
}
void semaphore_wait( Semaphore* semaphore ) {
    WaitForSingleObject( semaphore->handle, INFINITE );
}
b32 semaphore_wait_timed( Semaphore* semaphore, u32 ms ) {
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
    memory_zero( semaphore, sizeof(*semaphore) );
}

void thread_sleep( u32 ms ) {
    Sleep( ms );
}

void pipe_open( ReadPipe* out_read, WritePipe* out_write ) {
    HANDLE read = 0, write = 0;
    SECURITY_ATTRIBUTES sa;
    memory_zero( &sa, sizeof(sa) );

    sa.nLength        = sizeof(sa);
    sa.bInheritHandle = TRUE;

    expect( CreatePipe( &read, &write, &sa, 0 ), "failed to create pipes!" );

    *out_read  = (isize)read;
    *out_write = (isize)write;
}
void pipe_close( Pipe pipe ) {
    HANDLE handle = (HANDLE)pipe;
    CloseHandle( handle );
}
b32 process_in_path( const cstr* process_name ) {
    char* cmd = local_fmt( "where.exe %s /Q", process_name );
    return system( cmd ) == 0;
}
PID process_exec(
    Command cmd, b32 redirect_void, ReadPipe* opt_stdin,
    WritePipe* opt_stdout, WritePipe* opt_stderr
) {
    STARTUPINFOA        startup;
    PROCESS_INFORMATION info;

    memory_zero( &startup, sizeof(startup) );
    memory_zero( &info,    sizeof(info) );

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

    cstr* cmd_line = (cstr*)command_flatten_local( &cmd );

    BOOL res = CreateProcessA(
        NULL, cmd_line, NULL, NULL, bInheritHandle,
        flags, NULL, NULL, &startup, &info );
    expect( res, "failed to launch process '%s'!", cmd.args[0] );

    CloseHandle( info.hThread );
    return (isize)info.hProcess;
}
int process_wait( PID pid ) {
    DWORD res = WaitForSingleObject( (HANDLE)pid, INFINITE );
    switch( res ) {
        case WAIT_OBJECT_0: break;
        default: {
            string reason = win32_local_error_message( GetLastError() );
            panic( "failed to wait for pid! reason: %s", reason.cc );
            return -1;
        } break;
    }

    DWORD exit_code = 0;
    expect( GetExitCodeProcess( (HANDLE)pid, &exit_code ), "failed to get exit code!" );

    CloseHandle( (HANDLE)pid );
    return exit_code;
}
b32 process_wait_timed( PID pid, int* opt_out_res, u32 ms ) {
    b32 success = true;
    DWORD res   = WaitForSingleObject( (HANDLE)pid, ms );
    switch( res ) {
        case WAIT_OBJECT_0: break;
        case WAIT_TIMEOUT: {
            success = false;
        } break;
        default: {
            string reason = win32_local_error_message( GetLastError() );
            panic( "failed to wait for pid! reason: %s", reason.cc );
            return -1;
        } break;
    }

    DWORD exit_code = 0;
    expect( GetExitCodeProcess( (HANDLE)pid, &exit_code ), "failed to get exit code!" );

    if( success ) {
        CloseHandle( (HANDLE)pid );
    }

    *opt_out_res = exit_code;
    return success;
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
    expect(
        global_thread_id_source < (CBUILD_THREAD_COUNT + 1),
        "exceeded maximum number of threads!" );

    u32 id = atomic_add( &global_thread_id_source, 1 );

    struct Win32ThreadParams* p = global_win32_thread_params + (id - 1);
    p->id     = id;
    p->params = params;
    p->proc   = func;

    fence();

    HANDLE h = (HANDLE)_beginthreadex( 0, 0, win32_thread_proc, p, 0, 0 );
    expect( h != NULL, "failed to create thread!" );
}

char* internal_cwd(void) {
    DWORD len = GetCurrentDirectoryA( 0, 0 );
    char* buf = memory_alloc( len );

    expect( buf, "failed to allocate working directory buffer!" );

    GetCurrentDirectoryA( len, buf );

    for( usize i = 0; i < len; ++i ) {
        if( buf[i] == '\\' ) {
            buf[i] = '/';
        }
    }

    return buf;
}
char* internal_home(void) {
    const char* drive = getenv( "HOMEDRIVE" );
    expect( drive, "failed to get home directory drive!" );
    const char* home  = getenv( "HOMEPATH" );
    expect( home, "failed to get home path!" );

    dstring* buf = dstring_concat_cstr( drive, home );
    expect( buf, "failed to allocate home directory buffer!" );

    usize len = dstring_len( buf );
    for( usize i = 0; i < len; ++i ) {
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
#if defined(ARCH_64BIT)
    u32  __padding;
#endif
};

struct PosixThreadParams {
    JobFN* proc;
    void*  params;
    u32    id;
};
static struct PosixThreadParams global_posix_thread_params[CBUILD_THREAD_COUNT];

static struct timespec ms_to_timespec( u32 ms ) {
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    return ts;
}
static f64 timespec_to_ms( struct timespec* ts ) {
    return ((f64)ts->tv_sec * 1000.0) + ((f64)ts->tv_nsec / 1000000.0);
}
static const char* generate_semaphore_name(void) {
    atom val = atomic_add( &global_semaphore_val, 1 );
    return (const char*)local_fmt( "cbuild_sem%u", val );
}

b32 path_is_absolute( const cstr* path ) {
    return *path == '/';
}
b32 path_exists( const cstr* path ) {
    return access( path, F_OK ) == 0;
}
static b32 path_walk_dir_internal(
    dstring** path, b32 recursive, b32 include_dirs,
    usize* out_count, dstring** out_buffer
) {
    struct dirent* entry;
    DIR* dir = opendir( *path );
    if( !dir ) {
        return false;
    }

    usize original_len = dstring_len( *path );
    long  pos = 0;

    while( (entry = readdir( dir )) ) {
        if(
            cstr_cmp( entry->d_name, "." ) ||
            cstr_cmp( entry->d_name, ".." ) ||
            cstr_cmp( entry->d_name, ".git" )
        ) {
            continue;
        }
        dstring* _new = dstring_push( *path, '/' );
        if( !_new ) {
            closedir( dir );
            return false;
        }
        *path = _new;

        usize name_len = cstr_len( entry->d_name );
        _new = dstring_append( *path, string_new( name_len, entry->d_name ) );
        if( !_new ) {
            closedir( dir );
            return false;
        }
        *path = _new;

        if( entry->d_type == DT_DIR ) {
            if( include_dirs ) {
                _new = dstring_append(
                    *out_buffer, string_new( dstring_len( *path ) + 1, *path ) );
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

                dstring_truncate( *path, original_len );
                dir = opendir( *path );

                seekdir( dir, pos );
            }

            dstring_truncate( *path, original_len );
            continue;
        }

        _new = dstring_append(
            *out_buffer, string_new( dstring_len( *path ) + 1, *path ) );
        if( !_new ) {
            closedir( dir );
            return false;
        }
        *out_buffer = _new;

        *out_count += 1;

        dstring_truncate( *path, original_len );
    }

    closedir( dir );
    return true;
}

b32 fd_open( const cstr* path, FileOpenFlags flags, FD* out_file ) {
    if( !validate_file_flags( flags ) ) {
        return false;
    }
    int oflag = 0;
    if( (flags & (FOPEN_READ | FOPEN_WRITE)) == FOPEN_READ ) {
        oflag |= O_RDONLY;
    } else if( (flags & (FOPEN_READ | FOPEN_WRITE)) == FOPEN_WRITE ) {
        oflag |= O_WRONLY;
    } else { // read + write
        oflag |= O_RDWR;
    }
    if( flags & FOPEN_TRUNCATE ) {
        oflag |= O_TRUNC;
    }
    if( flags & FOPEN_CREATE ) {
        oflag |= O_CREAT;
    }
    if( flags & FOPEN_APPEND ) {
        oflag |= O_APPEND;
    }

    int fd = open( path, oflag );
    if( fd < 0 ) {
        cb_error( "fd_open: failed to open '%s'!", path );
        return false;
    }

    *out_file = fd;
    return true;
}
void fd_close( FD* file ) {
    close( *file );
    *file = 0;
}
b32 fd_write( FD* file, usize size, const void* buf, usize* opt_out_write_size ) {
    isize write_size = write( *file, buf, size );
    if( write_size < 0 ) {
        return false;
    }

    if( *opt_out_write_size ) {
        *opt_out_write_size = write_size;
    }
    return true;
}
b32 fd_read( FD* file, usize size, void* buf, usize* opt_out_read_size ) {
    isize read_size = read( *file, buf, size );
    if( read_size < 0 ) {
        return false;
    }

    if( opt_out_read_size ) {
        *opt_out_read_size = read_size;
    }

    return true;
}
b32 fd_truncate( FD* file ) {
    usize pos = fd_query_position( file ); // fd_query_position handles lseek fail
    return ftruncate( *file, pos ) == 0;
}
usize fd_query_size( FD* file ) {
    off_t pos = lseek( *file, 0, SEEK_CUR );
    expect( pos >= 0, "failed to query file descriptor size!" );
    off_t res = lseek( *file, 0, SEEK_END );
    expect( res >= 0, "failed to query file descriptor size!" );
    expect( lseek( *file, pos, SEEK_SET ) >= 0,
        "failed to query file descriptor size!" );

    return res;
}
void fd_seek( FD* file, FileSeek type, isize seek ) {
    static const int local_whence[] = {
        SEEK_CUR, // FSEEK_CURRENT
        SEEK_SET, // FSEEK_BEGIN
        SEEK_END, // FSEEK_END
    };
    expect( lseek( *file, seek, local_whence[type] ) >= 0, "failed to seek!" );
}
usize fd_query_position( FD* file ) {
    off_t pos = lseek( *file, 0, SEEK_CUR );
    expect( pos >= 0, "failed to get current file position!" );
    return pos;
}
time_t file_query_time_create( const cstr* path ) {
    struct stat st;
    expect( stat( path, &st ) == 0,
        "failed to query create time for '%s'!", path );
    return st.st_ctime;
}
time_t file_query_time_modify( const cstr* path ) {
    struct stat st;
    expect( stat( path, &st ) == 0,
        "failed to query modify time for '%s'!", path );
    return st.st_mtime;
}
b32 file_move( const cstr* dst, const cstr* src ) {
    return rename( src, dst ) == 0;
}
b32 file_copy( const cstr* dst, const cstr* src ) {
    FD src_file, dst_file;
    if( !fd_open( src, FOPEN_READ, &src_file ) ) {
        return false;
    }
    if( path_exists( dst ) ) {
        if( !fd_open( dst, FOPEN_WRITE | FOPEN_TRUNCATE, &dst_file ) ) {
            fd_close( &src_file );
            return false;
        }
    } else {
        if( !fd_open( dst, FOPEN_WRITE | FOPEN_CREATE, &dst_file ) ) {
            fd_close( &src_file );
            return false;
        }
    }

    char* buf = (char*)local_byte_buffer();
    usize rem = fd_query_size( &src_file );

    while( rem ) {
        usize max_write = rem;
        if( rem > CBUILD_LOCAL_BUFFER_CAPACITY ) {
            max_write = CBUILD_LOCAL_BUFFER_CAPACITY;
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
b32 file_remove( const cstr* path ) {
    int res = remove( path );
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
#if defined(ARCH_X86)
    __asm__ volatile ("mfence":::"memory");
#elif defined(ARCH_ARM)
    __asm__ volatile ("dmb":::"memory");
#else
    __asm__ volatile ("":::"memory");
#endif
}

b32 mutex_create( Mutex* out_mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)out_mutex;
    mtx->value = 0;
    return true;
}
b32 mutex_is_valid( Mutex* mutex ) {
    return true;
}
void mutex_lock( Mutex* mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)mutex;
    while( atomic_compare_swap( &mtx->value, 0, mtx->value + 1 ) != 0 ) {
        thread_sleep(1);
    }
}
b32 mutex_lock_timed( Mutex* mutex, u32 ms ) {
    if( ms == MT_WAIT_INFINITE ) {
        mutex_lock( mutex );
        return true;
    }

    struct PosixMutex* mtx = (struct PosixMutex*)mutex;

    for( u32 i = 0; i < ms; ++i ) {
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

b32 semaphore_create( Semaphore* out_semaphore ) {
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

        cb_error( "failed to create semaphore! reason: %s", reason );
        return false;
    }
    out_semaphore->handle = s;
    return true;
}
b32 semaphore_is_valid( Semaphore* semaphore ) {
    return semaphore->handle != NULL;
}
void semaphore_wait( Semaphore* semaphore ) {
    expect( sem_wait( semaphore->handle ) == 0, "failed to wait on semaphore!" );
}
b32 semaphore_wait_timed( Semaphore* semaphore, u32 ms ) {
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
        expect( res == 0, "failed to wait on semaphore!" );
        return true;
    }
}
void semaphore_signal( Semaphore* semaphore ) {
    expect( sem_post( semaphore->handle ) == 0, "failed to post semaphore!" );
}
void semaphore_destroy( Semaphore* semaphore ) {
    sem_close( semaphore->handle );
    *semaphore = semaphore_null();
}

void pipe_open( ReadPipe* out_read, WritePipe* out_write ) {
    int fd[2];
    expect( pipe( fd ) != -1, "failed to create pipes!" );
    *out_read  = fd[0];
    *out_write = fd[1];
}
void pipe_close( Pipe pipe ) {
    close( pipe );
}

b32 process_in_path( const cstr* process_name ) {
    Command cmd = command_new( "which", process_name );

    PID pid = process_exec( cmd, true, 0, 0, 0 );
    int res = process_wait( pid );

    return res == 0;
}
PID process_exec(
    Command cmd, b32 redirect_void, ReadPipe* opt_stdin,
    WritePipe* opt_stdout, WritePipe* opt_stderr
) {
    ReadPipe   stdin_;
    WritePipe stdout_;
    WritePipe stderr_;

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
    expect( pid >= 0, "failed to fork!" );

    if( pid ) { // thread that ran process_exec
        return pid;
    }

    // thread where process will run

    expect_crash( dup2( stdin_ , STDIN_FILENO  ) >= 0, "failed to setup stdin!" );
    expect_crash( dup2( stdout_, STDOUT_FILENO ) >= 0, "failed to setup stdout!" );
    expect_crash( dup2( stderr_, STDERR_FILENO ) >= 0, "failed to setup stderr!" );

    expect_crash( execvp(
        cmd.args[0], (char* const*)(cmd.args) // cmd.args always has a null string at the end
    ) >= 0, "failed to execute command!" );
    exit(0);
}
int process_wait( PID pid ) {
    int wstatus = 0;
    expect( waitpid( pid, &wstatus, 0 ) == pid, "failed to wait for process!" );

    if( WIFEXITED( wstatus ) ) {
        int status = WEXITSTATUS( wstatus );
        return status;
    }

    return -1;
}
b32 process_wait_timed( PID pid, int* opt_out_res, u32 ms ) {
    if( ms == MT_WAIT_INFINITE ) {
        int res = process_wait( pid );
        if( opt_out_res ) {
            *opt_out_res = res;
        }
        return true;
    }

    for( u32 i = 0; i < ms; ++i ) {
        int wstatus = 0;
        pid_t v = waitpid( pid, &wstatus, WNOHANG );
        expect( v != -1, "failed to wait for process!" );

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

f64 timer_milliseconds(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    fence();
    return timespec_to_ms( &ts );
}
f64 timer_seconds(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    fence();
    return timespec_to_ms( &ts ) / 1000.0;
}

void thread_sleep( u32 ms ) {
    struct timespec ts = ms_to_timespec( ms );

    // this should never fail.
    expect( nanosleep( &ts, 0 ) != EFAULT, "nanosleep failed!" );
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
    expect(
        global_thread_id_source < (CBUILD_THREAD_COUNT + 1),
        "exceeded maximum number of threads! max: %u", CBUILD_THREAD_COUNT );

    u32 id = atomic_add( &global_thread_id_source, 1 );

    struct PosixThreadParams* p = global_posix_thread_params + (id - 1);
    p->id     = id;
    p->params = params;
    p->proc   = func;

    fence();

    pthread_t thread;
    int res = pthread_create( &thread, NULL, posix_thread_proc, p );
    expect( res == 0, "failed to create thread!" );
}

char* internal_cwd(void) {
    char* buf = memory_alloc( PATH_MAX );
    expect( buf, "failed to allocate working directory buffer!" );

    char* res = getcwd( buf, PATH_MAX );
    expect( res, "failed to get working directory!" );

    return res;
}
char* internal_home(void) {
    char* home = getenv( "HOME" );
    expect( home, "failed to get home directory!" );

    return home;
}

#endif /* POSIX end */

#endif /* CBUILD_IMPLEMENTATION end */
