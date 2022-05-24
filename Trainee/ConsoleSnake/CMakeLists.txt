cmake_minimum_required(VERSION 3.16.3)
project(task_1)

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

add_executable(task_1 ${SOURCE_FILES})
