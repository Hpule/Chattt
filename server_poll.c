/******************************************************************************
* server.c
* 
* Writen by Prof. Smith, updated Jan 2023
* Use at your own risk.  
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdint.h>

#include "networks.h"
#include "safeUtil.h"
#include "sendPDU.h"
#include "recvPDU.h"
#include "pollLib.h"

#define MAXBUF 1024
#define DEBUG_FLAG 1


void serverControl(int mainServerSocket); 
void addNewSocket(int socketNumber); 
void processClient(int clientSocket); 

void recvFromClient(int clientSocket);
int checkArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    printf("Main\n"); 
	int mainServerSocket = 0;   //socket descriptor for the server socket
	// int clientSocket = 0;   //socket descriptor for the client socket
	int portNumber = 0;
	
	portNumber = checkArgs(argc, argv);
	
	//create the server socket
	mainServerSocket = tcpServerSetup(portNumber);

        
    serverControl(mainServerSocket);
	close(mainServerSocket);

        // wait for client to connect
		// clientSocket = tcpAccept(mainServerSocket, DEBUG_FLAG);
		// recvFromClient(clientSocket);
        
        // serverControl Control()
		/* close the sockets */
		// close(clientSocket);

	return 0;
}

void serverControl(int mainServerSocket){
    setupPollSet();
    addToPollSet(mainServerSocket);
    // printf("serverControl\n"); 


    while(1){
        int socketNumber = pollCall(-1); 
        printf("pollCall returned socketNumber: %d\n", socketNumber);

        if(socketNumber < 0){
            printf("pollCall failed\n"); 
            exit(-1); 
        }

        (socketNumber == mainServerSocket) ?  addNewSocket(mainServerSocket) : processClient(socketNumber);
    }
}


void addNewSocket(int socketNumber){
    // Processes a new connection (e.g. accept(), add to pollset())
    int newSocket = tcpAccept(socketNumber, DEBUG_FLAG); 
    addToPollSet(newSocket); 

    printf("New client connected: socket  %d\n", newSocket); 
}

void processClient(int clientSocket){
    // Calls recvPDU(), and outputs message
    recvFromClient(clientSocket); 
}

void recvFromClient(int clientSocket)
{
    printf("Processing client on socket: %d\n", clientSocket);

	//Null and Change Change everything!
	uint8_t dataBuffer[MAXBUF] = {0};
	int messageLen = 0;

	//now get the data from the client_socket
	// messageLen = recvPDU(clientSocket, dataBuffer, MAXBUF, 0); 
	messageLen = recvPDU(clientSocket, dataBuffer, MAXBUF); 
	if (messageLen < 0)
	{
		perror("recvPDU call");
		exit(-1);
	}

	if (messageLen > 0)
	{
    printf("Message received on socket %d, length: %d, Data: %s\n", clientSocket, messageLen, dataBuffer);
	}
	else
	{
        printf("Client disconnected: socket %d\n", clientSocket);
        removeFromPollSet(clientSocket); 
        close(clientSocket); 
    }
}

int checkArgs(int argc, char *argv[])
{
	// Checks args and returns port number
	int portNumber = 0;

	if (argc > 2)
	{
		fprintf(stderr, "Usage %s [optional port number]\n", argv[0]);
		exit(-1);
	}
	
	if (argc == 2)
	{
		portNumber = atoi(argv[1]);
	}
	
	return portNumber;
}


