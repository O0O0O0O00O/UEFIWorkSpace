#include <time.h>
#include <stdio.h>


//用户获取用户的账号密码和时间戳
/***
  Demonstrates basic workings of get the user auth

  @param[in]  username     the username
  @param[in]  password     the password
  @param[in] timestamp    the timestamp

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int UserInfo(IN OUT char *username,
    IN OUT char *password,
    IN OUT char *timestamp);