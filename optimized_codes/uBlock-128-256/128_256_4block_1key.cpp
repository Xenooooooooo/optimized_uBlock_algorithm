// Copyright (c) 2022 by the author. All rights reserved.
//
// Redistribution and use in source and binary forms
// are permitted under the MIT license.

#include <immintrin.h>

#include <cstdio>
#include <ctime>

unsigned char Subkey_1[25][32], Subkey_2[25][32];

#define _mm256_loadu2_m128i(__PH, __PL)                                  \
  _mm256_insertf128_si256(_mm256_castsi128_si256(_mm_loadu_si128(__PL)), \
                          _mm_loadu_si128(__PH), 1)
#define _mm256_storeu2_m128i(__PH, __PL, __A)                 \
  {                                                           \
    _mm_storeu_si128(__PL, _mm256_castsi256_si128(__A));      \
    _mm_storeu_si128(__PH, _mm256_extractf128_si256(__A, 1)); \
  }

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

inline void uBlock_256_KeySchedule(unsigned char *key) {
  int i;

  __m256i S =
      _mm256_setr_epi8(7, 4, 9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5, 7,
                       4, 9, 12, 11, 10, 13, 8, 15, 14, 1, 6, 0, 3, 2, 5);
  __m256i SK =
      _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 3, 1, 7, 5, 11, 9, 15, 13, 0,
                       2, 4, 6, 8, 10, 12, 14, 3, 1, 7, 5, 11, 9, 15, 13);

  __m256i con = _mm256_set1_epi8(0x0f);

  __m256i c5_and_c5 = _mm256_setr_epi8(10, 5, 15, 0, 2, 7, 8, 13, 14, 6, 4, 12,
                                       1, 3, 11, 9, 26, 21, 31, 16, 18, 23, 24,
                                       29, 30, 22, 20, 28, 17, 19, 27, 25);
  __m256i c6_and_c6 = _mm256_setr_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3,
                                       4, 5, 6, 7, 24, 25, 26, 27, 28, 29, 30,
                                       31, 16, 17, 18, 19, 20, 21, 22, 23);

  __m256i state1, state2, state3, state4, k_1, k_2, t1, t2, t3, t4;

  state2 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(key));
  state1 = _mm256_srli_epi16(state2, 4);
  state1 = _mm256_and_si256(state1, con);
  state2 = _mm256_and_si256(state2, con);
  t1 = _mm256_unpacklo_epi8(state1, state2);
  t2 = _mm256_unpackhi_epi8(state1, state2);

  state1 = _mm256_permute2x128_si256(t1, t2, 0b00000000);
  state2 = _mm256_permute2x128_si256(t1, t2, 0b00100010);
  state3 = _mm256_permute2x128_si256(t1, t2, 0b00010001);
  state4 = _mm256_permute2x128_si256(t1, t2, 0b00110011);

  t3 = _mm256_slli_epi16(state1, 4);
  t3 = _mm256_xor_si256(state1, t3);
  t4 = _mm256_slli_epi16(state2, 4);
  t4 = _mm256_xor_si256(state2, t4);
  _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey_1[0]), t3);
  _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey_2[0]), t4);

  for (i = 1; i <= 24; i++) {
    state1 = _mm256_shuffle_epi8(state1, c5_and_c5);
    state2 = _mm256_shuffle_epi8(state2, c6_and_c6);

    k_1 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(RC[i - 1]));
    k_1 = _mm256_xor_si256(state1, k_1);
    k_1 = _mm256_shuffle_epi8(S, k_1);
    k_2 = _mm256_shuffle_epi8(SK, state2);
    k_1 = _mm256_xor_si256(k_1, state3);
    k_2 = _mm256_xor_si256(k_2, state4);

    state3 = state2;
    state4 = state1;
    state1 = k_1;
    state2 = k_2;

    t3 = _mm256_slli_epi16(state1, 4);
    t3 = _mm256_xor_si256(state1, t3);
    t4 = _mm256_slli_epi16(state2, 4);
    t4 = _mm256_xor_si256(state2, t4);
    _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey_1[i]), t3);
    _mm256_storeu_si256(reinterpret_cast<__m256i *>(Subkey_2[i]), t4);
  }
}

inline void uBlock_128_Encrypt(unsigned char *plain_1, unsigned char *plain_2,
                               unsigned char *cipher_1, unsigned char *cipher_2,
                               unsigned char *plain_3, unsigned char *plain_4,
                               unsigned char *cipher_3, unsigned char *cipher_4,
                               int round) {
  int i;

  __m256i state1, state2, state3, state4, state5, state6, t1, t2, t3, t4, k, k1,
      k2, k3, con = _mm256_set1_epi8(0x0f), con1 = _mm256_set1_epi8(0xf0);

  __m256i L1 = _mm256_setr_epi8(2, 3, 6, 7, 8, 9, 12, 13, 0, 1, 4, 5, 14, 15,
                                10, 11, 18, 19, 22, 23, 24, 25, 28, 29, 16, 17,
                                20, 21, 30, 31, 26, 27);
  __m256i L2 = _mm256_setr_epi8(4, 5, 14, 15, 10, 11, 0, 1, 2, 3, 12, 13, 8, 9,
                                6, 7, 20, 21, 30, 31, 26, 27, 16, 17, 18, 19,
                                28, 29, 24, 25, 22, 23);

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

  __m256i c5 =
      _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 16, 18, 20, 22, 24, 26, 28, 30, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c6 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4,
                       6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 16, 18, 20, 22, 24, 26, 28, 30);
  __m256i c7 =
      _mm256_setr_epi8(1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 17, 19, 21, 23, 25, 27, 29, 31, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c8 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5,
                       7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15);

  state2 = _mm256_loadu2_m128i(reinterpret_cast<__m128i *>(plain_1),
                               reinterpret_cast<__m128i *>(plain_2));
  state4 = _mm256_loadu2_m128i(reinterpret_cast<__m128i *>(plain_3),
                               reinterpret_cast<__m128i *>(plain_4));

  state1 = _mm256_srli_epi16(state2, 4);
  state3 = _mm256_srli_epi16(state4, 4);
  state1 = _mm256_and_si256(state1, con);
  state3 = _mm256_and_si256(state3, con);
  state2 = _mm256_and_si256(state2, con);
  state4 = _mm256_and_si256(state4, con);

  state3 = _mm256_slli_epi16(state3, 4);
  state4 = _mm256_slli_epi16(state4, 4);
  state1 = _mm256_xor_si256(state1, state3);
  state2 = _mm256_xor_si256(state2, state4);

  t1 = _mm256_unpacklo_epi8(state1, state2);
  t2 = _mm256_unpackhi_epi8(state1, state2);

  for (i = 0; i < round; i++) {
    k1 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_1[i]));
    t1 = _mm256_xor_si256(t1, k1);

    k1 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_2[i]));
    t2 = _mm256_xor_si256(t2, k1);

    // divide high low
    t3 = _mm256_and_si256(t1, con1);
    t4 = _mm256_and_si256(t2, con1);
    t3 = _mm256_srli_epi16(t3, 4);
    t4 = _mm256_srli_epi16(t4, 4);
    t1 = _mm256_and_si256(t1, con);
    t2 = _mm256_and_si256(t2, con);

    // S
    t1 = _mm256_shuffle_epi8(S, t1);
    t2 = _mm256_shuffle_epi8(S, t2);
    t3 = _mm256_shuffle_epi8(S, t3);
    t4 = _mm256_shuffle_epi8(S, t4);

    // merge high low
    t3 = _mm256_slli_epi16(t3, 4);
    t4 = _mm256_slli_epi16(t4, 4);
    t1 = _mm256_xor_si256(t1, t3);
    t2 = _mm256_xor_si256(t2, t4);

    t2 = _mm256_xor_si256(t2, t1);
    k = _mm256_shuffle_epi8(t2, A1);
    t1 = _mm256_xor_si256(t1, k);
    k = _mm256_shuffle_epi8(t1, A2);
    t2 = _mm256_xor_si256(t2, k);
    k = _mm256_shuffle_epi8(t2, A2);
    t1 = _mm256_xor_si256(t1, k);
    k = _mm256_shuffle_epi8(t1, A3);
    t2 = _mm256_xor_si256(t2, k);
    t1 = _mm256_xor_si256(t1, t2);

    t1 = _mm256_shuffle_epi8(t1, L1);
    t2 = _mm256_shuffle_epi8(t2, L2);
  }

  k1 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_1[i]));
  t1 = _mm256_xor_si256(t1, k1);

  k1 = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_2[i]));
  t2 = _mm256_xor_si256(t2, k1);

  state1 = _mm256_permute2x128_si256(t1, t2, 0b00100000);
  state2 = _mm256_permute2x128_si256(t1, t2, 0b00110001);

  t1 = _mm256_shuffle_epi8(state1, c5);
  state1 = _mm256_shuffle_epi8(state1, c7);
  t2 = _mm256_shuffle_epi8(state2, c6);
  state2 = _mm256_shuffle_epi8(state2, c8);

  state2 = _mm256_xor_si256(state1, state2);
  state1 = _mm256_xor_si256(t1, t2);

  state5 = _mm256_and_si256(state1, con);
  state6 = _mm256_and_si256(state2, con);
  state5 = _mm256_slli_epi16(state5, 4);
  state5 = _mm256_xor_si256(state5, state6);
  state5 = _mm256_permute4x64_epi64(state5, 0b11011000);
  _mm256_storeu2_m128i(reinterpret_cast<__m128i *>(cipher_1),
                       reinterpret_cast<__m128i *>(cipher_2), state5);

  state5 = _mm256_and_si256(state1, con1);
  state6 = _mm256_and_si256(state2, con1);
  state6 = _mm256_srli_epi16(state6, 4);
  state5 = _mm256_xor_si256(state5, state6);
  state5 = _mm256_permute4x64_epi64(state5, 0b11011000);
  _mm256_storeu2_m128i(reinterpret_cast<__m128i *>(cipher_3),
                       reinterpret_cast<__m128i *>(cipher_4), state5);
}

inline void uBlock_128_Decrypt(unsigned char *cipher_1, unsigned char *cipher_2,
                               unsigned char *plain_1, unsigned char *plain_2,
                               unsigned char *cipher_3, unsigned char *cipher_4,
                               unsigned char *plain_3, unsigned char *plain_4,
                               int round) {
  int i;

  __m256i L1 = _mm256_setr_epi8(8, 9, 0, 1, 10, 11, 2, 3, 4, 5, 14, 15, 6, 7,
                                12, 13, 24, 25, 16, 17, 26, 27, 18, 19, 20, 21,
                                30, 31, 22, 23, 28, 29);
  __m256i L2 = _mm256_setr_epi8(6, 7, 8, 9, 0, 1, 14, 15, 12, 13, 4, 5, 10, 11,
                                2, 3, 22, 23, 24, 25, 16, 17, 30, 31, 28, 29,
                                20, 21, 26, 27, 18, 19);

  __m256i A1 = _mm256_setr_epi8(1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14,
                                15, 8, 17, 18, 19, 20, 21, 22, 23, 16, 25, 26,
                                27, 28, 29, 30, 31, 24);
  __m256i A2 = _mm256_setr_epi8(2, 3, 4, 5, 6, 7, 0, 1, 10, 11, 12, 13, 14, 15,
                                8, 9, 18, 19, 20, 21, 22, 23, 16, 17, 26, 27,
                                28, 29, 30, 31, 24, 25);
  __m256i A3 = _mm256_setr_epi8(5, 6, 7, 0, 1, 2, 3, 4, 13, 14, 15, 8, 9, 10,
                                11, 12, 21, 22, 23, 16, 17, 18, 19, 20, 29, 30,
                                31, 24, 25, 26, 27, 28);

  __m256i S_Inv =
      _mm256_setr_epi8(12, 10, 14, 13, 1, 15, 11, 0, 7, 2, 5, 4, 3, 6, 9, 8, 12,
                       10, 14, 13, 1, 15, 11, 0, 7, 2, 5, 4, 3, 6, 9, 8);

  __m256i c5 =
      _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 16, 18, 20, 22, 24, 26, 28, 30, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c6 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4,
                       6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 16, 18, 20, 22, 24, 26, 28, 30);
  __m256i c7 =
      _mm256_setr_epi8(1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 17, 19, 21, 23, 25, 27, 29, 31, 0x80,
                       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
  __m256i c8 =
      _mm256_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5,
                       7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15);

  __m256i state1, state2, state3, state4, state5, state6, k, k1, k2, t1, t2, t3,
      t4, con = _mm256_set1_epi8(0x0f), con1 = _mm256_set1_epi8(0xf0);

  state2 = _mm256_loadu2_m128i(reinterpret_cast<__m128i *>(cipher_1),
                               reinterpret_cast<__m128i *>(cipher_2));
  state4 = _mm256_loadu2_m128i(reinterpret_cast<__m128i *>(cipher_3),
                               reinterpret_cast<__m128i *>(cipher_4));

  state1 = _mm256_srli_epi16(state2, 4);
  state3 = _mm256_srli_epi16(state4, 4);
  state1 = _mm256_and_si256(state1, con);
  state2 = _mm256_and_si256(state2, con);
  state3 = _mm256_and_si256(state3, con);
  state4 = _mm256_and_si256(state4, con);

  state3 = _mm256_slli_epi16(state3, 4);
  state4 = _mm256_slli_epi16(state4, 4);
  state1 = _mm256_xor_si256(state1, state3);
  state2 = _mm256_xor_si256(state2, state4);

  t1 = _mm256_unpacklo_epi8(state1, state2);
  t2 = _mm256_unpackhi_epi8(state1, state2);

  for (i = round; i > 0; i--) {
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_1[i]));
    t1 = _mm256_xor_si256(t1, k);
    k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_2[i]));
    t2 = _mm256_xor_si256(t2, k);

    t1 = _mm256_shuffle_epi8(t1, L1);
    t2 = _mm256_shuffle_epi8(t2, L2);

    t1 = _mm256_xor_si256(t2, t1);
    k = _mm256_shuffle_epi8(t1, A3);
    t2 = _mm256_xor_si256(t2, k);
    k = _mm256_shuffle_epi8(t2, A2);
    t1 = _mm256_xor_si256(t1, k);
    k = _mm256_shuffle_epi8(t1, A2);
    t2 = _mm256_xor_si256(t2, k);
    k = _mm256_shuffle_epi8(t2, A1);
    t1 = _mm256_xor_si256(t1, k);
    t2 = _mm256_xor_si256(t1, t2);

    // divide high low
    t3 = _mm256_and_si256(t1, con1);
    t4 = _mm256_and_si256(t2, con1);
    t3 = _mm256_srli_epi16(t3, 4);
    t4 = _mm256_srli_epi16(t4, 4);
    t1 = _mm256_and_si256(t1, con);
    t2 = _mm256_and_si256(t2, con);

    // S_Inv
    t1 = _mm256_shuffle_epi8(S_Inv, t1);
    t2 = _mm256_shuffle_epi8(S_Inv, t2);
    t3 = _mm256_shuffle_epi8(S_Inv, t3);
    t4 = _mm256_shuffle_epi8(S_Inv, t4);

    // merge high low
    t3 = _mm256_slli_epi16(t3, 4);
    t4 = _mm256_slli_epi16(t4, 4);
    t1 = _mm256_xor_si256(t1, t3);
    t2 = _mm256_xor_si256(t2, t4);
  }

  k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_1[i]));
  t1 = _mm256_xor_si256(t1, k);
  k = _mm256_loadu_si256(reinterpret_cast<__m256i *>(Subkey_2[i]));
  t2 = _mm256_xor_si256(t2, k);

  state1 = _mm256_permute2x128_si256(t1, t2, 0b00100000);
  state2 = _mm256_permute2x128_si256(t1, t2, 0b00110001);

  t1 = _mm256_shuffle_epi8(state1, c5);
  state1 = _mm256_shuffle_epi8(state1, c7);
  t2 = _mm256_shuffle_epi8(state2, c6);
  state2 = _mm256_shuffle_epi8(state2, c8);

  state2 = _mm256_xor_si256(state1, state2);
  state1 = _mm256_xor_si256(t1, t2);

  state5 = _mm256_and_si256(state1, con);
  state6 = _mm256_and_si256(state2, con);
  state5 = _mm256_slli_epi16(state5, 4);
  state5 = _mm256_xor_si256(state5, state6);
  state5 = _mm256_permute4x64_epi64(state5, 0b11011000);
  _mm256_storeu2_m128i(reinterpret_cast<__m128i *>(plain_1),
                       reinterpret_cast<__m128i *>(plain_2), state5);

  state5 = _mm256_and_si256(state1, con1);
  state6 = _mm256_and_si256(state2, con1);
  state6 = _mm256_srli_epi16(state6, 4);
  state5 = _mm256_xor_si256(state5, state6);
  state5 = _mm256_permute4x64_epi64(state5, 0b11011000);
  _mm256_storeu2_m128i(reinterpret_cast<__m128i *>(plain_3),
                       reinterpret_cast<__m128i *>(plain_4), state5);
}

inline int Crypt_Enc_Block(unsigned char *input_1, unsigned char *input_2,
                           unsigned char *input_3, unsigned char *input_4,
                           int in_len, unsigned char *output_1,
                           unsigned char *output_2, unsigned char *output_3,
                           unsigned char *output_4, unsigned char *key) {
  uBlock_256_KeySchedule(key);

  for (int g = 0; g < in_len / 16; g++)
    uBlock_128_Encrypt(input_1 + g * 16, input_2 + g * 16, output_1 + g * 16,
                       output_2 + g * 16, input_3 + g * 16, input_4 + g * 16,
                       output_3 + g * 16, output_4 + g * 16, 24);

  return 0;
}

inline int Crypt_Dec_Block(unsigned char *input_1, unsigned char *input_2,
                           unsigned char *input_3, unsigned char *input_4,
                           int in_len, unsigned char *output_1,
                           unsigned char *output_2, unsigned char *output_3,
                           unsigned char *output_4, unsigned char *key) {
  uBlock_256_KeySchedule(key);

  for (int g = 0; g < in_len / 16; g++)
    uBlock_128_Decrypt(input_1 + g * 16, input_2 + g * 16, output_1 + g * 16,
                       output_2 + g * 16, input_3 + g * 16, input_4 + g * 16,
                       output_3 + g * 16, output_4 + g * 16, 24);

  return 0;
}

void print(unsigned char *input_1, unsigned char *input_2,
           unsigned char *input_3, unsigned char *input_4, int len) {
  int i;

  printf("print_1:\n");
  for (i = 0; i < len; i++) printf("%02X ", input_1[i]);
  printf("\n");

  printf("print_2:\n");
  for (i = 0; i < len; i++) printf("%02X ", input_2[i]);
  printf("\n");

  printf("print_3:\n");
  for (i = 0; i < len; i++) printf("%02X ", input_3[i]);
  printf("\n");

  printf("print_4:\n");
  for (i = 0; i < len; i++) printf("%02X ", input_4[i]);
  printf("\n");
}
int main() {
  int i, j, r;
  unsigned char key[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                           0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

  unsigned char plain_1[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                               0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
  unsigned char plain_2[16] = {0x02, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                               0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
  unsigned char plain_3[16] = {0x03, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                               0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
  unsigned char plain_4[16] = {0x04, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                               0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

  unsigned char cipher_1[16], cipher_2[16], cipher_3[16], cipher_4[16];

  // /////////////-------(1 + 1) * n speed testing-------///////////////// //

  u_int32_t op, ed;
  double times, total = 0;
  u_int32_t total_bits = 1280000000;
  printf("10000000-round-Encryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 10000000; r++)
      Crypt_Enc_Block(plain_1, plain_2, plain_3, plain_4, 16, cipher_1,
                      cipher_2, cipher_3, cipher_4, key);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n", 4 * static_cast<double>(total_bits / (times * 1024 * 1024)));
    total += 4 * static_cast<double>(total_bits / (times * 1024 * 1024));
  }

  printf("\n10000000-round-Encryption average speed: %f mbps\n\n", total / 10);

  print(cipher_1, cipher_2, cipher_3, cipher_4, 16);

  total = 0;

  /////////////-----------------------------/////////////////

  printf("\n\n10000000-round-Decryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 10000000; r++)
      Crypt_Dec_Block(cipher_1, cipher_2, cipher_3, cipher_4, 16, plain_1,
                      plain_2, plain_3, plain_4, key);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n", 4 * static_cast<double>(total_bits / (times * 1024 * 1024)));
    total += 4 * static_cast<double>(total_bits / (times * 1024 * 1024));
  }

  printf("\n10000000-round-Decryption average speed: %f mbps\n\n", total / 10);

  print(plain_1, plain_2, plain_3, plain_4, 16);

  total = 0;

  // ///////////------------(1 + n) long text test----------/////////////// //

  printf("\n\nlong-text-Encryption:\n");
  u_int32_t total_mb = 8192;
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 1024; r++)
      Crypt_Enc_Block(long_plain_1, long_plain_2, long_plain_3, long_plain_4,
                      262144, long_cipher_1, long_cipher_2, long_cipher_3,
                      long_cipher_4, key);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n", static_cast<double>(total_mb / times));
    total +=static_cast<double>(total_mb / times);
  }

  printf("\nlong-text-Encryption average speed: %f mbps\n\n", total / 10);

  print(long_cipher_1, long_cipher_2, long_cipher_3, long_cipher_4, 32);

  total = 0;

  /////////////-----------------------------/////////////////

  printf("\n\nlong-text-Decryption:\n");
  for (i = 0; i < 10; i++) {
    op = clock();
    for (r = 0; r < 1024; r++)
      Crypt_Dec_Block(long_cipher_1, long_cipher_2, long_cipher_3,
                      long_cipher_4, 262144, long_plain_1, long_plain_2,
                      long_plain_3, long_plain_4, key);
    ed = clock();
    times = static_cast<double>(ed - op) / CLOCKS_PER_SEC;
    printf("%f\n", static_cast<double>(total_mb / times));
    total +=static_cast<double>(total_mb / times);
  }

  printf("\nlong-text-Decryption average speed: %f mbps\n\n", total / 10);

  print(long_plain_1, long_plain_2, long_plain_3, long_plain_4, 32);

  return 0;
}
