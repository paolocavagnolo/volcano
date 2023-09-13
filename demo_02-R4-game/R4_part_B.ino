#include "WiFiS3.h"
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

unsigned int udpPort = 2390;  // local port to listen on

WiFiUDP Udp;

char sendBuffer[256];
char receiveBuffer[256];

IPAddress serverIP = IPAddress(192, 48, 56, 2);
IPAddress localIP = IPAddress(192, 48, 56, 3);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  delay(1000);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    WiFi.config(localIP);

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  delay(1000);

  Udp.begin(udpPort);
}

int packetSize = 0;

void loop() {

  readUdp();

  if (Serial.available()) {
    char c = Serial.read();
    if ((c != '\n') && (c != '\r')) {
      sendUdp(c);
    }
    
  }
}

void sendUdp(char m) {
  Udp.beginPacket(serverIP, udpPort);
  Udp.write(m);
  Udp.endPacket();
}

void readUdp() {
  if (Udp.parsePacket()) {
    Udp.read(receiveBuffer, 1);
    Serial.write(receiveBuffer);
  }
}
