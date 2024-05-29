# -O0 -g -gcodeview -fuse-ld=lld "-Wl,/debug"
Write-Output "building . . ."
clang main_new.c -o test.exe
./test.exe
