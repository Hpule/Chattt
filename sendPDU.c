/*

*/

#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "safeUtil.h"

int sendPDU(int clientSocket, uint8_t * dataBuffer, int lengthOfData){
    printf("\nclientSocket: %d\tdataBuffer: %s\tlengthOfData: %d", clientSocket, dataBuffer, lengthOfData); 


    // length of Data + 2
    int pduLen = lengthOfData + 2; 

    // length -> network order Bufferdata
    uint16_t length_network_order = htons(lengthOfData);

    // data buffer memcpy
    uint8_t * pdu = (uint8_t * )malloc(pduLen);    

    if(pdu == NULL){
        perror("Failed to allocate memory for PDU"); 
        return -1; 
    }
    memcpy(pdu, &length_network_order, sizeof(length_network_order)); 
    memcpy(pdu + 2 , dataBuffer,  lengthOfData);  

    // Safe Send 
    int bytesSent  = safeSend(clientSocket, pdu, pduLen, 0);

    printf("\n: %d\tPDU: %hhn\tlengthNetworkOrder: %d\tbytesSend: %d\n", pduLen, pdu, length_network_order, bytesSent); 

    if(bytesSent != pduLen ){
        perror("Safe Safe Fail");
        free(pdu); 
        return -1;  
    } 

    free(pdu); 
    return bytesSent; 
}
