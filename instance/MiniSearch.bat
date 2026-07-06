@echo off
cd /d %~dp0
wsl bash -c "cd /mnt/d/ccnu_code_6/instance && LD_LIBRARY_PATH=./build/bin ./build/bin/App input/"
echo.
pause
