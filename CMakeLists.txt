# CMakeLists.txt for CrossWordPuzzle project.

cmake_minimum_required(VERSION 3.27)

project(CrossWordPuzzle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Engine)
add_subdirectory(App)