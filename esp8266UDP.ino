#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "SIAFunctions.h"
#include "UDPFunctions.h"

const char* ssid = "LAUTARO";
const char* pass = "5373ND1P1";

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

  startUDP();
  
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
