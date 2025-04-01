<!--*
 * @file   CHANGELOG.md
 * @brief  Change log for cbuild.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 04, 2025
-->

Changelog
=========

Major version 0 means library is in development so anything
might change, including breaking changes.

## Major version
Number between 0-65535.

Indicates breaking changes to API or implementation.

## Minor version
Number between 0-255.

Indicates implementation changes or bug fixes that affect behavior.

## Patch version
Number between 0-255

Indicates additions to API, minor bug fixes and other small changes.

0.3.0
-----
- cb_read_entire_file() removed, replaced with cb_path_read_buffer()
- cb_file_size() and cb_file_read_buffer() implemented.
- cb_process_exec() and cb_process_exec_async() have 
        been turned into variadic macros for easier use.
- cb_rebuild() defines macro CB_BOOTSTRAPPED
- renamed macro CB_STRING_ARG() to CB_STRING_FMT_ARG()

0.2.0
-----
- new structs: CB_CommandLine, CB_Context
- new function prototypes: CB_AllocFN, CB_FreeFN
- new macros functions: CB_CL( argc, argv ), CB_CL_NEXT( cmdline )
- new functions: cb_context_set(), cb_context_get()
- stripped prefix macros for functions redefined to take variadic arguments
- CB_ALLOC and CB_FREE have been redefined to use context allocator
- CB_APPEND: cast count parameter to int to avoid warnings about integer comparison
- platform functions that used CB_ALLOC/FREE now use internal allocator

