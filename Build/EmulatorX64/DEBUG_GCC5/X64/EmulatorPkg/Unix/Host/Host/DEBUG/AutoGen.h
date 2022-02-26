/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_8863C0AD_7724_C84B_88E5_A33B116D1485
#define _AUTOGENH_8863C0AD_7724_C84B_88E5_A33B116D1485

#ifdef __cplusplus
extern "C" {
#endif

#include <Base.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;

#define EFI_CALLER_ID_GUID \
  {0x8863C0AD, 0x7724, 0xC84B, {0x88, 0xE5, 0xA3, 0x3B, 0x11, 0x6D, 0x14, 0x85}}
#define EDKII_DSC_PLATFORM_GUID \
  {0x05FD064D, 0x1073, 0xE844, {0x93, 0x6C, 0xA0, 0xE1, 0x63, 0x17, 0x10, 0x7D}}

// Guids
extern GUID gEfiFileSystemVolumeLabelInfoIdGuid;
extern GUID gEfiFileInfoGuid;
extern GUID gEfiFileSystemInfoGuid;
extern GUID gEmulatorPkgTokenSpaceGuid;
extern GUID gEfiMdeModulePkgTokenSpaceGuid;
extern GUID gEfiMdePkgTokenSpaceGuid;

// Protocols
extern GUID gEmuIoThunkProtocolGuid;
extern GUID gEmuGraphicsWindowProtocolGuid;
extern GUID gEmuThreadThunkProtocolGuid;
extern GUID gEmuBlockIoProtocolGuid;
extern GUID gEmuSnpProtocolGuid;
extern GUID gEfiSimpleFileSystemProtocolGuid;

// PPIs
extern GUID gEfiPeiStatusCodePpiGuid;
extern GUID gEmuThunkPpiGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// Definition of PCDs used in this module

#define _PCD_TOKEN_PcdEmulatorLazyLoadSymbols  0U
#define _PCD_SIZE_PcdEmulatorLazyLoadSymbols 1
#define _PCD_GET_MODE_SIZE_PcdEmulatorLazyLoadSymbols  _PCD_SIZE_PcdEmulatorLazyLoadSymbols 
#define _PCD_VALUE_PcdEmulatorLazyLoadSymbols  ((BOOLEAN)1U)
extern const  BOOLEAN  _gPcd_FixedAtBuild_PcdEmulatorLazyLoadSymbols;
#define _PCD_GET_MODE_BOOL_PcdEmulatorLazyLoadSymbols  _gPcd_FixedAtBuild_PcdEmulatorLazyLoadSymbols
//#define _PCD_SET_MODE_BOOL_PcdEmulatorLazyLoadSymbols  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuBootMode  0U
#define _PCD_SIZE_PcdEmuBootMode 4
#define _PCD_GET_MODE_SIZE_PcdEmuBootMode  _PCD_SIZE_PcdEmuBootMode 
#define _PCD_VALUE_PcdEmuBootMode  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdEmuBootMode;
#define _PCD_GET_MODE_32_PcdEmuBootMode  _gPcd_FixedAtBuild_PcdEmuBootMode
//#define _PCD_SET_MODE_32_PcdEmuBootMode  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFirmwareVolume  0U
#define _PCD_VALUE_PcdEmuFirmwareVolume  _gPcd_FixedAtBuild_PcdEmuFirmwareVolume
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuFirmwareVolume[21];
#define _PCD_GET_MODE_PTR_PcdEmuFirmwareVolume  _gPcd_FixedAtBuild_PcdEmuFirmwareVolume
#define _PCD_SIZE_PcdEmuFirmwareVolume 42
#define _PCD_GET_MODE_SIZE_PcdEmuFirmwareVolume  _PCD_SIZE_PcdEmuFirmwareVolume 
//#define _PCD_SET_MODE_PTR_PcdEmuFirmwareVolume  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuMemorySize  0U
#define _PCD_VALUE_PcdEmuMemorySize  _gPcd_FixedAtBuild_PcdEmuMemorySize
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuMemorySize[6];
#define _PCD_GET_MODE_PTR_PcdEmuMemorySize  _gPcd_FixedAtBuild_PcdEmuMemorySize
#define _PCD_SIZE_PcdEmuMemorySize 12
#define _PCD_GET_MODE_SIZE_PcdEmuMemorySize  _PCD_SIZE_PcdEmuMemorySize 
//#define _PCD_SET_MODE_PTR_PcdEmuMemorySize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFdBaseAddress  0U
#define _PCD_SIZE_PcdEmuFdBaseAddress 8
#define _PCD_GET_MODE_SIZE_PcdEmuFdBaseAddress  _PCD_SIZE_PcdEmuFdBaseAddress 
#define _PCD_VALUE_PcdEmuFdBaseAddress  0x102000000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdEmuFdBaseAddress;
#define _PCD_GET_MODE_64_PcdEmuFdBaseAddress  _gPcd_FixedAtBuild_PcdEmuFdBaseAddress
//#define _PCD_SET_MODE_64_PcdEmuFdBaseAddress  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFirmwareFdSize  0U
#define _PCD_SIZE_PcdEmuFirmwareFdSize 4
#define _PCD_GET_MODE_SIZE_PcdEmuFirmwareFdSize  _PCD_SIZE_PcdEmuFirmwareFdSize 
#define _PCD_VALUE_PcdEmuFirmwareFdSize  0x005a0000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdEmuFirmwareFdSize;
#define _PCD_GET_MODE_32_PcdEmuFirmwareFdSize  _gPcd_FixedAtBuild_PcdEmuFirmwareFdSize
//#define _PCD_SET_MODE_32_PcdEmuFirmwareFdSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFirmwareBlockSize  0U
#define _PCD_SIZE_PcdEmuFirmwareBlockSize 4
#define _PCD_GET_MODE_SIZE_PcdEmuFirmwareBlockSize  _PCD_SIZE_PcdEmuFirmwareBlockSize 
#define _PCD_VALUE_PcdEmuFirmwareBlockSize  0x10000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdEmuFirmwareBlockSize;
#define _PCD_GET_MODE_32_PcdEmuFirmwareBlockSize  _gPcd_FixedAtBuild_PcdEmuFirmwareBlockSize
//#define _PCD_SET_MODE_32_PcdEmuFirmwareBlockSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuApCount  0U
#define _PCD_VALUE_PcdEmuApCount  _gPcd_FixedAtBuild_PcdEmuApCount
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuApCount[2];
#define _PCD_GET_MODE_PTR_PcdEmuApCount  _gPcd_FixedAtBuild_PcdEmuApCount
#define _PCD_SIZE_PcdEmuApCount 4
#define _PCD_GET_MODE_SIZE_PcdEmuApCount  _PCD_SIZE_PcdEmuApCount 
//#define _PCD_SET_MODE_PTR_PcdEmuApCount  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuVirtualDisk  0U
#define _PCD_VALUE_PcdEmuVirtualDisk  _gPcd_FixedAtBuild_PcdEmuVirtualDisk
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuVirtualDisk[12];
#define _PCD_GET_MODE_PTR_PcdEmuVirtualDisk  _gPcd_FixedAtBuild_PcdEmuVirtualDisk
#define _PCD_SIZE_PcdEmuVirtualDisk 24
#define _PCD_GET_MODE_SIZE_PcdEmuVirtualDisk  _PCD_SIZE_PcdEmuVirtualDisk 
//#define _PCD_SET_MODE_PTR_PcdEmuVirtualDisk  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuGop  0U
#define _PCD_VALUE_PcdEmuGop  _gPcd_FixedAtBuild_PcdEmuGop
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuGop[11];
#define _PCD_GET_MODE_PTR_PcdEmuGop  _gPcd_FixedAtBuild_PcdEmuGop
#define _PCD_SIZE_PcdEmuGop 22
#define _PCD_GET_MODE_SIZE_PcdEmuGop  _PCD_SIZE_PcdEmuGop 
//#define _PCD_SET_MODE_PTR_PcdEmuGop  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFileSystem  0U
#define _PCD_VALUE_PcdEmuFileSystem  _gPcd_FixedAtBuild_PcdEmuFileSystem
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuFileSystem[2];
#define _PCD_GET_MODE_PTR_PcdEmuFileSystem  _gPcd_FixedAtBuild_PcdEmuFileSystem
#define _PCD_SIZE_PcdEmuFileSystem 4
#define _PCD_GET_MODE_SIZE_PcdEmuFileSystem  _PCD_SIZE_PcdEmuFileSystem 
//#define _PCD_SET_MODE_PTR_PcdEmuFileSystem  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuSerialPort  0U
#define _PCD_VALUE_PcdEmuSerialPort  _gPcd_FixedAtBuild_PcdEmuSerialPort
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuSerialPort[11];
#define _PCD_GET_MODE_PTR_PcdEmuSerialPort  _gPcd_FixedAtBuild_PcdEmuSerialPort
#define _PCD_SIZE_PcdEmuSerialPort 22
#define _PCD_GET_MODE_SIZE_PcdEmuSerialPort  _PCD_SIZE_PcdEmuSerialPort 
//#define _PCD_SET_MODE_PTR_PcdEmuSerialPort  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuNetworkInterface  0U
#define _PCD_VALUE_PcdEmuNetworkInterface  _gPcd_FixedAtBuild_PcdEmuNetworkInterface
extern const UINT16 _gPcd_FixedAtBuild_PcdEmuNetworkInterface[4];
#define _PCD_GET_MODE_PTR_PcdEmuNetworkInterface  _gPcd_FixedAtBuild_PcdEmuNetworkInterface
#define _PCD_SIZE_PcdEmuNetworkInterface 8
#define _PCD_GET_MODE_SIZE_PcdEmuNetworkInterface  _PCD_SIZE_PcdEmuNetworkInterface 
//#define _PCD_SET_MODE_PTR_PcdEmuNetworkInterface  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdNetworkPacketFilterSize  0U
#define _PCD_SIZE_PcdNetworkPacketFilterSize 4
#define _PCD_GET_MODE_SIZE_PcdNetworkPacketFilterSize  _PCD_SIZE_PcdNetworkPacketFilterSize 
#define _PCD_VALUE_PcdNetworkPacketFilterSize  524288U
extern const  UINT32  _gPcd_FixedAtBuild_PcdNetworkPacketFilterSize;
#define _PCD_GET_MODE_32_PcdNetworkPacketFilterSize  _gPcd_FixedAtBuild_PcdNetworkPacketFilterSize
//#define _PCD_SET_MODE_32_PcdNetworkPacketFilterSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFlashFvRecoveryBase  0U
#define _PCD_SIZE_PcdEmuFlashFvRecoveryBase 8
#define _PCD_GET_MODE_SIZE_PcdEmuFlashFvRecoveryBase  _PCD_SIZE_PcdEmuFlashFvRecoveryBase 
#define _PCD_VALUE_PcdEmuFlashFvRecoveryBase  0x102000000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdEmuFlashFvRecoveryBase;
#define _PCD_GET_MODE_64_PcdEmuFlashFvRecoveryBase  _gPcd_FixedAtBuild_PcdEmuFlashFvRecoveryBase
//#define _PCD_SET_MODE_64_PcdEmuFlashFvRecoveryBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFlashFvRecoverySize  0U
#define _PCD_SIZE_PcdEmuFlashFvRecoverySize 4
#define _PCD_GET_MODE_SIZE_PcdEmuFlashFvRecoverySize  _PCD_SIZE_PcdEmuFlashFvRecoverySize 
#define _PCD_VALUE_PcdEmuFlashFvRecoverySize  0x00580000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdEmuFlashFvRecoverySize;
#define _PCD_GET_MODE_32_PcdEmuFlashFvRecoverySize  _gPcd_FixedAtBuild_PcdEmuFlashFvRecoverySize
//#define _PCD_SET_MODE_32_PcdEmuFlashFvRecoverySize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFlashNvStorageVariableBase  0U
#define _PCD_SIZE_PcdEmuFlashNvStorageVariableBase 8
#define _PCD_GET_MODE_SIZE_PcdEmuFlashNvStorageVariableBase  _PCD_SIZE_PcdEmuFlashNvStorageVariableBase 
#define _PCD_VALUE_PcdEmuFlashNvStorageVariableBase  0x102580000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageVariableBase;
#define _PCD_GET_MODE_64_PcdEmuFlashNvStorageVariableBase  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageVariableBase
//#define _PCD_SET_MODE_64_PcdEmuFlashNvStorageVariableBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageVariableSize  0U
#define _PCD_SIZE_PcdFlashNvStorageVariableSize 4
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageVariableSize  _PCD_SIZE_PcdFlashNvStorageVariableSize 
#define _PCD_VALUE_PcdFlashNvStorageVariableSize  0x0000C000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFlashNvStorageVariableSize;
#define _PCD_GET_MODE_32_PcdFlashNvStorageVariableSize  _gPcd_FixedAtBuild_PcdFlashNvStorageVariableSize
//#define _PCD_SET_MODE_32_PcdFlashNvStorageVariableSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFlashNvStorageEventLogBase  0U
#define _PCD_SIZE_PcdEmuFlashNvStorageEventLogBase 8
#define _PCD_GET_MODE_SIZE_PcdEmuFlashNvStorageEventLogBase  _PCD_SIZE_PcdEmuFlashNvStorageEventLogBase 
#define _PCD_VALUE_PcdEmuFlashNvStorageEventLogBase  0x10258C000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageEventLogBase;
#define _PCD_GET_MODE_64_PcdEmuFlashNvStorageEventLogBase  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageEventLogBase
//#define _PCD_SET_MODE_64_PcdEmuFlashNvStorageEventLogBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFlashNvStorageEventLogSize  0U
#define _PCD_SIZE_PcdEmuFlashNvStorageEventLogSize 4
#define _PCD_GET_MODE_SIZE_PcdEmuFlashNvStorageEventLogSize  _PCD_SIZE_PcdEmuFlashNvStorageEventLogSize 
#define _PCD_VALUE_PcdEmuFlashNvStorageEventLogSize  0x00002000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageEventLogSize;
#define _PCD_GET_MODE_32_PcdEmuFlashNvStorageEventLogSize  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageEventLogSize
//#define _PCD_SET_MODE_32_PcdEmuFlashNvStorageEventLogSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFlashNvStorageFtwWorkingBase  0U
#define _PCD_SIZE_PcdEmuFlashNvStorageFtwWorkingBase 8
#define _PCD_GET_MODE_SIZE_PcdEmuFlashNvStorageFtwWorkingBase  _PCD_SIZE_PcdEmuFlashNvStorageFtwWorkingBase 
#define _PCD_VALUE_PcdEmuFlashNvStorageFtwWorkingBase  0x10258E000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageFtwWorkingBase;
#define _PCD_GET_MODE_64_PcdEmuFlashNvStorageFtwWorkingBase  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageFtwWorkingBase
//#define _PCD_SET_MODE_64_PcdEmuFlashNvStorageFtwWorkingBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwWorkingSize  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwWorkingSize 4
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwWorkingSize  _PCD_SIZE_PcdFlashNvStorageFtwWorkingSize 
#define _PCD_VALUE_PcdFlashNvStorageFtwWorkingSize  0x00002000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingSize;
#define _PCD_GET_MODE_32_PcdFlashNvStorageFtwWorkingSize  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwWorkingSize
//#define _PCD_SET_MODE_32_PcdFlashNvStorageFtwWorkingSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdEmuFlashNvStorageFtwSpareBase  0U
#define _PCD_SIZE_PcdEmuFlashNvStorageFtwSpareBase 8
#define _PCD_GET_MODE_SIZE_PcdEmuFlashNvStorageFtwSpareBase  _PCD_SIZE_PcdEmuFlashNvStorageFtwSpareBase 
#define _PCD_VALUE_PcdEmuFlashNvStorageFtwSpareBase  0x102590000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageFtwSpareBase;
#define _PCD_GET_MODE_64_PcdEmuFlashNvStorageFtwSpareBase  _gPcd_FixedAtBuild_PcdEmuFlashNvStorageFtwSpareBase
//#define _PCD_SET_MODE_64_PcdEmuFlashNvStorageFtwSpareBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFlashNvStorageFtwSpareSize  0U
#define _PCD_SIZE_PcdFlashNvStorageFtwSpareSize 4
#define _PCD_GET_MODE_SIZE_PcdFlashNvStorageFtwSpareSize  _PCD_SIZE_PcdFlashNvStorageFtwSpareSize 
#define _PCD_VALUE_PcdFlashNvStorageFtwSpareSize  0x00010000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareSize;
#define _PCD_GET_MODE_32_PcdFlashNvStorageFtwSpareSize  _gPcd_FixedAtBuild_PcdFlashNvStorageFtwSpareSize
//#define _PCD_SET_MODE_32_PcdFlashNvStorageFtwSpareSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdPeiServicesTablePage  0U
#define _PCD_SIZE_PcdPeiServicesTablePage 8
#define _PCD_GET_MODE_SIZE_PcdPeiServicesTablePage  _PCD_SIZE_PcdPeiServicesTablePage 
#define _PCD_VALUE_PcdPeiServicesTablePage  0x1013000000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdPeiServicesTablePage;
#define _PCD_GET_MODE_64_PcdPeiServicesTablePage  _gPcd_FixedAtBuild_PcdPeiServicesTablePage
//#define _PCD_SET_MODE_64_PcdPeiServicesTablePage  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

// Definition of PCDs used in libraries is in AutoGen.c


#ifdef __cplusplus
}
#endif

#endif
