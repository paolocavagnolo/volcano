#define PUNTI_PARTENZA 0
#define PUNTI_VALORE 1
#define TEMPO_PARTENZA 30

/*************************************************
   Public Constants
 *************************************************/

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 1);

void song();

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


void setup() {
  // put your setup code here, to run once:

  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  delay(200);

  Serial.begin(9600);
  pinMode(6, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
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

bool fB = false;
unsigned long dbB = 0;

bool p_song = true;


void loop() {

  // leggi pulsante
  if (!fB) {
    if ((millis() - dbB) > 200) {
      if (!digitalRead(13)) {
        fB = true;
        fine = true;
        tEnd = millis();
        dbB = millis();
      }
    }
  } else {
    if ((millis() - dbB) > 200) {
      if (digitalRead(13)) {
        fB = false;
        dbB = millis();
      }
    }
  }

  // leggi levetta
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
      if (p_song) {
        p_song = false;
        song();
      }
    }
  }

  if (fine) {
    // aspetta 5 secondi
    while ((millis() - tEnd) < 2500) {
      //lampeggia
      lc.setIntensity(0, 0);
      delay(250);
      lc.setIntensity(0, 15);
      delay(250);
    }
    fine = false;
    p_song = true;
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

void song() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
