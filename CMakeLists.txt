cmake_minimum_required(VERSION 3.20)
project(lab-2-algorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab-2-algorithms src/main.cpp src/astar.cpp src/generate.cpp src/print.cpp)