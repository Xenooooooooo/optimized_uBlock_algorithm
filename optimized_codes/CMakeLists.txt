cmake_minimum_required(VERSION 3.16.3)

project(optimized_codes)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-O3 -mfma -mavx2 -fno-stack-protector -funroll-loops -march=native)

add_executable(ublock_opt_128_128_4block_1key uBlock-128-128/128_128_4block_1key.cpp)
add_executable(ublock_opt_128_128_4block_4key uBlock-128-128/128_128_4block_4key.cpp)
add_executable(ublock_opt_128_256_4block_1key uBlock-128-256/128_256_4block_1key.cpp)
add_executable(ublock_opt_128_256_4block_4key uBlock-128-256/128_256_4block_4key.cpp)
add_executable(ublock_opt_256_256_4block_1key uBlock-256-256/256_256_2block_1key.cpp)
add_executable(ublock_opt_256_256_4block_4key uBlock-256-256/256_256_2block_2key.cpp)