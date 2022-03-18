#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h> //global gST gBS gImageHandle

#include <Protocol/LoadedImage.h>      //EFI_LOADED_IMAGE_PROTOCOL
#include <Protocol/DevicePath.h>       //EFI_DEVICE_PATH_PROTOCOL

#include <Library/DevicePathLib.h>     //link
#include <Protocol/SimpleFileSystem.h>
// 
// Try to find a file by browsing each device
// 
EFI_STATUS LocateFile( IN CHAR16* ImagePath, OUT EFI_DEVICE_PATH** DevicePath )
{
        EFI_FILE_IO_INTERFACE *ioDevice;
        EFI_FILE_HANDLE handleRoots, bootFile;
        EFI_HANDLE* handleArray;
        UINTN nbHandles, i;
        EFI_STATUS efistatus;
        
        *DevicePath = (EFI_DEVICE_PATH *)NULL;
        //
        //Get all the Handles which have Simple File System Protocol 
        //
        efistatus = gBS->LocateHandleBuffer( ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &nbHandles, &handleArray );
        if (EFI_ERROR( efistatus ))
                return efistatus;
        
        Print( L"\r\nNumber of UEFI Filesystem Devices: %d\r\n", nbHandles );
        
        for (i = 0; i < nbHandles; i++)
        {
                efistatus = gBS->HandleProtocol( handleArray[i], &gEfiSimpleFileSystemProtocolGuid, (void**)&ioDevice );
                if (efistatus != EFI_SUCCESS)
                        continue;
        
                efistatus = ioDevice->OpenVolume( ioDevice, &handleRoots );
                if (EFI_ERROR( efistatus ))
                        continue;
                //
                //Try to open the specific path on the device
                //
                efistatus = handleRoots->Open( handleRoots, &bootFile, ImagePath, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY );
                if (!EFI_ERROR( efistatus ))
                {
                        handleRoots->Close( bootFile );
                        *DevicePath = FileDevicePath( handleArray[i], ImagePath );
                        Print( L"\r\nFound file at \'%s\'\r\n", ConvertDevicePathToText( *DevicePath, TRUE, TRUE ) );
                        break;
                }
        }
        
        return efistatus;
}
        
EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
        // Windows Boot Manager x64 image path
        static CHAR16    *gRuntimeDriverImagePath = L"\\EFI\\Boot\\Shell.efi";
        EFI_DEVICE_PATH* RuntimeDriverDevicePath = NULL;
        EFI_HANDLE       RuntimeDriverHandle = NULL;        
        EFI_STATUS       efiStatus;
        
        //
        // Clear screen and make pretty
        //
        // gST->ConOut->ClearScreen( gST->ConOut );
        // gST->ConOut->SetAttribute( gST->ConOut, EFI_GREEN | EFI_BACKGROUND_LIGHTGRAY );
        
        //
        // Locate the runtime driver
        //
        efiStatus = LocateFile( gRuntimeDriverImagePath, &RuntimeDriverDevicePath );
        if (EFI_ERROR( efiStatus )) {
                Print(L"Can't find %s\n",gRuntimeDriverImagePath);
                goto Exit;
        }

        Print(L"Found %s\n",gRuntimeDriverImagePath);
        Print(L"Boot to it in 5 seconds \n");
        gBS->Stall(5000000UL);
        
        //
        // Load Runtime Driver into memory
        //
        efiStatus = gBS->LoadImage(TRUE, ImageHandle, RuntimeDriverDevicePath,NULL,0,&RuntimeDriverHandle);
        if (EFI_ERROR( efiStatus )) {
                Print(L"Loading %s failed\n",gRuntimeDriverImagePath);
                goto Exit;
        }
        
        //
        // Transfer executon to the Runtime Driver
        //
        efiStatus = gBS->StartImage( RuntimeDriverHandle, (UINTN *)NULL,(CHAR16**)NULL);
        if (EFI_ERROR( efiStatus )) {
                Print(L"Starting %s failed\n",gRuntimeDriverImagePath);
                goto Exit;
        }     
Exit:
        gBS->Stall(15000000UL);
  return EFI_SUCCESS;
}
