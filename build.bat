@echo off
REM @file   build.bat
REM @brief  Windows Batch script bootstrapper for C Build system.
REM
REM @details
REM This is for use with Microsoft Visual C++ toolchain.
REM MSVC environment must be setup with vcvarsall.bat or
REM some other mechanism before running this script.
REM
REM @author Alicia Amarilla (smushyaa@gmail.com)
REM @date   February 27, 2025

IF NOT EXIST .\cbuild.exe (
    ECHO Compiling cbuild . . .

    cl cbuild.c /Fecbuild.exe

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

