cmake_minimum_required(VERSION 3.16)

project(netlab_pp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(netlab src/bytes.cpp )

target_include_directories(netlab PUBLIC ${PROJECT_SOURCE_DIR}/include )

target_compile_options(netlab PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(codec_demo apps/codec_demo.cpp )

target_link_libraries(codec_demo PRIVATE netlab)

