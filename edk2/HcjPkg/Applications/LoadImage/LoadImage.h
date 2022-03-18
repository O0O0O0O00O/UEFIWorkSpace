#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathUtilities.h>

void pause();

void assert(
    EFI_STATUS Status, 
    CHAR16 *String
);


EFI_STATUS
EFIAPI
LoadedImage(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
);