# -O0 -g -gcodeview -fuse-ld=lld "-Wl,/debug"
clang main_new.c -o test.exe
./test.exe
