#include "WiFiS3.h"
#include "arduino_secrets.h"

char ssid[] = "Demo2023";
char pass[] = "32020m3D";

int status = WL_IDLE_STATUS;

unsigned int udpPort = 2390;

IPAddress serverIP = IPAddress(192, 48, 56, 2);
IPAddress remoteIP = IPAddress(192, 48, 56, 3);

char sendBuffer[256];
char receiveBuffer[256];

WiFiUDP Udp;

#include "Arduino_LED_Matrix.h"
#include "frames.h"

const uint32_t *opensource_is_love[] = { s_opensource, s_is, s_love };
const uint32_t *arduino_lover[] = { s_arduino, s_lover };
const uint32_t *keepcalm_and_love[] = { s_keepcalm, s_and, s_love, s_arduino };
const uint32_t *eat_code_love[] = { s_eat, s_code, s_love };
const uint32_t *peace_love_code[] = { s_peace, s_love, s_code };

ArduinoLEDMatrix matrix;

void setup() {
  //Initialize serial and wait for port to open:

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  delay(1000);
  WiFi.config(serverIP);

  status = WiFi.beginAP(ssid, pass);

  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    digitalWrite(LED_BUILTIN, HIGH);
    // don't continue
    while (true)
      ;
  }

  // delay(10000)
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(900);
  }

  Udp.begin(udpPort);
  matrix.begin();
}

int packetSize = 0;
int pause_game = 600;
bool firstGame = true;
bool firstIdle = true;
bool firstWaiting = true;

unsigned long tConnect = 0;
unsigned long tIdle = 0;
unsigned long tGame = 0;
unsigned long tWaiting = 0;

int s_index = 0;
int state = 0;
int sentence = 0;

bool myTurn = true;

void loop() {

  if (state == 0) {

    readUdp();

    firstWaiting = true;
    firstGame = true;

    if (myTurn) {
      if ((millis() - tIdle) > 10000) {
        tIdle = millis();
        printSentence();
      }

      if (Serial.available()) {
        char c = Serial.read();
        int a = c;
        if ((c != '\n') && (c != '\r')) {
          if ((a <= 48) || (a >= 54)) {
            errInput();
            tIdle = millis();
          } else {
            sendUdp(c);
            state = 2;
            myTurn = false;
          }
        }
      }
    } else {
      if ((millis() - tIdle) > 10000) {
        tIdle = millis();
        printWaiting();
      }
    }
    
  }

  else if (state == 1) {
    if (firstGame) {
      printChallenge();
      firstGame = false;
    }

    game(sentence);

    if (Serial.available()) {
      char c = Serial.read();
      if ((c != '\n') && (c != '\r')) {
        int a = c - 48;
        if (a == sentence) {
          Serial.println();
          Serial.println();
          Serial.println("===============");
          Serial.println("CORRECT! BRAVO!");
          Serial.println("===============");
          Serial.println();
          Serial.println();
          sendUdp(0);
          matrix.loadFrame(blank);
          delay(1000);
          state = 0;
        } else {
          Serial.println();
          Serial.println("WRONG! Try again...");
          Serial.println();
        }
      }
    }
  }

  else if (state == 2) {

    if (Udp.parsePacket()) {
      Udp.read(receiveBuffer, 1);
      int a = receiveBuffer[0];
      if (a == 0) {
        Serial.println();
        Serial.println();
        Serial.println("=============");
        Serial.println("THEY MADE IT!");
        Serial.println("=============");
        Serial.println();
        Serial.println();
      }
      delay(2000);
      state = 0;
    }

    if (firstWaiting) {
      waiting();
      firstWaiting = false;
    }

    if ((millis() - tWaiting) > 1000) {
      tWaiting = millis();
      Serial.print(".");
    }
  }


  status = WiFi.status();
  if (status != WL_AP_CONNECTED) {
    if ((millis() - tConnect) > 2000) {
      tConnect = millis();
      for (int i = 0; i < 2; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(90);
      }
    }
  }
}


void sendUdp(char m) {
  Udp.beginPacket(remoteIP, udpPort);
  Udp.write(m);
  Udp.endPacket();
}

void readUdp() {
  if (Udp.parsePacket()) {
    Udp.read(receiveBuffer, 1);
    int a = receiveBuffer[0];
    if ((a > 48) && (a < 54)) {
      state = 1;
      sentence = a - 48;
      myTurn = true;
    }
  }
}

void errInput() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.println();
  }
  Serial.println("only number from 1 to 5, please.");
}

void waiting() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.println();
  }
  Serial.print("Waiting for the other team to guess the sentence ");
}

void printSentence() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.println();
  }

  Serial.println("Choose a sentence, write the number here and click enter to send it to the other team");
  Serial.println("");
  Serial.println("1.Open source is love");
  Serial.println("2.Arduino lover");
  Serial.println("3.Keep calm and love Arduino");
  Serial.println("4.Eat, code, love");
  Serial.println("5.Peace, love, code");
}

void printWaiting() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.println();
  }

  Serial.println("Waiting for the other team to start a game...");
  Serial.println("");
}

void printChallenge() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.println();
  }
  Serial.println("-----------------------");
  Serial.println("YOU'VE BEEN CHALLENGED!");
  Serial.println("-----------------------");
  Serial.println("Look at the led-matrix and choose the corrisponding sentence, from the list:");
  Serial.println("");
  Serial.println("1.Open source is love");
  Serial.println("2.Arduino lover");
  Serial.println("3.Keep calm and love Arduino");
  Serial.println("4.Eat, code, love");
  Serial.println("5.Peace, love, code");
}

void game(int n) {
  if ((millis() - tGame) > pause_game) {
    tGame = millis();

    if (n == 1) {
      if (s_index == 3) {
        s_index = 0;
        matrix.loadFrame(blank);
        pause_game = 1500;
      } else {
        matrix.loadFrame(opensource_is_love[s_index]);
        s_index++;
        pause_game = 600;
      }
    } else if (n == 2) {
      if (s_index == 2) {
        s_index = 0;
        matrix.loadFrame(blank);
        pause_game = 1500;
      } else {
        matrix.loadFrame(arduino_lover[s_index]);
        s_index++;
        pause_game = 600;
      }
    } else if (n == 3) {
      if (s_index == 4) {
        s_index = 0;
        matrix.loadFrame(blank);
        pause_game = 1500;
      } else {
        matrix.loadFrame(keepcalm_and_love[s_index]);
        s_index++;
        pause_game = 600;
      }
    } else if (n == 4) {
      if (s_index == 3) {
        s_index = 0;
        matrix.loadFrame(blank);
        pause_game = 1500;
      } else {
        matrix.loadFrame(eat_code_love[s_index]);
        s_index++;
        pause_game = 600;
      }
    } else if (n == 5) {
      if (s_index == 3) {
        s_index = 0;
        matrix.loadFrame(blank);
        pause_game = 1500;
      } else {
        matrix.loadFrame(peace_love_code[s_index]);
        s_index++;
        pause_game = 600;
      }
    }
  }
}
