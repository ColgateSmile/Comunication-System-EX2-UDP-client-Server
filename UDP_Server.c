/* CHANGES FROM UNIX VERSION                                                   */
/*                                                                             */
/* 1.  Changed header files.                                                   */
/* 2.  Added WSAStartUP().                                                     */

// Don't forget to include "wsock32" in the library list.


//Dror's AND Noga UDP time_Server Program V 1.1//


#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */
#include <time.h>
#define ECHOMAX 255     /* Longest string to echo */
#pragma comment(lib,"wsock32")
void DieWithError(char *errorMessage)
{
    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
    exit(1);
}  /* External error handling function */






void main(int argc, char *argv[])
{
    int sock;                        /* Socket */
    struct sockaddr_in ServerAddr; /* Local address */
    struct sockaddr_in ClientAddr; /* Client address */
    struct tm* tm_info;
    char sendBuff[ECHOMAX];
    char recvBuff[ECHOMAX];       /* Buffer for echo string */
    unsigned short ServerPort;     /* Server port */
    int cliLen;                      /* Length of incoming message */
    int recvMsgSize;                 /* Size of received message */
    WSADATA wsaData;                 /* Structure for WinSock setup communication */
    time_t timer;
    char timeBuff[26];




     /* Test for correct number of parameters */
    if (argc < 1)
    {
        fprintf(stderr, "Error: no port provided\n");
        exit(1);
    }




    ServerPort = 1500; /*atoi(argv[1]); /* first arg:  Local port */




    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&ServerAddr, 0, sizeof(ServerAddr));   /* Zero out structure */
    ServerAddr.sin_family = AF_INET;                /* Internet address family */
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    ServerAddr.sin_port = htons(ServerPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr)) < 0)
        DieWithError("bind() failed");

    printf("The Server Is Ready,Waiting For Clients At Port : %d\n",ServerPort);

    while(1) /* Run forever */
    {


        /* Set the size of the in-out parameter */
        cliLen = sizeof(ClientAddr);

        /* Block until receive message from a client */
        recvMsgSize = recvfrom(sock, recvBuff, 255, 0,(struct sockaddr *) &ClientAddr, &cliLen);
        if (recvMsgSize < 0)
        {
                   char snum[5];
                   itoa(recvMsgSize, snum, 10);
                   DieWithError(snum);

        }
        recvBuff[recvMsgSize-1]='\0';

        printf("Handling client.. \n");
        printf("Server Recieved : %s\n",recvBuff);
        timer = time(NULL);
        tm_info = localtime(&timer);
        strftime(timeBuff, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Time : %s\n",timeBuff);



         /* Send received datagram back to the client */
         if (sendto(sock, timeBuff, strlen(timeBuff), 0, (struct sockaddr *) &ClientAddr,
                     sizeof(ClientAddr)) <0)
             DieWithError("sendto() sent a different number of bytes than expected");
         printf("Response sent.. \n");
         printf("Server Is Waiting for NEW Clients's requests. \n");
    }
    /* NOT REACHED */
}
