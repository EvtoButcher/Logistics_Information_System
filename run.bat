@echo off
setlocal

rem Keep these paths in sync with build.bat
set "QT_DIR=D:\Qt\5.15.2\mingw81_64"
set "MINGW_DIR=D:\Qt\Tools\mingw810_64"
set "OPENSSL_DIR=D:\OpenSSL-Win64"

set "PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%OPENSSL_DIR%\bin;%PATH%"

set "BUILD_TYPE=%~1"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=debug"

set "APP=%~dp0build\%BUILD_TYPE%\LogisticsInformationSystem.exe"
if not exist "%APP%" (
    echo Application is not built yet.
    echo Run build.bat first.
    exit /b 1
)

cd /d "%~dp0"
start "" "%APP%"
