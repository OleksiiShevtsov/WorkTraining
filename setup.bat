@echo off


cd .. && mkdir LIB && cd LIB && 
git clone https://github.com/google/googletest.git
git clone https://github.com/catchorg/Catch2.git

mkdir BUILD 2>nul
setx CMAKE_BUILDS_ROOT "%~dp0BUILD"