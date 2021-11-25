/* CHANGES FROM UNIX VERSION                                                   */
/*                                                                             */
/* 1.  Changed header files.                                                   */
/* 2.  Added WSAStartUP().                                                     */

/* Don't forget to include "wsock32" in the library list.


/*Dror's AND Noga UDP time_Server Program V 1.1*/


#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */
#include <time.h>

#define ECHOMAX 255     /* Longest string to echo */
#define SIZE    64
#define SECONDS 3600
#define PORTMAX 65535
#define PORTMIN 1024




#pragma comment(lib,"wsock32")


/*Functions Deceleration*/
void DieWithError(char *errorMessage);
int getPort();
char * ProcessrClientCommand(char *recvBuff);




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
    char *timeBuff= "\0";



     /* Test for correct number of argument */
    if (argc < 1)
    {
        fprintf(stderr, "\n Error: not Enough Arguments\n");
        exit(1);
    }


    ServerPort = getPort();




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
                   DieWithError("receive Function 'recvfrom()' Failed");
        }

        recvBuff[recvMsgSize]='\0';

        printf("Handling client... \n");
        printf("\n");
        printf("Server Received : %s\n",recvBuff);



        /*Calling the "- ProcessrClientCommand -" calculating the Right Answer To the client and casting the return char* to a Char Array*/
        strncpy(sendBuff,"\0", SIZE);
        strncpy(sendBuff, ProcessrClientCommand(recvBuff), SIZE);


         /* Send received datagram back to the client */
         if (sendto(sock, sendBuff, strlen(sendBuff), 0, (struct sockaddr *) &ClientAddr,sizeof(ClientAddr)) <0){
                    DieWithError("sendto() sent a different number of bytes than expected");
         }

         printf("Response sent.. \n");
         printf("Server Is Waiting for NEW Clients's requests. \n");
    }

    /*Server is Always On NOT REACHED */
}



void DieWithError(char *errorMessage)
{
    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
    exit(1);
}  /* External error handling function */



int getPort(){

    int portNum = 0;
    printf("welcome to The Time Server :) \n \n");
    printf("what Port Do you Want To connect to: \n");

    if (!scanf("%d",&portNum))
        {
            DieWithError("UserChoice is Invalid");
        }
    if(portNum <= PORTMIN || portNum> PORTMAX)
    {
        DieWithError("Invalid Port Number - it Should be more than 1024");
    }

}


/*classification Function -  Argument is The Char* sent from the client- it Calls The Right Time function and retrieve the Time Object*/
char *ProcessrClientCommand(char *recvBuff)
{
    time_t timer;
    struct tm *tm_info;
    char *timeBuff = malloc(SIZE);
    unsigned int ticks;

    /*Using Strncpy to flush the buffer- to prevent a bug*/
    strncpy(timeBuff,"\0", SIZE);

    /* Checking What Function We Should Use using string Compare (strcmp)*/

    /*Works*/
    if(!strcmp(recvBuff, "GetTime")){

        timer = time(NULL);
        tm_info = localtime(&timer);
        strftime(timeBuff, SIZE, "%d-%m-%Y %H:%M:%S", tm_info);
        return timeBuff;

    }

    /*Works*/
    if(!strcmp(recvBuff, "GetTimeWithoutYear")){


        timer = time(NULL);
        tm_info = localtime(&timer);
        strftime(timeBuff, SIZE, "%d-%m %H:%M:%S", tm_info);
        return timeBuff;


    }

    /*Works*/
    if(!strcmp(recvBuff, "GetTimeSinceEpoch")){

        timer = time(NULL);
        sprintf(timeBuff,"%d seconds since: 1/1/1970",timer);
        return timeBuff;

    }

    if(!strcmp(recvBuff, "GetClientToServerDelayEstimation")){
        /*GetTickCount of server, in order to calculate client-server delay*/
        char *serverTicks= (char*)malloc(sizeof(char)*SIZE);
        if (serverTicks == NULL)
            DieWithError("Allocation Failed\n");
        /*convert tick number to string*/
        sprintf(serverTicks, "%d", GetTickCount());
        return serverTicks;
    }

    /*Works*/
    if(!strcmp(recvBuff,"MeasureRTT")){

        /* Returns a flag to the Client so he will be able to know when to calculate the RTT By subtracting the ///
        /*GetTickCount()(after Receiving from Server)- GetTickCount()(before Send) - Both Times Have to Be Taken At The Same Client End */
        return "MRTT";/*MRTT IS A FLAG string*/
    }



    if(!strcmp(recvBuff,"GetDayAndMonth")){
        timer = time(NULL);
        tm_info = localtime(&timer);
        strftime(timeBuff, SIZE, "%d-%m", tm_info);
        return timeBuff;

    }

    /*we do not Receive  "User Closed The Program! :)" - server is always on not depends on Client's Activity*/

}


