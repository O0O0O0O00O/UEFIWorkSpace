#include <Uefi.h>  
#include <Library/UefiLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/UefiShellDebug1CommandsLib.h>
#include <Library/LibSmbiosView.h> 

 
EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{ 
  EFI_STATUS                    Status;
  SMBIOS_TABLE_ENTRY_POINT      *mSmbiosTable   = NULL;
  SMBIOS_STRUCTURE_POINTER      m_SmbiosStruct;
  SMBIOS_STRUCTURE_POINTER      *mSmbiosStruct = &m_SmbiosStruct;
  
  SMBIOS_STRUCTURE_POINTER  Smbios;
  SMBIOS_STRUCTURE_POINTER  SmbiosEnd;
  UINT8                     *Raw;

  UINT16                    Handle1 = 0;
  UINT8                     *Buffer1;
  UINT16                    Length1;
  
  
  UINT16                    *Handle;
  UINT8                     **Buffer;
  UINT16                    *Length;

  UINT8                     Type;
  
 
  mSmbiosTable = NULL;
 
  //
  // Get SMBIOS table from System Configure table
  //
  Status = GetSystemConfigurationTable(&gEfiSmbiosTableGuid,(VOID**)&mSmbiosTable);
 
  if (mSmbiosTable == NULL)
  {
    Print(L"%r.\n",Status);
  }
  //
  // Init SMBIOS structure table address
  //
  mSmbiosStruct->Raw  = (UINT8 *)(UINTN)(mSmbiosTable->TableAddress);
  
   //
  //Find the structure
  //
  Handle = &Handle1;
  Length = &Length1;
  Buffer = &Buffer1;
  
  *Length       = 0;
  Smbios.Hdr    = mSmbiosStruct->Hdr;
  SmbiosEnd.Raw = Smbios.Raw + mSmbiosTable->TableLength;
  Print(L"TableLenth:%02d\n",mSmbiosTable->TableLength);
  
  while (Smbios.Raw < SmbiosEnd.Raw)
  {
    if (Smbios.Hdr->Handle == *Handle)
	{
      Raw = Smbios.Raw;
	  Type = Smbios.Hdr->Type;
      //
      // Walk to next structure
      //
      LibGetSmbiosString(&Smbios,(UINT16)(-1));
      //
      // Length = Next structure head - this structure head
      //
      *Length = (UINT16)(Smbios.Raw - Raw);
      *Buffer = Raw;
      //
      // update with the next structure handle.
      //
      if (Smbios.Raw < SmbiosEnd.Raw)
      {
        *Handle = Smbios.Hdr->Handle;
      } else
       {
        *Handle = (UINT16)(-1);
      }
      Print(L"Handle:%04x Type:%04d Address:%08x Length:%04x\n", *Handle - 1, Type, *Buffer, *Length);
	  DumpHex(2, 0, *Length, *Buffer);
      //return DMI_SUCCESS;
    }
  }
  *Handle = (UINT16)(-1);
  return EFI_SUCCESS;
}
