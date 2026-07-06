@echo off
setlocal enabledelayedexpansion

REM MiniSearch - one-click build and run on Windows via WSL
REM
REM Double-click: run with tests/ demo files
REM CMD: run.bat D:\path\file1.txt D:\path\file2.txt
REM Drag-and-drop files onto this .bat icon

cd /d %~dp0

echo ===== [1/2] Building... =====
wsl cmake -S . -B build >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Make sure WSL has cmake and gcc installed.
    pause
    exit /b
)

wsl make -C build >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Build failed.
    pause
    exit /b
)

echo Build OK!

echo ===== [2/2] Running... =====
echo.

if "%1"=="" (
    REM No args - use tests/ demo
    wsl bash -c "cd /mnt/d/ccnu_code_6/instance && LD_LIBRARY_PATH=./build/bin ./build/bin/App tests/"
) else (
    REM User provided files - convert Windows paths to WSL paths
    set "WSL_ARGS="
    :loop
    if "%~1"=="" goto run
    set "FP=%~1"
    set "FP=!FP:\=/!"
    set "FP=!FP:D:=/mnt/d!"
    set "FP=!FP:C:=/mnt/c!"
    set "WSL_ARGS=!WSL_ARGS! !FP!"
    shift
    goto loop
    :run
    wsl bash -c "cd /mnt/d/ccnu_code_6/instance && LD_LIBRARY_PATH=./build/bin ./build/bin/App !WSL_ARGS!"
)

echo.
pause
endlocal
