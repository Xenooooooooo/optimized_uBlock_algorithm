cmake_minimum_required(VERSION 3.16.3)

project(opt_ublock)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(avx2_normal)
add_subdirectory(optimized_codes)