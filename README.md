<!--*
 * @file   README.md
 * @brief  README for cbuild command line utility
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
-->

Cbuild
========

Single-header library for building C projects.

Inspired by [nobuild](https://github.com/tsoding/nobuild).

## How-To

Download cbuild.h and include it once at the top
and again at the bottom with CBUILD_IMPLEMENTATION defined.
The library expects that you only use one source file but if you use
multiple, only define CBUILD_IMPLEMENTATION once.

If you are on windows download startup.bat, otherwise download startup.sh.
After you have a main function that calls init( __log_level__ ), run the startup
script to initialize cbuild. If you wish to use a different compiler,
edit the startup script with your compiler or define COMPILER before
CBUILD_IMPLEMENTATION with your prefered compiler and rebuild (run cbuild.exe again).
When distributing, include either (or both) startup files and source code,
please don't include the compiled binary.

```
Note: If running startup on windows, make sure to vcvarsall or use
Developer Command Prompt for Visual Studio to get cl enviroment setup
before running the script. The environment also needs to be set everytime cbuild is run.
```

This library provides cross-platform functions/structures for
string manipulation, timing, path searching, checking for executables,
a simple job system and atomics for
multithreading and functions for asynchronously running processes.

Use the included memory functions (memory_[alloc, realloc, free])
for keeping track of memory usage.

Includes doxygen documentation so if it's installed,
documentation for cbuild can be generated.

Check the example directory for more info.

## Requirements

- clang, gcc or Visual Studio (cl)

## Links

- [Source](https://github.com/smushy64/cbuild)
- [nobuild](https://github.com/tsoding/nobuild)

