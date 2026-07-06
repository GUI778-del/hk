@echo off
cd /d %~dp0
wsl bash -c "cd /mnt/d/ccnu_code_6/instance && mkdir -p build && cd build && cmake .. && make"
echo.
pause
