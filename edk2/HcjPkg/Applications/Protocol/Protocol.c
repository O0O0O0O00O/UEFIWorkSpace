#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
){
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN NumHandles = 0;
    EFI_HANDLE *Buffer = NULL;

    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        &NumHandles,
        &Buffer
    );

    Print(L"Status = %d \n", Status);

    if(EFI_ERROR (Status)){
        Print(L"Fail to LocateHandleBuffer.\n");
        return Status;
    }
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

    Status = gBS->HandleProtocol(
        Buffer[0],
        &gEfiGraphicsOutputProtocolGuid,
        (void **)&Gop
    );
    Print(L"Status = %d.\n", Status);

    if(EFI_ERROR (Status)){
        Print(L"Fail to OpenProtocol\n");
        return Status;
    }

    UINTN SizeOfInfo;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION * Info;

    for(UINTN i = 0; i < Gop->Mode->MaxMode; i++){
        Status = Gop->QueryMode(
            Gop,
            i,
            &SizeOfInfo,
            &Info
        );
        if(EFI_ERROR (Status)){
            Print(L"Fail to QueryMode.\n");
            return Status;
        }
        Print(L"Mode %d, H = %d, V = %d.\n",
        i,
        Info->HorizontalResolution,
        Info->VerticalResolution);
    }
    return Status;




}
