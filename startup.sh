#!/bin/sh

# change to your own cbuild source file if you named it something else
source=cbuild.c

# uncomment the following line to use clang
# clang $source -o cbuild -O2

cc $source -o cbuild

./cbuild

