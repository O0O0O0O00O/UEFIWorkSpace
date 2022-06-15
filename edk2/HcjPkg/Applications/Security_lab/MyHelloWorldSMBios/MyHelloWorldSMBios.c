#include "get_info.h"

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

EFI_STATUS LocatePciRootBridgeIo(EFI_FILE_PROTOCOL *FileHandle);

EFI_STATUS PciDevicePresent(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo,
    OUT PCI_TYPE00 *Pci,
    IN UINT8 Bus,
    IN UINT8 Device,
    IN UINT8 Func);

EFI_STATUS ListPciInformation(cJSON *array, EFI_FILE_PROTOCOL *FileHandle);

UINTN GetUSB(cJSON *json, EFI_FILE_PROTOCOL *FileHandle);
EFI_STATUS 
GetDevicePath(    cJSON *json    );

extern EFI_BOOT_SERVICES *gBS;
extern EFI_SYSTEM_TABLE *gST;

// EFI_GUID gEfiUsbIoProtocolGuid =
//     {0x2B2F68D6, 0x0CD2, 0x44CF, {0x8E, 0x8B, 0xBB, 0xA2, 0x0B, 0x1B, 0x5B, 0x75}};
// EFI_GUID gEfiPciRootBridgeIoProtocolGuid =
//     {0x2F707EBB, 0x4A1A, 0x11D4, {0x9A, 0x38, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D}};

BOOLEAN
EfiCompareGuid(
    IN EFI_GUID *Guid1,
    IN EFI_GUID *Guid2)
/*++

Routine Description:

  Compares two GUIDs

Arguments:

  Guid1 - guid to compare

  Guid2 - guid to compare

Returns:
  TRUE     if Guid1 == Guid2
  FALSE    if Guid1 != Guid2

--*/
{
    UINTN Index;

    //
    // compare byte by byte
    //
    for (Index = 0; Index < 16; ++Index)
    {
        if (*(((UINT8 *)Guid1) + Index) != *(((UINT8 *)Guid2) + Index))
        {
            return FALSE;
        }
    }
    return TRUE;
}

EFI_STATUS
EfiLibGetSystemConfigurationTable(
    IN EFI_GUID *TableGuid,
    OUT VOID **Table)
/*++

Routine Description:

  Get table from configuration table by name

Arguments:

  TableGuid       - Table name to search

  Table           - Pointer to the table caller wants

Returns:

  EFI_NOT_FOUND   - Not found the table

  EFI_SUCCESS     - Found the table

--*/
{
    UINTN Index;

    *Table = NULL;
    for (Index = 0; Index < gST->NumberOfTableEntries; Index++)
    {
        if (EfiCompareGuid(TableGuid, &(gST->ConfigurationTable[Index].VendorGuid)) == TRUE)
        {
            *Table = gST->ConfigurationTable[Index].VendorTable;
            return EFI_SUCCESS;
        }
    }

    return EFI_NOT_FOUND;
}

CHAR8 *
hcj_LibGetSmbiosString(SMBIOS_STRUCTURE_POINTER *Smbios,
                   UINT16 StringNumber)
{
    UINT16 Index;
    CHAR8 *String;

    // ASSERT (Smbios != NULL);

    String = (CHAR8 *)(Smbios->Raw + Smbios->Hdr->Length);

    for (Index = 1; Index <= StringNumber; Index++)
    {
        if (StringNumber == Index)
        {
            return String;
        }
        for (; *String != 0; String++)
            ;
        String++;

        if (*String == 0)
        {
            Smbios->Raw = (UINT8 *)++String;
            return NULL;
        }
    }

    return NULL;
}

EFI_STATUS
hcj_LibGetSmbiosStructure(UINT16 *Handle,
                      UINT8 **Buffer,
                      UINT16 *Length)
{
    SMBIOS_STRUCTURE_POINTER Smbios;
    SMBIOS_STRUCTURE_POINTER SmbiosEnd;
    UINT8 *Raw;

    if (*Handle == INVALID_HANDLE)
    {
        *Handle = mSmbiosStruct->Hdr->Handle;
        return DMI_INVALID_HANDLE;
    }

    if ((Buffer == NULL) || (Length == NULL))
    {
        Print(L"Invalid handle\n");
        return DMI_INVALID_HANDLE;
    }

    *Length = 0;
    Smbios.Hdr = mSmbiosStruct->Hdr;
    SmbiosEnd.Raw = Smbios.Raw + mSmbiosTable->TableLength;
    while (Smbios.Raw < SmbiosEnd.Raw)
    {
        if (Smbios.Hdr->Handle == *Handle)
        {
            Raw = Smbios.Raw;
            hcj_LibGetSmbiosString(&Smbios, (UINT16)(-1));
            *Length = (UINT16)(Smbios.Raw - Raw);
            *Buffer = Raw;
            if (Smbios.Raw < SmbiosEnd.Raw)
            {
                *Handle = Smbios.Hdr->Handle;
            }
            else
            {
                *Handle = INVALID_HANDLE;
            }
            return DMI_SUCCESS;
        }
        hcj_LibGetSmbiosString(&Smbios, (UINT16)(-1));
    }

    *Handle = INVALID_HANDLE;

    return DMI_INVALID_HANDLE;
}

int Strlen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}

EFI_STATUS
EFIAPI
SmbiosInfo(cJSON *array, EFI_FILE_PROTOCOL *FileHandle)
{
    EFI_STATUS status = EFI_SUCCESS;
    CHAR16 *ptr;
    CHAR8 *str;
    UINT16 Handle;
    UINTN Index;
    UINT16 Length;
    UINT8 *Buffer;
    SMBIOS_STRUCTURE_POINTER SmbiosStruct;

    CHAR16 *Textbuf;
    UINTN BufSize;

    // InitializeLib(image, systab);
    // Find the SMBIOS table
    Textbuf = (CHAR16 *)L"COLLETCING SMBIOS INFO START.\r\n";
    BufSize = StrLen(Textbuf) * 2;
    FileHandle->Write(FileHandle, &BufSize, Textbuf);

    status = EfiLibGetSystemConfigurationTable(&SMBIOSTableGuid, (VOID **)&mSmbiosTable);
    if (status != EFI_SUCCESS)
    {
        Textbuf = (CHAR16 *)L"ERROR: SMBIOS table not found.\r\n";
        BufSize = StrLen(Textbuf) * 2;
        FileHandle->Write(FileHandle, &BufSize, Textbuf);
        return status;
    }

    mSmbiosStruct->Raw = (UINT8 *)(UINTN)(mSmbiosTable->TableAddress);

    // Print(L"SMBIOS Ver: %x.%x  Rev: %x  Table Count: %d\n",
    //       mSmbiosTable->MajorVersion,
    //       mSmbiosTable->MinorVersion,
    //       mSmbiosTable->EntryPointRevision,
    //       mSmbiosTable->NumberOfSmbiosStructures);

    Handle = INVALID_HANDLE;
    hcj_LibGetSmbiosStructure(&Handle, NULL, NULL);

    //在数组上添加对象
    cJSON *obj1 = NULL;
    cJSON_AddItemToArray(array, obj1 = cJSON_CreateObject());
    int count1 = 1;
    //向对象中添加smbios信息
    cJSON_AddItemToObject(obj1, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj1, "SubType", cJSON_CreateString("SMBIOS_TYPE_BIOS_INFORMATION"));
    //在对象中再添加一个数组，包含同一个type的多个设备信息
    cJSON *subArray1 = cJSON_CreateArray();

    cJSON *obj2 = NULL;
    cJSON_AddItemToArray(array, obj2 = cJSON_CreateObject());
    int count2 = 1;
    cJSON_AddItemToObject(obj2, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj2, "SubType", cJSON_CreateString("SMBIOS_TYPE_SYSTEM_INFORMATION"));
    cJSON *subArray2 = cJSON_CreateArray();

    cJSON *obj3 = NULL;
    cJSON_AddItemToArray(array, obj3 = cJSON_CreateObject());
    int count3 = 1;
    cJSON_AddItemToObject(obj3, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj3, "SubType", cJSON_CreateString("SMBIOS_TYPE_BASEBOARD_INFORMATION"));
    cJSON *subArray3 = cJSON_CreateArray();

    cJSON *obj4 = NULL;
    cJSON_AddItemToArray(array, obj4 = cJSON_CreateObject());
    int count4 = 1;
    cJSON_AddItemToObject(obj4, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj4, "SubType", cJSON_CreateString("SMBIOS_TYPE_SYSTEM_ENCLOSURE"));
    cJSON *subArray4 = cJSON_CreateArray();

    cJSON *obj5 = NULL;
    cJSON_AddItemToArray(array, obj5 = cJSON_CreateObject());
    int count5 = 1;
    cJSON_AddItemToObject(obj5, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj5, "SubType", cJSON_CreateString("SMBIOS_TYPE_PROCESSOR_INFORMATION"));
    cJSON *subArray5 = cJSON_CreateArray();

    cJSON *obj6 = NULL;
    cJSON_AddItemToArray(array, obj6 = cJSON_CreateObject());
    int count6 = 1;
    cJSON_AddItemToObject(obj6, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj6, "SubType", cJSON_CreateString("SMBIOS_TYPE_CACHE_INFORMATION"));
    cJSON *subArray6 = cJSON_CreateArray();

    cJSON *obj7 = NULL;
    cJSON_AddItemToArray(array, obj7 = cJSON_CreateObject());
    int count7 = 1;
    cJSON_AddItemToObject(obj7, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj7, "SubType", cJSON_CreateString("SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION"));
    cJSON *subArray7 = cJSON_CreateArray();

    cJSON *obj8 = NULL;
    cJSON_AddItemToArray(array, obj8 = cJSON_CreateObject());
    int count8 = 1;
    cJSON_AddItemToObject(obj8, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj8, "SubType", cJSON_CreateString("SMBIOS_TYPE_SYSTEM_SLOTS"));
    cJSON *subArray8 = cJSON_CreateArray();

    cJSON *obj9 = NULL;
    cJSON_AddItemToArray(array, obj9 = cJSON_CreateObject());
    int count9 = 1;
    cJSON_AddItemToObject(obj9, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj9, "SubType", cJSON_CreateString("SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY"));
    cJSON *subArray9 = cJSON_CreateArray();

    cJSON *obj10 = NULL;
    cJSON_AddItemToArray(array, obj10 = cJSON_CreateObject());
    int count10 = 1;
    cJSON_AddItemToObject(obj10, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj10, "SubType", cJSON_CreateString("SMBIOS_TYPE_MEMORY_DEVICE"));
    cJSON *subArray10 = cJSON_CreateArray();

    cJSON *obj11 = NULL;
    cJSON_AddItemToArray(array, obj11 = cJSON_CreateObject());
    int count11 = 1;
    cJSON_AddItemToObject(obj11, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj11, "SubType", cJSON_CreateString("SMBIOS_TYPE_BUILT_IN_POINTING_DEVICE"));
    cJSON *subArray11 = cJSON_CreateArray();

    cJSON *obj12 = NULL;
    cJSON_AddItemToArray(array, obj12 = cJSON_CreateObject());
    int count12 = 1;
    cJSON_AddItemToObject(obj12, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj12, "SubType", cJSON_CreateString("SMBIOS_TYPE_PORTABLE_BATTERY"));
    cJSON *subArray12 = cJSON_CreateArray();

    cJSON *obj13 = NULL;
    cJSON_AddItemToArray(array, obj13 = cJSON_CreateObject());
    int count13 = 1;
    cJSON_AddItemToObject(obj13, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj13, "SubType", cJSON_CreateString("SMBIOS_TYPE_COOLING_DEVICE"));
    cJSON *subArray13 = cJSON_CreateArray();

    cJSON *obj14 = NULL;
    cJSON_AddItemToArray(array, obj14 = cJSON_CreateObject());
    int count14 = 1;
    cJSON_AddItemToObject(obj14, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj14, "SubType", cJSON_CreateString("SMBIOS_TYPE_OUT_OF_BAND_REMOTE_ACCESS"));
    cJSON *subArray14 = cJSON_CreateArray();

    cJSON *obj15 = NULL;
    cJSON_AddItemToArray(array, obj15 = cJSON_CreateObject());
    int count15 = 1;
    cJSON_AddItemToObject(obj15, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj15, "SubType", cJSON_CreateString("SMBIOS_TYPE_MANAGEMENT_DEVICE"));
    cJSON *subArray15 = cJSON_CreateArray();

    cJSON *obj16 = NULL;
    cJSON_AddItemToArray(array, obj16 = cJSON_CreateObject());
    int count16 = 1;
    cJSON_AddItemToObject(obj16, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj16, "SubType", cJSON_CreateString("SMBIOS_TYPE_IPMI_DEVICE_INFORMATION"));
    cJSON *subArray16 = cJSON_CreateArray();

    cJSON *obj17 = NULL;
    cJSON_AddItemToArray(array, obj17 = cJSON_CreateObject());
    int count17 = 1;
    cJSON_AddItemToObject(obj17, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj17, "SubType", cJSON_CreateString("SMBIOS_TYPE_SYSTEM_POWER_SUPPLY"));
    cJSON *subArray17 = cJSON_CreateArray();

    cJSON *obj18 = NULL;
    cJSON_AddItemToArray(array, obj18 = cJSON_CreateObject());
    int count18 = 1;
    cJSON_AddItemToObject(obj18, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj18, "SubType", cJSON_CreateString("SMBIOS_TYPE_ONBOARD_DEVICES_EXTENDED_INFORMATION"));
    cJSON *subArray18 = cJSON_CreateArray();

    cJSON *obj19 = NULL;
    cJSON_AddItemToArray(array, obj19 = cJSON_CreateObject());
    int count19 = 1;
    cJSON_AddItemToObject(obj19, "Type", cJSON_CreateString("Smbios"));
    cJSON_AddItemToObject(obj19, "SubType", cJSON_CreateString("SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS"));
    cJSON *subArray19 = cJSON_CreateArray();

    //临时对象，用于存储每一条存在obj[i]的信息
    cJSON *pointTempObj;

    // loop though the tables looking for a type 0 table.
    for (Index = 0; Index < mSmbiosTable->NumberOfSmbiosStructures; Index++)
    {
        if (Handle == INVALID_HANDLE)
        {
            break;
        }
        if (hcj_LibGetSmbiosStructure(&Handle, &Buffer, &Length) != DMI_SUCCESS)
        {
            break;
        }
        SmbiosStruct.Raw = Buffer;
        if (SmbiosStruct.Hdr->Type == 0)
        { // Type 0 - BIOS

            SMBIOS_TABLE_TYPE0 *Type0Record = (SMBIOS_TABLE_TYPE0 *)SmbiosStruct.Hdr;
            //把对象添加到数组里
            cJSON_AddItemToArray(subArray1, pointTempObj = cJSON_CreateObject());
            //在对象上添加键值对

            /* vendor string */
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "Vendor", str);

            /* version string */
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "BiosVersion", str);

            /* release string */
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "ReleaseDate", str);

            cJSON_AddNumberToObject(pointTempObj, "BiosSegment", Type0Record->BiosSegment);
            cJSON_AddNumberToObject(pointTempObj, "BiosSize", Type0Record->BiosSize);
            cJSON_AddNumberToObject(pointTempObj, "Count", count1);

            count1++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_BIOS_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 1)
        {

            SMBIOS_TABLE_TYPE1 *Type1Record = (SMBIOS_TABLE_TYPE1 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray2, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "Manufacturer", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "ProductName", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "Version", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 4);
            cJSON_AddStringToObject(pointTempObj, "SerialNumber", str);
            cJSON_AddNumberToObject(pointTempObj, "WakeUpType", Type1Record->WakeUpType);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 5);
            cJSON_AddStringToObject(pointTempObj, "SKUNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 6);
            cJSON_AddStringToObject(pointTempObj, "Family", str);

            cJSON_AddNumberToObject(pointTempObj, "Count", count2);
            count2++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_SYSTEM_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 2)
        {

            SMBIOS_TABLE_TYPE2 *Type2Record = (SMBIOS_TABLE_TYPE2 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray3, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "Manufacturer", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "ProductName", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "Version", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 4);
            cJSON_AddStringToObject(pointTempObj, "SerialNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 5);
            cJSON_AddStringToObject(pointTempObj, "AssetTag", str);
            cJSON_AddNumberToObject(pointTempObj, "BoardType", Type2Record->BoardType);
            cJSON_AddNumberToObject(pointTempObj, "Count", count3);
            count3++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_BASEBOARD_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 3)
        {
            SMBIOS_TABLE_TYPE3 *Type3Record = (SMBIOS_TABLE_TYPE3 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray4, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "Manufacturer", str);
            cJSON_AddNumberToObject(pointTempObj, "Type", Type3Record->Type);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "Version", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "SerialNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 4);
            cJSON_AddStringToObject(pointTempObj, "AssetTag", str);

            cJSON_AddNumberToObject(pointTempObj, "BootupState", Type3Record->BootupState);
            cJSON_AddNumberToObject(pointTempObj, "PowerSupplyState", Type3Record->PowerSupplyState);
            cJSON_AddNumberToObject(pointTempObj, "Count", count4);
            count4++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_SYSTEM_ENCLOSURE!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 4)
        {
            SMBIOS_TABLE_TYPE4 *Type4Record = (SMBIOS_TABLE_TYPE4 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray5, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "Socket", str);

            cJSON_AddNumberToObject(pointTempObj, "ProcessorType", Type4Record->ProcessorType);
            cJSON_AddNumberToObject(pointTempObj, "ProcessorFamily", Type4Record->ProcessorFamily);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "ProcessorManufacturer", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "ProcessorVersion", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 4);
            cJSON_AddStringToObject(pointTempObj, "SerialNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 5);
            cJSON_AddStringToObject(pointTempObj, "AssetTag", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 6);
            cJSON_AddStringToObject(pointTempObj, "PartNumber", str);

            cJSON_AddNumberToObject(pointTempObj, "Count", count5);
            count5++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_PROCESSOR_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 7)
        {
            SMBIOS_TABLE_TYPE7 *Type7Record = (SMBIOS_TABLE_TYPE7 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray6, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "SocketDesignation", str);

            cJSON_AddNumberToObject(pointTempObj, "CacheConfiguration", Type7Record->CacheConfiguration);
            cJSON_AddNumberToObject(pointTempObj, "CacheSpeed", Type7Record->CacheSpeed);
            cJSON_AddNumberToObject(pointTempObj, "MaximumCacheSize", Type7Record->MaximumCacheSize);
            cJSON_AddNumberToObject(pointTempObj, "Associativity", Type7Record->Associativity);
            cJSON_AddNumberToObject(pointTempObj, "Count", count6);
            count6++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_CACHE_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 8)
        {
            SMBIOS_TABLE_TYPE8 *Type8Record = (SMBIOS_TABLE_TYPE8 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray7, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "InternalReferenceDesignator", str);

            cJSON_AddNumberToObject(pointTempObj, "InternalConnectorType", Type8Record->InternalConnectorType);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "ExternalReferenceDesignator", str);

            cJSON_AddNumberToObject(pointTempObj, "ExternalConnectorType", Type8Record->ExternalConnectorType);
            cJSON_AddNumberToObject(pointTempObj, "PortType", Type8Record->PortType);
            cJSON_AddNumberToObject(pointTempObj, "Count", count7);
            count7++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 9)
        {
            SMBIOS_TABLE_TYPE9 *Type9Record = (SMBIOS_TABLE_TYPE9 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray8, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "SlotDesignation", str);

            cJSON_AddNumberToObject(pointTempObj, "SlotType", Type9Record->SlotType);
            cJSON_AddNumberToObject(pointTempObj, "SlotLength", Type9Record->SlotLength);
            cJSON_AddNumberToObject(pointTempObj, "SlotID", Type9Record->SlotID);
            cJSON_AddNumberToObject(pointTempObj, "Count", count8);
            count8++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_SYSTEM_SLOTS!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 16)
        {
            SMBIOS_TABLE_TYPE16 *Type16Record = (SMBIOS_TABLE_TYPE16 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray9, pointTempObj = cJSON_CreateObject());

            cJSON_AddNumberToObject(pointTempObj, "Location", Type16Record->Location);
            cJSON_AddNumberToObject(pointTempObj, "Use", Type16Record->Use);
            cJSON_AddNumberToObject(pointTempObj, "MaximumCapacity", Type16Record->MaximumCapacity);
            cJSON_AddNumberToObject(pointTempObj, "NumberOfMemoryDevices", Type16Record->NumberOfMemoryDevices);
            cJSON_AddNumberToObject(pointTempObj, "Count", count9);
            count9++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 17)
        {
            SMBIOS_TABLE_TYPE17 *Type17Record = (SMBIOS_TABLE_TYPE17 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray10, pointTempObj = cJSON_CreateObject());

            cJSON_AddNumberToObject(pointTempObj, "TotalWidth", Type17Record->TotalWidth);
            cJSON_AddNumberToObject(pointTempObj, "DataWidth", Type17Record->DataWidth);
            cJSON_AddNumberToObject(pointTempObj, "Size", Type17Record->Size);
            cJSON_AddNumberToObject(pointTempObj, "DeviceSet", Type17Record->DeviceSet);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "DeviceLocator", str);
            cJSON_AddNumberToObject(pointTempObj, "MemoryType", Type17Record->MemoryType);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "Manufacturer", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 4);
            cJSON_AddStringToObject(pointTempObj, "SerialNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 5);
            cJSON_AddStringToObject(pointTempObj, "AssetTag", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 6);
            cJSON_AddStringToObject(pointTempObj, "PartNumber", str);

            cJSON_AddNumberToObject(pointTempObj, "Count", count10);
            count10++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_MEMORY_DEVICE!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }

        else if (SmbiosStruct.Hdr->Type == 21)
        {
            SMBIOS_TABLE_TYPE21 *Type21Record = (SMBIOS_TABLE_TYPE21 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray11, pointTempObj = cJSON_CreateObject());

            cJSON_AddNumberToObject(pointTempObj, "Type", Type21Record->Type);
            cJSON_AddNumberToObject(pointTempObj, "Interface", Type21Record->Interface);
            cJSON_AddNumberToObject(pointTempObj, "NumberOfButtons", Type21Record->NumberOfButtons);
            cJSON_AddNumberToObject(pointTempObj, "Count", count11);
            count11++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_BUILT_IN_POINTING_DEVICE!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }

        else if (SmbiosStruct.Hdr->Type == 22)
        {

            SMBIOS_TABLE_TYPE22 *Type22Record = (SMBIOS_TABLE_TYPE22 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray12, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "Location", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "Manufacturer", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "ManufactureDate", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 4);
            cJSON_AddStringToObject(pointTempObj, "SerialNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 5);
            cJSON_AddStringToObject(pointTempObj, "DeviceName", str);

            cJSON_AddNumberToObject(pointTempObj, "Count", count12);
            count12++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_PORTABLE_BATTERY!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 27)
        {

            SMBIOS_TABLE_TYPE27 *Type27Record = (SMBIOS_TABLE_TYPE27 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray13, pointTempObj = cJSON_CreateObject());

            cJSON_AddNumberToObject(pointTempObj, "CoolingDevice", Type27Record->DeviceTypeAndStatus.CoolingDevice);
            cJSON_AddNumberToObject(pointTempObj, "CoolingDeviceStatus", Type27Record->DeviceTypeAndStatus.CoolingDeviceStatus);
            cJSON_AddNumberToObject(pointTempObj, "CoolingUnitGroup", Type27Record->CoolingUnitGroup);
            cJSON_AddNumberToObject(pointTempObj, "OEMDefined", Type27Record->OEMDefined);
            cJSON_AddNumberToObject(pointTempObj, "Count", count13);
            count13++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_COOLING_DEVICE!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 30)
        {

            SMBIOS_TABLE_TYPE30 *Type30Record = (SMBIOS_TABLE_TYPE30 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray14, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "ManufacturerName", str);

            cJSON_AddNumberToObject(pointTempObj, "Connections", Type30Record->Connections);
            cJSON_AddNumberToObject(pointTempObj, "Count", count14);
            count14++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_OUT_OF_BAND_REMOTE_ACCESS!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 34)
        {

            SMBIOS_TABLE_TYPE34 *Type34Record = (SMBIOS_TABLE_TYPE34 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray15, pointTempObj = cJSON_CreateObject());

            cJSON_AddNumberToObject(pointTempObj, "Type", Type34Record->Type);
            cJSON_AddNumberToObject(pointTempObj, "Address", Type34Record->Address);
            cJSON_AddNumberToObject(pointTempObj, "AddressType", Type34Record->AddressType);
            cJSON_AddNumberToObject(pointTempObj, "Count", count15);
            count15++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_MANAGEMENT_DEVICE!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 38)
        {

            SMBIOS_TABLE_TYPE38 *Type38Record = (SMBIOS_TABLE_TYPE38 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray16, pointTempObj = cJSON_CreateObject());

            cJSON_AddNumberToObject(pointTempObj, "InterfaceType", Type38Record->InterfaceType);
            cJSON_AddNumberToObject(pointTempObj, "IPMISpecificationRevision", Type38Record->IPMISpecificationRevision);
            cJSON_AddNumberToObject(pointTempObj, "I2CSlaveAddress", Type38Record->I2CSlaveAddress);
            cJSON_AddNumberToObject(pointTempObj, "NVStorageDeviceAddress", Type38Record->NVStorageDeviceAddress);
            cJSON_AddNumberToObject(pointTempObj, "BaseAddress", Type38Record->BaseAddress);
            cJSON_AddNumberToObject(pointTempObj, "Count", count16);
            count16++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_IPMI_DEVICE_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 39)
        {

            SMBIOS_TABLE_TYPE39 *Type39Record = (SMBIOS_TABLE_TYPE39 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray17, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "DeviceName", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 2);
            cJSON_AddStringToObject(pointTempObj, "Manufacturer", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 3);
            cJSON_AddStringToObject(pointTempObj, "SerialNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 4);
            cJSON_AddStringToObject(pointTempObj, "AssetTagNumber", str);
            str = hcj_LibGetSmbiosString(&SmbiosStruct, 5);
            cJSON_AddStringToObject(pointTempObj, "ModelPartNumber", str);

            cJSON_AddNumberToObject(pointTempObj, "Count", count17);
            count17++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_SYSTEM_POWER_SUPPLY!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 41)
        {

            SMBIOS_TABLE_TYPE41 *Type41Record = (SMBIOS_TABLE_TYPE41 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray18, pointTempObj = cJSON_CreateObject());

            str = hcj_LibGetSmbiosString(&SmbiosStruct, 1);
            cJSON_AddStringToObject(pointTempObj, "ReferenceDesignation", str);

            cJSON_AddNumberToObject(pointTempObj, "DeviceType", Type41Record->DeviceType);
            cJSON_AddNumberToObject(pointTempObj, "DeviceTypeInstance", Type41Record->DeviceTypeInstance);
            cJSON_AddNumberToObject(pointTempObj, "SegmentGroupNum", Type41Record->SegmentGroupNum);
            cJSON_AddNumberToObject(pointTempObj, "BusNum", Type41Record->BusNum);
            cJSON_AddNumberToObject(pointTempObj, "DevFuncNum", Type41Record->DevFuncNum);
            cJSON_AddNumberToObject(pointTempObj, "Count", count18);
            count18++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_ONBOARD_DEVICES_EXTENDED_INFORMATION!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
        else if (SmbiosStruct.Hdr->Type == 19)
        {

            SMBIOS_TABLE_TYPE19 *Type19Record = (SMBIOS_TABLE_TYPE19 *)SmbiosStruct.Hdr;
            cJSON_AddItemToArray(subArray19, pointTempObj = cJSON_CreateObject());

            cJSON_AddNumberToObject(pointTempObj, "DevFuncNum", Type19Record->StartingAddress);
            cJSON_AddNumberToObject(pointTempObj, "DevFuncNum", Type19Record->EndingAddress);
            cJSON_AddNumberToObject(pointTempObj, "DevFuncNum", Type19Record->PartitionWidth);

            cJSON_AddNumberToObject(pointTempObj, "Count", count19);
            count19++;
            Textbuf = (CHAR16 *)L"FIND SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS!.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
        }
    }

    cJSON_AddItemToObject(obj1, "physicalInfo", subArray1);
    //其他信息
    // cJSON_AddItemToObject(obj1, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj1, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj2, "physicalInfo", subArray2);
    // cJSON_AddItemToObject(obj2, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj2, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj3, "physicalInfo", subArray3);
    // cJSON_AddItemToObject(obj3, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj3, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj4, "physicalInfo", subArray4);
    // cJSON_AddItemToObject(obj4, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj4, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj5, "physicalInfo", subArray5);
    // cJSON_AddItemToObject(obj5, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj5, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj6, "physicalInfo", subArray6);
    // cJSON_AddItemToObject(obj6, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj6, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj7, "physicalInfo", subArray7);
    // cJSON_AddItemToObject(obj7, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj7, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj8, "physicalInfo", subArray8);
    // cJSON_AddItemToObject(obj8, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj8, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj9, "physicalInfo", subArray9);
    // cJSON_AddItemToObject(obj9, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj9, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj10, "physicalInfo", subArray10);
    // cJSON_AddItemToObject(obj10, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj10, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj11, "physicalInfo", subArray11);
    // cJSON_AddItemToObject(obj11, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj11, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj12, "physicalInfo", subArray12);
    // cJSON_AddItemToObject(obj12, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj12, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj13, "physicalInfo", subArray13);
    // cJSON_AddItemToObject(obj13, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj13, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj14, "physicalInfo", subArray14);
    // cJSON_AddItemToObject(obj14, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj14, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj15, "physicalInfo", subArray15);
    // cJSON_AddItemToObject(obj15, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj15, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj16, "physicalInfo", subArray16);
    // cJSON_AddItemToObject(obj16, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj16, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj17, "physicalInfo", subArray17);
    // cJSON_AddItemToObject(obj17, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj17, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj18, "physicalInfo", subArray18);
    // cJSON_AddItemToObject(obj18, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj18, "status", cJSON_CreateString("ok"));

    cJSON_AddItemToObject(obj19, "physicalInfo", subArray19);
    // cJSON_AddItemToObject(obj19, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    // cJSON_AddItemToObject(obj19, "status", cJSON_CreateString("ok"));

    Textbuf = (CHAR16 *)L"COLLETCING SMBIOS INFO END.\r\n";
    BufSize = StrLen(Textbuf) * 2;
    FileHandle->Write(FileHandle, &BufSize, Textbuf);
    return status;
}



EFI_STATUS
IntiateFileRoot(EFI_FILE_PROTOCOL **Root){


    EFI_STATUS Status = EFI_SUCCESS;
        //返回的接口
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;

    UINT16 TextHeader = 0xFEFF;
    //文件读写的handle
    
    Status = gBS->LocateProtocol(
        &gEfiSimpleFileSystemProtocolGuid,
        NULL,
        (VOID **)&SimpleFileSystem);

    if (EFI_ERROR(Status))
    {
         Print(L"Cannot find EFI_SIMPLE_FILE_SYSTEM_PROTOCOL \r\n");

        return Status;
    }

    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, Root);
    if (EFI_ERROR(Status))
    {
        Print(L"OpenVolume error \r\n");

        return Status;
    }


}

int get_info(cJSON *json, cJSON *Cold_Hardware)
{
    EFI_STATUS Status = EFI_SUCCESS;
    //文件读写的protocol 
    //ROOT目录
    EFI_FILE_PROTOCOL *Root;
    //log文件handle
    EFI_FILE_PROTOCOL *FileHandle = 0;
    UINTN BufSize;
    CHAR16 *Textbuf = (CHAR16 *)L"log:\r\n";

    //初始化根文件目录
    IntiateFileRoot(&Root);

    //在根目录下创建新文件，如果文件已经存在，则打开
    Status = Root->Open(Root,                      //根目录
                        &FileHandle,
                        (CHAR16 *)L"InfoLog.txt", //相对路径，此时处于*This指针指向的目录下，此时为根目录
                        EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                        0);

    if (EFI_ERROR(Status) || (FileHandle == 0))
    {
        Print(L"Open error \r\n");
        return Status;
    }

    //TXT文件中一位字符的大小为两个UINT64 
    //position  =  0xffffffffffffffff 使文件定位到末尾
    UINT64 position  =  0xffffffffffffffff;
    Status = FileHandle->SetPosition(FileHandle, position);
    if(Status != EFI_SUCCESS){
        printf("failed!\n");
        return Status;
    }
        Textbuf = (CHAR16 *)L"============================================NEW INFO=======================================\r\n";
        BufSize = StrLen(Textbuf) * 2;
        FileHandle->Write(FileHandle, &BufSize, Textbuf);

    //写json
    //先创建json空对象/根对象
    //cJSON *json = cJSON_CreateObject();
    
    //向对象中增加用户名
    // cJSON_AddItemToObject(json, "name", cJSON_CreateString("xxxxxxxxx"));
    //添加非热插拔数组
    cJSON_AddItemToObject(json, "NHSwapHardware", Cold_Hardware);


    SmbiosInfo(Cold_Hardware, FileHandle);

    // locate pcirootbridge
    LocatePciRootBridgeIo(FileHandle);

    //ListPciInformation
    cJSON *pci_array = NULL;
    cJSON_AddItemToObject(json, "PCIHardware", pci_array = cJSON_CreateArray());
    ListPciInformation(pci_array, FileHandle);

    //ListUSBInformation
    GetUSB(json, FileHandle);

    //ListDevicePath
    GetDevicePath(json);


    // //print json
    // char *buf = cJSON_Print(json);
    // printf("%s\n", buf);

    // //free space
    // free(buf);
    // cJSON_Delete(json);
    // FileHandle -> Close(FileHandle);
    // return Status;
    //return json;


    return Status;

}

EFI_STATUS LocatePciRootBridgeIo(EFI_FILE_PROTOCOL *FileHandle)
{
    EFI_STATUS Status;
    EFI_HANDLE *PciHandleBuffer = NULL;
    UINTN HandleIndex = 0;
    UINTN HandleCount = 0;

    CHAR16* Textbuf ;
    UINTN BufSize ;

    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiPciRootBridgeIoProtocolGuid,
        NULL,
        &HandleCount,
        &PciHandleBuffer //二级指针
    );
    if (EFI_ERROR(Status))
        return Status;

    for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
    {
        Status = gBS->HandleProtocol(
            PciHandleBuffer[HandleIndex],
            &gEfiPciRootBridgeIoProtocolGuid,
            (VOID **)&gPciRootBridgeIo);
        if (EFI_ERROR(Status))
            continue;
        else
            return EFI_SUCCESS;
    }
        if (EFI_ERROR(Status))
    {
        Textbuf = (CHAR16 *)L"Call LocatePciRootBridgeIo failed,Can't find protocol!\r\n";
        BufSize = StrLen(Textbuf) * 2;
        FileHandle->Write(FileHandle, &BufSize, Textbuf);
    }
    else
    {
        Textbuf = (CHAR16 *)L"Call LocatePciRootBridgeIo successed,Find protocol!\r\n";
        BufSize = StrLen(Textbuf) * 2;
        FileHandle->Write(FileHandle, &BufSize, Textbuf);
    }
    return Status;
}

EFI_STATUS ListPciInformation(cJSON *array, EFI_FILE_PROTOCOL *FileHandle)
{
    EFI_STATUS Status = EFI_SUCCESS;
    PCI_TYPE00 Pci;
    UINT16 Dev, Func, Bus, PciDevicecount = 0;

    CHAR16* Textbuf = (CHAR16 *)L"Find Pci Info Start..\r\n";
    UINTN BufSize = StrLen(Textbuf) * 2;
    FileHandle->Write(FileHandle, &BufSize, Textbuf);

    //先创建空对象
    cJSON *pciObj = NULL;
    //将对象加到数组中
    cJSON_AddItemToArray(array, pciObj = cJSON_CreateObject());
    //在对象上添加键值对
    cJSON_AddItemToObject(pciObj, "Type", cJSON_CreateString("PCI"));
    cJSON_AddItemToObject(pciObj, "SubType", cJSON_CreateString("PCI"));
    //在对象中再添加一个数组
    cJSON *PciSubArray = NULL;
    cJSON_AddItemToObject(pciObj, "physicalInfo", PciSubArray = cJSON_CreateArray());
    //临时对象
    cJSON *pointTempObj;

    for (Bus = 0; Bus < 64; Bus++)
        for (Dev = 0; Dev <= PCI_MAX_DEVICE; Dev++)
            for (Func = 0; Func <= PCI_MAX_FUNC; Func++)
            {
                Status = PciDevicePresent(gPciRootBridgeIo, &Pci, (UINT8)Bus, (UINT8)Dev, (UINT8)Func);
                if (Status == EFI_SUCCESS)
                {
                    PciDevicecount++;
 
                    cJSON_AddItemToArray(PciSubArray, pointTempObj = cJSON_CreateObject());
                    cJSON_AddNumberToObject(pointTempObj, "VendorID", Pci.Hdr.VendorId);
                    cJSON_AddNumberToObject(pointTempObj, "DeviceId", Pci.Hdr.DeviceId);
                    Textbuf = (CHAR16 *)L"FOUND ONE PCI DEVICE!\r\n";
                    BufSize = StrLen(Textbuf) * 2;
                    FileHandle->Write(FileHandle, &BufSize, Textbuf);
                }
            }

    cJSON_AddItemToObject(pciObj, "physicalInfoHash", cJSON_CreateString("1234567812345678923456789023456789"));
    cJSON_AddItemToObject(pciObj, "status", cJSON_CreateString("ok"));
    Textbuf = (CHAR16 *)L"Find Pci Info End..\r\n";
    BufSize = StrLen(Textbuf) * 2;
    Status = FileHandle->Write(FileHandle, &BufSize, Textbuf);
    return EFI_SUCCESS;
}

EFI_STATUS
PciDevicePresent(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo,
    OUT PCI_TYPE00 *Pci,
    IN UINT8 Bus,
    IN UINT8 Device,
    IN UINT8 Func)
{
    UINT64 Address;
    EFI_STATUS Status;

    //
    // Create PCI address map in terms of Bus, Device and Func
    //
    Address = EFI_PCI_ADDRESS(Bus, Device, Func, 0);

    //
    // Read the Vendor ID register
    //
    Status = PciRootBridgeIo->Pci.Read(
        PciRootBridgeIo,
        EfiPciWidthUint32,
        Address,
        1,
        Pci);

    if (!EFI_ERROR(Status) && (Pci->Hdr).VendorId != 0xffff)
    {
        //
        // Read the entire config header for the device
        //
        Status = PciRootBridgeIo->Pci.Read(
            PciRootBridgeIo,
            EfiPciWidthUint32,
            Address,
            sizeof(PCI_TYPE00) / sizeof(UINT32),
            Pci);

        return EFI_SUCCESS;
    }

    return EFI_NOT_FOUND;
}

UINTN GetUSB(cJSON *json, EFI_FILE_PROTOCOL *FileHandle)
{
    EFI_GUID gEfiUsbIoProtocolGuid =
     {0x2B2F68D6, 0x0CD2, 0x44CF, {0x8E, 0x8B, 0xBB, 0xA2, 0x0B, 0x1B, 0x5B, 0x75}};
    EFI_STATUS Status;
    UINTN HandleIndex, HandleCount;
    EFI_HANDLE *DevicePathHandleBuffer = NULL;
    EFI_USB_IO_PROTOCOL *USBIO;
    EFI_USB_DEVICE_DESCRIPTOR DeviceDescriptor;
    CHAR16 *Textbuf = (CHAR16 *)L"Find Usb Info Start..\r\n";
    UINTN BufSize = StrLen(Textbuf) * 2;
    FileHandle->Write(FileHandle, &BufSize, Textbuf);
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiUsbIoProtocolGuid,
        NULL,
        &HandleCount,
        &DevicePathHandleBuffer);

    if (EFI_ERROR(Status))
    {
        Textbuf = (CHAR16 *)L"Get USBIO count fail.\r\n";
        BufSize = StrLen(Textbuf) * 2;
        FileHandle->Write(FileHandle, &BufSize, Textbuf);
        // Print(L"ERROR : Get USBIO count fail.\n");
        return 0;
    }
    //添加一个usb对象
    cJSON *usbObj = NULL;
    cJSON_AddItemToObject(json, "HSwapHardware", usbObj = cJSON_CreateObject());

    cJSON_AddItemToObject(usbObj, "Type", cJSON_CreateString("USB"));
    cJSON_AddItemToObject(usbObj, "SubType", cJSON_CreateString("USB"));

    //在对象中再添加一个数组
    cJSON *USBsubArray = NULL;
    cJSON_AddItemToObject(usbObj, "physicalInfo", USBsubArray = cJSON_CreateArray());

    //临时对象
    cJSON *pointTempObj;

    for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
    {
        Status = gBS->HandleProtocol(
            DevicePathHandleBuffer[HandleIndex],
            &gEfiUsbIoProtocolGuid,
            (VOID **)&USBIO);

        if (EFI_ERROR(Status))
        {
            Textbuf = (CHAR16 *)L"ERROR : Open USBIO fail.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
            // Print(L"ERROR : Open USBIO fail.\n");
            gBS->FreePool(DevicePathHandleBuffer);
            return 0;
        }

        Status = USBIO->UsbGetDeviceDescriptor(USBIO, &DeviceDescriptor);
        if (EFI_ERROR(Status))
        {
            Textbuf = (CHAR16 *)L"ERROR : Usb Get Device Descriptor fail.\r\n";
            BufSize = StrLen(Textbuf) * 2;
            FileHandle->Write(FileHandle, &BufSize, Textbuf);
            // Print(L"ERROR : Usb Get Device Descriptor fail.\n");
            gBS->FreePool(DevicePathHandleBuffer);
            return 0;
        }

        //每次遇到usb设备时添加键值对
        cJSON_AddItemToArray(USBsubArray, pointTempObj = cJSON_CreateObject());
        cJSON_AddNumberToObject(pointTempObj, "VendorID", 12312311123);
        cJSON_AddNumberToObject(pointTempObj, "DeviceId", 1242154315);
        Textbuf = (CHAR16 *)L"FOUND ONE USB DEVICE!\r\n";
        BufSize = StrLen(Textbuf) * 2;
        Status = FileHandle->Write(FileHandle, &BufSize, Textbuf);
    }
    //清理工作

    Textbuf = (CHAR16 *)L"Find Usb Info End..\r\n";
    BufSize = StrLen(Textbuf) * 2;
    FileHandle->Write(FileHandle, &BufSize, Textbuf);
    gBS->FreePool(DevicePathHandleBuffer);
    return HandleCount;
}


EFI_STATUS 
GetDevicePath(    cJSON *json    )
{ 
      EFI_HANDLE *HandleBuffer;
  UINTN HandleCount;
  EFI_STATUS Status;
  UINTN HandleIndex;

  EFI_DEVICE_PATH *DevicePath;
  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *pDevicePath2TextProtocol;
  CHAR16 *pStrDevicePath;





  HandleIndex = 0;

  Status = gBS->LocateHandleBuffer(
      ByProtocol,
      &gEfiDevicePathProtocolGuid,
      NULL,
      &HandleCount,
      &HandleBuffer);


    //添加一个DevPath对象
    cJSON *DevPathObj = NULL;
    cJSON_AddItemToObject(json, "NHSwapHardware", DevPathObj = cJSON_CreateObject());

    cJSON_AddItemToObject(DevPathObj, "Type", cJSON_CreateString("DevPath"));
    cJSON_AddItemToObject(DevPathObj, "SubType", cJSON_CreateString("DevPath"));

    //在对象中再添加一个数组
    cJSON *DevPathsubArray = NULL;
    cJSON_AddItemToObject(DevPathObj, "physicalInfo", DevPathsubArray = cJSON_CreateArray());

    //临时对象
    cJSON *pointTempObj;

  if (!EFI_ERROR(Status))
  {
    for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
    {


      cJSON_AddItemToArray(DevPathsubArray, pointTempObj = cJSON_CreateObject());

      Status = gBS->HandleProtocol(HandleBuffer[HandleIndex], &gEfiDevicePathProtocolGuid, (VOID **)&DevicePath);


      Status = gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (VOID **)&pDevicePath2TextProtocol);
      pStrDevicePath = pDevicePath2TextProtocol->ConvertDevicePathToText(DevicePath, TRUE, TRUE);

      UINTN i = StrLen(pStrDevicePath);

     CHAR16 array[100] = {0};

    for(UINTN j = 0; j < i; j++){

      strcat(array,pStrDevicePath++);
      
    }
      
      cJSON_AddStringToObject(pointTempObj,  "path" , array);
      //传过来是%s
      //传过来的pStrDevicePath只能用Print%s输出
      Print(L"%s\n", pStrDevicePath);
      //自己定义的CHAR16* 只能用Print%a输出  自己定义就是ascii？
      //Print(L"%a\n", TEMPSTR);
      //FreePool(TEMPstr);
    }
  }
  return EFI_SUCCESS;


}
