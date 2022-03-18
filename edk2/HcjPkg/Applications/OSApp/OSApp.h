#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathUtilities.h>
#include <string.h>


void efi_init(EFI_SYSTEM_TABLE *SystemTable);

void Shell(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable);