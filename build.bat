@echo off
if not exist build mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
echo.
echo ========================================
echo Running tests...
echo ========================================
camellia_tests.exe
cd ..