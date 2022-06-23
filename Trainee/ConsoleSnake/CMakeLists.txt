cmake_minimum_required(VERSION 3.16.3)
project(console_snake)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
                 type.h
				 enum.h
				 snake.h
				 snake.cpp
				 menu.h
				 menu.cpp
				 game.h
				 game.cpp
				 fruit.h
				 fruit.cpp
				 board.h
				 board.cpp
)

add_executable(console_snake ${SOURCE_FILES})
