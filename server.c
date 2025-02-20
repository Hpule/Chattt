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

#define MAXBUF 1024
#define DEBUG_FLAG 1

void recvFromClient(int clientSocket);
int checkArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	int mainServerSocket = 0;   //socket descriptor for the server socket
	int clientSocket = 0;   //socket descriptor for the client socket
	int portNumber = 0;
	
	portNumber = checkArgs(argc, argv);
	
	//create the server socket
	mainServerSocket = tcpServerSetup(portNumber);

	while(1){
		// wait for client to connect
		clientSocket = tcpAccept(mainServerSocket, DEBUG_FLAG);

		recvFromClient(clientSocket);
		
		/* close the sockets */
		close(clientSocket);
	}
	close(mainServerSocket);

	
	return 0;
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
		printf("Message received, length: %d Data: %s\n", messageLen, dataBuffer);
		// Echo the message back to the client
		sendPDU(clientSocket, dataBuffer, messageLen);
		printf("Echoed message back to client\n");
	}
	else
	{
		printf("Connection closed by other side\n");
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