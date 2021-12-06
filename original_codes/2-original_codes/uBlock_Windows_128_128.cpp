
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <malloc.h>

#include <emmintrin.h>//sse2 header file(include sse header file)  
#include <pmmintrin.h> //SSE3(include emmintrin.h)  
#include <tmmintrin.h>//SSSE3(include pmmintrin.h)  
#include <smmintrin.h>//SSE4.1(include tmmintrin.h)  
#include <nmmintrin.h>//SSE4.2(include smmintrin.h)  
#include <intrin.h>//(include immintrin.h)  

__m128i S     = _mm_setr_epi8(0x7,0x4,0x9,0xc,0xb,0xa,0xd,0x8,0xf,0xe,0x1,0x6,0x0,0x3,0x2,0x5);
__m128i S_Inv = _mm_setr_epi8(0xc,0xa,0xe,0xd,0x1,0xf,0xb,0x0,0x7,0x2,0x5,0x4,0x3,0x6,0x9,0x8);

__m128i con = _mm_set1_epi8(0x0f);
__m128i A1 = _mm_setr_epi8(1,2,3,4,5,6,7,0,9,10,11,12,13,14,15,8);	
__m128i A2 = _mm_setr_epi8(2,3,4,5,6,7,0,1,10,11,12,13,14,15,8,9);		
__m128i A3 = _mm_setr_epi8(5,6,7,0,1,2,3,4,13,14,15,8,9,10,11,12);		

unsigned char Subkey[17][32];

unsigned char RC[16][16] = {0x9,0x8,0x8,0xc,0xc,0x9,0xd,0xd,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0xf,0x0,0xe,0x4,0xa,0x1,0xb,0x5,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x2,0x1,0x3,0x5,0x7,0x0,0x6,0x4,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x8,0x3,0x9,0x7,0xd,0x2,0xc,0x6,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0xc,0x7,0xd,0x3,0x9,0x6,0x8,0x2,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x4,0xf,0x5,0xb,0x1,0xe,0x0,0xa,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x5,0xe,0x4,0xa,0x0,0xf,0x1,0xb,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x7,0xc,0x6,0x8,0x2,0xd,0x3,0x9,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x3,0x9,0x2,0xd,0x6,0x8,0x7,0xc,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0xb,0x3,0xa,0x7,0xe,0x2,0xf,0x6,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0xa,0x7,0xb,0x3,0xf,0x6,0xe,0x2,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x8,0xe,0x9,0xa,0xd,0xf,0xc,0xb,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0xd,0xc,0xc,0x8,0x8,0xd,0x9,0x9,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x7,0x8,0x6,0xc,0x2,0x9,0x3,0xd,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x3,0x0,0x2,0x4,0x6,0x1,0x7,0x5,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0xa,0x1,0xb,0x5,0xf,0x0,0xe,0x4,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};


void uBlock_128_KeySchedule(unsigned char *key)
{
	int i;
	__m128i SK = _mm_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 3, 1, 7, 5, 11, 9, 15, 13);

	__m128i c1 = _mm_setr_epi8(0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80);	
	__m128i c2 = _mm_setr_epi8(0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7);		
	__m128i c3 = _mm_setr_epi8(8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15, 0x80);		
	__m128i c4 = _mm_setr_epi8(0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15);	

	__m128i c5 = _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);	
	__m128i c6 = _mm_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 8, 9, 10, 11, 12, 13, 14, 15);	
	__m128i c7 = _mm_setr_epi8(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7);	
	__m128i c8 = _mm_setr_epi8(6,0,8,13,1,15,5,10,4,9,12,2,11,3,7,14);	

	__m128i state1, state2, k, t1, t2, t3, t4;  

	state2 = _mm_loadu_si128((__m128i*)key);
	state1 = _mm_srli_epi16(state2, 4);
	state1 = _mm_and_si128(state1, con);
	state2 = _mm_and_si128(state2, con);
	t1 = _mm_shuffle_epi8(state1, c1);
	t2 = _mm_shuffle_epi8(state2, c2);
	t3 = _mm_shuffle_epi8(state1, c3);
	t4 = _mm_shuffle_epi8(state2, c4);
	state1 = _mm_xor_si128(t1, t2);
	state2 = _mm_xor_si128(t3, t4);

	_mm_storeu_si128((__m128i*)Subkey[0], state1);
	_mm_storeu_si128((__m128i*)(Subkey[0] + 16), state2);

	for (i = 1; i <= 16; i++)
	{
		state1 = _mm_shuffle_epi8(state1, c8);

		k = _mm_loadu_si128((__m128i*)RC[i - 1]);
		k = _mm_xor_si128(k, state1);
		t1 =  _mm_shuffle_epi8(S, k);
		t2 = _mm_shuffle_epi8(SK, k);
		t3 = _mm_shuffle_epi8(t1, c5);
		t4 = _mm_shuffle_epi8(t2, c6);
		k = _mm_xor_si128(t3, t4);

		k = _mm_xor_si128(k, state2);
		state2 = _mm_shuffle_epi8(state1, c7);
		state1 = k;

		_mm_storeu_si128((__m128i*)Subkey[i], state1);
		_mm_storeu_si128((__m128i*)(Subkey[i] + 16), state2);
	}

	return;
}

void uBlock_128_Encrypt(unsigned char *plain, unsigned char *cipher, int round)
{
	int i;
	__m128i L1 = _mm_setr_epi8(2,3,6,7,8,9,12,13,0,1,4,5,14,15,10,11);		
	__m128i L2 = _mm_setr_epi8(4,5,14,15,10,11,0,1,2,3,12,13,8,9,6,7);	

	__m128i c1 = _mm_setr_epi8(0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80);	
	__m128i c2 = _mm_setr_epi8(0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7);		
	__m128i c3 = _mm_setr_epi8(8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15, 0x80);		
	__m128i c4 = _mm_setr_epi8(0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15);	

	__m128i c5 = _mm_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);	
	__m128i c6 = _mm_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4, 6, 8, 10, 12, 14);		
	__m128i c7 = _mm_setr_epi8(1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);		
	__m128i c8 = _mm_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15);		

	__m128i state1, state2, k, t1, t2, t3, t4;  

	state2 = _mm_loadu_si128((__m128i*)plain);
	state1 = _mm_srli_epi16(state2, 4);
	state1 = _mm_and_si128(state1, con);
	state2 = _mm_and_si128(state2, con);
	t1 = _mm_shuffle_epi8(state1, c1);
	t2 = _mm_shuffle_epi8(state2, c2);
	t3 = _mm_shuffle_epi8(state1, c3);
	t4 = _mm_shuffle_epi8(state2, c4);
	state1 = _mm_xor_si128(t1, t2);
	state2 = _mm_xor_si128(t3, t4);

	for (i = 0; i < round; i++)
	{
		k = _mm_loadu_si128((__m128i*)Subkey[i]);
		state1 = _mm_xor_si128(state1, k);
		k = _mm_loadu_si128((__m128i*)(Subkey[i] + 16));
		state2 = _mm_xor_si128(state2, k);

		state1 = _mm_shuffle_epi8(S, state1);
		state2 = _mm_shuffle_epi8(S, state2);

		state2 = _mm_xor_si128(state2, state1);
		k = _mm_shuffle_epi8(state2, A1);
		state1 = _mm_xor_si128(state1, k);
		k = _mm_shuffle_epi8(state1, A2);
		state2 = _mm_xor_si128(state2, k);
		k = _mm_shuffle_epi8(state2, A2);
		state1 = _mm_xor_si128(state1, k);
		k = _mm_shuffle_epi8(state1, A3);
		state2 = _mm_xor_si128(state2, k);
		state1 = _mm_xor_si128(state1, state2);

		state1 = _mm_shuffle_epi8(state1, L1);
		state2 = _mm_shuffle_epi8(state2, L2);
	}

	if(round == 16)
	{
		k = _mm_loadu_si128((__m128i*)Subkey[round]);
		state1 = _mm_xor_si128(state1, k);
		k = _mm_loadu_si128((__m128i*)(Subkey[round] + 16));
		state2 = _mm_xor_si128(state2, k);
	}

	t1 = _mm_shuffle_epi8(state1, c5);
	t2 = _mm_shuffle_epi8(state2, c6);
	t3 = _mm_shuffle_epi8(state1, c7);
	t4 = _mm_shuffle_epi8(state2, c8);

	state1 = _mm_xor_si128(t1, t2);
	state2 = _mm_xor_si128(t3, t4);

	state1 = _mm_slli_epi16(state1, 4);
	state1 = _mm_xor_si128(state1, state2);

	_mm_storeu_si128((__m128i*)cipher, state1);

	return;
}

void uBlock_128_Decrypt(unsigned char *cipher, unsigned char *plain, int round)
{
	int i;
	__m128i L1 = _mm_setr_epi8(8,9,0,1,10,11,2,3,4,5,14,15,6,7,12,13);		
	__m128i L2 = _mm_setr_epi8(6,7,8,9,0,1,14,15,12,13,4,5,10,11,2,3);	

	__m128i c1 = _mm_setr_epi8(0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80);	
	__m128i c2 = _mm_setr_epi8(0x80, 0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7);		
	__m128i c3 = _mm_setr_epi8(8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15, 0x80);		
	__m128i c4 = _mm_setr_epi8(0x80, 8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15);	

	__m128i c5 = _mm_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);	
	__m128i c6 = _mm_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0, 2, 4, 6, 8, 10, 12, 14);		
	__m128i c7 = _mm_setr_epi8(1, 3, 5, 7, 9, 11, 13, 15, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);		
	__m128i c8 = _mm_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5, 7, 9, 11, 13, 15);		

	__m128i state1, state2, k, t1, t2, t3, t4;  

	state2 = _mm_loadu_si128((__m128i*)cipher);
	state1 = _mm_srli_epi16(state2, 4);
	state1 = _mm_and_si128(state1, con);
	state2 = _mm_and_si128(state2, con);
	t1 = _mm_shuffle_epi8(state1, c1);
	t2 = _mm_shuffle_epi8(state2, c2);
	t3 = _mm_shuffle_epi8(state1, c3);
	t4 = _mm_shuffle_epi8(state2, c4);
	state1 = _mm_xor_si128(t1, t2);
	state2 = _mm_xor_si128(t3, t4);

	for (i = round; i > 0; i--)
	{
		k = _mm_loadu_si128((__m128i*)Subkey[i]);
		state1 = _mm_xor_si128(state1, k);
		k = _mm_loadu_si128((__m128i*)(Subkey[i] + 16));
		state2 = _mm_xor_si128(state2, k);

		state1 = _mm_shuffle_epi8(state1, L1);
		state2 = _mm_shuffle_epi8(state2, L2);

		state1 = _mm_xor_si128(state1, state2);
		k = _mm_shuffle_epi8(state1, A3);
		state2 = _mm_xor_si128(state2, k);
		k = _mm_shuffle_epi8(state2, A2);
		state1 = _mm_xor_si128(state1, k);
		k = _mm_shuffle_epi8(state1, A2);
		state2 = _mm_xor_si128(state2, k);
		k = _mm_shuffle_epi8(state2, A1);
		state1 = _mm_xor_si128(state1, k);
		state2 = _mm_xor_si128(state2, state1);

		state1 = _mm_shuffle_epi8(S_Inv, state1);
		state2 = _mm_shuffle_epi8(S_Inv, state2);
	}

	k = _mm_loadu_si128((__m128i*)Subkey[0]);
	state1 = _mm_xor_si128(state1, k);
	k = _mm_loadu_si128((__m128i*)(Subkey[0] + 16));
	state2 = _mm_xor_si128(state2, k);

	t1 = _mm_shuffle_epi8(state1, c5);
	t2 = _mm_shuffle_epi8(state2, c6);
	t3 = _mm_shuffle_epi8(state1, c7);
	t4 = _mm_shuffle_epi8(state2, c8);

	state1 = _mm_xor_si128(t1, t2);
	state2 = _mm_xor_si128(t3, t4);

	state1 = _mm_slli_epi16(state1, 4);
	state1 = _mm_xor_si128(state1, state2);

	_mm_storeu_si128((__m128i*)plain, state1);

	return;
}

int Crypt_Enc_Block(unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen)
{
	int g;
	
	uBlock_128_KeySchedule(key);

	for (g = 0; g < in_len / 16; g++)
	{
		uBlock_128_Encrypt(input+g*16, output+g*16, 16);
	}
	*out_len = in_len - in_len % 16;

	return 0;
}

int Crypt_Dec_Block(unsigned char *input,int in_len, unsigned char *output,int *out_len, unsigned char *key, int keylen)
{
	int g;
	
	uBlock_128_KeySchedule(key);
	
	for (g = 0; g < in_len / 16; g++)
	{
		uBlock_128_Decrypt(input+g*16, output+g*16, 16);
	}
	*out_len = in_len - in_len % 16;
	
	return 0;
}

int Crypt_Enc_Block_Round(unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen, int cryptround)
{
	int g;
	
	uBlock_128_KeySchedule(key);
	
	for (g = 0; g < in_len / 16; g++)
	{
		uBlock_128_Encrypt(input+g*16, output+g*16, cryptround);
	}
	*out_len = in_len - in_len % 16;
	
	return 0;
}

int Key_Schedule(unsigned char *seedkey, int keylen, int direction, unsigned char *subkey)
{
   int i, j;
   
   uBlock_128_KeySchedule(seedkey);

   if(direction == 0)
   {
	   for(i = 0; i < 17; i++)
	   {
		   for(j = 0; j < 16; j++)
		   {
			   subkey[i * 16 + j] = (Subkey[i][2 * j] << 4) ^ Subkey[i][2 * j + 1];
		   }
	   }
   }

   if(direction == 1)
   {
	   for(i = 0; i < 17; i++)
	   {
		   for(j = 0; j < 16; j++)
		   {
			   subkey[i * 16 + j] = (Subkey[16 - i][2 * j] << 4) ^ Subkey[16 - i][2 * j + 1];
		   }
	   }
   }

   return 0;
}

int Crypt_Enc_Block_CBC(unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen)
{
	int g, j;
	unsigned char iv[16] = {0};
	
	uBlock_128_KeySchedule(key);

	for (g = 0; g < in_len / 16; g++)
	{
		for(j = 0; j < 16; j++)
		{
			iv[j] ^= input[g * 16 + j];
		}
		uBlock_128_Encrypt(iv, output+g*16, 16);
		memcpy(iv, output+g*16, 16);
	}
	*out_len = in_len - in_len % 16;

	return 0;
}

int Crypt_Dec_Block_CBC(unsigned char *input,int in_len, unsigned char *output,int *out_len, unsigned char *key, int keylen)
{
	int g, j;
	unsigned char iv[16] = {0};
	
	uBlock_128_KeySchedule(key);
	
	for (g = 0; g < in_len / 16; g++)
	{
		uBlock_128_Decrypt(input+g*16, output+g*16, 16);
		for(j = 0; j < 16; j++)
		{
			output[g * 16 + j] ^= iv[j];
		}
		memcpy(iv, input+g*16, 16);
	}
	*out_len = in_len - in_len % 16;
	
	return 0;
}


void main()
{
	int i, j, r;
	unsigned char key[16]   = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10};
	unsigned char plain[16]   = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10};
	unsigned char cipher[16], subkey[272];

	Crypt_Enc_Block(plain, 16, cipher, &j, key, 16);
	for(i = 0; i < 16; i++)
	{
		printf("%02x ", cipher[i]);
	}
	printf("\n");

	Crypt_Dec_Block(cipher, 16, plain, &j, key, 16);
	for(i = 0; i < 16; i++)
	{
		printf("%02x ", plain[i]);
	}
	printf("\n");

	Key_Schedule(key, 16, 0, subkey);
	for(i = 0; i < 17; i++)
	{
		for(j = 0; j < 16; j++)
		{
			printf("%02x ", subkey[i * 16 + j]);
		}
		printf("\n");
	}

	for(r = 1; r <= 16; r++)
	{
		Crypt_Enc_Block_Round(plain, 16, cipher, &j, key, 16, r);	
		for(i = 0; i < 16; i++)
		{
			printf("%02x ", cipher[i]);
		}
		printf("\n");
	}

	for(r = 0; r < 1000000; r++)
	{
		Crypt_Enc_Block(plain, 16, cipher, &j, key, 16);	
		memcpy(plain, cipher, 16);
	}
	for(i = 0; i < 16; i++)
	{
		printf("%02x ", cipher[i]);
	}
	printf("\n");

	return;
}
