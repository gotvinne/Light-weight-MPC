cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(WEBASSEMBLY "Compile to WebAssembly" OFF)

if(WEBASSEMBLY)
    set(CMAKE_CXX_FLAGS "--no-entry")
    include(FetchContent)
    
    FetchContent_Declare(nlohmann_json 
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(nlohmann_json)

    FetchContent_Declare(Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    )
    FetchContent_MakeAvailable(Eigen)

    FetchContent_Declare(osqp
        GIT_REPOSITORY https://github.com/osqp/osqp
    )
    FetchContent_MakeAvailable(osqp)

    project(webassembly VERSION 1.0 LANGUAGES CXX)

    # Compiling every file except main.cc
    set(BACK_END_SRC src/bindings/*.cc src/OsqpEigen/*.cpp src/IO/*.cc src/model/*.cc src/MPC/*.cc)
    file(GLOB_RECURSE SRC_FILES ${BACK_END_SRC})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../front-end/src") # Set webassembly output in src folder
    add_executable(webassembly ${SRC_FILES}) # Define executable

    target_include_directories(webassembly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(webassembly nlohmann_json::nlohmann_json Eigen3::Eigen osqp)

    set(EMCC_FLAGS "--bind --no-entry -s MAIN_MODULE -s SINGLE_FILE=1 -s STRICT=1 -s ALLOW_MEMORY_GROWTH=1 -s MALLOC=emmalloc -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web -s EXPORT_NAME='backend' -s ASSERTIONS=1")
    set_target_properties(webassembly PROPERTIES SUFFIX ".mjs" LINK_FLAGS ${EMCC_FLAGS})

else(WEBASSEMBLY)
    #set(CMAKE_CXX_FLAGS "-Wall -Wextra") # Print warnings
    project(backend VERSION 1.0 LANGUAGES CXX)

    find_package(OsqpEigen REQUIRED)
    find_package(nlohmann_json 3.11.2 REQUIRED)

    file(GLOB_RECURSE SRC_FILES src/*.cc)
    add_executable(backend ${SRC_FILES}) # Define executable

    target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(backend OsqpEigen::OsqpEigen nlohmann_json::nlohmann_json)
endif(WEBASSEMBLY)