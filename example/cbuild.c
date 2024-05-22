/**
 * @file   cbuild.c
 * @brief  C Build example
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
*/
#include "../cbuild.h" // including the header from root
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#if defined(_WIN32)
    #define TARGET "build/program.exe"
#else
    #define TARGET "build/program"
#endif

DynamicString* generate_compile_command( const char* compiler );
int      compile_target( const char* command_line );
void     print_help(void);

struct CollectSourcesJob {
    _Bool     success;
    FindFiles headers;
    FindFiles sources;
};
_Bool rebuild_necessary( struct CollectSourcesJob* csj );
void  collect_sources( uint32_t thread_id, void* params );

int main( int argc, char** argv ) {
    LogLevel level = LOG_LEVEL_INFO;

    // break arguments into sub string list.
    StringSlice* args = string_slice_darray_from_arg( argc, argv );
    // remove the first argument (executable path, not needed for this build 'script').
    darray_remove( args, 0, 0 );

    _Bool silent = false; {
        // find --silent argument
        size_t _argc = darray_len( args );
        for( size_t i = 0; i < _argc; ++i ) {
            if( string_slice_cmp( args[i], string_slice_text( "--silent" ) ) ) {
                // error disables info logs and warnings.
                level = LOG_LEVEL_ERROR;
                // remove this argument so further argument processing
                // doesn't encounter it again.
                darray_remove( args, i, 0 );
                break;
            }
        }
    }

    // initialize cbuild.
    // usually you must do this at the very beginning of the file
    // but in order to silence messages during initialization,
    // we need to parse the '--silent' argument before initializing.
    init( level );

    // start checking time
    double start = time_milliseconds();

    struct CollectSourcesJob csj;
    memory_zero( &csj, sizeof(csj) );

    // collect project source files on a separate thread
    // the UINT32_MAX is how many milliseconds to wait
    // for job_enqueue to complete, MAX means infinite time.
    job_enqueue( collect_sources, &csj, UINT32_MAX );

    _Bool run = false;

    // args parsing but for real this time.
    for( size_t i = 0; i < darray_len( args ); ++i ) {
        StringSlice current = args[i];

        if(
            string_slice_cmp( current, string_slice_text( "-h" ) ) ||
            string_slice_cmp( current, string_slice_text( "--help" ) )
        ) {
            print_help();
            return 0;
        }

        if( string_slice_cmp( current, string_slice_text( "run" ) ) ) {
            run = true;
            continue;
        }

        // print an error and exit.
        // logging functions include:
        //      cb_info  for logging misc info
        //      cb_warn  for logging warnings
        //      cb_error for logging (potentially) fatal errors
        cb_error( "unrecognized argument: %.*s", current.len, current.cc );
        print_help();
        job_wait_all( UINT32_MAX );
        return 1;
    }

    // get compiler used to build cbuild with.
    const char* compiler = cbuild_compiler_string();

    // make sure that compiler is available.
    if( !program_in_path( compiler ) ) {
        cb_error( "%s is not in path!", compiler );
        job_wait_all( UINT32_MAX );
        return 1;
    }

    // and now we wait for the collect sources job to complete.
    // all job related functions insert a compiler and cpu memory barrier so
    // we don't need to do anything here to ensure proper ordering.
    job_wait_all( UINT32_MAX );

    if( !csj.success ) {
        return 1;
    }

    if( rebuild_necessary( &csj ) ) {
        // TODO(alicia): check for build dir (cbuild function)
        mkdir( "build" );
        DynamicString* command = generate_compile_command( compiler );

        int res = compile_target( command );

        dstring_free( command );

        if( res != 0 ) {
            cb_error( "failed to compile " TARGET "!" );
            return 1;
        } else {
            cb_info( "successfully built " TARGET "!" );
        }
    }

    path_find_files_free( &csj.headers );
    path_find_files_free( &csj.sources );

    // free our args buffer.
    // unnecessary at the end of the program but let's be clean about it :)
    darray_free( args );

    // check how long cbuild took
    double end = time_milliseconds();

    cb_info( "build took: %.2fms", end - start );

    if( run ) {
        cb_info( "running " TARGET " . . ." );
        program_exec( TARGET, 0 );
    }
    return 0;
}
int compile_target( const char* command_line ) {
    cb_info( "compiling " TARGET " . . ." );
    cb_info( "command line: %s", command_line );

    int res = 0;
    if( !program_exec( command_line, &res ) ) {
        cb_error( "failed to run command!" );

        return -1;
    }
    return res;
}

_Bool rebuild_necessary( struct CollectSourcesJob* csj ) {
    // check if target already exists
    if( file_path_exists( TARGET ) ) {

        // check if target needs to be rebuilt.
        FileTimeCmp cmp = file_path_time_cmp_multi_substr(
            TARGET, csj->headers.count, csj->headers.paths, FILE_TIME_CMP_MODIFY );

        switch( cmp ) {
            case FILE_TIME_CMP_ERROR: {
                // womp womp
                exit(1);
            } break;
            case FILE_TIME_CMP_RIGHT_IS_NEWER: return true;
            default: break;
        }

        cmp = file_path_time_cmp_multi_substr(
            TARGET, csj->sources.count,
            csj->sources.paths, FILE_TIME_CMP_MODIFY );

        switch( cmp ) {
            case FILE_TIME_CMP_ERROR: {
                // womp womp
                exit(1);
            } break;
            case FILE_TIME_CMP_RIGHT_IS_NEWER: return true;
            default: return false;
        }
    } else {
        return true;
    }
}

DynamicString* generate_compile_command( const char* compiler ) {
#if defined(_MSC_VER)
    #define REBUILD_TARGET_COMMAND "%s /Fe:%s"
#else
    #define REBUILD_TARGET_COMMAND "%s -o %s"
#endif

    return dstring_fmt( "%s " REBUILD_TARGET_COMMAND,
            compiler, "src/main.c", TARGET );
}

void collect_sources( uint32_t thread_id, void* params ) {
    unused(thread_id);

    struct CollectSourcesJob* csj = params;
    csj->success = true;

    // use a glob pattern to search for files.
    //     *  - matches any characters multiple times
    //     ?  - matches any one character
    //     ** - recursively search for directories matching pattern.
    if( !path_find_files( "./src/**/*.c", &csj->sources ) ) {
        cb_error( "failed to collect project source files!" );
        csj->success = false;
        return;
    }

    if( !path_find_files( "./src/**/*.h", &csj->headers ) ) {
        cb_error( "failed to collect project source files!" );
        csj->success = false;
        return;
    }
}

void print_help(void) {
    #define println(format, ...) printf( format "\n", ##__VA_ARGS__ )

    println( "OVERVIEW: CBuild Example.\n" );
    println( "USAGE:    cbuild [args]\n" );
    println( "ARGUMENTS: " );
    println( "    run            run target after compiling." );
    println( "    --silent       don't print anything but errors to stderr." );
    println( "    -h or --help   print this message and quit." );

    #undef println
}

#define CBUILD_IMPLEMENTATION
#include "../cbuild.h"

