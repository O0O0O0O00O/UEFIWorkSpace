#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SerialIo.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Protocol/PciIo.h>						//获取PciIO protocol所需
#include <Protocol/PciRootBridgeIo.h>	//获取PciRootBridgeIO protocol所需
#include <IndustryStandard/Pci.h>  //pci访问所需的头文件，包含pci22.h,pci23.h...
#include <Protocol/Rng.h>  //Random Number Generator Protocol 2019-08-31 11:32:03 robin

#include <Protocol/SimpleFileSystem.h> //文件系统访问
#include <IndustryStandard/Bmp.h> //for bmp


EFI_STATUS ListProtocolMsg(IN EFI_GUID *ProtocolGuid, OUT VOID **Interface);


EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
){
    EFI_RNG_PROTOCOL *gRNGOut;
    EFI_SERIAL_IO_PROTOCOL *gSerialIO;

    gST->ConOut->SetAttribute(gST->ConOut, EFI_BACKGROUND_BLACK|EFI_RED);
    Print((const CHAR16*)L"Action: SerialIoProtocol\n");
    ListProtocolMsg(&gEfiSerialIoProtocolGuid, (VOID**)&gSerialIO);
    gST->ConOut->SetAttribute(gST->ConOut, EFI_BACKGROUND_BLACK|EFI_CYAN);
    Print((const CHAR16*)L"Action:RngProtocol]\n");
    ListProtocolMsg(&gEfiRngProtocolGuid, (VOID**)&gRNGOut);
    gST->ConOut->SetAttribute(gST->ConOut, EFI_BACKGROUND_BLACK|EFI_LIGHTGRAY);
    return EFI_SUCCESS;
}

EFI_STATUS ListProtocolMsg(IN EFI_GUID *ProtocolGuid, OUT VOID **Interface){
    EFI_STATUS Status;
    EFI_HANDLE *myHandleBuff = NULL;

    UINTN HandleCount = 0;
    UINTN i;
    EFI_OPEN_PROTOCOL_INFORMATION_ENTRY *InfEntryArray;
    UINTN InfEntryCount;

    Print((const CHAR16*)L" GUID:{0x%08x, 0x%04x, 0x%04x, {", ProtocolGuid->Data1, ProtocolGuid->Data2, ProtocolGuid->Data3);
    for(i = 0; i < 8; ++i){
        Print((const CHAR16*)L" 0x%02x", ProtocolGuid->Data4[i]);
    }
    Print((const CHAR16*)L"}}\n");

    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        ProtocolGuid,
        NULL,
        &HandleCount,
        &myHandleBuff
    );
    if(EFI_ERROR (Status)){
        Print((const CHAR16*)L"Not Found Handle!\n");
        return Status;
    }else{
        Print((const CHAR16*)L"Found Handle Count: %d\n",HandleCount);
    }
    for(i = 0; i < HandleCount; ++i){
        Status = gBS->HandleProtocol(
            myHandleBuff[i],
            ProtocolGuid,
            Interface
        );
        if(EFI_ERROR (Status)) continue;
        else
            break;
    }
    Status = gBS->OpenProtocolInformation(myHandleBuff[i],ProtocolGuid, &InfEntryArray, &InfEntryCount);
    if(EFI_ERROR (Status))
        Print((const CHAR16*)L"Not Get the Protocol's information!\n");
    else{
        Print((const CHAR16*)L"EntryCount=%d \n",InfEntryCount);
        gBS->CloseProtocol(myHandleBuff[i], ProtocolGuid, InfEntryArray->AgentHandle, InfEntryArray->ControllerHandle);
        if(InfEntryArray)
            gBS->FreePool(InfEntryArray);
    }

    if(myHandleBuff)
        gBS->FreePool(myHandleBuff);
    return Status;

}