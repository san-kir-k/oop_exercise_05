cmake_minimum_required(VERSION 3.17)

project(oop_exercise_05)

set(SOURCE main.cpp)
set(EXEC lab5)

add_definitions(-std=c++17 -pedantic -Wall -Werror)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${EXEC} ${SOURCE})
