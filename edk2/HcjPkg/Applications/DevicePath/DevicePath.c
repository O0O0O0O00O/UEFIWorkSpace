#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePath.h>

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
){
    EFI_STATUS status;


    EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL * DPFTP;
    status = gST->BootServices->LocateProtocol(
        &gEfiDevicePathFromTextProtocolGuid,
        NULL,
        (void **)&DPFTP
    );

    EFI_DEVICE_PATH_PROTOCOL *dev_path = NULL;
    dev_path = DPFTP->ConvertTextToDevicePath(L"\\EFI\\Boot\\HelloWorld.efi");

    gST->ConOut->ClearScreen(gST->ConOut);
    gST->ConOut->OutputString(gST->ConOut, L"dev_path: ");
    


    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
    gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (void **)&DPTTP);
    gST->ConOut->OutputString(gST->ConOut, DPTTP->ConvertDevicePathToText(dev_path, FALSE, FALSE));

    gST->ConOut->OutputString(gST->ConOut, L"\r\n");

    while(TRUE);
    return status;


}