cmake_minimum_required(VERSION 3.16)

project(netlab_pp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(netlab 
    src/bytes.cpp
    src/packet_codec.cpp 
)

target_include_directories(netlab PUBLIC ${PROJECT_SOURCE_DIR}/include )

target_compile_options(netlab PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# --- Target 1: The Demo Executable ---
add_executable(codec_demo 
    apps/codec_demo.cpp 
)

target_link_libraries(codec_demo PRIVATE 
    netlab
)

# --- Target 2: The Test Executable ---
add_executable(packet_codec_test 
    tests/packet_codec_test.cpp
)

target_link_libraries(packet_codec_test PRIVATE 
    netlab
)

# --- Testing Integration ---
enable_testing()

add_test(
    NAME packet_codec_test
    COMMAND packet_codec_test
)

