@echo off
setlocal

:: SmCLI Installer for Windows
:: ------------------------------

echo ----------------------------------------
echo SmCLI Installer for Windows
echo ----------------------------------------

:: 1. Check for g++
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] G++ compiler (MinGW) not found!
    echo Please install MinGW or a similar C++ compiler and add it to your PATH.
    pause
    exit /b 1
)

:: 2. Build from root
cd ..\..\..
echo Building SmCLI...
if not exist bin mkdir bin

:: Compile all source files
:: Using a simple loop or finding files (Windows style)
set "SRCS="
for %%f in (src\*.cpp) do set SRCS=!SRCS! %%f

:: Set local variable expansion for the loop
setlocal enabledelayedexpansion
set "SRCS="
for %%f in (src\*.cpp) do set "SRCS=!SRCS! %%f"

g++ -std=c++17 -Iinclude -Wall -Wextra -O3 -o bin\smcli.exe !SRCS!

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

:: 3. Install binary
echo Installing smcli.exe to C:\Windows\System32...
:: Attempt to copy to System32 (requires Admin)
copy bin\smcli.exe C:\Windows\System32\smcli.exe >nul 2>nul

if %ERRORLEVEL% neq 0 (
    echo [WARNING] Permission denied to C:\Windows\System32.
    echo Please run this script as Administrator OR:
    echo 1. Add %cd%\bin to your System PATH environment variable manually.
    echo 2. Run 'setx PATH "%%PATH%%;%cd%\bin"'
) else (
    echo [SUCCESS] SmCLI installed successfully!
)

echo.
echo Try typing: smcli help
echo ----------------------------------------
pause
