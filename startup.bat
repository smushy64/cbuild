@ECHO OFF

REM change to your own cbuild source file if you named it something else
set source=cbuild.c

REM uncomment the following line to use clang instead
REM clang %source% -o cbuild.exe

cl %source% /Fe:cbuild.exe /nologo

./cbuild.exe

