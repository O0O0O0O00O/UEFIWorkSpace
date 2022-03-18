#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathUtilities.h>
#include <string.h>


#define MAX_COMMAND_LINE 10

void hcj_pause(){
    for(int i = 0; i < 5; ++i)
        gBS->Stall(1000000);
}

void hcj_assert(EFI_STATUS Status, CHAR16 *String){
    gST->ConOut->OutputString(gST->ConOut, String);
    if(EFI_ERROR (Status)){
        gST->ConOut->OutputString(gST->ConOut, L" failed\r\n");
    }else{
        gST->ConOut->OutputString(gST->ConOut, L" successed\r\n");
    }
}

void hcj_putc(unsigned short c){
    unsigned short str[2];
    str[0] = c;
    str[1] = '\0';
    gST->ConOut->OutputString(gST->ConOut, str);
}

void hcj_puts(CHAR16 *String){
    gST->ConOut->OutputString(gST->ConOut, String);
}

unsigned short hcj_getc(void){
    EFI_INPUT_KEY key;
    unsigned long long waitidx;
    gST->BootServices->WaitForEvent(1, &(gST->ConIn->WaitForKey), &waitidx);
    while(gST->ConIn->ReadKeyStroke(gST->ConIn, &key))
        ;
    return key.UnicodeChar;
}

unsigned int hcj_gets(char *buf, unsigned int buf_size){
    unsigned int i;
    for(i = 0; i < buf_size-1; ++i){
        buf[i] = hcj_getc();
        if(buf[i] == 0x8){
            if(i == 0){
                i -= 1;
                continue;
            }else{
                hcj_putc(buf[i]);
                i -= 2;
            }
        }else if(buf[i] == L'\r'){
            hcj_putc(L'\r');
            hcj_putc(L'\n');
            break;
        }else{
            hcj_putc(buf[i]);
        }
    }
    buf[i] = L'\0';
    return i;

}

void efi_init(EFI_SYSTEM_TABLE *SystemTable){
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
}


void MyNotifyFunction(IN EFI_EVENT E, IN void *Context){
    static UINTN time = 0;
    Print(L"MyNotifyFunction wait %d\n", time);
    time++;
    
}

void hcj_judge(EFI_STATUS Status){
    if(Status == EFI_SUCCESS)
        Print(L"EFI_SUCCESS\n");
    if(Status == EFI_NOT_READY)
        Print(L"EFI_NOT_READY\n");
}

// void WaitKey(EFI_EVENT *Event){
//     UINTN index;
    
//     gBS->WaitForEvent(1, gST->ConIn->WaitForKey, &index);

// }


// EFI_STATUS Event_Test(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
//     EFI_EVENT Event;
//     EFI_STATUS Status = EFI_SUCCESS;
//     Status = gBS->CreateEvent(EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_NOTIFY, (EFI_EVENT_NOTIFY)MyNotifyFunction, (VOID*)NULL, &Event);
//     if(EFI_ERROR(Status)){
//         Print(L"CreateEvent ERROR");
//     }else
//         Print(L"CreateEvent Success\n");
//     Status = gBS->SetTimer(Event, TimerPeriodic, 50*1000*1000);
//     Status = gBS->SignalEvent(Event);
//     if(EFI_ERROR(Status))
//         Print(L"SetTimer Error");
//     UINTN index;
//     gBS->WaitForEvent(1, &(Event), &index);
//     Print(L"it's over\n");
//     //WaitKey(&Event)
//     gBS->CloseEvent(Event);

//     return EFI_SUCCESS;
// }

// VOID myEventNoify (
//         IN EFI_EVENT                Event,
//         IN VOID                     *Context
//         )
// {
//     static UINTN times = 0;
//     Print(L"myEventNotif Wait %d\n", times);
//     times ++;
//     if(times >5)
//         gBS->SignalEvent(Event);
// }

VOID myEventNoify (
        IN EFI_EVENT                Event,
        IN VOID                     *Context
        )
{
    static UINTN times = 0;
    EFI_STATUS Status;

    if(times>5) {
        Status = gBS->SignalEvent(Event);
        if (EFI_ERROR(Status)) {
            Print(L"gBS->SignalEvent error %d.\n", Status);
        }
        return;
    } else {
        Print(L"myEventNotify Wait %d\n", times);
        times ++;
    }
}

EFI_STATUS TestNotify()
{
    EFI_STATUS  Status;
    UINTN       index=0;
    EFI_EVENT myEvent;

    Status = gBS->CreateEvent(EVT_NOTIFY_WAIT, TPL_NOTIFY, (EFI_EVENT_NOTIFY)myEventNoify , (VOID*)NULL, &myEvent);
    Status = gBS->WaitForEvent(1, &myEvent, &index);
    Status = gBS->CloseEvent(myEvent);

    if (EFI_ERROR(Status)) {
        return Status;
    }
    return EFI_SUCCESS;
}

void WaitKey()
{
    EFI_STATUS   Status = 0;
    UINTN        Index=0;
    EFI_INPUT_KEY  Key;

    Status = gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &Index);
    if (EFI_ERROR(Status)) {
        Print(L"WaitKey: WaitForEvent Error!\n");
    }
    Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
    if (EFI_ERROR(Status)) {
        Print(L"WaitKey: ReadKeyStroke Error!\n");
    }
}

VOID
myEventNoify30 (
        IN EFI_EVENT                Event,
        IN VOID                     *Context
        )
{
    static UINTN times = 0;
    char *str = (char*)Context;
    Print(L"Context address %p\n", str);
    Print(L"%s\nmyEventNotif signal%d\n", str, times);
    times ++;
}

EFI_STATUS TestEventSingal()
{
    EFI_STATUS  Status;
    EFI_EVENT myEvent;
    Print(L"Test EVT_TIMER | EVT_NOTIFY_SIGNAL");
    // 生成Timer事件，并设置触发函数
    char* str = "Hello! Time Out!";
    Print(L"str address %p\n", str);
    Status = gBS->CreateEvent(EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, (EFI_EVENT_NOTIFY)myEventNoify30, str, &myEvent);
    if (EFI_ERROR(Status)) {
        Print(L"TestEventSignal: CreateEvent error %d!\n", Status);
    }
    // 设置Timer等待时间为10秒，属性为循环等待
    Status = gBS->SetTimer(myEvent,TimerPeriodic , 10 * 1000 * 1000);
    if (EFI_ERROR(Status)) {
        Print(L"TestEventSignal: SetTimer error %d!\n", Status);
    }
    WaitKey();
    Status = gBS->CloseEvent(myEvent);
    if (EFI_ERROR(Status)) {
        Print(L"TestEventSignal: CloseEvent error %d!\n", Status);
    }
    return EFI_SUCCESS;
}





EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
){
    // EFI_STATUS Status = EFI_SUCCESS;
    // Status = Event_Test(ImageHandle, SystemTable);
    TestNotify();
    return EFI_SUCCESS;


}