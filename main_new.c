#undef _CLANGD
#define CBUILD_ASSERTIONS
#include "cbuild_new.h"
#include <stdio.h>
#include <string.h>

static const char* name = "test";

void print_help(void) {
    cb_info( "OVERVIEW:  Testing Cbuild" );
    cb_info( "USAGE:     %s [args]", name );
    cb_info( "ARGUMENTS:" );
    cb_info( "    --silent Disable all messages except fatal." );
    cb_info( "    --help   Print this message and exit." );
}

int main( int argc, const char** argv ) {
    const char** args = darray_from_array( sizeof(const char*), argc, argv );
    name = argv[0];
    darray_remove( args, 0 );

    LoggerLevel level = LOGGER_LEVEL_INFO;
    for( usize i = 0; i < darray_len( args ); ++i ) {
        string arg = string_from_cstr( args[i] );
        if( string_cmp( arg, string_text( "--silent" ) ) ) {
            level = LOGGER_LEVEL_FATAL;

            darray_remove( args, i );
            break;
        }
    }

    init( level );

    f64 start = timer_milliseconds();

    WalkDirectory walk;
    memory_zero( &walk, sizeof(walk) );

    path_walk_dir( ".", true, false, &walk );

    string* sources = path_walk_glob( &walk, string_text( "*.c" ) );
    string* headers = path_walk_glob( &walk, string_text( "*.h" ) );

    cb_info( "sources: (%zu)", darray_len( sources ) );
    for( usize i = 0; i < darray_len( sources ); ++i ) {
        cb_info( "    %zu: %s", i, sources[i].cc );
    }
    cb_info( "headers: (%zu)", darray_len( headers ) );
    for( usize i = 0; i < darray_len( headers ); ++i ) {
        cb_info( "    %zu: %s", i, headers[i].cc );
    }

    darray_free( sources );
    darray_free( headers );

    path_walk_free( &walk );

    f64 end = timer_milliseconds();
    cb_info( "completed in %fms", end - start );

    return 0;
}

#define CBUILD_IMPLEMENTATION
#define CBUILD_COMPILER_NAME "clang"
#include "cbuild_new.h"

