TODO
=====

- [ ] some way to pass custom arguments to rebuild command
- [ ] builtin way to generate config files
    - [ ] simple .ini parser
    - [ ] generate a bash script to launch cbuild
- [ ] pkg-config helper

BUGS
-----

- [ ] win32: process_exec chdir doesn't actually change directory in some cases?

DONE
-----

- [x] cb_file_size( CB_File* ) -> uintptr_t
- [x] cb_file_read_string( CB_File*, CB_StringBuilder* ) -> bool
    - read into a string builder
- [x] posix: cbuild_rebuild doesn't replace exe
- [x] arbitrarily set dstring len

