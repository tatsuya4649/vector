#cmake version to be used
cmake_minimum_required(VERSION 3.0)

#project name
project( vector )

#flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
#files

if (UNIX)
	message("Hello,Unix")
endif()

#include
include_directories( ./include ./src )

#target
add_executable( vector ./src/main.cpp ./src/vector.cpp ./src/vector_member.cpp)

