/**
 * @file   main.c
 * @brief  C Build example project
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
*/
#include <stdio.h>
#include <string.h>

#if !defined(CBUILD_MESSAGE)
    #define CBUILD_MESSAGE ""
#endif

int main( int argc, char** argv ) {
    for( int i = 1; i < argc; ++i ) {
        printf( "%i: %s\n", i, argv[i] );
    }

    if( strlen( CBUILD_MESSAGE ) ) {
        printf( "message from cbuild: " CBUILD_MESSAGE "\n" );
    }
    return 0;
}


