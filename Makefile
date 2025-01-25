# Makefile for CPE464 tcp test code
# written by Hugh Smith - April 2019

CC = gcc
CFLAGS = -g -Wall -std=gnu99
LIBS = 

# Add sendPDU.o and recvPDU.o to the list of object files
OBJS = networks.o gethostbyname.o pollLib.o safeUtil.o sendPDU.o recvPDU.o

all: cclient server server_poll

cclient: cclient.c $(OBJS)
	$(CC) $(CFLAGS) -o cclient cclient.c $(OBJS) $(LIBS)

server: server.c $(OBJS)
	$(CC) $(CFLAGS) -o server server.c $(OBJS) $(LIBS)

server_poll: server.c $(OBJS)
	$(CC) $(CFLAGS) -o server_poll server_poll.c $(OBJS) $(LIBS)

sendPDU.o: sendPDU.c sendPDU.h
	$(CC) $(CFLAGS) -c sendPDU.c

recvPDU.o: recvPDU.c recvPDU.h
	$(CC) $(CFLAGS) -c recvPDU.c

# Rule for building .o files
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

cleano:
	rm -f *.o

clean:
	rm -f server cclient *.o
