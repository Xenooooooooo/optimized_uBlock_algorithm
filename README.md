# optimized_uBlock_algorithm

This is the optimized uBlock algorithm codes on github.

The original uBlock algorithm supports block lengths and key lengths of **128 bits** and **256 bits** respectively, denoted as 128/128, 128/256, and 256/256.

This work combines the use of the AVX2 instruction set, optimized data storage structure, high-level parallelism and other methods to optimize the software implementation of uBlock algorithm. Meanwhile, the implementation of single-key version and multi-key version is given for the above three modes respectively for flexible use in various scenarios.
