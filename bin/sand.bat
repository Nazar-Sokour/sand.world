@echo off

@REM This batch file temporary reset PATH environment variable and call sand.exe

set PATHOLD=%PATH%
set PATH=%PATHOLD%;%~dp0..\lib
%~dp0sand.exe %*
set PATH=%PATHOLD%
set PATHOLD=
