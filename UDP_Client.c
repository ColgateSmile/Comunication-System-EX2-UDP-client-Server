/* CHANGES FROM UNIX VERSION                                                   */
/*                                                                             */
/* 1.  Changed header files.                                                   */
/* 2.  Added WSAStartUP() and WSACleanUp().                                    */
/* 3.  Used closesocket() instead of close().                                  */

// Don't forget to include "wsock32" in the library list.


//Dror's AND Noga UDP time_Client Program V 1.1//



#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock2.h>    /* for socket(),... */
#include <stdlib.h>
#include <ws2tcpip.h>     /* for exit() */

#define ECHOMAX 255     /* Longest string to echo */
#define FOREVER 1       /*Forever While Loop*/


//@@@@Function Deceleration@@@@//
void DieWithError(char *errorMessage);
int usrChoise();
void Run(unsigned short ServerPort,WSADATA wsaData);
int getPort();




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


    ServerPort = getPort(); /*@#@!@Ask if we have to get the port number from Argument list  - than use atoi(argv[1]);*/
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }

    Run(ServerPort,wsaData);


    exit(0);
}





void DieWithError(char *errorMessage)
{
    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
    exit(1);
}  /* External error handling function */


/*this Function is the User Interface With the Client PROGRAM*/
int usrChoice()
{
    int Choice;
    printf("\n hello what Do the Server can help you with: \n");
    printf("\n choose one of the Options:\n 1-GetTime\n 2-GetTimeWithoutYear\n 3-GetTimeSinceEpoch\n 4-GetClientToServerDelayEstimation\n 5-MeasureRTT\n 6-GetDayAndMonth\n 0-ExitClient ");

    if (!scanf("%d",&Choice))
        {
            DieWithError("UserChoice is Invalid");
        }

    if(Choice == 0){
        exit(0);
    }


    return Choice;
}


/*this Function is the User Interface With the Client PROGRAM - Choose A port Number !!@!!! Ask if its important to get it from the ArgV*/
int getPort(){

    int portNum = 0;
    printf("hello what Port Do you Want To connect to: \n");

    if (!scanf("%d",&portNum))
        {
            DieWithError("UserChoice is Invalid");
        }
    if(portNum <= 1024)
    {
        DieWithError("Invalid Port Number - it Should be more than 1024");
    }

}



//this Function Runs The client Loop//
void Run( unsigned short ServerPort,WSADATA wsaData)
{

    int sock;                        /* Socket descriptor */
    struct sockaddr_in ServerAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char recvBuff[255];        /* Buffer for echo string */
    int sendRes;
    int i;
    char sendBuff[16] = "\0";
    int userChoice = 0;




 while(FOREVER)
        {


       userChoice = usrChoice();
       printf("\n Choice is: %d \n",userChoice);


               //Here We got Our Self A BUG @@!!@@##!@# //
               //Description: on the Second Time We Try To send To the Server We get socket() failed//

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
                DieWithError("Error at recvfrom() Server Not Found");
        }
        printf("Client Received : %s\n",recvBuff);
        printf("Closing Connection.");
        closesocket(sock);
        WSACleanup();  /* Cleanup Winsock */


        }


}

