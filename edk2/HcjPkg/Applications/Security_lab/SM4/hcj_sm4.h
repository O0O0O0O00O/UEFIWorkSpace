#include <string.h>
#include <stdio.h>
#include "sm4.h"

/***
  Demonstrates basic workings of the SM3 encrypt function

  @param[in]  key      the encrypt key
  @param[in]  input    the buffer need to be encrypt
  @param[out] output   the buffer after encrypt
  @param[out] len      the buffer size of the output

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int hcj_sm4_encrypt(unsigned char *key, unsigned char *input, unsigned char *output, int len);
/***
  Demonstrates basic workings of the SM3 decrypt function

  @param[in]  key      the decrypt key
  @param[in]  input    the buffer need to be decrypt
  @param[out] output   the buffer after decrypt
  @param[out] len      the buffer size of the output

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int hcj_sm4_decrypt(unsigned char *key, unsigned char *input, unsigned char *output, int len);
