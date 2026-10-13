@echo off

cd .. && mkdir LIB && cd LIB && 
git clone https://github.com/google/googletest.git
git clone https://github.com/catchorg/Catch2.git
git clone https://github.com/SFML/SFML.git

cd .. && mkdir BUILD 2>nul
setx CMAKE_BUILDS_ROOT "C:\workSolution\BUILD"
setx CMAKE_LIB_ROOT "C:\workSolution\LIB"