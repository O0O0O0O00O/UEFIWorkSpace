/*
 * SM4/SMS4 algorithm test programme
 * 2012-4-21
 */

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sm4.h"



const unsigned char allChar[63] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";


void generateString(unsigned char *dest, const unsigned int len){
	unsigned int cnt, randNo;
	srand((unsigned int)time(0));

	for(cnt = 0; cnt < len; ++cnt){
		randNo = rand()%62;
		*dest = allChar[randNo];
		dest++;
	}
}


int main(int argc, char *argv[])
{

	unsigned char key[16];
	memset(key, 0, sizeof(key));
	generateString(key, 16);
	printf("key:\n");
	for(int i = 0; i < 16; ++i){
		printf("0x%02x, ", key[i]);
	}
	printf("\n");
	// unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	//unsigned char input[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char input[1024];
	unsigned char output[1024];
	memset(input, 0, sizeof(input));
	memset(output, 0, sizeof(output));
	sm4_context ctx;
	unsigned long i;
	printf("please input message\n");
	gets(input);
	for(int i = 0; i < 100; ++i)
		printf("%02x ", input[i]);

	printf("encrypted:\n");
	sm4_setkey_enc(&ctx, key);
	sm4_crypt_ecb(&ctx, 1, strlen(input), input, output);
	for(i = 0; i < strlen(input); ++i){
		printf("%02x ", output[i]);
	}
	printf("\n");

	printf("decrypted:\n");
	sm4_setkey_dec(&ctx, key);
	sm4_crypt_ecb(&ctx, 0, strlen(input), output, output);
	for(i = 0; i < strlen(input); ++i){
		printf("%02x ", output[i]);
	}
	printf("\n");

	// printf("key:\n");
	// for(int i = 0; i < 16; ++i)
	// 	printf("%d ", key[i]);
	// printf("\n");

	//encrypt standard testing vector
	// printf("encrypted:\n");
	// sm4_setkey_enc(&ctx,key);
	// sm4_crypt_ecb(&ctx,1,16,input,output);
	// for(i=0;i<16;i++)
	// 	printf("%02x ", output[i]);
	// printf("\n");

	// for(i=0;i<16;i++)
	// 	printf("%c ", output[i]);
	// printf("\n");

	//decrypt testing
	// printf("decrypt:\n");
	// sm4_setkey_dec(&ctx,key);
	// sm4_crypt_ecb(&ctx,0,16,output,output);
	// for(i=0;i<16;i++)
	// 	printf("%02x ", output[i]);
	// printf("\n");

	// for(i=0;i<16;i++)
	// 	printf("%c ", output[i]);
	// printf("\n");

	
	
    return 0;
}
