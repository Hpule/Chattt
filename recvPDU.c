/*

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

#include "safeUtil.h"


int  recvPDU(int socketNumber, uint8_t * dataBuffer, int bufferSize){
        printf("\nsocketNumber: %d\tdataBuffer: %s\tBufferSize: %d\n", socketNumber, dataBuffer, bufferSize); 


    // ----- First Call receive 2 bytes ----- //
    uint16_t lengthField; 
    int bytesReceived = safeRecv(socketNumber, (uint8_t *)&lengthField, sizeof(lengthField), MSG_WAITALL);
    if(bytesReceived == 0){
        return 0; // Closed by other side
    }

    if(bytesReceived != sizeof(lengthField)){
        fprintf(stderr, "Error: Incomplete length field received\n");
        return -1; // Error
    }

    // Convert length field from network byte order to host byte order
    int length_host_order = ntohs(lengthField);

    // Step 2: Validate the length
    if (length_host_order <= 0 || length_host_order > bufferSize) {
        fprintf(stderr, "Error: Invalid or oversized PDU length: %d\n", length_host_order);
        return -1; // Error
    }

    // ----- Second Call receive rest ----- //
    bytesReceived = safeRecv(socketNumber, dataBuffer, length_host_order, MSG_WAITALL);
    if(bytesReceived == 0){
        return 0; // Closed by other side
    }

    if(bytesReceived != length_host_order){
        fprintf(stderr, "Error: Incomplete PDU received: expected %d bytes, received %d bytes.\n",
                length_host_order, bytesReceived);
        return -1; // Error
    }


    return bytesReceived ;
}
