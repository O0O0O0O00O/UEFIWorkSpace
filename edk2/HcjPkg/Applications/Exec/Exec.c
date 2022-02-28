#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>

#include <Guid/FileInfo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>

#include  <stdio.h>
#include  <stdlib.h>
#include  <wchar.h>

//#include <Protocol/EfiShell.h>
#include <Protocol/Shell.h>
#include <Library/ShellLib.h>

extern EFI_BOOT_SERVICES           	 *gBS;
extern EFI_SYSTEM_TABLE				 *gST;
extern EFI_RUNTIME_SERVICES 		 *gRT;

extern EFI_SHELL_PROTOCOL            *gEfiShellProtocol;
extern EFI_SHELL_ENVIRONMENT2 		 *mEfiShellEnvironment2;

extern EFI_HANDLE					 gImageHandle;
/**
  GET  DEVICEPATH
**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
ShellGetDevicePath (
  IN CHAR16                     * CONST DeviceName OPTIONAL
  )
{
  //
  // Check for UEFI Shell 2.0 protocols
  //
  if (gEfiShellProtocol != NULL) {
    return (gEfiShellProtocol->GetDevicePathFromFilePath(DeviceName));
  }

  //
  // Check for EFI shell
  //
  if (mEfiShellEnvironment2 != NULL) {
    return (mEfiShellEnvironment2->NameToPath(DeviceName));
  }

  return (NULL);
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle, 
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  //EFI_INPUT_KEY *key;
  EFI_HANDLE	NewHandle;
  EFI_STATUS	Status;
  UINTN			ExitDataSizePtr;
  UINTN         i;
  CHAR16 *R=L"\\EFI\\Boot\\HelloWorld.efi";

  UINTN NoHandles = 0;
  EFI_HANDLE *Buffer = NULL;


  Status = gBS->LocateHandleBuffer(
    ByProtocol,
    &gEfiDevicePathProtocolGuid,
    NULL,
    &NoHandles,
    &Buffer
  );



// Create device path
  DevicePath = FileDevicePath(Buffer[0], R);


  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Start Boot!\n");
  
  //DevicePath=ShellGetDevicePath(R);

  for(i = 0; i < 5; ++i)
    gBS->Stall(1000000);

  //
  // Load the image with:
  // FALSE - not from boot manager and NULL, 0 being not already in memory
  //
  Status = gBS->LoadImage(
    FALSE,
    gImageHandle,
    DevicePath,
    NULL,
    0,
    &NewHandle);  
  


  if (EFI_ERROR(Status)) {
    if (NewHandle != NULL) {
      gBS->UnloadImage(NewHandle);
    }
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Error during LoadImage\n");
    for(i = 0; i < 5; ++i)
      gBS->Stall(1000000);
    return (Status);
  }
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Everything down\n");

  for(i = 0; i < 5; ++i)
    gBS->Stall(1000000);
  //
  // now start the image, passing up exit data if the caller requested it
  //
  Status = gBS->StartImage(
                     NewHandle,
                     &ExitDataSizePtr,
                     NULL
              );
  if (EFI_ERROR(Status)) {
    if (NewHandle != NULL) {
      gBS->UnloadImage(NewHandle);
    }
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Error during StartImage\n");
    return (Status);
  }

  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"start down\n");
  for(i = 0; i < 5; ++i)
    gBS->Stall(1000000);
  

  gBS->UnloadImage (NewHandle);  
  return EFI_SUCCESS;
}
