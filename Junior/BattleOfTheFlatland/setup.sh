@echo off

mkdir lib && cd lib
git clone https://github.com/google/googletest.git

sudo apt-get install libsfml-dev