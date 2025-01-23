/*  
*/

#ifndef __sendPDU__
#define __sendPDU__

#include <stdint.h>

int sendPDU(int clientSocket, uint8_t * dataBuffer, int lengthOfData);

#endif