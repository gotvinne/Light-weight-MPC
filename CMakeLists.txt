# Defining how CMake should build the project
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(light_weight VERSION 1.0 LANGUAGES CXX)

# Setting up package manager conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)  # Includes the contents of the conanbuildinfo.cmake file.
conan_basic_setup()  # Prepares the CMakeList.txt for Conan.

# OSQP
find_package(osqp REQUIRED)

# Compiling all files in src
file(GLOB_RECURSE SRC_FILES src/*.cc)
add_executable(light_weight ${SRC_FILES}) # Define executable

# Include libs: include, conan, osqp
target_include_directories(light_weight PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(light_weight ${CONAN_LIBS})
target_link_libraries(light_weight PRIVATE osqp::osqp)

# scenarios and simulations 