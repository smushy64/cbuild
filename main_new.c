#undef _CLANGD
#define CBUILD_ASSERTIONS
#include "cbuild_new.h"

void print_integers( usize count, const int* integers, const char* format, ... ) {

    static dstring* buf = 0;
    if( !buf ) {
        buf = dstring_text( "{ " );
    } else {
        buf = dstring_append_text( buf, "{ " );
    }

    for( usize i = 0; i < count; ++i ) {
        char* fmt = local_char_buf_fmt( THREAD_MAIN, "%i", integers[i] );

        buf = dstring_append_cstr( buf, fmt );
        if( i != count - 1 ) {
            buf = dstring_append_text( buf, ", " );
        }
    }

    buf = dstring_append_text( buf, " }" );

    va_list va;
    va_start( va, format );
    char* fmt = local_char_buf_fmt_va( THREAD_MAIN, format, va );
    va_end( va );

    cb_info( THREAD_MAIN, "%s %s", buf, fmt );

    dstring_clear( buf );
}

int main( int argc, char** argv ) {
    int* array = darray_empty( sizeof(int), 10 );

    for( usize i = 0; i < 10; ++i ) {
        int item = i + 10;
        darray_try_push( array, &item );
    }

    print_integers( darray_len( array ), array, "(%zu/%zu)", darray_len(array), darray_cap(array) );

    darray_remove( array, 0 );

    print_integers( darray_len( array ), array, "(%zu/%zu)", darray_len(array), darray_cap(array) );

    int new_item = 255;
    darray_try_push( array, &new_item );

    print_integers( darray_len( array ), array, "(%zu/%zu)", darray_len(array), darray_cap(array) );

    darray_remove_range( array, 0, 3 );

    print_integers( darray_len( array ), array, "(%zu/%zu)", darray_len(array), darray_cap(array) );

    array = darray_emplace( array, &new_item, 2 );

    print_integers( darray_len( array ), array, "(%zu/%zu)", darray_len(array), darray_cap(array) );

    darray_free( array );
    return 0;
}

#define CBUILD_IMPLEMENTATION
#include "cbuild_new.h"

