#cmake version to be used
cmake_minimum_required(VERSION 3.0)

#project name
project( vector )

#flags

#files

#include
include_directories( ./include ./src )

#target
add_executable( vector ./src/main.cpp ./src/vector.cpp )

