#define PUNTI_PARTENZA 0
#define PUNTI_VALORE 2
#define TEMPO_PARTENZA 10

#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);

void setup() {
  // put your setup code here, to run once:

  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  delay(200);

  Serial.begin(9600);
  pinMode(6, INPUT_PULLUP);
  delay(200);
}

bool fS = false;
unsigned long dbS = 0;
int punti = PUNTI_PARTENZA;
int oldPunti = PUNTI_PARTENZA + 1;
long tTime = -1000;
int contaTempo = TEMPO_PARTENZA;
bool fine = false;
unsigned long tEnd = 0;


void loop() {

  // leggi pulsante
  if (!fS) {
    if ((millis() - dbS) > 200) {
      if (!digitalRead(6)) {
        fS = true;
        punti = punti + PUNTI_VALORE;
        dbS = millis();
      }
    }
  } else {
    if ((millis() - dbS) > 200) {
      if (digitalRead(6)) {
        fS = false;
        dbS = millis();
      }
    }
  }

  // segna i punti se sono cambiati
  if (punti != oldPunti) {
    lc.spiTransfer(0, 0, 0);
    lc.spiTransfer(0, 1, 0);
    lc.spiTransfer(0, 2, 0);
    segna(punti);
    oldPunti = punti;
  }

  // countdown
  if ((millis() - tTime) > 1000) {
    tTime = millis();
    if (contaTempo >= 0) {
      tempo(contaTempo);
      contaTempo--;
    } else {
      fine = true;
      tEnd = millis();
    }
  }

  if (fine) {
    // aspetta 5 secondi
    while ((millis() - tEnd) < 5000) {
      //lampeggia
      lc.setIntensity(0, 0);
      delay(250);
      lc.setIntensity(0, 15);
      delay(250);
    }
    delay(2000);
    fine = false;
    punti = PUNTI_PARTENZA;
    oldPunti = PUNTI_PARTENZA + 1;
    contaTempo = TEMPO_PARTENZA;
  }
}

void tempo(int p) {
  if (p > 999) {
    p = 999;
  }
  if (p < 0) {
    p = 0;
  }

  if (p > 99) {
    int p1 = p % 100;
    int p2 = p1 % 10;

    int c1 = (p - p1) / 100;
    int c2 = (p1 - p2) / 10;
    int c3 = p2;
    lc.spiTransfer(0, 7, 0);
    lc.spiTransfer(0, 6, 0);
    lc.spiTransfer(0, 5, 0);
    lc.spiTransfer(0, 4, 0);
    lc.setDigit(0, 6, c1, false);
    lc.setDigit(0, 5, c2, false);
    lc.setDigit(0, 4, c3, false);

  } else if (p > 9) {
    int p1 = p % 10;
    int p2 = (p - p1) / 10;

    lc.spiTransfer(0, 7, 0);
    lc.spiTransfer(0, 6, 0);
    lc.spiTransfer(0, 5, 0);
    lc.spiTransfer(0, 4, 0);
    lc.setDigit(0, 5, p2, false);
    lc.setDigit(0, 4, p1, false);

  } else {
    lc.spiTransfer(0, 6, 0);
    lc.spiTransfer(0, 5, 0);
    lc.spiTransfer(0, 4, 0);
    lc.setDigit(0, 4, p, false);
  }
}

void segna(int p) {
  if (p > 99) {
    p = 99;
  }
  if (p < 0) {
    p = 0;
  }

  if (p > 9) {
    int p1 = p % 10;
    int p2 = (p - p1) / 10;

    lc.setDigit(0, 1, p2, false);
    lc.setDigit(0, 0, p1, false);

  } else {
    lc.setDigit(0, 0, p, false);
  }
}
