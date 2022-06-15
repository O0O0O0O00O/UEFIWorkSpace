/** @file
    A simple, basic, application showing how the Hello application could be
    built using the "Standard C Libraries" from StdLib.

**/

//#include  <Uefi.h>
//#include  <Library/UefiLib.h>
//#include  <Library/ShellCEntryLib.h>
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
//#include <Protocol/SimpleTextInEx.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <wchar.h>
#include "Common.h"
#include "Keyboard.h"
#include "Network.h"

/***
  Demonstrates basic workings of the main() function by displaying a
  welcoming message.

  Note that the UEFI command line is composed of 16-bit UCS2 wide characters.
  The easiest way to access the command line parameters is to cast Argv as:
      wchar_t **wArgv = (wchar_t **)Argv;

  @param[in]  Argc    Number of argument tokens pointed to by Argv.
  @param[in]  Argv    Array of Argc pointers to command line tokens.
  @param[in]  msgStr  the message need to be send
  @param[in]  msg_length    the length of msgStr
  @param[out] RecvBuffer    the message received
  @param[out] recvLen   the length if RecvBuffer

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/


EFI_STATUS SendMessage(IN int Argc,
  IN char **Argv, IN unsigned char *msgStr, IN UINTN msg_length, OUT CHAR8 *RecvBuffer, OUT UINTN *recvLen)
{
  EFI_STATUS Status = 0;
  UINTN myfd;
  // CHAR8 *RecvBuffer = (CHAR8*) malloc(1024);
  // UINTN recvLen = 0;
  UINT32 ServerIPAddr[4];
  UINT16 ServerPort;
  // char msgStr[1024];
  //1 get the server ip and port
  if(Argc != 3)
  {
    printf("UEFI TCP Client by Security lab. Usage: %s <ServerIP> <port>\n", Argv[0]);
    Status = EFI_ABORTED;
    return Status;
  }
  else
  {
    sscanf(Argv[1], "%d.%d.%d.%d", &ServerIPAddr[0], &ServerIPAddr[1], &ServerIPAddr[2], &ServerIPAddr[3]);
    sscanf(Argv[2], "%d", &ServerPort);
  }
  //2 connect server
  myfd = CreateTCP4Socket();
  Status = ConnectTCP4Socket(myfd, MYIPV4(ServerIPAddr[0],ServerIPAddr[1],ServerIPAddr[2],ServerIPAddr[3]), ServerPort);
  if(EFI_ERROR(Status)){
    printf("connect failed\n");
    return Status;
  }

  //3 echo test

  //4 send message to server and get message from server
  // memset(msgStr, 0, 1024);

  // printf("Please input message:");
  // gets(msgStr); //get string 
    // if (!strcmp(msgStr, "q") || !strcmp(msgStr, "Q"))
    //   break;
  // for(int i = 0; i < msg_length; ++i){
  //   unsigned char temp = msgStr[i];
  //   printf("%02x ", msgStr[i]);
  //   unsigned char *p_temp = &temp;
  //   Status = SendTCP4Socket(myfd, p_temp, 1);
  // }
  // printf("\n");
  Status = SendTCP4Socket(myfd, msgStr, msg_length);
  if(EFI_ERROR(Status)){
    printf("send failed\n");
    return Status;
  }
  Status = RecvTCP4Socket(myfd, RecvBuffer, 1024, recvLen);
  if(EFI_ERROR(Status)){
    printf("recv failed\n");
    return Status;
  }
  // RecvBuffer[recvLen] = '\0';
  // printf("Message from server: %s\n", RecvBuffer);
  

  Status = CloseTCP4Socket(myfd);
  if(EFI_ERROR(Status))
    Print(L"close socket, %r\n", Status);

  // free(RecvBuffer);

  return Status;
}