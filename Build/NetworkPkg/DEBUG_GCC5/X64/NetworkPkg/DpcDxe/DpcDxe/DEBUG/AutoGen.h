/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_A210F973_229D_4f4d_AA37_9895E6C9EABA
#define _AUTOGENH_A210F973_229D_4f4d_AA37_9895E6C9EABA

#ifdef __cplusplus
extern "C" {
#endif

#include <PiDxe.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;

#define EFI_CALLER_ID_GUID \
  {0xA210F973, 0x229D, 0x4f4d, {0xAA, 0x37, 0x98, 0x95, 0xE6, 0xC9, 0xEA, 0xBA}}
#define EDKII_DSC_PLATFORM_GUID \
  {0x3FD34E9B, 0xE90C, 0x44e1, {0xB5, 0x10, 0x1F, 0x63, 0x2A, 0x50, 0x9F, 0x10}}

// Guids
extern EFI_GUID gEfiMdePkgTokenSpaceGuid;

// Protocols
extern EFI_GUID gEfiDpcProtocolGuid;
extern EFI_GUID gEfiLoadedImageProtocolGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// Definition of PCDs used in libraries is in AutoGen.c


EFI_STATUS
EFIAPI
DpcDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );





#ifdef __cplusplus
}
#endif

#endif
