# Defining how CMake should build the project

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(light_weight VERSION 1.0)
file(GLOB_RECURSE SRC_FILES src/*.cc)
add_executable(light_weight ${SRC_FILES})
target_include_directories(light_weight PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
