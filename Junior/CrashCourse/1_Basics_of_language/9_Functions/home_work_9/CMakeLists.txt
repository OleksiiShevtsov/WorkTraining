cmake_minimum_required(VERSION 3.16.3)
project(home_work_9)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
                 fold_implementation.h
                 histogram_of_argument.h
		 all_func.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
