#include "NetWork.h"


#define MAX_COMMAND_LINE 10


typedef struct MyTCP4Socket{
    EFI_HANDLE m_SocketHandle;
    EFI_TCP4_PROTOCOL* m_pTcp4Protocol;
    EFI_TCP4_CONFIG_DATA* m_pTcp4ConfigData;
    EFI_TCP4_TRANSMIT_DATA* m_TransData;
    EFI_TCP4_RECEIVE_DATA* m_RecvData;
    EFI_TCP4_CONNECTION_TOKEN ConnectToken;
    EFI_TCP4_CLOSE_TOKEN CloseToken;
    EFI_TCP4_IO_TOKEN SendToken, RecvToken;
}MYTCP4SOCKET;


static MYTCP4SOCKET *TCP4SocketFd[32];


void NopNoify(IN EFI_EVENT Event, IN VOID *Context){

}



EFI_STATUS InitTcp4SocketFd(INTN index){
    EFI_STATUS Status;
    MYTCP4SOCKET *CurSocket = TCP4SocketFd[index];

    //1 创建配置数据
    CurSocket->m_pTcp4ConfigData = (EFI_TCP4_CONFIG_DATA*)AllocatePool(sizeof(EFI_TCP4_CONFIG_DATA));
    //2 创建连接事件
    Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, (EFI_EVENT_NOTIFY)NopNoify, (VOID*)&CurSocket->ConnectToken, &CurSocket->ConnectToken.CompletionToken.Event);
    //hcj_assert(Status, L"CreateEvent(Connect)");
    //3 创建传输事件
    Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, (EFI_EVENT_NOTIFY)NopNoify, (VOID *)&CurSocket->SendToken, &CurSocket->SendToken.CompletionToken.Event);
    if(EFI_ERROR(Status)){
        Print(L"Init: Create Send Event fail!\n\r");
        return Status;
    }
    CurSocket->SendToken.CompletionToken.Status = EFI_ABORTED;
    CurSocket->m_TransData = (EFI_TCP4_TRANSMIT_DATA*)AllocatePool(sizeof(EFI_TCP4_TRANSMIT_DATA));

    //4. 创建接受数据
    Status = gBS->CreateEvent(EVT_NOTIFY_WAIT, TPL_CALLBACK, (EFI_EVENT_NOTIFY)NopNoify, (VOID*)&CurSocket->RecvToken, &CurSocket->RecvToken.CompletionToken.Event);
    CurSocket->RecvToken.CompletionToken.Status = EFI_ABORTED;
    CurSocket->m_RecvData = (EFI_TCP4_RECEIVE_DATA*)AllocatePool(sizeof(EFI_TCP4_RECEIVE_DATA));
    if(EFI_ERROR(Status)){
        Print(L"Init: Create Recv Event fail\n\r");
        return Status;
    }
    //5. 创建关闭数据
    CurSocket->CloseToken.CompletionToken.Status = EFI_ABORTED;
    Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, (EFI_EVENT_NOTIFY)NopNoify, (VOID*)&CurSocket->CloseToken, &CurSocket->CloseToken.CompletionToken.Event);
    if(EFI_ERROR(Status)){
        Print(L"Init: Create Close Event fail!\n\r");
        return Status;
    }
    return Status;

}

UINTN CreateTCP4Socket(VOID){
    EFI_STATUS Status;
    EFI_SERVICE_BINDING_PROTOCOL* pTcpServiceBinding;
    MYTCP4SOCKET *CurSocket = NULL;
    INTN i;
    INTN MyFd = -1;
    for(i = 0; i < 32; ++i){
        if(TCP4SocketFd[i] == NULL){
            CurSocket = (MYTCP4SOCKET *)AllocatePool(sizeof(MYTCP4SOCKET));
            TCP4SocketFd[i] = CurSocket;
            MyFd = i;
            break;
        }
    }
    if(CurSocket == NULL)
        return MyFd;
    gBS->SetMem((void*)CurSocket, sizeof(MYTCP4SOCKET), 0);
    CurSocket->m_SocketHandle = NULL;
    Status = gBS->LocateProtocol(&gEfiTcp4ServiceBindingProtocolGuid,
        NULL,
        (void**)&pTcpServiceBinding);
    if(EFI_ERROR(Status)){
        Print(L"Create: LocateProtocol gEfiTcp4ServiceBindingProtocolGuid fail\n\r");
        return Status;
    }
    //创建tcp子节点
    Status = pTcpServiceBinding->CreateChild(pTcpServiceBinding, &CurSocket->m_SocketHandle);
    if(EFI_ERROR(Status)){
        Print(L"Create: CreateChild failed.\n\r");
        return Status;
    }
    Status = gBS->OpenProtocol(CurSocket->m_SocketHandle,
        &gEfiTcp4ProtocolGuid,
        (VOID **)CurSocket->m_pTcp4Protocol,
        gImageHandle,
        CurSocket->m_SocketHandle,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    if(EFI_ERROR(Status)){
        Print(L"Create: OpenProtocol EfiTcp4Protocol fail.\n\r");
        return Status;
    }
    InitTcp4SocketFd(MyFd);
    return MyFd;
}

EFI_STATUS ConfigTCP4Socket(UINTN index, UINT32 Ip32, UINT16 Port){
    EFI_STATUS Status = EFI_NOT_FOUND;
    MYTCP4SOCKET *CurSocket = TCP4SocketFd[index];
    if(CurSocket->m_pTcp4ConfigData == NULL){
        return Status;
    }
    //设置为IPV4
    CurSocket->m_pTcp4ConfigData->TypeOfService = 0;
    //设置最大跳数
    CurSocket->m_pTcp4ConfigData->TimeToLive = 16;
    //设置远端的ip地址
    *(UINTN*)(CurSocket->m_pTcp4ConfigData->AccessPoint.RemoteAddress.Addr) = Ip32;
    //设置远端的prot端口
    CurSocket->m_pTcp4ConfigData->AccessPoint.RemotePort = Port;
    //这是子网掩码
    *(UINT32*)(CurSocket->m_pTcp4ConfigData->AccessPoint.SubnetMask.Addr) = (255 | 255 << 8 | 255 << 16 | 0 << 24);
    //这个一设置，上面的子网掩码默认就失效了
    CurSocket->m_pTcp4ConfigData->AccessPoint.UseDefaultAddress = TRUE;

    /// if UseDefaultAddress is FALSE， config StationAddress 
    //CurSocket->m_pTcp4ConfigData->AccessPoint.StationPort = 61558;
    //CurSocket->m_pTcp4ConfigData->AccessPoint.ActiveFlag = TRUE;
    CurSocket->m_pTcp4ConfigData->ControlOption = NULL;
    Status = CurSocket->m_pTcp4Protocol->Configure(CurSocket->m_pTcp4Protocol, CurSocket->m_pTcp4ConfigData);
    if(EFI_ERROR(Status)){
        Print(L"Config: Configure error");
        return Status;
    }
    return Status;
}


EFI_STATUS SendTCP4Socket(UINTN index, CHAR8 *Data, UINTN Length){
    EFI_STATUS Status = EFI_NOT_FOUND;
    MYTCP4SOCKET *CurSocket = TCP4SocketFd[index];
    UINTN waitIndex = 0;

    if(CurSocket->m_pTcp4Protocol == NULL){
        Print(L"Send: m_Tcp4Protocol is NULL\n\r");
        return Status;
    }
    CurSocket->m_TransData->Push = TRUE;
    CurSocket->m_TransData->Urgent = TRUE;
    CurSocket->m_TransData->DataLength = (UINT32)Length;
    CurSocket->m_TransData->FragmentCount = 1;
    CurSocket->m_TransData->FragmentTable[0].FragmentLength = CurSocket->m_TransData->DataLength;
    CurSocket->m_TransData->FragmentTable[0].FragmentBuffer = Data;
    CurSocket->SendToken.Packet.TxData = CurSocket->m_TransData;
    Status = CurSocket->m_pTcp4Protocol->Transmit(CurSocket->m_pTcp4Protocol, &CurSocket->SendToken);
    if(EFI_ERROR(Status)){
        Print(L"Send: Transmit fail!\n\r");
        return Status;
    }
    Status = gBS->WaitForEvent(1, &(CurSocket->SendToken.CompletionToken.Event), &waitIndex);
    if(EFI_ERROR(Status)){
        Print(L"Send: WaitForEvent fail\n\r");
        return Status;
    }
    return CurSocket->SendToken.CompletionToken.Status;
}


EFI_STATUS RecvTCP4Socket(IN UINTN index, IN CHAR8 *Buffer, IN UINTN Length, OUT UINTN *recvLength){
    EFI_STATUS Status = EFI_NOT_FOUND;
    MYTCP4SOCKET *CurSocket = TCP4SocketFd[index];
    UINTN waitIndex = 0;

    if(CurSocket->m_pTcp4Protocol == NULL)
        return Status;
    CurSocket->m_RecvData->UrgentFlag = TRUE;
    CurSocket->m_RecvData->DataLength = (UINT32)Length;
    CurSocket->m_RecvData->FragmentCount = 1;
    CurSocket->m_RecvData->FragmentTable[0].FragmentLength = CurSocket->m_RecvData->DataLength;
    CurSocket->RecvToken.Packet.RxData = CurSocket->m_RecvData;
    Status = CurSocket->m_pTcp4Protocol->Receive(CurSocket->m_pTcp4Protocol, &CurSocket->RecvToken);
    if(EFI_ERROR(Status)){
        Print(L"Recv: Receive fail!\n\r");
        return Status;
    }
    Status = gBS->WaitForEvent(1, &(CurSocket->RecvToken.CompletionToken.Event), &waitIndex);

    *recvLength = CurSocket->m_RecvData->DataLength;
    if(EFI_ERROR(Status)){
        Print(L"Recv: WaitForEvent fail!\n\r");
        return Status;
    }
    return CurSocket->RecvToken.CompletionToken.Status;
}





EFI_STATUS ConnectTCP4Socket(UINTN index, UINT32 Ip32, UINT16 Port){
    EFI_STATUS Status = EFI_NOT_FOUND;
    MYTCP4SOCKET *CurSocket = TCP4SocketFd[index];
    UINTN waitIndex = 0;
    ConfigTCP4Socket(index, Ip32, Port);

    if(CurSocket->m_pTcp4Protocol == NULL)
        return Status;
    Status = CurSocket->m_pTcp4Protocol->Connect(CurSocket->m_pTcp4Protocol, &CurSocket->ConnectToken);
    if(EFI_ERROR(Status)){
        Print(L"Connect: connect fail \n\r");
        return Status;
    }

    Status = gBS->WaitForEvent(1, &(CurSocket->ConnectToken.CompletionToken.Event), &waitIndex);

    if(EFI_ERROR(Status)){
        Print(L"Connect:WaitForEvent fail. \n\r");
        return Status;
    }
    return Status;
}


INTN DestroyTCP4Socket(UINTN index){
    EFI_STATUS Status = EFI_SUCCESS;
    MYTCP4SOCKET *CurSocket = TCP4SocketFd[index];
    
    if(CurSocket->m_SocketHandle){
        EFI_SERVICE_BINDING_PROTOCOL *pTcpServiceBinding;
        Status = gBS->LocateProtocol(&gEfiTcp4ServiceBindingProtocolGuid, NULL, (void**)&pTcpServiceBinding);
        Status = pTcpServiceBinding->DestroyChild(pTcpServiceBinding, CurSocket->m_SocketHandle);
    }
    if(CurSocket->ConnectToken.CompletionToken.Event){
        gBS->CloseEvent(CurSocket->ConnectToken.CompletionToken.Event);
    }
    if(CurSocket->SendToken.CompletionToken.Event){
        gBS->CloseEvent(CurSocket->ConnectToken.CompletionToken.Event);
    }
    if(CurSocket->RecvToken.CompletionToken.Event){
        gBS->CloseEvent(CurSocket->RecvToken.CompletionToken.Event);
    }
    if(CurSocket->m_pTcp4ConfigData){
        FreePool(CurSocket->m_pTcp4ConfigData);
        CurSocket->m_pTcp4ConfigData = NULL;
    }
    if(CurSocket->SendToken.Packet.TxData){
        FreePool(CurSocket->SendToken.Packet.TxData);
        CurSocket->SendToken.Packet.TxData = NULL;
    }
    if(CurSocket->RecvToken.Packet.RxData){
        FreePool(CurSocket->RecvToken.Packet.RxData);
        CurSocket->RecvToken.Packet.RxData = NULL;
    }
    return Status;
}


EFI_STATUS CloseTCP4Socket(UINTN index){
    EFI_STATUS Status;
    MYTCP4SOCKET *CurSocket = TCP4SocketFd[index];
    Status = CurSocket->m_pTcp4Protocol->Close(CurSocket->m_pTcp4Protocol, &CurSocket->CloseToken);

    DestroyTCP4Socket(index);
    FreePool(CurSocket);
    TCP4SocketFd[index] = NULL;
    return Status;
}

