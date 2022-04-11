#include "hcj_network.h"


// EFI_STATUS tcp4Client(IN int Argc,
//                       IN char **Argv);

// /***
//  @param[in]  Argc    Number of argument tokens pointed to by Argv.
//   @param[in]  Argv    Array of Argc pointers to command line tokens.

//   @retval  0         The application exited normally.
//   @retval  Other     An error occurred.
// ***/
// int
// main (
//   IN int Argc,
//   IN char **Argv
//   )
// {
// 	// UINT64 flag;
	
// 	// flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | PCI_ROOTBRIDGE_IO | PCI_IO | FILE_IO | RNG_OUT);
// 	// Print(L"flag=%x\n",flag);
// 	// // WaitKey();
// 	// //text out test

// 	// //Testnetwork
//   tcp4Client(Argc, Argv);
//   return 0;
// }

// EFI_STATUS tcp4Client(IN int Argc, IN char **Argv){
//     EFI_STATUS Status = EFI_SUCCESS;
//     UINT32 ServerIPAddr[4];
//     UINT16 ServerPort;
//     struct sockaddr_in ServerIp;
//     int socketID, recvLen, RetVal;
//     char msgStr[1024], recvStr[1024];

//     //1. 获取服务器端ip和端口
//     if(Argc != 3){
//         Print(L"UEFI TCP4 Client Usage: %a <ServerIP> <port>\n", Argv[0]);
//         Status = EFI_ABORTED;
//         return Status;
//     }else{
//         sscanf(Argv[1], "%d.%d.%d.%d", &ServerIPAddr[0], &ServerIPAddr[1], &ServerIPAddr[2], &ServerIPAddr[3]);
//         sscanf(Argv[2], "%d", &ServerPort);
//     }
//     // 2.创建Socket
//     socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//     if(socketID == -1){
//         Status = EFI_ABORTED;
//         printf("Can't create socket, quit the app!\n");
//     }else{
//         memset(&ServerIp, 0, sizeof(ServerIp));
//         ServerIp.sin_len = sizeof(ServerIp);
//         ServerIp.sin_family = AF_INET;
//         ServerIp.sin_addr.s_addr = (ServerIPAddr[3] << 24)
//                                     |(ServerIPAddr[2] << 16)
//                                     |(ServerIPAddr[1] << 8)
//                                     |ServerIPAddr[0];
//         ServerIp.sin_port = htons(ServerPort);
//     }
//     //3. 进行TCP连接
//     RetVal = connect(socketID, (struct sockaddr*)&ServerIp, sizeof(ServerIp));
//     if(-1 == RetVal){
//         Status = EFI_ABORTED;
//         printf("Connect() error, quit the app!\n");
//         return Status;
//     }
    
//     while(1){
//         //4. 发送TCP数据到服务器端
//         memset(msgStr, 0, 1024);
//         printf("please input message");
//         gets(msgStr);
//         if(!strcmp(msgStr, "q") || !strcmp(msgStr, "Q"))
//             break;
//         send(socketID, msgStr, strlen(msgStr), 0);
//         //5. 接收服务器端的数据
//         recvLen = (int)recv(socketID, recvStr, sizeof(recvStr), 0);
//         recvStr[recvLen] = '\0';
//         printf("Message from server: %s\n", recvStr);
//     }
    
//     close(socketID);
//     return Status;
    

// }




EFI_STATUS SendMessage(IN char* message, IN int mes_length){
    EFI_STATUS Status = EFI_SUCCESS;
    
    struct sockaddr_in ServerIp;
    int socketID, recvLen, RetVal;
    char msgStr[1024], recvStr[1024];


    printf("sendmessage:%s\n", message);
    // 2.创建Socket
    printf("begin create socket\n");
    socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketID == -1){
        Status = EFI_ABORTED;
        printf("Can't create socket, quit the app!\n");
        return Status;
    }
    printf("create socket success\n");
    memset(&ServerIp, 0, sizeof(ServerIp));
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_addr.s_addr = inet_addr("121.248.49.236");
    ServerIp.sin_port = htons(8887);
    
    //3. 进行TCP连接
    printf("begin connect\n");
    RetVal = connect(socketID, (struct sockaddr*)&ServerIp, sizeof(ServerIp));
    if(0 != RetVal){
        // Status = EFI_ABORTED;
        printf("Connect() error, quit the app!\n");
        return Status;
    }else{
        printf("Connect() success\n");
    }
    
    
    //4. 发送TCP数据到服务器端
    printf("begin sendmessage\n");
    RetVal = send(socketID, message, mes_length, 0);
    if(-1 == RetVal){
        printf("sendmessage error\n");
        return EFI_SUCCESS;
    }else{
        printf("Send success\n");
    }
    // 5. 接收服务器端的数据
    recvLen = (int)recv(socketID, recvStr, sizeof(recvStr), 0);
    recvStr[recvLen] = '\0';
    printf("Message from server: %s\n", recvStr);
    close(socketID);
    return Status;
}





