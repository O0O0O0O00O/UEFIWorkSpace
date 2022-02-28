#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathUtilities.h>
#include <string.h>


#define MAX_COMMAND_LINE 10

void hcj_pause(){
    for(int i = 0; i < 5; ++i)
        gBS->Stall(1000000);
}

void hcj_assert(EFI_STATUS Status, CHAR16 *String){
    gST->ConOut->OutputString(gST->ConOut, String);
    if(EFI_ERROR (Status)){
        gST->ConOut->OutputString(gST->ConOut, L" failed\r\n");
    }else{
        gST->ConOut->OutputString(gST->ConOut, L" successed\r\n");
    }
}

void hcj_putc(unsigned short c){
    unsigned short str[2];
    str[0] = c;
    str[1] = '\0';
    gST->ConOut->OutputString(gST->ConOut, str);
}

void hcj_puts(CHAR16 *String){
    gST->ConOut->OutputString(gST->ConOut, String);
}

unsigned short hcj_getc(void){
    EFI_INPUT_KEY key;
    unsigned long long waitidx;
    gST->BootServices->WaitForEvent(1, &(gST->ConIn->WaitForKey), &waitidx);
    while(gST->ConIn->ReadKeyStroke(gST->ConIn, &key))
        ;
    return key.UnicodeChar;
}

unsigned int hcj_gets(char *buf, unsigned int buf_size){
    unsigned int i;
    for(i = 0; i < buf_size-1; ++i){
        buf[i] = hcj_getc();
        if(buf[i] == 0x8){
            if(i == 0){
                i -= 1;
                continue;
            }else{
                hcj_putc(buf[i]);
                i -= 2;
            }
        }else if(buf[i] == L'\r'){
            hcj_putc(L'\r');
            hcj_putc(L'\n');
            break;
        }else{
            hcj_putc(buf[i]);
        }
    }
    buf[i] = L'\0';
    return i;

}

void efi_init(EFI_SYSTEM_TABLE *SystemTable){
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
}

void Shell(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable){
    char buf[MAX_COMMAND_LINE];
    while(TRUE){
        hcj_puts(L"hcjOS> ");
        if(hcj_gets(buf, MAX_COMMAND_LINE) <= 0){
            continue;
        }
        if(!strcmp(buf, "Hello")){
            hcj_puts(L"Hello!!!!\r\n");
        }else{
            hcj_puts(L"Command Wrong!\r\n");
        }
    }
}


EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
){
    EFI_STATUS status = EFI_SUCCESS;
    efi_init(SystemTable);
    Shell(ImageHandle, SystemTable);

    return status;


}