/* CHANGES FROM UNIX VERSION                                                   */
/*                                                                             */
/* 1.  Changed header files.                                                   */
/* 2.  Added WSAStartUP() and WSACleanUp().                                    */
/* 3.  Used closesocket() instead of close().                                  */

// Don't forget to include "wsock32" in the library list.

#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>
#include <ws2tcpip.h>     /* for exit() */

#define ECHOMAX 255     /* Longest string to echo */

void DieWithError(char *errorMessage)
{
    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
    exit(1);
}  /* External error handling function */

void main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in ServerAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short ServerPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char recvBuff[255];        /* Buffer for echo string */
    WSADATA wsaData;                 /* Structure for WinSock setup communication */
    int sendRes;
    int i;
    char sendBuff[16] = "What's the time?";
    if ((argc < 2))    /* Test for correct number of arguments */
    {
        fprintf(stderr, "Error: no server port provided\n");
        exit(1);
    }
    ServerPort = atoi(argv[1]);
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }

    /* Create a best-effort datagram socket using UDP */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&ServerAddr, 0, sizeof(ServerAddr));    /* Zero out structure */
    ServerAddr.sin_family = AF_INET;                 /* Internet address family */
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  /* Server IP address */

    ServerAddr.sin_port = htons(ServerPort);     /* Server port */

    /* Send the string, including the null terminator, to the server */

    if (sendRes = sendto(sock, sendBuff, strlen(sendBuff), 0, (struct sockaddr *)
               &ServerAddr, sizeof(ServerAddr)) < 0)
               {
                   DieWithError("Error at sendto()");
               }


    /* Recv a response */

    fromSize = sizeof(fromAddr);

    if (recvfrom(sock, recvBuff, 255, 0, (struct sockaddr *) &fromAddr,&fromSize) < 0)
    {
            DieWithError("Error at recvfrom()");
    }
    printf("Client Received : %s\n",recvBuff);
    printf("Closing Connection.");
    closesocket(sock);
    WSACleanup();  /* Cleanup Winsock */

    exit(0);
}
