#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "SIAFunctions.h"

const char* ssid = "LAUTARO";
const char* pass = "5373ND1P1";

//uint8_t server1[4] = { 192, 168, 100, 4 };
char * server1 = "192.168.100.4";
int port1 = 54000;

int portLocal = 54000;

char replyPacket[] = "Mensaje de prueba :-)";
char incomingPacket[255];

bool ack = false;

int repeatTime = 999, lastSendTime = 0;

WiFiUDP UDP;

int eventoTest[16] = { 1, 2, 3, 4, 1, 8, 1, 6, 0, 2, 0, 0, 0, 0, 0, 2 };
char account[5] = "1234";
int sequence = 77;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.print("Connecting to ");  
  Serial.println(ssid);  
  WiFi.mode(WIFI_STA);             // set mode to wifi station
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){ // check status of connection
    delay(500);    
    Serial.print(".");
  }  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP());  // print the Ip alocated by router
  delay(500);

  UDP.begin(random(49152, 65536));

  listNetworks();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!ack && (millis() - lastSendTime > repeatTime))
  {
    lastSendTime = millis();
    sendMessage();
  }
  receiveACK();
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}

void sendMessage()
{
  int beginP = 0, endP = 0; 
  //UDP.beginPacket(Udp.remoteIP(), Udp.remotePort());
  beginP = UDP.beginPacket(server1, port1);
  //UDP.write(replyPacket);
  UDP.write(genDC09String(1, eventoTest, sequence, account));
  endP = UDP.endPacket();
  Serial.print("*");
  if(endP != 1) Serial.print(beginP);
  if(beginP != 1) Serial.print(":");
  if(beginP != 1) Serial.print(endP);
}

void receiveACK()
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
    if(checkDC09CRC(&incomingPacket[1])) ack = true;
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
}
