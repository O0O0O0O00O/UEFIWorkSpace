#include "MyHelloWorldSMBios/get_info.h"
#include "SM3/sm3.h"
#include "SM4/sm4.h"
// #include "NetWork/hcj_network.h"
#include "EchoTcp4/EchoTcp4.h"
#include <string.h>


int main(int Argc, char **Argv){
    //get infomation
    cJSON* json = get_info();
    char *buf = cJSON_Print(json);
    int buf_length = strlen(buf);
    printf("get infomation complete.\n");
    //sm3 paration
    unsigned char Hash[32] = {0};
    SM3(buf, buf_length, Hash);
    char hcj_hash[65];
    StringToChar(Hash, hcj_hash);
    hcj_hash[64] = '\0';
    cJSON_AddItemToObject(json, "hash", cJSON_CreateString(hcj_hash));
    buf = cJSON_Print(json);
    buf_length = strlen(buf);

    int temp = buf_length%16;
    
    
    for(int i = temp; i < 16; ++i){
        strcat(buf, " ");
    }
    buf_length = strlen(buf);
    printf("buf_length:%d\n", buf_length);
    printf("%s\n", buf);

    // printf("2jinzhi\n");
    // for(int i = 0; i < buf_length; ++i){
    //     printf("%02x ", buf[i]);
    // }
    // printf("\n");
    // // strncat(buf, Hash, 32);


    //是否可以用一个结构体传递 TODO
    //SM4
    // unsigned char output[4096];
    // memset(output, 0, sizeof(output));
    // unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
    // sm4_context ctx;
    // printf("encrypted:\n");
    // sm4_setkey_enc(&ctx, key);
    // sm4_crypt_ecb(&ctx, 1, buf_length, buf, output);
    // for(int i = 0; i < buf_length; ++i){
    //     printf("%02x ", output[i]);
    // }
    // printf("\n");
    // printf("encrypt finished\n");

    // printf("decrypted:\n");
    // sm4_setkey_dec(&ctx, key);
    // sm4_crypt_ecb(&ctx, 0, buf_length, output, output);
    // for(int i = 0; i < buf_length; ++i){
    //     printf("%c", output[i]);
    // }
    // printf("\n");


    //发送消息
    // CHAR8 *RecvBuffer = (CHAR8*) malloc(1024);
    // UINTN recvLen = 0;
    // SendMessage(Argc, Argv, output, buf_length, RecvBuffer, &recvLen);
    // RecvBuffer[recvLen] = '\0';
    // printf("Message from server: %s\n", RecvBuffer);
    return 0;
}