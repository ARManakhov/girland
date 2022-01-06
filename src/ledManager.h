//
// Created by sirosh on 28.08.2021.
//

#ifndef GIRLAND_PALETTE_H
#define GIRLAND_PALETTE_H

#include <Arduino.h>
#include <NeoPixelBus.h>
#include "settings.h"
#include "configManager.h"

void ledShow();

void updateConfig(JsonObject newLedConfig);

LedConfig * getLedConfig();

void initLed();

#endif
