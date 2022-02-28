/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_3aade4ec_63cc_4a48_a928_5a374dd463eb
#define _AUTOGENH_3aade4ec_63cc_4a48_a928_5a374dd463eb

#ifdef __cplusplus
extern "C" {
#endif

#include <PiMm.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;

#define EFI_CALLER_ID_GUID \
  {0x3aade4ec, 0x63cc, 0x4a48, {0xa9, 0x28, 0x5a, 0x37, 0x4d, 0xd4, 0x63, 0xeb}}
#define EDKII_DSC_PLATFORM_GUID \
  {0x587CE499, 0x6CBE, 0x43cd, {0x94, 0xE2, 0x18, 0x62, 0x18, 0x56, 0x94, 0x78}}

// Guids
extern EFI_GUID gEdkiiWorkingBlockSignatureGuid;
extern EFI_GUID gEfiMdeModulePkgTokenSpaceGuid;
extern EFI_GUID gEfiMdePkgTokenSpaceGuid;

// Protocols
extern EFI_GUID gEfiSmmSwapAddressRangeProtocolGuid;
extern EFI_GUID gEfiSmmFirmwareVolumeBlockProtocolGuid;
extern EFI_GUID gEfiSmmFaultTolerantWriteProtocolGuid;
extern EFI_GUID gEfiMmEndOfDxeProtocolGuid;
extern EFI_GUID gEfiLoadedImageProtocolGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// Definition of PCDs used in this module

#define _PCD_TOKEN_PcdFullFtwServiceEnable  0U
#define _PCD_SIZE_PcdFullFtwServiceEnable 1
#define _PCD_GET_MODE_SIZE_PcdFullFtwServiceEnable  _PCD_SIZE_PcdFullFtwServiceEnable 
#define _PCD_VALUE_PcdFullFtwServiceEnable  ((BOOLEAN)1U)
extern const  BOOLEAN  _gPcd_FixedAtBuild_PcdFullFtwServiceEnable;
#define _PCD_GET_MODE_BOOL_PcdFullFtwServiceEnable  _gPcd_FixedAtBuild_PcdFullFtwServiceEnable
//#define _PCD_SET_MODE_BOOL_PcdFullFtwServiceEnable  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwWorkingBase  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwWorkingBase 4
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwWorkingBase  _PCD_SIZE_PcdFlashNvStorageFtwWorkingBase 
#define _PCD_VALUE_PcdFlashNvStorageFtwWorkingBase  0x0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingBase;
#define _PCD_GET_MODE_32_PcdFlashNvStorageFtwWorkingBase  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingBase
//#define _PCD_SET_MODE_32_PcdFlashNvStorageFtwWorkingBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwWorkingBase64  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwWorkingBase64 8
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwWorkingBase64  _PCD_SIZE_PcdFlashNvStorageFtwWorkingBase64 
#define _PCD_VALUE_PcdFlashNvStorageFtwWorkingBase64  0x0ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingBase64;
#define _PCD_GET_MODE_64_PcdFlashNvStorageFtwWorkingBase64  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingBase64
//#define _PCD_SET_MODE_64_PcdFlashNvStorageFtwWorkingBase64  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwWorkingSize  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwWorkingSize 4
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwWorkingSize  _PCD_SIZE_PcdFlashNvStorageFtwWorkingSize 
#define _PCD_VALUE_PcdFlashNvStorageFtwWorkingSize  0x0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingSize;
#define _PCD_GET_MODE_32_PcdFlashNvStorageFtwWorkingSize  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingSize
//#define _PCD_SET_MODE_32_PcdFlashNvStorageFtwWorkingSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwSpareBase  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwSpareBase 4
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwSpareBase  _PCD_SIZE_PcdFlashNvStorageFtwSpareBase 
#define _PCD_VALUE_PcdFlashNvStorageFtwSpareBase  0x0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareBase;
#define _PCD_GET_MODE_32_PcdFlashNvStorageFtwSpareBase  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareBase
//#define _PCD_SET_MODE_32_PcdFlashNvStorageFtwSpareBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwSpareBase64  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwSpareBase64 8
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwSpareBase64  _PCD_SIZE_PcdFlashNvStorageFtwSpareBase64 
#define _PCD_VALUE_PcdFlashNvStorageFtwSpareBase64  0x0ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareBase64;
#define _PCD_GET_MODE_64_PcdFlashNvStorageFtwSpareBase64  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareBase64
//#define _PCD_SET_MODE_64_PcdFlashNvStorageFtwSpareBase64  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwSpareSize  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwSpareSize 4
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwSpareSize  _PCD_SIZE_PcdFlashNvStorageFtwSpareSize 
#define _PCD_VALUE_PcdFlashNvStorageFtwSpareSize  0x0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareSize;
#define _PCD_GET_MODE_32_PcdFlashNvStorageFtwSpareSize  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareSize
//#define _PCD_SET_MODE_32_PcdFlashNvStorageFtwSpareSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

// Definition of PCDs used in libraries is in AutoGen.c


EFI_STATUS
EFIAPI
StandaloneMmFaultTolerantWriteInitialize (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_MM_SYSTEM_TABLE   *MmSystemTable
  );





#ifdef __cplusplus
}
#endif

#endif
