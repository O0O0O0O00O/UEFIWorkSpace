#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathUtilities.h>
#include <Protocol/Tcp4.h>
#include <Protocol/ServiceBinding.h>


#define MAX_COMMAND_LINE 10

void hcj_pause();

void hcj_assert(EFI_STATUS Status, CHAR16 *String);

void hcj_putc(unsigned short c);

void hcj_puts(CHAR16 *String);

unsigned short hcj_getc(void);

unsigned int hcj_gets(char *buf, unsigned int buf_size);


