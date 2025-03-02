#!/bin/bash
# @file   build.sh
# @brief  Bash script bootstrapper for C Build system.
# @author Alicia Amarilla (smushyaa@gmail.com)
# @date   February 27, 2025

# This can also be set to cc or clang
COMPILER=gcc

if [ ! -f "./cbuild" ]; then
    echo "Compiling cbuild using ${COMPILER} . . ."

    ${COMPILER} cbuild.c -o cbuild

    exit_code=$?
    if [ $exit_code -ne 0 ]; then
        echo "Failed to compile cbuild!" >&2
        exit $exit_code
    fi
    echo "Compiled cbuild successfully."
fi

./cbuild "$@"

