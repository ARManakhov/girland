//
// Created by sirosh on 28.08.2021.
//
#include <Arduino.h>
#include "espUtils.h"

void restartNow() {
    Serial.println("Restart");
    ESP.restart();
}