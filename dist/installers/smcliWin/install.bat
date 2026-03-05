@echo off
setlocal enabledelayedexpansion

:: SmCLI Installer for Windows
:: ------------------------------

echo ----------------------------------------
echo SmCLI Installer for Windows
echo ----------------------------------------

:: 1. Check for g++
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] G++ compiler (MinGW) not found!
    echo Please install MinGW and add it to your PATH.
    pause
    exit /b 1
)

:: 2. Build from root
pushd "%~dp0..\..\.."
echo Building SmCLI...
if not exist bin mkdir bin

:: Compile all source files in src directory
set "SRCS="
for %%f in (src\*.cpp) do (
    set "SRCS=!SRCS! %%f"
)

g++ -std=c++17 -Iinclude -Wall -Wextra -O3 -o bin\smcli.exe !SRCS!

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed!
    popd
    pause
    exit /b 1
)

:: 3. Install binary
echo Installing smcli.exe...
copy bin\smcli.exe C:\Windows\System32\smcli.exe >nul 2>nul

if %ERRORLEVEL% neq 0 (
    echo [WARNING] Permission denied to C:\Windows\System32.
    echo Please run this script as Administrator OR:
    echo 1. Add %cd%\bin to your System PATH environment variable manually.
) else (
    echo [SUCCESS] SmCLI installed successfully!
)

popd
echo.
echo Try typing: smcli help
echo ----------------------------------------
pause
