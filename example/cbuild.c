/**
 * @file   cbuild.c
 * @brief  C Build example
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
*/
#define CBUILD_THREAD_COUNT 16
#include "cbuild.h" // including the header from root

typedef enum Result {
    RESULT_SUCCESS,
    RESULT_ERROR_UNKNOWN,
    RESULT_ERROR_PARSE_MODE,
    RESULT_ERROR_PARSE_ARG,
    RESULT_ERROR_CLEAN_REMOVE_DIR,
    RESULT_ERROR_BUILD_CREATE_DIR,
    RESULT_ERROR_BUILD_COMPILE,
} Result;

typedef enum Mode {
    MODE_HELP,
    MODE_BUILD,
    MODE_RUN,
    MODE_CLEAN,

    MODE_COUNT
} Mode;

String mode_to_str( Mode mode );
String mode_description( Mode mode );
b32 mode_from_str( String string, Mode* out_mode );

#define DEFAULT_BUILD_DIR "./build"
#if defined(PLATFORM_WINDOWS)
    #define PROGRAM_EXT   ".exe"
#else
    #define PROGRAM_EXT   ""
#endif

#if defined(COMPILER_MSVC)
    #define ARG_OUT    "-nologo", "-Fe:"
    #define ARG_OPT    "-O2"
    #define ARG_NO_OPT "-Od"
    #define ARG_SYMB   "-link", "-debug"
#else
    #define ARG_OUT    "-o"
    #define ARG_OPT    "-O3"
    #define ARG_NO_OPT "-O0"
    #define ARG_SYMB   "-g"
#endif

#if defined(PLATFORM_MINGW) && !defined(COMPILER_MSVC)
    #undef  ARG_SYMB
    #define ARG_SYMB "-g", "-gcodeview", "-fuse-ld=lld", "-Wl,/debug"
#endif

#if defined(PLATFORM_WINDOWS)
    #define defstring( macro, string ) "-D" #macro "=\\\"" string "\\\"" 
#else
    #define defstring( macro, string ) "-D" #macro "=\"\\\"" string "\"\\\"" 
#endif

#define PROGRAM_NAME  "test-program" PROGRAM_EXT

typedef struct ParsedArgs {
    Mode mode;
    union {
        struct HelpArgs {
            Mode mode;
        } help;
        struct BuildArgs {
            const char* out_dir;
            b32 release;
            b32 strip_symbols;
            b32 dry;
            b32 no_time;
        } build;
        struct RunArgs {
            struct BuildArgs build;
            int start_args;
            int argc;
            const char** argv;
        } run;
        struct CleanArgs {
            const char* dir;
        } clean;
    };
} ParsedArgs;

int mode_help( ParsedArgs* args );
int mode_build( struct BuildArgs* args );
int mode_run( struct RunArgs* args );
int mode_clean( struct CleanArgs* args );

int main( int argc, const char** argv ) {
    init( LOGGER_LEVEL_INFO );

    if( argc <= 1 ) {
        return mode_help( NULL );
    }

    ParsedArgs parsed_args = {0};

    String arg = string_from_cstr( argv[1] );
    if( !mode_from_str( arg, &parsed_args.mode ) ) {
        cb_error( "unrecognized mode '%s'", arg.cc );
        mode_help( NULL );
        return RESULT_ERROR_PARSE_MODE;
    }

    if( parsed_args.mode == MODE_HELP ) {
        if( argc <= 2 ) {
            mode_help( NULL );
            return RESULT_SUCCESS;
        }
        arg = string_from_cstr( argv[2] );
        if( mode_from_str( arg, &parsed_args.help.mode ) ) {
            mode_help( &parsed_args );
            return RESULT_SUCCESS;
        } else {
            mode_help( NULL );
            return RESULT_SUCCESS;
        }
    }

    b32 break_loop = false;
    for( int i = 2; i < argc && !break_loop; ++i ) {
        String arg = string_from_cstr( argv[i] );

        if( string_is_empty( arg ) ) {
            continue;
        }

        if( arg.cc[0] != '-' ) {
            goto parse_args_end;
        }

        switch( parsed_args.mode ) {
            case MODE_BUILD:
            case MODE_RUN: {
                if( string_cmp( arg, string_text("-o"))) {
                    i++;
                    if( i >= argc ) {
                        cb_error( "argument -o requires a path after it!" );
                        mode_help( &parsed_args );
                        return RESULT_ERROR_PARSE_ARG;
                    }
                    parsed_args.build.out_dir = argv[i];
                    continue;
                }
                if( string_cmp( arg, string_text("-release"))) {
                    parsed_args.build.release = true;
                    continue;
                }
                if( string_cmp( arg, string_text("-strip-symbols"))) {
                    parsed_args.build.strip_symbols = true;
                    continue;
                }
                if( string_cmp( arg, string_text("-no-time"))) {
                    parsed_args.build.no_time = true;
                    continue;
                }
                if( string_cmp( arg, string_text("-dry"))) {
                    parsed_args.build.dry = true;
                    continue;
                }
            } break;
            case MODE_CLEAN:
            case MODE_COUNT:
            case MODE_HELP: break;
        }

        switch( parsed_args.mode ) {
            case MODE_RUN: {
                if( string_cmp( arg, string_text( "--"))) {
                    parsed_args.run.start_args = i + 1;
                    parsed_args.run.argc       = argc;
                    parsed_args.run.argv       = argv;

                    break_loop = true;
                    continue;
                }
            } break;

            case MODE_HELP:
            case MODE_BUILD:
            case MODE_CLEAN:
            case MODE_COUNT: break;
        }

        switch( parsed_args.mode ) {
            case MODE_CLEAN: {
                if( string_cmp( arg, string_text("-d"))) {
                    i++;
                    if( i >= argc ) {
                        cb_error( "argument -d requires a path after it!" );
                        mode_help( &parsed_args );
                        return RESULT_ERROR_PARSE_ARG;
                    }
                    parsed_args.clean.dir = argv[i];
                    continue;
                }
            } break;
            case MODE_HELP:
            case MODE_BUILD:
            case MODE_RUN:
            case MODE_COUNT: break;
        }

parse_args_end:
        cb_error( "unrecognized argument '%s'", arg.cc );
        mode_help( &parsed_args );
        return RESULT_ERROR_PARSE_ARG;
    }

    switch( parsed_args.mode ) {
        case MODE_HELP:  return mode_help( &parsed_args );
        case MODE_BUILD: return mode_build( &parsed_args.build );
        case MODE_RUN:   return mode_run( &parsed_args.run );
        case MODE_CLEAN: return mode_clean( &parsed_args.clean );
        case MODE_COUNT: return RESULT_ERROR_UNKNOWN;
    }
}

DString* path_join( DString* path, String other ) {
    String path_str = string_from_dstring( path );
    char last = *string_last( path_str );
#if defined(PLATFORM_WINDOWS)
    if( last != '/' || last != '\\' ) {
        path = dstring_push( path, '/' );
    }
#else
    if( last != '/' ) {
        path = dstring_push( path, '/' );
    }
#endif
    return dstring_append( path, other );
}

int mode_build( struct BuildArgs* args ) {
    f64 start = timer_milliseconds();

    String dir = string_empty();
    if( args->out_dir ) {
        dir = string_from_cstr( args->out_dir );
    } else {
        dir = string_text( DEFAULT_BUILD_DIR );

        if( !args->dry && !path_exists( DEFAULT_BUILD_DIR) ) {
            cb_info( "build: creating out directory . . .");
            if( !dir_create( DEFAULT_BUILD_DIR ) ) {
                cb_error( "build: failed to create output directory!" );
                return RESULT_ERROR_BUILD_CREATE_DIR;
            }
        }
    }

    DString* output = dstring_from_string( dir );
    output = path_join( output, string_text( PROGRAM_NAME ) );

    const char* compiler = cbuild_query_compiler().cc;

    CommandBuilder builder;
    command_builder_new( compiler, &builder );
    #define push( ... )\
        command_builder_append( &builder, __VA_ARGS__ )

    push( "src/main.c" );

    push( ARG_OUT, output );

    if( args->release ) {
        push( ARG_OPT );
    } else {
        push( ARG_NO_OPT );
    }

    push( defstring( CBUILD_MESSAGE, "hello, world!" ) );

    if( args->strip_symbols ) {
    } else {
        push( ARG_SYMB );
    }


    Command cmd = command_builder_cmd( &builder );

    if( args->dry ) {
        DString* flat = command_flatten_dstring( &cmd );
        command_builder_free( &builder );

        cb_info( "build: %s", flat );
        dstring_free( flat );
        dstring_free( output );
        return RESULT_SUCCESS;
    }

    PID pid = process_exec( cmd, false, NULL, NULL, NULL, NULL );
    dstring_free( output );
    command_builder_free( &builder );

    int res = process_wait( pid );

    if( res ) {
        cb_error( "build: failed to build! compiler exited with code %i", res );
        return RESULT_ERROR_BUILD_COMPILE;
    }

    f64 end = timer_milliseconds();
    if( !args->no_time ) {
        cb_info( "build: completed in %.2fms", end - start );
    }

    #undef push
    return RESULT_SUCCESS;
}
int mode_run( struct RunArgs* args ) {
    int build_res = mode_build( &args->build );
    if( build_res != RESULT_SUCCESS ) {
        return build_res;
    }

    String dir = string_empty();
    if( args->build.out_dir ) {
        dir = string_from_cstr( args->build.out_dir );
    } else {
        dir = string_text( DEFAULT_BUILD_DIR );
    }

    DString* program = dstring_from_string( dir );
    program = path_join( program, string_text( PROGRAM_NAME ) );

    CommandBuilder builder;
    command_builder_new( program, &builder );

    if( args->argv ) {
        for( int i = args->start_args; i < args->argc; ++i ) {
            command_builder_push( &builder, args->argv[i] );
        }
    }

    Command cmd = command_builder_cmd( &builder );
    if( args->build.dry ) {
        DString* flat = command_flatten_dstring( &cmd );
        cb_info( "run: %s", flat );
        command_builder_free( &builder );
        dstring_free( flat );
        return RESULT_SUCCESS;
    }

    PID pid = process_exec( cmd, false, NULL, NULL, NULL, NULL );
    int res = process_wait( pid );

    cb_info( "run: program exited with code %i", res );
    return RESULT_SUCCESS;
}
int mode_clean( struct CleanArgs* args ) {
    const char* dir = args->dir;
    if( !dir ) {
        dir = DEFAULT_BUILD_DIR;
    }
    if( !path_exists( dir ) ) {
        cb_info( "clean: nothing to do" );
        return RESULT_SUCCESS;
    }

    if( dir_remove( dir, true ) ) {
        cb_info( "clean: dir '%s' removed.", dir );
    } else {
        cb_error( "clean: failed to remove directory '%s'!", dir );
        return RESULT_ERROR_CLEAN_REMOVE_DIR;
    }

    return RESULT_SUCCESS;
}

int mode_help( ParsedArgs* args ) {
    Mode mode;
    if( !args ) {
        mode = MODE_HELP;
    } else {
        if( args->mode == MODE_HELP ) {
            mode = args->help.mode;
        } else {
            mode = args->mode;
        }
    }
    printf( "OVERVIEW:    Cbuild example.\n" );
    printf( "USAGE:       ./cbuild %s [args]\n",
        mode == MODE_HELP ? "<mode>" : mode_to_str( mode ).cc );


    printf( "DESCRIPTION:\n" );
    printf( "  %s\n", mode_description( mode ).cc );
    printf( "ARGUMENTS:   \n");
    switch( mode ) {
        case MODE_HELP: {
            printf( "  <mode>           Name of mode to run in.\n");
            printf( "                     valid: ");
            for( Mode mode_i = 0; mode_i < MODE_COUNT; ++mode_i ) {
                printf( "%s", mode_to_str( mode_i ).cc );
                if( mode_i + 1 < MODE_COUNT ) {
                    printf( ", " );
                } else {
                    printf( "\n" );
                }
            }
            printf( "  help <mode:opt>  Print help for mode. ( default = help )\n");
            printf( "                     valid: ");
            for( Mode mode_i = 0; mode_i < MODE_COUNT; ++mode_i ) {
                printf( "%s", mode_to_str( mode_i ).cc );
                if( mode_i + 1 < MODE_COUNT ) {
                    printf( ", " );
                } else {
                    printf( "\n" );
                }
            }
        } break;
        case MODE_BUILD: {
            printf( "  -o <dir>        Set output directory. ( default = build )\n" );
            printf( "                    Note: only creates directory if dir == 'build'\n" );
            printf( "  -release        Build in release mode.\n");
            printf( "  -strip-symbols  Strip debug symbols.\n");
            printf( "  -no-time        Don't print time it took to build.\n" );
            printf( "  -dry            Print configuration and exit.\n");
        } break;
        case MODE_RUN: {
            printf( "  -o <dir>        Set output directory. ( default = build )\n" );
            printf( "                    Note: only creates directory if dir == 'build'\n" );
            printf( "  -release        Build in release mode.\n");
            printf( "  -strip-symbols  Strip debug symbols.\n");
            printf( "  -no-time        Don't print time it took to build.\n" );
            printf( "  -dry            Print configuration and exit.\n");
            printf( "  --              Stop parsing cbuild arguments. Remaining arguments are passed to project.\n");
        } break;
        case MODE_CLEAN: {
            printf( "  -d <dir>  Set directory to clean. ( default = build )\n" );
        } break;

        case MODE_COUNT:
          break;
    }

    return 0;
}

String mode_to_str( Mode mode ) {
    switch( mode ) {
        case MODE_HELP  : return string_text( "help" );
        case MODE_BUILD : return string_text( "build" );
        case MODE_RUN   : return string_text( "run" );
        case MODE_CLEAN : return string_text( "clean" );

        case MODE_COUNT : return string_text( "Unknown" );
    }
    return string_text( "Unknown" );
}
String mode_description( Mode mode ) {
    switch( mode ) {
        case MODE_HELP  : return string_text( "Print help for mode and exit." );
        case MODE_BUILD : return string_text( "Build project." );
        case MODE_RUN   : return string_text( "Build and then run project." );
        case MODE_CLEAN : return string_text( "Clean build directory." );

        case MODE_COUNT : return string_text( "Unknown" );
    }
    return string_text( "Unknown" );
}
b32 mode_from_str( String string, Mode* out_mode ) {
    for( Mode mode = 0; mode < MODE_COUNT; ++mode ) {
        String mode_str = mode_to_str( mode );
        if( string_cmp( mode_str, string ) ) {
            *out_mode = mode;
            return true;
        }
    }
    return false;
}

#define CBUILD_IMPLEMENTATION
#define CBUILD_ADDITIONAL_FLAGS "-I.."
#include "cbuild.h"
