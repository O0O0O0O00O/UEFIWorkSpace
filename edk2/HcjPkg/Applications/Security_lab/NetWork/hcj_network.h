#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
//#include <Protocol/SimpleTextInEx.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>

#include <sys/EfiSysCall.h>
#include <sys/endian.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>

// #include "Common.h"
// #include "Keyboard.h"
// #include "NetWork.h"


EFI_STATUS SendMessage(IN char* message, IN int mes_length);



