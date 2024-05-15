#if !defined(CBUILD_H)
#define CBUILD_H
/**
 * @file   cbuild.h
 * @brief  C build system.
 * @details
 * Single header library, include to get access to API,
 * include again (ONCE) with CBUILD_IMPLEMENTATION defined.
 * Define MAX_THREADS before implementation to set maximum
 * number of threads allowed.
 * Define MAX_JOBS before implementation to set maximum number of
 * multi-threaded jobs allowed.
 * Inspired by https://github.com/tsoding/nobuild
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   April 06, 2024
*/
// IWYU pragma: begin_exports
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
// IWYU pragma: end_exports

#if defined(__STDC_NO_ATOMICS__) || defined(_MSC_VER) || defined(_WIN32)
    typedef volatile unsigned long Atom;
    typedef volatile uint64_t      Atom64;
#else
    #include <stdatomic.h> // IWYU pragma: export
    typedef volatile atomic_uint   Atom;
    typedef volatile atomic_ullong Atom64;
#endif

#if defined(_WIN32)
    typedef void* ProcessID;
    typedef intptr_t ssize_t;
#else
    #include <unistd.h>
    typedef pid_t ProcessID;
#endif

typedef char dstring;

typedef struct substring {
    size_t      len;
    const char* cc;
} substring;

typedef int FileType;
#define FILE_TYPE_UNKNOWN_  0
#define FILE_TYPE_FILE      1
#define FILE_TYPE_DIRECTORY 2

typedef struct File {
#if defined(_WIN32)
    void* handle;
#else
    int handle;
#endif
    FileType   type;
    time_t     create;
    time_t     modify;
    size_t     size;
} File;
#define file_null() (File){ .handle=0, .type=0, .create=0, .modify=0, .size=0 }

typedef struct FindFiles {
    size_t     count; // duplicate of darray_len( paths )
    substring* paths;
    dstring*   buffer;
} FindFiles;

typedef struct Pipe {
#if defined(_WIN32)
    void* handle;
#else
    int   handle;
#endif
} Pipe;
#define pipe_null() (Pipe){ .handle=0 }

typedef struct Mutex {
    void* handle;
} Mutex;
#define mutex_null() (Mutex){ .handle=0 }

typedef struct Semaphore {
    void* handle;
} Semaphore;
#define semaphore_null() (Semaphore){ .handle=0 }

#define static_array_len( array )\
    (sizeof((array)) / sizeof((array)[0]))
static inline void _0(int a,...) {(void)a;}
#define unused( ... ) _0( 0, __VA_ARGS__ )

#define kilobytes( kb ) (kb * 1000ULL)
#define megabytes( mb ) (kilobytes(mb) * 1000ULL)
#define gigabytes( gb ) (megabytes(gb) * 1000ULL)
#define terabytes( tb ) (gigabytes(tb) * 1000ULL)

#define kibibytes( kb ) (kb * 1024ULL)
#define mebibytes( mb ) (kibibytes(mb) * 1024ULL)
#define gibibytes( gb ) (mebibytes(gb) * 1024ULL)
#define tebibytes( tb ) (gibibytes(tb) * 1024ULL)

#define PATH_MAX_LEN (4096)

typedef int FileTimeCmp;
#define FILE_TIME_CMP_EQUAL           0
#define FILE_TIME_CMP_LEFT_IS_NEWER  -1
#define FILE_TIME_CMP_RIGHT_IS_NEWER  1
#define FILE_TIME_CMP_ERROR           2

typedef unsigned int FileTimeCmpWhich;
#define FILE_TIME_CMP_MODIFY 0
#define FILE_TIME_CMP_CREATE 1

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

typedef void JobFN( uint32_t thread_id, void* params );

typedef uint32_t LogLevel;
#define LOG_LEVEL_INFO  0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_ERROR 2

void     logger_level_set( LogLevel level );
LogLevel logger_level_get(void);
void     logger( LogLevel level, const char* format, ... );
#define cb_info( ... )  logger( LOG_LEVEL_INFO, __VA_ARGS__ )
#define cb_warn( ... )  logger( LOG_LEVEL_WARN, __VA_ARGS__ )
#define cb_error( ... ) logger( LOG_LEVEL_ERROR, __VA_ARGS__ )

void*  memory_alloc( size_t size );
void*  memory_realloc( void* memory, size_t old_size, size_t new_size );
void   memory_free( void* memory, size_t size );
size_t memory_query_allocated(void);
size_t memory_query_total_allocated(void);
void   memory_set( void* memory, char value, size_t size );
void   memory_zero( void* memory, size_t size );
void   memory_copy( void* restrict dst, const void* restrict src, size_t size );
void   memory_move( void* dst, const void* src, size_t size );
_Bool  memory_cmp( const void* a, const void* b, size_t size );

#define substr_empty()\
    (substring){ .cc=0, .len=0 }
#define substr_text( literal )\
    (substring){ .cc=literal, .len=sizeof(literal) - 1 }
#define substr_new( opt_len, string )\
    (substring){ .cc=string, .len=opt_len ? opt_len : string_len( string ) }
#define substr_from_dstring( dstring )\
    (substring){ .cc=dstring, .len=dstring_len( dstring ) }
_Bool      substr_is_empty( substring substr );
_Bool      substr_is_null_terminated( substring substr );
_Bool      substr_cmp( substring a, substring b );
_Bool      substr_cmp_max( substring a, substring b );
_Bool      substr_find( substring substr, char c, size_t* opt_out_pos );
_Bool      substr_find_last( substring substr, char c, size_t* opt_out_pos );
_Bool      substr_find_set( substring substr, substring set, size_t* opt_out_pos );
_Bool      substr_find_last_set(
    substring substr, substring set, size_t* opt_out_pos );
_Bool      substr_find_substr(
    substring source, substring substr, size_t* opt_out_pos );
char*      substr_first( substring substr );
char*      substr_last( substring substr );
substring  substr_advance( substring substr );
substring  substr_advance_by( substring substr, size_t amount );
substring  substr_truncate( substring substr, size_t max );
substring  substr_trim( substring substr, size_t amount );
substring  substr_trim_ws_lead( substring substr );
substring  substr_trim_ws_trail( substring substr );
substring  substr_trim_ws( substring substr );
size_t     substr_len_utf8( substring substr );
substring* substr_darray_from_arg( int argc, char** argv );
substring* substr_darray_from_delim( substring source, substring delim );

size_t string_len( const char* string );
size_t string_len_utf8( const char* string );
_Bool  string_cmp( const char* a, const char* b );
_Bool  string_find( const char* string, char c, size_t* out_pos );
_Bool  string_find_last( const char* string, char c, size_t* out_pos );
_Bool  string_find_set( const char* string, const char* set, size_t* out_pos );
_Bool  string_find_string(
    const char* string, const char* substr, size_t* out_pos );

const char* local_fmt( const char* format, ... );
const char* local_fmt_va( const char* format, va_list va );
const char* local_fmt_bytesize( size_t bytes );

dstring* dstring_empty( size_t size );
dstring* dstring_new( size_t opt_len, const char* source );
#define  dstring_literal( literal ) dstring_new( sizeof(literal) - 1, literal )
#define  dstring_from_substr( substr ) dstring_new( (substr)->len, (substr)->cc )
dstring* dstring_concat_array_substr( size_t len, substring* strings, substring sep );
dstring* dstring_concat_array( size_t len, const char** strings, const char* sep );
dstring* dstring_fmt_va( const char* format, va_list va );
dstring* dstring_fmt( const char* format, ... );
dstring* dstring_clone( const dstring* source );
void     dstring_free( dstring* string );
void*    dstring_head( dstring* string );
dstring* dstring_grow( dstring* string, size_t amount );
void     dstring_clear( dstring* string );
void     dstring_truncate( dstring* string, size_t max_len );
size_t   dstring_len( const dstring* string );
size_t   dstring_cap( const dstring* string );
size_t   dstring_remaining( const dstring* string );
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
size_t darray_remaining( const void* darray );
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
void*  darray_insert( void* darray, size_t index, const void* item );
_Bool  darray_remove( void* darray, size_t at, void* opt_out_item );
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

#if defined(_WIN32)
    unsigned long atomic_add( Atom* ptr, unsigned long val );
    unsigned long atomic_sub( Atom* ptr, unsigned long val );

    unsigned long long atomic_add64( Atom64* ptr, unsigned long long val );
    unsigned long long atomic_sub64( Atom64* ptr, unsigned long long val );
#else
    unsigned int atomic_add( Atom* ptr, unsigned int val );
    unsigned int atomic_sub( Atom* ptr, unsigned int val );

    unsigned long atomic_add64( Atom64* ptr, unsigned long val );
    unsigned long atomic_sub64( Atom64* ptr, unsigned long val );
#endif
void  thread_sleep( uint32_t ms );

_Bool mutex_create( Mutex* out_mtx );
void  mutex_destroy( Mutex* mtx );
_Bool mutex_lock( Mutex* mtx, uint32_t ms );
void  mutex_unlock( Mutex* mtx );

_Bool semaphore_create( Semaphore* out_sem );
void  semaphore_destroy( Semaphore* sem );
_Bool semaphore_wait( Semaphore* sem, uint32_t ms );
void  semaphore_signal( Semaphore* sem );

_Bool job_enqueue( JobFN* job, void* params, uint32_t ms );
_Bool job_wait( uint32_t ms );
_Bool job_wait_all( uint32_t ms );

_Bool    pipe_create( Pipe* out_read, Pipe* out_write );
uint32_t pipe_peek( Pipe* rpipe );
_Bool    pipe_read(
    Pipe* rpipe, uint32_t buf_size, void* buf, uint32_t* out_read_size );
_Bool    pipe_write(
    Pipe* wpipe, uint32_t buf_size, const void* buf, uint32_t* out_write_size );
_Bool    pipe_write_fmt_va( Pipe* wpipe, const char* format, va_list va );
_Bool    pipe_write_fmt( Pipe* wpipe, const char* format, ... );
void     pipe_close( Pipe* pipe );
_Bool    pipe_check( Pipe* pipe );

_Bool program_in_path( const char* prog );
_Bool program_exec( const char* command_line, int* opt_out_res );
_Bool program_exec_async(
    const char* command_line,
    Pipe* opt_stdin_r, Pipe* opt_stdout_w, Pipe* opt_stderr_w,
    ProcessID* out_pid );
_Bool program_wait( ProcessID pid, uint32_t ms, int* opt_out_res );
void  program_terminate( ProcessID pid );
void  program_close( ProcessID pid );

double time_seconds(void);
double time_milliseconds(void);

dstring* path_push_chunk( dstring* path, const char* chunk );
dstring* path_push_chunk_substr( dstring* path, substring chunk );
_Bool    path_peek_chunk(
    const char* path, size_t* out_chunk_len, const char** out_chunk );
_Bool    path_peek_chunk_substr( substring path, substring* out_chunk );
_Bool    path_pop_chunk(
    dstring* path, size_t opt_chunk_size, char* opt_out_chunk );
_Bool    path_file_name( substring path, substring* out_chunk );
substring* path_split_chunks_substr( substring path );
substring* path_split_chunks( const char* path );
_Bool    path_glob_match_substr( substring path, substring glob );
_Bool    path_glob_match( const char* path, const char* glob );

_Bool path_find_files( const char* path, FindFiles* out_result );
_Bool path_find_files_substr( substring path, FindFiles* out_result );
void  path_find_files_free( FindFiles* ff );

_Bool file_open( const char* path, FileOpenFlags flags, File* out_file );
_Bool file_write( File* file, size_t size, const void* buf );
_Bool file_write_fmt_va( File* file, const char* format, va_list va );
_Bool file_write_fmt( File* file, const char* format, ... );
_Bool file_read( File* file, size_t size, void* buf );
void  file_close( File* file );
_Bool file_check( File* file );
_Bool file_truncate( File* file );
_Bool file_seek( File* file, FileSeekType type, ssize_t seek );
_Bool file_query_seek( File* file, ssize_t* out_seek );
_Bool file_path_query_create( const char* path, time_t* out_time );
_Bool file_path_query_modify( const char* path, time_t* out_time );
_Bool file_path_move( const char* dst, const char* src );
_Bool file_path_copy( const char* dst, const char* src );
_Bool file_path_remove( const char* path );
FileTimeCmp file_path_time_cmp(
    const char* lhs, const char* rhs, FileTimeCmpWhich which );
FileTimeCmp file_path_time_cmp_multi(
    const char* lhs, size_t count, const char** rhs, FileTimeCmpWhich which );
FileTimeCmp file_path_time_cmp_multi_substr(
    const char* lhs, size_t count, substring* rhs, FileTimeCmpWhich which );

Compiler query_compiler(void);
Platform query_platform(void);
Arch     query_arch(void);
_Bool    query_64bit(void);
_Bool    compiler_is_mingw(void);
_Bool    platform_is_posix( Platform platform );

const char* cbuild_compiler(void);
const char* cbuild_command_line(void);
const char* cbuild_compile_time(void);

void _init_( const char* argv0, const char* file, LogLevel log );
#define init( log_level )\
    _init_( argv[0], __FILE__, log_level )

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
            #define COMPILER_CFLAGS " -O0 -g "
        #endif
    #else
        #define COMPILER_CFLAGS " -O2 "
    #endif
    #if !defined(_WIN32)
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

_Bool internal_thread_create( JobFN* proc, void* params );

static Mutex global_log_mtx = mutex_null();

// NOTE(alicia): C-stdlib implementation

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
FileTimeCmp file_path_time_cmp_multi_substr(
    const char* lhs, size_t count, substring* rhs, FileTimeCmpWhich which
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

_Bool path_find_files( const char* path, FindFiles* out_result ) {
    return path_find_files_substr( substr_new( 0, path ), out_result );
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

_Bool pipe_write_fmt_va( Pipe* wpipe, const char* format, va_list va ) {
    dstring* str = dstring_fmt_va( format, va );
    if( !str ) {
        cb_error( "pipe_write_fmt: failed to format string!" );
        return false;
    }
    substring substr = substr_from_dstring( str );
    while( substr.len ) {
        uint32_t written = 0;
        if( !pipe_write( wpipe, substr.len, substr.cc, &written ) ) {
            dstring_free( str );
            return false;
        }

        substr = substr_advance_by( substr, written );
    }
    dstring_free( str );
    return true;
}
_Bool pipe_write_fmt( Pipe* wpipe, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    _Bool res = pipe_write_fmt_va( wpipe, format, va );
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
    struct JobEntry entries[];
};
#if !defined(MAX_THREADS) || MAX_THREADS == 0
    #undef MAX_THREADS
    #define MAX_THREADS (8)
#endif
#if !defined(MAX_JOBS) || MAX_JOBS == 0
    #undef  MAX_JOBS
    #define MAX_JOBS    (32)
#endif

static struct JobQueue* global_queue = NULL;

_Bool job_dequeue( struct JobQueue* queue, struct JobEntry* out_entry ) {
    if( !queue->len ) {
        return false;
    }
    memory_fence();

    Atom front = atomic_add( &queue->front, 1 );

    memory_fence();

    *out_entry = queue->entries[ (front + 1) % MAX_JOBS ];
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

_Bool ___create_queue(void) {
    if( global_queue ) {
        return true;
    }
    cb_info( "creating job queue . . ." );
    size_t buf_size = sizeof(struct JobQueue) + (sizeof(struct JobEntry) * MAX_JOBS);
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

    for( size_t i = 0; i < MAX_THREADS; ++i ) {
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

    cb_info( "job queue created with %u threads.", MAX_THREADS );
    global_queue = queue;

    return true;
}

_Bool job_enqueue( JobFN* job, void* params, uint32_t ms ) {
    if( !___create_queue() ) {
        return false;
    }

    if( ms ) {
        if( global_queue->len >= MAX_JOBS ) {
            if( !job_wait( ms ) ) {
                return false;
            }
        }
    } else {
        if( global_queue->len >= MAX_JOBS ) {
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

    global_queue->entries[back % MAX_JOBS] = entry;

    memory_fence();

    semaphore_signal( &global_queue->wakeup );

    #undef check
    return true;
}
_Bool job_wait( uint32_t ms ) {
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
_Bool job_wait_all( uint32_t ms ) {
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

_Bool file_write_fmt_va( File* file, const char* format, va_list va ) {
    dstring* str = dstring_fmt_va( format, va );
    if( !str ) {
        cb_error( "file_write_fmt: failed to format string!" );
    }
    if( !dstring_len( str ) ) {
        dstring_free( str );
        return true;
    }

    _Bool res = file_write( file, dstring_len( str ), str );
    dstring_free( str );
    return res;
}
_Bool file_write_fmt( File* file, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    _Bool res = file_write_fmt_va( file, format, va );
    va_end( va );
    return res;
}
substring* path_split_chunks_substr( substring path ) {
    if( !path.len || !path.cc ) {
        cb_warn( "path_split_chunks: attempted to split empty path!" );
        return NULL;
    }

    substring* res = darray_empty( substring, 10 );
    if( !res ) {
        cb_error( "path_split_chunks: failed to allocate result!" );
        return NULL;
    }
    
    substring subpath = path;
    while( subpath.len ) {
        size_t sep = 0;
        if( substr_find( subpath, '/', &sep ) ) {
            if( !sep ) {
                subpath = substr_advance( subpath );
                continue;
            }

            substring chunk = subpath;
            chunk.len       = sep;

            substring* _new = darray_push( res, &chunk );
            if( !_new ) {
                darray_free( res );
                cb_error( "path_split_chunks: failed to reallocate result!" );
                return NULL;
            }

            res     = _new;
            subpath = substr_advance_by( subpath, sep + 1 );
        } else {
            substring* _new = darray_push( res, &subpath );
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
substring* path_split_chunks( const char* path ) {
    return path_split_chunks_substr( substr_new( 0, path ) );
}
_Bool path_glob_match_substr( substring path, substring glob ) {
    if( glob.len == 1 && *glob.cc == '*' ) {
        return true;
    }

    while( path.len && *glob.cc != '*' ) {
        if( *glob.cc != *path.cc && *glob.cc != '?' ) {
            return false;
        }
        glob = substr_advance( glob );
        path = substr_advance( path );
    }

    substring mp, cp;
    while( path.len ) {
        if( *glob.cc == '*' ) {
            glob = substr_advance( glob );
            if( !glob.len ) {
                return true;
            }

            mp = glob;
            cp = substr_advance( path );
        } else if( *glob.cc == *path.cc || *glob.cc == '?' ) {
            glob = substr_advance( glob );
            path = substr_advance( path );
        } else {
            glob = mp;
            cp   = substr_advance( cp );
            path = cp;
        }
    }

    while( glob.len && *glob.cc == '*' ) {
        glob = substr_advance( glob );
    }
    return glob.len ? false : true;
}
_Bool path_glob_match( const char* path, const char* glob ) {
    return path_glob_match_substr( substr_new( 0, path ), substr_new( 0, glob ) );
}
dstring* path_push_chunk( dstring* path, const char* chunk ) {
    return path_push_chunk_substr( path, substr_new( 0, chunk ) );
}
dstring* path_push_chunk_substr( dstring* path, substring chunk ) {
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

    return dstring_append_substr( head->buf, chunk );
}
_Bool path_peek_chunk(
    const char* path, size_t* out_chunk_len, const char** out_chunk
) {
    substring chunk;
    memory_zero( &chunk, sizeof(chunk) );
    _Bool res = path_peek_chunk_substr(
        substr_new( 0, path ), &chunk );

    if( res ) {
        *out_chunk_len = chunk.len;
        *out_chunk     = chunk.cc;
    }
    return res;
}
_Bool path_peek_chunk_substr(
    substring path, substring* out_chunk
) {
    if( !path.len ) {
        return false;
    }
    size_t sep = 0;
    if( substr_find_last( path, '/', &sep ) ) {
        if( !sep ) {
            return false;
        }
        substring chunk = path;
        chunk = substr_advance_by( chunk, sep + 1 );

        *out_chunk = chunk;
        return out_chunk->len != 0;
    }

    *out_chunk = path;
    return true;
}
_Bool path_pop_chunk(
    dstring* path, size_t opt_chunk_size, char* opt_out_chunk
) {
    substring chunk = substr_empty();
    if( !path_peek_chunk_substr( substr_from_dstring( path ), &chunk ) ) {
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
_Bool path_file_name( substring path, substring* out_chunk ) {
    if( !path.len ) {
        return false;
    }

    size_t sep = 0;
    if( substr_find_last( path, '/', &sep ) ) {
        if( !sep || sep == path.len - 1 ) {
            return false;
        }

        substring file_name = substr_advance_by( path, sep + 1 );
        if( !file_name.len ) {
            return false;
        }

        switch( file_name.len ) {
            case 1: {
                if( substr_cmp( file_name, substr_text( "." ) ) ) {
                    return false;
                }
            } break;
            case 2: {
                if( substr_cmp( file_name, substr_text( ".." ) ) ) {
                    return false;
                }
            } break;
            default: break;
        }

        *out_chunk = file_name;
        return true;
    }

    if( substr_cmp( path, substr_text( "~" ) ) ) {
        return false;
    }
    *out_chunk = path;
    return true;
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
_Bool substr_is_null_terminated( substring substr ) {
    if( !substr.len ) {
        return false;
    }

    if( !substr.cc[substr.len - 1] || !substr.cc[substr.len] ) {
        return true;
    }
    return false;
}
_Bool substr_cmp( substring a, substring b ) {
    if( a.len != b.len ) {
        return false;
    }
    return memory_cmp( a.cc, b.cc, a.len );
}
_Bool substr_cmp_max( substring a, substring b ) {
    size_t max = a.len > b.len ? b.len : a.len;
    return memory_cmp( a.cc, b.cc, max );
}
_Bool substr_find( substring substr, char c, size_t* opt_out_pos ) {
    const char* ptr = memchr( substr.cc, c, substr.len );
    if( !ptr ) {
        return false;
    }
    if( opt_out_pos ) {
        *opt_out_pos = ptr - substr.cc;
    }
    return true;
}
_Bool substr_find_last( substring substr, char c, size_t* opt_out_pos ) {
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
_Bool substr_find_set( substring substr, substring set, size_t* opt_out_pos ) {
    for( size_t i = 0; i < set.len; ++i ) {
        if( substr_find( substr, set.cc[i], opt_out_pos ) ) {
            return true;
        }
    }
    return false;
}
_Bool substr_find_last_set( substring substr, substring set, size_t* out_pos ) {
    for( size_t i = 0; i < set.len; ++i ) {
        if( substr_find_last( substr, set.cc[i], out_pos ) ) {
            return true;
        }
    }
    return false;
}
_Bool substr_find_substr( substring source, substring substr, size_t* out_pos ) {
    substring rem = source;
    while( rem.len ) {
        if( rem.len < substr.len ) {
            return false;
        }

        size_t start = 0;
        if( substr_find( rem, substr.cc[0], &start ) ) {
            rem = substr_advance_by( rem, start );
            if( rem.len < substr.len ) {
                return false;
            }

            substring cmp = rem;
            cmp.len       = substr.len;
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
    if( !substr.len ) {
        return substr;
    }
    substring res = substr;
    res.cc++;
    res.len--;
    return res;
}
substring substr_advance_by( substring substr, size_t amount ) {
    if( !substr.len ) {
        return substr;
    }

    substring res = substr;
    if( amount > res.len ) {
        res.cc  = substr_last( res );
        res.len = 0;
        return res;
    }
    res.cc  += amount;
    res.len -= amount;
    return res;
}
substring substr_truncate( substring substr, size_t max ) {
    if( max > substr.len ) {
        return substr;
    }
    substring res = substr;
    res.len       = max;
    return res;
}
substring substr_trim( substring substr, size_t amount ) {
    substring res = substr;
    if( amount > substr.len ) {
        res.len = 0;
        return res;
    }
    res.len -= amount;
    return res;
}
char* substr_first( substring substr ) {
    if( !substr.len ) {
        return NULL;
    }
    return (char*)substr.cc;
}
char* substr_last( substring substr ) {
    if( !substr.len ) {
        return NULL;
    }
    return (char*)substr.cc + (substr.len - 1);
}
substring substr_trim_ws_lead( substring substr ) {
    substring res = substr;
    while( res.len ) {
        if( isspace( *res.cc ) ) {
            res = substr_advance( res );
        } else {
            break;
        }
    }
    return res;
}
substring substr_trim_ws_trail( substring substr ) {
    substring res = substr;
    while( res.len ) {
        if( isspace( *substr_last( res ) ) ) {
            res.len--;
        } else {
            break;
        }
    }
    return res;
}
substring substr_trim_ws( substring substr ) {
    return substr_trim_ws_lead( substr_trim_ws_trail( substr ) );
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
substring* substr_darray_from_arg( int argc, char** argv ) {
    if( argc <= 0 ) {
        return NULL;
    }
    substring* res = darray_empty( substring, argc );
    if( !res ) {
        return NULL;
    }

    for( int i = 0; i < argc; ++i ) {
        substring ss = substr_new( 0, argv[i] );
        res = darray_push( res, &ss );
    }
    return res;
}
substring* substr_darray_from_delim( substring source, substring delim ) {
    substring substr = source;
    size_t cap = 0;
    while( substr.len ) {
        size_t delim_pos = 0;
        if( substr_find_substr( substr, delim, &delim_pos ) ) {
            cap++;

            substr = substr_advance_by( substr, delim_pos + delim.len );
        } else {
            cap++;
            break;
        }
    }

    substring* res = darray_empty( substring, cap );
    if( !res ) {
        return NULL;
    }

    substr = source;
    while( substr.len ) {
        size_t delim_pos = 0;
        if( substr_find_substr( substr, delim, &delim_pos ) ) {
            substring item = substr;
            item.len       = delim_pos;

            res    = darray_push( res, &item );
            substr = substr_advance_by( substr, item.len + delim.len );
        } else {
            res = darray_push( res, &substr );
            break;
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
dstring* dstring_concat_array_substr( size_t len, substring* strings, substring sep ) {
    size_t total_size = 0;
    for( size_t i = 0; i < len; ++i ) {
        total_size += strings[i].len + sep.len;
    }
    dstring* res = dstring_empty( total_size );
    if( !res ) {
        return NULL;
    }
    for( size_t i = 0; i < len; ++i ) {
        dstring* _new = dstring_append_substr( res, strings[i] );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;

        if( i + 1 >= len ) {
            break;
        }
        _new = dstring_append_substr( res, sep );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;
    }
    return res;
}
dstring* dstring_concat_array( size_t len, const char** strings, const char* sep ) {
    size_t total_size = 0;
    substring _sep    = substr_new( 0, sep );
    for( size_t i = 0; i < len; ++i ) {
        total_size += string_len( strings[i] ) + _sep.len;
    }
    dstring* res = dstring_empty( total_size );
    if( !res ) {
        return NULL;
    }
    for( size_t i = 0; i < len; ++i ) {
        dstring* _new = dstring_append_str( res, strings[i] );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;

        if( i + 1 >= len ) {
            break;
        }
        _new = dstring_append_substr( res, _sep );
        if( !_new ) {
            dstring_free( res );
            return NULL;
        }
        res = _new;
    }
    return res;
}
dstring* dstring_fmt_va( const char* format, va_list va ) {
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
dstring* dstring_fmt( const char* format, ... ) {
    va_list va;
    va_start( va, format );
    dstring* res = dstring_fmt_va( format, va );
    va_end( va );
    return res;
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
    if( string ) {
        dstring_header* head = dstring_head( string );
        size_t size = head->cap + sizeof(*head);
        memory_free( head, size );
    }
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
size_t dstring_remaining( const dstring* string ) {
    dstring_header* head = dstring_head( (dstring*)string );
    return head->cap - head->len;
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
_Bool memory_cmp( const void* a, const void* b, size_t size ) {
    return memcmp( a, b, size ) == 0;
}
void logger_level_set( LogLevel level ) {
    if( level <= LOG_LEVEL_ERROR ) {
        global_log_level = level;
    }
}
LogLevel logger_level_get(void) {
    return global_log_level;
}
void logger( LogLevel level, const char* format, ... ) {
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

    va_list va;
    va_start( va, format );
    
    vfprintf( streams[level], format, va );

    va_end( va );
    fprintf( streams[level], "\033[1;00m\n" );

    memory_fence();
    if( global_queue ) {
        mutex_unlock( &global_log_mtx );
    }
}
_Bool program_exec( const char* command_line, int* opt_out_res ) {
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

    dstring* old_name = dstring_concat_str( argv0, ".old" );

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
        if( substr_cmp( substr_text( "cl" ), substr_text( COMPILER ) ) ) {
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
wchar_t* win32_path_wide( size_t buf_cap, wchar_t* buf, substring path ) {
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

    substring subpath = path;

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

            home_drive_len = string_len( home_drive );
            home_len       = string_len( home );
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

            subpath = substr_advance( subpath );
        } break;
        default: break;
    }

    size_t last_chunk_len = 0;
    while( subpath.len ) {
        size_t sep = 0;
        if( substr_find( subpath, '/', &sep ) ) {
            if( !sep ) {
                subpath = substr_advance( subpath );
                continue;
            }

            substring chunk = subpath;
            chunk.len       = sep;

            if( chunk.len < 3 ) {
                if( substr_cmp( chunk, substr_text( "." ) ) ) {
                    subpath = substr_advance_by( subpath, chunk.len + 1 );
                    continue;
                }
                if( substr_cmp( chunk, substr_text( ".." ) ) ) {
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
                    subpath = substr_advance_by( subpath, chunk.len + 1 );
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

            subpath = substr_advance_by( subpath, chunk.len + 1 );
        } else {
            if( substr_cmp( subpath, substr_text( "." ) ) ) {
                break;
            }
            if( substr_cmp( subpath, substr_text( ".." ) ) ) {
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
wchar_t* win32_path( substring path ) {
    wchar_t* buf = win32_get_path_buffer();
    return win32_path_wide( PATH_MAX_LEN, buf, path );
}
wchar_t* win32_path_alloc( substring path ) {
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

_Bool win32_path_find_dirs( substring pattern, size_t* out_count, dstring** buffer ) {
    size_t dir_count = 0;

    size_t   wide_len    = pattern.len + 32;
    size_t   wide_size   = wide_len * sizeof( wchar_t );
    wchar_t* wide_buffer = memory_alloc( wide_size );

    wchar_t* wpath = win32_path_wide( wide_len, wide_buffer, pattern );

    _Bool recursive_dir = false;

    substring path = pattern; {
        substring last_chunk = substr_empty();
        path_peek_chunk_substr( pattern, &last_chunk );

        if( substr_cmp( last_chunk, substr_text( "**" ) ) ) {
            recursive_dir = true;
        }

        path = substr_trim( path, last_chunk.len + 1 );

        if( !path.len ) {
            path = substr_text( "./" );
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

        _Bool is_self = wcscmp( fd.cFileName, L"." ) == 0;
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

        dstring* _new = dstring_append_substr( *buffer, path );
        if( !_new ) {
            cb_error( "path_find_files: failed to add path to list!" );
            FindClose( f );
            memory_free( wide_buffer, wide_size );
            return false;
        }
        *buffer = _new;

        if( !is_self ) {
            {
                char* last = substr_last( path );
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
_Bool win32_path_find_dirs_recursive(
    substring base, substring pattern, _Bool is_recursive_level,
    dstring** accum, size_t* out_count, dstring** buffer
) {
    substring chunk = pattern; {
        size_t sep = 0;
        if( substr_find( pattern, '/', &sep ) ) {
            chunk.len = sep;
        }

        if( is_recursive_level ) {
            chunk.len--;
        }
    }

    dstring* _new = dstring_append_substr( *accum, base );
    if( !_new ) {
        cb_error( "path_find_files: failed to reallocate accumulator!" );
        return false;
    }
    *accum = _new;

    _new = path_push_chunk_substr( *accum, chunk );
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
    if( !win32_path_find_dirs( substr_from_dstring( *accum ), &count, buffer ) ) {
        return false;
    }

    if( is_recursive_level ) {
        chunk.len++;
    }

    _Bool recursive_dir = substr_cmp( chunk, substr_text( "**" ) );

    if( recursive_dir ) {
        char* dir = *buffer + dir_offset;
        for( size_t i = 0; i < count; ++i ) {
            dstring_clear( *accum );
            substring subdir = substr_new( 0, dir );

            if( substr_cmp( subdir, base ) ) {
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
            dir_offset += string_len( *buffer + dir_offset ) + 1;
            dir         = *buffer + dir_offset;
        }
    }

    *out_count += count;
    pattern = substr_advance_by( pattern, chunk.len + 1 );

    if( !pattern.len ) {
        return true;
    }

    char* dir = *buffer + dir_offset;
    for( size_t i = 0; i < count; ++i ) {
        dstring_clear( *accum );

        substring subdir = substr_new( 0, dir );

        if( !win32_path_find_dirs_recursive(
            subdir, pattern, false,
            accum, out_count, buffer
        ) ) {
            return false;
        }

        dir_offset += string_len( *buffer + dir_offset ) + 1;
        dir         = *buffer + dir_offset;
    }

    return true;
}
_Bool win32_path_find_files(
    size_t dir_count, dstring* dirs,
    substring pattern, size_t* out_count, dstring** buffer
) {
    size_t largest_dir = 0; {
        char* at = dirs;
        for( size_t dir_index = 0; dir_index < dir_count; ++dir_index ) {
            size_t len = string_len( at );
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
    dstring* path = dstring_empty( max_path );
    if( !path ) {
        memory_free( wide_buffer, wide_size );
        cb_error( "path_find_files: failed to allocate path buffer!" );
        return false;
    }

    char*  at = dirs;
    size_t path_count = 0;
    for( size_t dir_index = 0; dir_index < dir_count; ++dir_index ) {
        substring dir = substr_new( 0, at );
        at += dir.len + 1; {
            dstring* _new = dstring_append_substr( path, dir );
            if( !_new ) {
                cb_error( "path_find_files: failed to construct path!" );
                memory_free( wide_buffer, wide_size );
                dstring_free( path );
                return false;
            }
            path = _new;
        } {
            dstring* _new = path_push_chunk_substr( path, pattern );
            if( !_new ) {
                cb_error( "path_find_files: failed to construct path!" );
                memory_free( wide_buffer, wide_size );
                dstring_free( path );
                return false;
            }
            path = _new;
        }

        wchar_t* wpath =
            win32_path_wide( max_path, wide_buffer, substr_from_dstring( path ) );
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

            substring subpath;
            subpath.len = wcslen( fd.cFileName );
            if( !subpath.len ) {
                continue;
            }

            win32_wide_to_narrow_buffer(
                subpath.len, fd.cFileName,
                max_path * sizeof(wchar_t), (char*)wide_buffer );

            subpath.cc = (char*)wide_buffer; {
                dstring* _new = dstring_append_substr( path, subpath );
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

            dstring* buf = dstring_append_substr(
                *buffer, substr_from_dstring( path ) );
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
_Bool path_find_files_substr( substring path, FindFiles* out_result ) {
    substring file_pattern = substr_empty();
    char*  dir             = ".";
    _Bool  dir_allocated   = false;

    if( path_file_name( path, &file_pattern ) && !substr_cmp( path, file_pattern ) ) {
        substring remaining_path = path;
        remaining_path.len -= file_pattern.len;
        if( remaining_path.cc[remaining_path.len - 1] == '/' ) {
            remaining_path.len--;
        }

        size_t first_pat = 0;
        if( !substr_find_set( remaining_path, substr_text( "?*" ), &first_pat ) ) {
            dir           = dstring_from_substr( &remaining_path );
            if( !dir ) {
                cb_error( "path_find_files: failed to allocate dir!" );
                return false;
            }
            dir_allocated = true;
            goto path_find_files_simple;
        }

        substring base = substr_empty(), pat = substr_empty(); {
            base     = remaining_path;
            base.len = first_pat;

            size_t sep = 0;
            if( substr_find_last( base, '/', &sep ) ) {
                base.len = sep;
                pat      = substr_advance_by( remaining_path, sep + 1 );
            } else {
                base = substr_text( "./" );
                pat  = remaining_path;
            }
        }

        dstring* accum = dstring_empty( 255 );
        if( !accum ) {
            cb_error( "path_find_files: failed to allocate accumulation buffer!" );
            return false;
        }
        size_t   dir_count = 0;
        dstring* dirs      = dstring_empty( 255 );
        if( !dirs ) {
            cb_error( "path_find_files: failed to allocate directories buffer!" );
            dstring_free( accum );
            return false;
        }
        _Bool res = win32_path_find_dirs_recursive(
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

        dstring* file_buf = dstring_empty( 255 );

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

        substring* files = darray_empty( substring, file_count );
        if( !files ) {
            dstring_free( file_buf );
            cb_error( "path_find_files: failed to allocate files array!" );
            return false;
        }

        substring remaining_files = substr_from_dstring( file_buf );
        for(
            size_t file_index = 0;
            file_index < file_count && remaining_files.len;
            ++file_index
        ) {
            substring file = remaining_files;
            file.len       = string_len( file.cc );

            darray_push( files, &file );

            remaining_files = substr_advance_by( remaining_files, file.len + 1 );
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
        dstring* buf = dstring_empty(255);
        if( !buf ) {
            cb_error( "path_find_files: failed to allocate buffer!" );
            return false;
        }

        size_t path_count = 0;
        _Bool res = win32_path_find_files( 1, dir, file_pattern, &path_count, &buf );

        if( dir_allocated ) {
            dstring_free( dir );
        }
        if( !res ) {
            dstring_free( buf );
            return false;
        }

        substring* paths = darray_empty( substring, path_count );
        if( !paths ) {
            cb_error( "path_find_files: failed to allocate paths array!" );
            dstring_free( buf );
            return false;
        }

        substring subbuf = substr_from_dstring( buf );
        for( size_t i = 0; i < path_count && subbuf.len; ++i ) {
            substring path = subbuf;
            path.len = string_len( path.cc );
            darray_push( paths, &path );

            subbuf = substr_advance_by( subbuf, path.len + 1 );
        }

        out_result->count  = darray_len( paths );
        out_result->paths  = paths;
        out_result->buffer = buf;
        return true;
    }
}

_Bool mutex_create( Mutex* out_mtx ) {
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
_Bool mutex_lock( Mutex* mtx, uint32_t ms ) {
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

_Bool semaphore_create( Semaphore* out_sem ) {
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
_Bool semaphore_wait( Semaphore* sem, uint32_t ms ) {
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
DWORD thread_proc( void* params ) {
    struct Win32ThreadParams* p = params;
    p->proc( p->id, p->params );
    memory_fence();
    memory_free( p, sizeof(*p) );

    return 0;
}
_Bool internal_thread_create( JobFN* thread, void* params ) {
    struct Win32ThreadParams* p = memory_alloc( sizeof( *p ) );
    if( !p ) {
        cb_error( "failed to create thread!" );
        return false;
    }
    p->id     = atomic_add( &global_thread_id, 1 );
    p->proc   = thread;
    p->params = params;
    memory_fence();

    HANDLE h = CreateThread( 0, 0, thread_proc, p, 0, 0 );
    if( !h ) {
        cb_error( "failed to create thread!" );
        return false;
    }
    return true;
}

_Bool file_open( const char* path, FileOpenFlags flags, File* out_file ) {
    DWORD dwDesiredAccess       = 0;
    if( flags & FILE_OPEN_FLAG_READ ) {
        dwDesiredAccess |= GENERIC_READ;
    }
    if( flags & FILE_OPEN_FLAG_WRITE ) {
        dwDesiredAccess |= GENERIC_WRITE;
    }
    if( !dwDesiredAccess ) {
        cb_error( "file_open: flags provided are invalid!" );
        return false;
    }
    DWORD dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    if( flags & FILE_OPEN_FLAG_CREATE ) {
        dwCreationDisposition = CREATE_ALWAYS;
    } else if( flags & FILE_OPEN_FLAG_TRUNCATE ) {
        dwCreationDisposition = TRUNCATE_EXISTING;
    }
    DWORD dwFlagsAndAttributes = 0;

    wchar_t* wpath = win32_path( substr_new( 0, path ) );
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
                result.type = FILE_TYPE_DIRECTORY;
            } else {
                result.type = FILE_TYPE_FILE;
            }
        }
    }

    if( flags & FILE_OPEN_FLAG_APPEND ) {
        file_seek( result.handle, FILE_SEEK_TYPE_START, result.size );
    }

    *out_file = result;
    return true;
}
_Bool file_path_move( const char* dst, const char* src ) {
    wchar_t* wdst = win32_path( substr_new( 0, dst ) );
    if( !wdst ) {
        cb_error( "file_path_move: failed to allocate path buffer!" );
        return false;
    }
    wchar_t* wsrc = win32_path( substr_new( 0, src ) );
    if( !wsrc ) {
        cb_error( "file_path_move: failed to allocate path buffer!" );
        return false;
    }

    _Bool res = MoveFileW( wsrc, wdst ) != FALSE;
    if( !res ) {
        cb_error( "file_path_move: failed to move %S to %S!", wsrc, wdst );
    }
    return res;
}
_Bool file_path_copy( const char* dst, const char* src ) {
    wchar_t* wdst = win32_path( substr_new( 0, dst ) );
    if( !wdst ) {
        cb_error( "file_path_copy: failed to allocate path buffer!" );
        return false;
    }
    wchar_t* wsrc = win32_path( substr_new( 0, src ) );
    if( !wsrc ) {
        cb_error( "file_path_copy: failed to allocate path buffer!" );
        return false;
    }

    _Bool res = CopyFileW( wsrc, wdst, FALSE ) != FALSE;
    if( !res ) {
        cb_error( "file_path_copy: failed to copy %S to %S!", wsrc, wdst );
    }
    return res;
}
_Bool file_path_remove( const char* file ) {
    wchar_t* wfile = win32_path( substr_new( 0, file ) );
    if( !wfile ) {
        cb_error( "file_path_remove: failed to allocate path buffer!" );
        return false;
    }

    _Bool res = DeleteFileW( wfile ) != FALSE;
    if( !res ) {
        cb_error( "file_path_remove: failed to remove file %S!", wfile );
    }
    return res;
}
_Bool file_write( File* file, size_t size, const void* buf ) {
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
_Bool file_read( File* file, size_t size, void* buf ) {
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
_Bool file_check( File* file ) {
    if( file ) {
        if( !file->handle || file->handle == INVALID_HANDLE_VALUE ) {
            return false;
        }
        return true;
    }
    return false;
}
_Bool file_truncate( File* file ) {
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
_Bool file_seek( File* file, FileSeekType type, ssize_t seek ) {
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
_Bool file_query_seek( File* file, ssize_t* out_seek ) {
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
_Bool file_path_query_create( const char* path, time_t* out_time ) {
    wchar_t* wpath = win32_path( substr_new( 0, path ) );
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
    _Bool res = GetFileTime( handle, &ft, NULL, NULL );
    CloseHandle( handle );

    if( !res ) {
        cb_error( "file_path_query_create: failed to get file time!" );
        return false;
    }

    *out_time = win32_filetime_to_posix_time( ft );
    return true;
}
_Bool file_path_query_modify( const char* path, time_t* out_time ) {
    wchar_t* wpath = win32_path( substr_new( 0, path ) );
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
    _Bool res = GetFileTime( handle, NULL, NULL, &ft );
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
unsigned long atomic_add( Atom* ptr, unsigned long val ) {
    unsigned long res = _InterlockedExchangeAdd( (volatile long*)ptr, val );
    return res;
}
unsigned long atomic_sub( Atom* ptr, unsigned long val ) {
    unsigned long res = _InterlockedExchangeAdd( (volatile long*)ptr, -((long)val) );
    return res;
}
unsigned long long atomic_add64( Atom64* ptr, unsigned long long val ) {
    unsigned long long res =
        _InterlockedExchangeAdd64( (volatile long long*)ptr, val );
    return res;
}
unsigned long long atomic_sub64( Atom64* ptr, unsigned long long val ) {
    unsigned long long res =
        _InterlockedExchangeAdd64( (volatile long long*)ptr, -((long long)val) );
    return res;
}

void thread_sleep( uint32_t ms ) {
    Sleep( ms );
}
dstring* path_pwd(void) {
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
_Bool pipe_create( Pipe* out_read, Pipe* out_write ) {
    SECURITY_ATTRIBUTES attr;
    memory_zero( &attr, sizeof( attr ) );
    attr.nLength        = sizeof(attr);
    attr.bInheritHandle = TRUE;

    HANDLE pipes[2];
    _Bool res = CreatePipe( pipes + 0, pipes + 1, &attr, kibibytes(64) );
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
_Bool pipe_read(
    Pipe* rpipe, uint32_t buf_size, void* buf, uint32_t* out_read_size
) {
    if( !pipe_peek( rpipe ) ) {
        return false;
    }
    DWORD read_size = 0;
    _Bool res = ReadFile( rpipe->handle, buf, buf_size, &read_size, NULL );
    if( res ) {
        *out_read_size = read_size;
        return true;
    }
    return false;
}
_Bool pipe_write(
    Pipe* wpipe, uint32_t buf_size, const void* buf, uint32_t* out_write_size
) {
    DWORD write_size = 0;
    _Bool res = WriteFile( wpipe->handle, buf, buf_size, &write_size, NULL );
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
_Bool pipe_check( Pipe* pipe ) {
    if( pipe ) {
        if( !pipe->handle || pipe->handle == INVALID_HANDLE_VALUE ) {
            return false;
        }
        return true;
    }
    return false;
}
_Bool program_exec_async(
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

    _Bool bInheritHandle = FALSE;
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

    _Bool res = CreateProcessA(
        NULL, (char*)command_line, NULL, NULL,
        bInheritHandle, flags, NULL, NULL, &startup, &info );

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
            cb_warn( "process %p finished but failed to get return code!", pid );
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

// TODO(alicia): MAJOR CHANGES

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

void platform_init(void) {
    return;
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
        memory_free( res );
        return NULL;
    }
    res->len = string_len( res->buf );

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

