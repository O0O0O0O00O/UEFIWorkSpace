#ifndef SM3_H
#define SM3_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void out_hex();
void intToString(unsigned char *out_hash);
/***
  Demonstrates basic workings of the SM3 hash function

  @param[in]  msg      the buffer need to be hash
  @param[in]  msglen   the buffer size of the msg
  @param[out] out_hash   the buffer after hash

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int SM3(unsigned char *msg, unsigned int msglen, unsigned char *out_hash);
void StringToChar(unsigned char *in_hash, unsigned char *out_hash);

#endif
