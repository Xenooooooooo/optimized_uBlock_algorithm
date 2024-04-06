# optimized_uBlock_algorithm



## Introduction

uBlock is a family of block ciphers supporting 128-bit and 256-bit block sizes and key sizes. There are different versions of uBlock, which are denoted as **uBlock-128/128, uBlock-128/256, and uBlock-256/256**, respectively. The balance between security, implementation performance, and adaptability is reflected in the overall algorithm design, S-box, diffusion matrix, key schedule, and other details. The designing detail can be referred to the [original paper](https://github.com/Xenooooooooo/optimized_uBlock_algorithm/blob/main/original_codes/1-original_files/uBlock%E7%AE%97%E6%B3%95%E8%AE%BE%E8%AE%A1%E6%96%87%E6%A1%A3.pdf).

This work combines the use of the **AVX2 instruction set**, **optimized data storage structure**, **high-level parallelism** and other methods to optimize the software implementation of uBlock algorithm. Meanwhile, the implementation of single-key version and multi-key version is given for the above three modes respectively for flexible use in various scenarios. The paper of our **optimized** uBlock can be accessed [here](https://kns.cnki.net/kcms2/article/abstract?v=Skeo7MzZydbCU6pb_Gyz-FFBwk8aL_3paQD_8teclByF-FSyFrxvX_6VaOXTb1sv_j1eNpa5LChuIpxzHqWLrbq5eP4QgXWNEo7-iwYomy9nOzkRLMXOHWvW1NrvlV6xboflRaC1BPg=).  

## repository structure

Besides the original codes and algorithm paper, our implementation includes: 

````shell
├── avx2_normal
│   ├── AVX2_uBlock_Windows_128_128.cpp
│   ├── AVX2_uBlock_Windows_128_256.cpp
│   ├── AVX2_uBlock_Windows_256_256.cpp
└── optimized_codes
    ├── uBlock-128-128
    │   ├── 128_128_4block_1key.cpp
    │   └── 128_128_4block_4key.cpp
    ├── uBlock-128-256
    │   ├── 128_256_4block_1key.cpp
    │   └── 128_256_4block_4key.cpp
    └── uBlock-256-256
        ├── 256_256_2block_1key.cpp
        └── 256_256_2block_2key.cpp
````

The three codes in `./avx2_normal` are the simple application of uBlock which use AVX2 intrinsics to replace the original SSE intrinsics, and contains no structure optimization. This version of implementation is used for comparison.

The three codes in `./optimized_codes` are our full optimization version of uBlock, containing six implementation in total of three uBlocks divided by key and block sizes.

We also include the original code and designing paper in this repository, under the `original_codes` directory.

## Building

For this repository, one can easily uses cmake to complile all the optimized cpps at once. Using the following steps in the root directory of this project:

````shell
mkdir build && cd build
cmake .. && make
````

After the building process, the compiled executable flies will be showed up under your `build` directory, and simply run them in the terminal.

## Benchmarking

Here we put the detailed benchmarking result of this repository. The testing is under Microsoft Windows 10 Professional Edition 20H2 Build 19042.1466, using AMD Ryzen 9 5900X @3.70 GHz with 32 GB RAM. The compiler is gcc (x86_64-win32-seh-rev0，Built by MinGW-W64 project) 8.1.0。

![output__12.jpg](https://s2.loli.net/2024/04/06/ULvKrF6fXxh1o5P.jpg)

![output__14.jpg](https://s2.loli.net/2024/04/06/vYZUQs5S1fk2XMN.jpg)

![output__13 - .jpg](https://s2.loli.net/2024/04/06/bBnhw2qjk97RgDo.jpg)

![output__13.jpg](https://s2.loli.net/2024/04/06/KEfc8yTv4Rt2CJq.jpg)

## Contributors

Longxin Wang [github_link](https://github.com/Xenooooooooo)

Lei Tian [github_link](https://github.com/Tian-lei)

Yang Hu [github_link](https://github.com/huyang-819)

