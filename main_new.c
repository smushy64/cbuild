#undef _CLANGD
#define CBUILD_ASSERTIONS
#include "cbuild_new.h"
#include <stdio.h>

void test( u32 id, void* params ) {
    cb_info( id, "%s", params );
}

int main( int argc, char** argv ) {

    expect( false, "test" );

    u32 id = 0;
    #define push()\
        printf( ">>> %u . . .\n", id );\
        job_enqueue( test, "hello, world!" );\
        printf( "||| %u!\n", id++ );

    push();
    push();
    push();
    push();
    job_wait_all( MT_WAIT_INFINITE );

    return 0;
}

#define CBUILD_IMPLEMENTATION
#include "cbuild_new.h"

