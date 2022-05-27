cmake_minimum_required(VERSION 3.10)

set(APP_EXE Nyml)
project(${APP_EXE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

file(GLOB src_files
    "src/*.h"
    "src/main.cpp"
)

add_executable(${APP_EXE} ${src_files})

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

