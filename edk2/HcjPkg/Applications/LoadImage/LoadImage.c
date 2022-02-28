#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathUtilities.h>


void pause(){
    for(int i = 0; i < 5; ++i)
        gBS->Stall(1000000);
}

void assert(EFI_STATUS Status, CHAR16 *String){
    gST->ConOut->OutputString(gST->ConOut, String);
    if(EFI_ERROR (Status)){
        gST->ConOut->OutputString(gST->ConOut, L" failed\r\n");
    }else{
        gST->ConOut->OutputString(gST->ConOut, L" successed\r\n");
    }
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
){
    EFI_STATUS status;
    CHAR16 *file_path = L"\\EFI\\Boot\\Shell.efi";

    EFI_LOADED_IMAGE_PROTOCOL *lip;
    status = gST->BootServices->OpenProtocol(
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (void **)&lip,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    assert(status, L"OpenProtocol(lip)");
    


    EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *DPFTP;
    status = gST->BootServices->LocateProtocol(
        &gEfiDevicePathFromTextProtocolGuid,
        NULL,
        (void **)&DPFTP
    );
    assert(status, L"LocateProtocol(DPFTP)");

    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
    status = gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (void **)&DPTTP);
    assert(status, L"LocateProtocol(DPTTP)");


    EFI_DEVICE_PATH_PROTOCOL *dev_path;
    status = gST->BootServices->OpenProtocol(
        lip->DeviceHandle,
        &gEfiDevicePathProtocolGuid,
        (void **)&dev_path,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    assert(status, L"OpenProtocol(Dpp)");

    EFI_DEVICE_PATH_PROTOCOL *dev_node;
    dev_node = DPFTP->ConvertTextToDeviceNode(file_path);
    
    EFI_DEVICE_PATH_UTILITIES_PROTOCOL *DPUP;
    status = gBS->LocateProtocol(
        &gEfiDevicePathUtilitiesProtocolGuid,
        NULL,
        (void **)&DPUP
    );
    assert(status, L"LocateProtocol(DPUP)");
    
    EFI_DEVICE_PATH_PROTOCOL *dev_path_merged;
    dev_path_merged = DPUP->AppendDeviceNode(dev_path, dev_node);

    
    gST->ConOut->OutputString(gST->ConOut, L"dev_path_merged: ");
    gST->ConOut->OutputString(gST->ConOut, DPTTP->ConvertDevicePathToText(dev_path_merged, FALSE, FALSE));
    gST->ConOut->OutputString(gST->ConOut, L"\r\n");

    void *image;
    status = gBS->LoadImage(
        FALSE,
        ImageHandle,
        dev_path_merged,
        NULL,
        0,
        &image
    );
    assert(status, L"LoadImage");

    pause();
    status = gBS->StartImage(image, NULL, NULL);
    assert(status, L"StartImage");

    while(TRUE);
    

    return status;


}