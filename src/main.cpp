#include <Arduino.h>
#include <LittleFS.h>
#include "routing.h"
#include "wifi.h"
#include "ledManager.h"
#include "buttonsManager.h"

void loop() {
    ledShow();
    delay(1000 / UPDATES_PER_SECOND);
}


void setup() {
    randomSeed(analogRead(0));
    Serial.begin(115200);
    if (LittleFS.begin()) {
        Serial.println("LittleFS Active");
        Serial.println();
    } else {
        Serial.println("Unable to activate LittleFS");
    }
    initWifi();
    Serial.println("wifi Active");
    initLed();
    Serial.println("led Active");
    serverInit();
    buttonInit();
}
