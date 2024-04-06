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

#define ARRAY_SIZE_MAX (1024 * 256)

unsigned char long_plain_1[ARRAY_SIZE_MAX] = {0},
              long_plain_2[ARRAY_SIZE_MAX] = {0},
              long_plain_3[ARRAY_SIZE_MAX] = {0},
              long_plain_4[ARRAY_SIZE_MAX] = {0};
unsigned char long_cipher_1[ARRAY_SIZE_MAX], long_cipher_2[ARRAY_SIZE_MAX],
    long_cipher_3[ARRAY_SIZE_MAX], long_cipher_4[ARRAY_SIZE_MAX];

unsigned char RC[24][32] = {
    0x9, 0x8, 0x8, 0xc, 0xc, 0x9, 0xd, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0x0, 0xe, 0x4, 0xa, 0x1, 0xb, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2, 0x1, 0x3, 0x5, 0x7, 0x0, 0x6, 0x4, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x3, 0x9, 0x7, 0xd, 0x2, 0xc, 0x6, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x7, 0xd, 0x3, 0x9, 0x6, 0x8,
    0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xf, 0x5, 0xb, 0x1,
    0xe, 0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0xe, 0x4,
    0xa, 0x0, 0xf, 0x1, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7,
    0xc, 0x6, 0x8, 0x2, 0xd, 0x3, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3, 0x9, 0x2, 0xd, 0x6, 0x8, 0x7, 0xc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xb, 0x3, 0xa, 0x7, 0xe, 0x2, 0xf, 0x6, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xa, 0x7, 0xb, 0x3, 0xf, 0x6, 0xe, 0x2, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0xe, 0x9, 0xa, 0xd, 0xf, 0xc, 0xb,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xd, 0xc, 0xc, 0x8, 0x8, 0xd,
    0x9, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x8, 0x6, 0xc,
    0x2, 0x9, 0x3, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0,
    0x2, 0x4, 0x6, 0x1, 0x7, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xa, 0x1, 0xb, 0x5, 0xf, 0x0, 0xe, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x8, 0x2, 0x9, 0x6, 0xd, 0x3, 0xc, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xc, 0x5, 0xd, 0x1, 0x9, 0x4, 0x8, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xa, 0x5, 0xe, 0x1, 0xb, 0x0, 0xf, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0x5, 0x4, 0x1, 0x0, 0x4, 0x1,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xb, 0x7, 0xf, 0x3,
    0xa, 0x2, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x7, 0x0,
    0x3, 0x4, 0x6, 0x5, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe,
    0xf, 0xf, 0xb, 0xb, 0xe, 0xa, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x1, 0xf, 0x0, 0xb, 0x4, 0xe, 0x5, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0};

unsigned char Subkey[25][64];

void uBlock_256_KeySchedule(unsigned char *key) {
  int i;

  __m256i con = _mm256_set1_epi8(0x0f);

  __m256i state1, state2, k, t1, t2, t3, t4;

  __m128i hi_0, hi_1, lo_0, lo_1;

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

  __m256i S_first_SK_second =
      _mm256_setr_epi8(7, 4, 9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5, 0,
                       2, 4, 6, 8, 10, 12, 14, 3, 1, 7, 5, 11, 9, 15, 13);
  __m256i PK = _mm256_setr_epi8(10, 5, 15, 0, 2, 7, 8, 13, 1, 14, 4, 12, 9, 11,
                                3, 6, 24, 25, 26, 27, 28, 29, 30, 31, 16, 17,
                                18, 19, 20, 21, 22, 23);

  state2 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(key));
  state1 = _mm256_srli_epi16(state2, 4);
  state1 = _mm256_and_si256(
      state1, con);  // equals to the original code's state1 || state3
  state2 = _mm256_and_si256(
      state2, con);  // equals to the original code's state2 || state4
  t1 = _mm256_shuffle_epi8(state1,
                           c1);  // equals to the original code's dilute of the
                                 // state1 and state3's 1st half (0 front)
  t2 = _mm256_shuffle_epi8(state2,
                           c2);  // equals to the original code's dilute of the
                                 // state2 and state4's 1st half (0  end )
  t3 = _mm256_shuffle_epi8(state1,
                           c3);  // equals to the original code's dilute of the
                                 // state1 and state3's 2nd half (0 front)
  t4 = _mm256_shuffle_epi8(state2,
                           c4);  // equals to the original code's dilute of the
                                 // state2 and state4's 2nd half (0  end )
  state1 = _mm256_xor_si256(
      t1, t2);  // equals to the original code's state1 || state3
  state2 = _mm256_xor_si256(
      t3, t4);  // equals to the original code's state2 || state4

  lo_0 = _mm256_extractf128_si256(state1, 1);
  hi_1 = _mm256_extractf128_si256(state2, 0);
  state1 = _mm256_inserti128_si256(state1, hi_1, 1);
  state2 = _mm256_inserti128_si256(state2, lo_0, 0);

  _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey[0]), state1);
  _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey[0] + 32), state2);

  for (i = 1; i <= 24; i++) {
    state1 = _mm256_shuffle_epi8(state1, PK);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(RC[i - 1]));  // OK
    k = _mm256_xor_si256(state1, k);               // OK
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

void uBlock_256_Encrypt(unsigned char *plain, unsigned char *cipher,
                        int round) {
  int i;

  __m256i c5_and_c7 =
      _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 17, 19, 21, 23, 25, 27, 29, 31, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c6_and_c8 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4,
                       6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 17, 19, 21, 23, 25, 27, 29, 31);

  __m256i L1_and_L3_En =
      _mm256_setr_epi8(4, 5, 14, 15, 0x80, 0x80, 0x80, 0x80, 6, 7, 12, 13, 0x80,
                       0x80, 0x80, 0x80, 18, 19, 24, 25, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0x80, 26, 27, 16, 17);
  __m256i L2_and_L4_En =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0, 1, 10, 11, 0x80, 0x80, 0x80,
                       0x80, 2, 3, 8, 9, 0x80, 0x80, 0x80, 0x80, 30, 31, 20, 21,
                       28, 29, 22, 23, 0x80, 0x80, 0x80, 0x80);
  __m256i L5_and_L7_En =
      _mm256_setr_epi8(12, 13, 0x80, 0x80, 2, 3, 0x80, 0x80, 0x80, 0x80, 8, 9,
                       4, 5, 0x80, 0x80, 30, 31, 16, 17, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 22, 23, 0x80, 0x80, 26, 27);
  __m256i L6_and_L8_En =
      _mm256_setr_epi8(0x80, 0x80, 6, 7, 0x80, 0x80, 8, 9, 2, 3, 0x80, 0x80,
                       0x80, 0x80, 14, 15, 0x80, 0x80, 0x80, 0x80, 26, 27, 20,
                       21, 28, 29, 0x80, 0x80, 16, 17, 0x80, 0x80);

  __m256i S =
      _mm256_setr_epi8(7, 4, 9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5, 7,
                       4, 9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5);

  __m256i A1 = _mm256_setr_epi8(1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14,
                                15, 8, 17, 18, 19, 20, 21, 22, 23, 16, 25, 26,
                                27, 28, 29, 30, 31, 24);
  __m256i A2 = _mm256_setr_epi8(2, 3, 4, 5, 6, 7, 0, 1, 10, 11, 12, 13, 14, 15,
                                8, 9, 18, 19, 20, 21, 22, 23, 16, 17, 26, 27,
                                28, 29, 30, 31, 24, 25);
  __m256i A3 = _mm256_setr_epi8(5, 6, 7, 0, 1, 2, 3, 4, 13, 14, 15, 8, 9, 10,
                                11, 12, 21, 22, 23, 16, 17, 18, 19, 20, 29, 30,
                                31, 24, 25, 26, 27, 28);

  __m256i con = _mm256_set1_epi8(0x0f);

  __m256i state1, state2, k, t1, t2;

  state2 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(plain));
  state1 = _mm256_srli_epi16(state2, 4);
  state1 = _mm256_and_si256(state1, con);
  state2 = _mm256_and_si256(state2, con);

  t1 = _mm256_unpacklo_epi8(state1, state2);
  t2 = _mm256_unpackhi_epi8(state1, state2);
  state1 = _mm256_permute2x128_si256(t1, t2, 0b00100000);
  state2 = _mm256_permute2x128_si256(t1, t2, 0b00110001);

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

    t1 = _mm256_permute4x64_epi64(state1, 0b01000100);
    t1 = _mm256_shuffle_epi8(t1, L1_and_L3_En);
    t2 = _mm256_permute4x64_epi64(state1, 0b11101110);
    t2 = _mm256_shuffle_epi8(t2, L2_and_L4_En);

    state1 = _mm256_xor_si256(t1, t2);

    t1 = _mm256_permute4x64_epi64(state2, 0b01000100);
    t1 = _mm256_shuffle_epi8(t1, L5_and_L7_En);
    t2 = _mm256_permute4x64_epi64(state2, 0b11101110);
    t2 = _mm256_shuffle_epi8(t2, L6_and_L8_En);

    state2 = _mm256_xor_si256(t1, t2);
  }

  if (round == 24) {
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[round]));
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[round] + 32));
    state2 = _mm256_xor_si256(state2, k);
  }

  t1 = _mm256_permute4x64_epi64(state1, 0b01000100);
  t1 = _mm256_shuffle_epi8(t1, c5_and_c7);
  t2 = _mm256_permute4x64_epi64(state1, 0b11101110);
  t2 = _mm256_shuffle_epi8(t2, c6_and_c8);

  state1 = _mm256_xor_si256(t1, t2);

  t1 = _mm256_permute4x64_epi64(state2, 0b01000100);
  t1 = _mm256_shuffle_epi8(t1, c5_and_c7);
  t2 = _mm256_permute4x64_epi64(state2, 0b11101110);
  t2 = _mm256_shuffle_epi8(t2, c6_and_c8);

  state2 = _mm256_xor_si256(t1, t2);

  t1 = _mm256_unpacklo_epi64(state1, state2);
  t2 = _mm256_unpackhi_epi64(state1, state2);
  state1 = _mm256_permute4x64_epi64(t1, 0b11011000);
  state2 = _mm256_permute4x64_epi64(t2, 0b11011000);
  t1 = _mm256_unpacklo_epi64(state1, state2);
  t2 = _mm256_unpackhi_epi64(state1, state2);

  t1 = _mm256_slli_epi16(t1, 4);
  t1 = _mm256_xor_si256(t1, t2);

  _mm256_storeu_si256(reinterpret_cast<__m256i *>(cipher), t1);
}

void uBlock_256_Decrypt(unsigned char *cipher, unsigned char *plain,
                        int round) {
  int i;

  __m256i S_Inv =
      _mm256_setr_epi8(12, 10, 14, 13, 1, 15, 11, 0, 7, 2, 5, 4, 3, 6, 9, 8, 12,
                       10, 14, 13, 1, 15, 11, 0, 7, 2, 5, 4, 3, 6, 9, 8);

  __m256i A1 = _mm256_setr_epi8(1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14,
                                15, 8, 17, 18, 19, 20, 21, 22, 23, 16, 25, 26,
                                27, 28, 29, 30, 31, 24);
  __m256i A2 = _mm256_setr_epi8(2, 3, 4, 5, 6, 7, 0, 1, 10, 11, 12, 13, 14, 15,
                                8, 9, 18, 19, 20, 21, 22, 23, 16, 17, 26, 27,
                                28, 29, 30, 31, 24, 25);
  __m256i A3 = _mm256_setr_epi8(5, 6, 7, 0, 1, 2, 3, 4, 13, 14, 15, 8, 9, 10,
                                11, 12, 21, 22, 23, 16, 17, 18, 19, 20, 29, 30,
                                31, 24, 25, 26, 27, 28);

  __m256i c5_and_c7 =
      _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 17, 19, 21, 23, 25, 27, 29, 31, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c6_and_c8 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4,
                       6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 17, 19, 21, 23, 25, 27, 29, 31);

  __m256i L1_and_L3_De =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0, 1, 8, 9, 0x80, 0x80, 0x80,
                       0x80, 10, 11, 2, 3, 20, 21, 28, 29, 0x80, 0x80, 0x80,
                       0x80, 30, 31, 22, 23, 0x80, 0x80, 0x80, 0x80);
  __m256i L2_and_L4_De =
      _mm256_setr_epi8(14, 15, 0, 1, 0x80, 0x80, 0x80, 0x80, 2, 3, 12, 13, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 22, 23, 26, 27,
                       0x80, 0x80, 0x80, 0x80, 24, 25, 20, 21);
  __m256i L5_and_L7_De =
      _mm256_setr_epi8(0x80, 0x80, 4, 5, 12, 13, 0x80, 0x80, 10, 11, 0x80, 0x80,
                       0, 1, 0x80, 0x80, 0x80, 0x80, 24, 25, 0x80, 0x80, 18, 19,
                       22, 23, 0x80, 0x80, 0x80, 0x80, 30, 31);
  __m256i L6_and_L8_De =
      _mm256_setr_epi8(2, 3, 0x80, 0x80, 0x80, 0x80, 10, 11, 0x80, 0x80, 14, 15,
                       0x80, 0x80, 0, 1, 28, 29, 0x80, 0x80, 22, 23, 0x80, 0x80,
                       0x80, 0x80, 20, 21, 24, 25, 0x80, 0x80);

  __m256i con = _mm256_set1_epi8(0x0f);

  __m256i state1, state2, k, t1, t2;

  state2 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(cipher));
  state1 = _mm256_srli_epi16(state2, 4);
  state1 = _mm256_and_si256(state1, con);
  state2 = _mm256_and_si256(state2, con);

  t1 = _mm256_unpacklo_epi8(state1, state2);
  t2 = _mm256_unpackhi_epi8(state1, state2);
  state1 = _mm256_permute2x128_si256(t1, t2, 0b00100000);
  state2 = _mm256_permute2x128_si256(t1, t2, 0b00110001);

  for (i = round; i > 0; i--) {
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i]));
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i] + 32));
    state2 = _mm256_xor_si256(state2, k);

    t1 = _mm256_permute4x64_epi64(state1, 0b01000100);
    t1 = _mm256_shuffle_epi8(t1, L1_and_L3_De);
    t2 = _mm256_permute4x64_epi64(state1, 0b11101110);
    t2 = _mm256_shuffle_epi8(t2, L2_and_L4_De);
    state1 = _mm256_xor_si256(t1, t2);

    t1 = _mm256_permute4x64_epi64(state2, 0b01000100);
    t1 = _mm256_shuffle_epi8(t1, L5_and_L7_De);
    t2 = _mm256_permute4x64_epi64(state2, 0b11101110);
    t2 = _mm256_shuffle_epi8(t2, L6_and_L8_De);
    state2 = _mm256_xor_si256(t1, t2);

    state1 = _mm256_xor_si256(state1, state2);

    k = _mm256_shuffle_epi8(state1, A3);
    state2 = _mm256_xor_si256(k, state2);

    k = _mm256_shuffle_epi8(state2, A2);
    state1 = _mm256_xor_si256(state1, k);

    k = _mm256_shuffle_epi8(state1, A2);
    state2 = _mm256_xor_si256(state2, k);

    k = _mm256_shuffle_epi8(state2, A1);
    state1 = _mm256_xor_si256(state1, k);

    state2 = _mm256_xor_si256(state1, state2);

    state1 = _mm256_shuffle_epi8(S_Inv, state1);
    state2 = _mm256_shuffle_epi8(S_Inv, state2);
  }

  k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i]));
  state1 = _mm256_xor_si256(state1, k);
  k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey[i] + 32));
  state2 = _mm256_xor_si256(state2, k);

  t1 = _mm256_permute4x64_epi64(state1, 0b01000100);
  t1 = _mm256_shuffle_epi8(t1, c5_and_c7);
  t2 = _mm256_permute4x64_epi64(state1, 0b11101110);
  t2 = _mm256_shuffle_epi8(t2, c6_and_c8);

  state1 = _mm256_xor_si256(t1, t2);

  t1 = _mm256_permute4x64_epi64(state2, 0b01000100);
  t1 = _mm256_shuffle_epi8(t1, c5_and_c7);
  t2 = _mm256_permute4x64_epi64(state2, 0b11101110);
  t2 = _mm256_shuffle_epi8(t2, c6_and_c8);

  state2 = _mm256_xor_si256(t1, t2);

  t1 = _mm256_unpacklo_epi64(state1, state2);
  t2 = _mm256_unpackhi_epi64(state1, state2);
  state1 = _mm256_permute4x64_epi64(t1, 0b11011000);
  state2 = _mm256_permute4x64_epi64(t2, 0b11011000);
  t1 = _mm256_unpacklo_epi64(state1, state2);
  t2 = _mm256_unpackhi_epi64(state1, state2);

  t1 = _mm256_slli_epi16(t1, 4);
  t1 = _mm256_xor_si256(t1, t2);

  _mm256_storeu_si256(reinterpret_cast<__m256i *>(cipher), t1);
}

int Crypt_Enc_Block(unsigned char *input, int in_len, unsigned char *output,
                    unsigned char *key) {
  uBlock_256_KeySchedule(key);

  for (int g = 0; g < in_len / 32; g++)
    uBlock_256_Encrypt(input + g * 32, output + g * 32, 24);
  return 0;
}

int Crypt_Dec_Block(unsigned char *input, int in_len, unsigned char *output,
                    unsigned char *key) {
  uBlock_256_KeySchedule(key);

  for (int g = 0; g < in_len / 32; g++)
    uBlock_256_Decrypt(input + g * 32, output + g * 32, 24);
  return 0;
}

int Crypt_Enc_Block_CBC(unsigned char *input, int in_len, unsigned char *output,
                        int *out_len, unsigned char *key) {
  int g, j;

  unsigned char iv[32] = {0}, plain1[32] = {0};

  uBlock_256_KeySchedule(key);

  for (g = 0; g < in_len / 32; g++) {
    memcpy(plain1, input + g * 32, 32);
    for (j = 0; j < 32; j++) {
      iv[j] ^= input[j];
    }
    uBlock_256_Encrypt(iv, output + g * 32, 24);
    memcpy(iv, output + g * 32, 32);
    //        memcpy(output+g*32, cipher1, 32);
  }
  *out_len = in_len - in_len % 32;

  return 0;
}

int Crypt_Dec_Block_CBC(unsigned char *input, int in_len, unsigned char *output,
                        int *out_len, unsigned char *key) {
  int g, j;
  unsigned char iv[32] = {0}, plain1[32] = {0}, cipher1[32] = {0};

  uBlock_256_KeySchedule(key);

  for (g = 0; g < in_len / 32; g++) {
    memcpy(cipher1, input + g * 32, 32);
    uBlock_256_Decrypt(cipher1, output + g * 32, 24);
    for (j = 0; j < 32; j++) {
      output[j + g * 32] ^= iv[j];
    }
    //        memcpy(output+g*32, plain1, 32);
    memcpy(iv, input + g * 32, 32);
  }
  *out_len = in_len - in_len % 32;

  return 0;
}

int main() {
  int i, j, r;

  unsigned char plain[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                             0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                             0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                             0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  unsigned char key[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                           0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  unsigned char cipher[32];

  Crypt_Enc_Block(plain, 32, cipher, key);
  for (i = 0; i < 32; i++) {
    printf("%02x ", cipher[i]);
  }
  printf("\n");

  Crypt_Dec_Block(cipher, 32, plain, key);
  for (i = 0; i < 32; i++) {
    printf("%02x ", plain[i]);
  }
  printf("\n");

  u_int32_t op, ed;
  double times, total = 0;
  u_int32_t total_bits = 2560000000;
  printf("10000000-round-Encryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 10000000; r++) {
      Crypt_Enc_Block(plain, 32, cipher, key);
    }
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n",
           static_cast<double>(total_bits / (times * 1024 * 1024)));
    total += static_cast<double>(total_bits / (times * 1024 * 1024));
  }

  for (i = 0; i < 32; i++) {
    printf("%02x ", cipher[i]);
  }

  printf("\n");

  printf("\n10000000-round-Encryption average speed: %f mbps\n\n", total / 10);

  total = 0;

  printf("10000000-round-Decryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 10000000; r++) {
      Crypt_Dec_Block(cipher, 32, plain, key);
    }
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n",
           static_cast<double>(total_bits / (times * 1024 * 1024)));
    total += static_cast<double>(total_bits / (times * 1024 * 1024));
  }

  printf("\n10000000-round-Decryption average speed: %f mbps\n\n", total / 10);

  for (i = 0; i < 32; i++) {
    printf("%02x ", plain[i]);
  }
  printf("\n");

  // ///////////------------(1 + n) long text test----------/////////////// //

  printf("\n\nlong-text-Encryption:\n");
  u_int32_t total_mb = 2048;
  op = clock();
  for (i = 0; i < 1024; i++)
    Crypt_Enc_Block(long_plain_1, 262144, long_cipher_1, key);
  ed = clock();
  times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
  printf("%fs\n", times);
  printf("%f mbps\n", static_cast<double>(total_mb / times));

  printf("cipher_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_cipher_1[i]);
  printf("\n");

  printf("\n\nlong-text-Decryption:\n");
  op = clock();
  for (i = 0; i < 1024; i++)
    Crypt_Dec_Block(long_cipher_1, 262144, long_plain_1, key);
  ed = clock();
  times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
  printf("%fs\n", times);
  printf("%f mbps\n", static_cast<double>(total_mb / times));

  printf("plain_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_plain_1[i]);
  printf("\n");

  // ///////////------------CBC test----------/////////////// //

  printf("\n\nCBC-Encryption:\n");
  op = clock();
  for (i = 0; i < 1024; i++)
    Crypt_Enc_Block_CBC(long_plain_1, 262144, long_cipher_1, &j, key);
  ed = clock();
  times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
  printf("%fs\n", times);
  printf("%f mbps\n", static_cast<double>(total_mb / times));

  printf("cipher_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_cipher_1[i]);
  printf("\n");

  printf("\n\nCBC-Decryption:\n");
  op = clock();
  for (i = 0; i < 1024; i++)
    Crypt_Dec_Block_CBC(long_cipher_1, 262144, long_plain_1, &j, key);
  ed = clock();
  times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
  printf("%fs\n", times);
  printf("%f mbps\n", static_cast<double>(total_mb / times));

  printf("plain_1_sample:\n");
  for (i = 0; i < 64; i++) printf("%02x ", long_plain_1[i]);
  printf("\n");

  return 0;
}
