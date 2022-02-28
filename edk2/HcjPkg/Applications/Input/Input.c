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
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *STIP;
    status = gBS->LocateProtocol(
        &gEfiSimpleTextInProtocolGuid,
        NULL,
        (void **)&STIP
    );
    EFI_INPUT_KEY key;
    CHAR16 str[3];
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    while(1){
        if(!SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key)){
            if(key.UnicodeChar != L'\r'){
                str[0] = key.UnicodeChar;
                str[1] = L'\0';
            }else{
                str[0] = L'\r';
                str[1] = L'\n';
                str[2] = L'\0';
            }
            SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
        }
    }
    

    return status;


}