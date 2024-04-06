cmake_minimum_required(VERSION 3.16.3)

project(avx2_normal)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-O3 -mfma -mavx2 -fno-stack-protector -funroll-loops -march=native)

add_executable(avx2_normal_128_128 AVX2_uBlock_Windows_128_128.cpp)
add_executable(avx2_normal_128_256 AVX2_uBlock_Windows_128_256.cpp)
add_executable(avx2_normal_256_256 AVX2_uBlock_Windows_256_256.cpp)