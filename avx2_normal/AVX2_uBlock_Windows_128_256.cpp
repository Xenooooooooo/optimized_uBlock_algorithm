// Copyright (c) 2022 by the author. All rights reserved.
//
// Redistribution and use in source and binary forms
// are permitted under the MIT license.

#include <emmintrin.h>  // sse2 header file(include sse header file)
#include <immintrin.h>
#include <malloc.h>
#include <memory.h>
#include <nmmintrin.h>  // SSE4.2(include smmintrin.h)
#include <pmmintrin.h>  // SSE3(include emmintrin.h)
#include <smmintrin.h>  // SSE4.1(include tmmintrin.h)
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <tmmintrin.h>  // SSSE3(include pmmintrin.h)
// #include <intrin.h>  // (include immintrin.h)
#include <x86intrin.h>

#define _mm256_loadu2_m128i(__PH, __PL)                                  \
  _mm256_insertf128_si256(_mm256_castsi128_si256(_mm_loadu_si128(__PL)), \
                          _mm_loadu_si128(__PH), 1)
#define _mm256_storeu2_m128i(__PH, __PL, __A)                 \
  {                                                           \
    _mm_storeu_si128(__PL, _mm256_castsi256_si128(__A));      \
    _mm_storeu_si128(__PH, _mm256_extractf128_si256(__A, 1)); \
  }

__m256i S =
    _mm256_setr_epi8(7, 4, 9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5, 7, 4,
                     9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5);
__m256i S_Inv =
    _mm256_setr_epi8(12, 10, 14, 13, 1, 15, 11, 0, 7, 2, 5, 4, 3, 6, 9, 8, 12,
                     10, 14, 13, 1, 15, 11, 0, 7, 2, 5, 4, 3, 6, 9, 8);

__m256i con = _mm256_set1_epi8(0x0f);
__m256i A1 = _mm256_setr_epi8(1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14, 15,
                              8, 17, 18, 19, 20, 21, 22, 23, 16, 25, 26, 27, 28,
                              29, 30, 31, 24);
__m256i A2 = _mm256_setr_epi8(2, 3, 4, 5, 6, 7, 0, 1, 10, 11, 12, 13, 14, 15, 8,
                              9, 18, 19, 20, 21, 22, 23, 16, 17, 26, 27, 28, 29,
                              30, 31, 24, 25);
__m256i A3 = _mm256_setr_epi8(5, 6, 7, 0, 1, 2, 3, 4, 13, 14, 15, 8, 9, 10, 11,
                              12, 21, 22, 23, 16, 17, 18, 19, 20, 29, 30, 31,
                              24, 25, 26, 27, 28);

unsigned char Subkey[25][64];

#define ARRAY_SIZE_MAX (1024 * 256)

unsigned char long_plain_1[ARRAY_SIZE_MAX] = {0},
              long_plain_2[ARRAY_SIZE_MAX] = {0},
              long_plain_3[ARRAY_SIZE_MAX] = {0},
              long_plain_4[ARRAY_SIZE_MAX] = {0};
unsigned char long_cipher_1[ARRAY_SIZE_MAX], long_cipher_2[ARRAY_SIZE_MAX],
    long_cipher_3[ARRAY_SIZE_MAX], long_cipher_4[ARRAY_SIZE_MAX];

unsigned char RC[24][32] = {
    0x9, 0x8, 0x8, 0xc, 0xc, 0x9, 0xd, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x9, 0x8, 0x8, 0xc, 0xc, 0x9, 0xd, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0x0, 0xe, 0x4, 0xa, 0x1, 0xb, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xf, 0x0, 0xe, 0x4, 0xa, 0x1, 0xb, 0x5, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3, 0x5, 0x7, 0x0, 0x6, 0x4, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3, 0x5, 0x7, 0x0, 0x6, 0x4, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x3, 0x9, 0x7, 0xd, 0x2, 0xc, 0x6, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x3, 0x9, 0x7, 0xd, 0x2, 0xc, 0x6,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x7, 0xd, 0x3, 0x9, 0x6, 0x8,
    0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x7, 0xd, 0x3, 0x9, 0x6,
    0x8, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xf, 0x5, 0xb, 0x1,
    0xe, 0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xf, 0x5, 0xb,
    0x1, 0xe, 0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0xe, 0x4,
    0xa, 0x0, 0xf, 0x1, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0xe,
    0x4, 0xa, 0x0, 0xf, 0x1, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7,
    0xc, 0x6, 0x8, 0x2, 0xd, 0x3, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x7, 0xc, 0x6, 0x8, 0x2, 0xd, 0x3, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3, 0x9, 0x2, 0xd, 0x6, 0x8, 0x7, 0xc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0x9, 0x2, 0xd, 0x6, 0x8, 0x7, 0xc, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xb, 0x3, 0xa, 0x7, 0xe, 0x2, 0xf, 0x6, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xb, 0x3, 0xa, 0x7, 0xe, 0x2, 0xf, 0x6, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xa, 0x7, 0xb, 0x3, 0xf, 0x6, 0xe, 0x2, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa, 0x7, 0xb, 0x3, 0xf, 0x6, 0xe, 0x2, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0xe, 0x9, 0xa, 0xd, 0xf, 0xc, 0xb,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0xe, 0x9, 0xa, 0xd, 0xf, 0xc,
    0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xd, 0xc, 0xc, 0x8, 0x8, 0xd,
    0x9, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xd, 0xc, 0xc, 0x8, 0x8,
    0xd, 0x9, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x8, 0x6, 0xc,
    0x2, 0x9, 0x3, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x8, 0x6,
    0xc, 0x2, 0x9, 0x3, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0,
    0x2, 0x4, 0x6, 0x1, 0x7, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0x0, 0x2, 0x4, 0x6, 0x1, 0x7, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xa, 0x1, 0xb, 0x5, 0xf, 0x0, 0xe, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xa, 0x1, 0xb, 0x5, 0xf, 0x0, 0xe, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x8, 0x2, 0x9, 0x6, 0xd, 0x3, 0xc, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x8, 0x2, 0x9, 0x6, 0xd, 0x3, 0xc, 0x7, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xc, 0x5, 0xd, 0x1, 0x9, 0x4, 0x8, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x5, 0xd, 0x1, 0x9, 0x4, 0x8, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xa, 0x5, 0xe, 0x1, 0xb, 0x0, 0xf, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xa, 0x5, 0xe, 0x1, 0xb, 0x0, 0xf,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0x5, 0x4, 0x1, 0x0, 0x4, 0x1,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0x5, 0x4, 0x1, 0x0, 0x4,
    0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xb, 0x7, 0xf, 0x3,
    0xa, 0x2, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xb, 0x7, 0xf,
    0x3, 0xa, 0x2, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x7, 0x0,
    0x3, 0x4, 0x6, 0x5, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x7,
    0x0, 0x3, 0x4, 0x6, 0x5, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe,
    0xf, 0xf, 0xb, 0xb, 0xe, 0xa, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xe, 0xf, 0xf, 0xb, 0xb, 0xe, 0xa, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x1, 0xf, 0x0, 0xb, 0x4, 0xe, 0x5, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xf, 0x0, 0xb, 0x4, 0xe, 0x5, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0};

void uBlock_256_KeySchedule(unsigned char *key, int plen) {
  int i;

  __m256i S_first_SK_second =
      _mm256_setr_epi8(7, 4, 9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5, 0,
                       2, 4, 6, 8, 10, 12, 14, 3, 1, 7, 5, 11, 9, 15, 13);

  __m256i c1 =
      _mm256_setr_epi8(0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6,
                       0x80, 7, 0x80, 16, 0x80, 17, 0x80, 18, 0x80, 19, 0x80,
                       20, 0x80, 21, 0x80, 22, 0x80, 23, 0x80);
  __m256i c2 =
      _mm256_setr_epi8(0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5,
                       0x80, 6, 0x80, 7, 0x80, 16, 0x80, 17, 0x80, 18, 0x80, 19,
                       0x80, 20, 0x80, 21, 0x80, 22, 0x80, 23);
  __m256i c3 =
      _mm256_setr_epi8(8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80,
                       14, 0x80, 15, 0x80, 24, 0x80, 25, 0x80, 26, 0x80, 27,
                       0x80, 28, 0x80, 29, 0x80, 30, 0x80, 31, 0x80);
  __m256i c4 =
      _mm256_setr_epi8(0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13,
                       0x80, 14, 0x80, 15, 0x80, 24, 0x80, 25, 0x80, 26, 0x80,
                       27, 0x80, 28, 0x80, 29, 0x80, 30, 0x80, 31);

  __m256i PK = _mm256_setr_epi8(10, 5, 15, 0, 2, 7, 8, 13, 1, 14, 4, 12, 9, 11,
                                3, 6, 24, 25, 26, 27, 28, 29, 30, 31, 16, 17,
                                18, 19, 20, 21, 22, 23);

  __m256i state1, state2, k, t1, t2, t3, t4;

  __m128i hi_0, hi_1, lo_0, lo_1;

  state2 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(key));  // OK
  state1 = _mm256_srli_epi16(state2, 4);                          // OK
  state1 = _mm256_and_si256(
      state1, con);  // equals to the original code's state1 || state3
  state2 = _mm256_and_si256(
      state2, con);  // equals to the original code's state2 || state4
  t1 = _mm256_shuffle_epi8(state1,
                           c1);  // equals to the original code's dilute of
                                 // the state1 and state3's 1st half (0 front)
  t2 = _mm256_shuffle_epi8(state2,
                           c2);  // equals to the original code's dilute of
                                 // the state2 and state4's 1st half (0  end )
  t3 = _mm256_shuffle_epi8(state1,
                           c3);  // equals to the original code's dilute of
                                 // the state1 and state3's 2nd half (0 front)
  t4 = _mm256_shuffle_epi8(state2,
                           c4);  // equals to the original code's dilute of
                                 // the state2 and state4's 2nd half (0  end )
  state1 = _mm256_xor_si256(
      t1, t2);  // equals to the original code's state1 || state3
  state2 = _mm256_xor_si256(
      t3, t4);  // equals to the original code's state2 || state4

  hi_0 = _mm256_extractf128_si256(state1, 0);  // revised
  lo_0 = _mm256_extractf128_si256(state1, 1);
  hi_1 = _mm256_extractf128_si256(state2, 0);
  lo_1 = _mm256_extractf128_si256(state2, 1);

  state1 = _mm256_set_m128i(hi_1, hi_0);
  state2 = _mm256_set_m128i(lo_1, lo_0);
  lo_0 = _mm256_extractf128_si256(state1, 1);  // revised twice
  hi_1 = _mm256_extractf128_si256(state2, 0);
  state1 = _mm256_inserti128_si256(state1, hi_1, 1);
  state2 = _mm256_inserti128_si256(state2, lo_0, 0);

  _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey[0]), state1);
  _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey[0] + 32), state2);

  for (i = 1; i <= 24; i++) {
    state1 = _mm256_shuffle_epi8(state1, PK);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(RC[i - 1]));
    k = _mm256_xor_si256(state1, k);
    k = _mm256_shuffle_epi8(
        S_first_SK_second,
        k);  // now, in the original form, k = S(state1 xor RC) || SK(state2)
    k = _mm256_xor_si256(
        k, state2);  // now, in the original form, k = (state3 xor (S(state1 xor
                     // RC))) || state4 xor (SK(state2))

    state2 = _mm256_permute4x64_epi64(state1, 0b01001110);
    state1 = k;

    _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey[i]), k);
    _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey[i] + 32), state2);
  }
}

void uBlock_128_Encrypt(unsigned char *plain_1, unsigned char *plain_2,
                        unsigned char *cipher_1, unsigned char *cipher_2,
                        int round) {
  int i;
  __m256i L1 = _mm256_setr_epi8(2, 3, 6, 7, 8, 9, 12, 13, 0, 1, 4, 5, 14, 15,
                                10, 11, 18, 19, 22, 23, 24, 25, 28, 29, 16, 17,
                                20, 21, 30, 31, 26, 27);
  __m256i L2 = _mm256_setr_epi8(4, 5, 14, 15, 10, 11, 0, 1, 2, 3, 12, 13, 8, 9,
                                6, 7, 20, 21, 30, 31, 26, 27, 16, 17, 18, 19,
                                28, 29, 24, 25, 22, 23);

  __m256i c1 = _mm256_setr_epi8(
      0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80, 0,
      0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80);
  __m256i c2 = _mm256_setr_epi8(
      0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7,
      0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7);
  __m256i c3 =
      _mm256_setr_epi8(8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80,
                       14, 0x80, 15, 0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80,
                       12, 0x80, 13, 0x80, 14, 0x80, 15, 0x80);
  __m256i c4 =
      _mm256_setr_epi8(0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13,
                       0x80, 14, 0x80, 15, 0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11,
                       0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15);

  __m256i c5 =
      _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c6 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4,
                       6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0, 2, 4, 6, 8, 10, 12, 14);
  __m256i c7 =
      _mm256_setr_epi8(1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c8 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5,
                       7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15);

  __m256i state1, state2, k, t1, t2, t3, t4;

  state2 = _mm256_loadu2_m128i(reinterpret_cast<__m128i *>(plain_1),
                               reinterpret_cast<__m128i *>(plain_2));
  //    printf("state2 (plain ):    %016x %016x %016x %016x\n", state2[0],
  //    state2[1], state2[2], state2[3]);
  state1 = _mm256_srli_epi16(state2, 4);
  state1 = _mm256_and_si256(state1, con);
  state2 = _mm256_and_si256(state2, con);
  t1 = _mm256_shuffle_epi8(state1, c1);
  t2 = _mm256_shuffle_epi8(state2, c2);
  t3 = _mm256_shuffle_epi8(state1, c3);
  t4 = _mm256_shuffle_epi8(state2, c4);
  state1 = _mm256_xor_si256(t1, t2);
  state2 = _mm256_xor_si256(t3, t4);
  //    printf("state1: %016x %016x %016x %016x\n", state1[0], state1[1],
  //    state1[2], state1[3]); printf("state2: %016x %016x %016x %016x\n",
  //    state2[0], state2[1], state2[2], state2[3]);

  for (i = 0; i < round; i++) {
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i]));
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i] + 32));
    state2 = _mm256_xor_si256(state2, k);

    state1 = _mm256_shuffle_epi8(S, state1);
    state2 = _mm256_shuffle_epi8(S, state2);

    state2 = _mm256_xor_si256(state2, state1);
    k = _mm256_shuffle_epi8(state2, A1);
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_shuffle_epi8(state1, A2);
    state2 = _mm256_xor_si256(state2, k);
    k = _mm256_shuffle_epi8(state2, A2);
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_shuffle_epi8(state1, A3);
    state2 = _mm256_xor_si256(state2, k);
    state1 = _mm256_xor_si256(state1, state2);

    state1 = _mm256_shuffle_epi8(state1, L1);
    state2 = _mm256_shuffle_epi8(state2, L2);
  }

  if (round == 24) {
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[round]));
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[round] + 32));
    state2 = _mm256_xor_si256(state2, k);
  }

  t1 = _mm256_shuffle_epi8(state1, c5);
  t2 = _mm256_shuffle_epi8(state2, c6);
  t3 = _mm256_shuffle_epi8(state1, c7);
  t4 = _mm256_shuffle_epi8(state2, c8);

  state1 = _mm256_xor_si256(t1, t2);
  state2 = _mm256_xor_si256(t3, t4);

  t1 = _mm256_slli_epi16(state1, 4);
  t2 = _mm256_and_si256(state2, con);
  t1 = _mm256_xor_si256(t1, t2);

  _mm256_storeu2_m128i(reinterpret_cast<__m128i *>(cipher_1),
                       reinterpret_cast<__m128i *>(cipher_2), t1);
  //    printf("\n%02x\n", key[2]);
}

void uBlock_128_Decrypt(unsigned char *cipher_1, unsigned char *cipher_2,
                        unsigned char *plain_1, unsigned char *plain_2,
                        int round) {
  int i;
  __m256i L1 = _mm256_setr_epi8(8, 9, 0, 1, 10, 11, 2, 3, 4, 5, 14, 15, 6, 7,
                                12, 13, 24, 25, 16, 17, 26, 27, 18, 19, 20, 21,
                                30, 31, 22, 23, 28, 29);
  __m256i L2 = _mm256_setr_epi8(6, 7, 8, 9, 0, 1, 14, 15, 12, 13, 4, 5, 10, 11,
                                2, 3, 22, 23, 24, 25, 16, 17, 30, 31, 28, 29,
                                20, 21, 26, 27, 18, 19);

  __m256i c1 = _mm256_setr_epi8(
      0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80, 0,
      0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80);
  __m256i c2 = _mm256_setr_epi8(
      0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7,
      0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7);
  __m256i c3 =
      _mm256_setr_epi8(8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80,
                       14, 0x80, 15, 0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80,
                       12, 0x80, 13, 0x80, 14, 0x80, 15, 0x80);
  __m256i c4 =
      _mm256_setr_epi8(0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13,
                       0x80, 14, 0x80, 15, 0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11,
                       0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15);

  __m256i c5 =
      _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c6 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4,
                       6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0, 2, 4, 6, 8, 10, 12, 14);
  __m256i c7 =
      _mm256_setr_epi8(1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c8 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5,
                       7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15);

  __m256i state1, state2, k, t1, t2, t3, t4;

  state2 = _mm256_loadu2_m128i(reinterpret_cast<__m128i *>(cipher_1),
                               reinterpret_cast<__m128i *>(cipher_2));
  //    printf("state2 (plain ):    %016x %016x %016x %016x\n", state2[0],
  //    state2[1], state2[2], state2[3]);
  state1 = _mm256_srli_epi16(state2, 4);
  state1 = _mm256_and_si256(state1, con);
  state2 = _mm256_and_si256(state2, con);
  t1 = _mm256_shuffle_epi8(state1, c1);
  t2 = _mm256_shuffle_epi8(state2, c2);
  t3 = _mm256_shuffle_epi8(state1, c3);
  t4 = _mm256_shuffle_epi8(state2, c4);
  state1 = _mm256_xor_si256(t1, t2);
  state2 = _mm256_xor_si256(t3, t4);
  //    printf("state1: %016x %016x %016x %016x\n", state1[0], state1[1],
  //    state1[2], state1[3]); printf("state2: %016x %016x %016x %016x\n",
  //    state2[0], state2[1], state2[2], state2[3]);

  for (i = round; i > 0; i--) {
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i]));
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i] + 32));
    state2 = _mm256_xor_si256(state2, k);

    state1 = _mm256_shuffle_epi8(state1, L1);
    state2 = _mm256_shuffle_epi8(state2, L2);
    //  printf("state1: %016x %016x %016x %016x\n", state1[0], state1[1],
    //  state1[2], state1[3]); printf("state2: %016x %016x %016x %016x\n",
    //  state2[0], state2[1], state2[2], state2[3]);

    state1 = _mm256_xor_si256(state1, state2);
    k = _mm256_shuffle_epi8(state1, A3);
    state2 = _mm256_xor_si256(state2, k);
    k = _mm256_shuffle_epi8(state2, A2);
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_shuffle_epi8(state1, A2);
    state2 = _mm256_xor_si256(state2, k);
    k = _mm256_shuffle_epi8(state2, A1);
    state1 = _mm256_xor_si256(state1, k);
    state2 = _mm256_xor_si256(state2, state1);
    //  printf("state1: %016x %016x %016x %016x\n", state1[0], state1[1],
    //  state1[2], state1[3]); printf("state2: %016x %016x %016x %016x\n",
    //  state2[0], state2[1], state2[2], state2[3]);

    state1 = _mm256_shuffle_epi8(S_Inv, state1);
    state2 = _mm256_shuffle_epi8(S_Inv, state2);
    //  printf("state1: %016x %016x %016x %016x\n", state1[0], state1[1],
    //  state1[2], state1[3]); printf("state2: %016x %016x %016x %016x\n",
    //  state2[0], state2[1], state2[2], state2[3]);
  }

  k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[0]));
  state1 = _mm256_xor_si256(state1, k);
  k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[0] + 32));
  state2 = _mm256_xor_si256(state2, k);

  t1 = _mm256_shuffle_epi8(state1, c5);
  t2 = _mm256_shuffle_epi8(state2, c6);
  t3 = _mm256_shuffle_epi8(state1, c7);
  t4 = _mm256_shuffle_epi8(state2, c8);

  state1 = _mm256_xor_si256(t1, t2);
  state2 = _mm256_xor_si256(t3, t4);

  t1 = _mm256_slli_epi16(state1, 4);
  t2 = _mm256_and_si256(state2, con);
  t1 = _mm256_xor_si256(t1, t2);

  _mm256_storeu2_m128i(reinterpret_cast<__m128i *>(plain_1),
                       reinterpret_cast<__m128i *>(plain_2), t1);
}

int Crypt_Enc_Block(unsigned char *input_1, unsigned char *input_2, int in_len,
                    unsigned char *output_1, unsigned char *output_2,
                    int *out_len, unsigned char *key, int keylen) {
  int g;

  uBlock_256_KeySchedule(key, 32);

  for (g = 0; g < in_len / 16; g++) {
    uBlock_128_Encrypt(input_1 + g * 16, input_2 + g * 16, output_1 + g * 16,
                       output_2 + g * 16, 24);
  }
  *out_len = in_len - in_len % 16;

  return 0;
}

int Crypt_Dec_Block(unsigned char *input_1, unsigned char *input_2, int in_len,
                    unsigned char *output_1, unsigned char *output_2,
                    int *out_len, unsigned char *key, int keylen) {
  int g;

  uBlock_256_KeySchedule(key, 32);

  for (g = 0; g < in_len / 16; g++) {
    uBlock_128_Decrypt(input_1 + g * 16, input_2 + g * 16, output_1 + g * 16,
                       output_2 + g * 16, 24);
  }
  *out_len = in_len - in_len % 16;

  return 0;
}

int Key_Schedule(unsigned char *key, int keylen, int direction,
                 unsigned char *subkey) {
  int i, j;

  uBlock_256_KeySchedule(key, 32);

  if (direction == 0) {
    for (i = 0; i < 25; i++) {
      for (j = 0; j < 32; j++) {
        subkey[i * 32 + j] = (Subkey[i][2 * j] << 4) ^ Subkey[i][2 * j + 1];
      }
    }
  }

  if (direction == 1) {
    for (i = 0; i < 25; i++) {
      for (j = 0; j < 32; j++) {
        subkey[i * 32 + j] =
            (Subkey[24 - i][2 * j] << 4) ^ Subkey[24 - i][2 * j + 1];
      }
    }
  }

  return 0;
}

int Crypt_Enc_Block_CBC(unsigned char *input_1, unsigned char *input_2,
                        int in_len, unsigned char *output_1,
                        unsigned char *output_2, int *out_len,
                        unsigned char *key, int keylen) {
  int g, j;

  unsigned char iv_1[16] = {0}, iv_2[16] = {0};

  uBlock_256_KeySchedule(key, 32);

  for (g = 0; g < in_len / 16; g++) {
    for (j = 0; j < 16; j++) {
      iv_1[j] ^= (input_1 + g * 16)[j];
      iv_2[j] ^= (input_2 + g * 16)[j];
    }
    uBlock_128_Encrypt(iv_1, iv_2, output_1 + g * 16, output_2 + g * 16, 24);
    memcpy(iv_1, output_1 + g * 16, 16);
    memcpy(iv_2, output_2 + g * 16, 16);
  }
  *out_len = in_len - in_len % 16;

  return 0;
}

int Crypt_Dec_Block_CBC(unsigned char *input_1, unsigned char *input_2,
                        int in_len, unsigned char *output_1,
                        unsigned char *output_2, int *out_len,
                        unsigned char *key, int keylen) {
  int g, j;

  unsigned char iv_1[16] = {0}, iv_2[16] = {0};

  uBlock_256_KeySchedule(key, 32);

  for (g = 0; g < in_len / 16; g++) {
    uBlock_128_Decrypt(input_1 + g * 16, input_2 + g * 16, output_1 + g * 16,
                       output_2 + g * 16, 24);
    for (j = 0; j < 16; j++) {
      (output_1 + g * 16)[j] ^= iv_1[j];
      (output_2 + g * 16)[j] ^= iv_2[j];
    }
    memcpy(iv_1, input_1 + g * 16, 16);
    memcpy(iv_2, input_2 + g * 16, 16);
  }
  *out_len = in_len - in_len % 16;

  return 0;
}

int main() {
  int i, j, r;

  unsigned char key[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                           0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

  unsigned char plain_1[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                               0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
  unsigned char plain_2[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                               0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

  unsigned char cipher_1[16], cipher_2[16];

  u_int32_t op, ed;
  double times, total = 0;
  u_int32_t total_bits = 1280000000;
  printf("10000000-round-Encryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 10000000; r++) {
      Crypt_Enc_Block(plain_1, plain_2, 16, cipher_1, cipher_2, &j, key, 32);
    }
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n", static_cast<double>(2 * total_bits / (times * 1024 * 1024)));
    total += static_cast<double>(2 * total_bits / (times * 1024 * 1024));
  }

  for (i = 0; i < 16; i++) {
    printf("%02x ", cipher_1[i]);
  }
  printf("\n");
  for (i = 0; i < 16; i++) {
    printf("%02x ", cipher_2[i]);
  }
  printf("\n");

  printf("\n");

  printf("\n10000000-round-Encryption average speed: %f mbps\n\n", total / 10);

  total = 0;

  printf("\n\n10000000-round-Decryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 10000000; r++) {
      Crypt_Dec_Block(cipher_1, cipher_2, 16, plain_1, plain_2, &j, key, 32);
    }
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n", static_cast<double>(2 * total_bits / (times * 1024 * 1024)));
    total += static_cast<double>(2 * total_bits / (times * 1024 * 1024));
  }

  printf("\n10000000-round-Decryption average speed: %f mbps\n\n", total / 10);

  for (i = 0; i < 16; i++) {
    printf("%02x ", plain_1[i]);
  }
  printf("\n");

  for (i = 0; i < 16; i++) {
    printf("%02x ", plain_2[i]);
  }
  printf("\n");

  total = 0;

  // ///////////------------(1 + n) long text test----------/////////////// //

  printf("\n\nlong-text-Encryption:\n");
  u_int32_t total_mb = 2048;
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 1024; r++)
      Crypt_Enc_Block(long_plain_1, long_plain_2, 262144, long_cipher_1,
                      long_cipher_2, &j, key, 16);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f mbps\n", static_cast<double>(2 * total_mb / times));
    total += static_cast<double>(2 * total_mb / times);
  }

  printf("\nlong-text-Encryption average speed: %f mbps\n\n", total / 10);

  printf("cipher_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_cipher_1[i]);
  printf("\n");

  printf("cipher_2_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_cipher_2[i]);
  printf("\n");

  total = 0;

  printf("\n\nlong-text-Decryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 1024; r++)
      Crypt_Dec_Block(long_cipher_1, long_cipher_2, 262144, long_plain_1,
                      long_plain_2, &j, key, 16);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f mbps\n", static_cast<double>(2 * total_mb / times));
    total += static_cast<double>(2 * total_mb / times);
  }

  printf("\nlong-text-Decryption average speed: %f mbps\n\n", total / 10);

  total = 0;

  printf("plain_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_plain_1[i]);
  printf("\n");
  printf("plain_2_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_plain_2[i]);
  printf("\n");

  // ///////------------CBC test----------/////////////// //

  printf("\n\nCBC-Encryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 1024; r++)
      Crypt_Enc_Block_CBC(long_plain_1, long_plain_2, 262144, long_cipher_1,
                          long_cipher_2, &j, key, 16);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f mbps\n", static_cast<double>(2 * total_mb / times));
    total += static_cast<double>(2 * total_mb / times);
  }

  printf("\nCBC-Encryption average speed: %f mbps\n\n", total / 10);

  printf("cipher_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_cipher_1[i]);
  printf("\n");
  printf("cipher_2_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_cipher_2[i]);
  printf("\n");

  total = 0;

  printf("\n\nCBC-Decryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 1024; r++)
      Crypt_Dec_Block_CBC(long_cipher_1, long_cipher_2, 262144, long_plain_1,
                          long_plain_2, &j, key, 16);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f mbps\n", static_cast<double>(2 * total_mb / times));
    total += static_cast<double>(2 * total_mb / times);
  }

  printf("\nCBC-Decryption average speed: %f mbps\n\n", total / 10);

  total = 0;

  printf("plain_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_plain_1[i]);
  printf("\n");
  printf("plain_2_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_plain_2[i]);
  printf("\n");

  return 0;
}
