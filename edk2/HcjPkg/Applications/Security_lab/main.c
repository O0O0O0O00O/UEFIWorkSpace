#include "MyHelloWorldSMBios/get_info.h"
#include "Gmssl/sm2.h"
#include "SM3/sm3.h"
#include "SM4/hcj_sm4.h"
#include "UserAuth/UserAuth.h"
// #include "NetWork/hcj_network.h"
#include "EchoTcp4/EchoTcp4.h"
#include <string.h>
#include <gmssl/base64.h>

#define false_messageType -1
#define USER_AUTH_FAIL -2
#define WRONG_INFO -3


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


int base64_encode(unsigned char *input, int len, unsigned char *output){
    BASE64_CTX ctx;
    unsigned char temp[500003];
    memset(temp, 0, sizeof(temp));
    base64_encode_init(&ctx);
    uint8_t *p = temp;
    int outlen;
    base64_encode_update(&ctx, input, len, p, &outlen);
    p += outlen;
    base64_encode_finish(&ctx, p, &outlen);
    p += outlen;
    // printf("temp:\n%s\n", temp);
    outlen = (int)(p-temp);
    int i, j;
    for(i = 0, j = 0; i < outlen; ++i){
        if(temp[i] != '\n'){
            output[j++] = temp[i];
        }
        if(temp[i] == '\0'){
            output[j] = '\0';
            break;
        }
    }
    output[j] = '\0';
    return 0;
}

//对json中的message字段进行解密并且将他放到output中
int get_json_message(unsigned char *session_key, cJSON *json, char *message, char *sub_message, char *output){
    memset(output, 0, sizeof(output));
    if(json == NULL){
        return 0;
    }
    cJSON *json_message = cJSON_GetObjectItem(json, message);
    if(json_message == NULL){
        return 0;
    }
    char *str_message = cJSON_Print(json_message);
    //接下来要对双引号进行处理，将双引号删除
    char full_str_message[1024];
    strncpy(full_str_message, str_message+1, strlen(str_message)-2);
    // printf("full_str_message:\n%s\n", full_str_message);
    //base64解码
    unsigned char byte_full_str_message[1024];
    BASE64_CTX ctx;
    base64_decode_init(&ctx);
    int len;
    base64_decode_update(&ctx, (uint8_t *)full_str_message, strlen(full_str_message), byte_full_str_message, &len);
    //sm4解密
    // printf("len:\n%d\n, byte_full_str_message:\n", len);
    // for(int i = 0; i < len; ++i){
    //     printf("(byte)0x%02x, ", byte_full_str_message[i]);
    // }
    // printf("\n");
    unsigned char de_str_message[1024];
    hcj_sm4_decrypt(session_key, byte_full_str_message, de_str_message, len);
    cJSON *de_message = cJSON_ParseWithLength(de_str_message, len);
    printf("de_message:\n%s\n", cJSON_Print(de_message));
    cJSON *json_sub_message = cJSON_GetObjectItem(de_message, sub_message);
    char *str_sub_message = cJSON_Print(json_sub_message);
    if(str_sub_message[0] == '"'){
        strncpy(output, str_sub_message+1, strlen(str_sub_message)-2);
    }else{
        strcpy(output, str_sub_message);
    }
    return 0;

}


void sm4(char *input, int buf_length, char *output, unsigned char *session_key){
    //分组加密进行补全
    int temp = buf_length%16;
    for(int i = temp; i < 16; ++i){
        strcat(input, " ");
    }
    buf_length = strlen(input);
    printf("\n buquan finished\n\n\n\n\n");
   
    //SM4加密
    unsigned char *enc_send_msg = (unsigned char*)malloc(buf_length+3);
    memset(enc_send_msg, 0, sizeof(enc_send_msg));
    hcj_sm4_encrypt(session_key, (unsigned char*)input, enc_send_msg, buf_length);
    //BASE64编码
    base64_encode(enc_send_msg, buf_length, (unsigned char*)output);
    free(enc_send_msg);
}





int User_Auth(OUT char *username, OUT char *password, OUT char *timestamp, OUT unsigned char *session_key, char *token, 
              IN int Argc, IN char **Argv){
    BASE64_CTX ctx;
    
    UserInfo(username, password, timestamp);

    cJSON *User_info_json = cJSON_CreateObject();

    cJSON_AddItemReferenceToObject(User_info_json, "username", cJSON_CreateString(username));
    cJSON_AddItemReferenceToObject(User_info_json, "password", cJSON_CreateString(password));
    cJSON_AddItemReferenceToObject(User_info_json, "timestamp", cJSON_CreateString(timestamp));

    
    char *User_info_str = cJSON_Print(User_info_json);
    int source_len = strlen(User_info_str);
    printf("source len:%d\n", source_len);
    source_len = source_len%16;
    if(source_len != 0){
        for(int i = source_len; i < 16; ++i){
            strcat(User_info_str, " ");
        }
    }
    int len = strlen(User_info_str);
    printf("user_info_str len:%d\n", len);
    printf("User_info_json:\n%s\n", User_info_str);
    unsigned char sm4_User_info_str[1024];
    //必须要强转，不然会出问题
    hcj_sm4_encrypt(session_key, (unsigned char*)User_info_str, sm4_User_info_str, len);
    printf("sm4 encrypted User_info:\n");
    for(int i = 0; i < len; ++i){
        printf("(byte)0x%02x,", sm4_User_info_str[i]);
    }
    printf("\n");
    uint8_t base64_sm4_User_info_str[512];
    base64_encode(sm4_User_info_str, len, base64_sm4_User_info_str);
    
    cJSON *Connect_msg_json = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(Connect_msg_json, "enc_LoginInfo", cJSON_CreateString(base64_sm4_User_info_str));


    unsigned char sm2_session_key[SM2_MAX_CIPHERTEXT_SIZE];
    uint8_t base64_sm2_session_key[512];
    len = SM2_encrypt(session_key, 16, sm2_session_key, len);
    printf("sm2 encrypted len: %d\n", len);
    for(int i = 0; i < len; ++i){
        printf("(byte)0x%02x, ", sm2_session_key[i]);
    }
    printf("\n");
    base64_encode(sm2_session_key, len, (unsigned char*)base64_sm2_session_key);
    cJSON_AddItemReferenceToObject(Connect_msg_json, "sessionKey", cJSON_CreateString(base64_sm2_session_key));
    cJSON_AddItemReferenceToObject(Connect_msg_json, "messageType", cJSON_CreateString("Login"));
    char *outbuf = cJSON_Print(Connect_msg_json);
    
    printf("%s\n", outbuf);
    int outlen = strlen(outbuf);
    printf("len: %d\n", outlen);
    

    CHAR8 *RecvBuffer = (CHAR8*) malloc(1024);
    UINTN recvLen = 0;
    SendMessage(Argc, Argv, outbuf, outlen, RecvBuffer, &recvLen);
    RecvBuffer[recvLen] = '\0';
    printf("receive message:%s\n", RecvBuffer);
    cJSON *recv = cJSON_ParseWithLength(RecvBuffer, recvLen);

    //cJSON *recv = cJSON_Parse("{\"message\":\"tDzHh3Fu7w/rLSC5WbjBL8jffj+qb770teSsxsB0oKOAOE+aL8ldi6K6O0VP7JVU\",\"messageType\":\"Login\",\"username\":\"123\"}");
    char status[1024];
    get_json_message(session_key, recv, "message", "status", status);
    get_json_message(session_key, recv, "message", "token", token);
    printf("status: %s\n", status);
    if(strcmp(status, "true") == 0){
        return 0;
    }else{
        return USER_AUTH_FAIL;
    }
    

}





int hardwarecheck(unsigned char *session_key, char *token, IN int Argc, IN char **Argv){

    //有关时间的变量
    time_t tmpcal_ptr;
    struct tm *tmp_ptr = NULL;
    char timestamp[1024];

    

    cJSON* check_msg = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(check_msg, "token", cJSON_CreateString(token));

    //get infomation
    cJSON* json = cJSON_CreateObject();
    cJSON *Cold_Hardware = cJSON_CreateArray();
    get_info(json, Cold_Hardware);
    


    
    char *buf = cJSON_Print(cJSON_GetObjectItem(json, "NHSwapHardware"));
    int buf_length = strlen(buf);
    printf("%s\n", buf);


    //sm3 paration
    unsigned char Hash[32] = {0};
    SM3(buf, buf_length, Hash);
    char base64_hash[1024];
    memset(base64_hash, 0, sizeof(base64_hash));
    base64_encode(Hash, 32, base64_hash);
    cJSON_AddItemToObject(check_msg, "hash", cJSON_CreateString(base64_hash));


    //是否可以用一个结构体传递 TODO
    //补全位数
    buf = cJSON_Print(json);
    buf_length = strlen(buf);

    char *base64_enc_send_msg = (char *)malloc(buf_length+buf_length);
    memset(base64_enc_send_msg, 0, sizeof(base64_enc_send_msg));
    sm4(buf, buf_length, base64_enc_send_msg, session_key);
    /**
    int temp = buf_length%16;
    for(int i = temp; i < 16; ++i){
        strcat(buf, " ");
    }
    buf_length = strlen(buf);

    printf("\n buquan finished\n\n\n\n\n");
    

    //sm4
    

    unsigned char *enc_send_msg = (unsigned char*) malloc(buf_length+3);
    memset(enc_send_msg, 0, sizeof(enc_send_msg));
    printf("encrypted:\n");
    hcj_sm4_encrypt(session_key, (unsigned char*)buf, enc_send_msg, buf_length);
    // for(int i = 0; i < buf_length; ++i){
    //     printf("%02x ", output[i]);
    // }
    // printf("\n");
    printf("encrypt finished\n\n\n\n\n");

   
    //要强转
    base64_encode(enc_send_msg, buf_length, (unsigned char*)base64_enc_send_msg);
    **/

    cJSON_AddItemReferenceToObject(check_msg, "cipher", cJSON_CreateString(base64_enc_send_msg));
    
    time(&tmpcal_ptr);
    tmp_ptr = gmtime(&tmpcal_ptr);
    sprintf(timestamp, "%d.%d.%d  %d:%d:%d", (1900+tmp_ptr->tm_year), (1+tmp_ptr->tm_mon), tmp_ptr->tm_mday,
    (8+tmp_ptr->tm_hour), tmp_ptr->tm_min, tmp_ptr->tm_sec);
    cJSON_AddItemReferenceToObject(check_msg, "timeStamp", cJSON_CreateString(timestamp));

    cJSON_AddItemReferenceToObject(check_msg, "messageType", cJSON_CreateString("hardwareregister"));
    printf("\ncheck_msg:\n%s\n", cJSON_Print(check_msg));



    char *outbuf = cJSON_Print(check_msg);
    int outlen = strlen(outbuf);
    //发送消息
    CHAR8 *RecvBuffer = (CHAR8*) malloc(1024);
    UINTN recvLen = 0;
    SendMessage(Argc, Argv, outbuf, outlen, RecvBuffer, &recvLen);
    RecvBuffer[recvLen] = '\0';
    printf("Message from server: %s\n", RecvBuffer);
    cJSON *recv = cJSON_ParseWithLength(RecvBuffer, recvLen);



    // cJSON *recv = cJSON_Parse("123");
    char result[1024];
    get_json_message(session_key, recv, "message", "code", result);
    printf("reseult: %s\n", result);

    int return_val = 0;
    while(1){
        if(strcmp(result, "200")){
            return_val = 0;
            break;
        }else if(strcmp(result, "300")){  
            printf("please input verify_code:\n");
            char code[1024];
            gets(code);
            cJSON *Register_Msg = cJSON_CreateObject();
            cJSON_AddItemReferenceToObject(Register_Msg, "token", token);
            //todo发哪些信息
            cJSON_AddItemReferenceToObject(Register_Msg, "hardwarelist", cJSON_CreateString(base64_enc_send_msg));
            char enc_Code[1024];
            sm4(code, strlen(code), enc_Code, session_key);
            cJSON_AddItemReferenceToObject(Register_Msg, "registerCode", cJSON_CreateString(enc_Code));
            {
                cJSON_AddItemReferenceToObject(check_msg, "cipher", cJSON_CreateString(base64_enc_send_msg));
                time(&tmpcal_ptr);
                tmp_ptr = gmtime(&tmpcal_ptr);
                sprintf(timestamp, "%d.%d.%d  %d:%d:%d", (1900+tmp_ptr->tm_year), (1+tmp_ptr->tm_mon), tmp_ptr->tm_mday,
                (8+tmp_ptr->tm_hour), tmp_ptr->tm_min, tmp_ptr->tm_sec);
                cJSON_AddItemReferenceToObject(check_msg, "timeStamp", cJSON_CreateString(timestamp));
            }
            cJSON_AddItemReferenceToObject(Register_Msg, "timeStamp", cJSON_CreateString(timestamp));
            cJSON_AddItemReferenceToObject(Register_Msg, "hash", cJSON_CreateString(base64_hash));

            char *outbuf = cJSON_Print(Register_Msg);
            int outlen = strlen(outbuf);
            //发送消息
            CHAR8 *RecvBuffer = (CHAR8*) malloc(1024);
            UINTN recvLen = 0;
            SendMessage(Argc, Argv, outbuf, outlen, RecvBuffer, &recvLen);
            RecvBuffer[recvLen] = '\0';
            printf("Message from server: %s\n", RecvBuffer);
            cJSON *recv = cJSON_ParseWithLength(RecvBuffer, recvLen);
            
            get_json_message(session_key, recv, "message", "code", result);
            printf("reseult: %s\n", result);
            free(RecvBuffer);
            RecvBuffer = NULL;

        }else {
            return_val =  1;
            break;
        }
    }

    free(RecvBuffer);
    RecvBuffer = NULL;
    free(base64_enc_send_msg);
    base64_enc_send_msg = NULL;

    cJSON_Delete(json);
    cJSON_Delete(check_msg);
    // if(strcmp(result, "ok")){
    //     return 0;
    // }else{
    //     return WRONG_INFO;
    // }
    return return_val;
}


int main(int Argc, char **Argv){
    //用户认证部分
    char username[1024];
    char password[1024];
    char timestamp[1024];
    char token[1024];
    unsigned char session_key[16] = "Q5ud106nhKQwYtKQ";

    generateString(session_key, 16);
    printf("session_key:\n");
    for(int i = 0; i < 16; ++i){
        printf("%c ", session_key[i]);
    }
    printf("\n");

    int status = 0;
    
    status = User_Auth(username, password, timestamp, session_key, token, Argc, Argv);


    // while((status = User_Auth(username, password, timestamp, session_key, token, Argc, Argv)) != 0){
    //     if(status == USER_AUTH_FAIL)
    //         printf("your username or password is wrong!\n");
    //     else{
    //         printf("something wrong happen in User_Auth!\n");
    //     }
    // }
    printf("%s\n", token);
    printf("User Auth finished\n\n\n\n");

    hardwarecheck(session_key, token, Argc, Argv);
    
    
    return 0;
}