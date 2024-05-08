#include "UDPFunctions.h"

char * server1 = "192.168.100.4";
int port1 = 54000;

int portLocal = 54000;

char replyPacket[] = "Mensaje de prueba :-)";
char incomingPacket[255];

//bool ack = false;

void startUDP()
{
  UDP.begin(random(49152, 65536));
  UDP.flush();
}

void sendMessage()
{
  static long int lastTry = 0;
  int interval = 100;
  int beginP = 0, endP = 0; 
  if (millis() - lastTry > interval) 
  {
    //UDP.beginPacket(Udp.remoteIP(), Udp.remotePort());
    beginP = UDP.beginPacket(server1, port1);
    //UDP.write(replyPacket);
    //CORREGIR ESTE TEMA:
    UDP.write('\n');
    UDP.write(genDC09String(1, eventoTest, sequence, account));
    UDP.write('\r');
    endP = UDP.endPacket();
    Serial.print("*");
    if(endP != 1) Serial.print(beginP);
    if(beginP != 1) Serial.print(":");
    if(beginP != 1) Serial.print(endP);
    lastTry = millis();
  }
  
}

void receiveACK(bool * ack)
{
  int packetSize = UDP.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, UDP.remoteIP().toString().c_str(), UDP.remotePort());
    int len = UDP.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    //if(strcmp(incomingPacket, "ACK") == 0) ack = true;
    if(checkDC09CRC(&incomingPacket[1])) *ack = true;
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
}
