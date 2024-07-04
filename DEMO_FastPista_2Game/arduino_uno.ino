#define btnA 13
#define btnB 11
#define btnC 12

#define DEBOUNCE 50

#include <Arduino.h>
#include <TM1637Display.h>

#define clkA 8
#define dioA 9

#define clkB 6
#define dioB 7

#define clkC 4
#define dioC 5

TM1637Display displayA(clkA, dioA);
TM1637Display displayB(clkB, dioB);
TM1637Display displayC(clkC, dioC);

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define DATA_PIN 10
#define NUM_LEDS 300
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define FPS 25

CRGB leds[NUM_LEDS];

#define FPS_RED 100
#define FPS_GREEN 90
#define FPS_BLUE 80
unsigned long tFPS_RED, tFPS_GREEN, tFPS_BLUE;

int game = 1;

void setup() {

  delay(1000);

  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(btnC, INPUT_PULLUP);

  displayA.setBrightness(0x0f);
  displayB.setBrightness(0x0f);
  displayC.setBrightness(0x0f);

  displayA.clear();
  displayB.clear();
  displayC.clear();

  displayA.showNumberDec(0, false);
  displayB.showNumberDec(0, false);
  displayC.showNumberDec(0, false);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  if (!digitalRead(btnA)) {
    game = 2;
  } else {
    game = 1;
  }

  delay(1000);
}

int nA = 0, nB = 0, nC = 0;
int kA = 0, kB = 0, kC = 0;
int nA_old = 0, nB_old = 0, nC_old = 0;

unsigned long tA, tB, tC;
bool fA = false, fB = false, fC = false;

unsigned long tRead;
int tempAVG[20];
int pot = 0;
int pot_temp = 0, pot_temp_old = 0;

int lA = 5, sA = (NUM_LEDS / 3) * 0, eA = sA + lA;
int lB = 5, sB = (NUM_LEDS / 3) * 1, eB = sB + lB;
int lC = 5, sC = (NUM_LEDS / 3) * 2, eC = sC + lC;
unsigned long tShow, tVel;
int vel = 50;

int nLap = 3;
int tLap[] = { 0, 0, 0 };

void loop() {

  readButtons();

  readPot();

  displays();


  if (game == 1) {
    if ((millis() - tVel) > (1000 / vel)) {
      FastLED.clear();
      tVel = millis();

      sA = sA + 1;
      if (sA >= NUM_LEDS) {
        sA = sA - NUM_LEDS;
      }

      sB = sB + 1;
      if (sB >= NUM_LEDS) {
        sB = sB - NUM_LEDS;
      }

      sC = sC + 1;
      if (sC >= NUM_LEDS) {
        sC = sC - NUM_LEDS;
      }

      if ((sA + lA) >= NUM_LEDS) {
        eA = (sA + lA) - NUM_LEDS;
        fill_solid(&leds[0], eA, CRGB::Red);
        fill_solid(&leds[sA], (NUM_LEDS - sA), CRGB::Red);
      } else {
        eA = sA + lA;
        fill_solid(&leds[sA], lA, CRGB::Red);
      }

      if ((sB + lB) >= NUM_LEDS) {
        eB = (sB + lB) - NUM_LEDS;
        fill_solid(&leds[0], eB, CRGB::Blue);
        fill_solid(&leds[sB], (NUM_LEDS - sB), CRGB::Blue);
      } else {
        eB = sB + lB;
        fill_solid(&leds[sB], lB, CRGB::Blue);
      }

      if ((sC + lC) >= NUM_LEDS) {
        eC = (sC + lC) - NUM_LEDS;
        fill_solid(&leds[0], eC, CRGB::Green);
        fill_solid(&leds[sC], (NUM_LEDS - sC), CRGB::Green);
      } else {
        eC = sC + lC;
        fill_solid(&leds[sC], lC, CRGB::Green);
      }
    }
    if ((millis() - tShow) > (1000 / FPS)) {
      tShow = millis();
      FastLED.show();
    }
  }

  if (game == 2) {
    // START NEW BEAMS
    if (kA) {
      kA = false;
      tLap[0] = 0;
      leds[0][0] = 255;
    }
    if (kB) {
      kB = false;
      tLap[1] = 0;
      leds[0][1] = 255;
    }
    if (kC) {
      kC = false;
      tLap[2] = 0;
      leds[0][2] = 255;
    }

    // CORE BLUE
    if ((millis() - tFPS_BLUE) > (1000 / FPS_BLUE)) {
      tFPS_BLUE = millis();

      for (int i = NUM_LEDS; i > 0; i--) {

        if (leds[i - 1][2] > 0) {

          if (leds[i - 1][2] == 255) {
            if (i != NUM_LEDS) {
              leds[i][2] = 255;
            } else {
              if (tLap[0] < nLap-1) {
                leds[0][2] = 255;
                tLap[0]++;
              }
            }
          }

          leds[i - 1][2] = scale8(leds[i - 1][2], 150);
        }
      }
    }

    // CORE GREEN
    if ((millis() - tFPS_GREEN) > (1000 / FPS_GREEN)) {
      tFPS_GREEN = millis();

      for (int i = NUM_LEDS; i > 0; i--) {

        if (leds[i - 1][1] > 0) {

          if (leds[i - 1][1] == 255) {
            if (i != NUM_LEDS) {
              leds[i][1] = 255;
            } else {
              if (tLap[1] < nLap-1) {
                leds[0][1] = 255;
                tLap[1]++;
              }
            }
          }

          leds[i - 1][1] = scale8(leds[i - 1][1], 150);
        }
      }
    }

    // CORE RED
    if ((millis() - tFPS_RED) > (1000 / FPS_RED)) {
      tFPS_RED = millis();

      for (int i = NUM_LEDS; i > 0; i--) {

        if (leds[i - 1][0] > 0) {

          if (leds[i - 1][0] == 255) {
            if (i != NUM_LEDS) {
              leds[i][0] = 255;
            } else {
              if (tLap[2] < nLap-1) {
                leds[0][0] = 255;
                tLap[2]++;
              }
            }
          }

          leds[i - 1][0] = scale8(leds[i - 1][0], 150);
        }
      }

      FastLED.show();
    }
  }
}

void displays() {
  if (nA != nA_old) {
    displayA.showNumberDec(nA, false);
    nA_old = nA;
  }

  if (nB != nB_old) {
    displayB.showNumberDec(nB, false);
    nB_old = nB;
  }

  if (nC != nC_old) {
    displayC.showNumberDec(nC, false);
    nC_old = nC;
  }
}

void readPot() {
  if ((millis() - tRead) > 35) {
    tRead = millis();
    pot_temp = analogRead(A0) + 1;

    for (uint8_t i = 19; i > 0; i--) {
      tempAVG[i] = tempAVG[i - 1];
    }
    tempAVG[0] = pot_temp;
    pot = 0;
    for (uint8_t i = 0; i < 20; i++) {
      pot += tempAVG[i];
    }
    pot = pot / 20;
  }
}

void readButtons() {
  // read button A
  if (!fA) {
    if ((millis() - tA) > DEBOUNCE) {
      if (!digitalRead(btnA)) {
        fA = true;
        kA = true;
        tA = millis();
        nA++;
        lA++;
      }
    }
  } else {
    if ((millis() - tA) > DEBOUNCE) {
      if (digitalRead(btnA)) {
        fA = false;
        tA = millis();
      }
    }
  }

  // read button B
  if (!fB) {
    if ((millis() - tB) > DEBOUNCE) {
      if (!digitalRead(btnB)) {
        fB = true;
        kB = true;
        tB = millis();
        nB++;
        lB++;
      }
    }
  } else {
    if ((millis() - tB) > DEBOUNCE) {
      if (digitalRead(btnB)) {
        fB = false;
        tB = millis();
      }
    }
  }

  // read button C
  if (!fC) {
    if ((millis() - tC) > DEBOUNCE) {
      if (!digitalRead(btnC)) {
        fC = true;
        kC = true;
        tC = millis();
        nC++;
        lC++;
      }
    }
  } else {
    if ((millis() - tC) > DEBOUNCE) {
      if (digitalRead(btnC)) {
        fC = false;
        tC = millis();
      }
    }
  }
}
