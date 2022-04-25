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


/***
  Demonstrates basic workings of the SM2 encrypt function

  @param[in]  input    the buffer need to be encrypted
  @param[in]  inlen    the buffer size of the input
  @param[out] outbuf   the buffer after encrypt
  @param[out] outlen   the buffer size of the output

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int SM2_encrypt(uint8_t *input, size_t inlen, uint8_t *outbuf, size_t outlen);