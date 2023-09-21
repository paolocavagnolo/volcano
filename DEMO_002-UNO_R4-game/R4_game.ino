#include "Arduino_LED_Matrix.h"  //Include the LED_Matrix library
#include "frames.h"              //Include animation.h header file

#define pause 750

ArduinoLEDMatrix matrix;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  matrix.begin();
  printSentence();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char i = Serial.read();
    switch (i) {
      case ('1'):
        game(1);
        break;
      case ('2'):
        game(2);
        break;
      case ('3'):
        game(3);
        break;
      case ('4'):
        game(4);
        break;
      case ('5'):
        game(5);
        break;
      case ('s'):
        printSentence();
      default:
        break;
    }
  }
}

void printSentence() {
  Serial.println("1.Open source is love");
  Serial.println("2.Arduino lover");
  Serial.println("3.Keep calm and love Arduino");
  Serial.println("4.Eat, code, love");
  Serial.println("5.Peace, love, code");
}

void game(int n) {
  switch (n) {
    case 1:
      matrix.loadFrame(s_opensource);
      delay(pause);
      matrix.loadFrame(s_is);
      delay(pause);
      matrix.loadFrame(s_love);
      delay(pause);
      matrix.loadFrame(s_question);
      break;
    case 2:
      matrix.loadFrame(s_arduino);
      delay(pause);
      matrix.loadFrame(s_lover);
      delay(pause);
      matrix.loadFrame(s_question);
      break;
    case 3:
      matrix.loadFrame(s_keepcalm);
      delay(pause);
      matrix.loadFrame(s_and);
      delay(pause);
      matrix.loadFrame(s_love);
      delay(pause);
      matrix.loadFrame(s_arduino);
      delay(pause);
      matrix.loadFrame(s_question);
      break;
    case 4:
      matrix.loadFrame(s_eat);
      delay(pause);
      matrix.loadFrame(s_code);
      delay(pause);
      matrix.loadFrame(s_love);
      delay(pause);
      matrix.loadFrame(s_question);
      break;
    case 5:
      matrix.loadFrame(s_peace);
      delay(pause);
      matrix.loadFrame(s_love);
      delay(pause);
      matrix.loadFrame(s_code);
      delay(pause);
      matrix.loadFrame(s_question);
      break;
    default:
      matrix.loadFrame(blank);
      break;
  }
}