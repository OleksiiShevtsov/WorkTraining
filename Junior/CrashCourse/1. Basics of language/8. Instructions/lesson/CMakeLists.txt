cmake_minimum_required(VERSION 3.16.3)
project(lesson)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
                 random_number.h
                 namespace.h
                 narrow_caster.h
                 structured_bindings.h
                 conditional_statements.h
                 cycle.h
                 fibonacci_iterator.h
                 instructions_goto.h
)

add_executable(lesson ${SOURCE_FILES})
