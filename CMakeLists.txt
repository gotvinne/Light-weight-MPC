# Defining how CMake should build the project
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(light_weight VERSION 1.0 LANGUAGES CXX)

# OSQP Eigen
find_package(OsqpEigen REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)

# Compiling all files in src
file(GLOB_RECURSE SRC_FILES src/*.cc)
add_executable(light_weight ${SRC_FILES}) # Define executable

# Include libs: include, conan, osqp
target_include_directories(light_weight PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(light_weight OsqpEigen::OsqpEigen nlohmann_json::nlohmann_json)

