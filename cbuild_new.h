#if !defined(CBUILD_HEADER)
#define CBUILD_HEADER
/**
 * @file   cbuild_new.h
 * @brief  
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 21, 2024
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

#if defined(__clang__)
    #define COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define COMPILER_GCC 1
#elif defined(_MSC_VER)
    #define COMPILER_MSVC 1
#else
    #define COMPILER_UNKNOWN 1
#endif

#if defined(_WIN32)
    #define PLATFORM_WINDOWS 1
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define PLATFORM_MINGW 1
    #endif
#elif defined(__linux__)
    #define PLATFORM_LINUX 1
    #define PLATFORM_POSIX 1
#elif defined(__APPLE__)
    #define PLATFORM_MACOS 1
    #define PLATFORM_POSIX 1
#else
    #define PLATFORM_UNKNOWN 1
    #if __has_include(<unistd.h>)
        #define PLATFORM_POSIX 1
    #endif
#endif

#if defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64)
    #define ARCH_ARM 1
#elif defined(__i386__) || defined(_M_IX86) || defined(__X86__) || defined(__x86_64__)
    #define ARCH_X86 1
#else
    #define ARCH_UNKNOWN 1
#endif

#if UINT64_MAX == UINTPTR_MAX
    #define ARCH_64BIT 1
#else
    #define ARCH_32BIT 1
#endif

#define CBUILD_PATH_CAPACITY (4096)
#if defined(PLATFORM_WINDOWS)
    #undef CBUILD_PATH_CAPACITY
    #define CBUILD_PATH_CAPACITY (8192)
#endif

#if !defined(CBUILD_LOCAL_STRING_COUNT)
    #define CBUILD_LOCAL_STRING_COUNT (4)
#else
    #if CBUILD_LOCAL_STRING_COUNT <= 2
        #define CBUILD_LOCAL_STRING_COUNT (2)
    #endif
#endif
#define CBUILD_LOCAL_STRING_CAPACITY CBUILD_PATH_CAPACITY

#define CBUILD_THREAD_COUNT_MIN (1)
#define CBUILD_THREAD_COUNT_MAX (16)

#if !defined(CBUILD_THREAD_COUNT)
    #define CBUILD_THREAD_COUNT (8)
#else
    #if CBUILD_THREAD_COUNT > CBUILD_THREAD_COUNT_MAX
        #define CBUILD_THREAD_COUNT CBUILD_THREAD_COUNT_MAX
    #elif CBUILD_THREAD_COUNT < CBUILD_THREAD_COUNT_MIN
        #define CBUILD_THREAD_COUNT CBUILD_THREAD_COUNT_MIN
    #endif
#endif

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef uintptr_t usize;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef intptr_t isize;

typedef float  f32;
typedef double f64;

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

#if defined(PLATFORM_WINDOWS)
    typedef volatile long      atom;
    typedef volatile long long atom64;
#else
    typedef volatile i32 atom;
    typedef volatile i64 atom64;
#endif

typedef char cstr;
typedef char dstring;

typedef struct {
    usize       len;
    const char* cc;
} string;

typedef enum {
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_WARNING,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_FATAL,
} LoggerLevel;

typedef enum {
    FTYPE_UNKNOWN,
    FTYPE_FILE,
    FTYPE_DIRECTORY,
} FileType;
typedef enum {
    FSEEK_CURRENT,
    FSEEK_BEGIN,
    FSEEK_END,
} FileSeek;
typedef enum {
    FOPEN_READ     = (1 << 0),
    FOPEN_WRITE    = (1 << 1),
    FOPEN_CREATE   = (1 << 2),
    FOPEN_TRUNCATE = (1 << 3),
    FOPEN_APPEND   = (1 << 4),
} FileOpenFlags;

typedef struct {
    const void* opaque; // char* on posix, wchar_t* on windows
} PathCanonical;

#if defined(PLATFORM_WINDOWS)
    typedef isize File;
    typedef isize PID;
#else
    typedef int   File;
    typedef pid_t PID;
#endif
typedef isize Mutex;
typedef isize Semaphore;
typedef File  Pipe;
typedef u32   ThreadCtx;

#define THREAD_ANY_CONTEXT (UINT32_MAX)
#define THREAD_MAIN (0)
#define MT_WAIT_INFINITE (UINT32_MAX)

typedef void JobFN( ThreadCtx thread, void* params );

static inline void _0( int _, ... ) {(void)_;}
#define unused( ... ) _0( 0, __VA_ARGS__ )

#if defined(COMPILER_MSVC)
    #define panic()          __debugbreak()
    #define unreachable()    __assume(0)
    #define debugger_break() panic()
#else
    #define panic()          __builtin_trap()
    #define unreachable()    __builtin_unreachable()
    #define debugger_break() __builtin_debugtrap()
#endif

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

void* memory_alloc( usize size );
void* memory_realloc( void* memory, usize old_size, usize new_size );
void  memory_free( void* memory, usize size );
usize memory_query_usage(void);
usize memory_query_total_usage(void);
void  memory_stamp( void* memory, usize value_size, const void* value, usize size );
void  memory_set( void* memory, i8 value, usize size );
void  memory_zero( void* memory, usize size );
void  memory_copy( void* restrict dst, const void* restrict src, usize size );
void  memory_move( void* dst, const void* src, usize size );
b32   memory_cmp( const void* a, const void* b, usize size );

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
#define string_new( _len, buf )\
    (string){ .cc=buf, .len=_len }
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
usize string_len_utf8( string str );

dstring* dstring_empty( usize cap );
dstring* dstring_new( usize len, const char* str );
#define dstring_from_cstr( str ) dstring_new( cstr_len( str ), str )
#define dstring_from_string( str ) dstring_new( str.len, str.cc )
#define dstring_from_string_ptr( str ) dstring_new( (str)->len, (str)->cc )
#define dstring_text( literal ) dstring_new( sizeof(literal)-1, literal )
#define dstring_to_string( dstr ) string_from_dstring( dstr )
dstring* dstring_grow( dstring* str, usize amount );
dstring* dstring_clone( dstring* src );
dstring* dstring_concat( string lhs, string rhs );
#define dstring_concat_cstr( lhs, rhs )\
    dstring_concat( string_from_cstr( lhs ), string_from_cstr( rhs ) )
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

string path_cwd(void);
string path_home(void);
b32 path_is_absolute( string path );
usize path_chunk_count( string path );
usize path_canonical_calculate_len( string path );
PathCanonical path_canonicalize( string path, usize buf_cap, char* buf );
PathCanonical path_canonicalize_local( ThreadCtx ctx, string path );
#define path_canonicalize_local_text( ctx, literal )\
    path_canonicalize_local( ctx, string_text( literal ) )

void* darray_empty( usize stride, usize cap );
void* darray_from_array( usize stride, usize len, const void* buf );
usize darray_static_memory_requirement( usize stride, usize cap );
void* darray_static( usize stride, usize cap, void* buf );
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

atom   atomic_add( atom* _atom, atom val );
atom64 atomic_add64( atom64* atom, atom64 val );
void   fence(void);

#define mutex_null() (0)
b32  mutex_create( Mutex* out_mutex );
b32  mutex_is_valid( Mutex* mutex );
void mutex_lock( Mutex* mutex );
b32  mutex_lock_timed( Mutex* mutex, u32 ms );
void mutex_unlock( Mutex* mutex );
void mutex_destroy( Mutex* mutex );

#define semaphore_null() (0)
b32  semaphore_create( Semaphore* out_semaphore );
b32  semaphore_is_valid( Semaphore* semaphore );
void semaphore_wait( Semaphore* semaphore );
b32  semaphore_wait_timed( Semaphore* sem, u32 ms );
void semaphore_signal( Semaphore* semaphore );
void semaphore_destroy( Semaphore* semaphore );

char* local_char_buf( ThreadCtx ctx );
char* local_char_buf_fmt_va( ThreadCtx ctx, const char* format, va_list va );
char* local_char_buf_fmt( ThreadCtx ctx, const char* format, ... );

void logger_va( ThreadCtx ctx, LoggerLevel level, const char* format, va_list va );
void logger( ThreadCtx ctx, LoggerLevel level, const char* format, ... );

#define cb_info( ctx, ... )  logger( ctx, LOGGER_LEVEL_INFO, __VA_ARGS__ )
#define cb_warn( ctx, ... )  logger( ctx, LOGGER_LEVEL_WARNING, __VA_ARGS__ )
#define cb_error( ctx, ... ) logger( ctx, LOGGER_LEVEL_ERROR, __VA_ARGS__ )
#define cb_fatal( ctx, ... ) logger( ctx, LOGGER_LEVEL_FATAL, __VA_ARGS__ )

#if defined(CBUILD_ASSERTIONS)
    #define assertion( condition, ... ) do {\
        if( !(condition) ) {\
            cb_fatal( THREAD_ANY_CONTEXT,\
                "condtion '" #condition "' failed in " __FILE__ " on line %i!",\
                __LINE__ );\
            cb_fatal( THREAD_ANY_CONTEXT, __VA_ARGS__ );\
            panic();\
        }\
    } while(0)
#else
    #define assertion( ... ) unused( __VA_ARGS__ )
#endif

#endif /* header guard */

#if defined(CBUILD_IMPLEMENTATION) || defined(_CLANGD)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct GlobalBuffers {
    atom obtained; // atomic boolean

    string cwd;
    string home;
};
struct LocalBuffers {
    char buffers[CBUILD_THREAD_COUNT + 1]
        [CBUILD_LOCAL_STRING_COUNT][CBUILD_LOCAL_STRING_CAPACITY];
    atom index;
};
struct DynamicString {
    usize len, cap;
    char  buf[];
};
struct DynamicArray {
    usize len, cap, stride;
    u8 buf[];
};

static b32 global_is_mt              = false;
atom64     global_memory_usage       = 0;
atom64     global_total_memory_usage = 0;
atom       global_thread_id          = 1; // 0 is main thread

static Mutex       global_logger_mutex = 0;
static LoggerLevel global_logger_level = LOGGER_LEVEL_INFO;

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

static volatile struct LocalBuffers* get_local_buffers(void) {
    if( !global_local_buffers ) {
        global_local_buffers = memory_alloc( sizeof(*global_local_buffers) );
        if( !global_local_buffers ) {
            cb_fatal( THREAD_ANY_CONTEXT,
                "failed to allocate local buffers! size: %zu",
                sizeof(*global_local_buffers) );
            panic();
        }
    }
    return global_local_buffers;
}
static volatile char* get_next_local_buffer( ThreadCtx ctx ) {
    volatile struct LocalBuffers* b = get_local_buffers();

    u32 index = 0;
    if( global_is_mt ) {
        atom _atom = atomic_add( &b->index, 1 );
        index = (*(u32*)&_atom) % CBUILD_LOCAL_STRING_COUNT;
    } else {
        atom _atom = ++b->index;
        index = (*(u32*)&_atom) % CBUILD_LOCAL_STRING_COUNT;
    }

    volatile char* result = b->buffers[ctx][index];
    memory_zero( (void*)result, CBUILD_LOCAL_STRING_CAPACITY );
    return result;
}
static volatile struct GlobalBuffers* get_global_buffers(void) {
    if( !global_buffers.obtained ) {
        atomic_add( &global_buffers.obtained, 1 );
        
        char* cwd  = internal_cwd();
        char* home = internal_home();
        global_buffers.cwd  = string_from_cstr( cwd );
        global_buffers.home = string_from_cstr( home );
    }
    return &global_buffers;
}

void* memory_alloc( usize size ) {
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
void* memory_realloc( void* memory, usize old_size, usize new_size ) {
    assertion( new_size >= old_size, "attempted to reallocate to smaller buffer!" );
    void* res = realloc( memory, new_size );
    if( !res ) {
        return NULL;
    }
    usize diff = new_size - old_size;
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
void memory_free( void* memory, usize size ) {
    if( !memory ) {
        cb_warn( THREAD_ANY_CONTEXT, "attempted to free null pointer!" );
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
usize cstr_len_utf8( const cstr* _string ) {
    return string_len_utf8( string_from_cstr( _string ) );
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
b32 cstr_find_set_rev( const cstr* _string, const cstr* set, usize* opt_out_index ) {
    return string_find_set_rev(
        string_from_cstr( _string ), string_from_cstr( set ), opt_out_index );
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
    const cstr* _string, const cstr* substr, usize* opt_out_index
) {
    return string_find_string_rev(
        string_from_cstr( _string ), string_from_cstr( substr ), opt_out_index );
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
    usize _cap = cap ? cap : 1;
    struct DynamicString* res = memory_alloc( sizeof(*res) + _cap );
    res->cap = _cap;
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
            THREAD_ANY_CONTEXT,
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
            THREAD_ANY_CONTEXT,
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
            THREAD_ANY_CONTEXT,
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
void* darray_grow( void* darray, usize amount ) {
    struct DynamicArray* res = darray_head( darray );
    usize old_size = (res->stride * res->cap) + sizeof(*res);
    usize new_size = (res->stride * amount) + old_size;

    res = memory_realloc( res, old_size, new_size );
    if( !res ) {
        return NULL;
    }

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
        cb_warn( THREAD_ANY_CONTEXT,
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
        cb_warn( THREAD_ANY_CONTEXT,
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
        cb_warn( THREAD_ANY_CONTEXT,
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
        cb_warn( THREAD_ANY_CONTEXT,
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
            THREAD_ANY_CONTEXT,
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
            THREAD_ANY_CONTEXT,
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

char* local_char_buf( ThreadCtx ctx ) {
    fence();
    return (char*)get_next_local_buffer( ctx );
}
char* local_char_buf_fmt_va( ThreadCtx ctx, const char* format, va_list va ) {
    char* buf = local_char_buf( ctx );
    vsnprintf( buf, CBUILD_LOCAL_STRING_CAPACITY - 1, format, va ); // -1 to ensure null-terminated
    return buf;
}
char* local_char_buf_fmt( ThreadCtx ctx, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    char* res = local_char_buf_fmt_va( ctx, format, va );
    va_end( va );
    return res;
}

static b32 logger_check_level( LoggerLevel level ) {
    return level >= global_logger_level;
}

void logger_va( ThreadCtx ctx, LoggerLevel level, const char* format, va_list va ) {
    if( !logger_check_level( level ) ) {
        return;
    }

    if( global_is_mt ) {
        mutex_lock( &global_logger_mutex );
    }

    switch( level ) {
        case LOGGER_LEVEL_INFO: {
            if( ctx == THREAD_ANY_CONTEXT ) {
                printf( "[I:??] cbuild: " );
            } else {
                printf( "[I:%02u] cbuild: ", ctx );
            }
            vfprintf( stdout, format, va );
            printf( "\033[1;00m\n" );
        } break;
        case LOGGER_LEVEL_WARNING: {
            if( ctx == THREAD_ANY_CONTEXT ) {
                printf( "\033[1;33m" "[W:??] cbuild: " );
            } else {
                printf( "\033[1;33m" "[W:%02u] cbuild: ", ctx );
            }
            vfprintf( stdout, format, va );
            printf( "\033[1;00m\n" );
        } break;
        case LOGGER_LEVEL_ERROR: {
            if( ctx == THREAD_ANY_CONTEXT ) {
                fprintf( stderr, "\033[1;31m" "[E:??] cbuild: " );
            } else {
                fprintf( stderr, "\033[1;31m" "[E:%02u] cbuild: ", ctx );
            }
            vfprintf( stderr, format, va );
            fprintf( stderr, "\033[1;00m\n" );
        } break;
        case LOGGER_LEVEL_FATAL: {
            if( ctx == THREAD_ANY_CONTEXT ) {
                fprintf( stderr, "\033[1;35m" "[F:??] cbuild: " );
            } else {
                fprintf( stderr, "\033[1;35m" "[F:%02u] cbuild: ", ctx );
            }
            vfprintf( stderr, format, va );
            fprintf( stderr, "\033[1;00m\n" );
        } break;
    }

    if( global_is_mt ) {
        mutex_unlock( &global_logger_mutex );
    }
}
void logger( ThreadCtx ctx, LoggerLevel level, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    logger_va( ctx, level, format, va );
    va_end( va );
}

#if defined(PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>

struct Win32ThreadParams {
    JobFN* proc;
    void*  params;
    u32    id;
};

static struct Win32ThreadParams global_win32_thread_params[CBUILD_THREAD_COUNT];

b32 path_is_absolute( string path ) {
    if( path.len < 2 ) {
        return false;
    }
    return
        isalpha( path.cc[0] ) &&
        path.cc[1] == ':';
}

atom atomic_add( atom* _atom, atom val ) {
    return _InterlockedExchangeAdd( _atom, val );
}
atom64 atomic_add64( atom64* atom, atom64 val ) {
    return _InterlockedExchangeAdd64( atom, val );
}
void fence(void) {
    MemoryBarrier();
}

b32 mutex_create( Mutex* out_mutex ) {
    HANDLE handle = CreateMutexA( 0, 0, 0 );
    if( !handle ) {
        return false;
    }
    *out_mutex = *(isize*)&handle;
    return true;
}
b32 mutex_is_valid( Mutex* mutex ) {
    HANDLE handle = *(HANDLE*)mutex;
    return handle != NULL;
}
void mutex_lock( Mutex* mutex ) {
    HANDLE handle = *(HANDLE*)mutex;
    WaitForSingleObject( handle, INFINITE );
}
b32 mutex_lock_timed( Mutex* mutex, u32 ms ) {
    if( ms == INFINITE ) {
        mutex_lock( mutex );
        return true;
    }
    HANDLE handle = *(HANDLE*)mutex;
    DWORD  result = WaitForSingleObject( handle, ms );
    return result != WAIT_TIMEOUT;
}
void mutex_unlock( Mutex* mutex ) {
    HANDLE handle = *(HANDLE*)mutex;
    ReleaseMutex( handle );
}
void mutex_destroy( Mutex* mutex ) {
    HANDLE handle = *(HANDLE*)mutex;
    CloseHandle( handle );
    *mutex = 0;
}

b32 semaphore_create( Semaphore* out_semaphore ) {
    HANDLE handle = CreateSemaphoreA( NULL, 0, INT32_MAX, 0 );
    if( !handle ) {
        return false;
    }
    *out_semaphore = *(isize*)&handle;
    return true;
}
b32 semaphore_is_valid( Semaphore* semaphore ) {
    HANDLE handle = *(HANDLE*)semaphore;
    return handle != NULL;
}
void semaphore_wait( Semaphore* semaphore ) {
    mutex_lock( semaphore );
}
b32 semaphore_wait_timed( Semaphore* semaphore, u32 ms ) {
    return mutex_lock_timed( semaphore, ms );
}
void semaphore_signal( Semaphore* semaphore ) {
    HANDLE handle = *(HANDLE*)semaphore;
    ReleaseSemaphore( handle, 1, NULL );
}
void semaphore_destroy( Semaphore* semaphore ) {
    HANDLE handle = *(HANDLE*)semaphore;
    CloseHandle( handle );
    *semaphore = 0;
}

unsigned int win32_thread_proc( void* params ) {
    struct Win32ThreadParams* p = params;
    
    p->proc( p->id, p->params );
    fence();

    return 0;
}

void thread_create( JobFN* func, void* params ) {
    assertion(
        global_thread_id < (CBUILD_THREAD_COUNT + 1),
        "exceeded maximum number of threads!" );

    u32 id = atomic_add( &global_thread_id, 1 );

    struct Win32ThreadParams* p = global_win32_thread_params + (id - 1);
    p->id     = id;
    p->params = params;
    p->proc   = func;

    fence();

    HANDLE h = (HANDLE)_beginthreadex( 0, 0, win32_thread_proc, p, 0, 0 );
    if( !h ) {
        cb_fatal( THREAD_ANY_CONTEXT, "failed to create threads!" );
        panic();
    }
}

char* internal_cwd(void) {
    DWORD len = GetCurrentDirectoryA( 0, 0 );
    char* buf = memory_alloc( len );

    if( !buf ) {
        cb_fatal( THREAD_ANY_CONTEXT, "failed to allocate cwd buffer!" );
        panic();
    }

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
    if( !drive ) {
        cb_fatal( THREAD_ANY_CONTEXT, "failed to get home drive!" );
        panic();
    }
    const char* home  = getenv( "HOMEPATH" );
    if( !home ) {
        cb_fatal( THREAD_ANY_CONTEXT, "failed to get home path!" );
        panic();
    }

    dstring* buf = dstring_concat_cstr( drive, home );
    if( !buf ) {
        cb_fatal( THREAD_ANY_CONTEXT, "failed to allocate home path buffer!" );
        panic();
    }

    usize len = dstring_len( buf );
    for( usize i = 0; i < len; ++i ) {
        if( buf[i] == '\\' ) {
            buf[i] = '/';
        }
    }

    return buf;
}

#else /* WINDOWS */

#endif /* POSIX */

#endif
