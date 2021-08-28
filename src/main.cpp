//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <settings.h>
#include <LittleFS.h>
#include "routing.h"
#include "wifi.h"
#include "palette.h"

boolean b1ps = 1;
boolean b2ps = 1;
boolean b3ps = 1;
boolean b4ps = 1;

unsigned long b1pt = 0;
unsigned long b2pt = 0;
unsigned long b3pt = 0;
unsigned long b4pt = 0;
int idelta = 25;

void handleButton() {
    unsigned long curentTime = millis();
    if (b1ps != digitalRead(b1gpio) && b1pt + idelta < curentTime) {
        b1ps = digitalRead(b1gpio);
        if (!digitalRead(b1gpio)) {
            b1pt = curentTime;
            if (brightness != 250)
                brightness = brightness + 10;
            FastLED.setBrightness(brightness);
        }
    }

    if (b2ps != digitalRead(b2gpio) && b2pt + idelta < curentTime) {
        b2ps = digitalRead(b2gpio);
        if (!digitalRead(b2gpio)) {
            b2pt = curentTime;
            if (brightness != 0)
                brightness = brightness - 10;
            FastLED.setBrightness(brightness);
        }
    }

    if (b3ps != digitalRead(b3gpio) && b3pt + idelta < curentTime) {
        b3ps = digitalRead(b3gpio);
        if (!digitalRead(b3gpio)) {
            b3pt = curentTime;
            if (mode != 15)
                mode++;
            setPallete(mode);
        }
    }

    if (b4ps != digitalRead(b4gpio) && b4pt + idelta < curentTime) {
        b4ps = digitalRead(b4gpio);
        if (!digitalRead(b4gpio)) {
            b4pt = curentTime;
            if (mode != 0)
                mode--;
            setPallete(mode);
        }
    }
}

void loop() {
}

void setup() {


    Serial.begin(115200);

    if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }


    if (LittleFS.begin()) {
        Serial.println("LittleFS Active");
        Serial.println();
    } else {
        Serial.println("Unable to activate LittleFS");
    }


    initWifi();
    initFastLed();

    setPallete(mode);

    serverInit();


    pinMode(b1gpio, INPUT_PULLUP);
    pinMode(b2gpio, INPUT_PULLUP);
    pinMode(b3gpio, INPUT_PULLUP);
    pinMode(b4gpio, INPUT_PULLUP);

    while (true) {
//        if (needRestartNow) {
//            restartNow();
//        }
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */

        FillLEDsFromPaletteColors(startIndex);
        handleButton();
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
    }
}
