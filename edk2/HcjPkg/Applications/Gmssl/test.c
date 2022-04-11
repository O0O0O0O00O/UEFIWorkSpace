#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmssl/pem.h>
#include <gmssl/error.h>
#include <gmssl/hex.h>
#include <gmssl/sm2.h>
#include <gmssl/pkcs8.h>
#include <gmssl/x509.h>

int main(){
    char *pub_key_info = "MFkwEwYHKoZIzj0CAQYIKoEcz1UBgi0DQgAEiyi4Nz8Myp0ItdxV3TbpKIVklKT7HsuQxHSU3C+K/PjoDgRVYJFjt3MYu9I0snrUt1j4lzTuTXpIVt+wyjJsrA==";
    uint8_t inbuf[SM2_MAX_PLAINTEXT_SIZE];
	uint8_t outbuf[SM2_MAX_CIPHERTEXT_SIZE];
	size_t inlen, outlen = sizeof(outbuf);
    BASE64_CTX ctx;
    size_t len = 0;
    uint8_t data[512];
    const uint8_t *cp = data;
    

    char *pri_key_info = "MIGHAgEAMBMGByqGSM49AgEGCCqBHM9VAYItBG0wawIBAQQgFT5SQ6+xtFiOEjzz3hzrj/fw2L1mFfe+fKaEIyDy4DehRANCAASLKLg3PwzKnQi13FXdNukohWSUpPsey5DEdJTcL4r8+OgOBFVgkWO3cxi70jSyetS3WPiXNO5NekhW37DKMmys";
    base64_decode_init(&ctx);
    base64_decode_update(&ctx, (uint8_t *)pri_key_info, strlen(pri_key_info), data, &len);
    for(int i = 0 ; i < len; ++i){
        printf("(byte)0x%02x,", data[i]);
    }
    printf("\n");



    // base64_decode_init(&ctx);
    // base64_decode_update(&ctx, (uint8_t *)pub_key_info, strlen(pub_key_info), data, &len);
    // printf("key:\n");
    // for(int i = 0; i < len; ++i)
    //     printf("0x%02x ", data[i]);
    // printf("\n");
    // printf("%d\n", len);
    // SM2_KEY pub_key;
    // sm2_public_key_info_from_der(&pub_key, &cp, &len);//byte转成SM2_KEY

    // sprintf((char*)inbuf, "123");
    // inlen = strlen((char *)inbuf);
    // printf("inlen: %d\n", inlen);
    // sm2_encrypt(&pub_key, inbuf, inlen, outbuf, &outlen);
    // for(int i = 0; i < outlen; ++i)
    //     printf("(byte)0x%02x,", outbuf[i]);
    // printf("\n");
    // printf("outputlen:%d\n", outlen);


    


    return 0;

}