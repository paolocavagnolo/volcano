#define FILE_VISITORS_NAME "test3.txt"

#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;
#include <FastLED.h>

#define DATA_PIN 7
#define NUM_LEDS 71
#define MAX_POWER_MILLIAMPS 500
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#include "arduino_imgs.h"

int n_people = 0;
File myFile;
bool connected = false;

void setup() {

  Serial.begin(9600);

  carrier.noCase();
  carrier.begin();
  carrier.Buzzer.noSound();

  fileRead();

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);
  FastLED.clear();
  FastLED.show();
}

uint8_t btnTcd = 5;
bool btnState[] = { 0, 0, 0, 0, 0 };
uint8_t old_btnTcd = 5;
unsigned long tLeds = 0;
int btnOn = -1;
int old_btnOn = -1;
unsigned long tBtn = 0;

bool firstLogoOff = true;
bool firstLogoOn = true;

float temp;
float hum;
float press;
float gasResistor;
float volatileOrganicCompounds;
float co2;

unsigned long tSens = 0;
unsigned long tDispSens = 0;
bool firstOn = false;
bool firstOff = false;

bool fBlack = false;
unsigned long tBlack = 0;

bool changeOn = false;
bool changeOff = false;

bool readSens = false;
int sens_n = 0;

bool readPir = true;

bool presence = false;
unsigned long pirTime = 0;
unsigned long pauseTime = 0;

unsigned long tUpdateSlow = 0;
unsigned long tUpdateFast = 0;

bool heat = false;

bool cooling_on = false;
bool heating_on = false;

void loop() {

  carrier.Buttons.update();

  if (carrier.Buttons.getTouch(TOUCH0)) {
    btnTcd = 0;
  } else if (carrier.Buttons.getTouch(TOUCH1)) {
    btnTcd = 1;
  } else if (carrier.Buttons.getTouch(TOUCH2)) {
    btnTcd = 2;
  } else if (carrier.Buttons.getTouch(TOUCH3)) {
    btnTcd = 3;
  } else if (carrier.Buttons.getTouch(TOUCH4)) {
    btnTcd = 4;
  } else {
    btnTcd = 5;
  }

  if (btnTcd != old_btnTcd) {
    if (btnOn < 0) {
      if (btnTcd < 5) {
        btnOn = btnTcd;
        btnState[btnOn] = true;
        firstOn = true;
      }
    } else {
      if (btnTcd < 5) {
        btnState[btnOn] = false;
        firstOff = true;
        old_btnOn = btnOn;
        btnOn = -1;
      }
    }
    old_btnTcd = btnTcd;
  }

  if (btnOn >= 0) {
    if (firstOn) {
      firstOn = false;
      changeOn = true;
      Serial.print(btnOn);
      Serial.println(" ON");
    }
  }

  if (btnOn < 0) {
    if (firstOff) {
      firstOff = false;
      changeOff = true;
      Serial.print(old_btnOn);
      Serial.println(" OFF");
    }
  }

  if (changeOn) {
    changeOn = false;
    fBlack = false;

    // LEDS ON
    carrier.leds.clear();
    carrier.leds.setPixelColor(btnOn, carrier.leds.ColorHSV(random(65025), 255, 50));
    carrier.leds.show();

    // RELAY-1 ON
    if (btnOn == 3) {
      carrier.Relay1.open();
      if (heat) {
        heating_on = true;
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_RED);
        carrier.display.drawBitmap(70, 50, r_open, 100, 37, ST77XX_RED);
        carrier.display.setCursor(60, 100);
        carrier.display.setTextColor(ST77XX_RED);
        carrier.display.setTextSize(3);
        carrier.display.print("1 OPEN");
        carrier.display.setCursor(30, 150);
        carrier.display.print("HEATING ON");
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
      } else {
        cooling_on = true;
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_BLUE);
        carrier.display.drawBitmap(70, 50, r_open, 100, 37, ST77XX_BLUE);
        carrier.display.setCursor(60, 100);
        carrier.display.setTextColor(ST77XX_BLUE);
        carrier.display.setTextSize(3);
        carrier.display.print("1 OPEN");
        carrier.display.setCursor(30, 150);
        carrier.display.print("COOLING ON");
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
      }


      tBlack = millis();
      fBlack = true;
    }

    // RELAY-2 ON
    if (btnOn == 4) {
      carrier.Relay2.open();

      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_WHITE);
      carrier.display.drawBitmap(70, 50, r_open, 100, 37, ST77XX_WHITE);
      carrier.display.setCursor(60, 100);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(3);
      carrier.display.print("2 OPEN");
      carrier.display.setCursor(40, 150);
      carrier.display.print("LIGHT ON");


      tBlack = millis();
      fBlack = true;
    }

    // DISPLAY ON
    if (btnOn == 2) {
      heat = !heat;
      if (heat) {
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_RED);
        carrier.display.setCursor(70, 80);
        carrier.display.setTextColor(ST77XX_RED);
        carrier.display.setTextSize(3);
        carrier.display.print("MODE:");
        carrier.display.setCursor(50, 120);
        carrier.display.print("HEATING");
      } else {
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_BLUE);
        carrier.display.setCursor(70, 80);
        carrier.display.setTextColor(ST77XX_BLUE);
        carrier.display.setTextSize(3);
        carrier.display.print("MODE:");
        carrier.display.setCursor(50, 120);
        carrier.display.print("COOLING");
      }

      tBlack = millis();
      fBlack = true;
    }

    // SENSORS ON
    if (btnOn == 0) {
      sens_n = 0;
      readSens = true;
    }

    // PIR ON
    if (btnOn == 1) {
      readPir = false;
      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_MAGENTA);
      carrier.display.drawBitmap(70, 50, crowd, 103, 110, ST77XX_MAGENTA);
      fileRead();
      carrier.display.setCursor(100, 180);
      carrier.display.setTextColor(ST77XX_MAGENTA);
      carrier.display.setTextSize(3);
      carrier.display.print(n_people);
    }
  }

  if (changeOff) {
    changeOff = false;

    // LEDS OFF
    carrier.leds.clear();
    carrier.leds.show();

    // RELAY-1 OFF
    if (old_btnOn == 3) {
      carrier.Relay1.close();

      if (heat) {
        heating_on = false;
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_RED);
        carrier.display.drawBitmap(70, 50, r_close, 100, 37, ST77XX_RED);
        carrier.display.setCursor(60, 100);
        carrier.display.setTextColor(ST77XX_RED);
        carrier.display.setTextSize(3);
        carrier.display.print("1 CLOSE");
        carrier.display.setCursor(23, 150);
        carrier.display.print("HEATING OFF");

      } else {
        cooling_on = false;
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_BLUE);
        carrier.display.drawBitmap(70, 50, r_close, 100, 37, ST77XX_BLUE);
        carrier.display.setCursor(60, 100);
        carrier.display.setTextColor(ST77XX_BLUE);
        carrier.display.setTextSize(3);
        carrier.display.print("1 CLOSE");
        carrier.display.setCursor(23, 150);
        carrier.display.print("COOLING OFF");
      }
      FastLED.clear();
      FastLED.show();


      tBlack = millis();
      fBlack = true;
    }

    // RELAY-2 OFF
    if (old_btnOn == 4) {
      carrier.Relay2.close();

      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_WHITE);
      carrier.display.drawBitmap(70, 50, r_close, 100, 37, ST77XX_WHITE);
      carrier.display.setCursor(60, 100);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(3);
      carrier.display.print("2 CLOSE");
      carrier.display.setCursor(40, 150);
      carrier.display.print("LIGHT OFF");

      tBlack = millis();
      fBlack = true;
    }

    // DISPLAY OFF
    if (old_btnOn == 2) {
      tBlack = millis();
      fBlack = true;
    }

    // SENSORS OFF
    if (old_btnOn == 0) {
      readSens = false;
      tBlack = millis();
      fBlack = true;
    }

    // PIR OFF
    if (old_btnOn == 1) {
      readPir = true;
      tBlack = millis();
      fBlack = true;
    }
  }

  if (fBlack) {
    if ((millis() - tBlack) > 1200) {
      fBlack = false;
      carrier.display.fillScreen(ST77XX_BLACK);
    }
  }

  if (readSens) {
    if ((millis() - tSens) > 3000) {
      tSens = millis();
      if (sens_n == 0) {
        carrier.display.fillScreen(ST77XX_BLACK);  //oled clear()
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_RED);
        carrier.display.drawBitmap(72, 20, temp_icon, 100, 148, ST77XX_RED);
        temp = carrier.Env.readTemperature() - 4.1;
        carrier.display.setCursor(60, 170);
        carrier.display.setTextColor(ST77XX_RED);
        carrier.display.setTextSize(3);
        carrier.display.print(temp);
        carrier.display.print(" C");
      }
      if (sens_n == 1) {
        carrier.display.fillScreen(ST77XX_BLACK);  //oled clear()
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_BLUE);
        carrier.display.drawBitmap(72, 10, humi_icon, 100, 148, ST77XX_BLUE);
        hum = carrier.Env.readHumidity();
        carrier.display.setCursor(60, 170);
        carrier.display.setTextColor(ST77XX_BLUE);
        carrier.display.setTextSize(3);
        carrier.display.print(hum);
        carrier.display.print(" %");
      }
      if (sens_n == 2) {
        carrier.display.fillScreen(ST77XX_BLACK);  //oled clear()
        carrier.display.drawBitmap(0, 0, c_frame, 240, 240, ST77XX_GREEN);
        carrier.display.drawBitmap(78, 25, plant_water, 86, 140, ST77XX_GREEN);
        press = analogRead(A0);
        carrier.display.setTextColor(ST77XX_GREEN);
        carrier.display.setTextSize(3);
        if (press < 300) {
          carrier.display.setCursor(60, 170);
          carrier.display.print("very wet");
        } else if (press < 400) {
          carrier.display.setCursor(95, 170);
          carrier.display.print("wet");
        } else if (press < 600) {
          carrier.display.setCursor(90, 170);
          carrier.display.print("moist");
        } else if (press < 800) {
          carrier.display.setCursor(95, 170);
          carrier.display.print("dry");
        } else {
          carrier.display.setCursor(60, 170);
          carrier.display.print("very dry");
        }
      }
      if (sens_n >= 2) {
        sens_n = 0;
      } else {
        sens_n++;
      }
    }
  }

  if (readPir) {
    if (!presence) {
      if ((millis() - pauseTime) > 5000) {
        if (analogRead(A6) > 1000) {
          pirTime = millis();
          presence = true;
        }
      }
    }
  }

  if (presence) {
    if ((millis() - pirTime) > 10000) {
      if (analogRead(A6) < 100) {
        fileWrite();
        presence = false;
        pauseTime = millis();
      }
    }
  }


}  //END LOOP


void drawArduino(uint16_t color) {
  carrier.display.drawBitmap(44, 60, ArduinoLogo, 152, 72, color);
  carrier.display.drawBitmap(48, 145, ArduinoText, 144, 23, color);
}

void drawExplore(uint16_t color) {
  carrier.display.drawBitmap(0, 60, ExploreFrame, 240, 75, color);
  carrier.display.drawBitmap(0, 60, ExplreIoTKittext, 240, 75, color);
}

void fileWrite() {

  myFile = SD.open(FILE_VISITORS_NAME, FILE_WRITE);

  if (myFile) {
    myFile.write("a");
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void fileRead() {
  int n;
  String s = "";
  myFile = SD.open(FILE_VISITORS_NAME);

  if (myFile) {
    while (myFile.available()) {
      s += myFile.read();
    }

    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }

  n_people = s.length() / 2;
}
