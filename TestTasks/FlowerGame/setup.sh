@echo off

mkdir lib && cd lib
git clone https://github.com/google/googletest.git
git clone https://github.com/SFML/SFML.git

sudo apt-get install libsfml-dev
