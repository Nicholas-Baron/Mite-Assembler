cmake_minimum_required(VERSION 3.0)

project(mite_assembler)

add_executable(mite src/main.cpp src/cmdargs.cpp)
target_include_directories(mite PRIVATE src external)
target_compile_options(mite PUBLIC "-std=c++14" "-Wall" "-Wextra")

