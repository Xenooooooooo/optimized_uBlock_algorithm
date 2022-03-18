#include <cstdio>
#include <ctime>
#include <immintrin.h>
#include <cstring>

unsigned char Subkey[25][64];

typedef unsigned long long uint;

#define ARRAY_SIZE_MAX (1024 * 256)

unsigned char long_plain_1[ARRAY_SIZE_MAX] = {1};
unsigned char long_cipher_1[ARRAY_SIZE_MAX];

unsigned char RC[24][32] = {0x99,0x88,0x88,0xcc,0xcc,0x99,0xdd,0xdd,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xff,0x00,0xee,0x44,0xaa,0x11,0xbb,0x55,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x22,0x11,0x33,0x55,0x77,0x00,0x66,0x44,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x88,0x33,0x99,0x77,0xdd,0x22,0xcc,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xcc,0x77,0xdd,0x33,0x99,0x66,0x88,0x22,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x44,0xff,0x55,0xbb,0x11,0xee,0x00,0xaa,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x55,0xee,0x44,0xaa,0x00,0xff,0x11,0xbb,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x77,0xcc,0x66,0x88,0x22,0xdd,0x33,0x99,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x33,0x99,0x22,0xdd,0x66,0x88,0x77,0xcc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xbb,0x33,0xaa,0x77,0xee,0x22,0xff,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xaa,0x77,0xbb,0x33,0xff,0x66,0xee,0x22,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x88,0xee,0x99,0xaa,0xdd,0xff,0xcc,0xbb,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xdd,0xcc,0xcc,0x88,0x88,0xdd,0x99,0x99,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x77,0x88,0x66,0xcc,0x22,0x99,0x33,0xdd,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x33,0x00,0x22,0x44,0x66,0x11,0x77,0x55,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xaa,0x11,0xbb,0x55,0xff,0x00,0xee,0x44,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x88,0x22,0x99,0x66,0xdd,0x33,0xcc,0x77,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xcc,0x55,0xdd,0x11,0x99,0x44,0x88,0x00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x44,0xaa,0x55,0xee,0x11,0xbb,0x00,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x55,0x55,0x44,0x11,0x00,0x44,0x11,0x00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x66,0xbb,0x77,0xff,0x33,0xaa,0x22,0xee,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x11,0x77,0x00,0x33,0x44,0x66,0x55,0x22,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0xee,0xff,0xff,0xbb,0xbb,0xee,0xaa,0xaa,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                            0x11,0xff,0x00,0xbb,0x44,0xee,0x55,0xaa,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

inline void uBlock_256_KeySchedule(unsigned char *key)
{
    int i;

    __m256i con  = _mm256_set1_epi8(0x0f);
    __m256i con1 = _mm256_set1_epi8(0xf0);
    __m256i PK   = _mm256_setr_epi8(10,5,15,0,2,7,8,13,1,14,4,12,9,11,3,6,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7);
    __m256i S_first_SK_second = _mm256_setr_epi8(0x7,0x4,0x9,0xc,0xb,0xa,0xd,0x8,0xf,0xe,0x1,0x6,0x0,0x3,0x2,0x5,0, 2, 4, 6, 8, 10, 12, 14, 3, 1, 7, 5, 11, 9, 15, 13);

    __m256i state1, state2, state3, state4, tmp1, tmp2, k;

    //divide high low
    state2 = _mm256_loadu_si256((__m256i*)key);
    state4 = _mm256_loadu_si256((__m256i*)(key+32));

    state1 = _mm256_srli_epi16(state2,4);
    state1 = _mm256_and_si256(state1, con);
    state2 = _mm256_and_si256(state2, con);
    state3 = _mm256_and_si256(state4, con1);
    state4 = _mm256_and_si256(state4, con);
    state4 = _mm256_slli_epi16(state4, 4);

    state1 = _mm256_xor_si256(state1, state3);
    state2 = _mm256_xor_si256(state2, state4);

    //high bits-> tmp1, low bits -> tmp2
    tmp1 = _mm256_unpacklo_epi8(state1, state2);
    tmp2 = _mm256_unpackhi_epi8(state1, state2);

    state1 = _mm256_permute2x128_si256(tmp1, tmp2, 0x20);
    state2 = _mm256_permute2x128_si256(tmp1, tmp2, 0x31);

    _mm256_storeu_si256((__m256i*)Subkey[0], state1);
    _mm256_storeu_si256((__m256i*)(Subkey[0] + 32), state2);

    for(i = 1; i <= 24; i++)
    {
        state1 = _mm256_shuffle_epi8(state1, PK);
        k = _mm256_loadu_si256((__m256i*)RC[i - 1]);
        k = _mm256_xor_si256(state1, k);

        //divide high low
        tmp1 = _mm256_and_si256(k, con);
        tmp2 = _mm256_and_si256(k, con1);
        tmp2 = _mm256_srli_epi16(tmp2, 4);

        //S & SK
        tmp1 = _mm256_shuffle_epi8(S_first_SK_second, tmp1);
        tmp2 = _mm256_shuffle_epi8(S_first_SK_second, tmp2);

        //merge high low
        tmp2 = _mm256_slli_epi16(tmp2, 4);
        k = _mm256_xor_si256(tmp1, tmp2);

        k = _mm256_xor_si256(k, state2);
        state2 = _mm256_permute4x64_epi64(state1, 0b01001110);
        state1 = k;

        _mm256_storeu_si256((__m256i*)Subkey[i],k);
        _mm256_storeu_si256((__m256i*)(Subkey[i]+32),state2);
    }
}

inline void uBlock_256_Encrypt(unsigned char *plain, unsigned char *cipher, int round)
{
    int i = 0;

    __m256i con  = _mm256_set1_epi8(0x0f);
    __m256i con1 = _mm256_set1_epi8(0xf0);

    __m256i c5_to_c8 = _mm256_setr_epi8(0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15);
    __m256i L1_front = _mm256_setr_epi8(0x74,0x75,0x7e,0x7f,0x80,0x81,0x8a,0x8b,0x76,0x77,0x7c,0x7d,0x82,0x83,0x88,0x89,0xf2,0xf3,0xf8,0xf9,0x0e,0x0f,0x04,0x05,0x0c,0x0d,0x06,0x07,0xfa,0xfb,0xf0,0xf1);
    __m256i L1_rear  = _mm256_setr_epi8(0xf4,0xf5,0xfe,0xff,0x00,0x01,0x0a,0x0b,0xf6,0xf7,0xfc,0xfd,0x02,0x03,0x08,0x09,0x72,0x73,0x78,0x79,0x8e,0x8f,0x84,0x85,0x8c,0x8d,0x86,0x87,0x7a,0x7b,0x70,0x71);
    __m256i L2_front = _mm256_setr_epi8(0x7c,0x7d,0x86,0x87,0x72,0x73,0x88,0x89,0x82,0x83,0x78,0x79,0x74,0x75,0x8e,0x8f,0xfe,0xff,0xf0,0xf1,0x0a,0x0b,0x04,0x05,0x0c,0x0d,0xf6,0xf7,0x00,0x01,0xfa,0xfb);
    __m256i L2_rear  = _mm256_setr_epi8(0xfc,0xfd,0x06,0x07,0xf2,0xf3,0x08,0x09,0x02,0x03,0xf8,0xf9,0xf4,0xf5,0x0e,0x0f,0x7e,0x7f,0x70,0x71,0x8a,0x8b,0x84,0x85,0x8c,0x8d,0x76,0x77,0x80,0x81,0x7a,0x7b);

    __m256i S  = _mm256_setr_epi8(0x7,0x4,0x9,0xc,0xb,0xa,0xd,0x8,0xf,0xe,0x1,0x6,0x0,0x3,0x2,0x5,0x7,0x4,0x9,0xc,0xb,0xa,0xd,0x8,0xf,0xe,0x1,0x6,0x0,0x3,0x2,0x5);
    __m256i A1 = _mm256_setr_epi8(1,2,3,4,5,6,7,0,9,10,11,12,13,14,15,8,17,18,19,20,21,22,23,16,25,26,27,28,29,30,31,24);
    __m256i A2 = _mm256_setr_epi8(2,3,4,5,6,7,0,1,10,11,12,13,14,15,8,9,18,19,20,21,22,23,16,17,26,27,28,29,30,31,24,25);
    __m256i A3 = _mm256_setr_epi8(5,6,7,0,1,2,3,4,13,14,15,8,9,10,11,12,21,22,23,16,17,18,19,20,29,30,31,24,25,26,27,28);

    __m256i state1, state2, k, t1, t2, t3, t4;
    __m256i hi_1, hi_2;

    //first block
    state2 = _mm256_loadu_si256((__m256i*)plain);
    state1 = _mm256_srli_epi16(state2,4);
    state1 = _mm256_and_si256(state1,con);
    state2 = _mm256_and_si256(state2,con);

    //two blocks: high bits-> t3, low bits -> t4
    t3=_mm256_unpacklo_epi8(state1,state2);
    t4=_mm256_unpackhi_epi8(state1,state2);

    state1=_mm256_permute2x128_si256(t3,t4,0x20);
    state2=_mm256_permute2x128_si256(t3,t4,0x31);

    //second block(stored in high bits)
    hi_2=_mm256_loadu_si256((__m256i*)(plain+32));
    hi_1=_mm256_srli_epi16(hi_2,4);
    hi_1=_mm256_and_si256(hi_1,con);
    hi_2=_mm256_and_si256(hi_2,con);

    //two blocks: high bits-> t3, low bits -> t4
    t3=_mm256_unpacklo_epi8(hi_1,hi_2);
    t4=_mm256_unpackhi_epi8(hi_1,hi_2);

    hi_1=_mm256_permute2x128_si256(t3, t4,0x20);
    hi_2=_mm256_permute2x128_si256(t3, t4,0x31);

    //merge high low
    //---------------------------------------------------------------------------
    hi_1=_mm256_slli_epi16(hi_1,4);
    hi_2=_mm256_slli_epi16(hi_2,4);
    state1=_mm256_xor_si256(state1,hi_1);
    state2=_mm256_xor_si256(state2,hi_2);
    //----------------------------------------------------------------------------

    for (i = 0; i < round; i++)
    {
        k = _mm256_loadu_si256((__m256i*)Subkey[i]);
        state1 = _mm256_xor_si256(state1, k);
        k = _mm256_loadu_si256((__m256i*)(Subkey[i] + 32));
        state2 = _mm256_xor_si256(state2, k);

        //divide high low
        //----------------------------------------------------------------------------
        hi_1=_mm256_and_si256(state1,con1);
        hi_2=_mm256_and_si256(state2,con1);
        hi_1=_mm256_srli_epi16(hi_1,4);
        hi_2=_mm256_srli_epi16(hi_2,4);
        state1=_mm256_and_si256(state1,con);
        state2=_mm256_and_si256(state2,con);
        //----------------------------------------------------------------------------

        //S
        state1 = _mm256_shuffle_epi8(S, state1);
        hi_1 = _mm256_shuffle_epi8(S, hi_1);
        state2 = _mm256_shuffle_epi8(S, state2);
        hi_2 = _mm256_shuffle_epi8(S, hi_2);

        //after S, merge high low bits together
        //----------------------------------------------------------------------------
        hi_1=_mm256_slli_epi16(hi_1,4);
        hi_2=_mm256_slli_epi16(hi_2,4);
        state1=_mm256_xor_si256(state1,hi_1);
        state2=_mm256_xor_si256(state2,hi_2);
        //----------------------------------------------------------------------------

        state2 = _mm256_xor_si256(state2, state1);

        k =  _mm256_shuffle_epi8(state2, A1);
        state1 = _mm256_xor_si256(state1, k);

        k = _mm256_shuffle_epi8(state1, A2);
        state2 = _mm256_xor_si256(state2, k);

        k = _mm256_shuffle_epi8(state2, A2);
        state1 = _mm256_xor_si256(state1, k);

        k = _mm256_shuffle_epi8(state1, A3);
        state2 = _mm256_xor_si256(state2, k);

        state1 = _mm256_xor_si256(state1, state2);

        t1=_mm256_shuffle_epi8(state1,L1_front);
        t3=_mm256_permute4x64_epi64(state1,0x4E);
        t2=_mm256_shuffle_epi8(t3,L1_rear);
        state1=_mm256_xor_si256(t1,t2);

        t1=_mm256_shuffle_epi8(state2,L2_front);
        t3=_mm256_permute4x64_epi64(state2,0x4E);
        t2=_mm256_shuffle_epi8(t3,L2_rear);
        state2=_mm256_xor_si256(t1,t2);
    }

    k = _mm256_loadu_si256((__m256i*)Subkey[round]);
    state1 = _mm256_xor_si256(state1, k);
    k = _mm256_loadu_si256((__m256i*)(Subkey[round] + 32));
    state2 = _mm256_xor_si256(state2, k);

    //------------------------------------------------------------------------
    //Simplified
    t1=_mm256_shuffle_epi8(state1,c5_to_c8);
    t2=_mm256_permute4x64_epi64(t1,0xd8);
    t1=_mm256_shuffle_epi8(state2,c5_to_c8);
    t3=_mm256_permute4x64_epi64(t1,0xd8);
    state1=_mm256_permute2x128_si256(t2,t3,0x20);
    state2=_mm256_permute2x128_si256(t2,t3,0x31);

    //------------------------------------------------------------------------
    hi_1=_mm256_and_si256(state1,con1);
    hi_2=_mm256_and_si256(state2,con1);
    state1=_mm256_and_si256(state1,con);
    state2=_mm256_and_si256(state2,con);

    //------------------------------------------------------------------------
    hi_2=_mm256_srli_epi16(hi_2,4);
    hi_1 = _mm256_xor_si256(hi_1, hi_2);
    state1 = _mm256_slli_epi16(state1, 4);
    state1 = _mm256_xor_si256(state1, state2);

    _mm256_storeu_si256((__m256i*)cipher, state1);
    _mm256_storeu_si256((__m256i*)(cipher+32), hi_1);
}

inline void uBlock_256_Decrypt(unsigned char *cipher, unsigned char *plain, int round)
{
    int i = 0;

    __m256i con  = _mm256_set1_epi8(0x0f);
    __m256i con1 = _mm256_set1_epi8(0xf0);

    __m256i c5_to_c8     = _mm256_setr_epi8(0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15);
    __m256i L1_Inv_front = _mm256_setr_epi8(0x8e,0x8f,0x80,0x81,0x70,0x71,0x78,0x79,0x82,0x83,0x8c,0x8d,0x7a,0x7b,0x72,0x73,0xf4,0xf5,0xfc,0xfd,0x06,0x07,0x0a,0x0b,0xfe,0xff,0xf6,0xf7,0x08,0x09,0x04,0x05);
    __m256i L1_Inv_rear  = _mm256_setr_epi8(0x0e,0x0f,0x00,0x01,0xf0,0xf1,0xf8,0xf9,0x02,0x03,0x0c,0x0d,0xfa,0xfb,0xf2,0xf3,0x74,0x75,0x7c,0x7d,0x86,0x87,0x8a,0x8b,0x7e,0x7f,0x76,0x77,0x88,0x89,0x84,0x85);
    __m256i L2_Inv_front = _mm256_setr_epi8(0x82,0x83,0x74,0x75,0x7c,0x7d,0x8a,0x8b,0x7a,0x7b,0x8e,0x8f,0x70,0x71,0x80,0x81,0x0c,0x0d,0xf8,0xf9,0x06,0x07,0xf2,0xf3,0xf6,0xf7,0x04,0x05,0x08,0x09,0xfe,0xff);
    __m256i L2_Inv_rear  = _mm256_setr_epi8(0x02,0x03,0xf4,0xf5,0xfc,0xfd,0x0a,0x0b,0xfa,0xfb,0x0e,0x0f,0xf0,0xf1,0x00,0x01,0x8c,0x8d,0x78,0x79,0x86,0x87,0x72,0x73,0x76,0x77,0x84,0x85,0x88,0x89,0x7e,0x7f);
    __m256i S_Inv = _mm256_setr_epi8(0xc,0xa,0xe,0xd,0x1,0xf,0xb,0x0,0x7,0x2,0x5,0x4,0x3,0x6,0x9,0x8,0xc,0xa,0xe,0xd,0x1,0xf,0xb,0x0,0x7,0x2,0x5,0x4,0x3,0x6,0x9,0x8);
    __m256i A1    = _mm256_setr_epi8(1,2,3,4,5,6,7,0,9,10,11,12,13,14,15,8,17,18,19,20,21,22,23,16,25,26,27,28,29,30,31,24);
    __m256i A2    = _mm256_setr_epi8(2,3,4,5,6,7,0,1,10,11,12,13,14,15,8,9,18,19,20,21,22,23,16,17,26,27,28,29,30,31,24,25);
    __m256i A3    = _mm256_setr_epi8(5,6,7,0,1,2,3,4,13,14,15,8,9,10,11,12,21,22,23,16,17,18,19,20,29,30,31,24,25,26,27,28);

    __m256i state1, state2, k, t1, t2, t3, t4;
    __m256i hi_1, hi_2;

    state2=_mm256_loadu_si256((__m256i*)cipher);
    state1=_mm256_srli_epi16(state2,4);
    state1=_mm256_and_si256(state1,con);
    state2=_mm256_and_si256(state2,con);

    t3=_mm256_unpacklo_epi8(state1,state2);
    t4=_mm256_unpackhi_epi8(state1,state2);

    state1=_mm256_permute2x128_si256(t3,t4,0x20);
    state2=_mm256_permute2x128_si256(t3,t4,0x31);

    hi_2=_mm256_loadu_si256((__m256i*)(cipher+32));
    hi_1=_mm256_srli_epi16(hi_2,4);
    hi_1=_mm256_and_si256(hi_1,con);
    hi_2=_mm256_and_si256(hi_2,con);

    t3=_mm256_unpacklo_epi8(hi_1,hi_2);
    t4=_mm256_unpackhi_epi8(hi_1,hi_2);

    hi_1=_mm256_permute2x128_si256(t3,t4,0x20);
    hi_2=_mm256_permute2x128_si256(t3,t4,0x31);

    //merge high low
    //----------------------------------------------------------------------------
    hi_1=_mm256_slli_epi16(hi_1,4);
    hi_2=_mm256_slli_epi16(hi_2,4);
    state1=_mm256_xor_si256(state1,hi_1);
    state2=_mm256_xor_si256(state2,hi_2);
    //----------------------------------------------------------------------------

    for (i = round; i > 0; i--)
    {
        k=_mm256_loadu_si256((__m256i*)Subkey[i]);
        state1=_mm256_xor_si256(state1,k);
        k=_mm256_loadu_si256((__m256i*)(Subkey[i]+32));
        state2=_mm256_xor_si256(state2,k);

        t1=_mm256_shuffle_epi8(state1,L1_Inv_front);
        t3=_mm256_permute4x64_epi64(state1,0x4E);
        t2=_mm256_shuffle_epi8(t3,L1_Inv_rear);
        state1=_mm256_xor_si256(t1,t2);

        t1=_mm256_shuffle_epi8(state2,L2_Inv_front);
        t3=_mm256_permute4x64_epi64(state2,0x4E);
        t2=_mm256_shuffle_epi8(t3,L2_Inv_rear);
        state2=_mm256_xor_si256(t1,t2);

        state1=_mm256_xor_si256(state2,state1);

        k=_mm256_shuffle_epi8(state1,A3);
        state2=_mm256_xor_si256(k,state2);

        k=_mm256_shuffle_epi8(state2,A2);
        state1=_mm256_xor_si256(k,state1);

        k=_mm256_shuffle_epi8(state1,A2);
        state2=_mm256_xor_si256(k,state2);

        k=_mm256_shuffle_epi8(state2,A1);
        state1=_mm256_xor_si256(k,state1);

        state2=_mm256_xor_si256(state2,state1);

        //divide high low
        //----------------------------------------------------------------------------
        hi_1=_mm256_and_si256(state1,con1);
        hi_2=_mm256_and_si256(state2,con1);
        hi_1=_mm256_srli_epi16(hi_1,4);
        hi_2=_mm256_srli_epi16(hi_2,4);
        state1=_mm256_and_si256(state1,con);
        state2=_mm256_and_si256(state2,con);
        //----------------------------------------------------------------------------

        //S_Inv
        state1=_mm256_shuffle_epi8(S_Inv,state1);
        hi_1=_mm256_shuffle_epi8(S_Inv,hi_1);
        state2=_mm256_shuffle_epi8(S_Inv, state2);
        hi_2=_mm256_shuffle_epi8(S_Inv, hi_2);

        //merge high low
        //----------------------------------------------------------------------------
        hi_1=_mm256_slli_epi16(hi_1,4);
        hi_2=_mm256_slli_epi16(hi_2,4);
        state1=_mm256_xor_si256(state1,hi_1);
        state2=_mm256_xor_si256(state2,hi_2);
    }

    k=_mm256_loadu_si256((__m256i*)Subkey[0]);
    state1=_mm256_xor_si256(state1,k);
    k=_mm256_loadu_si256((__m256i*)(Subkey[0]+32));
    state2=_mm256_xor_si256(state2,k);

    //----------------------------------------------------------------------------
    //Simplified

    t1=_mm256_shuffle_epi8(state1,c5_to_c8);
    t2=_mm256_permute4x64_epi64(t1,0xd8);
    t1=_mm256_shuffle_epi8(state2,c5_to_c8);
    t3=_mm256_permute4x64_epi64(t1,0xd8);
    state2=_mm256_permute2x128_si256(t2,t3,0x20);
    state1=_mm256_permute2x128_si256(t2,t3,0x31);

    //divide high low
    //----------------------------------------------------------------------------
    hi_1=_mm256_and_si256(state1,con1);
    hi_2=_mm256_and_si256(state2,con1);
    state1=_mm256_and_si256(state1,con);
    state2=_mm256_and_si256(state2,con);
    //----------------------------------------------------------------------------

    hi_1=_mm256_srli_epi16(hi_1,4);
    hi_1 = _mm256_xor_si256(hi_1, hi_2);
    state2=_mm256_slli_epi16(state2,4);
    state1=_mm256_xor_si256(state2,state1);

    _mm256_storeu_si256((__m256i *)plain,state1);
    _mm256_storeu_si256((__m256i*)(plain+32), hi_1);
}

inline int Crypt_Enc_Block(unsigned char *input, int in_len, unsigned char *output, unsigned char *key)
{
    uBlock_256_KeySchedule(key);

    for (int g = 0; g < in_len / 64; g++)
        uBlock_256_Encrypt(input+g*64, output+g*64, 24);
    return 0;
}

inline int Crypt_Dec_Block(unsigned char *input,int in_len, unsigned char *output, unsigned char *key)
{
    uBlock_256_KeySchedule(key);

    for (int g = 0; g < in_len / 64; g++)
        uBlock_256_Decrypt(input+g*64, output+g*64, 24);
    return 0;
}

inline int Crypt_Enc_Block_CBC(unsigned char *input, int in_len, unsigned char *output, unsigned char *key)
{
    int g, j;
    unsigned char iv[64] = {0};

    uBlock_256_KeySchedule(key);

    for (g = 0; g < in_len / 64; g++)
    {
        for(j = 0; j < 64; j++)
            iv[j] ^= input[g * 64 + j];
        uBlock_256_Encrypt(iv, output+g*64, 24);
        memcpy(iv, output+g*64, 64);
    }

    return 0;
}

inline int Crypt_Dec_Block_CBC(unsigned char *input,int in_len, unsigned char *output, unsigned char *key)
{
    int g, j;
    unsigned char iv[64] = {0};

    uBlock_256_KeySchedule(key);

    for (g = 0; g < in_len / 64; g++)
    {
        uBlock_256_Decrypt(input+g*64, output+g*64, 24);
        for(j = 0; j < 64; j++)
            iv[j] ^= input[g * 64 + j];
        memcpy(iv, output+g*64, 64);
    }

    return 0;
}


int main() {
    int i, r;

    unsigned char key[64] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32,
                             0x10, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
                             0x0e, 0x0f,
                             0x00, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32,
                             0x10, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
                             0x0e, 0x0f};
    unsigned char plain_1[64] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                                 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
    unsigned char cipher_1[64];

    printf("\n10000000-round-Encryption:\n");
    uint op, ed, total_bits = 2560000000;
    double times, total = 0;
    for(i = 0; i < 10; i++)
    {
        op = clock();
        for (r = 0; r < 10000000; r++)
            Crypt_Enc_Block(plain_1, 64, cipher_1, key);
        ed = clock();
        times = (double) (ed - op) / CLOCKS_PER_SEC;
        printf("%f\n", 2 * (double) total_bits / (times * 1024 * 1024));
        total += (double)2 * total_bits / (times * 1024 * 1024);
    }

    printf("cipher1:\n");
    for(i = 0; i < 32; i++)
        printf("%02X ", cipher_1[i]);
    printf("\n");
    printf("cipher2:\n");
    for(i = 0; i < 32; i++)
        printf("%02X ", cipher_1[i+32]);
    printf("\n");

    printf("\n10000000-round-Encryption average speed: %f mbps\n\n", total / 10);

    total = 0;

    printf("\n10000000-round-Decryption:\n");
    for(i = 0; i < 10; i++)
    {
        op = clock();
        for (r = 0; r < 10000000; r++)
            Crypt_Dec_Block(cipher_1, 64, plain_1, key);
        ed = clock();
        times = (double) (ed - op) / CLOCKS_PER_SEC;
        printf("%f\n", 2 * (double) total_bits / (times * 1024 * 1024));
        total += (double)2 * total_bits / (times * 1024 * 1024);
    }

    printf("plain1:\n");
    for(i = 0; i < 32; i++)
        printf("%02X ", plain_1[i]);
    printf("\n");
    printf("plain2:\n");
    for(i = 0; i < 32; i++)
        printf("%02X ", plain_1[i+32]);
    printf("\n");

    printf("\n10000000-round-Decryption average speed: %f mbps\n\n", total / 10);

    total = 0;

/////////////------------(1 + n) long text test----------/////////////////

    printf("\n\nlong-text-Encryption:\n");
    uint total_mb = 4096;
    for(i = 0; i < 10; i ++)
    {
        op = clock();
        for(r = 0; r < 2048; r ++)
            Crypt_Enc_Block(long_plain_1, 262144, long_cipher_1, key);
        ed = clock();
        times = (double)(ed - op) / CLOCKS_PER_SEC;
        printf("%f\n", (double)total_mb / times);
        total += (double)total_mb / times;
    }

    printf("\n10000000-round-Encryption average speed: %f mbps\n\n", total / 10);

    printf("long_cipher_1_sample:\n");
    for(i = 0; i < 64; i++)
        printf("%02X ", long_cipher_1[i]);
    printf("\n");

    total = 0;

    printf("\n\nlong-text-Decryption:\n");
    for(i = 0; i < 10; i ++)
    {
        op = clock();
        for(r = 0; r < 2048; r ++)
            Crypt_Dec_Block(long_cipher_1, 262144, long_plain_1, key);
        ed = clock();
        times = (double)(ed - op) / CLOCKS_PER_SEC;
        printf("%f\n", (double)total_mb / times);
        total += (double)total_mb / times;
    }

    printf("\n10000000-round-Decryption average speed: %f mbps\n\n", total / 10);

    printf("long_plain_1_sample:\n");
    for(i = 0; i < 64; i++)
        printf("%02X ", long_plain_1[i]);
    printf("\n");

    total = 0;

/////////////------------CBC long text test----------/////////////////

    printf("\n\nCBC-Encryption:\n");
    for(i = 0; i < 10; i ++)
    {
        op = clock();
        for(r = 0; r < 2048; r ++)
            Crypt_Enc_Block_CBC(long_plain_1, 262144, long_cipher_1, key);
        ed = clock();
        times = (double)(ed - op) / CLOCKS_PER_SEC;
        printf("%f\n", (double)total_mb / times);
        total += (double)total_mb / times;
    }

    printf("\nCBC-Encryption average speed: %f mbps\n\n", total / 10);

    printf("long_cipher_1_sample:\n");
    for(i = 0; i < 64; i++)
        printf("%02X ", long_cipher_1[i]);
    printf("\n");

    total = 0;

/////////////-----------------------------/////////////////

    printf("\n\nCBC-Decryption:\n");
    for(i = 0; i < 10; i ++)
    {
        op = clock();
        for(r = 0; r < 2048; r ++)
            Crypt_Dec_Block_CBC(long_cipher_1, 262144, long_plain_1, key);
        ed = clock();
        times = (double)(ed - op) / CLOCKS_PER_SEC;
        printf("%f\n", (double)total_mb / times);
        total += (double)total_mb / times;
    }

    printf("\nCBC-Decryption average speed: %f mbps\n\n", total / 10);

    printf("long_plain_1_sample:\n");
    for(i = 0; i < 64; i++)
        printf("%02X ", long_plain_1[i]);
    printf("\n");

    return 0;
}
