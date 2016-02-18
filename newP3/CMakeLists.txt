cmake_minimum_required(VERSION 2.8.4)
project(p2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(SOURCE_FILES
    lex.cpp
    semantic.cpp
    main.cpp)
add_executable(p2 ${SOURCE_FILES})