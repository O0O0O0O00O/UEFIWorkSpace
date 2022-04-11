#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    char *pub_key_info = "MFkwEwYHKoZIzj0CAQYIKoEcz1UBgi0DQgAEiyi4Nz8Myp0ItdxV3TbpKIVklKT7HsuQxHSU3C+K/PjoDgRVYJFjt3MYu9I0snrUt1j4lzTuTXpIVt+wyjJsrA==";
    char *session_key = "ASNFZ4mrze/+3LqYdlQyEA==";
    printf("public key: ");
    for(int i = 0; i < strlen(pub_key_info); ++i){
        printf("%c", pub_key_info[i]);
    }
    printf("\n");
    printf("username: admin\n");
    printf("passwd: ******\n");
    printf("user authentication passed\n");
    printf("exchange key ....\n");
    printf("exchange key finished\n");
    printf("session key: ");
    for(int i = 0; i < strlen(session_key); ++i)
        printf("%c", session_key[i]);
    printf("\n");
    printf("get infomation.\n");
    printf("send crypted message\n");




    return 0;

}