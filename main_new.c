#undef _CLANGD
#define CBUILD_ASSERTIONS
#include "cbuild_new.h"
#include <stdio.h>
#include <string.h>
#include <glob.h>

int main( int argc, char** argv ) {
    init( LOGGER_LEVEL_INFO );

    glob_t buf;
    if( glob( "**/*", GLOB_NOSORT | GLOB_TILDE, 0, &buf ) == 0 ) {
        cb_info( "glob success" );

        for( usize i = 0; i < buf.gl_pathc; ++i ) {
            cb_info( "%zu: %s", i, buf.gl_pathv[i] );
        }

        globfree( &buf );
    }

    return 0;
}

#define CBUILD_IMPLEMENTATION
#include "cbuild_new.h"

