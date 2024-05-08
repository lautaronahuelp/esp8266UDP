#ifndef UDP_FUNCTIONS_H
#define UDP_FUNCTIONS_H

char * server1;
int port1;

int portLocal;

char replyPacket[];
char incomingPacket[];

bool ack;

void startUDP();

void receiveACK();

void sendMessage();

#endif
