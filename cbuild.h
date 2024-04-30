#if !defined(CBUILD_H)
#define CBUILD_H
/**
 * @file   cbuild.h
 * @brief  C build system.
 * @details
 * Single header library, include to get access to API,
 * include again (ONCE) with CBUILD_IMPLEMENTATION defined to
 * define implementation.
 * Inspired by https://github.com/tsoding/nobuild
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   April 06, 2024
*/
// IWYU pragma: begin_exports
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
// IWYU pragma: end_exports

#if defined(__STDC_NO_ATOMICS__) || defined(_MSC_VER)
    typedef volatile int Atom;
#else
    #include <stdatomic.h> // IWYU pragma: export
    typedef volatile atomic_int Atom;
#endif
#if defined(_WIN32)
    typedef void*    ProcessID;
    typedef void*    File;
    typedef ssize_t ssize_t;
#else
    #include <unistd.h>
    typedef pid_t ProcessID;
    typedef int   File;
#endif

#define static_array_len( array )\
    (sizeof((array)) / sizeof((array)[0]))
static inline void _0(int a,...) {(void)a;}
#define unused( ... ) _0( 0, __VA_ARGS__ )

typedef char    dstring;
typedef dstring dpath;

typedef struct substring {
    size_t      len;
    const char* cc;
} substring;

typedef unsigned int FileOpenFlags;
#define FILE_OPEN_FLAG_READ     (1 << 0)
#define FILE_OPEN_FLAG_WRITE    (1 << 1)
#define FILE_OPEN_FLAG_CREATE   (1 << 2)
#define FILE_OPEN_FLAG_TRUNCATE (1 << 3)
#define FILE_OPEN_FLAG_APPEND   (1 << 4)

typedef unsigned int FileSeekType;
#define FILE_SEEK_TYPE_CURRENT 0
#define FILE_SEEK_TYPE_START   1
#define FILE_SEEK_TYPE_END     2

typedef int FileType;
#define FILE_TYPE_UNKNOWN_  0
#define FILE_TYPE_FILE      1
#define FILE_TYPE_DIRECTORY 2

typedef struct {
    FileType type;
    time_t   time_create;
    time_t   time_modify;
    size_t   file_size;
} FileInfo;

typedef int Compiler;
#define COMPILER_UNKNOWN 0
#define COMPILER_GCC     1
#define COMPILER_CLANG   2
#define COMPILER_MSVC    3

typedef int Platform;
#define PLATFORM_UNKNOWN 0
#define PLATFORM_LINUX   1
#define PLATFORM_WINDOWS 2
#define PLATFORM_MACOS   3

typedef int Arch;
#define ARCH_UNKNOWN 0
#define ARCH_ARM     1
#define ARCH_X86     2

typedef _Bool SortCmpFN( size_t stride, const void* a, const void* b );
typedef _Bool FilterFN(
    size_t index, size_t stride, const void* item, void* params );

typedef uint32_t LogLevel;
#define LOG_LEVEL_INFO  0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_ERROR 2

void logger_level_set( LogLevel level );
LogLevel logger_level_get(void);
void logger( LogLevel level, const char* format, ... );
#define cb_info( ... )  logger( LOG_LEVEL_INFO, __VA_ARGS__ )
#define cb_warn( ... )  logger( LOG_LEVEL_WARN, __VA_ARGS__ )
#define cb_error( ... ) logger( LOG_LEVEL_ERROR, __VA_ARGS__ )

void* memory_alloc( size_t size );
void* memory_realloc( void* memory, size_t old_size, size_t new_size );
void  memory_free( void* memory );
void  memory_set( void* memory, char value, size_t size );
void  memory_zero( void* memory, size_t size );
void  memory_copy( void* restrict dst, const void* restrict src, size_t size );
void  memory_move( void* dst, const void* src, size_t size );

#define substr_text( literal )\
    (substring){ .cc=literal, .len=sizeof(literal) - 1 }
#define substr_new( opt_len, string )\
    (substring){ .cc=string, .len=opt_len ? opt_len : string_len( string ) }
#define substr_from_dstring( dstring )\
    (substring){ .cc=dstring, .len=dstring_len( dstring ) }
_Bool     substr_is_empty( substring substr );
_Bool     substr_cmp( substring a, substring b );
_Bool     substr_find( substring substr, char c, size_t* out_pos );
_Bool     substr_find_last( substring substr, char c, size_t* out_pos );
_Bool     substr_find_set( substring substr, substring set, size_t* out_pos );
_Bool     substr_find_last_set( substring substr, substring set, size_t* out_pos );
_Bool     substr_find_substr( substring source, substring substr, size_t* out_pos );
substring substr_advance( substring substr );
substring substr_advance_by( substring substr, size_t amount );
size_t    substr_len_utf8( substring substr );

size_t string_len( const char* string );
size_t string_len_utf8( const char* string );
_Bool  string_cmp( const char* a, const char* b );
_Bool  string_find( const char* string, char c, size_t* out_pos );
_Bool  string_find_last( const char* string, char c, size_t* out_pos );
_Bool  string_find_set( const char* string, const char* set, size_t* out_pos );
_Bool  string_find_string(
    const char* string, const char* substr, size_t* out_pos );

const char* local_fmt( const char* format, ... );

dstring* dstring_empty( size_t size );
dstring* dstring_new( size_t opt_len, const char* source );
#define  dstring_literal( literal ) dstring_new( sizeof(literal) - 1, literal )
#define  dstring_from_substr( substr ) dstring_new( (substr)->len, (substr)->cc )
dstring* dstring_fmt( const char* format, ... );
dstring* dstring_clone( const dstring* source );
void     dstring_free( dstring* string );
void*    dstring_head( dstring* string );
dstring* dstring_grow( dstring* string, size_t amount );
void     dstring_clear( dstring* string );
void     dstring_truncate( dstring* string, size_t max_len );
size_t   dstring_len( const dstring* string );
size_t   dstring_cap( const dstring* string );
_Bool    dstring_is_empty( const dstring* string );
_Bool    dstring_is_full( const dstring* string );
dstring* dstring_concat( const dstring* left, const dstring* right );
dstring* dstring_concat_substr( substring left, substring right );
dstring* dstring_concat_str( const char* left, const char* right );
dstring* dstring_prepend( dstring* string, const dstring* prepend );
dstring* dstring_prepend_substr( dstring* string, substring prepend );
dstring* dstring_prepend_str( dstring* string, const char* prepend );
dstring* dstring_append( dstring* string, const dstring* append );
dstring* dstring_append_substr( dstring* string, substring append );
dstring* dstring_append_str( dstring* string, const char* append );
dstring* dstring_insert( dstring* string, const dstring* insert, size_t index );
dstring* dstring_insert_substr( dstring* string, substring insert, size_t index );
dstring* dstring_insert_str( dstring* string, const char* insert, size_t index );
dstring* dstring_push( dstring* string, char c );
_Bool    dstring_pop( dstring* string, char* opt_out_c );
_Bool    dstring_remove( dstring* string, size_t index );
dstring* dstring_emplace( dstring* string, char c, size_t index );

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
_Bool  darray_is_empty( const void* darray );
_Bool  darray_is_full( const void* darray );
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
_Bool  darray_push_try( void* darray, const void* item );
void*  darray_pop( void* darray );
_Bool  darray_remove( void* darray, size_t at, void* opt_out_item );
#define darray_empty( type, cap )\
    (type*)darray_empty_generic( sizeof(type), cap )
#define darray_new( type, ... )\
    (type*)darray_new_generic( sizeof(type),\
        static_array_len((type[]){__VA_ARGS__}), (type[]){__VA_ARGS__} )
#define darray_from_array( array )\
    darray_new_generic( sizeof((array)[0]), static_array_len( (array) ), array )

int atomic_add( Atom* ptr, int val );

dpath*    path_pwd(void);
dpath*    path_from_pwd_substr( substring path );
dpath*    path_from_pwd( const char* path );
dpath*    path_canonicalize_substr( substring path );
dpath*    path_canonicalize( const char* path );
dpath*    path_push_chunk_substr( dpath* path, substring chunk );
dpath*    path_push_chunk( dpath* path, const char* chunk );
_Bool     path_peek_chunk( dpath* path, substring* opt_out_chunk );
_Bool     path_pop_chunk( dpath* path, char* opt_out_chunk );
dpath*    path_set_extension_substr( dpath* path, substring ext );
dpath*    path_set_extension( dpath* path, const char* ext );
dpath*    path_convert_separators( const char* path );
dpath*    path_convert_separators_substr( substring path );
char      path_platform_separator(void);
_Bool     path_char_is_separator( char c );
_Bool     path_is_absolute_substr( substring path );
_Bool     path_is_absolute( const char* path );
substring path_get_extension_substr( substring path );
substring path_get_extension( const char* path );
_Bool     path_match_glob_substr( substring path, substring glob );
_Bool     path_match_glob( const char* path, const char* glob );

_Bool program_in_path( const char* prog );
_Bool program_exec( const char* command_line, int* opt_out_res );
_Bool program_exec_async( const char* command_line, ProcessID* out_pid );
_Bool program_wait( ProcessID pid, uint32_t ms, int* opt_out_res );
void  program_terminate( ProcessID pid );
void  program_close( ProcessID pid );

double time_seconds(void);
double time_milliseconds(void);

_Bool file_open( const char* path, FileOpenFlags flags, File* out_file );
_Bool file_write( File file, size_t size, const void* buf );
_Bool file_read( File file, size_t size, void* buf );
void  file_close( File file );
_Bool file_truncate( File file );
_Bool file_query_size( File file, size_t* out_size );
_Bool file_seek( File file, FileSeekType type, ssize_t seek );
_Bool file_query_seek( File file, ssize_t* out_seek );
_Bool file_query_info( File file, FileInfo* out_info );
_Bool file_query_info_path( const char* path, FileInfo* out_info );
_Bool file_move( const char* dst, const char* src );
_Bool file_copy( const char* dst, const char* src );
_Bool file_remove( const char* path );

Compiler query_compiler(void);
Platform query_platform(void);
Arch     query_arch(void);
_Bool    query_64bit(void);
_Bool    compiler_is_mingw(void);
_Bool    platform_is_posix( Platform platform );

const char* cbuild_compiler(void);
const char* cbuild_command_line(void);
const char* cbuild_compile_time(void);

void _init_( const char* argv0, const char* file );
#define init()\
    _init_( argv[0], __FILE__ )

#endif /* Header Guard */

#if defined(CBUILD_IMPLEMENTATION) || defined(_CLANGD)
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

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
            #define COMPILER_CFLAGS " -O0 -g "
        #endif
    #else
        #define COMPILER_CFLAGS " -O2 "
    #endif
    #if !defined(_WIN32)
        #define COMPILER_ADDITIONAL_FLAGS\
            " -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=199309L"
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
        #define COMPILER_ADDITIONAL_FLAGS " -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=199309L"
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
        #define COMPILER_ADDITIONAL_FLAGS " -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=199309L"
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
#define MAX_GLOBAL_STRING_COUNT (4)
static char global_string_buffers[MAX_GLOBAL_STRING_COUNT][MAX_GLOBAL_STRING_SIZE];
Atom global_string_buffer_index = 0;

static const char* global_argv0        = NULL;
static const char* global_compiler     = "";
static const char* global_command_line = "";
static const char* global_compile_time = "";

static LogLevel global_log_level = LOG_LEVEL_INFO;

#if defined(_WIN32)
    #define PATH_SEPARATOR '\\'
    #define PATH_OTHER_SEPARATOR '/'
#else
    #define PATH_SEPARATOR '/'
    #define PATH_OTHER_SEPARATOR '\\'
#endif

// NOTE(alicia): C-stdlib implementation

char path_platform_separator(void) {
    return PATH_SEPARATOR;
}

dpath* path_convert_separators( const char* path ) {
    return path_convert_separators_substr( substr_new( 0, path ) );
}
dpath* path_convert_separators_substr( substring path ) {
    dpath* res = dstring_empty( path.len + 1 );
    if( !res ) {
        return NULL;
    }
    substring rem = path;
    while( rem.len ) {
        size_t unix = 0;
        if( substr_find( rem, PATH_OTHER_SEPARATOR, &unix ) ) {
            substring chunk = rem;
            chunk.len       = unix;
            res = dstring_append_substr( res, chunk );
            res = dstring_push( res, PATH_SEPARATOR );

            rem = substr_advance_by( rem, chunk.len + 1 );
        } else {
            res = dstring_append_substr( res, rem );
            break;
        }
    }
    return res;
}
dpath* path_canonicalize( const char* path ) {
    return path_canonicalize_substr( substr_new( 0, path ) );
}
dpath* path_from_pwd_substr( substring path ) {
    dpath* pwd = path_pwd();
    if( !pwd ) {
        return NULL;
    }
    dpath* res = path_push_chunk_substr( pwd, path );
    if( !res ) {
        dstring_free( pwd );
        return NULL;
    }
    return res;
}
dpath* path_from_pwd( const char* path ) {
    return path_from_pwd_substr( substr_new( 0, path ) );
}
dpath* path_push_chunk( dpath* path, const char* chunk ) {
    return path_push_chunk_substr( path, substr_new( 0, chunk ) );
}
_Bool path_is_absolute( const char* path ) {
    return path_is_absolute_substr( substr_new( 0, path ) );
}
dpath* path_set_extension_substr( dpath* path, substring ext ) {
    if( !ext.len ) {
        return path;
    }
    substring current = path_get_extension_substr( substr_from_dstring( path ) );
    dpath* res = path;
    if( !substr_is_empty( current ) ) {
        dstring_truncate( res, dstring_len( res ) - current.len );
    }

    if( ext.cc[0] != '.' ) {
        res = dstring_push( res, '.' );
        if( !res ) {
            return NULL;
        }
    }
    return dstring_append_substr( res, ext );
}
dpath* path_set_extension( dpath* path, const char* ext ) {
    return path_set_extension_substr( path, substr_new( 0, ext ) );
}
substring path_get_extension_substr( substring path ) {
    substring res = path;
    res.len       = 0;

    size_t dot = 0;
    if( substr_find_last( path, '.', &dot ) ) {
        res.cc  = path.cc  + dot;
        res.len = path.len - dot;

        // is dot file, not extension
        if( dot && path.cc[dot - 1] == '/' ) {
            res.len = 0;
        }
    }

    return res;
}
substring path_get_extension( const char* path ) {
    return path_get_extension_substr( substr_new( 0, path ) );
}
_Bool path_match_glob( const char* path, const char* glob ) {
    return path_match_glob_substr( substr_new( 0, path ), substr_new( 0, glob ) );
}
_Bool path_match_glob_substr( substring string, substring pattern ) {
    substring substr = string;
    substring subpat = pattern;
#if defined(_WIN32)
    dpath* win32_str  = path_convert_separators_substr( string );
    dpath* win32_glob = path_convert_separators_substr( pattern );
    substr = substr_from_dstring( win32_str );
    subpat = substr_from_dstring( win32_glob );
#endif

    _Bool res = true;
    #define end( _res )\
        res = _res;\
        goto path_match_glob_end

    const substring wildcards = substr_text( "?*" );

    while( substr.len && subpat.len ) {
        size_t wildcard_pos = 0;
        if( substr_find_set( subpat, wildcards, &wildcard_pos ) ) {
            if( wildcard_pos ) {
                substring to_wildcard = subpat;
                to_wildcard.len = wildcard_pos;
                if( substr.len < to_wildcard.len ) {
                    end( false );
                }
                substring substr_to_wildcard = substr;
                substr_to_wildcard.len = wildcard_pos;
                if( !substr_cmp( substr_to_wildcard, to_wildcard ) ) {
                    end( false );
                }
                substr = substr_advance_by( substr, wildcard_pos );
            }
            char wildcard = subpat.cc[wildcard_pos];

            subpat = substr_advance_by( subpat, wildcard_pos + 1 );

            switch( wildcard ) {
                case '*': {
                    if( !subpat.len ) {
                        // pattern ends with * so anything is valid
                        end( true );
                    }
                    size_t next       = subpat.len;
                    _Bool  next_found = 
                        substr_find_set( subpat, wildcards, &next );
                    if( substr.len < next ) {
                        end( false );
                    }
                    substring next_subpat = subpat;
                    next_subpat.len       = next;

                    if( next_subpat.len ) {
                        size_t advance = 0;
                        if( !substr_find_substr( substr, next_subpat, &advance ) ) {
                            end( false );
                        }
                        substr = substr_advance_by( substr, advance + next );
                        subpat = substr_advance_by( subpat, next );
                    } else {
                        if( !next_found ) {
                            end( true );
                        }
                        // subpat = substr_advance( subpat );
                    }
                } break;
                case '?': {
                    if( !substr.len ) {
                        end( false );
                    }
                    substr = substr_advance( substr );
                } break;
                default: break;
            }

        } else {
            end( substr_cmp( substr, subpat ) );
        }
    }

path_match_glob_end:
#if defined(_WIN32)
    dstring_free( win32_str );
    dstring_free( win32_glob );
#endif
    #undef end
    return res;
}

const char* cbuild_compiler(void) {
    return global_compiler;
}
const char* cbuild_command_line(void) {
    return global_command_line;
}
const char* cbuild_compile_time(void) {
    return global_compile_time;
}
Compiler query_compiler(void) {
#if defined(__clang__)
    return COMPILER_CLANG;
#elif defined(__GNUC__)
    return COMPILER_GCC;
#elif defined(_MSC_VER)
    return COMPILER_MSVC;
#else
    return COMPILER_UNKNOWN;
#endif
}
Platform query_platform(void) {
#if defined(__linux__)
    return PLATFORM_LINUX;
#elif defined(_WIN32)
    return PLATFORM_WINDOWS;
#elif defined(__APPLE__)
    return PLATFORM_MACOS;
#else
    return PLATFORM_UNKNOWN;
#endif
}
_Bool compiler_is_mingw(void) {
#if defined(__MINGW32__)
    return true;
#else
    return false;
#endif
}
Arch  query_arch(void) {
#if defined(__arm__) || defined(_M_ARM) || defined( __aarch64__ ) || defined( _M_ARM64 )
    return ARCH_ARM;
#elif defined(__i386__) || defined(_M_IX86) || defined(__X86__) || defined(__x86_64__)
    return ARCH_X86;
#else
    return ARCH_UNKNOWN;
#endif
}
_Bool query_64bit(void) {
#if UINT64_MAX == UINTPTR_MAX
    return true;
#else
    return false;
#endif
}
_Bool platform_is_posix( Platform platform ) {
    switch( platform ) {
        case PLATFORM_LINUX:
        case PLATFORM_MACOS: return true;
        default: return false;
    }
}

const char* local_fmt( const char* format, ... ) {
    int index = atomic_add( &global_string_buffer_index, 1 );
    index %= MAX_GLOBAL_STRING_COUNT;
    char* buf = global_string_buffers[index];
    memory_zero( buf, MAX_GLOBAL_STRING_SIZE );

    va_list va;
    va_start( va, format );
    
    vsnprintf( buf, MAX_GLOBAL_STRING_SIZE, format, va );

    va_end( va );

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

    memory_free( swap );
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
        free( darray_head( darray ) );
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
_Bool darray_is_empty( const void* darray ) {
    darray_header* head = darray_head( (void*)darray );
    return head->len != 0;
}
_Bool darray_is_full( const void* darray ) {
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
    size_t init_cap = (head->len / 2) ? (head->len / 2) : 2;
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
_Bool darray_push_try( void* darray, const void* item ) {
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
_Bool darray_remove( void* darray, size_t at, void* opt_out_item ) {
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

_Bool substr_is_empty( substring substr ) {
    return substr.len == 0;
}
_Bool substr_cmp( substring a, substring b ) {
    if( a.len != b.len ) {
        return false;
    }
    return memcmp( a.cc, b.cc, a.len ) == 0;
}
_Bool substr_find( substring substr, char c, size_t* out_pos ) {
    for( size_t i = 0; i < substr.len; ++i ) {
        if( substr.cc[i] == c ) {
            *out_pos = i;
            return true;
        }
    }
    return false;
}
_Bool substr_find_last( substring substr, char c, size_t* out_pos ) {
    for( size_t i = substr.len; i-- > 0; ) {
        if( substr.cc[i] == c ) {
            *out_pos = i;
            return true;
        }
    }
    return false;
}
_Bool substr_find_set( substring substr, substring set, size_t* out_pos ) {
    for( size_t i = 0; i < substr.len; ++i ) {
        for( size_t j = 0; j < set.len; ++j ) {
            if( substr.cc[i] == set.cc[j] ) {
                *out_pos = i;
                return true;
            }
        }
    }
    return false;
}
_Bool substr_find_last_set( substring substr, substring set, size_t* out_pos ) {
    for( size_t i = substr.len; i-- > 0; ) {
        for( size_t j = 0; j < set.len; ++j ) {
            if( substr.cc[i] == set.cc[j] ) {
                *out_pos = i;
                return true;
            }
        }
    }
    return false;
}
_Bool substr_find_substr( substring source, substring substr, size_t* out_pos ) {
    substring rem = source;
    while( rem.len ) {
        size_t start = 0;
        if( substr_find( rem, substr.cc[0], &start ) ) {
            rem = substr_advance_by( rem, start );
            if( rem.len < substr.len ) {
                return false;
            }
            substring cmp = rem;
            cmp.len = substr.len;
            if( substr_cmp( cmp, substr ) ) {
                *out_pos = rem.cc - source.cc;
                return true;
            }
            rem = substr_advance( rem );
        } else {
            return false;
        }
    }
    return false;
}
substring substr_advance( substring substr ) {
    substring res = substr;
    if( !res.len ) {
        return res;
    }
    res.cc++;
    res.len--;
    return res;
}
substring substr_advance_by( substring substr, size_t amount ) {
    substring res = substr;
    if( amount > res.len ) {
        res.len = 0;
        return res;
    }
    res.cc  += amount;
    res.len -= amount;
    return res;
}
size_t substr_len_utf8( substring substr ) {
    const unsigned char* ucc = (const unsigned char*)substr.cc;
    size_t res = 0;
    for( size_t i = 0; i < substr.len; ++i ) {
        if( (ucc[i] & 0xC0) != 0x80 ) {
            res++;
        }
    }
    return res;
}

size_t string_len( const char* string ) {
    return strlen( string );
}
size_t string_len_utf8( const char* string ) {
    return substr_len_utf8( substr_new( 0, string ) );
}
_Bool string_cmp( const char* a, const char* b ) {
    return strcmp( a, b ) == 0;
}
_Bool string_find( const char* string, char c, size_t* out_pos ) {
    char* res = strchr( string, c );
    if( !res ) {
        return false;
    }

    *out_pos = res - string;
    return true;
}
_Bool string_find_last( const char* string, char c, size_t* out_pos ) {
    char* res = strrchr( string, c );
    if( !res ) {
        return false;
    }
    *out_pos = res - string;
    return true;
}
_Bool string_find_set( const char* string, const char* set, size_t* out_pos ) {
    char* res = strpbrk( string, set );
    if( !res ) {
        return false;
    }
    *out_pos = res - string;
    return true;
}
_Bool string_find_string(
    const char* string, const char* substr, size_t* out_pos
) {
    char* res = strstr( string, substr );
    if( !res ) {
        return false;
    }
    *out_pos = res - string;
    return true;
}

dstring* dstring_empty( size_t size ) {
    dstring_header* res = memory_alloc( sizeof(*res) + size + 1 );
    if( !res ) {
        return NULL;
    }
    res->cap = size + 1;
    return res->buf;
}
dstring* dstring_new( size_t opt_len, const char* source ) {
    size_t len = opt_len;
    if( !len ) {
        len = string_len( source );
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
dstring* dstring_fmt( const char* format, ... ) {
    va_list len, fmt;
    va_start( len, format );
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
dstring* dstring_clone( const dstring* source ) {
    dstring_header* head = dstring_head( (dstring*)source );
    dstring_header* res  = memory_alloc( sizeof(*res) + head->len + 16 );
    if( !res ) {
        return NULL;
    }
    res->cap = head->len + 16;
    res->len = head->len;
    memory_copy( res->buf, source, head->len );
    return res->buf;
}
void dstring_free( dstring* string ) {
    free( dstring_head( string ) );
}
void* dstring_head( dstring* string ) {
    return string ? string - sizeof(dstring_header) : 0;
}
dstring* dstring_grow( dstring* string, size_t amount ) {
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
void dstring_clear( dstring* string ) {
    dstring_header* head = dstring_head( string );
    memory_zero( head->buf, head->len );
    head->len = 0;
}
void dstring_truncate( dstring* string, size_t max_len ) {
    dstring_header* head = dstring_head( string );
    size_t len  = max_len > head->len ? head->len : max_len;
    size_t zero = head->len - len;

    if( zero ) {
        memory_zero( string + len, zero );
    }
    head->len = len;
}
size_t dstring_len( const dstring* string ) {
    dstring_header* head = dstring_head( (dstring*)string );
    return head->len;
}
size_t dstring_cap( const dstring* string ) {
    dstring_header* head = dstring_head( (dstring*)string );
    return head->cap;
}
_Bool dstring_is_empty( const dstring* string ) {
    dstring_header* head = dstring_head( (dstring*)string );
    return head->len == 0;
}
_Bool dstring_is_full( const dstring* string ) {
    dstring_header* head = dstring_head( (dstring*)string );
    return head->len >= head->cap;
}
dstring* dstring_concat( const dstring* left, const dstring* right ) {
    dstring_header* hleft, *hright;
    hleft  = dstring_head( (dstring*)left );
    hright = dstring_head( (dstring*)right );

    dstring_header* res = dstring_head( dstring_empty( hleft->len + hright->len ) );
    if( !res ) {
        return NULL;
    }

    memory_copy( res->buf, left, hleft->len );
    memory_copy( res->buf + hleft->len, right, hright->len );
    res->len += hleft->len + hright->len;

    return res->buf;
}
dstring* dstring_concat_substr( substring left, substring right ) {
    dstring_header* res = dstring_head( dstring_empty( left.len + right.len ) );
    if( !res ) {
        return NULL;
    }

    memory_copy( res->buf, left.cc, left.len );
    memory_copy( res->buf + left.len, right.cc, right.len );
    res->len += left.len + right.len;

    return res->buf;
}
dstring* dstring_concat_str( const char* left, const char* right ) {
    substring _left  = substr_new( 0, left );
    substring _right = substr_new( 0, right );
    return dstring_concat_substr( _left, _right );
}
dstring* dstring_prepend( dstring* string, const dstring* prepend ) {
    dstring_header* head     = dstring_head( string );
    dstring_header* hprepend = dstring_head( (dstring*)prepend );

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
dstring* dstring_prepend_substr( dstring* string, substring prepend ) {
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
dstring* dstring_prepend_str( dstring* string, const char* prepend ) {
    return dstring_prepend_substr( string, substr_new( 0, prepend ) );
}
dstring* dstring_append( dstring* string, const dstring* append ) {
    dstring_header* head    = dstring_head( string );
    dstring_header* happend = dstring_head( (dstring*)append );

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
dstring* dstring_append_substr( dstring* string, substring append ) {
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
dstring* dstring_append_str( dstring* string, const char* append ) {
    return dstring_append_substr( string, substr_new( 0, append ) );
}
dstring* dstring_insert( dstring* string, const dstring* insert, size_t index ) {
    dstring_header* head    = dstring_head( string );

    size_t i = index;
    if( i >= head->len ) {
        i = head->len ? head->len - 1 : 0;
    }

    dstring_header* hinsert = dstring_head( (dstring*)insert );

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
dstring* dstring_insert_substr( dstring* string, substring insert, size_t index ) {
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
dstring* dstring_insert_str( dstring* string, const char* insert, size_t index ) {
    return dstring_insert_substr( string, substr_new( 0, insert ), index );
}
dstring* dstring_push( dstring* string, char c ) {
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
_Bool dstring_pop( dstring* string, char* opt_out_c ) {
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
_Bool dstring_remove( dstring* string, size_t index ) {
    dstring_header* head = dstring_head( string );
    if( index >= head->len ) {
        return false;
    }
    memory_move( head->buf + index, head->buf + index + 1, head->cap - (index + 1) );
    head->len--;
    return true;
}
dstring* dstring_emplace( dstring* string, char c, size_t index ) {
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

void* memory_alloc( size_t size ) {
    void* res = malloc( size );
    if( !res ) {
        return NULL;
    }
    memset( res, 0, size );
    return res;
}
void* memory_realloc( void* memory, size_t old_size, size_t new_size ) {
    char* res = realloc( memory, new_size );
    if( !res ) {
        return NULL;
    }
    memset( res + old_size, 0, new_size - old_size );
    return res;
}
void memory_free( void* memory ) {
    free( memory );
}
void memory_set( void* memory, char value, size_t size ) {
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
void logger_level_set( LogLevel level ) {
    global_log_level = level;
}
LogLevel logger_level_get(void) {
    return global_log_level;
}
void logger( LogLevel level, const char* format, ... ) {
    if( level < global_log_level ) {
        return;
    }
    switch( level ) {
        case LOG_LEVEL_INFO: {
            printf( "[I] cbuild: " );

            va_list va;
            va_start( va, format );

            vprintf( format, va );

            va_end( va );

            putc( '\n', stdout );
        } break;
        case LOG_LEVEL_WARN: {
            printf( "\033[1;33m" "[W] cbuild: " );

            va_list va;
            va_start( va, format );

            vprintf( format, va );

            va_end( va );

            printf( "\033[1;00m" "\n" );
        } break;
        case LOG_LEVEL_ERROR: {
            fprintf( stderr, "\033[1;31m" "[E] cbuild: " );

            va_list va;
            va_start( va, format );

            vfprintf( stderr, format, va );

            va_end( va );

            fprintf( stderr, "\033[1;00m" "\n" );
        } break;
        default: break;
    }
}
_Bool program_exec( const char* command_line, int* opt_out_res ) {
    ProcessID pid;
    if( !program_exec_async( command_line, &pid ) ) {
        return false;
    }
    program_wait( pid, UINT32_MAX, opt_out_res );
    program_close( pid );

    return true;
}

void platform_init(void);
void _init_( const char* argv0, const char* file ) {
    global_argv0        = argv0;
    global_compiler     = COMPILER;
    global_command_line = dstring_fmt(
        COMPILER COMPILER_BUILD_LINE COMPILER_CFLAGS
        COMPILER_ADDITIONAL_FLAGS CBUILD_COMPILE_ARGS,
        file, argv0 );
    global_compile_time = __TIMESTAMP__;
    platform_init();

    double start = time_milliseconds();

    struct stat exe_fs, cbuild_fs, main_fs;

    stat( argv0, &exe_fs );
    stat( __FILE__, &cbuild_fs );
    stat( file, &main_fs );

    double diff_main   = difftime( exe_fs.st_ctime, main_fs.st_ctime );
    double diff_cbuild = difftime( exe_fs.st_ctime, cbuild_fs.st_ctime );

    dstring* old_name = dstring_concat_str( argv0, ".old" );

    if( diff_main > 0.0 && diff_cbuild > 0.0 ) {
        FILE* file_old = fopen( old_name, "r" );
        if( file_old ) {
            fclose( file_old );
            cb_info( "removing %s . . . ", old_name );
            remove( old_name );
        }
        dstring_free( old_name );
        return;
    }

    cb_info( "changes in cbuild files detected, recompiling . . ." );
    if( !program_in_path( COMPILER ) ) {
        cb_error( "failed to find compiler '" COMPILER "' in path!" );
#if defined(_WIN32)
        if( substr_cmp( substr_text( "cl" ), substr_text( COMPILER ) ) ) {
            cb_error( "maybe you need to run vcvarsall.bat or run cbuild in Developer Command Prompt!" );
        }
#endif
        exit(-1);
    }

    cb_info( "renaming self to %s . . . ", old_name );
    if( !file_move( old_name, argv0 ) ) {
        cb_warn( "failed to rename to %s!", old_name );
        cb_warn( "attempting to remove %s . . .", old_name );
        remove( old_name );
        if( !file_move( old_name, argv0 ) ) {
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
        cb_error( "failed to rebuild, renaming self back" );
        file_move( argv0, old_name );
        exit(-1);
    }
}

// NOTE(alicia): platform-specific implementation
// SWIN32

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

static LARGE_INTEGER global_qpf;
static const substring global_separators = substr_text( "/\\" );

#define WIN32_TICKS_PER_SECOND (10000000)
#define WIN32_TO_POSIX_DIFF    (11644473600ULL)
static time_t win32_filetime_to_posix_time( FILETIME ft ) {
    ULARGE_INTEGER uli;
    uli.LowPart  = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    return (time_t)(
        (uli.QuadPart / WIN32_TICKS_PER_SECOND) - WIN32_TO_POSIX_DIFF );
}
static wchar_t* win32_utf8_to_wide( substring utf8 ) {
    wchar_t* buf = memory_alloc( sizeof(wchar_t) * (utf8.len + 1) );
    if( !buf ) {
        cb_error( "win32: failed to allocate UTF-8 to Windows Unicode buffer!" );
        return NULL;
    }

    #define MAX_SIZE INT32_MAX

    size_t rem = utf8.len;
    while( rem ) {
        size_t convert_size = rem;
        if( convert_size > MAX_SIZE ) {
            convert_size = MAX_SIZE;
        }
        int iconvert_size = (int)convert_size;

        MultiByteToWideChar(
            CP_UTF8, 0, utf8.cc + (utf8.len - rem),
            iconvert_size, buf + (utf8.len - rem), iconvert_size );

        rem -= convert_size;
    }
    buf[utf8.len] = 0;

    #undef MAX_SIZE
    return buf;
}
static wchar_t* win32_path_canonicalize_substr( substring path ) {
    dpath* canon = path_canonicalize_substr( path );
    if( !canon ) {
        return NULL;
    }
    wchar_t* buf = win32_utf8_to_wide( substr_from_dstring( canon ) );
    dstring_free( canon );

    if( !buf ) {
        return NULL;
    }
    return buf;
}
static wchar_t* win32_path_wide( substring path ) {
    if( path.len >= MAX_PATH ) {
        return win32_path_canonicalize_substr( path );
    } else {
        return win32_utf8_to_wide( path );
    }
}
// TODO(alicia): rewrite for windows unicode to avoid allocating utf8 buffer
// just to allocate another wide char buffer after it.
dpath* path_canonicalize_substr( substring path ) {
    if( !path.len ) {
        cb_error( "path_canonicalize: provided path is empty! returning null . . ." );
        return NULL;
    }
    size_t size       = sizeof("\\\\?\\");
    dpath* res        = NULL;
    size_t    pwd_len = 0;
    substring homedrive;
    substring home;
    memory_zero( &home, sizeof(home) );
    memory_zero( &homedrive, sizeof(homedrive) );

    #define TYPE_RELATIVE 0 
    #define TYPE_ABSOLUTE 1
    #define TYPE_HOME     2
    int type = TYPE_RELATIVE;

    if( path.cc[0] == '~' ) {
        type = TYPE_HOME;
        home.cc      = getenv( "HOMEPATH" );
        homedrive.cc = getenv( "HOMEDRIVE" );

        if( !home.cc ) {
            cb_error( "path_canonicalize: failed to get 'HOMEPATH' env var!" );
            return NULL;
        }
        if( !homedrive.cc ) {
            cb_error( "path_canonicalize: failed to get 'HOMEDRIVE' env var!" );
            return NULL;
        }
        home.len      = string_len( home.cc );
        homedrive.len = string_len( homedrive.cc );
    } else {
        if( path_is_absolute_substr( path ) ) {
            type = TYPE_ABSOLUTE;
        } else {
            type = TYPE_RELATIVE;
        }
    }

    switch( type ) {
        case TYPE_RELATIVE: {
            pwd_len = GetCurrentDirectoryA(0, 0) - 1; // result includes null-terminator
            size   += path.len + pwd_len;
        } break;
        case TYPE_ABSOLUTE: {
            size += path.len;
        } break;
        case TYPE_HOME: {
            size += path.len + homedrive.len + home.len;
        } break;
    }

    res = dstring_empty( size + 16 );
    if( !res ) {
        return NULL;
    }
    res = dstring_append_substr( res, substr_text( "\\\\?\\" ) );

    substring subpath = path;
    switch( type ) {
        case TYPE_RELATIVE: {
            dstring_header* head = dstring_head( res );
            GetCurrentDirectoryA( size, res + head->len );
            head->len           += pwd_len;
        } break;
        case TYPE_HOME: {
            res = dstring_append_substr( res, homedrive );
            res = dstring_append_substr( res, home );
            subpath = substr_advance( subpath );
        } break;
        case TYPE_ABSOLUTE: {
            res     = dstring_push( res, subpath.cc[0] );
            subpath = substr_advance_by( subpath, 2 );
            res     = dstring_push( res, ':' );
        } break;
    }

    while( subpath.len ) {
        size_t sep = 0;
        if( substr_find_set( subpath, global_separators, &sep ) ) {
            if( !sep ) {
                subpath = substr_advance( subpath );
                continue;
            }

            substring chunk = subpath;
            chunk.len       = sep;

            _Bool should_push = true;
            if( chunk.len < 3 ) {
                if( substr_cmp( chunk, substr_text( "." ) ) ) {
                    should_push = false;
                } else if( substr_cmp( chunk, substr_text( ".." ) ) ) {
                    should_push = false;
                    path_pop_chunk( res, NULL );
                }
            }

            if( should_push ) {
                res = path_push_chunk_substr( res, chunk );
            }

            subpath = substr_advance_by( subpath, chunk.len + 1 );
        } else {
            _Bool should_push = true;
            if( subpath.len < 3 ) {
                if( substr_cmp( subpath, substr_text( "." ) ) ) {
                    should_push = false;
                } else if( substr_cmp( subpath, substr_text( ".." ) ) ) {
                    should_push = false;
                    path_pop_chunk( res, NULL );
                }
            }

            if( should_push ) {
                res = path_push_chunk_substr( res, subpath );
            }
            break;
        }
    }

    if( path_char_is_separator( path.cc[path.len - 1] ) ) {
        res = dstring_push( res, PATH_SEPARATOR );
    }

    #undef TYPE_RELATIVE 
    #undef TYPE_ABSOLUTE
    #undef TYPE_HOME    
    return res;
}
_Bool path_peek_chunk( dpath* path, substring* opt_out_chunk ) {
    if( dstring_is_empty( path ) ) {
        return false;
    }

    substring subpath = substr_from_dstring( path );
    size_t sep = 0;
    if( path_is_absolute_substr( subpath ) ) {
        if( substr_find_last_set( subpath, global_separators, &sep ) ) {
            // separator position can't be zero if path is absolute on win32
            // can't remove separator from drive letter ie: C:/ <- cannot be removed
            if( sep + 1 >= subpath.len && path[sep - 1] == ':' ) {
                return false;
            }

            goto win32_path_peek_chunk_end;
        } else {
            // path is malformed
            cb_warn(
                "path_peek_chunk: attempted to peek chunk "
                "from malformed path: '%s'", path );
            return false;
        }
    } else {
        if( substr_find_last_set( subpath, global_separators, &sep ) ) {
win32_path_peek_chunk_end:
            subpath = substr_advance_by( subpath, sep + 1 );
            if( !subpath.len ) {
                return false;
            }

            if( opt_out_chunk ) {
                *opt_out_chunk = subpath;
            }
            return true;
        } else {
            if( opt_out_chunk ) {
                *opt_out_chunk = subpath;
            }
            return true;
        }

    }
}
_Bool path_pop_chunk( dpath* path, char* opt_out_chunk ) {
    if( dstring_is_empty( path ) ) {
        return false;
    }
    substring subpath = substr_from_dstring( path );
    size_t sep = 0;
    _Bool  is_absolute_last = false;
    if( path_is_absolute_substr( subpath ) ) {
        if( substr_find_last_set( subpath, global_separators, &sep ) ) {
            // separator position can't be zero if path is absolute on win32
            // can't remove separator from drive letter ie: C:/ <- cannot be removed
            is_absolute_last = path[sep - 1] == ':';
            if( sep + 1 >= subpath.len && is_absolute_last ) {
                return false;
            }

            goto win32_path_pop_chunk_end;
        } else {
            // path is malformed
            cb_warn(
                "path_pop_chunk: attempted to pop chunk "
                "from malformed path: '%s'", path );
            return false;
        }
    } else {
        if( substr_find_last_set( subpath, global_separators, &sep ) ) {
win32_path_pop_chunk_end:
            subpath = substr_advance_by( subpath, sep + 1 );
            if( !subpath.len ) {
                dstring_pop( path, NULL );
                return false;
            }

            if( opt_out_chunk ) {
                memory_copy( opt_out_chunk, subpath.cc, subpath.len );
                opt_out_chunk[subpath.len] = 0;
            }

            if( is_absolute_last ) {
                sep++;
            }
            dstring_truncate( path, sep );
            return true;
        } else {
            if( opt_out_chunk ) {
                memory_copy( opt_out_chunk, subpath.cc, subpath.len );
                opt_out_chunk[subpath.len] = 0;
            }
            dstring_clear( path );
            return true;
        }
    }
}
_Bool path_char_is_separator( char c ) {
    return c == '/' || c == '\\';
}
_Bool path_is_absolute_substr( substring path ) {
    if( path.len < 3 ) {
        return false;
    }
    if( isalpha( path.cc[0] ) ) {
        return
            path.cc[1] == ':' &&
            path_char_is_separator( path.cc[2] );
    } else {
        if( path.len < sizeof("\\\\?\\A:") ) {
            return false;
        }
        // TODO(alicia): check for '\\*'?
        
        return
            isalpha( path.cc[4] ) &&
            path.cc[5] == ':' &&
            path_char_is_separator( path.cc[6] );
    }
}
static _Bool path_starts_with_drive_letter( substring path ) {
    if( path.len < 2 ) {
        return false;
    }
    return
        isalpha( path.cc[0] ) &&
        path.cc[1] == ':';
}
dpath* path_push_chunk_substr( dpath* path, substring chunk ) {
    dstring_header* head = dstring_head( path );
    if( head->len + chunk.len + 1 > head->cap ) {
        head = dstring_head( dstring_grow( head->buf, chunk.len + 1 ) );
        if( !head ) {
            return NULL;
        }
    }

    if( path_starts_with_drive_letter( chunk ) ) {
        substring drive = chunk;
        drive.len       = 2;
        head            = dstring_head( dstring_append_substr( head->buf, drive ) );
        chunk           = substr_advance_by( chunk, drive.len );
    }

    while( chunk.len ) {
        size_t sep = 0;
        if( substr_find_set( chunk, global_separators, &sep ) ) {
            if( !sep ) {
                chunk = substr_advance( chunk );
                continue;
            }
            substring subchunk = chunk;
            subchunk.len       = sep;

            if( !head->len || head->buf[head->len - 1] != PATH_SEPARATOR ) {
                head = dstring_head( dstring_push( head->buf, PATH_SEPARATOR ) );
            }
            head  = dstring_head( dstring_append_substr( head->buf, subchunk ) );
            chunk = substr_advance_by( chunk, subchunk.len + 1 );
        } else {
            if( !head->len || head->buf[head->len - 1] != PATH_SEPARATOR ) {
                head = dstring_head( dstring_push( head->buf, PATH_SEPARATOR ) );
            }
            head = dstring_head( dstring_append_substr( head->buf, chunk ) );
            break;
        }
    }

    return head->buf;
}
_Bool file_open( const char* in_path, FileOpenFlags flags, File* out_file ) {
    DWORD  dwDesiredAccess       = 0;
    DWORD  dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD  dwCreationDisposition = OPEN_EXISTING;
    DWORD  dwFlagsAndAttributes  = 0;

    if( flags & FILE_OPEN_FLAG_READ ) {
        dwDesiredAccess |= GENERIC_READ;
    }
    if( flags & FILE_OPEN_FLAG_WRITE ) {
        dwDesiredAccess |= GENERIC_WRITE;
    }

    if( !(dwDesiredAccess & (GENERIC_READ | GENERIC_WRITE)) ) {
        cb_error( "file_open: invalid flags! path: %s", in_path );
        return false;
    }

    if( flags & FILE_OPEN_FLAG_CREATE ) {
        dwCreationDisposition |= CREATE_ALWAYS;
    } else if( flags & FILE_OPEN_FLAG_TRUNCATE ) {
        dwCreationDisposition |= TRUNCATE_EXISTING;
    }

    wchar_t* path = win32_path_wide( substr_new( 0, in_path ) );
    if( !path ) {
        cb_error( "file_open: failed to convert path to Windows Unicode!" );
        return false;
    }

    HANDLE handle = CreateFileW(
        path, dwDesiredAccess, dwShareMode,
        NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL );
    free( path );

    if( handle == INVALID_HANDLE_VALUE ) {
        cb_error( "file_open: failed to open '%s'!", in_path );
        return false;
    }

    *out_file = handle;

    if( flags & FILE_OPEN_FLAG_APPEND ) {
        if( !file_seek( handle, FILE_SEEK_TYPE_END, 0 ) ) {
            cb_warn(
                "file_open: failed to seek to end of file! path: '%s'", in_path );
        }
    }
    return true;
}
_Bool file_write( File file, size_t size, const void* buf ) {
#if UINTPTR_MAX == UINT64_MAX
    DWORD bytes_written = 0;
    DWORD part0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD part1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    if( part0 ) {
        BOOL res = WriteFile( file, buf, part0, &bytes_written, NULL );
        if( !res ) {
            return false;
        }
    }
    if( part1 ) {
        BOOL res = WriteFile(
            file, (char*)buf + part0, part1, &bytes_written, NULL );
        if( !res ) {
            return false;
        }
    }
    return true;
#else
    DWORD bytes_written = 0;
    return WriteFile( file, buf, size, &bytes_written, NULL ) == TRUE;
#endif
}
_Bool file_read( File file, size_t size, void* buf ) {
#if UINTPTR_MAX == UINT64_MAX
    DWORD bytes_read = 0;
    DWORD part0 = size > UINT32_MAX ? UINT32_MAX : size;
    DWORD part1 = size > UINT32_MAX ? size - UINT32_MAX : 0;

    if( part0 ) {
        BOOL res = ReadFile( file, buf, part0, &bytes_read, NULL );
        if( !res ) {
            return false;
        }
    }
    if( part1 ) {
        BOOL res = ReadFile(
            file, (char*)buf + part0, part1, &bytes_read, NULL );
        if( !res ) {
            return false;
        }
    }
    return true;
#else
    DWORD bytes_read = 0;
    return ReadFile( file, buf, size, &bytes_read, NULL ) == TRUE;
#endif

}
void  file_close( File file ) {
    CloseHandle( file );
}
_Bool file_truncate( File file ) {
    return SetEndOfFile( file ) == TRUE;
}
_Bool file_query_size( File file, size_t* out_size ) {
    LARGE_INTEGER li;
    memory_zero( &li, sizeof(li) );
    if( GetFileSizeEx( file, &li ) == FALSE ) {
        cb_error( "file_query_size: failed to get file size!" );
        return false;
    }

    *out_size = li.QuadPart;
    return true;
}
_Bool file_seek( File file, FileSeekType type, ssize_t seek ) {
    LARGE_INTEGER li;
    li.QuadPart = seek;
    DWORD dwMoveMethod = 0;
    switch( seek ) {
        case FILE_SEEK_TYPE_START: {
            dwMoveMethod = FILE_BEGIN;
        } break;
        case FILE_SEEK_TYPE_END: {
            dwMoveMethod = FILE_END;
        } break;
        default: {
            dwMoveMethod = FILE_CURRENT;
        } break;
    }
    return SetFilePointerEx( file, li, NULL, dwMoveMethod ) != FALSE;
}
_Bool file_query_seek( File file, ssize_t* out_seek ) {
    LARGE_INTEGER move;
    move.QuadPart = 0;
    LARGE_INTEGER li;
    if( SetFilePointerEx( file, move, &li, FILE_CURRENT ) == FALSE ) {
        cb_error( "file_query_seek: failed to query file pointer!" );
        return false;
    }
    *out_seek = li.QuadPart;
    return true;
}
_Bool file_query_info( File file, FileInfo* out_info ) {
    FileInfo info;
    memory_zero( &info, sizeof(info) );

    BY_HANDLE_FILE_INFORMATION fi;
    memory_zero( &fi, sizeof(fi) );
    if( !GetFileInformationByHandle( file, &fi ) ) {
        cb_error( "file_query_info: failed to query file info!" );
        return false;
    }

    ULARGE_INTEGER sz;
    sz.LowPart     = fi.nFileSizeLow;
    sz.HighPart    = fi.nFileSizeHigh;
    info.file_size = sz.QuadPart;

    info.time_create = win32_filetime_to_posix_time( fi.ftCreationTime );
    info.time_modify = win32_filetime_to_posix_time( fi.ftLastWriteTime );

    if( fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
        info.type = FILE_TYPE_DIRECTORY;
    } else {
        info.type = FILE_TYPE_FILE;
    }

    return true;
}
_Bool file_query_info_path( const char* in_path, FileInfo* out_info ) {
    wchar_t* path = win32_path_wide( substr_new( 0, in_path ) );
    if( !path ) {
        cb_error( "file_query_info_path: failed to convert path to Windows Unicode!" );
        return false;
    }

    HANDLE handle = CreateFileW(
        path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, 0 );
    free( path );

    if( handle == INVALID_HANDLE_VALUE ) {
        cb_error( "file_query_info_path: failed to open path '%s'", in_path );
        return false;
    }

    _Bool query_result = file_query_info( handle, out_info );
    CloseHandle( handle );

    return query_result;
}
_Bool file_move( const char* in_dst, const char* in_src ) {
    wchar_t* dst = win32_path_wide( substr_new( 0, in_dst ) );
    if( !dst ) {
        cb_error( "file_move: failed to convert path to Windows Unicode!" );
        return false;
    }
    wchar_t* src = win32_path_wide( substr_new( 0, in_src ) );
    if( !src ) {
        cb_error( "file_move: failed to convert path to Windows Unicode!" );
        free( dst );
        return false;
    }

    _Bool res = MoveFileW( src, dst ) != FALSE;
    free( src );
    free( dst );

    return res;
    // return MoveFileA( src, dst ) == TRUE;
}
_Bool file_copy( const char* in_dst, const char* in_src ) {
    wchar_t* dst = win32_path_wide( substr_new( 0, in_dst ) );
    if( !dst ) {
        cb_error( "file_copy: failed to convert path to Windows Unicode!" );
        return false;
    }
    wchar_t* src = win32_path_wide( substr_new( 0, in_src ) );
    if( !src ) {
        cb_error( "file_copy: failed to convert path to Windows Unicode!" );
        free( dst );
        return false;
    }

    _Bool res = CopyFileW( src, dst, FALSE ) != FALSE;
    free( src );
    free( dst );

    return res;
}
_Bool file_remove( const char* in_path ) {
    wchar_t* path = win32_path_wide( substr_new( 0, in_path ) );
    if( !path ) {
        cb_error( "file_remove: failed to convert path to Windows Unicode!" );
        return false;
    }

    _Bool res = DeleteFileW( path ) == TRUE;
    free( path );

    return res;
}

void platform_init(void) {
    QueryPerformanceFrequency( &global_qpf );
}
int atomic_add( Atom* ptr, int val ) {
    return _InterlockedExchangeAdd( (volatile long*)ptr, val );
}
dstring* path_pwd(void) {
    dstring_header* res = dstring_head( dstring_empty( MAX_PATH ) );
    if( !res ) {
        return NULL;
    }

    DWORD len = GetCurrentDirectoryA( res->cap, res->buf );
    if( !len ) {
        free( res );
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
_Bool program_in_path( const char* prog ) {
    DWORD dwCreationFlags = CREATE_SUSPENDED;
    STARTUPINFOA startup;
    memory_zero( &startup, sizeof(startup) );
    startup.cb = sizeof(startup);

    PROCESS_INFORMATION info;
    memory_zero( &info, sizeof(info) );

    _Bool res = CreateProcessA(
        NULL, (char*)prog, NULL, NULL, FALSE,
        dwCreationFlags, NULL, NULL, &startup, &info );

    if( res ) {
        TerminateProcess( info.hProcess, 0 );
        CloseHandle( info.hProcess );
        CloseHandle( info.hThread );
    }

    return res;
}
_Bool program_exec_async( const char* command_line, ProcessID* out_pid ) {
    STARTUPINFOA        startup;
    PROCESS_INFORMATION info;

    memory_zero( &startup, sizeof(startup) );
    memory_zero( &info, sizeof(info) );

    startup.cb         = sizeof(startup);
    startup.hStdInput  = GetStdHandle( STD_INPUT_HANDLE );
    startup.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    startup.hStdError  = GetStdHandle( STD_ERROR_HANDLE );

    DWORD flags = 0;

    _Bool res = CreateProcessA(
        NULL, (char*)command_line, NULL, NULL,
        FALSE, flags, NULL, NULL, &startup, &info );

    if( !res ) {
        cb_error( "failed to run command '%s'!", command_line );
        return false;
    }

    CloseHandle( info.hThread );
    *out_pid = info.hProcess;
    return true;
}
_Bool program_wait( ProcessID pid, uint32_t ms, int* opt_out_res ) {
    if( WaitForSingleObject( pid, ms ) == WAIT_TIMEOUT ) {
        cb_error( "process %p timed out!", pid );
        return false;
    }
    if( opt_out_res ) {
        DWORD exit_code = 0;
        if( !GetExitCodeProcess( pid, &exit_code ) ) {
            cb_warn( "process %p finished but failed to get return code!" );
            return true;
        }
        *opt_out_res = exit_code;
    }

    return true;
}
void  program_terminate( ProcessID pid ) {
    TerminateProcess( pid, 0 );
    CloseHandle( pid );
}
void  program_close( ProcessID pid ) {
    CloseHandle( pid );
}

#else /* Platform Windows */

// NOTE(alicia): SPOSIX

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

void platform_init(void) {
    return;
}
_Bool path_peek_chunk( dpath* path, substring* opt_out_chunk ) {
    size_t path_len = dstring_len( path );
    if( !path_len ) {
        return false;
    }
    size_t pos   = 0;
#if defined(_WIN32)
    substr_find_last_set(
        substr_new( path_len, path ), substr_text( "/\\" ), &pos );
#else
    substr_find_last( substr_new( path_len, path ), '/', &pos );
#endif
    if( opt_out_chunk ) {
        opt_out_chunk->cc  = path     + pos;
        opt_out_chunk->len = path_len - pos;
    }
    return true;
}
_Bool path_pop_chunk( dpath* path, char* opt_out_chunk ) {
    substring chunk;
    memory_zero( &chunk, sizeof(chunk) );
    if( !path_peek_chunk( path, &chunk ) ) {
        return false;
    }
    if( opt_out_chunk ) {
        memory_copy( opt_out_chunk, chunk.cc, chunk.len );
        opt_out_chunk[chunk.len] = 0;
    }
    dstring_truncate( path, dstring_len( path ) - chunk.len );
    return true;
}
dpath* path_canonicalize_substr( substring path ) {
    size_t size      = 0;
    dpath* res       = NULL;
    substring home;
    memory_zero( &home, sizeof(home) );

    #define TYPE_RELATIVE 0 
    #define TYPE_ABSOLUTE 1
    #define TYPE_HOME     2
    int type = TYPE_RELATIVE;

    if( path.cc[0] == '~' ) {
        type = TYPE_HOME;
    } else {
        if( path_is_absolute_substr( path ) ) {
            type = TYPE_ABSOLUTE;
        } else {
            type = TYPE_RELATIVE;
        }
    }

    switch( type ) {
        case TYPE_RELATIVE: {
            size = PATH_MAX;
        } break;
        case TYPE_ABSOLUTE: {
            size = path.len + 16;
        } break;
        case TYPE_HOME: {
            home.cc = getenv("HOME");
            if( !home.cc ) {
                cb_error( "path_canonicalize: failed to get 'HOME' env var!" );
                return NULL;
            }
            home.len = string_len( home.cc );

            size = path.len + home.len + 16;
        } break;
    }

    res = dstring_empty( size );
    if( !res ) {
        return NULL;
    }

    substring subpath = path;
    switch( type ) {
        case TYPE_RELATIVE: {
            if( !getcwd( res, dstring_cap( res ) ) ) {
                dstring_free( res );
                cb_error( "path_canonicalize: failed to get pwd!" );
                return NULL;
            }
            dstring_header* head = dstring_head( res );
            head->len = string_len( res );
        } break;
        case TYPE_HOME: {
            res     = dstring_append_substr( res, home );
            subpath = substr_advance( subpath );
        } break;
        case TYPE_ABSOLUTE: {
            res     = dstring_push( res, '/' );
            subpath = substr_advance( subpath );
        } break;
    }

    while( subpath.len ) {
        size_t sep = 0;
        if( substr_find( subpath, '/', &sep ) ) {
            if( !sep ) {
                subpath = substr_advance( subpath );
                continue;
            }

            substring chunk = subpath;
            chunk.len       = sep;

            _Bool should_push = true;
            if( chunk.len < 3 ) {
                if( substr_cmp( chunk, substr_text( "." ) ) ) {
                    should_push = false;
                } else if( substr_cmp( chunk, substr_text( ".." ) ) ) {
                    should_push = false;
                    path_pop_chunk( res, NULL );
                }
            }

            if( should_push ) {
                res = path_push_chunk_substr( res, subpath );
            }

            subpath = substr_advance_by( subpath, chunk.len + 1 );
        } else {
            res = path_push_chunk_substr( res, subpath );
            break;
        }
    }

    #undef TYPE_RELATIVE 
    #undef TYPE_ABSOLUTE
    #undef TYPE_HOME    
    return res;
}
_Bool path_is_absolute_substr( substring path ) {
    return path.len ? path.cc[0] == '/' : false;
}
_Bool path_char_is_separator( char c ) {
    return c == '/';
}
dpath* path_push_chunk_substr( dpath* path, substring chunk ) {
    dstring_header* head = dstring_head( path );

    head = dstring_head( dstring_grow( head->buf, chunk.len + 2 ) );
    if( !head ) {
        return NULL;
    }

    if( chunk.cc[0] != '/' ) {
        head = dstring_head( dstring_push( head->buf, '/' ) );
    }

    head = dstring_head( dstring_append_substr( head->buf, chunk ) );
    return head->buf;
}
_Bool file_open( const char* path, FileOpenFlags flags, File* out_file ) {
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
_Bool file_write( File file, size_t size, const void* buf ) {
    return write( file, buf, size ) != -1;
}
_Bool file_read( File file, size_t size, void* buf ) {
    return read( file, buf, size ) != -1;
}
void file_close( File file ) {
    close( file );
}
_Bool file_truncate( File file ) {
    off_t pos = lseek( file, 0, SEEK_CUR );
    return ftruncate( file, pos ) == 0;
}
_Bool file_query_size( File file, size_t* out_size ) {
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
_Bool file_seek( File file, FileSeekType type, ssize_t seek ) {
    int whence = 0;
    switch( type ) {
        case FILE_SEEK_TYPE_START: {
            whence = SEEK_SET;
        } break;
        case FILE_SEEK_TYPE_END: {
            whence = SEEK_END;
        } break;
        default: {
            whence = SEEK_CUR;
        } break;
    }
    return lseek( file, seek, whence ) >= 0;
}
_Bool file_query_seek( File file, ssize_t* out_seek ) {
    off_t pos = lseek( file, 0, SEEK_CUR );
    if( pos < 0 ) {
        cb_error( "file_query_seek: failed to get current position!" );
        return false;
    }
    *out_seek = pos;
    return true;
}
_Bool file_query_info( File file, FileInfo* out_info ) {
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
_Bool file_query_info_path( const char* path, FileInfo* out_info ) {
    File fd;
    if( !file_open( path, FILE_OPEN_FLAG_READ, &fd ) ) {
        cb_error( "file_query_info_path: failed to open file '%s'", path );
        return false;
    }
    _Bool res = file_query_info( fd, out_info );
    file_close( fd );
    return res;
}
_Bool file_move( const char* dst, const char* src ) {
    if( rename( src, dst ) == 0 ) {
        return true;
    } else {
        cb_error( "failed to move '%s' to '%s'", src, dst );
        return false;
    }
}
_Bool file_copy( const char* dst, const char* src ) {
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
_Bool file_remove( const char* path ) {
    if( remove(path) == 0 ) {
        return true;
    } else {
        cb_error( "failed to remove file '%s'", path );
        return false;
    }
}

_Bool program_exec_async( const char* command_line, ProcessID* out_pid ) {
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
_Bool program_wait( ProcessID pid, uint32_t ms, int* opt_out_res ) {
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

_Bool program_in_path( const char* prog ) {
    dstring* cmd = dstring_fmt( "which %s 2>&1 /dev/null", prog );

    if( !cmd ) {
        cb_error( "program_in_path: failed to allocate command buffer!" );
        return false;
    }

    _Bool result = system( cmd ) == 0;
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
dstring* path_pwd(void) {
    dstring_header* res = dstring_head( dstring_empty( 1024 ) );
    if( !res ) {
        return NULL;
    }
    
    if( !getcwd( res->buf, res->cap ) ) {
        free( res );
        return NULL;
    }
    res->len = string_len( res->buf );

    return res->buf;
}

int atomic_add( Atom* ptr, int val ) {
#if defined(__STDC_NO_ATOMICS__)
    return __sync_fetch_and_add( (ptr), (val) );
#else
    return atomic_fetch_add( ptr, val );
#endif
}

#endif /* Platform POSIX */

#endif /* Implementation */

