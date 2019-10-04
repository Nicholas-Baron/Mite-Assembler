cmake_minimium_required(VERSION 3.0.0)

project(mite_assembler)

add_executable(mite main.cpp)
target_include_directories(mite PRIVATE src)
target_compile_options(mite PUBLIC "-std=c++11 -Wall -Wextra")

