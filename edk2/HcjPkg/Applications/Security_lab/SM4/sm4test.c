/*
 * SM4/SMS4 algorithm test programme
 * 2012-4-21
 */

#include "hcj_sm4.h"

int hcj_sm4_encrypt(unsigned char *key, unsigned char *input, unsigned char *output, int len)
{
	// unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	// unsigned char input[1024];
	// unsigned char output[1024];
	// memset(input, 0, sizeof(input));
	memset(output, 0, sizeof(output));
	sm4_context ctx;
	sm4_setkey_enc(&ctx, key);
	sm4_crypt_ecb(&ctx, 1, len, input, output);

	// printf("decrypted:\n");
	// sm4_setkey_dec(&ctx, key);
	// sm4_crypt_ecb(&ctx, 0, strlen(input), output, output);
	// for(i = 0; i < strlen(input); ++i){
	// 	printf("%02x ", output[i]);
	// }
	
    return 0;
}


int hcj_sm4_decrypt(unsigned char *key, unsigned char *input, unsigned char *output, int len){
	sm4_context ctx;
	sm4_setkey_dec(&ctx, key);
	sm4_crypt_ecb(&ctx, 0, len, input, output);
	return 0;
}