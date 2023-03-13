# Defining how CMake should build the project
cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra") # Print warnings
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(WEB "Compile to WebAssembly" OFF)

if(WEB)
    include(FetchContent)
    # Nlohmann_json
    FetchContent_Declare(json 
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(json)

    FetchContent_Declare(Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    )
    FetchContent_MakeAvailable(Eigen)

    FetchContent_Declare(osqp
        GIT_REPOSITORY https://github.com/osqp/osqp
    )
    FetchContent_MakeAvailable(osqp)

    project(web VERSION 1.0 LANGUAGES CXX)

    # Compiling all files in src
    file(GLOB_RECURSE SRC_FILES src/*.cc src/*.cpp)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public") # Set web output in public folder
    add_executable(web ${SRC_FILES}) # Define executable

    target_include_directories(web PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(web nlohmann_json::nlohmann_json Eigen3::Eigen osqp)

    # In order to use bindings
    set_target_properties(web PROPERTIES LINK_FLAGS "--bind")

else(WEB)
    project(light_weight VERSION 1.0 LANGUAGES CXX)

    find_package(OsqpEigen REQUIRED)
    find_package(nlohmann_json 3.11.2 REQUIRED)

    # Compiling all files in src
    file(GLOB_RECURSE SRC_FILES src/*.cc src/*.cpp)
    add_executable(light_weight ${SRC_FILES}) # Define executable

    # Include libs: include, conan, osqp, boost
    target_include_directories(light_weight PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(light_weight OsqpEigen::OsqpEigen nlohmann_json::nlohmann_json)
endif(WEB)