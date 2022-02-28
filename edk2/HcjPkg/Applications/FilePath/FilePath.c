#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePath.h>

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
){
    EFI_STATUS status;
    EFI_LOADED_IMAGE_PROTOCOL *lip;

    status = gST->BootServices->OpenProtocol(
        ImageHandle, 
        &gEfiLoadedImageProtocolGuid,
        (void **)&lip,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );

    gST->ConOut->ClearScreen(gST->ConOut);
    gST->ConOut->OutputString(gST->ConOut, L"lip->FilePath: ");
    
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
    gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (void **)&DPTTP);
    
    gST->ConOut->OutputString(gST->ConOut, DPTTP->ConvertDevicePathToText(lip->FilePath, FALSE, FALSE));
    gST->ConOut->OutputString(gST->ConOut, L"\r\n");

    
    while(TRUE);
    return status;


}