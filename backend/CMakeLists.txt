cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(WEBASSEMBLY "Compile to WebAssembly" OFF)

if(WEBASSEMBLY)
    set(CMAKE_CXX_FLAGS "--no-entry -fPIC")

    # Fetch nlohmann_json, Eigen3 and osqp
    include(FetchContent)
    FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.11.2
    )

    FetchContent_Declare(Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    set(EIGEN_BUILD_DOC OFF)
    set(BUILD_TESTING OFF)
    set(OFF)

    FetchContent_Declare(osqp
        GIT_REPOSITORY https://github.com/osqp/osqp
    )

    FetchContent_MakeAvailable(nlohmann_json Eigen osqp)

    project(webassembly VERSION 1.0 LANGUAGES CXX)
    set(BACKEND_SRC src/wasm/*.cc src/OsqpEigen/*.cpp src/IO/*.cc src/model/*.cc src/MPC/*.cc src/simulations.cc)
    file(GLOB_RECURSE SRC_FILES ${BACKEND_SRC})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../frontend/src") # Set webassembly output in src folder
    add_executable(webassembly ${SRC_FILES}) 

    target_include_directories(webassembly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(webassembly nlohmann_json::nlohmann_json Eigen3::Eigen osqp)

    # emcc compiler flags
    set(EMCC_FLAGS "--bind -s MAIN_MODULE=2 -s NO_DISABLE_EXCEPTION_CATCHING -s USE_ES6_IMPORT_META=0 -s WASM=1 -s SINGLE_FILE=1 -s ALLOW_MEMORY_GROWTH=1 -s MALLOC=emmalloc -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web -s EXPORT_NAME='backend' -s ASSERTIONS=1 -s LINKABLE=1 -std=c++11")
    set_target_properties(webassembly PROPERTIES SUFFIX ".mjs" LINK_FLAGS ${EMCC_FLAGS})

else(WEBASSEMBLY)
    #set(CMAKE_CXX_FLAGS "-Wall -Wextra") # Print warnings
    project(mpc_core VERSION 1.0 LANGUAGES CXX)

    find_package(OsqpEigen REQUIRED)
    find_package(nlohmann_json 3.11.2 REQUIRED)

    file(GLOB_RECURSE SRC_FILES src/*.cc)
    add_executable(mpc_core ${SRC_FILES}) # Define executable

    target_include_directories(mpc_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(mpc_core OsqpEigen::OsqpEigen nlohmann_json::nlohmann_json)
endif(WEBASSEMBLY)