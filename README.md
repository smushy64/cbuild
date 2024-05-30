<!--*
 * @file   README.md
 * @brief  README for cbuild command line utility
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
 * @mainpage
-->

C Build
========

Single-header library for building C projects.

Inspired by [nobuild](https://github.com/tsoding/nobuild).

## How-To

- Download cbuild.h.
- Create a cbuild.c file in project root.
- Include cbuild.h at the top and at the bottom of cbuild.c
- Define CBUILD_IMPLEMENTATION before second include.
- Run init() function before anything else in main function.
- Compile cbuild.c using prefered compiler.

Minimal Example:
```C
#include "cbuild.h"

int main( int argc, const char** argv ) {
    init( LOGGER_LEVEL_INFO );
    /*
     * Build code goes here.
    */
    return 0;
}

#define CBUILD_IMPLEMENTATION
#include "cbuild.h"
```
```sh
cc cbuild.c -o cbuild
./cbuild
```

Documentation can be generated using doxygen README.md and provided Doxygen file.

The `startup.bat` and `startup.sh` scripts are included to easily
generate cbuild program.

```
Note: On Windows, startup.bat assumes you have run vcvarsall.bat or are running
in Developer Command Prompt for VS *.
clang is provided as an alternative in the script.
```

## Additional Setup

At the top of cbuild.c:

Define `CBUILD_THREAD_COUNT` with a number from 2-16 to
change how many threads are created by job system.

Define `CBUILD_ASSERTIONS` to enable debug assertions.

Define `CBUILD_ADDTIONAL_FLAGS` with additional compiler flags
thatare used to compile cbuild.

Example:
```C
// flags for enabling debug symbols
#define CBUILD_ADDITIONAL_FLAGS "-g", "-O0"
```

## Requirements

- Windows or any POSIX environment.
- clang, gcc or Visual Studio (cl).
- (optional) Doxygen.

## Links

- [Source](https://github.com/smushy64/cbuild)
- [nobuild](https://github.com/tsoding/nobuild)

