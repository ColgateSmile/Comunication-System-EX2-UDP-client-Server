/* CHANGES FROM UNIX VERSION                                                   */
/*                                                                             */
/* 1.  Changed header files.                                                   */
/* 2.  Added WSAStartUP() and WSACleanUp().                                    */
/* 3.  Used closesocket() instead of close().                                  */

/* Don't forget to include "wsock32" in the library list.


/*Dror's AND Noga UDP time_Client Program V 1.1*/


#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock2.h>    /* for socket(),... */
#include <stdlib.h>
#include <ws2tcpip.h>     /* for exit() */

#define ECHOMAX 255     /* Longest string to echo */
#define FOREVER 1       /*Forever While Loop*/
#define SIZE    64
#define AVERAGE_CASES 100


/*@@@@Function Deceleration@@@@*/
void DieWithError(char *errorMessage);
int getPort();
int usrChoise();
void Run(unsigned short ServerPort,WSADATA wsaData);
void recvResponse(int sock, char recvBuff[ECHOMAX], struct sockaddr_in fromAddr, unsigned int fromSize);
unsigned int sendRequest(int sock, char *sendBuff, struct sockaddr_in ServerAddr);
struct sockaddr_in SetSocketAddr();



/*Main*/
void main(int argc, char *argv[])
{
    unsigned short ServerPort;     /* Echo server port */
    WSADATA wsaData;                 /* Structure for WinSock setup communication */

    /* Test for correct number of arguments */
    if ((argc < 1))
    {
        fprintf(stderr, "Error: no server port provided\n");
        exit(1);
    }

    ServerPort = getPort();

    Run(ServerPort,wsaData);


    exit(0);
}



/* @@@@@Functions implementation@@@@@ */


/* External error handling function */
void DieWithError(char *errorMessage)
{
    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
    exit(1);
}


/* Function for the User Interface With the Client program*/
int usrChoice()
{
    int Choice;

    printf("\n");
    printf(" welcome to The 'Time Client Application'\n\n");
    printf(" what Do the Server can help you with: \n\n");
    printf(" choose one of the Options(1-6):\n\n 1-Get Time\n\n 2-Get Time Without Year\n\n 3-Get Time Since Epoch\n\n 4-Get Client To Server Delay Estimation\n\n 5-Measure RTT\n\n 6-Get Day And Month\n\n 0-ExitClient ");
    printf("\n");
    printf(" \n");

    if (!scanf("%d",&Choice))
        {
            DieWithError("UserChoice is Invalid");
        }
    return Choice;
}

/*Additional Function for the User Interface With the Client program - Choose A port Number*/
int getPort(){

    int portNum = 0;
    printf("'Time Client Application:'\n\n");
    printf(" what Port Number Do you Want To connect to: \n\n");

    if (!scanf("%d",&portNum))
        {
            DieWithError("User Choice of port is Invalid");
        }
    if(portNum <= 1024)
    {
        DieWithError("Invalid Port Number - it Should be more than 1024");
    }

}


/*function that Loads The Send Buffer With The According String to to the User Choice, sendBuffer is a String That we send to the server */
char *loadToSendBuffer(int Choice){


    switch (Choice)
    {
    case 1:
        return "GetTime";
    case 2:
        return "GetTimeWithoutYear";
    case 3:
        return "GetTimeSinceEpoch";
    case 4:
        return "GetClientToServerDelayEstimation";
    case 5:
        return "MeasureRTT";
    case 6:
        return "GetDayAndMonth";
    case 0:
        return "User Closed The Program! :)";
    }
}

/*This Function Construct The Server Address*/
struct sockaddr_in SetSocketAddr(){
    struct sockaddr_in ServerAddr; /* Echo server address */
    /* Construct the server address structure */
    memset(&ServerAddr, 0, sizeof(ServerAddr));    /* Zero out structure */
    ServerAddr.sin_family = AF_INET;                 /* Internet address family */
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  /* Server IP address */
    return ServerAddr;
}

/*This Function Checks The User Input*/
int UserInputCheck(){

    int flag = 1;
    int userChoice =0;
    while(flag)
    {
        userChoice = usrChoice();
        if(userChoice<=6 && userChoice>=0){
            flag=0;
        }
        else{
            printf("\n");
            printf("user input is invalid! -> please enter a number between 1-6\n");
        }
    }
    return userChoice;

}


/*This Function Sends And Checks using Sendto()- after The send the Function returns the time of send*/
unsigned int sendRequest(int sock, char *sendBuff, struct sockaddr_in ServerAddr){
    int sendRes;
    unsigned int ticksSend;


    /* Send the string, including the null terminator, to the server */
    if (sendRes = sendto(sock, sendBuff, strlen(sendBuff), 0, (struct sockaddr *)
               &ServerAddr, sizeof(ServerAddr)) < 0)
               {
                   DieWithError("Error at sendto()");
               }

    /*For The RTT measurement - we save the time in MiliSeconds after Last Byte Was Sent in this packet*/
    ticksSend = GetTickCount();


return ticksSend;
}



/*This Function Receives The Servers Response */
void recvResponse(int sock, char recvBuff[ECHOMAX], struct sockaddr_in fromAddr, unsigned int fromSize){

    if ((recvfrom(sock, recvBuff, 255, 0, (struct sockaddr *) &fromAddr,&fromSize) < 0) )
        {
            DieWithError("\nError Server Not Found\n");
        }
}




/*this Function Runs The client Loop*/
void Run(unsigned short ServerPort, WSADATA wsaData)
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in ServerAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char recvBuff[ECHOMAX];        /* Buffer for echo string */
    int sendRes;
    int flag = 1;
    char *sendBuff = "\0";
    int userChoice = 0;
    unsigned int ticksSend;
    unsigned int ticksRecive;
    /*unsigned int ticksServer;
    unsigned int ticksClient;*/


    ServerAddr = SetSocketAddr();

    while(FOREVER){


        userChoice = UserInputCheck();


        /*closing the Program here*/
        if(userChoice == 0)
        {
            printf("\n");
            printf("Closing Connection.");
            printf("\n\n");
            closesocket(sock);
            printf("User Closed The Program! :) all Sockets are closed");
            exit(0);
        }
            sendBuff = loadToSendBuffer(userChoice);

            printf("\n");
            printf("User Choice is: %d \n",userChoice);
            printf("%s \n",sendBuff);


            if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
                {
                    fprintf(stderr, "WSAStartup() failed");
                    exit(1);
                }

            /* Create a best-effort datagram socket using UDP */
            if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            {
                DieWithError("socket() failed");

            }

             /* Server port */
            ServerAddr.sin_port = htons(ServerPort);


            fromSize = sizeof(fromAddr);
            strncpy(recvBuff,"\0\0\0\0\0\0\0\0\0", SIZE);


            /*send request*/
            sendRequest(sock, sendBuff, ServerAddr);
            /*Recv a response*/
            recvResponse(sock, recvBuff, fromAddr, fromSize);


            if(userChoice == 4){
                unsigned int ticksServer[AVERAGE_CASES] = {0};
                unsigned int ticksClient[AVERAGE_CASES] = {0};
                int i = 0;
                int averageFlag = 100;
                double totalDelay = 0.0;
                double delayAverage = 0.0;

                while (averageFlag){
                    /*send 100 requests and save server's time stamp*/
                    sendRequest(sock, sendBuff, ServerAddr);
                    averageFlag--;
                    i++;
                }

                averageFlag = AVERAGE_CASES;
                i = 0;

                while (averageFlag){
                    /*receive 100 responses and save client's time stamp*/
                    recvResponse(sock, recvBuff, fromAddr, fromSize);
                    ticksServer[i] = atoi(recvBuff);
                    ticksClient[i] = GetTickCount();
                    averageFlag--;
                    i++;
                }

                for (i=0; i<AVERAGE_CASES; i++)
                    totalDelay += ticksClient[i] - ticksServer[i];

                delayAverage = totalDelay/AVERAGE_CASES;
                printf("Average delay of 100 requests is %.6f MiliSeconds\n", delayAverage);
            }


            /*in case of RTT measurement - The server Send us a Flag AND we calculate the Time here => TimeNow - Time of packet Sent*/
            if(!strcmp(recvBuff, "MRTT")){
                double totalRtt = 0.0;
                double rttAverage = 0.0;
                int averageFlag = 100;

                /*100 Iteration of Send-receive to server*/
                while (averageFlag){
                     double  rtt = 0;

                    /*send request*/
                    ticksSend = sendRequest(sock, sendBuff, ServerAddr);
                    /*receive request*/
                    recvResponse(sock, recvBuff, fromAddr, fromSize);

                    ticksRecive = GetTickCount();
                    rtt += (double)(ticksRecive - ticksSend);

                    totalRtt += rtt;
                    averageFlag--;

                }
                rttAverage = totalRtt/AVERAGE_CASES;
                printf("\nAverage RTT of 100 requests is %.6f milliseconds", rttAverage);
            }



            else{
                printf("Client Received : %s\n",recvBuff);
            }

            printf("\n\n");

            WSACleanup();  /* Cleanup Winsock */


        }


}

