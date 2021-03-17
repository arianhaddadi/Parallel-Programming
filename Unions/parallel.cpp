#include <stdio.h>
#include <iostream>
#include <x86intrin.h>

#ifdef 		_WIN32					//  Windows
#define 	cpuid    __cpuid
#else								//  Linux
void cpuid(int CPUInfo[4], int InfoType){
	__asm__ __volatile__(
		"cpuid":
	    "=a" (CPUInfo[0]),
		"=b" (CPUInfo[1]),
		"=c" (CPUInfo[2]),
		"=d" (CPUInfo[3]) :
		"a" (InfoType)
		);
}
#endif

typedef union
{
	__m128 float128;

	float m128_f32[4];
} floatVec;

typedef union
{
	__m128i int128;

	unsigned char m128_u8[16];
	signed char m128_i8[16];

	unsigned short m128_u16[8];
	signed short m128_i16[8];

	unsigned int m128_u32[4];
	signed int m128_i32[4];

	unsigned long m128_u64[2];
	signed long m128_i64[2];

} intVec;

void printSystemInfo() {
	printf("CPU Info:\n");
	system("lscpu | grep \"Model name\"");
	system("lscpu | grep \"CPU(s)\" -m1");
	printf("-------------\n");
}

void printSIMDSupportInfo() {
	int info[4];
	bool MMX   = false;
	bool SSE   = false;
	bool SSE2  = false;
	bool SSE3  = false;
	bool AES   = false;
	bool SSE41 = false;
	bool SSE42 = false;
	bool AVX   = false;
	bool AVX2  = false;
	bool SHA   = false;

	cpuid(info, 0x00000001);

	MMX   = (info[3] & ((int)1 << 23)) != 0;

	SSE   = (info[3] & ((int)1 << 25)) != 0;
	SSE2  = (info[3] & ((int)1 << 26)) != 0;
	SSE3  = (info[2] & ((int)1 << 0))  != 0;
	AES   = (info[2] & ((int)1 << 25)) != 0;
	SSE41 = (info[2] & ((int)1 << 19)) != 0;
	SSE42 = (info[2] & ((int)1 << 20)) != 0;

	AVX   = (info[2] & ((int)1 << 28)) != 0;

	cpuid(info, 0x80000000);
	if (info[0] >= 0x00000007){
		cpuid(info, 0x00000007);
		AVX2   = (info[1] & ((int)1 <<  5)) != 0;
		SHA    = (info[1] & ((int)1 << 29)) != 0;
	}

	printf("Intel SIMD Support Info:\n");
	printf("%s\n", MMX   ? "MMX   Supported" : "MMX   NOT Supported");
	printf("%s\n", SSE   ? "SSE   Supported" : "SSE   NOT Supported");
	printf("%s\n", SSE2  ? "SSE2  Supported" : "SSE2  NOT Supported");
	printf("%s\n", SSE3  ? "SSE3  Supported" : "SSE3  NOT Supported");
	printf("%s\n", SSE41 ? "SSE41 Supported" : "SSE41 NOT Supported");
	printf("%s\n", SSE42 ? "SSE42 Supported" : "SSE42 NOT Supported");
	printf("%s\n", AES   ? "AES   Supported" : "AES   NOT Supported");
	printf("%s\n", SHA   ? "SHA   Supported" : "SHA   NOT Supported");
	printf("%s\n", AVX   ? "AVX   Supported" : "AVX   NOT Supported");
	printf("%s\n", AVX2  ? "AVX2  Supported" : "AVX2  NOT Supported");
	printf("-------------\n");
}

void printGroupMemberNames() {
	printf("Group Members:\n");
	printf("1 - Aryan Haddadi 810196448\n");
	printf("2 - Iman Moradi 810196560\n");
	printf("-------------\n");
}


void Q1() {
	printGroupMemberNames();
	printSystemInfo();
	printSIMDSupportInfo();
}


void print_spfp_vector(__m128 float128) {
	floatVec fvec;
	fvec.float128 = float128;

	printf("Floating point numbers are: [");
	for(int i = 0; i < 3; i++) {
		printf("%.2f,", fvec.m128_f32[i]);
	}
	printf("%.2f]\n", fvec.m128_f32[3]);
}

void print_int_vector(__m128i int128, unsigned char type) {
	intVec ivec;
	ivec.int128 = int128;
	
	if (type == 1) {
		printf("Unsigned Byte: [");
		for(int i = 15; i > 0; i--) {
			printf("%X, ", ivec.m128_u8[i]);
		}
		printf("%X]\n", ivec.m128_u8[0]);
	}
	else if(type == 2) {
		printf("Signed Byte: [");
		for(int i = 15; i > 0; i--) {
			printf("%d, ", ivec.m128_i8[i]);
		}
		printf("%d]\n", ivec.m128_i8[0]);
	}
	else if(type == 3) {
		printf("Unsigned Word: [");
		for(int i = 7; i > 0; i--) {
			printf("%X, ", ivec.m128_u16[i]);
		}
		printf("%X]\n", ivec.m128_u16[0]);
	}
	else if(type == 4) {
		printf("Signed Word: [");
		for(int i = 7; i > 0; i--) {
			printf("%d, ", ivec.m128_i16[i]);
		}
		printf("%d]\n", ivec.m128_i16[0]);
	}
	else if(type == 5) {
		printf("Unsigned Double Word: [");
		for(int i = 3; i > 0; i--) {
			printf("%X, ", ivec.m128_u32[i]);
		}
		printf("%X]\n", ivec.m128_u32[0]);
	}
	else if(type == 6) {
		printf("Signed Double Word: [");
		for(int i = 3; i > 0; i--) {
			printf("%d, ", ivec.m128_i32[i]);
		}
		printf("%d]\n", ivec.m128_i32[0]);
	}
	else if(type == 7) {
		printf("Unsigned Quad Word: [");
		for(int i = 1; i > 0; i--) {
			printf("%lX, ", ivec.m128_u64[i]);
		}
		printf("%lX]\n", ivec.m128_u64[0]);
	}
	else if(type == 8) {
		printf("Signed Quad Word: [");
		for(int i = 1; i > 0; i--) {
			printf("%ld, ", ivec.m128_i64[i]);
		}
		printf("%ld]\n", ivec.m128_i64[0]);

	}	
}


int main() {
	Q1();

	// print int vector
	unsigned char intArray [16] = {	0X00, 0X11, 0X22, 0X33, 0X44, 0X55, 0X66, 0X77,
									0X88, 0X99, 0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0XFF};
	__m128i int128;
	int128 = _mm_load_si128((const __m128i*)intArray);
	for (int i = 1; i <= 8; i += 2) {
		print_int_vector(int128, i);
	}
	for (int i = 2; i <= 8; i += 2) {
		print_int_vector(int128, i);
	}
	printf("-------------\n");


	// print 4 floating point numbers
	float f_arr[4] = {32.5, 45.67, 123.45, 89.65}; 
	__m128 float128;
	float128 = _mm_load_ps(f_arr);
	print_spfp_vector(float128);
}