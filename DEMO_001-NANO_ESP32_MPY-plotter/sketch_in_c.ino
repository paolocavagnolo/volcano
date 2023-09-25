/* List of sensors :
  A0      Potentiometer   INPUT
  A2      LDR             INPUT
  A5      Mic             INPUT
  I2C     LIS3DH_I2C      INPUT
  D2      Led             OUTPUT
  D4      Buzzer          OUTPUT
  D6      Button          OUTPUT
*/

#define pot A0
#define ldr A2
#define mic A6
#define btn 6
#define led 2

#include "Arduino_SensorKit.h"

  unsigned long last_convert_time = 0;
int convert_interval = 100;
float pitch = 0;
float roll = 0;

bool firstOn = true;
bool firstOff = true;

unsigned long tPOT = 0;
unsigned long tLDR = 0;
unsigned long tIMU = 0;
unsigned long tMIC = 0;

int vPOT = 0;
int vLDR = 0;
int vIMU = 0;
int vMIC = 0;

float p=0;
float r=0;

void readImu() {
  if (last_convert_time < millis()) {
    last_convert_time = millis() + convert_interval;

    if (Accelerometer.available()) {
      float x_Buff = Accelerometer.readX();
      float y_Buff = Accelerometer.readY();
      float z_Buff = Accelerometer.readZ();

      roll = atan2(y_Buff, z_Buff) * 57.3;
      pitch = atan2((-x_Buff), sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
    }
  }
}

int state = 0;
int minVal = 0;
int maxVal = 1023;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Accelerometer.begin();
}

void loop() {

  if (digitalRead(btn) == 1) {
    if (firstOn) {
      firstOff = true;
      firstOn = false;
      digitalWrite(led, HIGH);
      state = state + 1;
      if (state > 3) {
        state = 0;
      }
      Serial.println(state);
      delay(100);
    }
  } else {
    if (firstOff) {
      firstOn = true;
      firstOff = false;
      delay(100);
    }
  }

  if (state == 0) {
    if ((millis() - tPOT) > 100) {
      tPOT = millis();
      vPOT = analogRead(pot);
      analogWrite(led, map(vPOT, 0, 1023, 0, 255));
      char strBuf[50];
      sprintf(strBuf, "%d,%d,%d", vPOT, minVal, maxVal);
      Serial.println(strBuf);
    }
  } else if (state == 1) {
    if ((millis() - tLDR) > 100) {
      tLDR = millis();
      vLDR = analogRead(pot);
      analogWrite(led, map(vLDR, 0, 1023, 0, 255));
      char strBuf[50];
      sprintf(strBuf, "%d,%d,%d", vLDR, minVal, maxVal);
      Serial.println(strBuf);
    }
  } else if (state == 2) {
    if ((millis() - tMIC) > 100) {
      tMIC = millis();
      vMIC = analogRead(pot);
      analogWrite(led, map(vMIC, 0, 1023, 0, 255));
      char strBuf[50];
      sprintf(strBuf, "%d,%d,%d", vMIC, minVal, maxVal);
      Serial.println(strBuf);
    }
  } else if (state == 3) {
    if ((millis() - tIMU) > 100) {
      tIMU = millis();
      p = int((pitch + 180)/360*maxVal);
      r = int((roll + 180)/360*maxVal);
      char strBuf[50];
      sprintf(strBuf, "%d,%d,%d,%d", p, r, minVal, maxVal);
      Serial.println(strBuf);
    }
  }
}
