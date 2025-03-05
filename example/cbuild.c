/**
 * @file   cbuild.c
 * @brief  C Build example
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
*/
#undef CB_IMPLEMENTATION
#define CB_IMPLEMENTATION 1
#include "cbuild.h"

enum Mode {
    M_HELP,
    M_BUILD,
    M_RUN,

    M_COUNT
};
const char* cstr_from_mode( enum Mode mode );
bool        mode_from_cstr( const char* cstr, enum Mode* out_mode );
const char* mode_description( enum Mode mode );

enum Optimization {
    OPT_NONE,
    OPT_PERF,
    OPT_SPACE,
};
const char* cstr_from_opt( enum Optimization opt );

struct Args {
    enum Mode mode;
    union {
        struct ArgsHelp {
            enum Mode mode;
        } help;
        struct ArgsBuild {
            enum Optimization opt;
            bool              is_debug : 1;
            bool              is_dry   : 1;
        } build;
        struct ArgsRun {
            struct ArgsBuild      build;
            struct CB_CommandLine cl;
        } run;
    };
};
int mode_help( struct Args* args );
int mode_build( struct Args* args );
int mode_run( struct Args* args );

int main( int argc, char** argv ) {
    CB_INITIALIZE( CB_LOG_ALL );

    CB_CommandLine cl = CB_CL( argc, argv );
    cl = CB_CL_NEXT( &cl );

    struct Args args = {};
    if( !cl.len ) {
        return mode_help( NULL );
    }

    if( !mode_from_cstr( cl.buf[0], &args.mode ) ) {
        CB_ERROR( "unrecognized mode '%s'", cl.buf[0] );
        mode_help( NULL );
        return 1;
    }
    cl = CB_CL_NEXT( &cl );

    while( cl.len ) {
        switch( args.mode ) {
            case M_HELP: {
                mode_from_cstr( cl.buf[0], &args.help.mode );
                cl = CB_CL_NEXT( &cl );
            } continue;

            case M_BUILD:
            case M_RUN:  {

                if( strcmp( cl.buf[0], "-opt-perf" ) == 0 ) {
                    args.build.opt = OPT_PERF;
                    goto next_iteration;
                }

                if( strcmp( cl.buf[0], "-opt-space" ) == 0 ) {
                    args.build.opt = OPT_SPACE;
                    goto next_iteration;
                }

                if( strcmp( cl.buf[0], "-debug" ) == 0 ) {
                    args.build.is_debug = true;
                    goto next_iteration;
                }

                if( strcmp( cl.buf[0], "-dry" ) == 0 ) {
                    args.build.is_dry = true;
                    goto next_iteration;
                }

                if( args.mode == M_RUN ) {
                    if( strcmp( cl.buf[0], "--" ) == 0 ) {
                        args.run.cl = CB_CL_NEXT( &cl );
                        goto exit_loop;
                    }
                }
            } break;

            case M_COUNT:
                break;
        }

        CB_ERROR( "unrecognized argument '%s'", cl.buf[0] );
        mode_help( &args );
        return 1;

    next_iteration:
        cl = CB_CL_NEXT( &cl );
    }
exit_loop:

    switch( args.mode ) {
        case M_HELP:  return mode_help( &args );
        case M_BUILD: return mode_build( &args );
        case M_RUN:   return mode_run( &args );
        case M_COUNT:
            break;
    }

    return 0;
}
int mode_help( struct Args* args ) {
    enum Mode mode = M_HELP;
    if( args ) {
        if( args->mode == M_HELP ) {
            mode = args->help.mode;
        } else {
            mode = args->mode;
        }
    }

    printf( "OVERVIEW    Build example project for CBuild.\n" );
    printf( "USAGE       ./cbuild %s [args...]\n", mode == M_HELP ? "<mode>" : cstr_from_mode( mode ) );
    printf( "DESCRIPTION \n" );
    printf( "  %s\n", mode_description( mode ) );
    printf( "ARGUMENTS   \n" );

    switch( mode ) {
        case M_HELP: {
            printf( "  <mode>  Print help for mode.\n" );
            printf( "            valid: " );
            for( enum Mode m = 0; m < M_COUNT; ++m ) {
                printf( "%s", cstr_from_mode( m ) );

                if( (m + 1) < M_COUNT ) {
                    printf( ", " );
                } else {
                    putchar( '\n' );
                }
            }
        } break;
        case M_BUILD:
        case M_RUN: {
            printf( "  -opt-perf   Optimize for performance.\n" );
            printf( "  -opt-space  Optimize for space.\n" );
            printf( "  -debug      Generate debugging symbols.\n" );
            printf( "  -dry        Don't build, just print build command.\n" );
            if( mode == M_RUN ) {
                printf( "  --          Stop parsing arguments and pass them along to project.\n" );
            }
        } break;
        case M_COUNT:
            break;
    }

    return 0;
}
int mode_build( struct Args* args ) {
    if( !cb_make_directories( "build" ) ) {
        return 1;
    }

    if( !args->build.is_dry ) {
        CB_INFO( "building project . . ." );
    }

    double start = cb_time_msec();

    CB_CommandBuilder builder = {};
    cb_command_builder_append( &builder, "clang", "src/main.c", "-Wall", "-Wextra", "-o" );

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    cb_command_builder_append( &builder, "build/project.exe" );
#else
    cb_command_builder_append( &builder, "build/project" );
#endif

    switch( args->build.opt ) {
        case OPT_NONE:
            break;
        case OPT_PERF: {
            cb_command_builder_append( &builder, "-O2" );
        } break;
        case OPT_SPACE: {
            cb_command_builder_append( &builder, "-Os" );
        } break;
    }

    if( args->build.is_debug ) {
        cb_command_builder_append( &builder, "-g" );
    }

    if( args->build.is_dry ) {
        CB_StringBuilder string = {};
        cb_command_flatten( builder.cmd, &string );
        CB_PUSH( &string, 0 );

        CB_INFO( "%s", string.buf );
        CB_FREE( string.buf, string.cap );

        cb_command_builder_free( &builder );
        return 0;
    }

    CB_ProcessID pid = {};
    bool success = cb_process_exec_async( builder.cmd, &pid, NULL, NULL, NULL, NULL, NULL );
    cb_command_builder_free( &builder );

    if( !success ) {
        CB_ERROR( "failed to run build command!" );
        return 1;
    }

    int exit_code = cb_process_wait( &pid );
    if( exit_code ) {
        CB_ERROR( "failed to build project!" );
        return exit_code;
    }

    CB_INFO( "built project in %fms", cb_time_msec() - start );
    return 0;
}
int mode_run( struct Args* args ) {
    int result = 0;
    if( ( result = mode_build( args ) ) ) {
        return result;
    }

    if( !args->build.is_dry ) {
        CB_INFO( "running project . . ." );
    }

    CB_CommandBuilder builder = {};
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    cb_command_builder_append( &builder, "./build/project.exe" );
#else
    cb_command_builder_append( &builder, "./build/project" );
#endif

    for( int i = 0; i < args->run.cl.len; ++i ) {
        cb_command_builder_append( &builder, args->run.cl.buf[i] );
    }

    if( args->build.is_dry ) {
        CB_StringBuilder string = {};
        cb_command_flatten( builder.cmd, &string );

        CB_INFO( "%s", string.buf );

        CB_FREE( string.buf, string.cap );
        cb_command_builder_free( &builder );
        return 0;
    }

    CB_ProcessID pid = {};
    if( !cb_process_exec_async( builder.cmd, &pid, NULL, NULL, NULL, NULL, NULL ) ) {
        return 1;
    }

    int exit_code = cb_process_wait( &pid );
    if( exit_code < 0 ) {
        CB_ERROR( "failed to wait for project.");
        return 1;
    }

    CB_INFO( "project exited with code %i.", exit_code );
    return 0;
}

const char* cstr_from_opt( enum Optimization opt ) {
    switch( opt ) {
        case OPT_NONE:  return "none";
        case OPT_PERF:  return "performance";
        case OPT_SPACE: return "space";
    }
    return "";
}

const char* cstr_from_mode( enum Mode mode ) {
    switch( mode ) {
        case M_HELP:  return "help";
        case M_BUILD: return "build";
        case M_RUN:   return "run";
        case M_COUNT:
            break;
    }
    return "";
}
bool mode_from_cstr( const char* cstr, enum Mode* out_mode ) {
    for( enum Mode m = 0; m < M_COUNT; ++m ) {
        if( strcmp( cstr, cstr_from_mode( m ) ) == 0 ) {
            *out_mode = m;
            return true;
        }
    }
    return false;
}
const char* mode_description( enum Mode mode ) {
    switch( mode ) {
        case M_HELP:
            return "Print help and exit.";
        case M_BUILD:
            return "Build project.";
        case M_RUN:
            return "Build and run project.";
        case M_COUNT:
            break;
    }
    return "";
}


