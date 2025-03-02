#!/bin/bash
# @file   build-mingw32.sh
# @brief  Bash script bootstrapper for C Build system.
# 
# @details
# This is for mingw32 toolchain on linux.
#
# @author Alicia Amarilla (smushyaa@gmail.com)
# @date   February 27, 2025

# IMPORTANT(alicia): 
# x86_64-w64-mingw32-gcc and wine are required.

COMPILER=x86_64-w64-mingw32-gcc

if [ ! -f "./cbuild.exe" ]; then
    echo "Compiling cbuild . . ."

    ${COMPILER} cbuild.c -o cbuild.exe

    exit_code=$?
    if [ $exit_code -ne 0 ]; then
        echo "Failed to compile cbuild!" >&2
        exit $exit_code
    fi
    echo "Compiled cbuild successfully."
fi

wine ./cbuild.exe "$@"

