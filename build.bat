@echo off
setlocal

rem Toolchain installed for this project:
rem   Qt 5.15.2 (MinGW 8.1, 64-bit)  -> D:\Qt\5.15.2\mingw81_64
rem   MinGW 8.1.0                    -> D:\Qt\Tools\mingw810_64
rem   OpenSSL 1.1.1w (x64)           -> D:\OpenSSL-Win64
set "QT_DIR=D:\Qt\5.15.2\mingw81_64"
set "MINGW_DIR=D:\Qt\Tools\mingw810_64"
set "OPENSSL_DIR=D:\OpenSSL-Win64"

set "PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%OPENSSL_DIR%\bin;%PATH%"

set "BUILD_TYPE=%~1"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=debug"

if not exist "%~dp0build" mkdir "%~dp0build"
pushd "%~dp0build"

echo [1/2] Running qmake (%BUILD_TYPE%)...
qmake "%~dp0LogisticsInformationSystem.pro" -spec win32-g++ "CONFIG+=%BUILD_TYPE%"
if errorlevel 1 goto :fail

echo [2/2] Building with mingw32-make...
mingw32-make -j%NUMBER_OF_PROCESSORS%
if errorlevel 1 goto :fail

popd
echo.
echo Build finished: %~dp0build\%BUILD_TYPE%\LogisticsInformationSystem.exe
exit /b 0

:fail
popd
echo.
echo Build failed.
exit /b 1
