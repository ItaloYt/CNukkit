cmake_minimum_required(VERSION 3.20)
project(cnukkit)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE src "src/*.c" "include/*.h")

add_executable(cnukkit ${src})

target_include_directories(cnukkit PUBLIC "include/")
target_compile_options(cnukkit PUBLIC -Wall -Werror -g -O0)
