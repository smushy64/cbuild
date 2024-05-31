/**
 * @file   cbuild.c
 * @brief  C Build example
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
*/
#undef _CLANGD
#define CBUILD_THREAD_COUNT 16
#include "../cbuild.h" // including the header from root

static const char* global_program_name = "cbuild";

typedef enum {
    MODE_NULL,
    MODE_BUILD,
    MODE_REBUILD,
    MODE_RUN,
    MODE_CLEAN,
    MODE_HELP,
} Mode;
void print_help( Mode mode );

b32 mode_build(
    string target, string output,
    b32 output_time, b32 dry_run, dstring* path );
b32 mode_clean( string output );

int main( int argc, const char** argv ) {
    global_program_name = argv[0];

    b32 silent = false;
    for( int i = 1; i < argc; ++i ) {
        const char* arg = argv[i];
        if( arg[0] == '-' ) {
            if( cstr_cmp( arg, "--silent" ) ) {
                silent = true;
                break;
            }
        }
    }

    init( silent ? LOGGER_LEVEL_ERROR : LOGGER_LEVEL_INFO );

    Mode mode = MODE_NULL;
    string target =
#if defined(PLATFORM_WINDOWS)
        string_text( "program.exe" );
#else
        string_text( "program" );
#endif
    string output  = string_text( "./build" );
    b32 print_time = true;
    b32 dry_run    = false;

    int start_args = -1;

    for( int i = 1; i < argc; ++i ) {
        string arg = string_from_cstr( argv[i] );

        switch( mode ) {
            case MODE_NULL: {
                if( string_cmp( arg, string_text( "build" ) ) ) {
                    mode = MODE_BUILD;
                    continue;
                }
                if( string_cmp( arg, string_text( "run" ) ) ) {
                    mode = MODE_RUN;
                    continue;
                }
                if( string_cmp( arg, string_text( "clean" ) ) ) {
                    mode = MODE_CLEAN;
                    continue;
                }
                if( string_cmp( arg, string_text( "rebuild" ) ) ) {
                    mode = MODE_REBUILD;
                    continue;
                }
                if(
                    string_cmp( arg, string_text( "help" ) ) ||
                    string_cmp( arg, string_text( "--help" ) )
                ) {
                    print_help( MODE_HELP );
                    return 0;
                }
                if( string_cmp( arg, string_text( "--silent" ) ) ) {
                    continue;
                }
            } break;
            case MODE_CLEAN: {
                if( arg.cc[0] != '-' ) {
                    break; // break out of switch
                }
                string output_arg = string_text( "--out=" );

                if( string_cmp_clamped( arg, output_arg ) ) {
                    string path = string_adv_by( arg, output_arg.len );
                    if( !path.len ) {
                        cb_error( "--out=: no path provided!" );
                        print_help( mode );
                        return -1;
                    }

                    output = path;
                    continue;
                }
                if( string_cmp( arg, string_text( "--silent" ) ) ) {
                    continue;
                }
                if( string_cmp( arg, string_text( "--help" ) ) ) {
                    print_help( mode );
                    return 0;
                }
            } break;
            case MODE_BUILD:
            case MODE_RUN: {
                if( arg.cc[0] != '-' ) {
                    break; // break out of switch
                }
                if( mode == MODE_RUN ) {
                    if( string_cmp( arg, string_text( "--" ) ) ) {
                        start_args = i + 1;
                        break;
                    }
                } else { // MODE_BUILD
                    if( string_cmp( arg, string_text( "--dryrun" ) ) ) {
                        dry_run = true;
                        continue;
                    }
                }

                string output_arg = string_text( "--out=" );
                string target_arg = string_text( "--target=" );

                if( string_cmp_clamped( arg, output_arg ) ) {
                    string path = string_adv_by( arg, output_arg.len );
                    if( !path.len ) {
                        cb_error( "--out=: no path provided!" );
                        print_help( mode );
                        return -1;
                    }

                    output = path;
                    continue;
                }
                if( string_cmp_clamped( arg, target_arg ) ) {
                    string path = string_adv_by( arg, target_arg.len );
                    if( !path.len ) {
                        cb_error( "--target=: no path provided!" );
                        print_help( mode );
                        return -1;
                    }

                    target = path;
                    continue;
                }

                if( string_cmp( arg, string_text( "--notime" ) ) ) {
                    print_time = false;
                    continue;
                }
                if( string_cmp( arg, string_text( "--silent" ) ) ) {
                    continue;
                }
                if( string_cmp( arg, string_text( "--help" ) ) ) {
                    print_help( mode );
                    return 0;
                }
            } break;
            case MODE_HELP: {
                print_help( MODE_HELP );
                return 0;
            } break;
            case MODE_REBUILD: {
                if( string_cmp( arg, string_text( "--help" ) ) ) {
                    print_help( MODE_REBUILD );
                    return 0;
                }
            };
        }

        if( start_args != -1 ) {
            break;
        }
        cb_error( "unrecognized argument '%s'", arg.cc );
        print_help( mode );
        return -1;
    }

    switch( mode ) {
        case MODE_NULL:
        case MODE_HELP: {
            print_help( mode );
            return 0;
        } break;
        case MODE_REBUILD: {
            cbuild_rebuild( __FILE__, argv[0], false );
            return 0;
        } break;
        default: break;
    }

    dstring* target_path = dstring_concat_multi(
        2, (string[]){ output, target }, string_text( "/" ) );
    expect( target_path, "failed to create target path!" );

    switch( mode ) {
        case MODE_BUILD: {
            if( !mode_build( target, output, print_time, dry_run, target_path ) ) {
                return -1;
            }
        } break;
        case MODE_RUN: {
            if( !mode_build( target, output, print_time, false, target_path ) ) {
                return -1;
            }

            CommandBuilder builder;
            memory_zero( &builder, sizeof(builder) );
            expect(
                command_builder_new( target_path, &builder ),
                "failed to create command builder!" );

            if( start_args != -1 ) {
                for( int i = start_args; i < argc; ++i ) {
                    const char* arg = argv[i];

#if defined(PLATFORM_WINDOWS)
                    if( cstr_find( arg, ' ', 0 ) ) {
                        arg = local_fmt( "\"%s\"", arg );
                    }
#endif /* PLATFORM_WINDOWS */

                    expect(
                        command_builder_push( &builder, arg ), "failed to push arg!" );
                }
            }

            Command cmd      = command_builder_cmd( &builder );
            const char* flat = command_flatten_local( &cmd );

            cb_info( "%s", flat );

            PID pid = process_exec( cmd, false, 0, 0, 0 );
            int res = process_wait( pid );

            cb_info( "program '%s' exited with code %i", target_path, res );

            command_builder_free( &builder );
        } break;
        case MODE_CLEAN: {
            if( !mode_clean( output ) ) {
                return -1;
            }
        } break;
        case MODE_REBUILD:
        case MODE_NULL:
        case MODE_HELP: unreachable();
    }

    return 0;
}
b32 mode_clean( string output ) {
    if( !path_exists( output.cc ) ) {
        cb_info( "nothing to clean!" );
        return true;
    }

    cb_info( "removing output path %s . . .", output.cc );
    if( !path_is_directory( output.cc ) ) {
        cb_error( "clean: output path '%s' is not a directory!", output.cc );
        print_help( MODE_CLEAN );
        return false;
    }

    if( !dir_remove( output.cc, true ) ) {
        return false;
    }

    return true;
}
static CommandBuilder builder;
static b32            builder_ready = false;
Command get_build_command( string compiler, const cstr* output_path ) {
    if( !builder_ready ) {
        memory_zero( &builder, sizeof(builder) );
        if( !command_builder_new( compiler.cc, &builder ) ) {
            panic( "failed to create command builder!" );
        }

        #define cmd_push( builder, arg ) do {\
            if( !command_builder_push( builder, arg ) ) {\
                panic( "failed to push command argument!" );\
            }\
        } while(0)

        cmd_push( &builder, "./src/main.c" );
#if defined(PLATFORM_WINDOWS)
        if( string_cmp( compiler, string_text( "cl" ) ) ) {
            cmd_push( &builder, "-Fe:" );
        } else {
            cmd_push( &builder, "-o" );
        }
#else
        cmd_push( &builder, "-o" );
#endif
        cmd_push( &builder, output_path );
        cmd_push( &builder, "-DCBUILD_MESSAGE=\"" "hello, from cbuild" "\"" );

        builder_ready = true;
    }

    return command_builder_cmd( &builder );
}
b32 build( string compiler, dstring* path ) {
    Command cmd = get_build_command( compiler, path );

    PID pid = process_exec( cmd, false, 0, 0, 0 );
    int res = process_wait( pid );

    if( res != 0 ) {
        cb_error( "failed to rebuild project!" );
        return false;
    }

    #undef cmd_push
    return true;
}

struct SplitGlobParams {
    const WalkDirectory* wd;
    string  glob;
    string* list;
};

void job_split_glob( void* params ) {
    struct SplitGlobParams* p = params;

    p->list = path_walk_glob_filter( p->wd, p->glob );
}

b32 mode_build(
    string target, string output, b32 output_time, b32 dry_run, dstring* path
) {
    string compiler = cbuild_query_compiler();

    if( dry_run ) {
        cb_info( "compiler:   '%s'", compiler.cc );
        cb_info( "output dir: '%s'", output.cc );
        cb_info( "target:     '%s'", target.cc );
        cb_info( "path:       '%s'", path );
        Command cmd = get_build_command( compiler, path );

        const char* flat = command_flatten_local( &cmd );
        cb_info( "build command: %s", flat );
        return true;
    }

    f64 start;
    if( output_time ) {
        start = timer_milliseconds();
    }
    
    if( !process_in_path( compiler.cc ) ) {
        cb_error( "compiler '%s' was not found in path!", compiler.cc );
#if defined(PLATFORM_WINDOWS)
        if( string_cmp( compiler, string_text( "cl" ) ) ) {
            cb_error(
                "run vcvarsall.bat or run cbuild "
                "in Developer Command Prompt for Visual Studio?" );
        }
#endif
        return false;
    }

    if( !path_exists( output.cc ) ) {
        if( !dir_create( output.cc ) ) {
            cb_error( "failed to create output directory '%s'!", output.cc );
            return false;
        }
    }

    if( !path_exists( "./src/main.c" ) ) {
        cb_error( "main file missing!" );
        return false;
    }

    b32 needs_rebuild = false;

    if( path_exists( path ) ) {
        WalkDirectory walk;
        memory_zero( &walk, sizeof(walk) );

        if( !path_walk_dir( "./src", true, false, &walk ) ) {
            cb_error(
                "failed to walk source directory! "
                "maybe cbuild is not being run from project root?" );
            return false;
        }

        struct SplitGlobParams s, h;
        memory_zero( &s, sizeof(s) );
        memory_zero( &h, sizeof(s) );

        s.wd   = &walk;
        s.glob = string_text( "*.c" );
        h.wd   = &walk;
        h.glob = string_text( "*.h" );

        fence();
        job_enqueue( job_split_glob, &s );
        job_enqueue( job_split_glob, &h );

        time_t path_time = file_query_time_modify( path );

        job_wait_all( MT_WAIT_INFINITE );

        if( !s.list ) {
            cb_error( "failed to gather sources!" );
            return false;
        }
        if( !h.list ) {
            cb_error( "failed to gather headers!" );
            return false;
        }

        string* sources = s.list;
        string* headers = s.list;

        for( usize i = 0; i < darray_len( sources ); ++i ) {
            time_t source_time = file_query_time_modify( sources[i].cc );
            f64    diff = difftime( path_time, source_time );

            if( diff < 0.0 ) {
                needs_rebuild = true;
                goto rebuild_check_end;
            }
        }

        for( usize i = 0; i < darray_len( headers ); ++i ) {
            time_t header_time = file_query_time_modify( headers[i].cc );
            f64    diff = difftime( path_time, header_time );

            if( diff < 0.0 ) {
                needs_rebuild = true;
                goto rebuild_check_end;
            }
        }

rebuild_check_end:
        darray_free( s.list );
        darray_free( h.list );
        path_walk_free( &walk );
    } else {
        needs_rebuild = true;
    }

    if( needs_rebuild ) {
        cb_info( "compiler:   '%s'", compiler.cc );
        cb_info( "output dir: '%s'", output.cc );
        cb_info( "target:     '%s'", target.cc );
        cb_info( "path:       '%s'", path );

        b32 res = build( compiler, path );

        if( res && output_time ) {
            f64 end = timer_milliseconds();
            cb_info( "rebuild completed in %fms", end - start );
        }

        return res;
    }

    return true;
}

void print_help( Mode mode ) {
    printf( "OVERVIEW:  C Build Example\n" );
    switch( mode ) {
        case MODE_BUILD: {
            printf( "USAGE:     %s build [args]\n", global_program_name );
            printf( "DESCRIPTION:\n" );
            printf( "  Build program and exit.\n" );
            printf( "ARGUMENTS: \n" );
            printf( "  --dryrun        Only print configuration.\n" );
            printf( "  --out=<path>    Set output directory.\n" );
            printf( "  --target=<path> Set output name.\n" );
            printf( "  --notime        Do not print how long build took.\n" );
            printf( "  --silent        Only print error and fatal messages to stderr.\n" );
            printf( "  --help          Print this message and exit.\n" );
        } break;
        case MODE_RUN: {
            printf( "USAGE:     %s run [args]\n", global_program_name );
            printf( "DESCRIPTION:\n" );
            printf( "  Build program and execute it.\n" );
            printf( "ARGUMENTS: \n" );
            printf( "  --              End cbuild arguments. After this, arguments are passed to compiled program.\n" );
            printf( "  --out=<path>    Set output directory.\n" );
            printf( "  --target=<path> Set output name.\n" );
            printf( "  --notime        Do not print how long build took.\n" );
            printf( "  --silent        Only print error and fatal messages to stderr.\n" );
            printf( "  --help          Print this message and exit.\n" );
        } break;
        case MODE_CLEAN: {
            printf( "USAGE:     %s clean [args]\n", global_program_name );
            printf( "DESCRIPTION:\n" );
            printf( "  Clean output directory.\n" );
            printf( "ARGUMENTS: \n" );
            printf( "  --out=<path>    Set output directory.\n" );
            printf( "  --silent        Only print error and fatal messages to stderr.\n" );
            printf( "  --help          Print this message and exit.\n" );
        } break;
        case MODE_REBUILD: {
            printf( "USAGE:     %s rebuild [args]\n", global_program_name );
            printf( "DESCRIPTION:\n" );
            printf( "  Force cbuild to rebuild itself and exit.\n" );
        } break;
        case MODE_NULL:
        case MODE_HELP: {
            printf( "USAGE:     %s <mode> [args]\n", global_program_name );
            printf( "MODES:\n" );
            printf( "  build   Build program and exit.\n" );
            printf( "  run     Build program and execute it.\n" );
            printf( "  rebuild Force cbuild to rebuild itself and exit.\n" );
            printf( "  clean   Clean output directory.\n" );
            printf( "  help    Print this message and exit.\n" );
            printf( "ARGUMENTS:\n" );
            printf( "  --silent Only print error and fatal messages to stderr.\n" );
            printf( "  --help   Print help for given mode.\n" );
        } break;
    }
}

#define CBUILD_IMPLEMENTATION
#include "../cbuild.h"
