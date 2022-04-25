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


cJSON* get_info();