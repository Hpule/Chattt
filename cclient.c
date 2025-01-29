/******************************************************************************
* cclient.c
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

// ----- Lab Functions -----
void clientControl(int socketNum); 
void processStdin(int socketNum);
void processMsgFromServer(int socketNum); 

void sendToServer(int socketNum);
int readFromStdin(uint8_t * buffer);
void checkArgs(int argc, char * argv[]);

int main(int argc, char * argv[])
{
	int socketNum = 0;         //socket descriptor
	
	checkArgs(argc, argv);

	/* set up the TCP Client socket  */
	socketNum = tcpClientSetup(argv[1], argv[2], DEBUG_FLAG);

	clientControl(socketNum);	

	// sendToServer(socketNum);
	
	close(socketNum);
	
	return 0;
}



// void sendToServer(int socketNum)
// {
// 	uint8_t sendBuf[MAXBUF];   //data buffer
// 	int sendLen = 0;        //amount of data to send
// 	int sent = 0;            //actual amount of data sent/* get the data and send it   */
	
// 	sendLen = readFromStdin(sendBuf);
// 	printf("read: %s string len: %d (including null)\n", sendBuf, sendLen);
	
// 	sent = sendPDU(socketNum, sendBuf, sendLen); 
// 	if (sent < 0)
// 	{
// 		perror("sendPDU call");
// 		exit(-1);
// 	}

// 	printf("Amount of data sent is: %d\n", sent);
// }

void clientControl(int socketNum){
	setupPollSet();

	addToPollSet(STDIN_FILENO);	// Monitor user input
	addToPollSet(socketNum); 	// Monitor server messages

	while(1){
		printf("$: "); // Display the prompt
		fflush(stdout); // Flush the output buffer
		
		int socketNumber = pollCall(-1); 
		printf("pollCall returned socketNumber: %d\n", socketNumber);

		if(socketNumber < 0){
			printf("pollCall failed\n"); 
			exit(-1); 
		}

        // Handle activity on STDIN (user input)
        if (socketNumber == STDIN_FILENO) {
            printf("Processing stdin...\n");
            processStdin(socketNum); // Pass the server socket to send user input
        }
        // Handle activity on the server socket
        else if (socketNumber == socketNum) {
            printf("Processing server message...\n");
            processMsgFromServer(socketNum);
        }
	}
}

void processStdin(int socketNum){
	uint8_t sendBuf[MAXBUF];   //data buffer
	int sendLen = 0;        //amount of data to send

	sendLen = readFromStdin(sendBuf);

	sendBuf[sendLen - 1] = '\0'; // Remove the trailing newline added by readFromStdin

	if(sendPDU(socketNum, sendBuf, sendLen) < 0){
		perror("sendPDU call");
		exit(-1);
	}

    printf("Sent %d bytes to server: %s\n", sendLen, sendBuf);
}


void processMsgFromServer(int socketNum){
	uint8_t dataBuffer[MAXBUF] = {0};
	int recvLen = 0;

	recvLen = recvPDU(socketNum, dataBuffer, MAXBUF);

    if (recvLen == 0) {
        // Server closed the connection
        printf("Server closed the connection.\n");
        close(socketNum);
        exit(0);
    } else if (recvLen < 0) {
        perror("recvPDU failed");
        close(socketNum);
        exit(-1);
    }

    // Null-terminate and print the server's response
    dataBuffer[recvLen] = '\0';
    printf("Server response: %s\n", dataBuffer);
}


int readFromStdin(uint8_t * buffer)
{
	char aChar = 0;
	int inputLen = 0;        
	
	// Important you don't input more characters than you have space 
	buffer[0] = '\0';
	printf("Enter data: ");
	while (inputLen < (MAXBUF - 1) && aChar != '\n')
	{
		aChar = getchar();
		if (aChar != '\n')
		{
			buffer[inputLen] = aChar;
			inputLen++;
		}
	}
	
	// Null terminate the string
	buffer[inputLen] = '\0';
	inputLen++;
	
	return inputLen;
}

void checkArgs(int argc, char * argv[])
{
	/* check command line arguments  */
	if (argc != 3)
	{
		printf("usage: %s host-name port-number \n", argv[0]);
		exit(1);
	}
}