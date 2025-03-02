/**
 * @file   main.c
 * @brief  Main function for example.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 01, 2025
*/
#undef CB_IMPLEMENTATION
#define CB_IMPLEMENTATION 1
#include "../cbuild.h" /* CBuild can be used both in build program and project! */

#include <stdio.h>

int main( int argc, char** argv ) {
    printf( CB_COLOR_WHITE );

    printf( "hello from project!\n" );
    for( int i = 0; i < argc; ++i ) {
        printf( "  %i: %s\n", i, argv[i] );
    }

    printf( CB_COLOR_RESET );
    fflush( stdout );
    return 0;
}

