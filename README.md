<!--*
 * @file   README.md
 * @brief  README for cbuild command line utility
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 15, 2024
 * @mainpage
-->

C Build
========
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Single-header library for building C projects.

Inspired by [tsoding's nob.h](https://github.com/tsoding/nob.h).

## How-To

- Download cbuild.h.
- Create a cbuild.c file in project root.
- Include cbuild.h at the top and at the bottom of cbuild.c
- Define CB_IMPLEMENTATION before second include.
- Run CB_INITIALIZE() function before anything else in main function.
- Compile cbuild.c using preferred compiler.

Minimal Example:
```C
#include "cbuild.h"

int main( int argc, const char** argv ) {
    CB_INITIALIZE( CB_LOG_ALL );
    /*
     * Build code goes here.
    */
    return 0;
}

#define CB_IMPLEMENTATION
#include "cbuild.h"
```

```sh
cc cbuild.c -o cbuild
./cbuild
```

Documentation can be generated using doxygen README.md and provided Doxygen file.

The `build.bat` and `build.sh` scripts are included to easily
generate cbuild program.

```
Note: On Windows, build.bat assumes you have run vcvarsall.bat or are running
in Developer Command Prompt for VS *.
clang is provided as an alternative in the script.
```
## Interactive Example

- Compile example project.
```sh
cd example
cc cbuild.c -o cbuild
```
- Run cbuild.
```sh
./cbuild
```

## Requirements

- Windows or any POSIX environment.
- clang, gcc or Visual Studio (cl).
- (optional) Doxygen.

## Generate Documentation

- Run doxygen in root directory.
```sh
doxygen
```
- Result will be in ./doxyout/html
- Open index.html with preferred browser.
```sh
firefox ./doxyout/html/index.html
```

## Links

- [Source](https://github.com/smushy64/cbuild)
- [nob](https://github.com/tsoding/nob.h)

