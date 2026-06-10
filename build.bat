@echo off
if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo ========================================
echo Running tests...
echo ========================================
Debug\camellia_tests.exe
cd ..