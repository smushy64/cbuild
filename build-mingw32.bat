REM @file   build-mingw32-clang.bat
REM @brief  Windows Batch script bootstrapper for C Build system.
REM 
REM @details
REM This is for use with gcc or clang mingw32 toolchain.
REM
REM @author Alicia Amarilla (smushyaa@gmail.com)
REM @date   March 01, 2025

REM this can be changed to clang
SET compiler=gcc

IF NOT EXIST .\cbuild.exe (
    ECHO Compiling cbuild using %compiler% . . .

    %compiler% cbuild.c -o cbuild.exe

    IF %ERRORLEVEL% NEQ 0 (
        ECHO Failed to compile cbuild! >&2
        EXIT /b %ERRORLEVEL%
    ) ELSE (
        ECHO Compiled cbuild successfully.
    )
)

IF EXIST .\cbuild.exe (
    .\cbuild.exe %*
)

