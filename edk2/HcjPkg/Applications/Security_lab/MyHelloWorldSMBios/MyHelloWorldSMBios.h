/*************************************************************************
        > File Name: MyHelloWorldSMBios.h
        > Author:1234
        > E-mail:1234@163.com
        > Created Time: Tues Apr 19 10:38:50 2022
 ************************************************************************/


#ifndef HARDWARE_INFO_H
#define HARDWARE_INFO_H

#include <Uefi.h>
// uefiLib中有Print（L）函数，在shell里实验不能用printf
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Smbios.h>
#include "IndustryStandard/SmBios.h" // from EDK2. GNU_EFI libsmbios.h is defective
#include "cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/UsbIo.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/FileInfo.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathUtilities.h>
#include <Protocol/DevicePath.h>
#define EFI_SMBIOS_TABLE_GUID                                                         \
    {                                                                                 \
        0xeb9d2d31, 0x2d88, 0x11d3, { 0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } \
    }
#define INVALID_HANDLE (UINT16)(-1)
#define DMI_INVALID_HANDLE 0x83
#define DMI_SUCCESS 0x00

EFI_GUID SMBIOSTableGuid = EFI_SMBIOS_TABLE_GUID;
STATIC SMBIOS_TABLE_ENTRY_POINT *mSmbiosTable = 0; // NULL;
STATIC SMBIOS_STRUCTURE_POINTER m_SmbiosStruct;
STATIC SMBIOS_STRUCTURE_POINTER *mSmbiosStruct = &m_SmbiosStruct;

EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *gPciRootBridgeIo;

extern EFI_BOOT_SERVICES *gBS;
extern EFI_SYSTEM_TABLE *gST;

// EFI_GUID gEfiUsbIoProtocolGuid =
//     {0x2B2F68D6, 0x0CD2, 0x44CF, {0x8E, 0x8B, 0xBB, 0xA2, 0x0B, 0x1B, 0x5B, 0x75}};
EFI_GUID gEfiPciRootBridgeIoProtocolGuid =
    {0x2F707EBB, 0x4A1A, 0x11D4, {0x9A, 0x38, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D}};



cJSON* get_info(char* username);
cJSON* get_array();


/**
 * \brief                Compares two GUIDs
 * \param Guid1          guid to compare
 * \param Guid2          guid to compare
 */
BOOLEAN EfiCompareGuid(
    IN EFI_GUID *Guid1,
    IN EFI_GUID *Guid2);


/**
 * \brief                Get table from configuration table by name
 * \param TableGuid      Table name to search
 * \param Table          Pointer to the table caller wants
 */
EFI_STATUS 
EfiLibGetSystemConfigurationTable(
    IN EFI_GUID *TableGuid,
    OUT VOID **Table);

/**
 * \brief                Return SMBIOS string for the given string number
 * \param Smbios         Pointer to SMBIOS structure.
 * \param StringNumber   String number to return. -1 is used to skip all strings and
 *                       point to the next SMBIOS structure.
 */
CHAR8 *hcj_LibGetSmbiosString(
    SMBIOS_STRUCTURE_POINTER *Smbios,
    UINT16 StringNumber);


/**
 * \brief                Get SMBIOS structure for the given Handle,
 *                       Handle is changed to the next handle or 0xFFFF when the end is
 *                       reached or the handle is not found.
 * \param Handle         0xFFFF: get the first structure
 *                       Others: get a structure according to this value.
 * \param Buffer         The pointer to the pointer to the structure.
 * \param Length         Length of the structure.             
 */
EFI_STATUS
LibGetSmbiosStructure(
    UINT16 *Handle,
    UINT8 **Buffer,
    UINT16 *Length);


/**
 * \brief                Calculate the length of str
 * \param str            char* string            
 */
int Strlen(const char *str);

/**
 * \brief                Get SMBIOS information according to SMBIOS structure of the given Handle,
 *                       SMBIOS info will be transformed into json format and stored in array
 *                       The process of finding SMBIOS info will be recorded in FileLog
 * \param array          Json array, to store SMBIOS info 
 * \param FileHandle     The handle of "InfoLog.txt", to record events             
 */
EFI_STATUS
EFIAPI
SmbiosInfo(cJSON *array, EFI_FILE_PROTOCOL *FileHandle);

/**
 * \brief                To initiate a EFI_FILE_PROTOCOL as root handle by LocateProtocol 
 * \param Root           EFI file protocol handle          
 */
EFI_STATUS
IntiateFileRoot(EFI_FILE_PROTOCOL **Root);


/**
 * \brief                Intiate PCI handle gPciRootBridgeIo by  LocatePciRootBridgeIo
 * \param FileHandle     The handle of "InfoLog.txt", to record events               
 */
EFI_STATUS 
LocatePciRootBridgeIo(EFI_FILE_PROTOCOL *FileHandle);


/**
 * \brief                       Check if the pci device with the BDF is on the pcirootbrige
 * \param PciRootBridgeIo       the handle of EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL
 * \param Pci                   Pci device struct, contains handle and header of pci device 
 * \param Bus                   a code to locate the only pci device 
 * \param Device                a code to locate the only pci device 
 * \param Func                  a code to locate the only pci device 
 */
EFI_STATUS 
PciDevicePresent(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo,
    OUT PCI_TYPE00 *Pci,
    IN UINT8 Bus,
    IN UINT8 Device,
    IN UINT8 Func);


/**
 * \brief                
 * \param array          Json array, to store PCI info
 * \param FileHandle     The handle of "InfoLog.txt", to record events          
 */
EFI_STATUS 
ListPciInformation(
    cJSON *array,  
    EFI_FILE_PROTOCOL *FileHandle);


/**
 * \brief                
 * \param json           json root, to store USB info   
 * \param FileHandle     The handle of "InfoLog.txt", to record events          
 */
UINTN 
GetUSB(
    cJSON *json, 
    EFI_FILE_PROTOCOL *FileHandle);

/**
 * \brief                
 * \param json           json root, to store DevicePath info         
 */
EFI_STATUS 
GetDevicePath(    cJSON *json    );

#endif